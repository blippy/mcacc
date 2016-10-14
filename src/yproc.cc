#include <algorithm>
#include <ctime>
#include <fstream>
#include <set>
#include <string>

#include "common.hpp"
#include "reusable.hpp"
#include "yproc.hpp"

using namespace std;


typedef struct downloads_t {
	string dstamp;
	string tstamp;
	string usd;
	set<yahoo_t> ys;
} downloads_t;


void download(const comm_ts& the_comms, downloads_t& ds)
{
	std::time_t t = std::time(nullptr);
	char dstamp[80], tstamp[80];
	strftime(dstamp, 80, "%Y-%m-%d", std::localtime(&t));
	ds.dstamp = dstamp;
	strftime(tstamp, 80, "%H:%M:%S", std::localtime(&t));
	ds.tstamp = tstamp;
	
	strings tickers;
	for(auto& cm:the_comms) {
		auto &c = cm.second;
		if(c.down == "W") tickers.push_back(c.ticker);
	}

	strings retrs = fetch_tickers(tickers, ds.usd);
	for(auto& line: retrs) {
		strings fields = split(line, ',');
		yahoo_t y;
		y.dstamp = dstamp;
		y.tstamp = tstamp;
		y.ticker = fields[0]; 
		erase_all(y.ticker, '"');
		y.yprice.set(fields[1]);
		y.chg.set(fields[2]);
		y.chgpc = y.chg.get() / (y.yprice.get() - y.chg.get()) * 100;
		ds.ys.insert(y);
	}
}
 
/* Create a yahoo cache file */
void mkyahoos(downloads_t& ds)
{
	string yout_name = rootdir() + "/yahoo/" + ds.dstamp + ".txt";
	ofstream yout;
	yout.open(yout_name);
	for(auto& y: ds.ys){
		string chgpc = format_num(y.chgpc, 2);
		strings fields = {"yahoo", ds.dstamp, ds.tstamp, 
			y.ticker, "1.0000", y.yprice.str6(), 
			y.chg.str2(), chgpc, "P"};
		yout << intercalate("\t", fields);
		yout << endl;
	}
	yout.close();

}





void mksnap(const inputs_t& inps, const downloads_t& ds)
{
	string fname;
	s3("snap.rep", fname); 
	ofstream sout;
	sout.open(fname, ofstream::out);

	strings fields = {pad_right("EPIC", 6), pad_left("PROFIT", 12), 
		ret_str("CHG%"), pad_left("VALUE", 12), pad_left("QTY", 12), 
		pad_left("PRICE", 12)};
	sout << intercalate(" ", fields) << endl;
	
	centis total_profit, total_value;

	bool total_written = false;
	for(auto& y:ds.ys) {
		quantity qty;
		bool is_index = y.ticker[0] == '^';
		for(auto& e:inps.etrans) 
			if(y.ticker == e.ticker) 
				qty += e.qty;

		centis profit;
		if(is_index) {
			profit.set(y.chg.get() * 100);
		} else {
			recentis(profit, y.chg, qty);
		}
		total_profit += profit;
		
		string chgpc_str =ret_str(y.chgpc);
		string price_str = y.yprice.str6();
		centis value;
	       	recentis(value, y.yprice, qty);
		total_value += value;
		string value_str = value.str();
		strings fields = strings {pad_right(y.ticker, 6), 
			profit.str(), chgpc_str, value_str, 
			qty.str(), price_str};

		if(is_index && ! total_written) {
			string chgpc_str = retchg_str(total_profit, total_value);
			strings fields = {pad_right("TOTAL", 6), 
				total_profit.str(), 
				chgpc_str, total_value.str()};
			sout << intercalate(" ", fields) << endl << endl;
			total_written = true;
		}

		sout << intercalate(" ", fields) << endl;

	}
	sout << endl << intercalate(" ", strings {"DSTAMP:", ds.dstamp, ds.tstamp}) << endl;
	sout.close();
}

yahoo_ts process_yahoos(const inputs_t& inps)
{
	//cout << "process_yahoos() performing download\n";
	downloads_t ds;
	download(inps.comms, ds);
	mkyahoos(ds);
	mksnap(inps, ds);
	return ds.ys;
}

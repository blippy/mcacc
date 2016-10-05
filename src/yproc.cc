#include <algorithm>
#include <ctime>
#include <fstream>
#include <set>
#include <sstream>
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
		y.price = stod(fields[1]);
		y.chg = stod(fields[2]);
		y.chgpc = y.chg / (y.price - y.chg) * 100;
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
		//yahoo_t y2 = y.second;
		string price = format_num(y.price, 6);
		string chg = format_num(y.chg, 6);
		string chgpc = format_num(y.chgpc, 2);
		strings fields = {"yahoo", ds.dstamp, ds.tstamp, y.ticker, "1.0000", price, chg, chgpc, "P"};
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

	strings fields = {pad_right("EPIC", 6), pad_left("PROFIT", 12), pad_left("CHG%", 6), 
		pad_left("VALUE", 12), pad_left("QTY", 6), pad_left("PRICE", 8)};
	sout << intercalate(" ", fields) << endl;
	
	pennies_t total_profit = 0, total_value = 0;

	bool total_written = false;
	for(auto& y:ds.ys) {
		quantity qty;
		bool is_index = y.ticker[0] == '^';
		for(auto& e:inps.etrans) 
			if(y.ticker == e.ticker) 
				qty.inc(e.qty);
		//string qty_str = pad_left(format_num(qty , 0), 6);
		pennies_t profit = is_index? y.chg : y.chg *qty.get()/100;
		total_profit += profit;
		string chgpc_str = pad_left(format_num(y.chgpc, 2), 6);
		string price_str = pad_left(format_num(y.price, 2), 8);
		pennies_t value = y.price * qty.get()/100;
		total_value += value;
		string value_str = to_gbx(value);
		strings fields = strings {pad_right(y.ticker, 6), 
			to_gbx(profit), chgpc_str, value_str, 
			qty.str(), price_str};

		if(is_index && ! total_written) {
			double chgpc = total_profit/total_value * 100;
			string chgpc_str = pad_left(format_num(chgpc, 2), 6);
			strings fields = {pad_right("TOTAL", 6), to_gbx(total_profit), 
				chgpc_str, to_gbx(total_value)};
			sout << intercalate(" ", fields) << endl << endl;
			total_written = true;
		}

		sout << intercalate(" ", fields) << endl;

	}
	sout << endl << intercalate(" ", strings {"DSTAMP:", ds.dstamp, ds.tstamp}) << endl;
	sout.close();
}
void process_yahoos(inputs_t& inps, bool with_fetch)
{
	if(!with_fetch) return;
	downloads_t ds;
	download(inps.comms, ds);
	mkyahoos(ds);
	for(auto& y:ds.ys) insert_yahoo(y, inps);
	mksnap(inps, ds);
}

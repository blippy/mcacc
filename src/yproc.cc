#include <algorithm>
#include <ctime>
#include <fstream>
#include <set>
#include <sstream>
#include <string>

//#include <boost/algorithm/string.hpp>

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

//bool yorder (vector<string> a, vector<string> b) { return a[0] < b[0];}

/*
bool yorder(yahoo_t a, yahoo_t b)
{
	return tie(a.ticker, a.dstamp, a.tstamp) < tie(b.ticker, b.dstamp, b.tstamp);
}
*/

// TODO reusable
// http://stackoverflow.com/questions/236129/split-a-string-in-c
void split(const string &s, char delim, vector<string> &elems) {
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


// TODO reusable
vector<string> split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}


// TODO reusable
/* remove all charachters in-place */ 
void erase_all(string& str, const char c)
{
	str.erase(std::remove(str.begin(), str.end(), c), str.end());
}

//void download(inputs_t& inputs, downloads_t& ds)
void download(const comm_ts& the_comms, downloads_t& ds)
{
	//const comm_ts& the_comms = inputs.comms;
	//load(the_comms);

	std::time_t t = std::time(nullptr);
	char dstamp[80], tstamp[80];
	strftime(dstamp, 80, "%Y-%m-%d", std::localtime(&t));
	ds.dstamp = dstamp;
	//s2("dstamp", fname);
	//spit(fname, dstamp);
	strftime(tstamp, 80, "%H:%M:%S", std::localtime(&t));
	//s2("tstamp", fname);
	//spit(fname, tstamp);
	ds.tstamp = tstamp;
	
	strings tickers;
	for(auto& cm:the_comms) {
		auto &c = cm.second;
		if(c.down == "W") tickers.push_back(c.ticker);
	}

	//string fname;
	//string usd;
	strings retrs = fetch_tickers(tickers, ds.usd);
	//ds.usd = usd;
	//s2("usd.csv", fname);
	//spit(fname, usd);
	//s3("yproc-down.csv", fname);
	//spit(fname, intercalate("", retrs));

	for(auto& line: retrs) {
		//strings fields;
		//boost::split(fields, line, boost::is_any_of(","), boost::token_compress_on);
		strings fields = split(line, ',');
		yahoo_t y;
		y.dstamp = dstamp;
		y.tstamp = tstamp;
		y.ticker = fields[0]; 
		//boost::erase_all(y.ticker, "\"");
		erase_all(y.ticker, '"');
		y.price = stod(fields[1]);
		y.chg = stod(fields[2]);
		y.chgpc = y.chg / (y.price - y.chg) * 100;
		//insert_yahoo(y, ds.ys);
		ds.ys.insert(y);
	}
	//sort(ds.ys.begin(), ds.ys.end());
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
		double qty = 0;
		bool is_index = y.ticker[0] == '^';
		for(auto& e:inps.etrans) if(y.ticker == e.ticker) qty += e.qty;
		string qty_str = pad_left(format_num(qty , 0), 6);
		pennies_t profit = is_index? y.chg : y.chg *qty/100;
		total_profit += profit;
		string chgpc_str = pad_left(format_num(y.chgpc, 2), 6);
		string price_str = pad_left(format_num(y.price, 2), 8);
		pennies_t value = y.price * qty/100;
		total_value += value;
		string value_str = to_gbx(value);
		strings fields = strings {pad_right(y.ticker, 6), to_gbx(profit), chgpc_str, value_str, 
			qty_str, price_str};

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

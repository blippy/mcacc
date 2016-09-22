#include <ctime>
#include <fstream>
#include <set>
#include <string>

#include <boost/algorithm/string.hpp>

#include "common.hpp"
#include "reusable.hpp"
#include "yproc.hpp"

using namespace std;

void download(inputs_t& inputs)
{
	const comm_ts& the_comms = inputs.comms;
	//load(the_comms);

	strings tickers;
	for(auto& cm:the_comms) {
		auto &c = cm.second;
		if(c.down == "W") tickers.push_back(c.ticker);
	}

	string fname;
	string usd;
	strings retrs = fetch_tickers(tickers, usd);
	s2("usd.csv", fname);
	spit(fname, usd);
	s2("yfetch.csv", fname);
	spit(fname, intercalate("", retrs));



	std::time_t t = std::time(nullptr);
	char dstamp[80], tstamp[80];
	strftime(dstamp, 80, "%Y-%m-%d", std::localtime(&t));
	s2("dstamp", fname);
	spit(fname, dstamp);
	strftime(tstamp, 80, "%H:%M:%S", std::localtime(&t));
	s2("tstamp", fname);
	spit(fname, tstamp);

	// TODO LOW This is incorrect for commodities not downloaded in pennies
	for(auto& line: retrs) {
		strings fields;
		boost::split(fields, line, boost::is_any_of(","), boost::token_compress_on);
		yahoo_t y;
		y.dstamp = dstamp;
		y.tstamp = tstamp;
		y.ticker = fields[0]; 
		boost::erase_all(y.ticker, "\"");
		//cout << "tocker = " << y.ticker << endl;
		y.price = stod(fields[1]); //enpennies(fields[1]); //stod(fields[1])/100;
		y.chg = stod(fields[2]); // enpennies(fields[2]); //stod(fields[2])/100;
		y.chgpc = y.chg / (y.price - y.chg) * 100;
		insert_yahoo(y, inputs);

	}
}

bool yorder (vector<string> a, vector<string> b) { return a[0] < b[0];}



void yproc_main(const comm_ts & the_comms)
{	

/*
IN: s1/comm.dsv s2/dstamp s2/ystamp s2/yfetch.csv s2/usd.csv
OUT: s3/yrpoc.dsv ~/.mca/yahoo/<YYYY-MM-DD>.txt

NB Further processing will pick up the generated yahoo file from the cache
(in ~/.mca), but it is still useful to have s3/yproc.dsv for creating
a snapshot of gains
*/


	set<string> usds; // those comms which are in UDB
	for(auto &cm: the_comms) {
		auto & c = cm.second;
		if(c.unit == "USD") 
			usds.insert(c.ticker);
	}

	string fname;
	s2("usd.csv", fname); 
	if(! file_exists(fname)) 
		throw runtime_error("ERR-102 (fatal): File s2/usd.csv does not exist.");

	string file_contents = slurp(fname);
	strings usdv = commasepstr(file_contents);
	string rox_str;
	try { rox_str = usdv.at(1); } // TODO needs checking to see if it's pulling out right value
	catch (const out_of_range& ex) { throw runtime_error("Couldn't decompose <"s + file_contents + ">"); }
	double rox = 100.0 * stod(rox_str);
	//cout << rox;


	s2("dstamp", fname);
	string dstamp = slurp(fname);
	s2("tstamp", fname);
	string tstamp = slurp(fname);
	string dtstamp = dstamp + "\t" + tstamp;
	
	s2("yfetch.csv", fname);
	vecvec_t yfetch = commasep(fname);

	ofstream yout;
	s3("yproc.dsv", fname);
	yout.open(fname);
	sort(yfetch.begin(), yfetch.end(), yorder);
	for(auto y: yfetch) {
		yout << "yahoo " << dtstamp << "\t" ;
		string ticker = y[0];
		yout << ticker << "\t" ;
		string rox_str = "1.0000";
		double price = stod(y[1]);
		double chg   = stod(y[2]);
		if(usds.find(ticker) != usds.end()) {
			rox_str = to_string(rox);
			price *= rox;
			chg *= rox;
		}
		yout << rox_str << "\t" << to_string(price) << "\t" ;;
		yout << chg << "\t" ;

		// elminate the + and % from chgpc
		string chgpc ; // =y[3];
		char chars[] = {'+', '%' };
		for(auto c : y[3]) if(c != '+' && c!= '%') chgpc += c;
		yout << chgpc << "\tP" ;
	
		yout << endl;
	}
	yout.close();

	string yout_name = rootdir() + "/yahoo/" + dstamp + ".txt";
	s3("yproc.dsv", fname);
	string content = slurp(fname);
	spit(yout_name, content);

}

int yproc_main_nox(const comm_ts & the_comms) 
{
	try {
		yproc_main(the_comms);
	} catch (const runtime_error& ex) {
		cerr << "ERR-101 (Non-fatal): USD decoding: " << ex.what() << endl;
	}
	return EXIT_SUCCESS;
}
	
void process_yahoos(inputs_t& inps, bool with_fetch)
{
	if(with_fetch) download(inps);

	yproc_main_nox(inps.comms);
}

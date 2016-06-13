#include <assert.h>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <string>
//#include <tuple>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

#include "assets.hpp"
#include "common.h"
#include "parse.h"
#include "reusable.h"
#include "autotypes.h"
#include "comm.hpp"
#include "nacc.hpp"
#include "yahoo.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::function;
using std::string;
using std::vector;




bool operator<(const etrana& lhs, const etrana& rhs)
{
	return std::tie(lhs.sym, lhs.dstamp) < std::tie(rhs.sym, rhs.dstamp);
}

bool operator>(const etrana &a, const etrana &b)
{
	return std::tie(a.sym, a.dstamp) > std::tie(b.sym, b.dstamp);
}

etranas_t load_etranas()
{
	vecvec_t rows = read_registered_dsv(etransa);
	etranas_t res;
	for(vs_t &r: rows) { 
		etrana e;
		//e.from_vec(r);
		convert(r, e);
		res.push_back(e);
	}
	

	// cubie seems to have problems with sorting, so I'll write my own algo: insertion sort
	// https://en.wikipedia.org/wiki/Insertion_sort
	for(int i=0; i< res.size(); i++) {
		etrana x = res[i];
		int j = i-1;
		//while(j>=0 && etrana_gt(res[j],x)) {
		while(j>=0 && res[j] > x) {
			res[j+1] = res[j];
			j -= 1;
		}
		res[j+1] = x;
	}

	return res;
}



vecvec_t commasep(string  &filename)
{
        vecvec_t res;
        ifstream fin;
        string line;
        fin.open(filename.c_str(), ifstream::in);
        while(getline(fin, line)) {
		for(auto it = line.begin(); it != line.end(); ++it) {
			if(*it == ',') *it = ' '; }
                vector<string> fields = tokenize_line(line);
                if(fields.size() >0) res.push_back(fields);
        }
        return res;
}

void stage0()
{
	string htm = get_html();
	string fname = rootdir() + "/mcacc.htm";
	//cout << fname << "\n" << htm << "\n" ;
	spit(fname, htm.c_str());
}

void stage2()
{
	comm_ts the_comms;
	load(the_comms);

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
	char buffer[80];
	strftime(buffer, 80, "%Y-%m-%d", std::localtime(&t));
	s2("dstamp", fname);
	spit(fname, buffer);
	strftime(buffer, 80, "%H:%M:%S", std::localtime(&t));
	s2("tstamp", fname);
	spit(fname, buffer);

}

bool yorder (vector<string> a, vector<string> b) { return a[0] < b[0];}
int yproc_main(const comm_ts & the_comms) 
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
	if(! file_exists(fname)) {
		cout << "INFO: yproc_main(): " 
			<< "File s2/usd.csv does not exist. Quitting\n";
		return EXIT_SUCCESS;
	}
	vecvec_t usdv = commasep(fname);
	double rox = 100.0 * stod(usdv[0][1]);
	//cout << rox;


	s2("dstamp", fname);
	string dstamp = slurp(fname.c_str());
	//cout << "*" << dstamp << "*" << endl;
	s2("tstamp", fname);
	string tstamp = slurp(fname.c_str());
	//cout << "*" << tstamp << "*" << endl;
	string dtstamp = dstamp + "\t" + tstamp;
	
	s2("yfetch.csv", fname);
	vecvec_t yfetch = commasep(fname);

	//cout << yout_name << endl;
	ofstream yout;
	s3("yproc.dsv", fname);
	yout.open(fname);
	//prin_vecvec1(yfetch);
	sort(yfetch.begin(), yfetch.end(), yorder);
	//prin_vecvec1(yfetch);
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
	string content = slurp(fname.c_str()); // TODO write a verson of slurp that takes a string
	spit(yout_name, content);

	return EXIT_SUCCESS;
}

int dsv_extract()
{
	string input;
	while(getline(cin, input)) {
		vector<string> output = tokenize_line(input);
		if(output.size() == 0) continue;
		string fname = output[0];
		fname  += ".dsv";
		fstream fs;
		ios_base::openmode mode =  fstream::out | fstream::app;
		fs.open(fname.c_str(), mode); // user has to pre-delete the file
		for(int i = 1; i < output.size() -1; i++) { fs << output[i] << "\t"; }
		fs << output.back() << endl ;
		fs.close();
		//cout << "In: " << trimmed << endl;
	}
	return EXIT_SUCCESS;
}



void cgt(const etranas_t& es, const period &per)
{
	cout << "mcacc cgt" << endl;

	//string start_date, end_date;
	//get_period(start_date, end_date);
	
	std::set<string> tickers;
	for(auto e: es) {
		if(e.taxable && per.during(e.dstamp)  && ! e.buy) tickers.insert(e.ticker);
	}

	for(auto & t: tickers) {
		cout << t << endl; // TODO it's here that we need to do something with the hit tickers
	}
	cout << "TODO: cgt - pending something interesting to report" << endl;
}

void stage3a()
{
	comm_ts the_comms;
	load(the_comms);
	yproc_main(the_comms);

	period p = get_period();

	stend_main(p);
	eaug_main(p);

	// TODO this shoould be able to be passed in from eaug. Check ordering, though
	etranas_t es = load_etranas();
	nacc_ts the_naccs;
	load(the_naccs);
	posts_main(es, p, the_naccs);
	etb_main(the_naccs);
	epics_main(es);
	cgt(es, p);
}

void print_version()
{
	cout << "mcacc " << VERSION << '\n';
}

void print_rootdir()
{
	cout << rootdir() << "\n";
}

// command dispatch table
typedef struct dte { string cmd ; function<void()> fn ; } dte;
const auto ditab = vector<dte> {
	{"--root", print_rootdir},
	{"--version", print_version},
	{"dsv", dsv_extract},
	{"stage0", stage0},
	{"stage2", stage2},
	{"stage3a", stage3a}
};

void dispatch(string cmd)
{
	for(auto d : ditab) {
		if(d.cmd == cmd) { d.fn() ; return ; }
	}

	cerr << "Command not understood: " << cmd << endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{


	if(argc>1) {
		string cmd;
	       	cmd = argv[1];
		dispatch(cmd);

	} else {
		cout << "No command specified" << endl;
	}
	return EXIT_SUCCESS;
}

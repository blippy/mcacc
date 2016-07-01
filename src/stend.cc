/* calculate the start and end prices for yahoo
created 16-Feb-2016

OUT: s3/stend.dsv

*/

#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <iostream> // for ifstream
#include <unordered_map>
#include <assert.h>
#include <dirent.h>
#include <set>
#include <stdexcept>

#include "common.hpp"
#include "parse.hpp"
#include "types.hpp"
#include "reusable.hpp"
#include "stend.hpp"
//#endif

//namespace fs = std::filesystem;

using namespace std;

#define SEP '\t'
/*
void scan_cache(vecvec_t &yahoos)
{

	string ydir = rootdir() + "/yahoo";
	auto dirp = opendir(ydir.c_str());
	struct dirent *dp;
	set<string> filenames;
	while(dp = readdir(dirp)) {
		if(dp == NULL) break;
		string dname = dp->d_name;
		if(dname == "." || dname == "..") continue;
		int pos = dname.size() -4 ;
		if(pos <0) continue;
		filenames.insert(dname);
	}
	closedir(dirp);


	for(auto& f: filenames) {
		string full = ydir + "/" + f;
		vecvec_t vecs = vecvec( full);		
		for(auto &v : vecs) { v.erase(v.begin(), v.begin()+1); }
		yahoos.insert(yahoos.end(), vecs.begin(), vecs.end());
	}

}
*/


void scan_inputs(vecvec_t &yahoos)
{
	string fname;
	s1("yahoo.dsv", fname);
	vecvec_t vecs = vecvec(fname);
	yahoos.insert(yahoos.end(), vecs.begin(), vecs.end());


	s1("etran.dsv", fname);
	vecvec_t etrans = vecvec(fname);
	for(auto e: etrans) {
		if(e.size() != 7) {
			string msg("stend/scan_inputs() expecting exactly 7 arguments for an etran.");
			for(auto& f:e) msg += " <" + f + ">";
			//msg += "\n";
			throw std::runtime_error(msg);
		}


		string dstamp = e[1];
		string tstamp = "03:00:00";
		double qty  = stod(e[5]);
		double cost = stod(e[6]);
		double price = cost/qty *100;
		string pstr = format_num(price, 4);
		string ticker = e[4];
		vector<string> v = {dstamp, tstamp, ticker, "1.0000", pstr, "0", "0", "P"};
		yahoos.push_back(v);
	}
	
}

bool ysorter(vector<string> a, vector<string> b)
{
	return std::tie(a[0], a[1], a[2]) < std::tie(b[0], b[1], b[2]);
}

void reorder(vecvec_t &yahoos)
{
	// ticker belongs at the beginning
	for(auto &y :yahoos) {
		string ticker = y[2];
		y.erase( y.begin()+2, y.begin()+3);
		y.insert(y.begin(), ticker);
	}

	sort(yahoos.begin(), yahoos.end(), ysorter);
}
	
void output(vecvec_t tbl, const period& per)
{

	ofstream fout;
	string fname;
	s3("stend.dsv", fname);
	fout.open(fname);
	std::string line;

	string yr0  = "0000-00-00";
	string p0  = "0.00";
	string start_dstamp = yr0;
	string start_price  = p0;
	string end_dstamp = yr0;
	string end_price = p0;
	string cur_sym = "";

	for(int i =0 ; i< tbl.size(); i++) {
		if(i==0 || tbl[i-1][0] != tbl[i][0]) {
			//begin group
			cur_sym = tbl[i][0];
			start_dstamp = yr0;
			start_price = p0;
			end_dstamp = yr0;
			end_price  = p0;	
		}

		// normal
		string dstamp = tbl[i][1];
		string price  = tbl[i][4];

		PeriodTiming pt = per.when(dstamp);
		if(pt == perBefore) { start_dstamp = dstamp ; start_price = price;}
		if(pt != perAfter)  { end_dstamp = dstamp ; end_price = price;}

		// end of group
		if(i +1 == tbl.size() || tbl[i+1][0] != tbl[i][0]) {
			fout << cur_sym << SEP ;
			fout << start_dstamp << SEP << start_price << SEP;
			fout  << end_dstamp << SEP << end_price << endl ;
		}
		
	}

	fout.close();
}

stend_ts stend_main(const inputs_t& inputs, period& per)
{
	// TODO possibly doesn't pick up from s2
	stend_ts stends;
	cerr << "TODO stend_main()\n";
	vecvec_t yahoos;
	//scan_cache(yahoos);
	scan_inputs(yahoos);
	reorder(yahoos);
	output(yahoos, per);
	return stends;
}

#include <assert.h>
#include <iostream>
#include <utility>
#include <math.h>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "common.hpp"
#include "reusable.hpp"
#include "yahoo.hpp"
#include "wiegley.hpp"

using namespace std;
using namespace boost;

typedef pair<string, string> spair;

bool sorter(spair a, spair b)
{
	return a.first < b.first;
}

void wiegley(const inputs_t& inputs)
{
	vector<spair> trans;

	// LEDGER.DAT
	
	for(auto& e: inputs.etrans) {
		//char sgn = e.buy ? ' ' : '-';
		string t1 = (format("%1%\t*\tetran\n") % e.dstamp).str() ;
		assert(e.typ != unknown);
		string at = (e.typ == leak) ? "@" : "@@";
		string t2 = (format("\tEquity:%1%\t\"%2%\"\t%3%\t%4%\tGBP\t%5%\n") % 
				e.folio % e.ticker %  e.qty % at % to_gbp(labs(e.cost))).str();
		string t3 = (format("\t%1%\n\n") % e.folio).str();
		string t = t1 + t2 + t3;
		//cout << t;
		trans.push_back(make_pair(e.dstamp, t));
	}

	for(auto& n:inputs.ntrans) {
		string t1 = (format("%1%\t*\t%2%\n") % n.dstamp % n.desc).str();
		string t2 = (format("\t%1%\tGBP\t%2%\n") % n.dr % to_gbp(n.amount)).str();
		string t3 = (format("\t%1%\n\n") % n.cr).str();
		string t = t1 + t2 + t3;
		trans.push_back(make_pair(n.dstamp, t));
	}

	sort(begin(trans), end(trans), sorter);


	string dat = "N\tGBP\n\n";
	string fname = rootdir() + "/ledger.dat";
	for(auto& p: trans) {
		dat += p.second;
	}
	spit(fname, dat);

	trans.clear();
	for(auto& y:inputs.yahoos) {
		for(auto& y1: y.second) {
			string t = (format("P\t%1%\t%2%\t\"%3%\"\tGBP\t%4$.7f\n") % y1.dstamp % y1.tstamp % y1.ticker % (y1.price/100)).str();
			trans.push_back(make_pair(y1.dstamp, t));
		}
	}
	sort(begin(trans), end(trans), sorter);

	dat = "";
	for(auto& p:trans) dat += p.second;
	fname = rootdir() + "/prices.dat";
	spit(fname, dat);
}

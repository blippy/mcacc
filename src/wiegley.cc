#include <assert.h>
#include <iostream>
#include <fstream>
#include <set>
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

/* Create the ledger.dat file */
void mkledger(const etran_ts& es, const ntran_ts& ns)
{
	vector<spair> trans;

	for(auto& e: es) {
		//char sgn = e.buy ? ' ' : '-';
		string t1 = (format("%1%\t*\tetran\n") % e.dstamp).str() ;
		assert(e.typ != unknown);
		string at = (e.typ == leak) ? "@" : "@@";
		string lcost = format_num(labs(e.cost.get()), 2);
		string t2 = (format("\tEquity:%1%\t\"%2%\"\t%3%\t%4%\tGBP\t%5%\n") % 
				e.folio % e.ticker %  e.qty.str() % at % lcost).str();
		string t3 = (format("\t%1%\n\n") % e.folio).str();
		string t = t1 + t2 + t3;
		//cout << t;
		trans.push_back(make_pair(e.dstamp, t));
	}

	for(auto& n: ns) {
		string t1 = (format("%1%\t*\t%2%\n") % n.dstamp % n.desc).str();
		//string t2 = (format("\t%1%\tGBP\t%2%\n") % n.dr % to_gbp(n.amount)).str();
		string amt = n.amount.str();
		string t2 = (format("\t%1%\tGBP\t%2%\n") % n.dr % amt).str();

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
}


// TODO reusable
//template<class T<string> >
void spit_strings(const string& filename, const multiset<string>& lines)
{
	ofstream out;
	out.open(filename, ofstream::trunc);
	for(auto& line: lines) out << line << endl;
	out.close();

}

void mkprices(const yahoo_ts&  ys)
{
	multiset<string> prices;
	for(auto& y: ys) {
		//for(auto& y1: y.second) {
			string price_str = format_num(y.price/100, 7);
			string ticker = "\"" + y.ticker + "\"";
			strings fields = {"P", y.dstamp, y.tstamp, ticker, "GBP", price_str};
			string line = intercalate("\t", fields);			
			prices.insert(line);
		//}
	}

	string fname = rootdir() + "/prices.dat";
	spit_strings(fname, prices);
}

void wiegley(const inputs_t& inputs)
{
	// note that I split this out into two functions for profiling purposes
	mkledger(inputs.etrans, inputs.ntrans);
	mkprices(inputs.yahoos);
}

#include "cgt.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <cmath>
#include <set>
#include <string>

#include "common.hpp"
#include "reusable.hpp"

//using std::cout;
using std::endl;
using std::ofstream;
using std::string;

string mkrow(etran_t& e)
{
	string bs = e.buy? "B" : "S"; // TODO LOW could be abstracted into etran..cc

	string y = e.dstamp.substr(0, 4);
	string m = e.dstamp.substr(5, 2);
	string d = e.dstamp.substr(8, 2);
	string dstamp = d + "-" + m + "-" + y;

	string share_str = format_num(fabs(e.qty), 4);
	
	string price_str = format_num(fabs(e.cost/e.qty/100), 5);

	return  intercalate("\t", {bs, dstamp, e.ticker, share_str, price_str, "0.00", "0.00"});
	//return result;
}

//bool match(etran_t &e)
//{
//	return true;
//}

void cgt(const etran_ts& es, const period &per)
{
	string fname;
	s3("cgt-1.rep", fname);
	ofstream sout;
	sout.open(fname);

	std::set<string> tickers;
	for(auto& e: es)
		if(e.taxable && per.during(e.dstamp)  && ! e.buy) tickers.insert(e.ticker);

	etran_ts es1;
	for(auto& e:es)
		if(tickers.find(e.ticker) != end(tickers))
			es1.push_back(e);

	for(auto& e:es1) sout << mkrow(e) << endl;

	sout.close();

}
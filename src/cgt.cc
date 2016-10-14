#include "cgt.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <cmath>
#include <set>
#include <string>

#include "common.hpp"
#include "cpq.hpp"
#include "reusable.hpp"

//using std::cout;
using std::endl;
using std::ofstream;
using std::string;

string mkrow(etran_t& e)
{

	string y = e.dstamp.substr(0, 4);
	string m = e.dstamp.substr(5, 2);
	string d = e.dstamp.substr(8, 2);
	string dstamp = d + "-" + m + "-" + y;

	string share_str = e.qty.pos_str();
	
	price p = e.cost/e.qty;
	//string price_str = format_num(fabs(e.cost.get()/e.qty.get()/100), 5);
	string price_str = format_num(p.get(), 5);

	return  intercalate("\t", {e.buystr(), dstamp, e.ticker, share_str, 
			price_str, "0.00", "0.00"});
}


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

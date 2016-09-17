#include "cgt.hpp"

#include <fstream>
#include <iostream>
#include <set>
#include <string>

using std::cout;
using std::endl;
using std::string;

void cgt(const etran_ts& es, const period &per)
{
	cout << "mcacc cgt" << endl;

	//string start_date, end_date;
	//get_period(start_date, end_date);
	
	std::set<string> tickers;
	for(auto e: es) {
		if(e.taxable && per.during(e.dstamp)  && ! e.buy) tickers.insert(e.ticker);
	}

	for(auto & t: tickers) {
		// NB take care of leak-1 entries, which confuse the issue
		cout << t << endl; // TODO it's here that we need to do something with the hit tickers
	}
	cout << "TODO: cgt - pending something interesting to report" << endl;
}

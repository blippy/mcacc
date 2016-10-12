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
#include <map>
#include <set>
#include <stdexcept>

#include "common.hpp"
#include "types.hpp"
#include "reusable.hpp"
#include "stend.hpp"

using namespace std;

bool has_key(const stend_ts& stends, const std::string& ticker)
{
	return stends.find(ticker) != stends.end();
}


//stend_ts stend_main(const inputs_t& inputs, period& per)
stend_ts stend_main(const yahoo_ts& yahoos, const period& per)
{
	stend_ts stends;

	//auto& ys = inputs.yahoos; // NB it is sorted!
	for(auto& y:yahoos) { 
		stend s;
		s.ticker = y.ticker;
	       	stends[y.ticker] = s;
	}

	for(auto& y:yahoos) {
		stend& s = stends[y.ticker];
		if(y.dstamp<per.start_date){
			s.start_dstamp = y.dstamp;
			s.start_price = y.yprice ;
		}

		if(y.dstamp<=per.end_date) {
			s.end_dstamp = y.dstamp;
			s.end_price.set(y.yprice);
		}
		stends[s.ticker] = s;
	}

	ofstream ofs;
	string fname;
	s3("stend.dsv", fname);
	ofs.open(fname);
	for(auto& s1:stends) {
		stend s = s1.second;
		strings fields = { s.ticker , s.start_dstamp, 
			s.start_price.str6(), s.end_dstamp, 
			s.end_price.str6() }; 
		ofs << intercalate("\t", fields) << endl;
	}
	ofs.close();

	return stends;
}

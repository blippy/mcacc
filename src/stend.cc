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
#include "parse.hpp"
#include "types.hpp"
#include "reusable.hpp"
#include "stend.hpp"

using namespace std;



bool ticker_sorter(yahoo_t a, yahoo_t b)
{
	tie(a.ticker, a.dstamp, a.tstamp) < tie(b.ticker, b.dstamp, b.tstamp);
}
stend_ts stend_main(const inputs_t& inputs, period& per)
{
	stend_ts stends;

	auto ys = inputs.yahoos;
	for(auto& y:ys) {
		stend_t s;
		s.ticker = y.first;
		//cout << y.first << endl;
		auto& vals = y.second;
		sort(begin(vals), end(vals), ticker_sorter);
		for(auto& v:vals) {
			if(v.dstamp<per.start_date) {
				s.start_dstamp = v.dstamp;
				s.start_price = v.price ;
			}
			if(v.dstamp<=per.end_date) {
				s.end_dstamp = v.dstamp;
				s.end_price = v.price;
			}


		}
		stends[s.ticker] = s;
	}

	ofstream ofs;
	string fname;
	s3("stend.dsv", fname);
	ofs.open(fname);
	for(auto& s1:stends) {
		stend_t s = s1.second;
		//cout << s.ticker << endl;
		strings fields = { s.ticker , s.start_dstamp, to_string(s.start_price), s.end_dstamp, to_string(s.end_price) }; 
		ofs << intercalate("\t", fields) << endl;
	}
	ofs.close();

	return stends;
}

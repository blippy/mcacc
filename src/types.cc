/** File containing type information */

#include <tuple>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <sstream>

#include "common.h"
#include "types.h"
#include "reusable.h"


double enpennies(std::string s, bool round)	
{
	double p= 100.0* stod(s);
	if(round) p = bround(p);
	return p;
}
string to_gbx(pennies_t p)
{
	string s1 =format_num(p, 2);
	string s2 = pad_left(s1, 12);
	return s2 ;
}

string to_gbp(pennies_t p) { return to_gbx(p/100.0); }


string pad_ticker(string ticker) { return pad_right(ticker, 7); }
string pad_ticker(char c) { return nchars(c, 7);}

string pad_gbp(char c) { return nchars(c, 12);}
string pad_gbp(string s) { return pad_left(s, 12); }

string ret_str(double r)
{
	char ret[7];
	snprintf(ret, 7, "%7.2f", r);
	return string(ret);
}


string ret_str(double num, double denom) {  return ret_str(num/denom*100-100); }
string ret_str(char c) { return nchars(c, 6); }
string ret_str(string s) { return pad_left(s, 6); }


void print_strings(ostream &ost, const strings &strs)
{
	auto line = intercalate(" ", strs);
	ost << line << endl;
}


void stend_c::from_vec(strings &row)
{
	ticker = row[0];
	start_dstamp = row[1];
	start_price = stod(row[2]);
	end_dstamp = row[3];
	end_price = stod(row[4]);

}

stends_t load_stends()
{
	string fname;
	s3("stend.dsv", fname);
	vecvec_t rows = vecvec(fname);
	stends_t smap;
	for(strings r : rows) {
		stend_c s;
		s.from_vec(r);
		smap[r[0]] = s;
	}
	return smap;
}

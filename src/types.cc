/** File containing type information */

#include <tuple>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <sstream>

#include "common.hpp"
#include "types.hpp"
#include "reusable.hpp"

using namespace std;


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

std::string ret_str(const centis& num, const centis& denom)
{
	return ret_str(num.get(), denom.get());
}

void print_strings(ostream &ost, const strings &strs)
{
	auto line = intercalate(" ", strs);
	ost << line << endl;
}



PeriodTiming period::when (const std::string &dstamp) const
{
	if(dstamp < start_date) return perBefore;
	if(dstamp > end_date) return perAfter;
	return perDuring;
}

bool period::during(const std::string &dstamp) const
{
	return when(dstamp) == perDuring;
}



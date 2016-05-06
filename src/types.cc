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

// pad right to n characters
string pright(string s, int n)
{
	string res = s;
	for(int i = 0; i < n-s.size(); i++) res += " ";
	return res;
}

string pleft(string s, int n)
{
	string res = "";
	for(int i=0; i< n- s.size(); i++) { res += " "; }
	res += s;
	return res;
}
/** format a double N as a string to DP decimal places
 *TODO - document this, as it is very useful
 Includes required are #include <sstream> [sic]
 * */
string format_num(double n, int dp)
{
	//string s;
	std::ostringstream s;
	s.precision(dp);
	s << std::fixed;
	//ostr << setprecision(dp) << n;
	s <<  n;
	//cout << "format_num called" << endl;
	return s.str();
}

double enpennies(std::string s, bool round)	
{
	double p= 100.0* stod(s);
	if(round) p = bround(p);
	return p;
}
string to_gbx(pennies_t p)
{
	string s1 =format_num(p, 2);
	string s2 = pleft(s1, 12);
	return s2 ;
}

string to_gbp(pennies_t p) { return to_gbx(p/100.0); }


string char_times(char c, int num) 
{
	string res = "";
	for(int i=0; i<num; i++) res += c;
	return res;
}
string pad_ticker(string ticker) { return pright(ticker, 7); }
string pad_ticker(char c) { return char_times(c, 7);}

string pad_gbp(char c) { return char_times(c, 12);}
string pad_gbp(string s) { return pleft(s, 12); }

string ret_str(double r)
{
	char ret[7];
	snprintf(ret, 7, "%7.2f", r);
	return string(ret);
}


string ret_str(double num, double denom) {  return ret_str(num/denom*100-100); }
string ret_str(char c) { return char_times(c, 6); }
string ret_str(string s) { return pleft(s, 6); }


void print_strings(ostream &ost, const strings &strs)
{
	auto line = interspersing(" ", strs);
	ost << line << endl;
}





void etrana::from_vec(std::vector<std::string> &row)
{
	taxable = row[0] == "T";
	dstamp = row[1];
	buy = row[2] == "B";
	folio = row[3];
	sym = row[4];
	qty = stod(row[5]);
	cost = stod(row[6]);
	ucost = stod(row[7]);
	ticker = row[8];
	start_dstamp = row[9];
	start_price = stod(row[10]);
	end_dstamp = row[11];
	end_price = stod(row[12]);
	prior_year_profit = stod(row[13]);
	vbefore = stod(row[14]);
	flow = stod(row[15]);
	profit = stod(row[16]);
	vto = stod(row[17]);
	//cout << "*"  << cost << endl ;

}



bool etrana::operator<(const etrana& rhs)
{
	return std::tie(sym, dstamp) < std::tie(rhs.sym, rhs.dstamp);
}

bool etrana_gt(etrana &a, etrana &b)
{
	return std::tie(a.sym, a.dstamp) > std::tie(b.sym, b.dstamp);
}

etranas_t load_etranas()
{
	vecvec_t rows = read_registered_dsv(etransa);
	etranas_t res;
	for(vs_t &r: rows) { 
		etrana e;
		e.from_vec(r);
		res.push_back(e);
	}
	

	// cubie seems to have problems with sorting, so I'll write my own algo: insertion sort
	// https://en.wikipedia.org/wiki/Insertion_sort
	for(int i=0; i< res.size(); i++) {
		etrana x = res[i];
		int j = i-1;
		while(j>=0 && etrana_gt(res[j],x)) {
			res[j+1] = res[j];
			j -= 1;
		}
		res[j+1] = x;
	}

	return res;
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

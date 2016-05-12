#ifndef TYPES_H
#define TYPES_H

#include <cstdio>
#include <string>
#include <vector>
#include <map>

//#include "autotypes.h"
#include "parse.h"

typedef double pennies_t;
typedef std::vector<std::string> strings;


string to_gbp(pennies_t p);
string to_gbx(pennies_t p);


double enpennies(std::string s, bool round = false);
typedef std::string dstamp_t;

string pad_ticker(string ticker);
string pad_ticker(char c);

string pad_gbp(char c); 
string pad_gbp(string s);
string ret_str(double r);
string ret_str(double num, double denom); 
string ret_str(char c);
string ret_str(string s);

void print_strings(ostream &ost, const strings &strs);



class stend_c {
	public:
		std::string ticker;
		dstamp_t start_dstamp;
		pennies_t start_price;
		dstamp_t end_dstamp;
		pennies_t end_price;

		stend_c() {};
		void from_vec(strings &row);
};


typedef std::map<string, stend_c> stends_t;
stends_t load_stends();

#endif

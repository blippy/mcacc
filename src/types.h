#ifndef TYPES_H
#define TYPES_H

#include <cstdio>
#include <string>
#include <vector>
#include <map>

#include "parse.h"

typedef double pennies_t;
typedef std::vector<std::string> strings;


string pright(string s, int n);
string pleft(string s, int n);
string format_num(double n, int dp);

string to_gbp(pennies_t p);
string to_gbx(pennies_t p);


double enpennies(std::string s, bool round = false);
typedef std::string dstamp_t;

string char_times(char c, int num);
string pad_ticker(string ticker);
string pad_ticker(char c);

string pad_gbp(char c); 
string pad_gbp(string s);
string ret_str(double r);
string ret_str(double num, double denom); 
string ret_str(char c);
string ret_str(string s);

void print_strings(ostream &ost, const strings &strs);

/** enhanced etran
 * */
class etrana {
	public:
		bool taxable;
		dstamp_t dstamp;
		bool buy;
		std::string folio;
		std::string sym;
		double qty;
		pennies_t cost;
		pennies_t ucost;
		std::string ticker;
		dstamp_t start_dstamp;
		pennies_t start_price;
		dstamp_t end_dstamp;
		pennies_t end_price;
		pennies_t prior_year_profit;
		pennies_t vbefore;
		pennies_t flow;
		pennies_t profit;
		pennies_t vto;

		etrana() {};
		bool operator<(const etrana& rhs);
		void from_vec(std::vector<std::string> &row);

};

bool same_ticker(etrana a, etrana b);
typedef vector<etrana> etranas_t;
etranas_t load_etranas();

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

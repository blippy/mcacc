#ifndef TYPES_H
#define TYPES_H

#include <cstdio>
#include <string>
#include <vector>
#include <map>

//#include "autotypes.h"
//#include "parse.h"

typedef double pennies_t;
typedef std::vector<std::string> strings;


std::string to_gbp(pennies_t p);
std::string to_gbx(pennies_t p);


double enpennies(std::string s, bool round = false);
typedef std::string dstamp_t;

std::string pad_ticker(std::string ticker);
std::string pad_ticker(char c);

std::string pad_gbp(char c); 
std::string pad_gbp(std::string s);
std::string ret_str(double r);
std::string ret_str(double num, double denom); 
std::string ret_str(char c);
std::string ret_str(std::string s);

void print_strings(std::ostream &ost, const strings &strs);



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


enum PeriodTiming { perBefore, perDuring, perAfter };
class period {
	public:
		period() {};
		std::string start_date = "000-01-01";
		std::string end_date = "3000-12-31";
		PeriodTiming when(const std::string &dstamp) const;
		bool during(const std::string &dstamp) const;		
};

typedef std::map<std::string, stend_c> stends_t;
stends_t load_stends();

#endif

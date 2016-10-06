#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

//#include "inputs.hpp"
#include "price.hpp"

typedef struct yahoo_t {
	std::string dstamp;
	std::string tstamp;
	std::string ticker;
	//double rox = 1;
	price yprice;
	price chg;
	double chgpc = 0;
	std::string currency = "P";
	std::string desc = "no desc";
} yahoo_t;

bool operator<(yahoo_t a, yahoo_t b);

//typedef std::map<std::string, std::vector<yahoo_t> > yahoo_ts;
//typedef std::multimap<std::string, yahoo_t> yahoo_ts;
typedef std::set<yahoo_t> yahoo_ts;

std::vector<std::string> fetch_tickers(const std::vector<std::string>& tickers, std::string& usd);

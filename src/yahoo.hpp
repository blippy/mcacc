#pragma once

#include <map>
#include <string>
#include <vector>

//#include "inputs.hpp"

typedef struct yahoo_t {
	std::string dstamp;
	std::string tstamp;
	std::string ticker;
	double rox = 1;
	double price = 0;
	double chg = 0;
	double chgpc = 0;
	std::string currency = "P";
	std::string desc = "no desc";
} yahoo_t;

bool operator<(yahoo_t a, yahoo_t b);

typedef std::map<std::string, std::vector<yahoo_t> > yahoo_ts;

//void process_yahoos(const inputs_t& inps, bool with_fetch);
std::vector<std::string> fetch_tickers(const std::vector<std::string>& tickers, std::string& usd);

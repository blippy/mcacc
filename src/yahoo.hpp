#pragma once

#include <map>
#include <string>
#include <vector>


typedef struct yahoo_t {
	std::string dstamp;
	std::string tstamp;
	std::string ticker;
	double rox;
	double price;
	double chg;
	double chgpc;
	std::string currency;
	std::string desc;
} yahoo_t;

//typedef std::vector<yahoo_t> yahoo_ts;
typedef std::map<std::string, std::vector<yahoo_t> > yahoo_ts;

// TODO LOW is fetch_tickers used any more?
std::vector<std::string> fetch_tickers(const std::vector<std::string>& tickers, std::string& usd);

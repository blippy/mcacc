#pragma once
//#ifndef NACC_H
//#define NACC_H

#include <map>
#include <string>
#include <vector>

#include "types.hpp"



typedef struct nacc_t {
	std::string acc;
	std::string alt;
	std::string typ;
	double scale;
	std::string desc;
	pennies_t bal = 0;
} nacc_t;






typedef std::map<std::string, nacc_t> nacc_ts;
void load(nacc_ts & results);
void save(const nacc_ts &xs);

//#endif

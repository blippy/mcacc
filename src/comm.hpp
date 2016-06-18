#pragma once

#include <map>
#include <string>
#include <vector>

typedef struct comm_t {
	std::string ticker;
	std::string down;
	std::string typ;
	std::string unit;
	std::string desc;
} comm_t;


typedef std::map<std::string, comm_t> comm_ts;
void load(comm_ts & results);
void save(const comm_ts &xs);

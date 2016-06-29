#pragma once

#include <string>
#include <vector>

#include "types.hpp"

typedef struct post_t {
	std::string dstamp;
	std::string dr;
	std::string cr;
	pennies_t amount;
	std::string desc;
} post_t;

typedef std::vector<post_t> post_ts;


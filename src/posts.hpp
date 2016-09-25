#pragma once

#include <algorithm>
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

post_ts posts_main(const inputs_t& inputs);

bool operator<(const post_t& a, const post_t& b);

#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "types.hpp"
//#include "centis.hpp"
#include "dec.hpp"

typedef struct post_t {
	std::string dstamp;
	std::string dr;
	std::string cr;
	currency amount;
	std::string desc;
} post_t;

typedef std::vector<post_t> post_ts;

//post_ts posts_main(const inputs_t& inputs);
post_ts posts_main(const inputs_t& inputs, const detran_cs& augetrans);

bool operator<(const post_t& a, const post_t& b);

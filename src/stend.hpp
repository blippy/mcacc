#pragma once

#include <string>

#include "inputs.hpp"
#include "types.hpp"


typedef struct stend_t {
		std::string ticker = "UNKNOWN";
		dstamp_t start_dstamp = "0000-00-00";
		pennies_t start_price = 0;
		dstamp_t end_dstamp = "3000-12-31";
		pennies_t end_price = 0;
} stend_t;

typedef std::map<std::string, stend_t> stend_ts;


stend_ts stend_main(const inputs_t& inputs, period& per);


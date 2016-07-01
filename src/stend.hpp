#pragma once

#include <string>

#include "inputs.hpp"
#include "types.hpp"


typedef struct stend_t {
		std::string ticker;
		dstamp_t start_dstamp;
		pennies_t start_price;
		dstamp_t end_dstamp;
		pennies_t end_price;
} stend_t;

typedef std::map<std::string, stend_t> stend_ts;
//stends_t load_stends();


stend_ts stend_main(const inputs_t& inputs, period& per);

/*
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
*/

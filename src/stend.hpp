#pragma once

#include <string>

#include "inputs.hpp"
#include "types.hpp"


class stend {
	public:
		stend() {};
		std::string ticker = "UNKNOWN";
		dstamp_t start_dstamp = "0000-00-00";
		price start_price;
		dstamp_t end_dstamp = "3000-12-31";
		price end_price;
};

typedef std::map<std::string, stend> stend_ts;


//stend_ts stend_main(const inputs_t& inputs, period& per);
stend_ts stend_main(const yahoo_ts& yahoos, const period& per);


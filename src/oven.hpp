#pragma once

#include "inputs.hpp"

class oven
{
	public:
		void load_inputs();
		void fetch();
		void process();
		period curr_period() const;
	private:
		inputs_t user_inputs;
		yahoo_ts fetched_yahoos;
};


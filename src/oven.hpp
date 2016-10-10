#pragma once

#include "inputs.hpp"

class oven
{
	public:
		void load_inputs();
		void fetch();
		//void process();
		void process(bool do_wiegley);
		period curr_period() const;
	private:
		inputs_t user_inputs;
		yahoo_ts fetched_yahoos;
};


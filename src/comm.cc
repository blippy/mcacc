#include "comm.hpp"
#include "common.hpp"
#include "types.hpp"

/*
void load(comm_ts & results)
{
	//comm_ts results;
	std::string filename;
	s1("comm.dsv", filename);
	vecvec_t vvs = vecvec(filename);
	for(auto& vs: vvs) {
		comm_t astruct; 

		conv(astruct.ticker, vs.at(0)); 
		conv(astruct.down, vs.at(1)); 
		conv(astruct.typ, vs.at(2)); 
		conv(astruct.unit, vs.at(3)); 
		conv(astruct.desc, vs.at(4)); 

		results[astruct.ticker] = astruct;
	}

}

void save(const comm_ts &xs) {}
*/

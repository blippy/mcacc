include(common.m4)
/* This header file was automatically created. Do not edit */
`#'include "NAME.hpp"

divert(-1)
define(`defstruct', `')
define(STRING, std::string)
define(FN, 0)
define(`sfield', `	conv(astruct.$1, vs.at(FN)); define(`FN', incr(FN))')
define(`endstruct', `')
divert

#include <common.h>

NAME_S load()
{
	NAME_S results;
	std::string filename;
	s1("NAME.dsv", filename);
	vecvec_t vvs = vecvec(filename);
	for(auto& vs: vvs) {
		NAME_T astruct; 
include(nacc.m4)
		results[astruct.MAPKEY] = astruct;
	}
	return results;

}

void save(const NAME_S &xs) {}

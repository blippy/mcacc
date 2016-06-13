#include "assets.hpp"





std::string get_html()
{
#include "assets_mcacc_htm.xxd"
	std::string str;
	str.assign((const char*) ___assets_mcacc_htm, 
			(size_t) ___assets_mcacc_htm_len);
	return str;
}

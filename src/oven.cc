#include <algorithm>
//#include <iterator>

#include "oven.hpp"

#include "stend.hpp"
#include "yproc.hpp"


using namespace std;

void oven::load_inputs()
{
	user_inputs = read_inputs();
}

void oven::fetch()
{
	for(const auto& y:process_yahoos(user_inputs))
		fetched_yahoos.insert(y);
}

period oven::curr_period() const
{
	return user_inputs.p;
}
void oven::process()
{
	yahoo_ts all_yahoos;
	set_union(user_inputs.yahoos.begin(), user_inputs.yahoos.end(),
			fetched_yahoos.begin(), fetched_yahoos.end(),
			inserter(all_yahoos, all_yahoos.begin()));
	stend_main(all_yahoos, curr_period());
}
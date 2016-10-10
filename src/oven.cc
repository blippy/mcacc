#include <algorithm>
//#include <iterator>

#include "oven.hpp"

#include "cgt.hpp"
#include "epics.hpp"
#include "etb.hpp"
#include "etrans-aug.hpp"
#include "gaap.hpp"
#include "posts.hpp"
#include "stend.hpp"
#include "wiegley.hpp"
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
void oven::process(bool do_wiegley)
{
	yahoo_ts all_yahoos;
	set_union(user_inputs.yahoos.begin(), user_inputs.yahoos.end(),
			fetched_yahoos.begin(), fetched_yahoos.end(),
			inserter(all_yahoos, all_yahoos.begin()));
	const period& per = curr_period();
	const stend_ts stends = stend_main(all_yahoos, per);
	const augetran_ts augetrans = eaug_main(user_inputs.etrans, stends,
			per);
	const post_ts posts = posts_main(user_inputs, augetrans);
	etb_main(user_inputs.naccs, posts);
	gaap_main(user_inputs.naccs, per);
	epics_main(augetrans, stends);
	cgt(user_inputs.etrans, per);

	if(do_wiegley) wiegley(user_inputs); // TODO not sure if this requires augmented etrans

}

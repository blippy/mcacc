#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "common.hpp"
#include "inputs.hpp"
#include "reusable.hpp"
#include "posts.hpp"

using namespace std;

void push(post_ts &ps, const string& dstamp, const string& ticker, string acc, string str, int amount)
{
	post_t p;
	if(amount == 0) return;
	p.dstamp = dstamp;
	p.dr = acc;
	p.cr = str;
	p.amount = amount;
	p.desc = str + ":" + ticker;
	ps.push_back(p);
}

post_ts posts_main(const inputs_t& inputs)
{

	post_ts ps;
	for(auto& n:inputs.ntrans) {
		if(n.dstamp > inputs.p.end_date) continue;
		if(n.amount ==0) continue;

		post_t p;
		p.dstamp = n.dstamp;
		p.dr = n.dr;
		p.cr = n.cr;
		if(n.dstamp < inputs.p.start_date) {
			p.dr = inputs.naccs.at(p.dr).alt;
			p.cr = inputs.naccs.at(p.cr).alt;
		}
		p.amount = n.amount;
		p.desc = n.desc;
		ps.push_back(p);

		std::swap(p.dr, p.cr);
		p.amount = -p.amount;
		ps.push_back(p);
	}
	for(auto& e: inputs.etrans) {
		if(e.dstamp > inputs.p.end_date) continue;

		push(ps, e.dstamp, e.ticker, e.folio,        "pCost", -e.cost);
		push(ps, e.dstamp, e.ticker, e.folio + "_c", "pVcd", e.vto);
		push(ps, e.dstamp, e.ticker, e.folio + "_g", "pPdp", -e.profit);
		push(ps, e.dstamp, e.ticker, "opn",          "pbd", -e.prior_year_profit);
	}

	return ps;
}

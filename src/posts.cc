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

bool operator<(const post_t& a, const post_t& b)
{
	return std::tie(a.dr, a.dstamp) < std::tie(b.dr, b.dstamp);
}

void push(post_ts &ps, const string& dstamp, const string& ticker, string acc, string str, double sgn, const centis& amount)
{
	post_t p;
	if(amount.get() == 0) return;
	p.dstamp = dstamp;
	p.dr = acc;
	p.cr = str;
	p.amount.set(sgn * amount.get());
	p.desc = str + ":" + ticker;
	ps.push_back(p);
}

post_ts posts_main(const inputs_t& inputs, const augetran_ts& augetrans)
{

	post_ts ps;
	for(auto& n:inputs.ntrans) {
		if(n.dstamp > inputs.p.end_date) continue;
		if(n.amount.get() ==0) continue;

		post_t p;
		p.dstamp = n.dstamp;
		p.dr = n.dr;
		p.cr = n.cr;
		if(n.dstamp < inputs.p.start_date) {
			p.dr = inputs.naccs.at(p.dr).alt;
			p.cr = inputs.naccs.at(p.cr).alt;
		}
		//p.amount = n.amount;
		p.amount.set(n.amount.get());
		p.desc = n.desc;
		ps.push_back(p);

		std::swap(p.dr, p.cr);
		//p.amount = -p.amount;
		p.amount.negate();
		ps.push_back(p);
	}
	for(const auto& ae: augetrans) {
		const etran_t& e = ae.etran;
		if(e.dstamp > inputs.p.end_date) continue;

		push(ps, e.dstamp, e.ticker, e.folio,        "pCost", 
				-1, e.cost);
		push(ps, e.dstamp, e.ticker, e.folio + "_c", "pVcd", 
				1, ae.vto);
		push(ps, e.dstamp, e.ticker, e.folio + "_g", "pPdp",
			       -1, ae.profit);
		push(ps, e.dstamp, e.ticker, "opn",          "pbd", 
				-1, ae.prior_year_profit);
	}

	sort(begin(ps), end(ps));
	return ps;
}

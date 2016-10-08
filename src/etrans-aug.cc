#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>

#include "inputs.hpp"
#include "reusable.hpp"
#include "common.hpp"
#include "types.hpp"
#include "etrans-aug.hpp"

using namespace std;




void augment(const inputs_t& inputs, etran_t& e, const stend_ts& stends)
{
	const stend& s = stends.at(e.ticker);

	e.ucost.reprice(e.cost, e.qty); 
	e.start_dstamp = s.start_dstamp;
	e.start_price.set(s.start_price);
	e.end_dstamp = s.end_dstamp;
	e.end_price.set(s.end_price);

	quantity qty;
	qty.inc(e.qty);
	e.vbefore.set(0);
	e.flow.set(0);
	e.prior_year_profit.set(0);
	recentis(e.vto, s.end_price, qty);
	const period& per = inputs.p;
	switch(per.when(e.dstamp)) {
		case perBefore:
			recentis(e.vbefore, s.start_price, qty);
			e.prior_year_profit.set(e.vbefore.get() - e.cost.get());
			break;
		case perDuring:
			e.flow = e.cost;
			break;
		case perAfter:
			e.vto.set(0);
	}

	e.profit.set(e.vto.get() - e.vbefore.get() - e.flow.get());
}

void write_etran(ofstream& ofs, const etran_t& e)
{
	auto bout = [&ofs](bool b) { ofs << b << '\t';};
	auto ceout = [&ofs](const centis& c) { ofs << c.str() << '\t' ; };
	auto sout = [&ofs](string s) { ofs << s << '\t' ; };
	auto dout = [&ofs](double d, int dp) { ofs << format_num(d, dp) 
		<< '\t' ; };
	auto pout = [&ofs](const price& p){ ofs << p.str6() << '\t' ; };
	bout(e.taxable? 'T' : 'F'); // 1
	sout(e.dstamp); // 2
	bout(e.buy? 'B' : 'S'); // 3
	sout(e.folio); // 4
	sout(e.ticker); // 5
	ofs << e.qty.str(); //dout(e.qty, 6); // 6
	ceout(e.cost); // 7
	pout(e.ucost); //dout(e.ucost, 6); // 8
	sout(e.ticker); // 9
	sout(e.start_dstamp); // 10
	pout(e.start_price); //dout(e.start_price, 6); // 11
	sout(e.end_dstamp); // 12
	pout(e.end_price); //dout(e.end_price, 6); //13
	ceout(e.prior_year_profit); // 14
	ceout(e.vbefore); // 15
	ceout(e.flow); // 16
	ceout(e.profit); // 17
	ceout(e.vto); // 18
	ofs << endl;
}

void eaug_main(inputs_t& inputs, const stend_ts& stends)
{
	string fname;
	for(auto& e:inputs.etrans) augment(inputs, e, stends);

	// save
	s3("etrans-aug.dsv", fname);
	ofstream ofs;
	ofs.open(fname);
	for(auto& e:inputs.etrans) write_etran(ofs, e);
	ofs.close();
}


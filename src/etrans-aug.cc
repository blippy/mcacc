#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>

#include "inputs.hpp"
#include "reusable.hpp"
#include "common.hpp"
#include "parse.hpp"
#include "types.hpp"
#include "etrans-aug.hpp"

using namespace std;




void augment(const inputs_t& inputs, etran_t& e, const stend_ts& stends)
{
	stend_t s;
	try {s = stends.at(e.ticker);}
	catch (const std::out_of_range& oor) {		 
		cerr << "WARN: Creating a fake stend for " 
			<< e.ticker << endl;
		s.ticker = e.ticker;
	}

	e.ucost = e.cost.get()/e.qty;
	e.start_dstamp = s.start_dstamp;
	e.start_price = s.start_price;
	e.end_dstamp = s.end_dstamp;
	e.end_price = s.end_price;

	double qty = e.qty;
	e.vbefore.set(0);
	e.flow.set(0);
	e.prior_year_profit.set(0);
	e.vto.set(qty * s.end_price);
	const period& per = inputs.p;
	switch(per.when(e.dstamp)) {
		case perBefore:
			e.vbefore.set(qty * s.start_price);
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
	//auto out = [&ofs](double d) { ofs << d << '\t' ;};
	//auto out = [&ofs](const centis& c) { out(c.get()) ;};
	//auto dout = [&out](double d, int dp) { out(format_num(d, dp)); };
	//auto iout = [&out](int i) { out(i); } ;
	auto bout = [&ofs](bool b) { ofs << b << '\t';};
	auto ceout = [&ofs](const centis& c) { ofs << c.str() << '\t' ; };
	//auto dout = [&ofs](double d) { ofs << d << '\t' ; };
	auto sout = [&ofs](string s) { ofs << s << '\t' ; };
	auto dout = [&ofs](double d, int dp) { ofs << format_num(d, dp) 
		<< '\t' ; };
	bout(e.taxable? 'T' : 'F'); // 1
	sout(e.dstamp); // 2
	bout(e.buy? 'B' : 'S'); // 3
	sout(e.folio); // 4
	sout(e.ticker); // 5
	dout(e.qty, 6); // 6
	ceout(e.cost); // 7
	dout(e.ucost, 6); // 8
	sout(e.ticker); // 9
	sout(e.start_dstamp); // 10
	dout(e.start_price, 6); // 11
	sout(e.end_dstamp); // 12
	dout(e.end_price, 6); //13
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

	//etran_ts& aes =  inputs.etrans;

	/* TODO not sure if we need this
	s1("etran.dsv", fname);
	vecvec_t etrans = vecvec(fname);
	for(vs_t &e: etrans) {
		etran_t cooked;
		cooked.taxable = e[0] == "T";
		cooked.dstamp = e[1];
		cooked.buy = e[2] == "B";
		cooked.folio = e[3];
		cooked.sym = e[4];
		double sgn = cooked.buy ? 1 : -1;
		cooked.qty = sgn * stod(e[5]);
		cooked.cost = bround(100.0* sgn * stod(e[6]));
		aes.push_back(cooked);
	}
	*/


	/* TODO not sure if we need this
	s1("leak-1.dsv", fname);
	vecvec_t leak_1s = vecvec(fname);
	for(auto& x: leak_1s) {
		etrana cooked;
		cooked.taxable = x[0] == "T";
		cooked.dstamp = x[1];
		cooked.buy = false; // we're always assuming a sell at theis stage
		cooked.folio = x[2];
		cooked.sym = x[3];
		cooked.qty = -stod(x[4]);
		cooked.cost = 0;
		// skip description, as there's nowhere to put it
		aes.push_back(cooked);
	}
	*/


	// sort, just to be sure
	// 2016-06-29 shouldn't be necessary as the inputs should have already been sorted
	//sort(begin(aes), end(aes));
	
	// do the actual processing
	//stends_t stends = load_stends();
	//for(auto& e:inputs.etrans) augment(inputs, e);

	// save
	s3("etrans-aug.dsv", fname);
	ofstream ofs;
	ofs.open(fname);
	for(auto& e:inputs.etrans) write_etran(ofs, e);
	ofs.close();
}


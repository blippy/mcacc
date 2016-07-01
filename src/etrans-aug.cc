#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>

#include "inputs.hpp"
#include "reusable.hpp"
//#include "etran.hpp"
#include "common.hpp"
#include "parse.hpp"
#include "types.hpp"
//#include "stend.hpp"
#include "etrans-aug.hpp"

using namespace std;


//string to_intstring(double d) { return to_string(int(d)); }


void augment(const inputs_t& inputs, etran_t& e, const stend_ts& stends) // etrana& e, const stends_t& stends, const period& per)
{
	stend_t s; //tends = inputs.stends;
	try {s = stends.at(e.ticker);}
	catch (const std::out_of_range& oor) {		 
		cerr << "WARN: Creating a fake stend for " 
			<< e.ticker << endl;
		s.ticker = e.ticker;
		//auto v = strings { e.ticker, "X" , "0", "X", "0" };
		//s.from_vec(v);
		//cerr << "etrans-aug.cc:augment(). I give up with fatal exit" << endl;
		//exit(EXIT_FAILURE);
	}

	e.ucost = e.cost/e.qty;
	e.start_dstamp = s.start_dstamp;
	e.start_price = s.start_price;
	e.end_dstamp = s.end_dstamp;
	e.end_price = s.end_price;

	double qty = e.qty;
	e.vbefore = 0;
	e.flow = 0;
	e.prior_year_profit = 0;
	e.vto = bround(qty * s.end_price);
	const period& per = inputs.p;
	switch(per.when(e.dstamp)) {
		case perBefore:
			e.vbefore = bround(qty * s.start_price);
			e.prior_year_profit = e.vbefore - e.cost;
			break;
		case perDuring:
			e.flow = e.cost;
			break;
		case perAfter:
			e.vto = 0;
	}

	e.profit = e.vto - e.vbefore - e.flow;
}

void write_etran(ofstream& ofs, const etran_t& e)
{
	auto out = [&ofs](auto v) { ofs << v << '\t' ;};
	auto dout = [&out](double d, int dp) { out(format_num(d, dp)); };
	auto iout = [&out](int i) { out(i); } ;
	out(e.taxable? 'T' : 'F'); // 1
	out(e.dstamp); // 2
	out(e.buy? 'B' : 'S'); // 3
	out(e.folio); // 4
	out(e.ticker); // 5
	dout(e.qty, 6); // 6
	iout(e.cost); // 7
	dout(e.ucost, 6); // 8
	out(e.ticker); // 9
	out(e.start_dstamp); // 10
	dout(e.start_price, 6); // 11
	out(e.end_dstamp); // 12
	dout(e.end_price, 6); //13
	iout(e.prior_year_profit); // 14
	iout(e.vbefore); // 15
	iout(e.flow); // 16
	iout(e.profit); // 17
	iout(e.vto); // 18
	ofs << endl;
}

void eaug_main(inputs_t& inputs, const stend_ts& stends) //const period &per)
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


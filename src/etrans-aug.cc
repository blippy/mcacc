#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>

#include "reusable.hpp"
#include "etran.hpp"
#include "common.hpp"
#include "parse.hpp"
#include "types.hpp"

using namespace std;


string to_intstring(double d) { return to_string(int(d)); }

/*
// push back double as integer string 
void pbdais(vector<string> &v, double d)
{
	v.push_back(to_intstring(d));
}
*/

void augment(etrana& e, const stends_t& stends, const period& per)
{
	//for(auto &e: aes) {
	//string ticker = e[4];
	stend_c s;
	try {s = stends.at(e.sym);}
	catch (const std::out_of_range& oor) {		 
		cerr << "WARN: Creating a fake stend for " 
			<< e.sym << endl;
		auto v = strings { e.sym, "X" , "0", "X", "0" };
		s.from_vec(v);
	}

	//double unit = stod(e[6])*100.0/stof(e[5]);
	e.ucost = e.cost/e.qty;
	//e.push_back(to_string(unit));
	//e.push_back(s.ticker);
	//e.push_back(s.start_dstamp);
	e.start_dstamp = s.start_dstamp;
	//e.push_back(to_string(s.start_price));
	e.start_price = s.start_price;
	//e.push_back(s.end_dstamp);
	e.end_dstamp = s.end_dstamp;
	//e.push_back(to_string(s.end_price));
	e.end_price = s.end_price;

	//double sgn = e[2] == "B" ? 1 : -1;
	//double qty = sgn * stod(e[5]);
	//e[5] = to_string(qty); // adjust for sign change
	//string dstamp = e[1];

	//double cost = bround(100.0* sgn * stod(e[6]));
	//e[6] = to_intstring(cost); // replace it with signed pennies

	double qty = e.qty;
	//double vbefore = 0, vflow = 0, profit_bd = 0;
	e.vbefore = 0;
	e.flow = 0;
	e.prior_year_profit = 0;
	e.vto = bround(qty * s.end_price);
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


	//pbdais(e, profit_bd);
	//pbdais(e, vbefore);
	//pbdais(e, vflow);
	//pbdais(e, vprofit);
	//pbdais(e, vto);


	
}

void write_etrana(ofstream& ofs, const etrana& e)
{
	auto out = [&ofs](auto v) { ofs << v << '\t' ;};
	auto dout = [&out](double d, int dp) { out(format_num(d, dp)); };
	auto iout = [&out](int i) { out(i); } ;
	out(e.taxable? 'T' : 'F'); // 1
	out(e.dstamp); // 2
	out(e.buy? 'B' : 'S'); // 3
	out(e.folio); // 4
	out(e.sym); // 5
	dout(e.qty, 6); // 6
	iout(e.cost); // 7
	dout(e.ucost, 6); // 8
	out(e.sym); // 9
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

int eaug_main(const period &per)
{
	string fname;
	etranas_t aes;

	s1("etran.dsv", fname);
	vecvec_t etrans = vecvec(fname);
	for(vs_t &e: etrans) {
		etrana cooked;
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


	// sort, just to be sure
	sort(begin(aes), end(aes));
	
	// do the actual processing
	stends_t stends = load_stends();
	for(auto& e:aes) augment(e, stends, per);

	// save
	s3("etrans-aug.dsv", fname);
	//prin_vecvec(etrans, "\t", "\n", fname.c_str());
	ofstream ofs;
	ofs.open(fname);
	for(auto& e:aes) write_etrana(ofs, e);
	ofs.close();


	// output in rec format
	/*
	s3("etrans-aug.rec", fname);
	std::ofstream ofs;
	ofs.open(fname, std::ofstream::out);
	for(auto e:etrans) { 
		for(int i=0 ; i < etrana_fieldnames.size(); i++) {
			ofs << etrana_fieldnames[i] << ": " ;
			ofs << e[i] << endl;
		}
		ofs << endl;
	}
	*/
	
	return EXIT_SUCCESS;
}


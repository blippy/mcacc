#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <typeinfo>
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

string asstr(const centis& c) { return c.str();}
string asstr(const string& s) { return s;}
string asstr(const char& c) { return to_string(c); }
string asstr(const price& p) { return p.str6(); }
template <typename T>
void recline(ofstream& ofs, const string& fname, T const& fvalue)
{
	string fvs= asstr(fvalue);
	/*
	if(typeid(T) == typeid(centis)) { 
		fvs = fvalue.str();
	} else {
		fvs = fvalue;
	}
	*/

	string fstr = pad_right(fname + ": ", 8);
	ofs << fstr << fvs << endl;
}

void write_etran(ofstream& ofs, const etran_t& e)
{
	
	//auto bout = [&ofs](const string& s, char b) { ofs << fstr(s) << b << '\n';};
	//auto bout = [&ofs](const string& s, char b) { recline(ofs, s, b);};
	//auto ceout = [&ofs](const centis& c) { ofs << c.str() << '\t' ; };
	//auto ceout = [&ofs](const string& s, const centis& c) 
	//	{ recline(ofs, s,  c.str()); };
	//auto sout = [&ofs](string s) { ofs << s << '\t' ; };
	//auto sout = [&ofs](const string& s1, const string& s2) 
	//	{ recline(ofs, s1, s2); };
	//auto dout = [&ofs](double d, int dp) { ofs << format_num(d, dp) 
	//	<< '\t' ; };
	//auto pout = [&ofs](const price& p){ ofs << p.str6() << '\t' ; };
	//auto pout = [&ofs](const string& s, const price& p)
	//	{ recline(ofs, s, p.str6()); };
	auto gout = [&ofs](const string& s, const auto& v) {
		recline(ofs, s, v);};

	gout("Tax", e.taxable? 'T' : 'F'); // 1
	gout("Dstamp", e.dstamp); // 2
	gout("Buy", e.buy? 'B' : 'S'); // 3
	gout("Folio", e.folio); // 4
	gout("Eticker", e.ticker); // 5
	//ofs << e.qty.str(); //dout(e.qty, 6); // 6
	gout("Qty", e.qty.str());
	gout("Cost", e.cost); // 7
	gout("Ucost", e.ucost); //dout(e.ucost, 6); // 8
	gout("Eticker", e.ticker); // 9
	gout("Start_dstamp", e.start_dstamp); // 10
	gout("Start_price", e.start_price); //dout(e.start_price, 6); // 11
	gout("End_dstamp", e.end_dstamp); // 12
	gout("End_price", e.end_price); //dout(e.end_price, 6); //13
	gout("Prior_year_profit", e.prior_year_profit); // 14
	gout("Vbefore", e.vbefore); // 15
	gout("Flow", e.flow); // 16
	gout("Profit", e.profit); // 17
	gout("Vto", e.vto); // 18
	ofs << endl;
}

void eaug_main(inputs_t& inputs, const stend_ts& stends)
{
	string fname;
	for(auto& e:inputs.etrans) augment(inputs, e, stends);

	// save
	s3("etrans-aug.rec", fname);
	ofstream ofs;
	ofs.open(fname);
	for(auto& e:inputs.etrans) write_etran(ofs, e);
	ofs.close();
}


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




augetran_t augment(const etran_t& e, const stend_ts& stends, const period& per)
{
	augetran_t aug = e;
	const stend& s = stends.at(e.ticker);

	aug.ucost.reprice(e.cost, e.qty); 
	aug.start_dstamp = s.start_dstamp;
	aug.start_price.set(s.start_price);
	aug.end_dstamp = s.end_dstamp;
	aug.end_price.set(s.end_price);

	quantity qty;
	qty.inc(e.qty);
	aug.vbefore.set(0);
	aug.flow.set(0);
	aug.prior_year_profit.set(0);
	recentis(aug.vto, s.end_price, qty);
	//const period& per = inputs.p;
	switch(per.when(aug.dstamp)) {
		case perBefore:
			recentis(aug.vbefore, s.start_price, qty);
			aug.prior_year_profit.set(aug.vbefore.get() - aug.cost.get());
			break;
		case perDuring:
			aug.flow = aug.cost;
			break;
		case perAfter:
			aug.vto.set(0);
	}

	aug.profit.set(aug.vto.get() - aug.vbefore.get() - aug.flow.get());

	return aug;
}

string asstr(const char& c) { return to_string(c); }
string asstr(const centis& c) { return c.str();}
string asstr(const string& s) { return s;}
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

void write_augetran(ofstream& ofs, const augetran_t& e)
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

	gout("Tax", e.taxable? "T" : "F"); // 1
	gout("Dstamp", e.dstamp); // 2
	gout("Buy", e.buy? "B" : "S"); // 3
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

augetran_ts eaug_main(const etran_ts& etrans, const stend_ts& stends, 
		const period &per)
{
	augetran_ts augs;

	string fname;
	for(auto& e:etrans) 
		if(e.dstamp <= per.end_date)
			augs.push_back(augment(e, stends, per));

	// save
	s3("etrans-aug.rec", fname);
	ofstream ofs;
	ofs.open(fname);
	for(auto& e:augs) write_augetran(ofs, e);
	ofs.close();

	return augs;
}


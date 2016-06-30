#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>
#include <set>

#include "common.hpp"
//#include "autotypes.hpp"
#include "reusable.hpp"
#include "stend.hpp"
#include "epics.hpp"

using namespace std;

void underline(ostream &ost, char c)
{ 
	auto fields = strings {pad_ticker(c), pad_gbp(c), pad_gbp(c), pad_gbp(c), pad_gbp(c), ret_str(c) };
	print_strings(ost, fields);
}
struct folio {
	string name;
	int ftype;
};

vector<folio> folios = {
	{"hal",   0},
	{"hl",    0},
	{"igg",   0},
	{"tdi",   0},
	{"tdn",   0},
	{"mine",  1},
	{"ut",    0},
	{"total", 2}
};

void process_folio(folio &f, set<string> &epic_names, const etran_ts& es, ostream &eout, ostream &pout)
{
	eout << f.name << endl;
	set<string> zeros;
	strings fields;
	string line;
	double tqty, tcost;
	double grand_cost =0, grand_value = 0;
	pennies_t vbefore =0, vflow = 0, vprofit = 0, vto = 0;

	fields = { pad_ticker("TICK"), pad_gbp("COST"), pad_gbp("VALUE"), ret_str("RET%"), 
		pad_gbp("QTY"), pad_gbp("UCOST"), pad_gbp("UVALUE") };
	print_strings(eout, fields);

	for(auto k:epic_names) {
		tqty =0; tcost = 0;
		double uvalue;

		for(auto e:es){

			// determine if we are interested in this epic
			bool match = k == e.ticker;
			switch(f.ftype) {
				case 0: match = match && (e.folio == f.name); break;
				case 1: match = match && (e.folio != "ut"); break;
			}
			if(!match) continue;

			uvalue = e.end_price;
			if(e.buy) {tqty += e.qty; tcost += e.cost;}
			else { double ucost = tcost/tqty; tqty += e.qty; tcost += ucost * e.qty;}

			vbefore += e.vbefore;
			vflow += e.flow;
			vprofit += e.profit;
			vto += e.vto;
		}

		if(tqty == 0) { zeros.insert(k) ; continue; }
		double ucost = tcost/tqty;
		double value = uvalue * tqty;

		fields = {pad_right(k, 7), to_gbp(tcost), to_gbp(value) , ret_str(value, tcost), 
			to_gbx(tqty), to_gbx(ucost), to_gbx(uvalue)};
		print_strings(eout, fields);
		grand_cost += tcost;
		grand_value += value;

	}
	fields = {pad_right("Grand:", 7), to_gbp(grand_cost), to_gbp(grand_value), ret_str(grand_value, grand_cost), 
		pad_gbp(' '), pad_gbp(' '), pad_gbp(' ') };
	print_strings(eout, fields);
	eout << endl;

	// maybe print zeros
	if(f.ftype == 2) {
		eout << "Zeros:" << endl;
		int i = 0;
		for(auto z :zeros) {
			eout << pad_right(z, 8) ;
			i++;
			if(i%10 == 0) eout << endl;
		}
		eout << endl;
	}
		
	
	// portfolios output
	if(f.ftype ==1 || f.ftype==2) underline(pout, '-');
	fields = {pad_ticker(f.name), to_gbp(vbefore), to_gbp(vflow), to_gbp(vprofit), 
		to_gbp(vto), ret_str(vprofit+vbefore, vbefore) };
	print_strings(pout, fields);
	if(f.ftype==2) underline(pout, '=');


}

void print_indices(stend_ts& stends, ostream &pout)
{
	pout << endl;
	string fname;
	//s3("stend.dsv", fname);
	//stends_t stends = load_stends();

	auto indices = strings {"^FTSE", "^FTMC", "^FTAS"};
	for(string& i:indices){
		stend_t s = stends[i];
		double sp = s.start_price;
		double ep = s.end_price;
		double chg = ep - sp;
		auto fields = strings { pad_ticker(i), to_gbx(sp), pad_gbp(' '), to_gbx(chg), to_gbx(ep), ret_str(ep, sp)};
		print_strings(pout, fields);
	}
}

void epics_main(const etran_ts& es, stend_ts& stends)
{
	//etranas_t es = load_etranas();
	// TODO determine sorting requirements
	//assert(std::is_sorted(es.begin(), es.end()));
	//if(!std::is_sorted(es.begin(), es.end()))
	//	std::sort(es.begin(), es.end());

	set<string> keys;
	for(auto e:es) { keys.insert(e.ticker);}

	string filename;
	s3("epics.rep", filename);
	ofstream eout;
	eout.open(filename);

	s3("portfolios.rep", filename);
	ofstream pout;
	pout.open(filename);
	strings fields = strings { pad_ticker("FOLIO"), pad_gbp("VBEFORE"), pad_gbp("VFLOW"), pad_gbp("VPROFIT"), 
		pad_gbp("VTO"), ret_str("VRET")};
	print_strings(pout, fields);

	for(auto f:folios){ process_folio(f, keys, es, eout, pout); }

	print_indices(stends, pout);

	eout.close();
	pout.close();
}

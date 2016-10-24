#include <decimal/decimal>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>
#include <set>

#include "common.hpp"
//#include "cpq.hpp"
#include "dec.hpp"
#include "reusable.hpp"
#include "stend.hpp"
#include "epics.hpp"

using namespace std;
using namespace std::decimal;

void underline(ostream &ost, char c)
{ 
	auto fields = strings {pad_ticker(c), pad_gbp(c), 
		pad_gbp(c), pad_gbp(c), pad_gbp(c), ret_str(c) };
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

void process_folio(folio &f, set<string> &epic_names, const augetran_ts& es, 
		ostream &eout, ostream &pout)
{
	eout << f.name << endl;
	set<string> zeros;
	strings fields;
	string line;
	currency grand_cost, grand_value;
	currency vbefore, vflow, vprofit, vto;

	fields = { pad_ticker("TICK"), pad_gbp("COST"), pad_gbp("VALUE"), 
		ret_str("RET%"), 
		pad_gbp("QTY"), pad_gbp("UCOST"), pad_gbp("UVALUE") };
	print_strings(eout, fields);

	for(const auto& k:epic_names) {
		quantity tqty; 
		currency tcost;
		price uvalue;

		for(const auto& e:es){

			// determine if we are interested in this epic
			bool match = k == e.etran.ticker;
			switch(f.ftype) {
				case 0: match = match && (e.etran.folio == f.name); break;
				case 1: match = match && (e.etran.folio != "ut"); break;
			}
			if(!match) continue;

			uvalue = e.end_price;
			const currency cost = e.etran.buy? e.etran.cost :
				(tcost/tqty) * e.etran.qty; // TODO can we just write (tcost/tqty)*e.etran.qty somehow?
			tcost += cost;
			tqty += e.etran.qty; 

			vbefore += e.vbefore;
			vflow += e.flow;
			vprofit += e.profit;
			vto += e.vto;
		}

		if(tqty.zerop()) { zeros.insert(k) ; continue; }
		//price ucost;
	       	//ucost.reprice(tcost, tqty);
		const price ucost = tcost/tqty;
		currency value = uvalue * tqty;
		//recentis(value, uvalue, tqty);

		fields = {pad_right(k, 7),
		       	tcost.str(), value.str() , 
			ret_str(value.dbl()/ tcost.dbl()), 
			tqty.str(), ucost.str(), uvalue.str()};
		print_strings(eout, fields);
		grand_cost += tcost;
		grand_value +=value;

	}
	fields = {pad_right("Grand:", 7), grand_cost.str(), 
		grand_value.str(),
	       	ret_str(grand_value.dbl()/grand_cost.dbl()), 
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
	fields = {pad_ticker(f.name), vbefore.str(), vflow.str(), 
		vprofit.str(), vto.str(),
	       	ret_str((vprofit + vbefore).dbl()/vbefore.dbl()) };
	print_strings(pout, fields);
	if(f.ftype==2) underline(pout, '=');


}

void print_indices(const stend_ts& stends, ostream &pout)
{
	pout << endl;
	string fname;

	auto indices = strings {"^FTSE", "^FTMC", "^FTAS"};
	for(string& i:indices){
		const stend s = stends.at(i, 
			"epic.cc:print_indices() couldn't find stend with key " + i);
		const price& sp = s.start_price;
		const price&  ep = s.end_price;
	       	//sp.set(s.start_price);
		//ep.set(s.end_price);
		const price chg = ep-sp;
		const double rat = ep.dbl()/sp.dbl();
		//const price
		auto fields = strings { pad_ticker(i), 
			sp.str(), pad_gbp(' '), chg.str(), ep.str(), 
			ret_str(rat)};
		print_strings(pout, fields);
	}
}

void epics_main(const augetran_ts& es, const stend_ts& stends)
{
	set<string> keys;
	for(auto e:es) { keys.insert(e.etran.ticker);}

	string filename;
	s3("epics.rep", filename);
	ofstream eout;
	eout.open(filename);

	s3("portfolios.rep", filename);
	ofstream pout;
	pout.open(filename);
	strings fields = strings { pad_ticker("FOLIO"), pad_gbp("VBEFORE"), 
		pad_gbp("VFLOW"), pad_gbp("VPROFIT"), 
		pad_gbp("VTO"), ret_str("VRET")};
	print_strings(pout, fields);

	for(auto f:folios){ process_folio(f, keys, es, eout, pout); }

	print_indices(stends, pout);

	eout.close();
	pout.close();
}

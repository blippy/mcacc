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

//bool always() { return true;}
//struct folio { string name; int ftype; } ;

        
class folio_c {
	public:
		folio_c(const string& name): m_name(name) {};
		string m_name;
		detran_cs filter(const detran_cs& es) const;
		void calculate(const detran_cs& all_etrans);
		set<string> zeros;
		currency cost;
		currency value;
		void print_to_epic_file(ofstream& ofs) const;
		void print_to_portfolio_file(ofstream& ofs) const;
		vector<detran_c> reduced_epics;
	//string name;
	//int ftype;
	//function<bool(const string& folio accept 
};

bool operator<(const detran_c& lhs, const detran_c& rhs)
{
	return lhs.etran < rhs.etran;
}

detran_cs folio_c::filter(const detran_cs& es) const
{
	detran_cs result;
	for(const auto& e:es){
		const string& folio = e.etran.folio;
		bool match = folio == m_name || 
			(m_name == "mine" && folio != "ut") ||
			m_name == "total";
		if(match)
			result.push_back(e);
	}
	std::sort(result.begin(), result.end()); // mostly for debugging convenience
	return result;
}

// TODO find better home
string ticker(const detran_c& e) { return e.etran.ticker; }

detran_c reduce_all (const detran_cs& es)
{
	detran_c result;
	for(const auto& e:es) result += e;
	return result;
}
detran_cs reduce(const detran_cs& es)
{
	detran_cs result;

	set<string> tickers;
       	for(const auto& e:es) tickers.insert(ticker(e));

	for(const auto& t:tickers) {
		detran_cs ticker_etrans;
		for(const auto& e:es) 
			if(ticker(e) == t) ticker_etrans.push_back(e);

		const detran_c reduction = reduce_all(ticker_etrans); 
		result.push_back(reduction);
	}
	return result;
}
vector<folio_c> g_folios = { 
	folio_c("hal"), folio_c("hl"), folio_c("igg"),
	folio_c("tdi"), folio_c("tdn"), folio_c("mine"), folio_c("ut"), 
	folio_c("total")
};

/*
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
*/

void debug(const detran_c& e, const currency& vto)
{
	static int n_azn = 0;
	return; // just switch it off for now
	if(n_azn==0) cout << "epics.cc:debug() output follows\n";
	if(e.etran.ticker == "AZN.L") n_azn++;
	if(n_azn>1) return;
	if(e.etran.folio != "hal") return;

	cout << pad_right(e.etran.ticker, 6) << e.vto.str() << " ";
	cout << e.etran.qty.str() << e.end_price.str();
       cout << vto.str() << endl;
}

/*
void process_folio(folio &f, set<string> &epic_names, const detran_cs& es, 
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
			debug(e, vto);
		}

		if(tqty.zerop()) { 
			zeros.insert(k);
			continue;
			//Can sometimes get the case where there 
			// are slight rounding errors, e.g. with ICP.L
			//
		       //if(vto.zerop())continue; 
		}
		//price ucost;
		//ucost.reprice(tcost, tqty);
		const price ucost = tcost/tqty;
		currency value = uvalue * tqty;
		//recentis(value, uvalue, tqty);

		fields = {pad_right(k, 7),
			tcost.str(), value.str() , 
			ret_str(value.dbl(), tcost.dbl()), 
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
	//string gain_str = "FOO";
	string gain_str = ret_str(vprofit.dbl()+vbefore.dbl(), vbefore.dbl());
	fields = {pad_ticker(f.name), vbefore.str(), vflow.str(), 
		vprofit.str(), vto.str(), gain_str};
	print_strings(pout, fields);
	if(f.ftype==2) underline(pout, '=');


}
*/

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


void folio_c::calculate(const detran_cs& all_etrans)
{
	//folio_calcs result;
	const detran_cs by_epics = reduce(filter(all_etrans));

	//currency cost, value;
	for(const auto& esum:by_epics){
		if(esum.etran.qty.zerop()) { 
			zeros.insert(esum.etran.ticker);
		} else {
			reduced_epics.push_back(esum);
			cost += esum.etran.cost;
			value += esum.vto;
		}
	}
}

void folio_c::print_to_epic_file(ofstream& ofs) const
{
	ofs << m_name << endl;

	string hdr = "TICKER        COST       VALUE   RET%         QTY"s +
		"       UCOST      UVALUE"s;
	//strings fields = { pad_ticker("TICK"), pad_gbp("COST"), 
	//	pad_gbp("VALUE"), ret_str("RET%"), 
	//	pad_gbp("QTY"), pad_gbp("UCOST"), pad_gbp("UVALUE") };
	//print_strings(cout, fields);
	ofs << hdr << endl;

	for(const auto& e:reduced_epics)
	{
		ofs << pad_left(e.etran.ticker, 6)
			<< e.etran.cost
			<< e.vto
			<< ret_curr(e.vto, e.etran.cost)
			<< e.etran.qty
			<< e.ucost
			<< e.end_price
			<< endl;
	}

	ofs << pad_right("Grand:", 6) << cost << value 
		<< ret_curr(value, cost) << endl << endl;

	if(m_name != "total") return;
	ofs << "Zeros:\n";
	int i=0;
	for(const auto& z:zeros) {
		ofs << pad_right(z, 6) << " ";
		i++;
		if(i==10) {i = 0; ofs << endl; }
	}
}

void folio_c::print_to_portfolio_file(ofstream& ofs) const
{
	cout << "TODO folio_c::print_to_portfolio_file()\n";
}
void epics_main(const detran_cs& es, const stend_ts& stends)
{

	for(auto& f:g_folios)
		f.calculate(es);
		//folio_etrans = f.filter(es);
		//const folio_calcs fc = process_folio(f, es);
		//calc_map[f.
	//}

	//for(const auto& f:g_folios) 

	//for(auto f:folios){ process_folio(f, keys, es, eout, pout); }
	//set<string> keys;
	//for(auto e:es) { keys.insert(e.etran.ticker);}

	string filename = s3("epics.rep");
	ofstream eout;
	eout.open(filename);
	for(const auto& f:g_folios) f.print_to_epic_file(eout);
	eout.close();
/*
	for(const folio_c& f:g_folios){
		cout << f.m_name << endl;
		const detran_cs fes = f.filter(es);
		const detran_cs reds = reduce(fes);
		cout << "doing subtotal\n";
		currency cost, value;
		//for(const auto& r:reds) { 
		//	if(r.etran.qty.zerop()) continue;
			// ostenbile printing
			//cost+= r.etran.cost;
			//value += r.vto;
		//}
		//detran_c subtotal = reduce_all(reds);
		//reduce(reds);
	//	cout << "done\n";
	}
	*/

	filename = s3("portfolios.rep");
	ofstream pout;
	pout.open(filename);
	strings fields = strings { pad_ticker("FOLIO"), pad_gbp("VBEFORE"), 
		pad_gbp("VFLOW"), pad_gbp("VPROFIT"), 
		pad_gbp("VTO"), ret_str("VRET")};
	print_strings(pout, fields);
	for(const auto& f:g_folios)
		f.print_to_portfolio_file(pout);
	print_indices(stends, pout);
	pout.close();
}

/*
 * 18-Jun-2016 Can't use autosprintf on cygwin
 * */

#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
#include <locale.h>
//#include "autosprintf.h"
#include <string.h>

#include "common.hpp"
#include "parse.hpp"
#include "reusable.hpp"
#include "gaap.hpp"



using namespace std;

void ul1(ostream* ofs) { (*ofs) << nchars(' ', 11) << nchars('-', 10) << endl;};

void emit(ostream* ofs, const string& title, const centis& value)
{
	//char sgn = ' ';
	//if (value<0) { sgn = '-' ; value = -value;}
	//double bal = round2(double(value)/double(100));
	double bal = round2(value.get()/100);
	char sgn = bal<0 ? '-' : ' ';
	bal = fabs(bal);
	setlocale(LC_NUMERIC, "");
	char buf[80];
	snprintf(buf, sizeof(buf), "%10.10s %'10.2f%c", title.c_str() , bal, sgn);
	(*ofs) << buf << endl;


}

// both set by gaap_main()
ostream* m_ofs = nullptr;
nacc_ts m_naccs;

centis get_bal(string key)
{ 
	//centis bal;
	try {
		return m_naccs.at(key).bal;
	} catch (const std::out_of_range& ex) {
		return centis();
	}	

	//return bal;
}	

struct Lie { 
	string desc; 
	centis amount; 
	bool oline = false ; 
	bool uline = false;
}; // line entry
void print_lie(const Lie& lie) 
{ 
	if(lie.oline) ul1(m_ofs);
	emit(m_ofs, lie.desc, lie.amount);
	if(lie.uline) ul1(m_ofs);
}

class section {
	public:
		section(string desc) {
			total_lie.desc = desc;
		       	total_lie.oline = true;
			total_lie.uline = true;
		}
		
		section add(string desc) {
			centis amount = get_bal(desc);
			struct Lie lie = {desc, amount};
			add(lie);
			return *this;
		}

		section add(section s) {
			struct Lie lie {s.total_lie.desc, s.total_lie.amount};
			add(lie);
			return *this;
		}

		section add(Lie lie) {
			lies.push_back(lie);
			//total_lie.amount += lie.amount;
			total_lie.amount.inc(lie.amount);
			return *this;
		}

		section adds(vector<string> descs) {
			for(auto& d:descs) add(d);
			return *this;
		}

		section running(string desc) {
			struct Lie lie = total_lie;
			lie.desc = desc;
			lie.uline = false;
			lies.push_back(lie);
			return *this;
		}

		void print() {
		       	for(auto& lie:lies)  print_lie(lie);
			//ul1(m_ofs);
			print_lie( total_lie);
			(*m_ofs) << endl;
		}
	private:
		struct Lie total_lie; 
		vector<Lie> lies;
};

void subtotal(ostream* ofs, const string& title, centis& value)
{ 
	ul1(ofs); emit(ofs, title, value); 
	ul1(ofs);
	(*ofs) << endl;

}

void write_period(const string& s)
{
	(*m_ofs) << nchars(' ', 11) << s << endl;
}

void gaap_main(const nacc_ts& the_naccs, const period& per)
{
	m_naccs = the_naccs;

	auto get_bal = [the_naccs](auto key){ 
		centis bal;
		try {
			bal.set( the_naccs.at(key).bal);
		} catch (const std::out_of_range& ex) {
			//bal = 0;
		}	
		return bal;
	};	


	string fname;
	s3("gaap-0.rep", fname);
	ofstream ofs;
	ofs.open(fname.c_str(), ofstream::out);
	m_ofs = &ofs;

	write_period(per.start_date);
	write_period(per.end_date);
	ofs << endl;

	section inco = section("Income").adds({"div", "int", "wag"});
	section exps = section("Expenses").adds( {"amz", "car", "chr", "cmp", "hol", "isp", "msc", "mum"});
	section mygains = section("Folio Gain").adds( {"hal_g", "hl_g", "igg_g", "tdi_g", "tdn_g"});
	section balcd = section("Bal c/d").add(inco).add(exps).running("Ord profit")
		.add("tax").add(mygains).add("ut_g").running("Net Profit")
		.add("opn");
	section folio = section("Folio")
		.adds( {"hal_c", "hl_c", "igg_c", "tdi_c", "tdn_c"})
		.running("My shares")
		.add("ut_c");

	section nass = section("Net Assets")
		.adds( {"hal", "hl", "igg", "ut", "rbs", "rbd", "sus", "tdi", "tdn", "tds", "vis"})
		.running("Cash equiv")
		.add(folio)
		.add("msa");

	balcd.print();
	nass.print();
	inco.print();
	exps.print();
	mygains.print();
	folio.print();
	ofs.close();
}

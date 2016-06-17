#include <map>
#include <iostream>
#include "autosprintf.h"

#include "parse.h"
#include "reusable.h"
#include "gaap.hpp"

using namespace std;

void ul1(ostream& ofs) { ofs << nchars(' ', 11) << nchars('-', 10) << endl;};

void emit(ostream& ofs, const string& title, pennies_t value)
{
	char sgn = ' ';
	if (value<0) { sgn = '-' ; value = -value;}
	double bal = round2(double(value)/double(100));
	ofs << gnu::autosprintf("%10.10s %10.2f%c", title.c_str() , bal, sgn);
	ofs << endl;


}

void subtotal(ostream& ofs, const string& title, pennies_t value)
{ 
	ul1(ofs); emit(ofs, title, value); ul1(ofs);
	ofs << endl;

}

void gaap_main(const nacc_ts& the_naccs)
{
	auto get_bal = [the_naccs](auto key){ 
		pennies_t bal = 0;
		try {
			bal = the_naccs.at(key).bal;
		} catch (const std::out_of_range& ex) {
			bal = 0;
			//cerr << "gaap_main() couldn't look up key " << key << " in naccs" << endl;
			//throw ex;
		}	
		return bal;
	};	


	pennies_t inter = get_bal("int");
	pennies_t div   = get_bal("div");
	pennies_t wag   = get_bal("wag");
	pennies_t inc = inter + div + wag;

	pennies_t amz = get_bal("amz");
	pennies_t car = get_bal("car");
	pennies_t chr = get_bal("chr");
	pennies_t cmp = get_bal("cmp");
	pennies_t hol = get_bal("hol");
	pennies_t isp = get_bal("isp");
	pennies_t msc = get_bal("msc");
	pennies_t mum = get_bal("mum");
	pennies_t exp = amz + car + chr + hol +isp + msc + mum;

	pennies_t xinc = inc + exp;


	auto& ofs = cout;

	emit(ofs, "Income", inc);
	emit(ofs, "Expenses", exp);
	ul1(ofs);
	emit(ofs, "Excess Income", xinc);
	pennies_t  tax = get_bal("tax");
	emit(ofs, "Taxes", tax);
	pennies_t ninc = xinc + tax;
	subtotal(ofs, "Net Income", ninc);


	ofs <<endl;

	emit(ofs, "Interest", inter);
	emit(ofs, "Dividends", div);
	emit(ofs, "Wages", wag);
	subtotal(ofs, "Income", inc);

	emit(ofs, "Amazon", amz);
	emit(ofs, "Car", car);
	emit(ofs, "Charity", chr);
	emit(ofs, "Computing", cmp);
	emit(ofs, "Holidays", hol);
	emit(ofs, "ISP", isp);
	emit(ofs, "Misc", msc);
	emit(ofs, "Mum", mum);
	subtotal(ofs, "Expenses", exp);
	emit(ofs, "Taxes", tax);

	emit(ofs, "Amazon", amz);
	emit(ofs, "Amazon", amz);
	emit(ofs, "Amazon", amz);
	emit(ofs, "Amazon", amz);
	emit(ofs, "Amazon", amz);

	
	
}

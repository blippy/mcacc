#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "common.hpp"
#include "autotypes.hpp"
#include "nacc.hpp"
#include "reusable.hpp"

//using std::string;
//using std::vector;
using namespace std;

int posts_main(const etranas_t& es, const period& per, const nacc_ts& the_naccs)
{
	vecvec_t ps1; 

	string fname;
	s1("ntran.dsv", fname);
	auto ns = vecvec(fname);
	for(auto n:ns) {
		if(n.size() != 5) { 
			string msg("posts_main(): Expected 5 arguments in: ");
			msg += "{" + intercalate(", ", n) + "}";
			throw std::range_error(msg); 
		}
		std::string dstamp = n[0];
		if(dstamp > per.end_date) {
			//cout << "Dropping: ";
			//print(n);
			continue;
			assert(false); // we should never get here
		}
		int  amount = enpennies(n[3], true);
		if(amount ==0) continue;
		string amountstr = to_string(amount);

		std::string dr = n[1];		
		std::string cr = n[2];
		if(dstamp < per.start_date) {
			dr = the_naccs.at(dr).alt;
			cr = the_naccs.at(cr).alt;
		}
		std::string desc = n[4];
		ps1.push_back({dr, dstamp, cr, amountstr, desc});
		amountstr = to_string(-amount);
		ps1.push_back({cr, dstamp, dr, amountstr, desc});
	}

	//etranas_t es = load_etranas();
	for(auto e: es) {
		if(e.dstamp > per.end_date) continue;
		string folio = e.folio;
		string sym = e.sym;
		strings p;

		auto push = [ &e = e, &ps1 = ps1](string acc, string str, int amount) {
			strings p;
			if(amount == 0) return;
			p = {acc, e.dstamp, str, to_string(amount), str + ":" + e.sym};
			ps1.push_back(p);
		};

		push(folio, "pCost", -e.cost);
		push(folio + "_c", "pVcd", e.vto);
		push(folio + "_g", "pPdp", -e.profit);
		push("opn", "pbd", -e.prior_year_profit);
	}

	std::sort(ps1.begin(), ps1.end());

	s3("posts.dsv", fname);
	prin_vecvec(ps1, "\t", "\n", fname.c_str());


	return EXIT_SUCCESS;
}

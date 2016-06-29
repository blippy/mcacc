#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "common.hpp"
//#include "etran.hpp"
//#include "nacc.hpp"
#include "inputs.hpp"
#include "reusable.hpp"
#include "posts.hpp"

//using std::string;
//using std::vector;
using namespace std;

//int posts_main(const etranas_t& es, const period& per, const nacc_ts& the_naccs)
int posts_main(const inputs_t& inputs)
{
	//vecvec_t ps1; 

	//string fname;
	//s1("ntran.dsv", fname);
	//auto ns = vecvec(fname);
	/*
	for(auto& n:inputs.ntrans) {
		if(n.size() != 5) { 
			string msg("posts_main(): Expected 5 arguments in: ");
			msg += "{" + intercalate(", ", n) + "}";
			throw std::range_error(msg); 
		}
		std::string dstamp = n[0];
		if(dstamp > inputs.p.end_date) {
			//cout << "Dropping: ";
			//print(n);
			continue;
			assert(false); // we should never get here
		}
		int  amount = enpennies(n[3], true);
		if(amount ==0) continue;
		string amountstr = to_string(amount);

		std::string dr = n.dr;		
		std::string cr = n.cr;
		if(dstamp < inputs.p.start_date) {
			dr = inputs.naccs.at(dr).alt;
			cr = inputs.naccs.at(cr).alt;
		}
		std::string desc = n[4];
		ps1.push_back({dr, dstamp, cr, amountstr, desc});
		amountstr = to_string(-amount);
		ps1.push_back({cr, dstamp, dr, amountstr, desc});
	}
	*/

	post_ts ps;
	for(auto& n:inputs.ntrans) {
		if(n.dstamp > inputs.p.end_date) continue;
		if(n.amount ==0) continue;
		//string amountstr = to_string(amount);

		post_t p;
		p.dstamp = n.dstamp;
		p.dr = n.dr;
		p.cr = n.cr;
		if(dstamp < inputs.p.start_date) {
			p.dr = inputs.naccs.at(dr).alt;
			p.cr = inputs.naccs.at(cr).alt;
		}
		p.desc = n.desc;
		ps.push_back(p);
		//ps1.push_back({dr, dstamp, cr, amountstr, desc});
		//amountstr = to_string(-amount);
		//ps1.push_back({cr, dstamp, dr, amountstr, desc});
		p.amount = -p.amount;
		ps.push_back(p);
	}
	//etranas_t es = load_etranas();
	for(auto& e: inputs.etrans) {
		if(e.dstamp > per.end_date) continue;
		//string folio = e.folio;
		//string sym = e.ticker;
		//post p;

		auto push = [ &e = e, &ps = ps](string acc, string str, int amount) {
			post_t p;
			if(amount == 0) return;
			p = {acc, e.dstamp, str, to_string(amount), str + ":" + e.ticker};
			ps.push_back(p);
		};

		push(folio, "pCost", -e.cost);
		push(folio + "_c", "pVcd", e.vto);
		push(folio + "_g", "pPdp", -e.profit);
		push("opn", "pbd", -e.prior_year_profit);
	}

	//std::sort(ps1.begin(), ps1.end());
	//Shouldn't need sorting, as that is already done by inputs

	//s3("posts.dsv", fname);
	//prin_vecvec(ps1, "\t", "\n", fname.c_str());


	return EXIT_SUCCESS;
}

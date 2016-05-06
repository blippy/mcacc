#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "common.h"
#include "types.h"

using std::string;
using std::vector;

int posts_main()
{
	vecvec_t ps1; 

	string fname;
	s1("ntran.dsv", fname);
	auto ns = vecvec(fname);
	for(auto n:ns) {
		int  amount = enpennies(n[3], true);
		if(amount ==0) continue;
		string amountstr = to_string(amount);
		ps1.push_back({n[1], n[0], n[2], amountstr, n[4], n[5]});
		amountstr = to_string(-amount);
		ps1.push_back({n[2], n[0], n[1], amountstr, n[4], n[5]});
	}

	//s3("etrans-aug.dsv", fname);
	etranas_t es = load_etranas();
	for(auto e: es) {
		string folio = e.folio;
		string sym = e.sym;
		//vector<string> p;
		strings p;

		auto push = [ &e = e, &ps1 = ps1](string acc, string str, int amount) {
			strings p;
			if(amount == 0) return;
			p = {acc, e.dstamp, str, to_string(amount),
				"C", str + ":" + e.sym};
			ps1.push_back(p);
		};

		//string amount = to_string(-e.cost);
		//p = {folio, e.dstamp, "pCost", amount, "C", "pCost:", sym};
		//ps1.push_back(p);
		push(folio, "pCost", -e.cost);

		//amount = to_string(e.to);
		//p = {folio + "_c", e.dstamp, "pVcd", amount, "C", "pVcd:", sym};
		//ps1.push_back(p);
		push(folio + "_c", "pVcd", e.vto);
		push(folio + "_g", "pPdp", -e.profit);
		push("opn", "pbd", -e.prior_year_profit);

		//amount = to_string(e.p

	}

	std::sort(ps1.begin(), ps1.end());

	s3("posts.dsv", fname);
	prin_vecvec(ps1, "\t", "\n", fname.c_str());


	return EXIT_SUCCESS;
}

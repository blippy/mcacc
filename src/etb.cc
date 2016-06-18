/*
IN: posts.dsv
OUT: accs.rep etb.rep
*/

#include <set>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

#include <boost/format.hpp>


#include "common.hpp"
#include "parse.hpp"
#include "types.hpp"

using namespace std;

int etb_main(nacc_ts& the_naccs)
{

	//reset the account balances
	for(auto& n:the_naccs) n.second.bal = 0;

	std::string fname;
	s3("posts.dsv", fname);
        vecvec_t posts = vecvec(fname);
	int total;

	ofstream aout, eout;
	s3("accs.rep", fname);
	aout.open(fname);
	s3("etb.dsv", fname);
	eout.open(fname);

	set<string> keys;
	for(auto p:posts) {keys.insert(p[0]); }

	for(auto k: keys) {
		total = 0;

		for(auto p:posts){
			if(p[0] != k) continue;
			// normal case
			aout << setw(7) << p[0];
			aout << setw(11) << p[1];
			aout << setw(7) << p[2];
			aout << setw(30) << p[4];
			int pennies = stoi(p[3]);
			aout << fixed << setw(12) << setprecision(2) << (double(pennies) /100);
			total += pennies;
			aout << setw(12) << setprecision(2) << (double(total)/100);
			aout << endl;
		}

		aout << endl;

		if(the_naccs.count(k) == 0) {
			string msg =  "etb_main() couldn't look up key "s + k + " in naccs";
			throw std::out_of_range(msg);
		}

		nacc_t& a_nacc = the_naccs.at(k);

		double scale = a_nacc.scale;
		eout << boost::format("%-6.6s %12.2f %2.0f %+010d\n") % k % (double(total)/100) % scale % total;
		a_nacc.bal = total;

	}
	eout.close();
	aout.close();

}

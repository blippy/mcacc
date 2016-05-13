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

#include "common.h"
#include "parse.h"
#include "types.h"

msvs_t mkmap(const char *filename, int k)
{
	msvs_t m;
	vecvec_t vvs = vecvec(filename);
	for(auto vs : vvs) m[vs[k]] = vs;
	return m;
}
msvs_t mkmap(std::string &filename, int k) { return mkmap(filename.c_str(), k); }

void prin_msvs(msvs_t &m)
{
	std::cout << "prin_msvs:" ;
	for(auto it = m.begin(); it != m.end(); ++it) {
		std::cout << ".";
		std::cout << it->first;
		std::cout << (it->second)[0];
	}
}

int etb_main()
{
	std::string start_date, end_date;
        get_period(start_date, end_date);

	std::string fname;
	s3("posts.dsv", fname);
        vecvec_t posts = vecvec(fname);
	int total;

	s1("nacc.dsv", fname);
	msvs_t nacc_map = mkmap(fname, 0);

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

			eout << k << "\t" ;
			eout << fixed <<  setprecision(2) << (double(total) /100);
			strings n;
			try {
				n = nacc_map.at(k);
			} catch (std::out_of_range& err) {
				cerr << "etb_main() couldn't look up key " << k << " in naccs" << endl;
				throw err;
			}

			eout << "\t" << n.at(3);
			eout << endl;
	}
	eout.close();
	aout.close();

}

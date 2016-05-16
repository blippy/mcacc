#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unordered_map>

#include "autotypes.h"
#include "common.h"
#include "parse.h"
#include "types.h"


string to_intstring(double d) { return to_string(int(d)); }

// push back double as integer string 
void pbdais(vector<string> &v, double d)
{
	v.push_back(to_intstring(d));
}

int eaug_main(const period &per)
{
	string fname;
	//string start_date, end_date;
	//get_period(start_date, end_date);


	stends_t stends = load_stends();

	s1("etran.dsv", fname);
	vecvec_t etrans = vecvec(fname);
	for(vs_t &e: etrans) {
		//string ticker = cmap.at(e[4]);
		string ticker = e[4];
		stend_c s;
	       	try {s = stends.at(ticker);}
		catch (const std::out_of_range& oor) {		 
			cerr << "WARN: Creating a fake stend for " 
				<< ticker << endl;
			auto v = strings { e[4], "X" , "0", "X", "0" };
			s.from_vec(v);
		}

		double unit = stod(e[6])*100.0/stof(e[5]);
		e.push_back(to_string(unit));
		//e.insert(e.end(), s.begin(), s.end());
		e.push_back(s.ticker);
		e.push_back(s.start_dstamp);
		e.push_back(to_string(s.start_price));
		e.push_back(s.end_dstamp);
		e.push_back(to_string(s.end_price));

		double sgn = e[2] == "B" ? 1 : -1;
		double qty = sgn * stod(e[5]);
		e[5] = to_string(qty); // adjust for sign change
		string dstamp = e[1];

		double cost = bround(100.0* sgn * stod(e[6]));
		e[6] = to_intstring(cost); // replace it with signed pennies

		/*
		double vflow = (dstamp >= start_date && dstamp <= end_date) ?
			cost : 0;
		double vbefore =0, profit_bd = 0;
		if(dstamp < start_date) {
			vbefore = bround(qty * s.start_price);
			profit_bd = vbefore - cost;
		}

		double vto = (dstamp <= end_date) ?  bround(qty * s.end_price) : 0;
		*/
	
		double vbefore = 0, vflow = 0, profit_bd = 0;
		double vto = bround(qty * s.end_price);
		switch(per.when(dstamp)) {
			case perBefore:
				vbefore = bround(qty * s.start_price);
				profit_bd = vbefore - cost;
				break;
			case perDuring:
				vflow = cost;
				break;
			case perAfter:
				vto = 0;
		}




		double vprofit = vto - vbefore - vflow;


		pbdais(e, profit_bd);
		pbdais(e, vbefore);
		pbdais(e, vflow);
		pbdais(e, vprofit);
		pbdais(e, vto);
	
		
	}

	s3("etrans-aug.dsv", fname);
	prin_vecvec(etrans, "\t", "\n", fname.c_str());

	// output in rec format
	s3("etrans-aug.rec", fname);
	std::ofstream ofs;
	ofs.open(fname, std::ofstream::out);
	for(auto e:etrans) { 
		for(int i=0 ; i < etrana_fieldnames.size(); i++) {
			ofs << etrana_fieldnames[i] << ": " ;
			ofs << e[i] << endl;
		}
		ofs << endl;
	}
	
	return EXIT_SUCCESS;
}


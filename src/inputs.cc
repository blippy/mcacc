#include <iostream>
#include <string>
#include <stdlib.h>

#include "common.hpp"
#include "reusable.hpp"
#include "inputs.hpp"
#include "parse.hpp"

using namespace std;

void insert_nacc(nacc_ts& ns, const strings& fields)
{	
	nacc_t n;
	n.acc = fields[2];
	n.alt = fields[3];
	n.typ = fields[4];
	n.scale = stod(fields[5]);
	n.desc = fields[5];
	ns[n.acc] = n;
}


void insert_comm(comm_ts& cs, const strings& fields)
{
	comm_t c;
	c.ticker = fields[2];
	c.down =fields[3];
	c.typ =fields[4];
	c.unit = fields[5];
	c.desc = fields[6];
	cs[c.ticker] = c;
}

etran_t mketran(const strings& fields)
{
	etran_t e;
	e.taxable = fields[8] == "T";
	e.dstamp = fields[1];
	e.buy = fields[7] == "B";
	e.folio = fields[2];
	e.ticker = fields[3];
	e.qty= stod(fields[5]);
	e.cost= stod(fields[6]);
	//e.ticker= fields[3];
	return e;
}

void TODO() { cout << "TODO in inputs.cc\n" ; }

void insert_LVL03(inputs_t& inputs, const strings& fields)
{
	string subtype = fields[4];
	if(subtype == "ETRAN-1") { 
		inputs.etrans.push_back(mketran(fields));
	} else if (subtype == "LEAK-1") {
		TODO();
	} else if (subtype == "NTRAN-1") {
		TODO();
	} else {
		cerr << "inputs.cc:read_inputs()/LVL03 couldn't understand type ";
		cerr << subtype << ". Fatal exit." << endl;
		exit(EXIT_FAILURE);
	}
}


inputs_t read_inputs()
{
	inputs_t inputs;

	string fname;
	s1("derive-2", fname);
	vecvec_t mat = vecvec(fname);

	for(auto& row:mat) {
		string cmd = row[0]; // => LVL0?
		string level_str = cmd.substr(3, 2);
		int level =stoi(level_str);
		switch(level) {
			case 0:
				// these are just notes, so we can ignore them
				break;
			case 1:
				insert_nacc(inputs.naccs, row);
				break;
			case 2:
				insert_comm(inputs.comms, row);
				break;
			case 3:
				insert_LVL03(inputs, row);
				break;
			case 4:
				break;
			case 5:
				break;
			default:
				cerr << "Unhandled level number " << level << " in inputs.cc/read_inputs()\n";
				exit(EXIT_FAILURE);
		}
		// TODO NOW
	}
	return inputs;

}

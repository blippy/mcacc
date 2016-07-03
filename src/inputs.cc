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
	e.sgn = fields[7] =="B"? 1 : -1;
	e.buy = e.sgn == 1;
	e.folio = fields[2];
	e.ticker = fields[3];
	e.qty= e.sgn * stod(fields[5]);
	e.cost= e.sgn * enpennies(fields[6]);
	e.regular = true;
	//e.ticker= fields[3];
	return e;
}

etran_t mkleak_1(const strings& fields)
{
	// TODO LEAK-1 should avaoid making a stend, which I think it does automatically
	etran_t e = mketran(fields);
	e.buy = false;;
	e.regular = false;
	return e;
}

ntran_t mkntran(const strings& fields)
{
	ntran_t n;
	n.dstamp=fields[1];
	n.dr=fields[2];
	n.cr=fields[3];
	n.amount=enpennies(fields[6]);
	n.desc=fields[9];
	return n;
}
void TODO() { cout << "TODO in inputs.cc\n" ; }

void insert_LVL03(inputs_t& inputs, const strings& fields)
{
	string subtype = fields[4];
	if(subtype == "ETRAN-1") { 
		inputs.etrans.push_back(mketran(fields));
	} else if (subtype == "LEAK-1") {
		inputs.etrans.push_back(mkleak_1(fields));
	} else if (subtype == "NTRAN-1") {
		inputs.ntrans.push_back(mkntran(fields));
	} else {
		cerr << "inputs.cc:read_inputs()/LVL03 couldn't understand type ";
		cerr << subtype << ". Fatal exit." << endl;
		exit(EXIT_FAILURE);
	}
}

void insert_yahoo(inputs_t& inputs, const strings& fields)
{
	yahoo_t y;
	y.dstamp = fields[2];
	y.tstamp = fields[3];
	y.ticker = fields[4];
	y.rox = stod(fields[5]);
	y.price = stod(fields[6]);
	y.chg = stod(fields[7]);
	y.chgpc = stod(fields[8]);
	y.currency = fields[9];
	y.desc = fields[10];

	auto& ys = inputs.yahoos;
	//inputs.yahoos.emplace(y.ticker, y);
	if(ys.find(y.ticker) == ys.end())
		ys[y.ticker] = vector<yahoo_t> {};
	ys[y.ticker].push_back(y);
}


void set_period(inputs_t& inputs, const strings& fields)
{
	inputs.p.start_date = fields[2];
	inputs.p.end_date = fields[3];
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
			case 0: //LVL00
				// these are just notes, so we can ignore them
				break;
			case 1: // LVL01
				insert_nacc(inputs.naccs, row);
				break;
			case 2: // LVL02
				insert_comm(inputs.comms, row);
				break;
			case 3: // LVL03
				insert_LVL03(inputs, row);
				break;
			case 4: // LVL04
				set_period(inputs, row);
				break;
			case 5: // LVL05
				insert_yahoo(inputs, row);
				break;
			default:
				cerr << "Unhandled level number " << level << " in inputs.cc/read_inputs()\n";
				exit(EXIT_FAILURE);
		}
	}
	return inputs;

}

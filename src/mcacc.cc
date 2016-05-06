#include <fstream>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <string>
//#include <tuple>
#include <vector>

#include "common.h"
#include "parse.h"
#include "types.h"

using std::cerr;
using std::cout;
using std::endl;
using std::function;
using std::string;
//using std::tuple;
using std::vector;



int dsv_extract()
{
	string input;
	while(getline(cin, input)) {
		vector<string> output = tokenize_line(input);
		if(output.size() == 0) continue;
		string fname = output[0];
		fname  += ".dsv";
		fstream fs;
		ios_base::openmode mode =  fstream::out | fstream::app;
		fs.open(fname.c_str(), mode); // user has to pre-delete the file
		for(int i = 1; i < output.size() -1; i++) { fs << output[i] << "\t"; }
		fs << output.back() << endl ;
		fs.close();
		//cout << "In: " << trimmed << endl;
	}
	return EXIT_SUCCESS;
}



void cgt()
{
	cout << "mcacc cgt" << endl;

	// TODO allow possibility of passing in
	etranas_t es = load_etranas(); 
	string start_date, end_date;
	get_period(start_date, end_date);
	//cout << start_date << endl;
	
	std::set<string> tickers;
	for(auto e: es) {
		bool during =  start_date  <= e.dstamp && e.dstamp <=  end_date; // TODO promote to etrana
		//if(during && ! e.buy) cout << "+ " << e.ticker << " " << e.qty << endl;
		if(e.taxable && during && ! e.buy) tickers.insert(e.ticker);
	}

	for(auto & t: tickers) {
		cout << t << endl; // TODO it's here that we need to do something with the hit tickers
	}
	cout << "TODO: cgt - pending something interesting to report" << endl;
}

// command dispatch table
// TODO why not a mpa?
typedef struct dte { string cmd ; function<void()> fn ; } dte;
const auto ditab = vector<dte> {
	{"cgt" , cgt },
	{"dsv", dsv_extract},
	{"eaug", eaug_main},
	{"epics", epics_main},
	{"etb", etb_main},
	{"posts", posts_main},
	{"stend", stend_main},
	{"yproc", yproc_main}
};

void dispatch(string cmd)
{
	for(auto d : ditab) {
		if(d.cmd == cmd) { d.fn() ; return ; }
	}

	cerr << "Command not understood: " << cmd << endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{

	if(argc>1) {
		string cmd;
	       	cmd = argv[1];
		//cout << "Command is: " << cmd << endl;
		dispatch(cmd);

	} else {
		cout << "No command specified" << endl;
	}
	return EXIT_SUCCESS;
}

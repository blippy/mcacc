#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <string>
//#include <tuple>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

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



/*
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <vector>

#include "common.h"
#include "parse.h"

using namespace std;
*/

/*
void info(string msg)
{
	printf("INFO: mca-yproc: ");
	cout <<  msg;
}
*/

// TODO promote
bool file_exists(string filename)
{
	return ( access( filename.c_str(), F_OK ) != -1 );
}
// TODO promote to parse.cc
string slurp(const char *filename)
{
	ifstream in;
	in.open(filename, ifstream::in);
	stringstream sstr;
	sstr << in.rdbuf();
	in.close();
    	return sstr.str();
}


vecvec_t commasep(string  &filename)
{
        vecvec_t res;
        ifstream fin;
        string line;
        fin.open(filename.c_str(), ifstream::in);
        while(getline(fin, line)) {
		for(auto it = line.begin(); it != line.end(); ++it) {
			if(*it == ',') *it = ' '; }
                vector<string> fields = tokenize_line(line);
                if(fields.size() >0) res.push_back(fields);
        }
        return res;
}

bool yorder (vector<string> a, vector<string> b) { return a[0] < b[0];}
int yproc_main() 
{

/*
IN: s1/comm.dsv s2/dstamp s2/ystamp s2/yfetch.csv s2/usd.csv
OUT: s3/yrpoc.dsv ~/.mca/yahoo/<YYYY-MM-DD>.txt

NB Further processing will pick up the generated yahoo file from the cache
(in ~/.mca), but it is still useful to have s3/yproc.dsv for creating
a snapshot of gains
*/
	string fname;

	s1("comm.dsv", fname);
	vecvec_t comms = commasep(fname);
	set<string> usds; // those comms which are in USD
	for(auto c : comms) { 
		//cout << c[1];
		if(c[3] =="USD") {
			string ticker = c[4];
			//cout << ticker <<endl;
			usds.insert(ticker);
		}
	}


	s2("usd.csv", fname); 
	if(! file_exists(fname)) {
		cout << "INFO: yproc_main(): " 
			<< "File s2/usd.csv does not exist. Quitting\n";
		return EXIT_SUCCESS;
	}
	vecvec_t usdv = commasep(fname);
	double rox = 100.0 * stod(usdv[0][1]);
	//cout << rox;


	s2("dstamp", fname);
	string dstamp = slurp(fname.c_str());
	//cout << "*" << dstamp << "*" << endl;
	s2("tstamp", fname);
	string tstamp = slurp(fname.c_str());
	//cout << "*" << tstamp << "*" << endl;
	string dtstamp = dstamp + "\t" + tstamp;
	
	s2("yfetch.csv", fname);
	vecvec_t yfetch = commasep(fname);

	//cout << yout_name << endl;
	ofstream yout;
	s3("yproc.dsv", fname);
	yout.open(fname);
	//prin_vecvec1(yfetch);
	sort(yfetch.begin(), yfetch.end(), yorder);
	//prin_vecvec1(yfetch);
	for(auto y: yfetch) {
		yout << "yahoo " << dtstamp << "\t" ;
		string ticker = y[0];
		yout << ticker << "\t" ;
		string rox_str = "1.0000";
		double price = stod(y[1]);
		double chg   = stod(y[2]);
		if(usds.find(ticker) != usds.end()) {
			rox_str = to_string(rox);
			price *= rox;
			chg *= rox;
		}
		yout << rox_str << "\t" << to_string(price) << "\t" ;;
		yout << chg << "\t" ;

		// elminate the + and % from chgpc
		string chgpc ; // =y[3];
		char chars[] = {'+', '%' };
		for(auto c : y[3]) if(c != '+' && c!= '%') chgpc += c;
		yout << chgpc << "\tP" ;
	
		yout << endl;
	}
	yout.close();

	string yout_name = "/home/mcarter/.mca/yahoo/" + dstamp + ".txt";
	string cmd = string("cp /home/mcarter/.mca/work/s3/yproc.dsv ") + yout_name;
	system(cmd.c_str());

	return EXIT_SUCCESS;
}

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
// TODO why not a map?
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

/*
IN: s1/comm.dsv s2/dstamp s2/ystamp s2/yfetch.csv s2/usd.csv
OUT: s3/yrpoc.dsv ~/.mca/yahoo/<YYYY-MM-DD>.txt

NB Further processing will pick up the generated yahoo file from the cache
(in ~/.mca), but it is still useful to have s3/yproc.dsv for creating
a snapshot of gains
*/

//#include <pstream.h>

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <algorithm>
#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <sstream>

#include "common.h"
#include "parse.h"

using namespace std;

void info(string msg)
{
	printf("INFO: mca-yproc: ");
	cout <<  msg;
}

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
		info("File s2/usd.csv does not exist. Quitting\n");
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

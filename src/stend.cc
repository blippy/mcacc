/* calculate the start and end prices for yahoo
created 16-Feb-2016

IN: s1/yahoo.dsv (any order)
OUT: s3/stend.dsv

*/

//#include <filesystem>
#include <algorithm>
#include <stdlib.h>
#include <fstream>
#include <iostream> // for ifstream
//#include <string>
#include <unordered_map>
#include <assert.h>
//#include <glob.h>
#include <dirent.h>
#include <set>

#include "common.h"
#include "parse.h"
#include "types.h"

//namespace fs = std::filesystem;

using namespace std;

#define SEP '\t'

void scan_cache(vecvec_t &yahoos)
{

	string ydir = "/home/mcarter/.mca/yahoo";
	auto dirp = opendir(ydir.c_str());
	struct dirent *dp;
	set<string> filenames;
	while(dp = readdir(dirp)) {
		if(dp == NULL) break;
		string dname = dp->d_name;
		if(dname == "." || dname == "..") continue;
		int pos = dname.size() -4 ;
		if(pos <0) continue;
		//if(dname.size() >=4 && dname.stubstr(dname
		//cout << dname.substr(pos) << endl;
		filenames.insert(dname);
		//cout << dp->d_name << endl;
	}
	closedir(dirp);


//	for(auto& p: fs::directory_iterator("/home/mcarter/.mca/yahoo")) {
//		std::cout << pp << '\n';
//	}

	//exit(0);

//	glob_t globbuf;
//	char **p;
//	int i = glob("~/.mca/yahoo/*.txt", GLOB_TILDE, NULL, &globbuf);
//	if(i== GLOB_NOMATCH) goto cleanup;
//	assert(i ==0);


//	p = globbuf.gl_pathv;
	for(auto& f: filenames) {
//	while(*p) {
//		vecvec_t vecs = vecvec(*p);
		string full = ydir + "/" + f;
		vecvec_t vecs = vecvec( full);		
		for(auto &v : vecs) { v.erase(v.begin(), v.begin()+1); }
		yahoos.insert(yahoos.end(), vecs.begin(), vecs.end());
//		p++;
	}
//cleanup:
//	globfree(&globbuf);

}

void scan_inputs(vecvec_t &yahoos)
{
	string fname;
	s1("yahoo.dsv", fname);
	vecvec_t vecs = vecvec(fname);
	yahoos.insert(yahoos.end(), vecs.begin(), vecs.end());


	s1("etran.dsv", fname);
	vecvec_t etrans = vecvec(fname);
	for(auto e: etrans) {
		string dstamp = e[1];
		string tstamp = "03:00:00";
		double qty  = stod(e[5]);
		double cost = stod(e[6]);
		double price = cost/qty *100;
		string pstr = format_num(price, 4);
		//string ticker = cmap[e[4]];
		string ticker = e[4];
		vector<string> v = {dstamp, tstamp, ticker, "1.0000", pstr, "0", "0", "P"};
		yahoos.push_back(v);
	}
	
}

bool ysorter(vector<string> a, vector<string> b)
{
	//return (a[0] < b[0]); // && a[1] < b[1] && a[2] < b[2]);
	if(a[0] != b[0]) return a[0] < b[0];
	if(a[1] != b[1]) return a[1] < b[1];
	return a[2] < b[2];
}

void reorder(vecvec_t &yahoos)
{
	// ticker belongs at the beginning
	for(auto &y :yahoos) {
		//auto y0 = y;
		string ticker = y[2];
		y.erase( y.begin()+2, y.begin()+3);
		y.insert(y.begin(), ticker);
	}

	sort(yahoos.begin(), yahoos.end(), ysorter);
}
	
void output(vecvec_t tbl)
{

	ofstream fout;
	string fname;
	s3("stend.dsv", fname);
	fout.open(fname);
	std::string line;

	string start_date, end_date;
	get_period(start_date, end_date); 
	//cout << "Start: " << start_date << ", End: " << end_date << endl ;
	string yr0  = "0000-00-00";
	string p0  = "0.00";
	string start_dstamp = yr0;
	string start_price  = p0;
	string end_dstamp = yr0;
	string end_price = p0;
	string cur_sym = "";

	//vecvec_t &tbl = yahoos;
	for(int i =0 ; i< tbl.size(); i++) {
		if(i==0 || tbl[i-1][0] != tbl[i][0]) {
			//begin group
			cur_sym = tbl[i][0];
			//cout << "start " << cur_sym << endl ;
			start_dstamp = yr0;
			start_price = p0;
			end_dstamp = yr0;
			end_price  = p0;	
		}

		// normal
		string dstamp = tbl[i][1];
		string price  = tbl[i][4];
		if(dstamp < start_date) { start_dstamp = dstamp ; start_price = price;}
		if(dstamp <= end_date) { end_dstamp = dstamp ; end_price = price;}	

		// end of group
		if(i +1 == tbl.size() || tbl[i+1][0] != tbl[i][0]) {
			fout << cur_sym << SEP ;
			fout << start_dstamp << SEP << start_price << SEP;
			fout  << end_dstamp << SEP << end_price << endl ;
		}
		
	}

	fout.close();
}

int stend_main()
{
	//cout << "TODO: stend: utilise etran-generated prices" << endl;
	vecvec_t yahoos;
	scan_cache(yahoos);
	scan_inputs(yahoos);
	reorder(yahoos);
	//prin_vecvec(yahoos);
	output(yahoos);
	return EXIT_SUCCESS;
}

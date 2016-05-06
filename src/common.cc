#include <assert.h>
#include <string>

#include "common.h"

// create a filename that is based in the stage N directory
void sn(int n, const char *name, string &outname) 
{ 
	outname = "/home/mcarter/.mca/work/s";
	outname += to_string(n) + "/" + string(name);
	//return outname;
}

void s1(const char *name, string &outname) { sn(1, name, outname);}
void s2(const char *name, string &outname) { sn(2, name, outname);}
void s3(const char *name, string &outname) { sn(3, name, outname);}

typedef map<string, vector<string> > msvs_t;


vecvec_t read_registered_dsv(dsv_e dsv)
{
	assert(dsv == etransa); // enhance via some kind of lookup table
	std::string filename;
        s3("etrans-aug.dsv", filename);

	vecvec_t rows = vecvec(filename);
	return rows;
}

string interspersing(string inter, vector<string> strings)
{
	string res = "";
	for(auto it = strings.begin(); it != strings.end(); it++) {
		res += *it;
		if(it< strings.end()-1) res += inter;
	}
	return res;
}

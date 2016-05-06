/* separate out inputs into dsv files
created 19-Feb-2016
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <memory>
//#include <set>
//#include <vector>

#include "common.h"
#include "parse.h"

using namespace std;


const char white[] = " \t\r";

string trim(string& str)
{
    if(str.length() ==0) { return str;}
    size_t first = str.find_first_not_of(white);
    if(first == string::npos) return "";
    size_t last = str.find_last_not_of(white);
    return str.substr(first, (last-first+1));
}

vector<string> tokenize_line(string &input) //, const char sep= ' ')
{
	vector<string> output;
	string trimmed = input;
	string token;
	size_t first;
	int i;
loop:
	trimmed = trim(trimmed);
	if(trimmed.length() == 0) goto fin;
	//cout << "len = " << trimmed.length() << endl;
	if(trimmed[0] == '#') goto fin;
	if(trimmed[0] == '"') {
		token = "";
		for(i=1; i<trimmed.length(); i ++) {
			switch (trimmed[i]) {
			case '"':
				output.push_back(token);
				trimmed = trimmed.substr(i+1);
				goto loop;
			case ' ':
				token += '_' ; //167 ; // 26; // so that fields work
				break;
			default :
				token += trimmed[i];
			}
		}
		// hit eol without enclosing "
		output.push_back(token);
		goto fin;
	} else { // normal case
		first = trimmed.find_first_of(white);
		if(first == string::npos) {
			output.push_back(trimmed);
			goto fin;
		}
		token = trimmed.substr(0, first);
		output.push_back(token);
		trimmed = trimmed.substr(first+1);
		goto loop;
	}
	assert(false); // we should never get here


fin:
	return output;
}


void get_period(string &start, string &end)
{
	string fname;
	s1("period.dsv", fname);
	//fin.open("s1/period.dsv", ifstream::in);
	string line;

	start = "0000-01-01";
	end   = "3000-12-31";
	ifstream fin;
	fin.open(fname, ifstream::in);
	while(getline(fin, line)) {
		vector<string> fields = tokenize_line(line);
		start = fields[0];
		end = fields[1];
	}
	fin.close();

	//cerr << start << " " << end << endl ;
}



vecvec_t vecvec(string  &filename)
{
	ifstream fin;
	fin.open(filename.c_str(), ifstream::in);
	auto res  = vecvec(fin);
	fin.close();
	return res;
}

vecvec_t vecvec(istream  &istr)
{
	vecvec_t res;
	string line;
	while(getline(istr, line)) {
		vector<string> fields = tokenize_line(line);
		if(fields.size() >0) res.push_back(fields);
	}
	return res;
}


void prin_vecvec(vecvec_t & vvs, const char *sep, const char *recsep, const char *filename )
{
	
	std::ofstream ofs;
	bool use_file = strlen(filename);
	if(use_file) ofs.open(filename, std::ofstream::out);
	ostream &os = use_file ? ofs : cout ;

	string ssep = string(sep);
	int i;
	for(i=0; i< vvs.size(); i++) {
		vector<string> v = vvs[i];
		int j, len;
		len = v.size();
		if(len == 0) continue;
		for(j=0; j<len; j++) {
			os << v[j];
			if(j+1<len) os << ssep;
		}
		if(len>0) os << recsep ;
	}

	if(use_file) ofs.close();
}


void prin_vecvec1(vecvec_t &vv)
{
	prin_vecvec(vv, "\n", "\n", "");
}
vecvec_t vecvec(const char *fname)
{
	string fn = (fname);
	return vecvec(fn);
}



double bround(double x) 
{
  double c ,f, det;
  c = ceil(x);
  f = floor(x);

  // eliminate strange artifacts of creating "negative 0"
  if(c == 0) c = 0.0;
  if(f == 0) f = 0.0;
  // printf("c: %f ", c);


  det = c + f - 2.0*x;
  if (det < 0) {return c;}
  if (det > 0) {return f;}

  /* banker's tie */
 if(2*ceil(c/2) == c) {return c;} else {return f;};

}

double round2(double x) { return bround(x*100)/100; }


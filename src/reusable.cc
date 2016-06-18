#include <fstream>
#include <sstream>
#include <unistd.h>

#include "reusable.hpp"


using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::string;

bool file_exists(const string& filename)
{
	return ( access( filename.c_str(), F_OK ) != -1 );
}

string slurp(const char *filename)
{
	ifstream in;
	in.open(filename, ifstream::in);
	stringstream sstr;
	sstr << in.rdbuf();
	in.close();
    	return sstr.str();
}

void spit(const char *filename, const char *content)
{
	ofstream out;
	out.open(filename, ofstream::trunc);
	out << content;
	out.close();

}


void spit(const std::string& filename, const std::string& content)
{ 
	spit(filename.c_str(), content.c_str());
}

string nchars(char c, int num) 
{
	string res = "";
	for(int i=0; i<num; i++) res += c;
	return res;
}
// pad right to n characters
string pad_right(string s, int n) { return s + nchars(' ', n - s.size()); }

string pad_left(string s, int n) { return nchars(' ', n-s.size()) + s; }

/** format a double N as a string to DP decimal places
 Includes required are #include <sstream> [sic]
 * */
string format_num(double n, int dp)
{
	std::ostringstream s;
	s.precision(dp);
	s << std::fixed;
	s <<  n;
	return s.str();
}

std::string intercalate(std::string inter, std::vector<std::string> strings)
{
	string res = "";
	for(auto it = strings.begin(); it != strings.end(); it++) {
		res += *it;
		if(it< strings.end()-1) res += inter;
	}
	return res;
}

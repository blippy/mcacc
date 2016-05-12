#include <fstream>
#include <sstream>
#include <unistd.h>

#include "reusable.h"


using std::ifstream;
using std::stringstream;

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

string nchars(char c, int num) 
{
	string res = "";
	for(int i=0; i<num; i++) res += c;
	return res;
}
// pad right to n characters
string pad_right(string s, int n) { return s + nchars(' ', n - s.size()); }
/*{
	string res = s;
	for(int i = 0; i < n-s.size(); i++) res += " ";
	return res;
}*/

string pad_left(string s, int n) { return nchars(' ', n-s.size()) + s; }
/*{
	string res = "";
	for(int i=0; i< n- s.size(); i++) { res += " "; }
	res += s;
	return res;
}*/
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


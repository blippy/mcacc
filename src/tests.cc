#include <iostream>
#include <string>

using namespace std;
#include "reusable.hpp"

void check(bool ok, string msg)
{
	string s = ok? "PASS" : "FAIL";
	cout << s << " " << msg << endl;
}


void check_erase_all(string src, char c, string targ)
{
	string s = src;
	erase_all(s, c);
	string cstr;
       	cstr	= c;
	string msg = "erase_all() " + cstr + " in " + src;
	//msg +=	c + " in " + src;
	check(s == targ, msg);
}

void run_all_tests()
{
	check_erase_all("", 'a', "");
	check_erase_all("aaa", 'a', "");
	check_erase_all("alibaba", 'a', "libb");
	check_erase_all("baad", 'a', "bd");
}

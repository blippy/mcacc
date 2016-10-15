#include <cmath>
#include <iostream>
#include <string>

//using namespace std;
#include "cpq.hpp"
#include "inputs.hpp"
#include "reusable.hpp"

using std::cout;
using std::string;

void check(bool ok, std::string msg)
{
	std::string s = ok? "PASS" : "FAIL";
	std::cout << s << " " << msg << std::endl;
}


void check_near(double src, double targ, string msg)
{
	check(fabs(src-targ) <0.0001, msg);
}
void check_erase_all(std::string src, char c, std::string targ)
{
	std::string s = src;
	erase_all(s, c);
	std::string cstr;
       	cstr	= c;
	std::string msg = "erase_all() " + cstr + " in " + src;
	//msg +=	c + " in " + src;
	check(s == targ, msg);
}

void check_tokeniser(std::string str, std::vector<std::string> strs)
{
	std::vector<std::string> result = parse::tokenise_line(str);
	std::string msg = "tokenise_line() <" + str + ">";

	bool ok = strs.size() == result.size();
	for(int i = 0; i< strs.size() && ok; i++) {
		if(strs[i] != result[i]) { 
			//std::cout << strs[i] <<result[i] << strs[i].size() << result[i].size() ; 
			//std::cout << strs[i].size() << result[i].size();
			ok = false; 
		}
	}
	check(ok, msg);
}

void check_fundamentals()
{	
	price p;
	p.set("123.45");
	check_near(p.get(), 123.45, "price 123.45");


	centis c;
	c.set("456.78");
	check_near(c.get(), 45678, "centis 456.78");

	quantity q;
	q.set("37.46");
	check_near(q.get(), 37.46, "quantity 37.46");

	p.reprice(c, q);
	check_near(p.get(), 1219.3806, "reprice");
	
	//recentis(c, p, q);
	c = p*q;
	check_near(c.get(), 45678, "recentis");
}
void run_all_tests()
{
	check_erase_all("", 'a', "");
	check_erase_all("aaa", 'a', "");
	check_erase_all("alibaba", 'a', "libb");
	check_erase_all("baad", 'a', "bd");

	check_tokeniser("hello world",  { "hello", "world" });
	check_tokeniser("foo  \"baz shazam\"  bar", {"foo", "baz shazam", "bar"});
       	check_tokeniser("    hello  \"joe blogg\"  world   # just a comment   ", {"hello", "joe blogg", "world"});
	check_tokeniser("this is \"tricky disco", {"this", "is", "tricky disco"});

	check_fundamentals();
}

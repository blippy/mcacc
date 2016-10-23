#include <cmath>
#include <decimal/decimal>
#include <iostream>
#include <string>
#include <type_traits>

#include "cpq.hpp"
#include "inputs.hpp"
#include "reusable.hpp"



using std::cout;
using std::endl;
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

////////////////////////////////////////////////////////////
// experiments with decimals

template<int DP>
class decn 
{ 
	public:
		decn() {};
		decn(long before, long after) { dec = std::decimal::make_decimal128((long long)(before* pow(10, DP) + after), -DP); };
		std::decimal::decimal128 dec ;
	       	int dp = DP; 
		bool operator==(const decn<DP>& other) { return this->dec == other.dec; };
		bool operator!=(const decn<DP>& other) { return this->dec != other.dec; };
		friend decn<DP> operator+(decn<DP> lhs, const decn<DP>& rhs) { lhs.dec += rhs.dec; return lhs; };
};

//bool operator==(const decn& lhs, const decn& rhs) { return lhs.dp == rhs.dp && lhs.dec == rhs.dec; }

typedef decn<2> dec2;
//class dec2 : public decn<2> {};

void check_decimals()
{
	dec2 d1 = dec2(12, 34);
	static_assert(true, "always works");
	//assert(d.dp ==2);
	check(d1==d1, "decimal trivial equality");
	dec2 d2 = dec2(12, 35);
	check(d1!=d2, "decimal trivial inequality");
	check(dec2(10, 12) + dec2(13, 14) == dec2(23, 26), "decimal simple addition");
	
}

////////////////////////////////////////////////////////////

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
	check_decimals();
}

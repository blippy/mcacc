#include <string>

#include "price.hpp"
#include "reusable.hpp"

using namespace std;

price::price(const centis& c, const quantity& q)
{
	reprice(c, q);
}
void price::get(double& num, double& den) const
{
	num = rat.numerator();
	den = rat.denominator();	
}
double price::get() const
{
	double num, den;
	num = rat.numerator();
	den = rat.denominator();
	return num/den;
}
void price::set(const price& p)
{
	rat = p.rat;
}

void price::set(const std::string& s)
{

	double num = bround(scale * stod(s));
	rat = price_t (num, scale);

}


void price::reprice(const centis& c, const quantity& q)
{
	double qnum, qden;
	q.get(qnum, qden);
	rat = boost::rational<long int>(c.get()) /
		boost::rational<long int>(qnum, qden);
}

std::string price::str2() const
{
	string s1 =format_num(get(), 2);
	string s2 = pad_left(s1, 12);
	return s2 ;
}

std::string price::str6() const
{
	string s1 =format_num(get(), 6);
	string s2 = pad_left(s1, 12);
	return s2 ;
}
void price::diff(const price& p1, const price& p2)
{
	rat = p1.rat - p2.rat;
}

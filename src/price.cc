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
	//return boost::rational_cast<double>(rat);
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

// TODO deprecate
void recentis(centis& out_centis, const price& p, const quantity& q)
{
	price_t v1 = p.rat * price_t(q.num(), q.scale);
	double v2 = boost::rational_cast<double>(v1);
	out_centis.set(v2);
}

centis price::recentis(const quantity& q) const
{
	price_t v1 = this->rat * price_t(q.num(), q.scale);
	double v2 = boost::rational_cast<double>(v1);
	centis c;
	c.set(v2);
	return c;
}

void price::reprice(const centis& c, const quantity& q)
{
	double qnum, qden;
	//c.get(cnum, cden);
	q.get(qnum, qden);
	//rat = boost::rational<long int>(c.get() * qden, qnum);
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

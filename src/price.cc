#include <string>

#include "price.hpp"
#include "reusable.hpp"

using namespace std;

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

void recentis(centis& out_centis, const price& p, const quantity& q)
{
	/*
	double pnum, pden, qnum, qden;
	p.get(pnum, pden);
	q.get(qnum, qden);
	double newval = (pnum * qnum) * (pden * qden)/100;
	out_centis.set(newval);
	*/

	price_t v1 = p.rat * price_t(q.num(), q.scale);
	//double n = v1.numerator();
	//double d = v1.denominator();
	//double v2 = n/d;
	//cout << n << '\t' << d <<  '\t' << v2 << endl;
	double v2 = boost::rational_cast<double>(v1);
	//cout << v2 << endl;
	out_centis.set(v2);
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

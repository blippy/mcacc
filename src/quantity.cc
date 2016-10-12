#include <cmath>

#include "quantity.hpp"
#include "reusable.hpp"

using namespace std;

quantity::quantity(const std::string& s)
{
	from_str(1, s);
}

string format(double d)
{
	string s = format_num(d, 4);
	return pad_left(s, 12);
}

std::string quantity::pos_str() const
{
	return format(fabs(value/scale));
}

std::string quantity::str() const
{
	return format(value/scale);
}

void quantity::from_str(double sgn, const std::string & s)
{
	value = bround(sgn * scale * stod(s));
}

double quantity::get() const
{
	return value/scale;
}

void quantity::get(double& num, double& den) const
{
	num = value;
	den = scale;
}

double quantity::num() const
{
	return value;
}

void quantity::inc(const quantity& q)
{
	value += q.num();
}


void quantity::set(const std::string & s) { from_str(1, s); }

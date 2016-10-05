#include <cmath>

#include "quantity.hpp"
#include "reusable.hpp"

using namespace std;

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
	value = sgn * scale * bround(stod(s));
}

double quantity::get() const
{
	return value/scale;
}

double quantity::num() const
{
	return value;
}

void quantity::inc(const quantity& q)
{
	value += q.num();
}



#include "dec.hpp"



std::decimal::decimal128 dbl_to_dec(double d, int dp) { 
	double d1 = bround(d * pow(10, dp));
	return std::decimal::make_decimal128((long long)d1, -dp); 
}

std::decimal::decimal128 str_to_dec(const std::string& s, int dp) { return dbl_to_dec(stod(s), dp); }
std::decimal::decimal128 str_to_dec(double sgn, const std::string& s, int dp) { return dbl_to_dec(sgn *stod(s), dp); }


price operator/(const currency& c, const quantity& q) { return price(c.dec/q.dec*100); }
currency operator*(const price& p, const quantity& q) { return currency(p.dbl()*q.dbl()/100); }




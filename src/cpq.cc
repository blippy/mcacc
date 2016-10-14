#include "cpq.hpp"

price operator/(const centis& c, const quantity& q)
{
	price p;
	p.reprice(c, q);
	return p;
}

centis operator*(const price& p, const quantity& q)
{
	centis c;
	price_t v1 = p.rat * price_t(q.num(), q.scale);
	double v2 = boost::rational_cast<double>(v1);
	c.set(v2);
	return c;
}

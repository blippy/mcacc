#include "cpq.hpp"

price operator/(const centis& c, const quantity& q)
{
	price p;
	p.reprice(c, q);
	return p;
}

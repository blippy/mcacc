using namespace std;

#include "centis.hpp"
#include "reusable.hpp"



double centis::get() const { return value;};
void centis::set(double newval) { value = bround(newval); }

void centis::set(double sgn, std::string str)
{
	value = bround(sgn * 100 * stod(str));
}

centis::centis(const std::string& s)
{
	set(s);
}
void centis::set(std::string str) { set(1, str); }

//void centis::inc(double by) { value += bround(by); }

centis& centis::operator+=(const centis& c)
{
	this->value += c.value;
}

std::string centis::str () const 
{
	std::ostringstream s;
	s << std::fixed;
	s << setw(12);
	s << setprecision(2);
	s <<  value/100;
	return s.str();
}
void centis::write(std::ofstream& ofs)
{
	//ofs << fixed << setw(12) << setprecision(2) ;
	//ofs << (value/100);
	ofs << str();
} 


void write_centis(std::ofstream& ofs, const centis& c)
{

	ofs << fixed << setw(12) << setprecision(2) ;
	ofs << (c.get()/100);
}

void centis::negate()
{
	value = -value;
}

/*centis operator*(const price& p, const quantity& q)
{
	centis c;
	recentis(c, p, q);
	return c;
}
*/
centis operator-(const centis& c1, const centis& c2)
{
	centis c;
	c.set(c1.get() - c2.get());
	return c;
}

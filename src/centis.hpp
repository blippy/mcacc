#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>

class centis {
	public:
		centis() {};
		centis(const std::string& s);
		double get() const;
		void set(double newval);
		void set(double sgn, std::string str);
		void set(std::string str);
		//void inc(double by);
		centis& operator+=(const centis& c);
		void write(std::ofstream& ofs);
		void negate();
		std::string str() const;
	private:
		double value = 0;
};

void write_centis(std::ofstream& ofs, const centis& c);
centis operator-(const centis& c1, const centis& c2);

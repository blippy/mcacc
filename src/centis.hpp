#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>

class centis {
	public:
		centis() {};
		double get() const;
		void set(double newval);
		void inc(double by);
		void inc(const centis& by);
		void write(std::ofstream& ofs);
		void negate();
		std::string str() const;
	private:
		double value = 0;
};

void write_centis(std::ofstream& ofs, const centis& c);

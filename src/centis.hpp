#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>

class centis {
	public:
		centis() {};
		double get() const;
		//void get(double& num, );
		void set(double newval);
		void set(double sgn, std::string str);
		void set(std::string str);
		void inc(double by);
		void inc(const centis& by);
		void write(std::ofstream& ofs);
		void negate();
		std::string str() const;
	private:
		double value = 0;
};

void write_centis(std::ofstream& ofs, const centis& c);

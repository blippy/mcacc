#pragma once

#include <string>

#include <boost/rational.hpp>

#include "centis.hpp"
#include "quantity.hpp"

typedef boost::rational<long int> price_t;

class price {
	public:		
		price() {};
		price(const centis& c, const quantity& q);
		price_t rat;
		void set(const price& p);
		void set(const std::string& s);
		void get(double& num, double& den) const;
		double get() const;
		std::string str2() const;
		std::string str6() const;
		void reprice(const centis& c, const quantity& q);
		void diff(const price& p1, const price& p2);
	private:
		static const long int scale = 100'000;
};

#pragma once

#include <string>

class quantity {
	public:
		quantity() {};
		std::string pos_str() const;
		std::string str() const;
		double get() const;
		void get(double& num, double& den) const;
		double num() const;
		void inc(const quantity& q);
		void from_str(double sgn, const std::string & s);
		void set(const std::string & s);
		const double scale = 1'000;
	private:
		double value = 0;
};

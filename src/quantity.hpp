#pragma once

#include <string>

class quantity {
	public:
		quantity() {};
		std::string pos_str() const;
		std::string str() const;
		double get() const;
		double num() const;
		void inc(const quantity& q);
		void from_str(double sgn, const std::string & s);
	private:
		double value = 0;
		const double scale = 1'000;
};

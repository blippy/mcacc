#pragma once

#include <string>
#include <vector>
#include "types.hpp"

enum Etype { unknown, leak, regular };

typedef struct etran_t {
	bool	taxable;
	dstamp_t	dstamp;
	double sgn;
	bool	buy;
	std::string	folio;
	//std::string	sym;
	double	qty;
	pennies_t	cost = 0;
	std::string	ticker;
	Etype typ = unknown;

	// derived fields:
	pennies_t	ucost; 
	dstamp_t	start_dstamp;
	pennies_t	start_price;
	dstamp_t	end_dstamp;
	pennies_t	end_price;
	pennies_t	prior_year_profit;
	pennies_t	vbefore;
	pennies_t	flow;
	pennies_t	profit;
	pennies_t	vto;
} etran_t ;

bool operator<(const etran_t& lhs, const etran_t& rhs);

bool same_ticker(etran_t a, etran_t b);
typedef std::vector<etran_t> etran_ts;

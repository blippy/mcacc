#pragma once

#include <string>
#include <vector>
#include "types.hpp"

typedef struct etrana {
	bool	taxable;
	dstamp_t	dstamp;
	bool	buy;
	std::string	folio;
	std::string	sym;
	double	qty;
	pennies_t	cost;
	pennies_t	ucost;
	std::string	ticker;
	dstamp_t	start_dstamp;
	pennies_t	start_price;
	dstamp_t	end_dstamp;
	pennies_t	end_price;
	pennies_t	prior_year_profit;
	pennies_t	vbefore;
	pennies_t	flow;
	pennies_t	profit;
	pennies_t	vto;
} etrana ;

bool operator<(const etrana& lhs, const etrana& rhs);

void convert(const strings &str, etrana& e);
bool same_ticker(etrana a, etrana b);
typedef std::vector<etrana> etranas_t;
etranas_t load_etranas();
extern strings etrana_fieldnames;


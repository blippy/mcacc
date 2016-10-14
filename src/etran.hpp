#pragma once

#include <string>
#include <vector>
#include "types.hpp"
#include "quantity.hpp"
#include "centis.hpp"
#include "price.hpp"

enum Etype { unknown, leak, regular };

typedef struct etran_t {
	bool		taxable;
	dstamp_t	dstamp;
	double 		sgn;
	bool		buy;
	std::string	folio;
	quantity	qty;
	centis  	cost;
	std::string	ticker;
	Etype typ = unknown;
	std::string	buystr() const { return buy? "B" : "S"; };
} etran_t ;

typedef struct augetran_t {
	etran_t etran;
	// derived fields:
	price		ucost; 
	dstamp_t	start_dstamp;
	price		start_price;
	dstamp_t	end_dstamp;
	price		end_price;
	centis		prior_year_profit;
	centis		vbefore;
	centis		flow;
	centis		profit;
	centis		vto;
} augetran_t ;


bool operator<(const etran_t& lhs, const etran_t& rhs);

bool same_ticker(etran_t a, etran_t b);
typedef std::vector<etran_t> etran_ts;

typedef std::vector<augetran_t> augetran_ts;

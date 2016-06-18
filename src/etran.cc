#include "etran.hpp"

void convert(const strings &strs, etrana& e)
{

	e.taxable	= strs[0] == "T";
	e.dstamp	= strs[1];
	e.buy	= strs[2] == "B";
	e.folio	= strs[3];
	e.sym	= strs[4];
	e.qty	= stod(strs[5]);
	e.cost	= stod(strs[6]);
	e.ucost	= stod(strs[7]);
	e.ticker	= strs[8];
	e.start_dstamp	= strs[9];
	e.start_price	= stod(strs[10]);
	e.end_dstamp	= strs[11];
	e.end_price	= stod(strs[12]);
	e.prior_year_profit	= stod(strs[13]);
	e.vbefore	= stod(strs[14]);
	e.flow	= stod(strs[15]);
	e.profit	= stod(strs[16]);
	e.vto	= stod(strs[17]);

}

strings etrana_fieldnames = {	

	"taxable",
	"dstamp",
	"buy",
	"folio",
	"sym",
	"qty",
	"cost",
	"ucost",
	"ticker",
	"start_dstamp",
	"start_price",
	"end_dstamp",
	"end_price",
	"prior_year_profit",
	"vbefore",
	"flow",
	"profit",
	"vto"

};


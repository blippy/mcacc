#include <iostream>
#include <utility>
#include <math.h>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "common.hpp"
#include "reusable.hpp"
#include "yahoo.hpp"
#include "wiegley.hpp"

using namespace std;
using namespace boost;

typedef pair<string, string> spair;

bool sorter(spair a, spair b)
{
	return a.first < b.first;
}

void wiegley(const inputs_t& inputs)
{
	vector<spair> trans;

	// LEDGER.DAT
	
	for(auto& e: inputs.etrans) {
		//char sgn = e.buy ? ' ' : '-';
		string t1 = (format("%1%\t*\tetran\n") % e.dstamp).str() ;
		string at = e.regular ? "@@" : "@";
		string t2 = (format("\tEquity:%1%\t\"%2%\"\t%3%\t%4%\tGBP\t%5%\n") % 
				e.folio % e.ticker %  e.qty % at % to_gbp(labs(e.cost))).str();
		string t3 = (format("\t%1%\n\n") % e.folio).str();
		string t = t1 + t2 + t3;
		//cout << t;
		trans.push_back(make_pair(e.dstamp, t));
	}

	for(auto& n:inputs.ntrans) {
		string t1 = (format("%1%\t*\t%2%\n") % n.dstamp % n.desc).str();
		string t2 = (format("\t%1%\tGBP\t%2%\n") % n.dr % to_gbp(n.amount)).str();
		string t3 = (format("\t%1%\n\n") % n.cr).str();
		string t = t1 + t2 + t3;
		trans.push_back(make_pair(n.dstamp, t));
	}

	sort(begin(trans), end(trans), sorter);


	string dat = "N\tGBP\n\n";
	string fname = rootdir() + "/ledger.dat";
	for(auto& p: trans) {
		dat += p.second;
	}
	spit(fname, dat);

	/*
# LEDGER

#function LVL00 { true ; }
#function LVL01 { true ; }
#function LVL02 { true ; }
#function LVL04 { true ; }
#function LVL05 { true ; }

function LVL03 {
        ds=$1 ; acc1=$2 ; acc2=$3 ; typ=$4 ; qty=$5 ; amount=$6 ; buy=$7 ; tax=$8 ; desc=$9
        echo "$ds       *       $desc"
        case "$typ" in
                NTRAN-1)
                        echo "  $acc1   GBP     $amount"
                        echo "  $acc2"
                        ;;
                ETRAN-1)
                        sgn="" ; if [ "$buy" == "S" ]; then sgn="-"; fi
                        echo "  Eqty:$acc1      \"$acc2\"       $sgn$qty        @@      GBP     $amount"
                        echo "  $acc1"
                        ;;
                LEAK-1)
                        echo "  Eqty:$acc1      \"$acc2\"       -$qty   @       GBP     0.00"
                        echo "  $acc1   GBP 0.00"
                        ;;
                *) echo "unrecognised type $typ" ;;
        esac
        echo
}

LDAT=~/.mcacc/ledger.dat
printf "N       GBP\n\n" > $LDAT
source <(grep LVL03 $d2) >>$LDAT

# PRICES.DAT

function LVL05 {
        ds=$2 ; ts=$3 ; ticker=$4 ; val=$6
        pennies=`bc <<< "scale=7 ; $val/100"`
        echo "P $ds     $ts     \"$ticker\"     GBP     $pennies"
}

# anecdotally, the prices.dat file seems to take the most time, on account of use of `bc'
PDAT=~/.mcacc/prices.dat
source <(grep LVL05 $d2) >$PDAT
*/
	// PRICES.DAT
	trans.clear();
	for(auto& y:inputs.yahoos) {
		for(auto& y1: y.second) {
			string t = (format("P\t%1%\t%2%\t\"%3%\"\tGBP\t%4$.7f\n") % y1.dstamp % y1.tstamp % y1.ticker % (y1.price/100)).str();
			trans.push_back(make_pair(y1.dstamp, t));
		}
	}
	sort(begin(trans), end(trans), sorter);

	dat = "";
	for(auto& p:trans) dat += p.second;
	fname = rootdir() + "/prices.dat";
	spit(fname, dat);
}

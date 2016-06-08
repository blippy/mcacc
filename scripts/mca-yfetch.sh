#!/usr/bin/env bash

w=$HOME/.mca/work
s1=$w/s1
cd $w/s2

# dollar exchange rate
url="http://download.finance.yahoo.com/d/quotes.csv?s=USDGBP=X&f=nl1d1t1"
curl -s -L $url > usd.csv &




# symbols that need to be downloaded
PRE="http://download.finance.yahoo.com/d/quotes.csv?s="
POST="&f=sl1c1p2&e=.csv"

rm -f yfetch.csv

while read SYM
do
	URL="$PRE$SYM$POST"
	#echo $URL
	curl -s -L $URL >> yfetch.csv &
	#echo $URL >>yftech.csv &
done < <(awk '$2 == "W" { print $1}' $s1/comm.dsv)

wait



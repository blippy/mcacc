#!/usr/bin/env bash

#IN: s1/ntran.dsv s3/etrans-aug.dsv
#OUT: s3/posts.dsv

echo "WARN: posts: doesn't adjust for prior year, or note end dates"
cd ~/.mca/work

PFILE=s3/posts.dsv

awk  '
BEGIN {OFS = "\t" ; OFMT="%.0f"; }
{
amount = $4*100;
print $2, $1, $3, amount, $5, $6;
print  $3, $1, $2,-amount, $5, $6;
}
' s1/ntran.dsv > $PFILE


awk '
BEGIN {OFS = "\t"; OFMT="%.0f" }
{ dstamp = $2; way = $3; folio = $4; sym = $5;
sgn = (way == "B") ? 1 : -1;
#cost = - sgn * $7 * 100;
cost = -$7;
profit = - $17;
cd =  $18;
pbd = -$14;
print folio,      dstamp, "pCost", cost,   "C", "pCost:" sym;
print folio "_c", dstamp, "pVcd",  cd,     "C", "pVcd:"  sym;
print folio "_g", dstamp, "pPdp",  profit, "C", "pPdp:"  sym;
print "opn",      dstamp, "pbd",   pbd,    "C", "pbd:"   sym;;
}
' s3/etrans-aug.dsv >>$PFILE

rm0 () {
awk '$4 != 0 {print $0}'
}

sort $PFILE | rm0 | soak $PFILE


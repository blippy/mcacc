#!/usr/bin/env bash

#IN: s3/etrans-aug.dsv s3/yproc.dsv
#OUT: s3/snap*

root=`mcacc --root`
w=$root/work
s2=$w/s2
s3=$w/s3

REP=$s3/snap.rep

if [[ ! -f $s3/yproc.dsv ]]; then
	echo "INFO: Skipping production of $REP"
	exit 0
fi

PFILE=$s3/posts.dsv

awk  '
BEGIN {OFS = "\t"  }
$4 != "ut" { qtys[$9] += $6 }
END {
for (sym in qtys)
	if(qtys[sym]>0) {print sym, qtys[sym];}
}
' $s3/etrans-aug.dsv | sort > $s3/snap-1.dsv


awk '
BEGIN {OFS = "\t" }
{print $4, $6, $7, $8}
' $s3/yproc.dsv > $s3/snap-2.dsv

join $s3/snap-1.dsv $s3/snap-2.dsv >$s3/snap-3.dsv


awk '
BEGIN {
printf "%6s %12s %5s %12s %7s %12s\n", "EPIC", "PROFIT", "CHG%", "VALUE", "QTY", "PRICE"
}

{
qty = $2
val = qty * $3/100; tval += val
profit = qty * $4/100; tprofit += profit
chgpc = $5
printf "%6s ", $1 # ticker
printf "%12.2f ", profit 
printf "%5.2f ", chgpc
printf "%12.2f ", val
printf "%7d ", qty
printf "%12.2f ", $3 # price
printf "\n"
}

END {
chgpc = tprofit/(tval-tprofit) * 100
printf "%6s %12.2f %5.2f %12.2f\n\n", "TOTAL", tprofit, chgpc, tval
}
' $s3/snap-3.dsv >$REP

for idx in FTAS FTSE FTMC ; do
	grep $idx $s3/yproc.dsv | awk '{printf "%6s %12.2f %5.2f %12.2f\n", $4, $7, $8, $6}' >> $REP
done

printf "\nDTSTAMP: %s %s\n" `cat $s2/dstamp` `cat $s2/tstamp` >> $REP

while getopts "p" opt
do
	case $opt in
		p) cat $REP | nl -v 0 | green ;;
		*) echo "Unknown option: $opt" ;;
	esac
done

#!/usr/bin/env bash

source mcacc-env.sh

if false; then
	# traditional way which we should probably ignore from 21-Sep-2016
	dtstamp=`date --rfc-3339=seconds`
	nass=`grep "Net Assets" $s3/gaap-0.rep`
	line="$dtstamp $nass"
	echo $line >> $s3/nass-0.rep
	tac <$s3/nass-0.rep >$s3/nass-1.rep
fi

# alternate way
(cd $w/gaap && grep "Net Assets" *.txt) | sed -e 's/.txt:/   /' | sort -r >$s3/nass-2.rep



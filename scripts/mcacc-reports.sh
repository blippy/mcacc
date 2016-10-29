#!/usr/bin/env bash
# We do these at the end. It is automatically executed by mcacc

source mcacc-env.sh

financials.sh
returns.sh
#snap.sh

paste $s3/gaap-0.rep $s1/gaap-1.rep >$s3/gaap.rep
mkdir -p $root/arc/gaap
DS=`grep END $s3/gaap-0.rep | awk '{print $2}' `
#echo "ds is $DS"
cp $s3/gaap-0.rep $root/arc/gaap/$DS.txt
nass.sh

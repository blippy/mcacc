#!/usr/bin/env bash
# We do these at the end. It is automatically executed by mcacc

#root=`mcacc --root`
#w=$root/work
#s1=$w/s1
#s2=$w/s2
#s3=$w/s3
#cd $w

source mcacc-env.sh

financials.sh
returns.sh
snap.sh
paste $s3/gaap-0.rep $s1/gaap-1.rep >$s3/gaap.rep
#if [[ "$snap" == "--snap" ]]; then cat $s3/snap.rep ; fi
nass.sh

#!/usr/bin/env bash

w=~/.mca/work
s1=$w/s1
s2=$w/s2
s3=$w/s3
cd $w


clean () { # stage 0
	mkdir -p ~/.mca/{text,yahoo,work/{s1,s2,s3}}
	cp ~/repos/mcacc/mca.htm ~/.mca
	#rm -f *rec *txt all.txt *.dsv *.rep *~ *.csv *.sc
	#rm -f dsv endpoint
	rm -f $s1/* $s2/* $s3/*
}

stage1 () {
	rm -f $s1/*
	cd $s1
	while read fname
	do
		cat $fname
		echo # take care of file without line end
	done < <(ls ~/.mca/inputs/*.txt) | dos2unix | mcacc dsv
	echo `date` > 1
	cd -
}

download=0

stage2 () {
	cd $s2
	rm -f *
	mca-yfetch.sh
	echo -n `date --iso` > dstamp
	echo -n `date  +"%H:%M:%S"` >tstamp
	cd -
	download=1
	echo "Finished stage 2"
}

stage3 () {
	rm -f s3/*
	
	# perform processing of yahoos
	mcacc yproc
	mcacc stend
	mcacc eaug

	# additional processing
	mcacc posts
	mcacc etb

	# make reports
	mcacc epics
	mcacc cgt
	cd $s3
	financials.sh
	cd -
	returns.sh
	snap.sh
	if [[ "$download" == "1" ]]; then cat $s3/snap.rep ; fi
	 
}

usage () {
cat << EOF
Perform discrete stages of processing, and then exit:
-0 clean
-1 prep user input
-2 fetch prices
-3 main processing

-d turn on debugging
-h print this help and exit
-w web: lynx
EOF
}



####################################################################
# parse command-line parameters
# long form param parsing here:
# http://is.gd/X3rOuU
params="$(getopt -o dh0123e:vw -l exclude:,help,verbose --name "$(basename -- "$0")" -- "$@")"

#if [ $? -ne 0 ]
#then
#    usage
#fi

eval set -- "$params"
echo "$params"
unset params

while true
do
case $1 in
	-0) clean  ;;
	-1) stage1 ;; 
	-2) stage2 ;;
	-3) stage3 ;;
	-d) set -x ;;
	-h | --help) usage ; exit 0;;
	-w) lynx ~/.mca/mca.htm ;;
	--) break ;;
	*) echo "Unknown option. Try using -h" ; exit 1;;
esac
shift
done


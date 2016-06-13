#!/usr/bin/env bash

root=`mcacc --root`
w=$root/work
s1=$w/s1
s2=$w/s2
s3=$w/s3
cd $w


clean () { # stage 0
	# TODO make into mcacc stage0
	mkdir -p $root/{text,yahoo,work/{s1,s2,s3}}
	rm -f $s1/* $s2/* $s3/*
}

stage1 () {
	rm -f $s1/*
	cd $s1
	while read fname
	do
		cat $fname
		echo # take care of file without line end
	done < <(ls $root/inputs/*.txt) | dos2unix | mcacc dsv
}

download=0

stage2 () {
	rm -f $s2/*
	mcacc stage2
	download=1
}

stage3 () {
	rm -f s3/*
	mcacc stage3a
	financials.sh
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
	-w) lynx $root/mcacc.htm ;;
	--) break ;;
	*) echo "Unknown option. Try using -h" ; exit 1;;
esac
shift
done


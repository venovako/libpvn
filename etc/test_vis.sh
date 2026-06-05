#!/bin/bash
if [ "$2" = "" ]
then
	B=8
else
	B=$2
fi
if [ "$1" = "" ]
then
	C=128
else
	C=$1
fi
cp -fv jet$B.csv test_vis_r$B.csv
./test_vis.exe $C $B
rm -fv test_vis_r$B.csv
unset B C

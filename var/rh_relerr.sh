#!/bin/bash
echo "Using the executable $1.exe" > $5.log
echo "MPFR precision $3" >> $5.log
# e.g., $4 = 1073741824 = 2^30
for ((I=1;I<=$4;I*=2))
do
	$1.exe $2 $3 $I >> $5.txt 2>> $5.log
done
unset I

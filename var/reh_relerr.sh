#!/bin/bash
echo "MPFR precision $2" > $1.txt
for ((I=1;I<=1073741824;I*=2))
do
	./reh_relerr.exe $1 $2 $I >> $1.txt 2>&1
done
unset I

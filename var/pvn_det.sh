#!/bin/bash
# Usage: ./pvn_det.sh RUNS
echo '"RUN", "GENns", "DDETns", "RELERRns", "INFS", "MINRELERR", "MAXRELERR", "ZDETns", "ERR"' > pvn_det.csv
for ((R=1;R<=$1;++R))
do
	printf '%2d' $R >> pvn_det.csv
	echo -n ',' >> pvn_det.csv
	../src/pvn_det.exe 1073741824 256 >> pvn_det.csv
done
unset R

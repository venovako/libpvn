#!/bin/bash
# for example:
# ./gen_cbar.sh -6.60000000000000000E+01 5.84962500721172063E-01 2 0 jet8.csv cbar_ex_32.bmp lg '%#.6LE'
S=`mktemp`
echo -n "Generating the script $S ... "
./gen_cbar.exe $* > $S
chmod +x $S
echo "done"
$S
P=`basename $6 bmp`png
magick .$6 -colors 256 -quality 90 $P
rm -fv $S .$6
echo "$P generated"
unset P S

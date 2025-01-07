#!/bin/bash
MACHINE=`uname -m`
echo "MACHINE=$MACHINE"
if [ "$MACHINE" = "ppc64le" ]
then
	gcc -DCORE_MATH_SUPPORT_ERRNO -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c hypotf_noerrno.c -o hypotf.o
	gcc -DCORE_MATH_SUPPORT_ERRNO -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c rsqrtf_noerrno.c -o rsqrtf.o
	gcc -DCORE_MATH_SUPPORT_ERRNO -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c hypot_noerrno.c -o hypot.o
	gcc -DCORE_MATH_SUPPORT_ERRNO -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c rsqrt_noerrno.c -o rsqrt.o
else
	gcc -DCORE_MATH_SUPPORT_ERRNO -Og -ggdb3 -march=native -W -Wall $* -c hypotf_noerrno.c -o hypotf.o
	gcc -DCORE_MATH_SUPPORT_ERRNO -Og -ggdb3 -march=native -W -Wall $* -c rsqrtf_noerrno.c -o rsqrtf.o
	gcc -DCORE_MATH_SUPPORT_ERRNO -Og -ggdb3 -march=native -W -Wall $* -c hypot_noerrno.c -o hypot.o
	gcc -DCORE_MATH_SUPPORT_ERRNO -Og -ggdb3 -march=native -W -Wall $* -c rsqrt_noerrno.c -o rsqrt.o
	if [ "$MACHINE" = "x86_64" -o "$MACHINE" = "amd64" ]
	then
		gcc -Og -ggdb3 -march=native -W -Wall $* -c rsqrtl_noerrno.c -o rsqrtl.o
	fi
fi

#!/bin/bash
if [ `uname -m` = "ppc64le" ]
then
	echo ppc64le
	gcc -O3 -mcpu=native -mtraceback=full -fno-math-errno -W -Wall $* -c hypotf_noerrno.c
	gcc -O3 -mcpu=native -mtraceback=full -fno-math-errno -W -Wall $* -c rsqrtf_noerrno.c
	gcc -O3 -mcpu=native -mtraceback=full -fno-math-errno -W -Wall $* -c hypot_noerrno.c
	gcc -O3 -mcpu=native -mtraceback=full -fno-math-errno -W -Wall $* -c rsqrt_noerrno.c
else
	gcc -O3 -march=native -fno-math-errno -W -Wall $* -c hypotf_noerrno.c
	gcc -O3 -march=native -fno-math-errno -W -Wall $* -c rsqrtf_noerrno.c
	gcc -O3 -march=native -fno-math-errno -W -Wall $* -c hypot_noerrno.c
	gcc -O3 -march=native -fno-math-errno -W -Wall $* -c rsqrt_noerrno.c
fi

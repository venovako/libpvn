#!/bin/bash
if [ `uname -m` = "ppc64le" ]
then
	echo ppc64le
	gcc -Og -ggdb3 -mcpu=native -mtraceback=full -W -Wall $* -c hypotf_noerrno.c -o hypotf.o
	gcc -Og -ggdb3 -mcpu=native -mtraceback=full -W -Wall $* -c rsqrtf_noerrno.c -o rsqrtf.o
	gcc -Og -ggdb3 -mcpu=native -mtraceback=full -W -Wall $* -c hypot_noerrno.c -o hypot.o
	gcc -Og -ggdb3 -mcpu=native -mtraceback=full -W -Wall $* -c rsqrt_noerrno.c -o rsqrt.o
else
	gcc -Og -ggdb3 -march=native -W -Wall $* -c hypotf_noerrno.c -o hypotf.o
	gcc -Og -ggdb3 -march=native -W -Wall $* -c rsqrtf_noerrno.c -o rsqrtf.o
	gcc -Og -ggdb3 -march=native -W -Wall $* -c hypot_noerrno.c -o hypot.o
	gcc -Og -ggdb3 -march=native -W -Wall $* -c rsqrt_noerrno.c -o rsqrt.o
fi

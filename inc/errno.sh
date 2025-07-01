#!/bin/bash
MACHINE=`uname -m`
if [ "$GCC" = "" ]
then
	GCC=gcc
fi
echo "MACHINE=$MACHINE; GCC=$GCC"
if [ "$MACHINE" = "ppc64le" ]
then
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c hypotf_noerrno.c -o hypotf.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c rsqrtf_noerrno.c -o rsqrtf.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c sincosf_noerrno.c -o sincosf.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c hypot_noerrno.c -o hypot.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c rsqrt_noerrno.c -o rsqrt.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c sincos_noerrno.c -o sincos.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c hypotq_portable.c -o hypotq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c hypotq_noerrno.c -o hypotq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c rsqrtq_portable.c -o rsqrtq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c rsqrtq_noerrno.c -o rsqrtq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c sqrtq_portable.c -o sqrtq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -mcpu=native -mpower8-fusion -mtraceback=full -W -Wall $* -c sqrtq_noerrno.c -o sqrtq.o
else
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c hypotf_noerrno.c -o hypotf.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c rsqrtf_noerrno.c -o rsqrtf.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c sincosf_noerrno.c -o sincosf.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c hypot_noerrno.c -o hypot.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c rsqrt_noerrno.c -o rsqrt.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c sincos_noerrno.c -o sincos.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c hypotq_portable.c -o hypotq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c hypotq_noerrno.c -o hypotq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c rsqrtq_portable.c -o rsqrtq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c rsqrtq_noerrno.c -o rsqrtq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c sqrtq_portable.c -o sqrtq.o
	$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c sqrtq_noerrno.c -o sqrtq.o
	if [ "$MACHINE" = "x86_64" -o "$MACHINE" = "amd64" ]
	then
		$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c hypotl_noerrno.c -o hypotl.o
		$GCC -DCORE_MATH_SUPPORT_ERRNO -I../src -Og -ggdb3 -march=native -W -Wall $* -c rsqrtl_noerrno.c -o rsqrtl.o
	fi
fi

#!/bin/bash
MACHINE=`uname -m`
if [ "$GCC" = "" ]
then
	GCC=gcc
fi
echo "MACHINE=$MACHINE; GCC=$GCC"
if [ "$MACHINE" = "ppc64le" ]
then
	$GCC -DNDEBUG -I../src -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c hypotf_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c rsqrtf_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c sincosf_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c hypot_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c rsqrt_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c sincos_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c rsqrtq_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c sqrtq_noerrno.c
else
	$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c hypotf_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c rsqrtf_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c sincosf_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c hypot_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c rsqrt_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c sincos_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c rsqrtq_noerrno.c
	$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c sqrtq_noerrno.c
	if [ "$MACHINE" = "x86_64" -o "$MACHINE" = "amd64" ]
	then
		$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c hypotl_noerrno.c
		$GCC -DNDEBUG -I../src -O3 -march=native -fno-math-errno -W -Wall $* -c rsqrtl_noerrno.c
	fi
fi

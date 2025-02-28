#!/bin/bash
MACHINE=`uname -m`
echo "MACHINE=$MACHINE"
if [ "$MACHINE" = "ppc64le" ]
then
	gcc -DNDEBUG -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c hypotf_noerrno.c
	gcc -DNDEBUG -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c rsqrtf_noerrno.c
	gcc -DNDEBUG -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c hypot_noerrno.c
	gcc -DNDEBUG -O3 -mcpu=native -mpower8-fusion -mtraceback=full -fno-math-errno -W -Wall $* -c rsqrt_noerrno.c
else
	gcc -DNDEBUG -O3 -march=native -fno-math-errno -W -Wall $* -c hypotf_noerrno.c
	gcc -DNDEBUG -O3 -march=native -fno-math-errno -W -Wall $* -c rsqrtf_noerrno.c
	gcc -DNDEBUG -O3 -march=native -fno-math-errno -W -Wall $* -c hypot_noerrno.c
	gcc -DNDEBUG -O3 -march=native -fno-math-errno -W -Wall $* -c rsqrt_noerrno.c
	if [ "$MACHINE" = "x86_64" -o "$MACHINE" = "amd64" ]
	then
		gcc -DNDEBUG -O3 -march=native -fno-math-errno -W -Wall $* -c hypotl_noerrno.c
		gcc -DNDEBUG -O3 -march=native -fno-math-errno -W -Wall $* -c rsqrtl_noerrno.c
	fi
fi

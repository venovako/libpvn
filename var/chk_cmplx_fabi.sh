#!/bin/bash
#CC=(gcc*|clang*|icx|nvc|icc)
#FC=(gfortran*|flang*|ifx|nvfortran|ifort)
$CC chk_cmplx_fabi.c -c -o chk_cmplx_fabi.o
$FC chk_cmplx_fabi.F90 chk_cmplx_fabi.o -o chk_cmplx_fabi.exe
./chk_cmplx_fabi.exe
rm -f chk_cmplx_fabi.exe chk_cmplx_fabi.o

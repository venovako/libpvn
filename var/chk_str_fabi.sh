#!/bin/bash
#CC=(gcc*|clang*|icx|nvc)
#FC=(gfortran*|flang*|ifx|nvfortran)
$CC chk_str_fabi.c -c -o chk_str_fabi.o
$FC chk_str_fabi.F90 chk_str_fabi.o -o chk_str_fabi.exe
./chk_str_fabi.exe
rm -f chk_str_fabi.exe chk_str_fabi.o

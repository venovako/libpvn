icx /nologo /c chk_cmplx_fabi.c /Fochk_cmplx.obj /MT
ifx /nologo chk_cmplx_fabi.F90 chk_cmplx.obj /Fechk_cmplx_fabi.exe /MT
chk_cmplx_fabi.exe
DEL /F chk_cmplx_fabi.exe chk_cmplx_fabi.obj chk_cmplx.obj

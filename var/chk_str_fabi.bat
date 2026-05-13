icx /nologo /c chk_str_fabi.c /Fochk_str.obj /MT
ifx /nologo chk_str_fabi.F90 chk_str.obj /Fechk_str_fabi.exe /MT
chk_str_fabi.exe
DEL /F chk_str_fabi.exe chk_str.obj

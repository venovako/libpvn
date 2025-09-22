RM=del /F
AR=lib.exe
ARFLAGS=/NOLOGO /VERBOSE
CC=icx.exe
!IFNDEF MARCH
MARCH=Host
!ENDIF # !MARCH
!IFNDEF NDEBUG
NDEBUG=d
!ENDIF # !NDEBUG
!IFNDEF CRT
CRT=MT
!ENDIF # !CRT
PFLAGS=/DNDEBUG=$(NDEBUG) /DPVN_OS=Windows /DPVN_ARCH=x64 /DPVN_COMPILER=icx /DPVN_CR_MATH=1 /DPVN_QUADMATH=2
CFLAGS=/nologo /Qstd:c18 /Qlong-double /Qoption,cpp,--extended_float_types /EHsc /O$(NDEBUG) /Qx$(MARCH) /Ob2 /fp:precise /Qfma /Qftz- /Qopt-report:3 /Qvec-threshold:0 /traceback /Wno-deprecated-declarations /Wno-overriding-option
!IF "$(CRT)"=="MD"
CFLAGS=$(CFLAGS) $(PFLAGS) /I. /MD
CCFLAGS=$(CFLAGS) /DPVN_DYNAMIC=1
CDFLAGS=$(CFLAGS) /LD /Wno-unused-command-line-argument
CLFLAGS=$(CFLAGS) /DPVN_TEST=1 /DPVN_CPPFLAGS="\"$(PFLAGS) /I$(MAKEDIR)\"" /DPVN_CFLAGS="\"$(CFLAGS) /MD\"" /DPVN_LDFLAGS="/link /RELEASE /LIBPATH:$(MAKEDIR)"
LIBS=pvn.dll
!IFNDEF MSYS64
MSYS64=C:\msys64
!ENDIF # !MSYS64
!ELSE # MT
CFLAGS=$(CFLAGS) $(PFLAGS) /I. /MT
CCFLAGS=$(CFLAGS)
CLFLAGS=$(CFLAGS) /DPVN_TEST=2 /DPVN_CPPFLAGS="\"$(PFLAGS) /I$(MAKEDIR)\"" /DPVN_CFLAGS="\"$(CFLAGS) /MT\"" /DPVN_LDFLAGS="/link /RELEASE /LIBPATH:$(MAKEDIR)"
LIBS=pvn.lib
!ENDIF # MD

SRCS= \
pvn.c \
pvn_aux.c \
pvn_bio.c \
pvn_bmp.c \
pvn_cjs.c \
pvn_cma.c \
pvn_crm.c \
pvn_det.c \
pvn_djs.c \
pvn_dot.c \
pvn_err.c \
pvn_ev2.c \
pvn_fmt.c \
pvn_jv2.c \
pvn_mem.c \
pvn_mm2.c \
pvn_nrm.c \
pvn_ran.c \
pvn_sv2.c \
pvn_tar.c \
pvn_vec.c \
pvn_vis.c \
pvn_lock.c \
pvn_timer.c \
..\inc\hypotf_noerrno.c \
..\inc\powf_noerrno.c \
..\inc\rsqrtf_noerrno.c \
..\inc\sincosf_noerrno.c \
..\inc\hypot_noerrno.c \
..\inc\pow_noerrno.c \
..\inc\rsqrt_noerrno.c \
..\inc\sincos_noerrno.c \
..\inc\hypotl_noerrno.c \
..\inc\powl_noerrno.c \
..\inc\rsqrtl_noerrno.c \
..\inc\hypotq_noerrno.c \
..\inc\rsqrtq_noerrno.c \
..\inc\sqrtq_noerrno.c \
..\inc\getopt_long.c
OBJS= \
pvn.obj \
pvn_aux.obj \
pvn_bio.obj \
pvn_bmp.obj \
pvn_cjs.obj \
pvn_cma.obj \
pvn_crm.obj \
pvn_det.obj \
pvn_djs.obj \
pvn_dot.obj \
pvn_err.obj \
pvn_ev2.obj \
pvn_fmt.obj \
pvn_jv2.obj \
pvn_mem.obj \
pvn_mm2.obj \
pvn_nrm.obj \
pvn_ran.obj \
pvn_sv2.obj \
pvn_tar.obj \
pvn_vec.obj \
pvn_vis.obj \
pvn_lock.obj \
pvn_timer.obj \
..\inc\hypotf_noerrno.obj \
..\inc\powf_noerrno.obj \
..\inc\rsqrtf_noerrno.obj \
..\inc\sincosf_noerrno.obj \
..\inc\hypot_noerrno.obj \
..\inc\pow_noerrno.obj \
..\inc\rsqrt_noerrno.obj \
..\inc\sincos_noerrno.obj \
..\inc\hypotl_noerrno.obj \
..\inc\powl_noerrno.obj \
..\inc\rsqrtl_noerrno.obj \
..\inc\hypotq_noerrno.obj \
..\inc\rsqrtq_noerrno.obj \
..\inc\sqrtq_noerrno.obj \
..\inc\getopt_long.obj
EXES= \
pvn.exe \
pvn_aux.exe \
pvn_bio.exe \
pvn_bmp.exe \
pvn_cjs.exe \
pvn_cma.exe \
pvn_crm.exe \
pvn_det.exe \
pvn_djs.exe \
pvn_dot.exe \
pvn_err.exe \
pvn_ev2.exe \
pvn_fmt.exe \
pvn_jv2.exe \
pvn_mem.exe \
pvn_mm2.exe \
pvn_nrm.exe \
pvn_ran.exe \
pvn_sv2.exe \
pvn_tar.exe \
pvn_vec.exe \
pvn_vis.exe \
pvn_lock.exe \
pvn_timer.exe

all: $(LIBS) $(EXES)

help:
	@echo "nmake.exe [MARCH=Host|...] [NDEBUG=d|1|2|3|...] [CRT=MT|MD] [all|clean|help]"

pvn.lib: $(OBJS) Makefile
	$(AR) $(ARFLAGS) /OUT:$@ $(OBJS)

pvn.dll: $(OBJS) Makefile
	@echo "=== EXPERIMENTALLY LINKING WITH THE GCC's LIBQUADMATH ==="
	@echo "=== DO NOT ATTEMPT THIS FOR ANYTHING REDISTRIBUTABLE! ==="
	$(CC) $(CDFLAGS) /Fe$@ $(OBJS) /link /RELEASE /LIBPATH:$(MSYS64)\ucrt64\lib libquadmath.dll.a
	copy /Y /V $(MSYS64)\ucrt64\bin\libgcc_s_seh-1.dll .
	copy /Y /V $(MSYS64)\ucrt64\bin\libquadmath-0.dll .
	copy /Y /V $(MSYS64)\ucrt64\bin\libwinpthread-1.dll .

.c.obj:
	$(CC) $(CCFLAGS) /c $< /Fo$@

.c.exe:
	$(CC) $(CLFLAGS) $< /link /RELEASE /LIBPATH:. pvn.lib

clean:
	-$(RM) *.exe
	-$(RM) *.dll
	-$(RM) *.lib
	-$(RM) *.obj
	-$(RM) ..\inc\*.obj
	-$(RM) *.optrpt
	-$(RM) *.pdb

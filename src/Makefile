RM=del /F
AR=lib.exe
ARFLAGS=/NOLOGO /VERBOSE
CC=icx.exe
!IFNDEF MARCH
MARCH=Host
!ENDIF # !MARCH
!IFNDEF NDEBUG
NDEBUG=d
!ENDIF # NDEBUG
CFLAGS=/nologo /Qstd:c18 /Qlong-double /Qoption,cpp,--extended_float_types /DNDEBUG=$(NDEBUG) /DPVN_OS="\"Windows\"" /DPVN_ARCH="\"x64\"" /DPVN_COMPILER="\"icx\"" /DPVN_CR_MATH="\"bundled_noerrno\"" /DPVN_QUADMATH="\"Intel\"" /I. /EHsc /MT /O$(NDEBUG) /Qx$(MARCH) /fp:precise /Qfma /Qftz- /Qopt-report:3 /Qvec-threshold:0 /Wno-deprecated-declarations /Wno-overriding-option
SRCS= \
pvn.c \
pvn_aux.c \
pvn_bio.c \
pvn_bmp.c \
pvn_cjs.c \
pvn_cma.c \
pvn_crm.c \
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
pvn_vis.c \
..\inc\hypotf_noerrno.c \
..\inc\rsqrtf_noerrno.c \
..\inc\hypot_noerrno.c  \
..\inc\rsqrt_noerrno.c  \
..\inc\hypotl_noerrno.c \
..\inc\rsqrtl_noerrno.c
OBJS= \
pvn.obj \
pvn_aux.obj \
pvn_bio.obj \
pvn_bmp.obj \
pvn_cjs.obj \
pvn_cma.obj \
pvn_crm.obj \
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
pvn_vis.obj \
..\inc\hypotf_noerrno.obj \
..\inc\rsqrtf_noerrno.obj \
..\inc\hypot_noerrno.obj  \
..\inc\rsqrt_noerrno.obj  \
..\inc\hypotl_noerrno.obj \
..\inc\rsqrtl_noerrno.obj
LIBS=pvn.lib
EXES= \
pvn.exe \
pvn_aux.exe \
pvn_bio.exe \
pvn_bmp.exe \
pvn_cjs.exe \
pvn_cma.exe \
pvn_crm.exe \
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
pvn_vis.exe

all: $(LIBS) $(EXES)

help:
	@echo "nmake.exe [MARCH=Host|...] [NDEBUG=d|1|2|3|...] [all|clean|help]"

pvn.lib: $(OBJS) Makefile
	$(AR) $(ARFLAGS) /OUT:$@ $(OBJS)

.c.obj:
	$(CC) $(CFLAGS) /c $< /Fo$@

.c.exe:
	$(CC) $(CFLAGS) /DPVN_TEST="\"static_exe\"" $< /link /RELEASE /LIBPATH:. $(LIBS)

clean:
	-$(RM) *.exe
	-$(RM) *.lib
	-$(RM) *.obj
	-$(RM) ..\inc\*.obj
	-$(RM) *.optrpt
	-$(RM) *.pdb

OS=Windows
ARCH=x64
AR=xilib.exe
ARFLAGS=-qnoipo -lib /NOLOGO /VERBOSE
!IFNDEF COMPILER
COMPILER=icx
!ENDIF # !COMPILER
CC=$(COMPILER).exe
CFLAGS=/nologo /Qstd=c18 /Qlong-double /Qopenmp-simd /D_CRT_SECURE_NO_WARNINGS /EHsc /traceback
!IFDEF NDEBUG
CFLAGS=$(CFLAGS) /DNDEBUG=$(NDEBUG) /O$(NDEBUG) /Ob2 /Qvec-threshold:0
!ELSE # !NDEBUG
CFLAGS=$(CFLAGS) /Od
!ENDIF # ?NDEBUG
CFLAGS=$(CFLAGS) /QxHost /fp:precise /Qfma /Qftz- /Qprotect-parens
!IF "$(COMPILER)"=="icl"
CFLAGS=$(CFLAGS) /Qcomplex-limited-range- /Qfast-transcendentals- /Qprec-div /Qprec-sqrt /Qopt-multi-version-aggressive /Qsimd-honor-fp-model /Qsimd-serialize-fp-reduction /Qopt-report:5 /Qdiag-disable:10397,10441
!ELSE # icx
CFLAGS=$(CFLAGS) /Oy- /Qfp-speculation:safe /Qimf-precision:high /Qopt-report:3 /Wno-deprecated-declarations
!ENDIF # ?COMPILER
!IFDEF OPENMP
CFLAGS=$(CFLAGS) /Qopenmp
!IF "$(OPENMP)"!="true"
CFLAGS=$(CFLAGS) $(OPENMP)
!ENDIF # !true
!ENDIF # OPENMP
!IFDEF NDEBUG
CFLAGS=$(CFLAGS) /MD
LDFLAGS=/link /RELEASE pvn.lib
!ELSE # !NDEBUG
CFLAGS=$(CFLAGS) /MDd
LDFLAGS=/link /DEBUG pvn.lib
!ENDIF # ?NDEBUG
CFLAGS=$(CFLAGS) /DPVN_OS="\"$(OS)\"" /DPVN_ARCH="\"$(ARCH)\"" /DPVN_COMPILER="\"$(COMPILER)\""
!IFDEF VECLEN
CFLAGS=$(CFLAGS) /DPVN_VECLEN=$(VECLEN)u
!ENDIF # VECLEN

SRCS=pvn.c \
pvn_aux.c  \
pvn_bmp.c  \
pvn_cjs.c  \
pvn_crm.c  \
pvn_ev2.c  \
pvn_fmt.c  \
pvn_mem.c  \
pvn_vis.c
OBJS=pvn.obj \
pvn_aux.obj  \
pvn_bmp.obj  \
pvn_cjs.obj  \
pvn_crm.obj  \
pvn_ev2.obj  \
pvn_fmt.obj  \
pvn_mem.obj  \
pvn_vis.obj
EXES=pvn.exe \
pvn_aux.exe  \
pvn_bmp.exe  \
pvn_cjs.exe  \
pvn_crm.exe  \
pvn_ev2.exe  \
pvn_fmt.exe  \
pvn_mem.exe  \
pvn_vis.exe

all: pvn.lib $(EXES)

help:
	@echo "nmake.exe [NDEBUG=0|1|2|3|...] [VECLEN=...] [OPENMP=...] [all|clean|help]"

pvn.lib: $(OBJS)
	$(AR) $(ARFLAGS) /OUT:$@ $(OBJS)

.c.obj:
	$(CC) $(CFLAGS) /c $<

.c.exe:
	$(CC) $(CFLAGS) /DPVN_TEST $< $(LDFLAGS)

clean:
	-del /F *.exe
	-del /F *.lib
	-del /F *.obj
	-del /F *.pdb
	-del /F *.optrpt
	-del /F *.opt.yaml

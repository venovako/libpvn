OS=$(shell uname)
ifdef STATIC
ifeq ($(OS),Darwin)
$(error static linking is not supported on Darwin)
endif # Darwin
else # !STATIC
ifeq ($(findstring MINGW64,$(OS)),MINGW64)
STATIC=-s
endif # MINGW64
endif # ?STATIC
MACHINE=$(shell uname -m)
ifeq ($(MACHINE),amd64)
ARCH=x86_64
else # !amd64
ifeq ($(MACHINE),aarch64)
ARCH=arm64
else # !aarch64
ARCH=$(MACHINE)
endif # ?aarch64
endif # ?amd64
ifeq ($(OS),Darwin)
DYNAMIC=dylib
else # !Darwin
DYNAMIC=so
endif # ?Darwin
ifndef LIB64
LIB64=lib
endif # !LIB64
RM=rm -rfv

ifndef COMPILER
ifeq ($(OS),Linux)
COMPILER=gcc
else # !Linux
COMPILER=clang
endif # ?Linux
endif # !COMPILER
ifneq ($(COMPILER),gcc)
ifndef GCC
ifeq ($(OS),Darwin)
GCC=gcc-15
else # !Darwin
GCC=gcc
endif # ?Darwin
endif # !GCC
endif # !gcc
include $(COMPILER).mk
ifndef GCC
GCC=$(CC)
endif # !GCC
MKFS=GNUmakefile $(COMPILER).mk

ifeq ($(findstring 86,$(ARCH)),86)
ifneq ($(COMPILER),clang)
ifneq ($(COMPILER),nvc)
ifndef QUADMATH
QMD=$(realpath $(shell $(GCC) -print-file-name=libquadmath.$(DYNAMIC)))
ifeq ($(QMD),)
QMA=$(realpath $(shell $(GCC) -print-file-name=libquadmath.a))
ifneq ($(QMA),)
QUADMATH=$(QMA)
endif # QMA
else # QMD
QUADMATH=$(QMD)
endif # ?QMD
endif # !QUADMATH
endif # !nvc
endif # !clang
endif # ?86

INCS=-I$(CURDIR)

SRCS=pvn.c \
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
pvn_timer.c
ifneq ($(findstring MINGW64,$(OS)),MINGW64)
SRCS += pvn_prof.c
endif # !MINGW64
ifeq ($(findstring mpi,$(COMPILER_PREFIX)),mpi)
PFLAGS += -DPVN_MPI=50 -DPVN_MPI_ABI=10
CFLAGS += -mpi-abi
SRCS += pvn_mpi.c
endif # MPI

ifdef SLEEF
INCS += -I$(SLEEF)/include
PFLAGS += -DPVN_SLEEF=1
endif # SLEEF
ifdef MPC
INCS += -I$(MPC)/include
PFLAGS += -DPVN_MPC=1
ifndef MPFR
MPFR=/usr/local
endif # !MPFR
endif # MPC
ifdef MPFR
SRCS += pvn_mpfr.c
INCS += -I$(MPFR)/include
PFLAGS += -DPVN_MPFR=1
ifndef GMP
GMP=/usr/local
endif # !GMP
endif # MPFR
ifdef GMP
INCS += -I$(GMP)/include
PFLAGS += -DPVN_GMP=1
endif # GMP

ifdef SAFE
ifeq ($(findstring NRM,$(SAFE)),NRM)
PFLAGS += -DPVN_NRM_SAFE=15
endif # NRM
ifeq ($(findstring nRM,$(SAFE)),nRM)
PFLAGS += -DPVN_NRM_SAFE=13
endif # nRM
ifeq ($(findstring NrM,$(SAFE)),NrM)
PFLAGS += -DPVN_NRM_SAFE=11
endif # NrM
ifeq ($(findstring nrM,$(SAFE)),nrM)
PFLAGS += -DPVN_NRM_SAFE=9
endif # nrM
ifeq ($(findstring NRm,$(SAFE)),NRm)
PFLAGS += -DPVN_NRM_SAFE=7
endif # NRm
ifeq ($(findstring nRm,$(SAFE)),nRm)
PFLAGS += -DPVN_NRM_SAFE=5
endif # nRm
ifeq ($(findstring Nrm,$(SAFE)),Nrm)
PFLAGS += -DPVN_NRM_SAFE=3
endif # Nrm
ifeq ($(findstring nrm,$(SAFE)),nrm)
PFLAGS += -DPVN_NRM_SAFE=1
endif # nrm
ifeq ($(findstring SV2,$(SAFE)),SV2)
PFLAGS += -DPVN_SV2_SAFE=7
endif # SV2
ifeq ($(findstring sV2,$(SAFE)),sV2)
PFLAGS += -DPVN_SV2_SAFE=5
endif # sV2
ifeq ($(findstring Sv2,$(SAFE)),Sv2)
PFLAGS += -DPVN_SV2_SAFE=3
endif # Sv2
ifeq ($(findstring sv2,$(SAFE)),sv2)
PFLAGS += -DPVN_SV2_SAFE=1
endif # sv2
endif # SAFE

ifdef VECLEN
PFLAGS += -DPVN_VECLEN=$(VECLEN)
endif # VECLEN

ifdef CR_MATH
ifdef NDEBUG
PFLAGS += -DPVN_CR_MATH=2
else # !NDEBUG
PFLAGS += -DPVN_CR_MATH=4 -DCORE_MATH_SUPPORT_ERRNO
endif # ?NDEBUG
CR_SRCS= \
$(realpath $(CR_MATH)/src/binary32/hypot/hypotf.c) \
$(realpath $(CR_MATH)/src/binary32/pow/powf.c) \
$(realpath $(CR_MATH)/src/binary32/rsqrt/rsqrtf.c) \
$(realpath $(CR_MATH)/src/binary32/sincos/sincosf.c) \
$(realpath $(CR_MATH)/src/binary64/hypot/hypot.c) \
$(realpath $(CR_MATH)/src/binary64/pow/pow.c) \
$(realpath $(CR_MATH)/src/binary64/rsqrt/rsqrt.c) \
$(realpath $(CR_MATH)/src/binary64/sincos/sincos.c)
ifdef QUADMATH
CR_SRCS += \
$(realpath $(CR_MATH)/src/binary128/rsqrt/hypotq.c) \
$(realpath $(CR_MATH)/src/binary128/rsqrt/rsqrtq.c) \
$(realpath $(CR_MATH)/src/binary128/sqrt/sqrtq.c)
endif # QUADMATH
ifeq ($(findstring 86,$(ARCH)),86)
ifneq ($(findstring MINGW64,$(OS)),MINGW64)
CR_SRCS += \
$(realpath $(CR_MATH)/src/binary80/hypot/hypotl.c) \
$(realpath $(CR_MATH)/src/binary80/pow/powl.c) \
$(realpath $(CR_MATH)/src/binary80/rsqrt/rsqrtl.c)
endif # !MINGW64
endif # 86
else # !CR_MATH
ifdef NDEBUG
PFLAGS += -DPVN_CR_MATH=1
else # !NDEBUG
PFLAGS += -DPVN_CR_MATH=3 -DCORE_MATH_SUPPORT_ERRNO
endif # ?NDEBUG
CR_SRCS= \
../inc/hypotf_noerrno.c \
../inc/powf_noerrno.c \
../inc/rsqrtf_noerrno.c \
../inc/sincosf_noerrno.c \
../inc/hypot_noerrno.c \
../inc/pow_noerrno.c \
../inc/rsqrt_noerrno.c \
../inc/sincos_noerrno.c
ifdef QUADMATH
CR_SRCS += \
../inc/hypotq_noerrno.c \
../inc/rsqrtq_noerrno.c \
../inc/sqrtq_noerrno.c
else # !QUADMATH
ifeq ($(ARCH),ppc64le)
CR_SRCS += \
../inc/hypotq_portable.c \
../inc/rsqrtq_portable.c \
../inc/sqrtq_portable.c
endif # ppc64le
endif # ?QUADMATH
ifeq ($(findstring 86,$(ARCH)),86)
ifneq ($(findstring MINGW64,$(OS)),MINGW64)
CR_SRCS += \
../inc/hypotl_noerrno.c \
../inc/powl_noerrno.c \
../inc/rsqrtl_noerrno.c
endif # !MINGW64
endif # 86
endif # ?CR_MATH
CR_OBJS=$(CR_SRCS:.c=.o)

ifdef OPENMP
ifdef PROFILE
ifeq ($(PROFILE),0)
PROFILE=1
endif # ?PROFILE
endif # PROFILE
endif # OPENMP

ifdef PROFILE
PFLAGS += -DPVN_PROFILE=$(PROFILE)u
ifeq ($(COMPILER),nvc)
CFLAGS += -Minstrument
FCFLAGS += -Minstrument
else # !nvc
CFLAGS += -fno-inline -finstrument-functions
FCFLAGS += -fno-inline -finstrument-functions
endif # ?nvc
endif # PROFILE

ifdef PRINTOUT
PFLAGS += -DPVN_PRINTOUT=STD$(PRINTOUT)_FILENO
endif # PRINTOUT

ifdef QUADMATH
ifeq ($(COMPILER),gcc)
PFLAGS += -DPVN_QUADMATH=1
else # !gcc
PFLAGS += -DPVN_QUADMATH=2
endif # ?gcc
endif # QUADMATH

HDRS=$(SRCS:.c=.h)
OBJS=$(SRCS:.c=.o) $(CR_OBJS)
EXES=$(SRCS:.c=.exe) ../etc/gen_cbar.exe
LIBS=libpvn.a
ifneq ($(findstring MINGW64,$(OS)),MINGW64)
LIBS += libpvn.$(DYNAMIC)
endif # !MINGW64

ifeq ($(OS),Darwin)
PFLAGS += -DPVN_DYNAMIC=2
DLDFLAGS=-Wl,-install_name,$(CURDIR)/libpvn.$(DYNAMIC) -Wl,-warn_unused_dylibs -Wl,-warn_commons
LDFLAGS=-rdynamic -Wl,-warn_unused_dylibs -Wl,-warn_commons
else # !Darwin
ifndef STATIC
PFLAGS += -DPVN_DYNAMIC=1
endif # !STATIC
DLDFLAGS=--warn-common
ifdef NDEBUG
ifneq ($(NDEBUG),g)
DLDFLAGS += -O$(NDEBUG)
endif # !g
endif # NDEBUG
ifeq ($(COMPILER),nvc)
LDFLAGS=-Wl,-E
else # !nvc
ifeq ($(findstring MINGW64,$(OS)),MINGW64)
LDFLAGS=-Wl,--major-os-version=10
else # !MINGW64
LDFLAGS=-rdynamic
endif # ?MINGW64
endif # ?nvc
ifdef STATIC
LDFLAGS += -static $(STATIC)
else # !STATIC
LDFLAGS += -Wl,--warn-common
endif # ?STATIC
endif # ?Darwin
LNK=
ifdef SLEEF
LNK += -L$(SLEEF)/$(LIB64)
ifeq ($(OS),Darwin)
LNK += -Wl,-rpath,$(SLEEF)/$(LIB64)
else # !Darwin
ifndef STATIC
LNK += -Wl,-rpath=$(SLEEF)/$(LIB64)
endif # !STATIC
endif # ?Darwin
LNK += -lsleef -ltlfloat
endif # SLEEF
ifdef MPC
LNK += -L$(MPC)/$(LIB64)
ifeq ($(OS),Darwin)
LNK += -Wl,-rpath,$(MPC)/$(LIB64)
else # !Darwin
ifndef STATIC
LNK += -Wl,-rpath=$(MPC)/$(LIB64)
endif # !STATIC
endif # ?Darwin
LNK += -lmpc
endif # MPC
ifdef MPFR
LNK += -L$(MPFR)/$(LIB64)
ifeq ($(OS),Darwin)
LNK += -Wl,-rpath,$(MPFR)/$(LIB64)
else # !Darwin
ifndef STATIC
LNK += -Wl,-rpath=$(MPFR)/$(LIB64)
endif # !STATIC
endif # ?Darwin
LNK += -lmpfr
endif # MPFR
ifdef GMP
LNK += -L$(GMP)/$(LIB64)
ifeq ($(OS),Darwin)
LNK += -Wl,-rpath,$(GMP)/$(LIB64)
else # !Darwin
ifndef STATIC
LNK += -Wl,-rpath=$(GMP)/$(LIB64)
endif # !STATIC
endif # ?Darwin
LNK += -lgmp
endif # GMP
ifdef QUADMATH
ifdef QMA
LNK += $(QUADMATH)
else # !QMA
ifdef QMD
QMP=$(dir $(QUADMATH))
LNK += -L$(QMP)
ifeq ($(OS),Darwin)
LNK += -Wl,-rpath,$(QMP)
else # !Darwin
ifndef STATIC
LNK += -Wl,-rpath=$(QMP)
endif # !STATIC
endif # ?Darwin
LNK += -lquadmath
else # !QMD
LNK += $(QUADMATH)
endif # ?QMD
endif # ?QMA
endif # QUADMATH
ifeq ($(OS),Darwin)
ifdef OPENMP
ifeq ($(COMPILER),clang)
LOMP=omp
else # gcc
LOMP=gomp
endif # ?COMPILER
DOMP=$(dir $(realpath $(shell $(CC) -print-file-name=lib$(LOMP).dylib)))
LNK += -L$(DOMP) -Wl,-rpath,$(DOMP) -l$(LOMP)
endif # OPENMP
LGCC=$(dir $(realpath $(shell $(GCC) -print-file-name=libgcc_s.1.1.dylib)))
LNK += -L$(LGCC) -Wl,-rpath,$(LGCC) -lgcc_s.1.1
else # !Darwin
ifdef STATIC
LNK += -static-libgcc
else # !STATIC
LNK += -lgcc_s
endif # ?STATIC
endif # ?Darwin
ifeq ($(findstring BSD,$(OS)),BSD)
LNK += -lexecinfo -lelf
endif # BSD
LNK += -lpthread -lm
ifneq ($(findstring MINGW64,$(OS)),MINGW64)
LNK += -ldl
endif # !MINGW64
LNKX=-L$(CURDIR)
ifeq ($(OS),Darwin)
LNKX += -Wl,-rpath,$(CURDIR)
else # !Darwin
ifndef STATIC
LNKX += -Wl,-rpath=$(CURDIR)
endif # !STATIC
endif # ?Darwin
LNKX += -lpvn $(subst / , ,$(LNK))

PFLAGS += $(subst / , ,$(INCS))
AFLAGS := -DPVN_CC="\"$(CC)\"" -DPVN_FC="\"$(FC)\"" -DPVN_CPPFLAGS="\"$(PFLAGS)\"" -DPVN_CFLAGS="\"$(CFLAGS)\"" -DPVN_FCFLAGS="\"$(FCFLAGS)\"" -DPVN_LDFLAGS="\"$(LDFLAGS)\"" -DPVN_LIBS="\"$(LNKX)\""

CCFLAGS=$(CFLAGS) $(PFLAGS)
CLFLAGS=$(subst PIC,PIE,$(CCFLAGS)) $(AFLAGS)

.PHONY: all help clean

all: $(LIBS) $(EXES)
	@echo "BUILT: $(LIBS) $(EXES)"

help:
	@echo $(MAKE) "[COMPILER=clang|gcc|icx|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [MARCH=...] [NDEBUG=0|1|2|3|...] [PRINTOUT=OUT|ERR] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [PROFILE=...] [SAFE=...] [QUADMATH=...] [LIB64=lib|lib64] [GMP=...] [MPFR=...] [MPC=...] [SLEEF=...] [STATIC=-s] [all|clean|help]"

libpvn.a: $(OBJS) $(MKFS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

libpvn.dylib: libpvn.a $(MKFS)
	clang -dynamiclib $(DLDFLAGS) -o $@ $(OBJS) $(LNK) 

libpvn.so: libpvn.a $(MKFS)
	ld -shared $(DLDFLAGS) -o $@ $(OBJS)

%.o : %.c $(HDRS) $(MKFS)
	$(CC) $(CCFLAGS) -c $< -o $@

ifdef STATIC
%.exe : %.c $(HDRS) libpvn.a $(MKFS)
	$(CC) $(CLFLAGS) -DPVN_TEST=2 $< -o $@ $(LDFLAGS) $(LNKX)
else # !STATIC
%.exe : %.c $(HDRS) libpvn.$(DYNAMIC) $(MKFS)
	$(CC) $(CLFLAGS) -DPVN_TEST=1 $< -o $@ $(LDFLAGS) $(LNKX)
endif # ?STATIC

../etc/gen_cbar.exe: ../etc/gen_cbar.c $(LIBS) $(MKFS)
	$(CC) $(CLFLAGS) $< -o $@ $(LDFLAGS) $(LNKX)

clean:
	-$(RM) *.exe
	-$(RM) ../etc/*.exe
	-$(RM) *.so
	-$(RM) *.dylib
	-$(RM) *.a
	-$(RM) *.o
	-$(RM) *.optrpt
	-$(RM) ../inc/*.o
	-$(RM) ../inc/*.optrpt
	-$(RM) *.dSYM

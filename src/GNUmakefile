OS=$(shell uname)
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

ifeq ($(findstring 86,$(ARCH)),86)
ifneq ($(COMPILER),clang)
ifndef QUADMATH
ifdef DYNAMIC
QUADMATH=$(realpath $(shell $(GCC) -print-file-name=libquadmath.$(DYNAMIC)))
else # !DYNAMIC
QMA=$(realpath $(shell $(GCC) -print-file-name=libquadmath.a))
ifeq ($(QMA),)
ifeq ($(OS),Darwin)
QUADMATH=$(realpath $(shell $(GCC) -print-file-name=libquadmath.dylib))
else # !Darwin
QUADMATH=$(realpath $(shell $(GCC) -print-file-name=libquadmath.so))
endif # ?Darwin
else # QMA
QUADMATH=$(QMA)
endif # ?QM
endif # ?DYNAMIC
endif # !QUADMATH
endif # !clang
endif # ?86

CFLAGS += -I. -DPVN_OS="\"$(OS)\"" -DPVN_ARCH="\"$(ARCH)\"" -DPVN_COMPILER="\"$(COMPILER)\""

SRCS=pvn.c \
pvn_aux.c  \
pvn_bio.c  \
pvn_bmp.c  \
pvn_cjs.c  \
pvn_cma.c  \
pvn_crm.c  \
pvn_det.c  \
pvn_djs.c  \
pvn_dot.c  \
pvn_err.c  \
pvn_ev2.c  \
pvn_fmt.c  \
pvn_jv2.c  \
pvn_mem.c  \
pvn_mm2.c  \
pvn_nrm.c  \
pvn_ran.c  \
pvn_sv2.c  \
pvn_tar.c  \
pvn_vec.c  \
pvn_vis.c  \
pvn_lock.c \
pvn_timer.c

ifneq ($(findstring MINGW64,$(OS)),MINGW64)
SRCS += pvn_prof.c
endif # !MINGW64

ifdef MPC
CFLAGS += -I$(MPC)/include -DPVN_MPC="\"$(MPC)\""
ifndef MPFR
MPFR=/usr/local
endif # !MPFR
endif # MPC
ifdef MPFR
SRCS += pvn_mpfr.c
CFLAGS += -I$(MPFR)/include -DPVN_MPFR="\"$(MPFR)\""
ifndef GMP
GMP=/usr/local
endif # !GMP
endif # MPFR
ifdef GMP
CFLAGS += -I$(GMP)/include -DPVN_GMP="\"$(GMP)\""
endif # GMP
ifdef LAPACK
CFLAGS += -DPVN_LAPACK="\"$(LAPACK)\""
endif # LAPACK

ifdef SAFE
ifeq ($(findstring ev2,$(SAFE)),ev2)
CFLAGS += -DPVN_EV2_SAFE="\"hypotq\""
endif # ev2
ifeq ($(findstring jv2,$(SAFE)),jv2)
CFLAGS += -DPVN_JV2_SAFE="\"hypotq\""
endif # jv2
ifeq ($(findstring nrm,$(SAFE)),nrm)
CFLAGS += -DPVN_NRM_SAFE="\"cr_hypot\""
endif # nrm
ifeq ($(findstring ran,$(SAFE)),ran)
CFLAGS += -DPVN_RAN_SAFE="\"all_outputs\""
endif # ran
ifeq ($(findstring sv2,$(SAFE)),sv2)
CFLAGS += -DPVN_SV2_SAFE="\"precise\""
endif # sv2
endif # SAFE

ifdef VECLEN
CFLAGS += -DPVN_VECLEN=$(VECLEN)u
endif # VECLEN

ifdef CR_MATH
CFLAGS += -DPVN_CR_MATH="\"$(CR_MATH)\""
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
CR_SRCS += \
$(realpath $(CR_MATH)/src/binary80/hypot/hypotl.c) \
$(realpath $(CR_MATH)/src/binary80/pow/powl.c) \
$(realpath $(CR_MATH)/src/binary80/rsqrt/rsqrtl.c)
endif # 86
else # !CR_MATH
ifdef NDEBUG
CFLAGS += -DPVN_CR_MATH="\"bundled_noerrno\""
else # !NDEBUG
CFLAGS += -DPVN_CR_MATH="\"bundled\"" -DCORE_MATH_SUPPORT_ERRNO
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
CR_SRCS += \
../inc/hypotl_noerrno.c \
../inc/powl_noerrno.c \
../inc/rsqrtl_noerrno.c
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
CFLAGS += -DPVN_PROFILE=$(PROFILE)u
ifeq ($(COMPILER),nvc)
CFLAGS += -Minstrument
else # !nvc
CFLAGS += -fno-inline -finstrument-functions
endif # ?nvc
endif # PROFILE

ifdef PRINTOUT
CFLAGS += -DPVN_PRINTOUT=STD$(PRINTOUT)_FILENO
endif # PRINTOUT

ifdef QUADMATH
CFLAGS += -DPVN_QUADMATH="\"$(QUADMATH)\""
endif # QUADMATH

HDRS=$(SRCS:.c=.h)
OBJS=$(SRCS:.c=.o) $(CR_OBJS)
EXES=$(SRCS:.c=.exe) ../etc/gen_cbar.exe
ifdef MPFR
EXES += \
../var/crh_relerr.exe \
../var/laf_relerr.exe \
../var/reh_relerr.exe \
../var/rhh_relerr.exe
endif # MPFR
LIBS=libpvn.a
LNK=-L.
ifdef DYNAMIC
LIBS += libpvn.$(DYNAMIC)
LNK += -Wl,-rpath=.
endif # DYNAMIC
LNK += -lpvn
ifdef MPC
LNK += -L$(MPC)/lib -lmpc
endif # MPC
ifdef MPFR
LNK += -L$(MPFR)/lib -lmpfr
endif # MPFR
ifdef GMP
LNK += -L$(GMP)/lib -lgmp
endif # GMP
ifdef LAPACK
LNK += -L$(LAPACK) -ltmglib -llapack -lrefblas
endif # LAPACK
ifdef QUADMATH
LNK += $(QUADMATH)
endif # QUADMATH

ifeq ($(COMPILER),icc)
CLFLAGS=$(CFLAGS)
else # !icc
CLFLAGS=$(subst PIC,PIE,$(CFLAGS))
endif # ?icc

.PHONY: all help clean

all: $(LIBS) $(EXES)

help:
	@echo $(MAKE) "[COMPILER=clang|gcc|icx|icc|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [MARCH=...] [NDEBUG=0|1|2|3|...] [PRINTOUT=OUT|ERR] [VECLEN=...] [CR_MATH=...] [OPENMP=true|...] [PROFILE=...] [SAFE=...] [DYNAMIC=so] [QUADMATH=...] [GMP=...] [MPFR=...] [MPC=...] [LAPACK=...] [all|clean|help]"

libpvn.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

libpvn.so: libpvn.a
	ld -shared -o $@ $(OBJS)

%.o : %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

%.exe : %.c $(HDRS)
	$(CC) $(CLFLAGS) -DPVN_TEST="\"dynamic_exe\"" $< -o $@ $(LNK) $(LDFLAGS)

../etc/gen_cbar.exe: ../etc/gen_cbar.c $(LIBS)
	$(CC) $(CLFLAGS) $< -o $@ $(LNK) $(LDFLAGS)

../var/crh_relerr.exe: ../var/crh_relerr.c $(LIBS)
	$(CC) $(CLFLAGS) $< -o $@ $(LNK) $(LDFLAGS)

../var/laf_relerr.exe: ../var/laf_relerr.c $(LIBS)
	$(CC) $(CLFLAGS) $< -o $@ $(LNK) $(LDFLAGS)

../var/reh_relerr.exe: ../var/reh_relerr.c $(LIBS)
	$(CC) $(CLFLAGS) $< -o $@ $(LNK) $(LDFLAGS)

../var/rhh_relerr.exe: ../var/rhh_relerr.c $(LIBS)
	$(CC) $(CLFLAGS) $< -o $@ $(LNK) $(LDFLAGS)

clean:
	-$(RM) *.exe
	-$(RM) ../etc/*.exe
	-$(RM) ../var/*.exe
	-$(RM) *.so
	-$(RM) *.dylib
	-$(RM) *.a
	-$(RM) *.o
	-$(RM) *.optrpt
	-$(RM) ../inc/*.o
	-$(RM) ../inc/*.optrpt
	-$(RM) *.dSYM

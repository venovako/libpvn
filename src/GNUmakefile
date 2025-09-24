OS=$(shell uname)
ifeq ($(findstring MINGW64,$(OS)),MINGW64)
$(error MINGW64 not supported for now)
endif # ?MINGW64
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

ifeq ($(findstring 86,$(ARCH)),86)
ifneq ($(COMPILER),clang)
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
endif # !clang
endif # ?86

INCS=-I$(CURDIR)

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
pvn_prof.c \
pvn_timer.c

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
ifdef LAPACK
PFLAGS += -DPVN_LAPACK=1
endif # LAPACK

ifdef SAFE
ifeq ($(findstring ev2,$(SAFE)),ev2)
PFLAGS += -DPVN_EV2_SAFE=1
endif # ev2
ifeq ($(findstring jv2,$(SAFE)),jv2)
PFLAGS += -DPVN_JV2_SAFE=1
endif # jv2
ifeq ($(findstring nrm,$(SAFE)),nrm)
PFLAGS += -DPVN_NRM_SAFE=1
endif # nrm
ifeq ($(findstring ran,$(SAFE)),ran)
PFLAGS += -DPVN_RAN_SAFE=1
endif # ran
ifeq ($(findstring sv2,$(SAFE)),sv2)
PFLAGS += -DPVN_SV2_SAFE=1
endif # sv2
endif # SAFE

ifdef VECLEN
PFLAGS += -DPVN_VECLEN=$(VECLEN)u
endif # VECLEN

ifdef CR_MATH
PFLAGS += -DPVN_CR_MATH=3
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
PFLAGS += -DPVN_CR_MATH=1
else # !NDEBUG
PFLAGS += -DPVN_CR_MATH=2 -DCORE_MATH_SUPPORT_ERRNO
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
PFLAGS += -DPVN_PROFILE=$(PROFILE)u
ifeq ($(COMPILER),nvc)
CFLAGS += -Minstrument
else # !nvc
CFLAGS += -fno-inline -finstrument-functions
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
LIBS=libpvn.a libpvn.$(DYNAMIC)

ifeq ($(OS),Darwin)
PFLAGS += -DPVN_DYNAMIC=2
DLDFLAGS=-Wl,-install_name,$(CURDIR)/libpvn.$(DYNAMIC) -Wl,-warn_unused_dylibs -Wl,-warn_commons
LDFLAGS=-rdynamic -Wl,-warn_unused_dylibs -Wl,-warn_commons
else # !Darwin
PFLAGS += -DPVN_DYNAMIC=1
DLDFLAGS=--warn-common
ifdef NDEBUG
ifneq ($(NDEBUG),g)
DLDFLAGS += -O$(NDEBUG)
endif # !g
endif # NDEBUG
ifeq ($(COMPILER),nvc)
LDFLAGS=-Wl,-E
else # !nvc
LDFLAGS=-rdynamic
endif # ?nvc
LDFLAGS += -Wl,--warn-common
endif # ?Darwin
LNK=
ifdef MPC
LNK += -L$(MPC)/$(LIB64)
ifeq ($(OS),Darwin)
LNK += -Wl,-rpath,$(MPC)/$(LIB64)
else # !Darwin
LNK += -Wl,-rpath=$(MPC)/$(LIB64)
endif # ?Darwin
LNK += -lmpc
endif # MPC
ifdef MPFR
LNK += -L$(MPFR)/$(LIB64)
ifeq ($(OS),Darwin)
LNK += -Wl,-rpath,$(MPFR)/$(LIB64)
else # !Darwin
LNK += -Wl,-rpath=$(MPFR)/$(LIB64)
endif # ?Darwin
LNK += -lmpfr
endif # MPFR
ifdef GMP
LNK += -L$(GMP)/$(LIB64)
ifeq ($(OS),Darwin)
LNK += -Wl,-rpath,$(GMP)/$(LIB64)
else # !Darwin
LNK += -Wl,-rpath=$(GMP)/$(LIB64)
endif # ?Darwin
LNK += -lgmp
endif # GMP
ifdef LAPACK
LNK += -L$(LAPACK)
ifeq ($(OS),Darwin)
LNK += -Wl,-rpath,$(LAPACK)
else # !Darwin
LNK += -Wl,-rpath=$(LAPACK)
endif # ?Darwin
LNK += -ltmglib -llapack -lrefblas
endif # LAPACK
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
LNK += -Wl,-rpath=$(QMP)
endif # ?Darwin
LNK += -lquadmath
else # !QMD
LNK += $(QUADMATH)
endif # ?QMD
endif # ?QMA
endif # QUADMATH
ifeq ($(OS),Darwin)
LGCC=$(dir $(realpath $(shell $(GCC) -print-file-name=libgcc_s.1.1.dylib)))
LNK += -L$(LGCC) -Wl,-rpath,$(LGCC) -lgcc_s.1.1
else # !Darwin
LNK += -lgcc_s
endif # ?Darwin
LNK += -lpthread -lm
ifeq ($(findstring BSD,$(OS)),BSD)
LNK += -lexecinfo
else # !BSD
LNK += -ldl
endif # ?BSD
LNKX=-L$(CURDIR)
ifeq ($(OS),Darwin)
LNKX += -Wl,-rpath,$(CURDIR)
else # !Darwin
LNKX += -Wl,-rpath=$(CURDIR)
endif # ?Darwin
LNKX += -lpvn $(subst / , ,$(LNK))

PFLAGS += $(subst / , ,$(INCS))
AFLAGS=-DPVN_CC="\"$(CC)\"" -DPVN_CPPFLAGS="\"$(PFLAGS)\"" -DPVN_CFLAGS="\"$(shell echo $(CFLAGS))\"" -DPVN_LDFLAGS="\"$(LDFLAGS) $(LNKX)\""

CCFLAGS=$(CFLAGS) $(PFLAGS)
CLFLAGS=$(subst PIC,PIE,$(CCFLAGS)) $(AFLAGS)

.PHONY: all help clean

all: $(LIBS) $(EXES)
	@echo "BUILT: $(LIBS) $(EXES)"

help:
	@echo $(MAKE) "[COMPILER=clang|gcc|icx|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [MARCH=...] [NDEBUG=0|1|2|3|...] [PRINTOUT=OUT|ERR] [VECLEN=...] [CR_MATH=...] [OPENMP=true|...] [PROFILE=...] [SAFE=...] [QUADMATH=...] [LIB64=lib|lib64] [GMP=...] [MPFR=...] [MPC=...] [LAPACK=...] [all|clean|help]"

libpvn.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

libpvn.dylib: libpvn.a
	clang -dynamiclib $(DLDFLAGS) -o $@ $(OBJS) $(LNK) 

libpvn.so: libpvn.a
	ld -shared $(DLDFLAGS) -o $@ $(OBJS)

%.o : %.c $(HDRS)
	$(CC) $(CCFLAGS) -c $< -o $@

%.exe : %.c $(HDRS) $(LIBS)
	$(CC) $(CLFLAGS) -DPVN_TEST=1 $< -o $@ $(LDFLAGS) $(LNKX)

../etc/gen_cbar.exe: ../etc/gen_cbar.c $(LIBS)
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

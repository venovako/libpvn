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
include $(COMPILER).mk
CFLAGS += -DPVN_OS="\"$(OS)\"" -DPVN_ARCH="\"$(ARCH)\"" -DPVN_COMPILER="\"$(COMPILER)\""
ifdef SAFE
ifeq ($(findstring cma,$(SAFE)),cma)
CFLAGS += -DPVN_CMA_SAFE="\"all_inputs\""
endif # cma
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
$(realpath $(CR_MATH)/src/binary32/rsqrt/rsqrtf.c) \
$(realpath $(CR_MATH)/src/binary64/hypot/hypot.c)  
$(realpath $(CR_MATH)/src/binary64/rsqrt/rsqrt.c)
ifeq ($(findstring 86,$(ARCH)),86)
CR_SRCS += $(realpath $(CR_MATH)/src/binary80/rsqrt/rsqrtl.c)
endif # 86
else # !CR_MATH
ifdef NDEBUG
CFLAGS += -DPVN_CR_MATH="\"bundled_noerrno\""
else # !NDEBUG
CFLAGS += -DPVN_CR_MATH="\"bundled\""
endif # ?NDEBUG
CR_SRCS= \
../inc/hypotf_noerrno.c \
../inc/rsqrtf_noerrno.c \
../inc/hypot_noerrno.c  \
../inc/rsqrt_noerrno.c
ifeq ($(findstring 86,$(ARCH)),86)
CR_SRCS += ../inc/rsqrtl_noerrno.c
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

SRCS=pvn.c  \
pvn_aux.c   \
pvn_bio.c   \
pvn_bmp.c   \
pvn_cjs.c   \
pvn_cma.c   \
pvn_crm.c   \
pvn_dot.c   \
pvn_err.c   \
pvn_ev2.c   \
pvn_fmt.c   \
pvn_mem.c   \
pvn_mm2.c   \
pvn_nrm.c   \
pvn_ran.c   \
pvn_sv2.c   \
pvn_vis.c   \
pvn_prof.c  \
pvn_lock.c  \
pvn_timer.c

OBJS=$(SRCS:.c=.o) $(CR_OBJS)
EXES=$(SRCS:.c=.exe)
LIBS=libpvn.a
LNK=-L.
ifdef DYNAMIC
CFLAGS += -DPVN_DYNAMIC="\"$(DYNAMIC)\""
LIBS += libpvn.$(DYNAMIC)
ifdef QUADMATH
ifeq ($(QUADMATH),-lquadmath)
ifeq ($(COMPILER),gcc)
override QUADMATH=$(realpath $(shell $(CC) -print-file-name=libquadmath.$(DYNAMIC)))
else # !gcc
override QUADMATH=$(realpath $(shell gcc -print-file-name=libquadmath.$(DYNAMIC)))
endif # ?gcc
FSYS=$(dir $(QUADMATH))
ifeq ($(OS),Darwin)
GNU=-L$(FSYS) -lquadmath
else # !Darwin
XRP=-rpath=$(FSYS)
GNU += $(XRP) $(QUADMATH)
endif # ?Darwin
else # !-lquadmath
ifeq ($(OS),Darwin)
GNU=-load_hidden $(QUADMATH)
else # !Darwin
GNU=$(QUADMATH)
endif # ?Darwin
endif # ?-lquadmath
endif # QUADMATH
ifeq ($(OS),Darwin)
ifeq ($(COMPILER),gcc)
GNU += -load_hidden
endif # gcc
ifeq ($(realpath /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/System.framework),)
FSYS=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks
else # Xcode
FSYS=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks
endif # ?Xcode
else # !Darwin
LNK += -Wl,-rpath=.
endif # ?Darwin
ifeq ($(COMPILER),gcc)
GNU += $(realpath $(shell $(CC) -print-file-name=libgcc.a))
else # !gcc
ifeq ($(COMPILER),icx)
GNU += $(realpath $(shell gcc -print-file-name=libgcc.a))
endif # icx
endif # ?gcc
endif # DYNAMIC
LNK += -lpvn
ifdef XRP
LNK += -Wl,$(XRP)
endif # XRP
LNK += $(QUADMATH)

.PHONY: all help clean

all: $(LIBS) $(EXES)

help:
	@echo $(MAKE) "[COMPILER=clang|gcc|icx|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [CPU=...] [NDEBUG=0|1|2|3|...] [PRINTOUT=OUT|ERR] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [PROFILE=...] [SAFE=...] [DYNAMIC=dylib|so] [QUADMATH=-lquadmath] [all|clean|help]"

libpvn.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

libpvn.dylib: libpvn.a
	libtool -dynamic -o $@ -v $(GNU) -F$(FSYS) -framework System libpvn.a

libpvn.so: libpvn.a
	ld -shared -o $@ $(OBJS) $(GNU)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.exe : %.c
	$(CC) $(CFLAGS) -DPVN_TEST="\"dynamic_exe\"" $< -o $@ $(LNK) $(LDFLAGS)

clean:
	-$(RM) *.exe
	-$(RM) *.so
	-$(RM) *.dylib
	-$(RM) *.a
	-$(RM) *.o
	-$(RM) ../inc/*.o
	-$(RM) *.opt.yaml
	-$(RM) ../inc/*.opt.yaml
	-$(RM) *.dSYM

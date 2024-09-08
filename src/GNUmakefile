OS=$(shell uname)
CPU=$(shell uname -m)
ifeq ($(CPU),amd64)
ARCH=x86_64
else # !amd64
ifeq ($(CPU),i86pc)
ARCH=x86_64
else # !i86pc
ifeq ($(CPU),aarch64)
ARCH=arm64
else # !aarch64
ARCH=$(CPU)
endif # ?aarch64
endif # ?i86pc
endif # ?amd64
RM=rm -rfv
ifndef COMPILER
ifeq ($(OS),Linux)
COMPILER=gcc
else # !Linux
ifeq ($(OS),SunOS)
COMPILER=gcc
else # !SunOS
COMPILER=clang
endif # ?SunOS
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
ifeq ($(OS),Darwin)
LIBS += libpvn.dylib
ifeq ($(realpath /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/System.framework),)
FSYS=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks
else # Xcode
FSYS=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks
endif # ?Xcode
ifeq ($(COMPILER),gcc)
ifeq ($(QUADMATH),-lquadmath)
GNU=-L$(dir $(realpath $(shell $(CC) -print-file-name=libquadmath.dylib))) -lquadmath
else # libquadmath.a
GNU=-load_hidden $(QUADMATH)
endif # ?libquadmath
GNU += -load_hidden $(realpath $(shell $(CC) -print-file-name=libgcc.a))
endif # gcc
else # !Darwin
LIBS += libpvn.so
LNK += -Wl,-rpath=.
ifdef NDEBUG
GNU=-O$(NDEBUG)
else # !NDEBUG
GNU=-t
endif # ?NDEBUG
ifeq ($(QUADMATH),-lquadmath)
ifeq ($(COMPILER),gcc)
FSYS=$(dir $(realpath $(shell $(CC) -print-file-name=libquadmath.so)))
GNU += -L$(FSYS) -lquadmath $(realpath $(shell $(CC) -print-file-name=libgcc.a))
else # !gcc
FSYS=$(dir $(realpath $(shell gcc -print-file-name=libquadmath.so)))
GNU += -L$(FSYS) -lquadmath $(realpath $(shell gcc -print-file-name=libgcc.a))
endif # ?gcc
LNK += -L$(FSYS) -Wl,-rpath=$(FSYS)
else # libquadmath.a
ifeq ($(COMPILER),gcc)
GNU += $(QUADMATH) $(realpath $(shell $(CC) -print-file-name=libgcc.a))
else # !gcc
GNU += $(QUADMATH) $(realpath $(shell gcc -print-file-name=libgcc.a))
endif # ?gcc
endif # ?libquadmath
endif # ?Darwin
LNK += -lpvn

.PHONY: all help clean

all: $(LIBS) $(EXES)

help:
	@echo $(MAKE) "[COMPILER=clang|gcc|icx|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [NDEBUG=0|1|2|3|...] [PRINTOUT=OUT|ERR] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [QUADMATH=...] [PROFILE=...] [SAFE=...] [all|clean|help]"

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

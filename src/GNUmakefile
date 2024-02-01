OS=$(shell uname)
ARCH=$(shell uname -m)
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
ifdef VECLEN
CFLAGS += -DPVN_VECLEN=$(VECLEN)u
endif # VECLEN
ifdef CR_MATH
CFLAGS += -DPVN_CR_MATH="\"$(CR_MATH)\""
ifdef NDEBUG
CR_OBJS= \
$(CR_MATH)/src/binary32/hypot/hypotf_noerrno.o \
$(CR_MATH)/src/binary32/rsqrt/rsqrtf_noerrno.o \
$(CR_MATH)/src/binary64/hypot/hypot_noerrno.o  \
$(CR_MATH)/src/binary64/rsqrt/rsqrt_noerrno.o
else # !NDEBUG
CR_OBJS= \
$(CR_MATH)/src/binary32/hypot/hypotf.o \
$(CR_MATH)/src/binary32/rsqrt/rsqrtf.o \
$(CR_MATH)/src/binary64/hypot/hypot.o  \
$(CR_MATH)/src/binary64/rsqrt/rsqrt.o
endif # ?NDEBUG
LDFLAGS += $(CR_OBJS)
endif # CR_MATH

SRCS=pvn.c  \
pvn_aux.c   \
pvn_bmp.c   \
pvn_cjs.c   \
pvn_crm.c   \
pvn_ev2.c   \
pvn_sv2.c   \
pvn_fmt.c   \
pvn_mem.c   \
pvn_mtx.c   \
pvn_error.c \
pvn_lock.c  \
pvn_timer.c
OBJS=$(SRCS:.c=.o)
EXES=$(SRCS:.c=.exe)

.PHONY: all help clean

all: libpvn.a $(EXES)

help:
	@echo $(MAKE) "[COMPILER=clang|gcc|icx|nvc|icc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [NDEBUG=0|1|2|3|...] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [QUADMATH=-lquadmath] [all|clean|help]"

libpvn.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.exe : %.c
	$(CC) $(CFLAGS) -DPVN_TEST $< -o $@ -L. -lpvn $(LDFLAGS)

clean:
	-$(RM) $(EXES)
	-$(RM) libpvn.a
	-$(RM) $(OBJS)
	-$(RM) *.optrpt
	-$(RM) *.opt.yaml
	-$(RM) *.dSYM

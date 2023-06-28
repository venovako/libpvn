OS=$(shell uname)
ARCH=$(shell uname -m)
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
ifdef VECLEN
CFLAGS += -DPVN_VECLEN=$(VECLEN)u
endif # VECLEN

SRCS=pvn.c  \
pvn_bmp.c   \
pvn_error.c \
pvn_fmt.c   \
pvn_mem.c   \
pvn_lock.c  \
pvn_timer.c
OBJS=$(SRCS:.c=.o)
EXES=$(SRCS:.c=.exe)

.PHONY: all help clean

all: libpvn.a $(EXES)

help:
	@echo $(MAKE) "[COMPILER=clang|gcc|icx|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [NDEBUG=0|1|2|3|...] [VECLEN=...] [all|clean|help]"

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
	-$(RM) *.opt.yaml
	-$(RM) *.dSYM

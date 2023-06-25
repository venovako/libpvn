SHELL=/bin/bash
OS=$(shell uname)
ARCH=$(shell uname -m)
RM=rm -rfv
ifndef COMPILER
COMPILER=gcc
endif # !COMPILER
include $(COMPILER).mk

OBJS=     \
pvn_fmt.o \
pvn_lock.o

.PHONY: all help clean

all: libpvn.a

help:
	@echo $(MAKE) "[COMPILER=gcc|icx|clang] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [CFLAGS_SUFFIX=...] [NDEBUG=0|1|2|3|...] [all|clean|help]"

libpvn.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-$(RM) libpvn.a
	-$(RM) $(OBJS)
	-$(RM) $(OBJS:.o=.opt.yaml)

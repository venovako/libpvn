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
ifdef NDEBUG
CR_OBJS=
ifneq ($(strip $(wildcard $(CR_MATH)/src/binary32/hypot/hypotf_noerrno.o)),)
CR_OBJS += $(CR_MATH)/src/binary32/hypot/hypotf_noerrno.o
else
CR_OBJS += $(CR_MATH)/src/binary32/hypot/hypotf.o
endif
ifneq ($(strip $(wildcard $(CR_MATH)/src/binary32/rsqrt/rsqrtf_noerrno.o)),)
CR_OBJS += $(CR_MATH)/src/binary32/rsqrt/rsqrtf_noerrno.o
else
CR_OBJS += $(CR_MATH)/src/binary32/rsqrt/rsqrtf.o
endif
ifneq ($(strip $(wildcard $(CR_MATH)/src/binary64/hypot/hypot_noerrno.o)),)
CR_OBJS += $(CR_MATH)/src/binary64/hypot/hypot_noerrno.o
else
CR_OBJS += $(CR_MATH)/src/binary64/hypot/hypot.o
endif
ifneq ($(strip $(wildcard $(CR_MATH)/src/binary64/rsqrt/rsqrt_noerrno.o)),)
CR_OBJS += $(CR_MATH)/src/binary64/rsqrt/rsqrt_noerrno.o
else
CR_OBJS += $(CR_MATH)/src/binary64/rsqrt/rsqrt.o
endif
ifeq ($(findstring 86,$(ARCH)),86)
ifneq ($(strip $(wildcard $(CR_MATH)/src/binary80/rsqrt/rsqrtl_noerrno.o)),)
CR_OBJS += $(CR_MATH)/src/binary80/rsqrt/rsqrtl_noerrno.o
else
CR_OBJS += $(CR_MATH)/src/binary80/rsqrt/rsqrtl.o
endif
endif # 86
else # !NDEBUG
CR_OBJS= \
$(CR_MATH)/src/binary32/hypot/hypotf.o \
$(CR_MATH)/src/binary32/rsqrt/rsqrtf.o \
$(CR_MATH)/src/binary64/hypot/hypot.o  \
$(CR_MATH)/src/binary64/rsqrt/rsqrt.o
ifeq ($(findstring 86,$(ARCH)),86)
CR_OBJS += $(CR_MATH)/src/binary80/rsqrt/rsqrtl.o
endif # 86
endif # ?NDEBUG
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
CR_OBJS=$(CR_SRCS:.c=.o)
endif # ?CR_MATH
ifdef OPENMP
ifdef PROFILE
ifeq ($(PROFILE),0)
PROFILE=1
endif # ?PROFILE
endif # PROFILE
endif # OPENMP
ifdef PROFILE
CFLAGS += -DPVN_PROFILE=$(PROFILE)u -fno-inline -finstrument-functions
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
LIBS=libpvn.a
EXES=$(SRCS:.c=.exe)

.PHONY: all help clean

all: $(LIBS) $(EXES)

help:
	@echo $(MAKE) "[COMPILER=clang|gcc|icx] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [NDEBUG=0|1|2|3|...] [PRINTOUT=OUT|ERR] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [QUADMATH=...] [PROFILE=...] [SAFE=...] [all|clean|help]"

libpvn.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.exe : %.c
	$(CC) $(CFLAGS) -DPVN_TEST="\"dynamic_exe\"" $< -o $@ -L. -lpvn $(LDFLAGS)

clean:
	-$(RM) *.exe
	-$(RM) *.a
	-$(RM) *.o
	-$(RM) ../inc/*.o
	-$(RM) *.opt.yaml
	-$(RM) ../inc/*.opt.yaml
	-$(RM) *.dSYM

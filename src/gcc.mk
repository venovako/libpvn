ARFLAGS=rsv
CC=$(COMPILER_PREFIX)gcc$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno -fvect-cost-model=unlimited
else # DEBUG
CFLAGS=-Og -ggdb3 -ftrapv
endif # ?NDEBUG
CFLAGS += -Wall -Wextra
ifeq ($(OS),Linux)
CFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 14 ]; then echo 23; else echo 18; fi) -fPIC -fexceptions -fasynchronous-unwind-tables -ffp-contract=fast -fno-omit-frame-pointer -pthread
ifdef OPENMP
CFLAGS += -fopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
else # !OPENMP
CFLAGS += -fopenmp-simd
endif # ?OPENMP
ifndef MARCH
MARCH=native
endif # !MARCH
ifeq ($(ARCH),ppc64le)
CFLAGS += -mcpu=$(MARCH) -mpower8-fusion -mtraceback=full
else # !ppc64le
CFLAGS += -march=$(MARCH)
endif # ?ppc64le
ifeq ($(findstring MINGW64,$(OS)),MINGW64)
CFLAGS += -DPVN_EXTERN_C=EXTERN_C
LDFLAGS=
else # !MINGW64
LDFLAGS=-rdynamic
endif # ?MINGW64
LDFLAGS=-static-libgcc #-pie
ifeq ($(findstring BSD,$(OS)),BSD)
LDFLAGS += -lexecinfo
else # !BSD
ifneq ($(findstring MINGW64,$(OS)),MINGW64)
LDFLAGS += -ldl
endif # !MINGW64
endif # ?BSD
ifeq ($(findstring 86,$(ARCH)),86)
ifndef QUADMATH
QUADMATH=$(abspath $(shell $(CC) -print-file-name=libquadmath.a))
ifeq ($(QUADMATH),libquadmath.a)
QUADMATH=-lquadmath
endif # ?QUADMATH
endif # !QUADMATH
endif # ?86
LDFLAGS += -lm

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
CFLAGS += -std=gnu$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 14 ]; then echo 23; else echo 18; fi) -fPIC -fexceptions -fasynchronous-unwind-tables -ffp-contract=fast -fno-omit-frame-pointer -fopenmp-simd -pthread
ifdef OPENMP
CFLAGS += -fopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
endif # OPENMP
ifeq ($(ARCH),ppc64le)
CFLAGS += -mcpu=native -mpower8-fusion -mtraceback=full
else # !ppc64le
CFLAGS += -march=native
endif # ?ppc64le
LDFLAGS=-rdynamic -static-libgcc
ifeq ($(findstring BSD,$(OS)),BSD)
LDFLAGS += -lexecinfo
else # !BSD
LDFLAGS += -ldl
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

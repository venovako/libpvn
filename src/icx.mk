AR=ar
ARFLAGS=rsv
CC=$(COMPILER_PREFIX)icx$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG) -fno-math-errno -qopt-report=3
ifndef PROFILE
CFLAGS += -inline-level=2
endif # !PROFILE
else # DEBUG
PFLAGS=-DPVN_DEBUG=STDERR_FILENO
CFLAGS=-O0 -g -debug extended -debug inline-debug-info -debug pubnames -ftrapv
endif # ?NDEBUG
ifndef MARCH
MARCH=Host
# common-avx512 for KNLs
endif # !MARCH
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
CFLAGS += -std=gnu2x -fPIC -fp-model=precise -fp-speculation=safe -fprotect-parens -fma -no-ftz -fimf-precision=high -mprefer-vector-width=512 -pthread -vec-threshold0 -x$(MARCH)
ifndef VECLEN
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer -traceback
endif # !VECLEN
ifdef OPENMP
CFLAGS += -qopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
ifndef NDEBUG
CFLAGS += -debug parallel
endif # !NDEBUG
else # !OPENMP
CFLAGS += -qopenmp-simd
endif # ?OPENMP
CFLAGS += -Wno-overriding-option
ifdef STATIC
# 1=scalar, 2=simd, 3=both
PFLAGS += -DPVN_INTEL=3
endif # STATIC

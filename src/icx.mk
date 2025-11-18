AR=ar
ARFLAGS=rsv
CC=$(COMPILER_PREFIX)icx$(COMPILER_SUFFIX)
FC=$(COMPILER_PREFIX)ifx$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG) -fno-math-errno -qopt-report=3
FCFLAGS=-fpp -O$(NDEBUG) -fno-math-errno -qopt-report=3
ifndef PROFILE
CFLAGS += -inline-level=2
FCFLAGS += -inline-level=2
endif # !PROFILE
else # DEBUG
PFLAGS=-DPVN_DEBUG=STDERR_FILENO
CFLAGS=-O0 -g3
FCFLAGS=-fpp -O0 -g3 -init=arrays,zero #-check all
endif # ?NDEBUG
ifndef MARCH
MARCH=Host
# common-avx512 for KNLs
endif # !MARCH
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
CFLAGS += -std=gnu2x -fPIC -fp-model=precise -fp-speculation=safe -fprotect-parens -fma -no-ftz -fimf-precision=high -mprefer-vector-width=512 -pthread -vec-threshold0 -x$(MARCH)
FCFLAGS += -fPIC -fp-model=precise -fp-speculation=safe -fprotect-parens -fma -no-ftz -fimf-precision=high -mprefer-vector-width=512 -recursive -standard-semantics -threads -vec-threshold0 -x$(MARCH)
ifndef VECLEN
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer -traceback
FCFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer -traceback
endif # !VECLEN
ifdef OPENMP
PFLAGS += -DPVN_OPENMP=$(OPENMP)
CFLAGS += -qopenmp
FCFLAGS += -qopenmp
else # !OPENMP
CFLAGS += -qopenmp-simd
FCFLAGS += -qopenmp-simd
endif # ?OPENMP
CFLAGS += -Wno-overriding-option
ifdef STATIC
# 1=scalar, 2=simd, 3=both
PFLAGS += -DPVN_INTEL=3
endif # STATIC

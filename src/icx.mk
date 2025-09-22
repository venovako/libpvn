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
PFLAGS=
CFLAGS=-O0 -g -debug extended -debug inline-debug-info -debug pubnames -debug parallel -ftrapv
endif # ?NDEBUG
ifndef MARCH
MARCH=Host
# common-avx512 for KNLs
endif # !MARCH
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
CFLAGS += -std=gnu18 -fPIC -fp-model=precise -fp-speculation=safe -fprotect-parens -fma -no-ftz -fimf-precision=high -mprefer-vector-width=512 -pthread -vec-threshold0 -x$(MARCH) -Wno-overriding-option
ifndef LAPACK
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer -traceback
endif # !LAPACK
ifdef OPENMP
CFLAGS += -qopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
else # !OPENMP
CFLAGS += -qopenmp-simd
endif # ?OPENMP

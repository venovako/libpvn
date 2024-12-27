AR=ar
ARFLAGS=rsv
CC=$(COMPILER_PREFIX)icx$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno -qopt-report=3
ifndef PROFILE
CFLAGS += -inline-level=2
endif # !PROFILE
else # DEBUG
CFLAGS=-O0 -g -debug extended -debug inline-debug-info -debug pubnames -debug parallel -ftrapv
endif # ?NDEBUG
ifndef MARCH
MARCH=Host
# common-avx512 for KNLs
endif # !MARCH
CFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -std=gnu18 -fPIC -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer -fp-model=precise -fp-speculation=safe -fprotect-parens -fma -no-ftz -fimf-precision=high -mprefer-vector-width=512 -pthread -traceback -vec-threshold0 -x$(MARCH)
ifdef OPENMP
CFLAGS += -qopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
else # !OPENMP
CFLAGS += -qopenmp-simd
endif # ?OPENMP
LDFLAGS=-pie -rdynamic -static-libgcc -ldl
ifndef QUADMATH
QUADMATH=$(abspath $(shell gcc -print-file-name=libquadmath.a))
ifeq ($(QUADMATH),libquadmath.a)
QUADMATH=-lquadmath
endif # ?QUADMATH
endif # !QUADMATH
LDFLAGS += -lm

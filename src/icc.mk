AR=xiar
ARFLAGS=-qnoipo -lib rsv
CC=$(COMPILER_PREFIX)icc$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno -qopt-report=5
ifndef PROFILE
CFLAGS += -inline-level=2
endif # !PROFILE
else # DEBUG
CFLAGS=-O0 -g -debug extended -debug inline-debug-info -debug pubnames
ifeq ($(OS),Linux)
CFLAGS += -debug parallel
endif # Linux
endif # ?NDEBUG
ifeq ($(OS),Linux)
CFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
ifndef MARCH
MARCH=Host
# COMMON-AVX512 for KNLs
endif # !MARCH
CFLAGS += -std=gnu18 -diag-disable=265,1183,2171,4073,10397,10441,15519 -fPIC -fp-model precise -fprotect-parens -fma -no-complex-limited-range -no-ftz -prec-div -prec-sqrt -qsimd-honor-fp-model -qsimd-serialize-fp-reduction -qopt-multi-version-aggressive -qopt-zmm-usage=high -pthread -vec-threshold0 -x$(MARCH)
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
CFLAGS += -DPVN_ICC="\"$(shell icc -diag-disable=10441 -dumpversion)\""
LDFLAGS=-rdynamic
ifeq ($(OS),Darwin)
LDFLAGS += $(realpath $(shell $(GCC) -print-file-name=libgcc.a))
else # !Darwin
LDFLAGS += -static-libgcc
endif # ?Darwin
LDFLAGS += -ldl -lm

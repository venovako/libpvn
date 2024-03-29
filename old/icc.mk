AR=xiar
ARFLAGS=-qnoipo -lib rsv
CC=$(COMPILER_PREFIX)icc$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno -inline-level=2 -qopt-report=5 -vec-threshold0
else # DEBUG
CFLAGS=-O0 -g -debug extended -debug inline-debug-info -debug pubnames
ifeq ($(OS),Linux)
CFLAGS += -debug parallel
endif # Linux
endif # ?NDEBUG
ifeq ($(OS),Linux)
CFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu18 -diag-disable=10397,10441,15519 -fPIC -fexceptions -fp-model precise -fprotect-parens -fma -no-complex-limited-range -no-ftz -prec-div -prec-sqrt -qsimd-honor-fp-model -qsimd-serialize-fp-reduction -qopenmp-simd -pthread -fno-omit-frame-pointer -traceback -xHost
ifdef OPENMP
CFLAGS += -qopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
endif # OPENMP
LDFLAGS=-rdynamic
ifeq ($(OS),Linux)
LDFLAGS += -static-libgcc
endif # Linux
LDFLAGS += -ldl
ifndef QUADMATH
QUADMATH=$(shell gcc -print-file-name=libquadmath.a)
ifeq ($(QUADMATH),libquadmath.a)
QUADMATH=-lquadmath
endif # ?QUADMATH
endif # !QUADMATH
CFLAGS += -DPVN_QUADMATH="\"$(QUADMATH) -limf\""
LDFLAGS += $(QUADMATH) -lm

AR=xiar
ARFLAGS=-qnoipo -lib rsv
CC=$(COMPILER_PREFIX)icx$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno -inline-level=2 -qopt-report=3 -vec-threshold0
else # DEBUG
CFLAGS=-O0 -g -debug extended -debug inline-debug-info -debug pubnames -debug parallel -ftrapv
endif # ?NDEBUG
CFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -std=gnu18 -fPIC -fexceptions -fp-model precise -fprotect-parens -fma -no-ftz -qopenmp-simd -pthread -fno-omit-frame-pointer -traceback -xHost
ifdef OPENMP
CFLAGS += -qopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
endif # OPENMP
LDFLAGS=-rdynamic -static-libgcc -ldl -lm
CFLAGS += -DPVN_QUADMATH="\"-limf\""

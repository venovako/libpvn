AR=xiar
ARFLAGS=-qnoipo -lib rsv
CC=$(COMPILER_PREFIX)icx$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno -inline-level=2 -qopt-report=3 -vec-threshold0
else # DEBUG
CFLAGS=-O0 -g -debug extended -debug inline-debug-info -debug pubnames -debug parallel -ftrapv
endif # ?NDEBUG
CFLAGS += -std=gnu18 -fPIC -fexceptions -fp-model precise -fprotect-parens -fma -no-ftz -qopenmp -pthread -fno-omit-frame-pointer -traceback -xHost
LDFLAGS=-rdynamic -ldl -lm
ifeq ($(OS),Linux)
LDFLAGS += -static-libgcc
endif # Linux

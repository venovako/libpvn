ARFLAGS=rsv
CC=$(COMPILER_PREFIX)gcc$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG) -fno-math-errno
else # DEBUG
PFLAGS=-DPVN_DEBUG=STDERR_FILENO
CFLAGS=-Og -ggdb3 -ftrapv
endif # ?NDEBUG
ifeq ($(OS),Linux)
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 15 ]; then echo 2y; else echo 2x; fi) -fPIC -ffp-contract=fast -fvect-cost-model=unlimited -pthread
ifndef VECLEN
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer
endif # !VECLEN
ifdef OPENMP
PFLAGS += -DPVN_OPENMP=$(OPENMP)
CFLAGS += -fopenmp
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
CFLAGS += -Wall -Wextra -Wno-stringop-truncation
ifeq ($(findstring MINGW64,$(OS)),MINGW64)
PFLAGS += -DPVN_EXTERN_C=EXTERN_C -DPVN_MINGW64=UCRT64
endif # ?MINGW64

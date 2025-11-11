ARFLAGS=rsv
CC=$(COMPILER_PREFIX)clang$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG) -fno-math-errno
else # DEBUG
PFLAGS=-DPVN_DEBUG=STDERR_FILENO
CFLAGS=-Og -g -ftrapv
endif # ?NDEBUG
ifeq ($(OS),Linux)
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 19 ]; then echo 2y; else echo 18; fi) -fPIC -ffp-contract=fast -pthread
ifndef VECLEN
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer
endif # !VECLEN
ifdef OPENMP
ifeq ($(OPENMP),cilk)
PFLAGS += -DPVN_CILK=3
CFLAGS += -fopencilk -fopenmp-simd
else # !cilk
PFLAGS += -DPVN_OPENMP=$(OPENMP)
CFLAGS += -fopenmp
endif # ?cilk
else # !OPENMP
CFLAGS += -fopenmp-simd
endif # ?OPENMP
ifndef MARCH
MARCH=native
endif # !MARCH
ifeq ($(findstring BSD,$(OS)),BSD)
ifeq ($(ARCH),arm64)
CFLAGS += -mcpu=$(MARCH)
else # !arm64
CFLAGS += -march=$(MARCH)
endif # ?arm64
else # !BSD
CFLAGS += -march=$(MARCH)
endif # ?BSD

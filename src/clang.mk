ARFLAGS=rsv
CC=$(COMPILER_PREFIX)clang$(COMPILER_SUFFIX)
CXX=$(COMPILER_PREFIX)clang$(COMPILER_SUFFIX)
FC=$(COMPILER_PREFIX)flang$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG) -fno-math-errno
CXXFLAGS=-O$(NDEBUG) -fno-math-errno
FCFLAGS=-cpp -O$(NDEBUG)
else # DEBUG
PFLAGS=-DPVN_DEBUG=STDERR_FILENO
CFLAGS=-Og -g
CXXFLAGS=-Og -g
FCFLAGS=-cpp -O0 -g
endif # ?NDEBUG
ifeq ($(OS),Linux)
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 19 ]; then echo 2y; else echo 18; fi) -fPIC -ffp-contract=fast -pthread
CXXFLAGS += -std=gnu++$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 19 ]; then echo 2c; else echo 2b; fi) -fPIC -ffp-contract=fast -pthread
FCFLAGS += -fPIC -ffp-contract=fast -fimplicit-none -fhonor-infinities -fhonor-nans -fstack-arrays -fvectorize -fslp-vectorize -pthread
ifndef VECLEN
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer
CXXFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer
FCFLAGS += -fno-omit-frame-pointer
endif # !VECLEN
ifdef OPENMP
ifeq ($(OPENMP),cilk)
PFLAGS += -DPVN_CILK=3
CFLAGS += -fopencilk -fopenmp-simd
CXXFLAGS += -fopencilk -fopenmp-simd
else # !cilk
PFLAGS += -DPVN_OPENMP=$(OPENMP)
CFLAGS += -fopenmp
CXXFLAGS += -fopenmp
FCFLAGS += -fopenmp
endif # ?cilk
else # !OPENMP
CFLAGS += -fopenmp-simd
CXXFLAGS += -fopenmp-simd
endif # ?OPENMP
ifndef MARCH
MARCH=native
endif # !MARCH
ifeq ($(findstring BSD,$(OS)),BSD)
ifeq ($(ARCH),arm64)
CFLAGS += -mcpu=$(MARCH)
CXXFLAGS += -mcpu=$(MARCH)
FCFLAGS += -mcpu=$(MARCH)
else # !arm64
CFLAGS += -march=$(MARCH)
CXXFLAGS += -march=$(MARCH)
FCFLAGS += -march=$(MARCH)
endif # ?arm64
else # !BSD
CFLAGS += -march=$(MARCH)
CXXFLAGS += -march=$(MARCH)
FCFLAGS += -march=$(MARCH)
endif # ?BSD

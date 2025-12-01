ARFLAGS=rsv
CC=$(COMPILER_PREFIX)gcc$(COMPILER_SUFFIX)
CXX=$(COMPILER_PREFIX)g++$(COMPILER_SUFFIX)
FC=$(COMPILER_PREFIX)gfortran$(COMPILER_SUFFIX)
FCFLAGS=-cpp -ffree-line-length-none
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG) -fno-math-errno
CXXFLAGS=-O$(NDEBUG) -fno-math-errno
FCFLAGS += -O$(NDEBUG) -fno-math-errno
else # DEBUG
PFLAGS=-DPVN_DEBUG=STDERR_FILENO
CFLAGS=-Og -ggdb3
CXXFLAGS=-Og -ggdb3
FCFLAGS += -Og -ggdb3 -finit-local-zero -finit-derived -fcheck=all
endif # ?NDEBUG
ifeq ($(OS),Linux)
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 15 ]; then echo 2y; else echo 2x; fi) -fPIC -ffp-contract=fast -fvect-cost-model=unlimited -pthread
CXXFLAGS += -std=gnu++$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 14 ]; then echo 2c; else echo 2b; fi) -fPIC -ffp-contract=fast -fvect-cost-model=unlimited -pthread
FCFLAGS += -fPIC -ffp-contract=fast -fprotect-parens -frecursive -fstack-arrays -fvect-cost-model=unlimited -pthread
ifndef VECLEN
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer
CXXFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer
FCFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer
endif # !VECLEN
ifdef OPENMP
PFLAGS += -DPVN_OPENMP=$(OPENMP)
CFLAGS += -fopenmp
CXXFLAGS += -fopenmp
FCFLAGS += -fopenmp
else # !OPENMP
CFLAGS += -fopenmp-simd
CXXFLAGS += -fopenmp-simd
FCFLAGS += -fopenmp-simd
endif # ?OPENMP
ifndef MARCH
MARCH=native
endif # !MARCH
ifeq ($(ARCH),ppc64le)
CFLAGS += -mcpu=$(MARCH) -mpower8-fusion -mtraceback=full
CXXFLAGS += -mcpu=$(MARCH) -mpower8-fusion -mtraceback=full
FCFLAGS += -mcpu=$(MARCH) -mpower8-fusion -mtraceback=full
else # !ppc64le
CFLAGS += -march=$(MARCH)
CXXFLAGS += -march=$(MARCH)
FCFLAGS += -march=$(MARCH)
endif # ?ppc64le
CFLAGS += -Wall -Wextra -Wno-stringop-truncation
CXXFLAGS += -Wall -Wextra -Wno-stringop-truncation
ifeq ($(findstring MINGW64,$(OS)),MINGW64)
PFLAGS += -DPVN_EXTERN_C=EXTERN_C -DPVN_MINGW64=UCRT64
endif # ?MINGW64

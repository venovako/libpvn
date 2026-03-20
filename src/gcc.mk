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
CFLAGS += -std=gnu$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 15 ]; then echo 2y; else echo 2x; fi) -fPIC -pthread -fvect-cost-model=unlimited
CXXFLAGS += -std=gnu++$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 14 ]; then echo 2c; else echo 2b; fi) -fPIC -pthread -fvect-cost-model=unlimited
FCFLAGS += -fPIC -pthread -fvect-cost-model=unlimited -frecursive -fstack-arrays
ifdef STRICT
PFLAGS += -DPVN_STRICT=$(STRICT)
ifneq ($(STRICT),0)
CFLAGS += -frounding-math -fsignaling-nans
CXXFLAGS += -frounding-math -fsignaling-nans
FCFLAGS += -frounding-math -fsignaling-nans
endif # ?STRICT
endif # STRICT
CFLAGS += -ffp-contract=fast
CXXFLAGS += -ffp-contract=fast
FCFLAGS += -ffp-contract=fast -fprotect-parens
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
ifdef SLEEF
ifneq ($(sleef),0)
CFLAGS += -Wno-unknown-pragmas -Wno-unused-function -Wno-unused-parameter
CXXFLAGS += -Wno-unknown-pragmas -Wno-unused-function -Wno-unused-parameter
endif # ?sleef
endif # SLEEF
ifeq ($(findstring MINGW64,$(OS)),MINGW64)
PFLAGS += -DPVN_EXTERN_C=EXTERN_C -DPVN_MINGW64=UCRT64
endif # ?MINGW64

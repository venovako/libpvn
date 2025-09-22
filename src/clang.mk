ARFLAGS=rsv
CC=$(COMPILER_PREFIX)clang$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG) -fno-math-errno
else # DEBUG
PFLAGS=
CFLAGS=-Og -g -ftrapv
endif # ?NDEBUG
ifeq ($(OS),Linux)
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu17 -fPIC -ffp-contract=fast -pthread
ifndef LAPACK
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer
endif # !LAPACK
ifdef OPENMP
ifeq ($(OPENMP),cilk)
PFLAGS += -DPVN_CILK=3
CFLAGS += -fopencilk
else # !cilk
CFLAGS += -fopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
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

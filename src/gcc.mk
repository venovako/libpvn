ARFLAGS=rsv
CC=$(COMPILER_PREFIX)gcc$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG) -fno-math-errno -fvect-cost-model=unlimited
else # DEBUG
PFLAGS=
CFLAGS=-Og -ggdb3 -ftrapv
endif # ?NDEBUG
CFLAGS += -Wall -Wextra -Wno-stringop-truncation
ifeq ($(OS),Linux)
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu$(shell if [ `$(CC) -dumpversion | cut -f1 -d.` -ge 14 ]; then echo 23; else echo 18; fi) -fPIC -ffp-contract=fast -pthread
ifndef LAPACK
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer
endif # !LAPACK
ifdef OPENMP
CFLAGS += -fopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
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

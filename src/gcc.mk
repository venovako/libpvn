ARFLAGS=rsv
CC=$(COMPILER_PREFIX)gcc$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno -fopt-info-optimized-vec -fvect-cost-model=unlimited
else # DEBUG
CFLAGS=-Og -ggdb -ftrapv
endif # ?NDEBUG
ifeq ($(OS),Linux)
CFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu18 -fPIC -fexceptions -ffp-contract=fast -fno-omit-frame-pointer -fopenmp-simd -pthread #-fopenmp
ifeq ($(ARCH),ppc64le)
CFLAGS += -mcpu=native -mtraceback=full
else # !ppc64le
CFLAGS += -march=native
endif # ?ppc64le
LDFLAGS=-rdynamic
ifeq ($(findstring BSD,$(OS)),BSD)
LDFLAGS += -lexecinfo
else # !BSD
LDFLAGS += -ldl
endif # ?BSD
LDFLAGS += -lm

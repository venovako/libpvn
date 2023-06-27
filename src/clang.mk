ARFLAGS=rsv
CC=$(COMPILER_PREFIX)clang$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno
else # DEBUG
CFLAGS=-Og -g -ftrapv
endif # ?NDEBUG
CFLAGS += -std=gnu17 -fPIC -fexceptions -ffp-contract=fast -fno-omit-frame-pointer -fopenmp-simd -pthread -march=native
LDFLAGS=-rdynamic
ifeq ($(findstring BSD,$(OS)),BSD)
LDFLAGS += -lexecinfo
else # !BSD
LDFLAGS += -ldl
endif # ?BSD
LDFLAGS += -lm

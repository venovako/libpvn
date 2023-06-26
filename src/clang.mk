ARFLAGS=rsv
CC=$(COMPILER_PREFIX)clang$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno
else # DEBUG
CFLAGS=-Og -g -ftrapv
endif # ?NDEBUG
CFLAGS += -std=gnu17 -fPIC -fexceptions -ffp-contract=fast -fno-omit-frame-pointer -march=native
ifeq ($(OS),Linux)
CFLAGS += -fopenmp
else # !Linux
CFLAGS += -fopenmp-simd
endif # ?Linux
ifdef CFLAGS_SUFFIX
CFLAGS += $(CFLAGS_SUFFIX)
endif # CFLAGS_SUFFIX
LDFLAGS=-rdynamic

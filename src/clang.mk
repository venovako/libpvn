ARFLAGS=rsv
CC=$(COMPILER_PREFIX)clang$(COMPILER_SUFFIX)
CFLAGS=-std=gnu17 -fPIC -fexceptions -ffp-contract=fast -march=native
ifeq ($(OS),Linux)
CFLAGS += -fopenmp
else # !Linux
CFLAGS += -fopenmp-simd
endif # ?Linux
ifdef NDEBUG
CFLAGS += -DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno
else # DEBUG
CFLAGS += -Og -g -ftrapv
endif # ?NDEBUG
ifdef CFLAGS_SUFFIX
CFLAGS += $(CFLAGS_SUFFIX)
endif # CFLAGS_SUFFIX
LDFLAGS=-rdynamic

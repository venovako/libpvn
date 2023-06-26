ARFLAGS=rsv
CC=$(COMPILER_PREFIX)clang$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno
else # DEBUG
CFLAGS=-Og -g -ftrapv
endif # ?NDEBUG
CFLAGS += -std=gnu17 -fPIC -fexceptions -ffp-contract=fast -fno-omit-frame-pointer -march=native -pthread
ifeq ($(OS),Linux)
ifeq ($(ARCH),ppc64le)
CFLAGS += -fopenmp-simd
else # !ppc64le
CFLAGS += -fopenmp
endif # ?ppc64le
else # !Linux
CFLAGS += -fopenmp-simd
endif # ?Linux
ifdef CFLAGS_SUFFIX
CFLAGS += $(CFLAGS_SUFFIX)
endif # CFLAGS_SUFFIX
LDFLAGS=-rdynamic
ifeq ($(findstring BSD,$(OS)),BSD)
LDFLAGS += -lexecinfo
else # !BSD
LDFLAGS += -ldl
endif # ?BSD
LDFLAGS += -lm

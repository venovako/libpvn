ARFLAGS=rsv
CC=$(COMPILER_PREFIX)clang$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno
else # DEBUG
CFLAGS=-Og -g -ftrapv
endif # ?NDEBUG
ifeq ($(OS),Linux)
CFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
endif # Linux
CFLAGS += -std=gnu17 -fPIC -fexceptions -ffp-contract=fast -fno-omit-frame-pointer -fopenmp-simd -pthread
ifeq ($(ARCH),arm64)
CFLAGS += -mcpu=native
else # !arm64
CFLAGS += -march=native
endif # ?arm64
LDFLAGS=-rdynamic
ifeq ($(findstring BSD,$(OS)),BSD)
LDFLAGS += -lexecinfo
else # !BSD
LDFLAGS += -ldl
endif # ?BSD
LDFLAGS += -lm

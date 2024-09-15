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
CFLAGS += -std=gnu17 -fPIC -fexceptions -fasynchronous-unwind-tables -ffp-contract=fast -fno-omit-frame-pointer -fopenmp-simd -pthread
ifdef OPENMP
CFLAGS += -fopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
endif # OPENMP
LDFLAGS=-rdynamic
ifndef CPU
CPU=native
endif # !CPU
ifeq ($(findstring BSD,$(OS)),BSD)
ifeq ($(ARCH),arm64)
CFLAGS += -mcpu=native
else # !arm64
CFLAGS += -march=native
endif # ?arm64
LDFLAGS += -lexecinfo
else # !BSD
CFLAGS += -march=native
LDFLAGS += -ldl
endif # ?BSD
LDFLAGS += -lm

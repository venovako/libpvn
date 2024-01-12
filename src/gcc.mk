ARFLAGS=rsv
CC=$(COMPILER_PREFIX)gcc$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG) -fno-math-errno -fvect-cost-model=unlimited
else # DEBUG
CFLAGS=-Og -ggdb3 -ftrapv
endif # ?NDEBUG
CFLAGS += -Wall -Wextra
ifeq ($(OS),Linux)
CFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
else # !Linux
ifeq ($(OS),SunOS)
CFLAGS += -D_LARGEFILE64_SOURCE -m64 -Wno-int-to-pointer-cast
endif # SunOS
endif # ?Linux
CFLAGS += -std=gnu18 -fPIC -fexceptions -ffp-contract=fast -fno-omit-frame-pointer -fopenmp-simd -pthread
ifdef OPENMP
CFLAGS += -fopenmp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
endif # OPENMP
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
ifdef QUADMATH
CFLAGS += -DPVN_QUADMATH="\"$(QUADMATH)\""
LDFLAGS += $(QUADMATH)
endif # QUADMATH
LDFLAGS += -lm

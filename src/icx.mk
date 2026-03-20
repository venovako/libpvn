AR=ar
ARFLAGS=rsv
CC=$(COMPILER_PREFIX)icx$(COMPILER_SUFFIX)
CXX=$(COMPILER_PREFIX)icpx$(COMPILER_SUFFIX)
FC=$(COMPILER_PREFIX)ifx$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG) -fno-math-errno -qopt-report=3
CXXFLAGS=-O$(NDEBUG) -fno-math-errno -qopt-report=3
FCFLAGS=-fpp -O$(NDEBUG) -fno-math-errno -qopt-report=3
ifndef PROFILE
CFLAGS += -inline-level=2
CXXFLAGS += -inline-level=2
FCFLAGS += -inline-level=2
endif # !PROFILE
else # DEBUG
PFLAGS=-DPVN_DEBUG=STDERR_FILENO
CFLAGS=-O0 -g3
CXXFLAGS=-O0 -g3
FCFLAGS=-fpp -O0 -g3 -init=arrays,zero -check all
endif # ?NDEBUG
ifndef MARCH
MARCH=Host
# common-avx512 for KNLs
endif # !MARCH
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
CFLAGS += -std=gnu2x -fPIC -pthread -mprefer-vector-width=512 -vec-threshold0 -x$(MARCH)
CXXFLAGS += -std=gnu++2b -fPIC -pthread -mprefer-vector-width=512 -vec-threshold0 -x$(MARCH)
FCFLAGS += -fPIC -threads -recursive -standard-semantics -mprefer-vector-width=512 -vec-threshold0 -x$(MARCH)
ifdef STRICT
PFLAGS += -DPVN_STRICT=$(STRICT)
CFLAGS += -fp-model=strict
CXXFLAGS += -fp-model=strict
FCFLAGS += -fp-model=strict
else # !STRICT
CFLAGS += -fp-model=precise
CXXFLAGS += -fp-model=precise
FCFLAGS += -fp-model=precise
endif # ?STRICT
CFLAGS += -fp-speculation=safe -ffp-contract=fast -fma -no-ftz -fprotect-parens
CXXFLAGS += -fp-speculation=safe -ffp-contract=fast -fma -no-ftz -fprotect-parens
FCFLAGS += -fp-speculation=safe -ffp-contract=fast -fma -no-ftz -fprotect-parens
ifndef VECLEN
CFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer -traceback
CXXFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer -traceback
FCFLAGS += -fexceptions -fasynchronous-unwind-tables -fno-omit-frame-pointer -traceback
endif # !VECLEN
ifdef OPENMP
PFLAGS += -DPVN_OPENMP=$(OPENMP)
CFLAGS += -qopenmp
CXXFLAGS += -qopenmp
FCFLAGS += -qopenmp
else # !OPENMP
CFLAGS += -qopenmp-simd
CXXFLAGS += -qopenmp-simd
FCFLAGS += -qopenmp-simd
endif # ?OPENMP
CFLAGS += -Wno-overriding-option
CXXFLAGS += -Wno-overriding-option
ifdef STATIC
# 1=scalar, 2=simd, 3=both
PFLAGS += -DPVN_INTEL=3
endif # STATIC

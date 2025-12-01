ARFLAGS=rsv
CC=$(COMPILER_PREFIX)nvc$(COMPILER_SUFFIX)
CXX=$(COMPILER_PREFIX)nvc++$(COMPILER_SUFFIX)
FC=$(COMPILER_PREFIX)nvfortran$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG)
CXXFLAGS=-O$(NDEBUG)
FCFLAGS=-Mpreprocess -O$(NDEBUG)
else # DEBUG
PFLAGS=-DPVN_DEBUG=STDERR_FILENO
CFLAGS=-O0 -g -Mbounds -Mchkstk
CXXFLAGS=-O0 -g -Mbounds -Mchkstk
FCFLAGS=-Mpreprocess -O0 -g -Mbounds -Mchkptr -Mchkstk
endif # ?NDEBUG
ifndef MARCH
MARCH=native
endif # !MARCH
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
CFLAGS += -c23 -fpic -Kieee -Mcache_align -Mfma -Minfo -Mint128 -Mm128 -Mnodaz -Mnoflushz -Mnofpapprox -Mnofprelaxed -Mno-recip-div -nvmalloc -pthread -tp=$(MARCH)
CXXFLAGS += -std=c++20 -fpic -Kieee -Mcache_align -Mfma -Minfo -Mint128 -Mm128 -Mnodaz -Mnoflushz -Mnofpapprox -Mnofprelaxed -Mno-recip-div -nvmalloc -pthread -tp=$(MARCH)
FCFLAGS += -fpic -Kieee -Mcache_align -Mdclchk -Mfma -Mnodaz -Mnoflushz -Mnofpapprox -Mnofprelaxed -Mno-recip-div -Mlarge_arrays -Mrecursive -Mstack_arrays -nvmalloc -pthread -tp=$(MARCH)
ifndef VECLEN
CFLAGS += -Meh_frame -Mframe -traceback
CXXFLAGS += -Meh_frame -Mframe -traceback
FCFLAGS += -Meh_frame -Mframe -traceback
endif # !VECLEN
ifdef OPENMP
PFLAGS += -DPVN_OPENMP=$(OPENMP)
CFLAGS += -mp
CXXFLAGS += -mp
FCFLAGS += -mp
endif # OPENMP
CFLAGS += --diag_suppress integer_sign_change
CXXFLAGS += --diag_suppress integer_sign_change

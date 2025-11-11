ARFLAGS=rsv
CC=$(COMPILER_PREFIX)nvc$(COMPILER_SUFFIX)
ifdef NDEBUG
PFLAGS=-DNDEBUG=$(NDEBUG)
CFLAGS=-O$(NDEBUG)
else # DEBUG
PFLAGS=-DPVN_DEBUG=STDERR_FILENO
CFLAGS=-O0 -g -Mbounds -Mchkstk
endif # ?NDEBUG
ifndef MARCH
MARCH=native
endif # !MARCH
PFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE
CFLAGS += -c18 -fpic -Kieee -Mfma -Minfo -Mint128 -Mm128 -Mnodaz -Mnoflushz -Mnofpapprox -Mnofprelaxed -Mno-recip-div -nvmalloc -pthread -tp=$(MARCH)
ifndef VECLEN
CFLAGS += -Meh_frame -Mframe -traceback
endif # !VECLEN
ifdef OPENMP
PFLAGS += -DPVN_OPENMP=$(OPENMP)
CFLAGS += -mp
endif # OPENMP
CFLAGS += --diag_suppress integer_sign_change

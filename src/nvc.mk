ARFLAGS=rsv
CC=$(COMPILER_PREFIX)nvc$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG=$(NDEBUG) -O$(NDEBUG)
else # DEBUG
CFLAGS=-O0 -g -Mbounds -Mchkstk
endif # ?NDEBUG
ifndef MARCH
MARCH=native
endif # !MARCH
CFLAGS += --diag_suppress integer_sign_change -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -c18 -fpic -Kieee -Mfma -Minfo -Mint128 -Mm128 -Mnodaz -Mnoflushz -Mnofpapprox -Mnofprelaxed -Mno-recip-div -nvmalloc -pthread -tp=$(MARCH)
ifndef LAPACK
CFLAGS += -Meh_frame -Mframe -traceback
endif # !LAPACK
ifdef OPENMP
CFLAGS += -mp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
endif # OPENMP
LDFLAGS=-Wl,-E -static-nvidia -ldl -lm

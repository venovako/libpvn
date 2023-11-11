ARFLAGS=rsv
CC=$(COMPILER_PREFIX)nvc$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG -O$(NDEBUG)
else # !NDEBUG
CFLAGS=-O0 -g -Mbounds -Mchkstk
endif # ?NDEBUG
CFLAGS += -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -KPIC -Mframe -Meh_frame -Minfo -Mlarge_arrays -Mstack_arrays -Kieee -Mfma -Mnodaz -Mnoflushz -Mnofpapprox -Mnofprelaxed -pthread -traceback
ifdef OPENMP
CFLAGS += -mp
ifneq ($(OPENMP),true)
CFLAGS += $(OPENMP)
endif # !true
endif # OPENMP
LDFLAGS=-Wl,-E -ldl -lm

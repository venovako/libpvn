ARFLAGS=rsv
CC=$(COMPILER_PREFIX)nvc$(COMPILER_SUFFIX)
ifdef NDEBUG
CFLAGS=-DNDEBUG -O$(NDEBUG)
else # !NDEBUG
CFLAGS=-O0 -g -Mbounds -Mchkstk
endif # ?NDEBUG
CFLAGS += -mp -KPIC -Mframe -Meh_frame -Minfo -Mlarge_arrays -Mstack_arrays -Kieee -Mfma -Mnodaz -Mnoflushz -Mnofpapprox -Mnofprelaxed -traceback
ifdef CFLAGS_SUFFIX
CFLAGS += $(CFLAGS_SUFFIX)
endif # CFLAGS_SUFFIX
LDFLAGS=-Wl,-export-dynamic -lm

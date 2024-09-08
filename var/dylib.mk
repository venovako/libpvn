# add to src/GNUmakefile

ifeq ($(OS),Darwin)
LIBS += libpvn.dylib
ifeq ($(realpath /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/System.framework),)
FSYS=/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks
else # Xcode
FSYS=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks
endif # ?Xcode
ifeq ($(COMPILER),gcc)
ifeq ($(QUADMATH),-lquadmath)
GNU=-L$(dir $(realpath $(shell $(CC) -print-file-name=libquadmath.so))) -lquadmath
else # libquadmath.a
GNU=-load_hidden $(QUADMATH)
endif # ?libquadmath
GNU += -load_hidden $(realpath $(shell $(CC) -print-file-name=libgcc.a))
endif # gcc
endif # Darwin

libpvn.dylib: libpvn.a
	libtool -dynamic -o $@ -v $(GNU) -F$(FSYS) -framework System libpvn.a

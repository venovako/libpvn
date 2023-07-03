# libpvn
A small portable C library with several utility functions.

(... work in progress ...)

Some functions have been adapted from those in the repositories [JACSD](https://github.com/venovako/JACSD) and [VecJac](https://github.com/venovako/VecJac).

## Building

The library has been successfully built using:
| compiler | `uname` | platform |
| -------- | ------- | -------- |
| clang(1) | Darwin  | x86_64   |
| clang(2) | Linux   | ppc64le  |
| clang(3) | FreeBSD | amd64    |
| clang(4) | OpenBSD | amd64    |
| gcc(5)   | Darwin  | x86_64   |
| gcc(6)   | Linux   | ppc64le  |
| gcc(7)   | Linux   | x86_64   |
| icc(8)   | Darwin  | x86_64   |
| icc(8)   | Linux   | x86_64   |
| icx(8)   | Linux   | x86_64   |
| nvc(9)   | Linux   | ppc64le  |
| nvc(9)   | Linux   | x86_64   |

Recent versions of the compilers have been povided by:
1. Apple,
2. IBM Open XL C/C++,
3. FreeBSD,
4. OpenBSD,
5. Homebrew,
6. openSUSE,
7. Oracle Linux,
8. Intel oneAPI,
9. NVIDIA HPC SDK.

Examples of building the library:
```bash
cd src
# query the building options (GNU make is necessary)
make help
# the output should be something like:
# make [COMPILER=clang|gcc|icc|icx|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [NDEBUG=0|1|2|3|...] [VECLEN=...] [all|clean|help]
# where gcc is the default compiler to be used on Linux, and clang is otherwise
#
# non-debug build with icx on x86_64
make COMPILER=icx NDEBUG=3 clean all
#
# debug build with clang on ppc64le
make COMPILER=clang COMPILER_PREFIX=ibm- COMPILER_SUFFIX=_r clean all
#
# debug build with clang on Free/OpenBSD (note the usage of gmake instead of make)
gmake clean all
```

## Using

Include the `pvn.h` header file in your C/C++ sources where needed and link with the `libpvn.a` static library, as shown in `GNUmakefile`.
There are no global nor static variables present, and the functions can safely be called from multiple threads on *unrelated* data without locking.
You might wish to tune the compiler flags to match yours, especially if you are building a static executable and/or not using OpenMP.

Caveat: the library has never been tested on a big-endian system!

## Examples

Several examples and tests are built as `*.exe` executables in the `src` subdirectory and are meant to be run from there.

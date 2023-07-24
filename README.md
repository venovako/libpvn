# libpvn
A small portable C library with several utility functions.

(... work in progress ...)

Some functions have been adapted from those in the repositories [JACSD](https://github.com/venovako/JACSD) and [VecJac](https://github.com/venovako/VecJac).

## Building

The library has been successfully built using:
| compiler | `uname` | platform |
| -------- | ------- | -------- |
| clang(1) | Darwin  | x86_64   |
| clang(2) | FreeBSD | amd64    |
| clang(3) | Linux   | ppc64le  |
| clang(4) | OpenBSD | amd64    |
| gcc(5)   | Darwin  | x86_64   |
| gcc(6)   | Linux   | ppc64le  |
| gcc(7)   | Linux   | x86_64   |
| gcc(8)   | SunOS   | i86pc    |
| icc(9)   | Darwin  | x86_64   |
| icc(9)   | Linux   | x86_64   |
| icl(9)   | Windows | x64      |
| icx(9)   | Linux   | x86_64   |
| icx(9)   | Windows | x64      |
| nvc(10)  | Linux   | ppc64le  |
| nvc(10)  | Linux   | x86_64   |

Recent versions of the compilers have been povided by:
 1. Apple,
 2. FreeBSD,
 3. IBM Open XL C/C++,
 4. OpenBSD,
 5. Homebrew,
 6. openSUSE,
 7. Oracle Linux,
 8. Oracle Solaris,
 9. Intel oneAPI,
10. NVIDIA HPC SDK.

Examples of building the library:
```bash
cd src
# query the building options (GNU make is necessary everywhere except on Windows)
make help
# the output should be something like:
# make [COMPILER=clang|gcc|icc|icx|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [NDEBUG=0|1|2|3|...] [VECLEN=...] [all|clean|help]
# where gcc is the default compiler to be used on Linux, and clang is otherwise
#
# a release build with icx on x86_64
make COMPILER=icx NDEBUG=3 clean all
#
# a debug build with clang on ppc64le
make COMPILER=clang COMPILER_PREFIX=ibm- COMPILER_SUFFIX=_r clean all
#
# a debug build with clang on Free/OpenBSD (note the usage of gmake instead of make)
gmake clean all
#
# a release build on Windows (note the usage of nmake instead of make)
nmake NDEBUG=3 clean all
```

## Using

Include the `pvn.h` header file in your C/C++ sources where needed and link with the `libpvn.a` static library, as shown in `GNUmakefile`.
There are no mutable global/static variables present, but some static constant lookup tables are used.
The functions can safely be called from multiple threads on *unrelated* data without locking.
You might wish to tune the compiler flags to match yours, especially if you are building a static executable and/or using OpenMP.

Fortran (column-major) array order is assumed for the functions that operate on matrices.

A function with the name ending with an underscore (`_`) should be callable from Fortran.
Explicit interfaces are not needed, *except on Windows*, but are recommended to be written.

On Windows, the library is called `pvn.lib` and does *not* contain all routines.
Please consult `Makefile` for more information.

*Caveat*: the library has never been tested on a big-endian system and certain parts are likely not to work there.

## Examples

Several examples and tests are built as `*.exe` executables in the `src` subdirectory and are meant to be run from there.

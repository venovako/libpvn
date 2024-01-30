# libpvn
A small portable C library with several utility functions.

(... work in progress ...)

This software is a supplementary material for the preprint
arXiv:[2308.14222](https://arxiv.org/abs/2308.14222 "Accurate complex Jacobi rotations") \[math.NA\].

Some functions have been adapted from those in the repositories [JACSD](https://github.com/venovako/JACSD) and [VecJac](https://github.com/venovako/VecJac).

## Building

The library has been successfully built using:
| compiler | `uname` | platform |
| -------- | ------- | -------- |
| clang(1) | Darwin  | x86_64   |
| clang(2) | FreeBSD | amd64    |
| clang(2) | FreeBSD | arm64    |
| clang(3) | Linux   | ppc64le  |
| gcc(4)   | Darwin  | x86_64   |
| gcc(2)   | FreeBSD | amd64    |
| gcc(2)   | FreeBSD | arm64    |
| gcc(5)   | Linux   | ppc64le  |
| gcc(6)   | Linux   | x86_64   |
| gcc(7)   | SunOS   | i86pc    |
| icc(8)   | Darwin  | x86_64   |
| icc(8)   | Linux   | x86_64   |
| icl(8)   | Windows | x64      |
| icx(8)   | Linux   | x86_64   |
| icx(8)   | Windows | x64      |
| nvc(9)   | Linux   | ppc64le  |
| nvc(9)   | Linux   | x86_64   |

Recent versions of the compilers have been povided by:
1. Apple,
2. FreeBSD,
3. IBM Open XL C/C++,
4. Homebrew,
5. openSUSE,
6. Oracle Linux,
7. Oracle Solaris,
8. Intel oneAPI,
9. NVIDIA HPC SDK.

Examples of building the library:
```bash
cd src
# query the building options (GNU make is necessary everywhere except on Windows)
make help
# the output should be something like:
# make [COMPILER=clang|gcc|icx|nvc|icc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [NDEBUG=0|1|2|3|...] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [QUADMATH=-lquadmath] [all|clean|help]
# where gcc is the default compiler to be used on Linux, and clang is otherwise
#
# a release build with icx on x86_64
make COMPILER=icx NDEBUG=3 clean all
#
# a debug build with clang on ppc64le
make COMPILER=clang COMPILER_PREFIX=ibm- COMPILER_SUFFIX=_r clean all
#
# a debug build with clang on FreeBSD (note the usage of gmake instead of make)
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

A function with the name ending with an underscore (`_`) should be callable from Fortran without an explicit interface, just by declaring it `EXTERNAL` (without the underscore).

The correctly-rounded `cr_hypot[f]` and `cr_rsqrt[f]` functions might optionally be used if provided by, e.g., the [CORE-MATH](https://core-math.gitlabpages.inria.fr) project.
If their implementation is to be linked with, set the `CR_MATH` variable in a `[g]make` invocation to the cloned `core-math` source code directory path.
Note, the `hypot*_noerrno.c` files are not provided there but can be easily modified from the corresponding `hypot*.c` files by eliminating all references to `errno` (see the `inc` subdirectory here for examples).
Then, `hypotf*.c` in `src/binary32/hypot` and `hypot*.c` in `src/binary64/hypot` subdirectories of the cloned CORE-MATH repository have to be compiled manually.
The same holds for `rsqrt*_noerrno.c` and `rsqrt*.c` files and `src/binary32/rsqrt` and `src/binary64/rsqrt` subdirectories.
References to more core-math functions might be added soon, and will follow the same pattern.

The `OPENMP` option enables OpenMP and its content is appended to the compiler's flags.
Set it to `true` if no additional compiler flags are desired.

The `QUADMATH` option should be set to the name of the library implementing quadruple precision arithmetic, if `long double` is not the 128-bit floating-point datatype.
This makes sense on, e.g., the x86_64 platform, where, for `gcc`, `QUADMATH=-lquadmath`.
For `icx` and `icc` it is not necessary, since the `imf` library is implicitly linked with.

On Windows, the library is called `pvn.lib` and does *not* contain all routines.
Please consult `Makefile` for more information.

*Caveat*: certain parts of the library will *not* work on big-endian systems!

## Examples

Several examples and tests are built as `*.exe` executables in the `src` subdirectory and are meant to be run from there.

This work has been supported in part by Croatian Science Foundation under the project IP-2014-09-3670 ([MFBDA](https://web.math.pmf.unizg.hr/mfbda/)).

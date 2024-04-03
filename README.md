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
| gcc(3)   | Darwin  | x86_64   |
| gcc(2)   | FreeBSD | amd64    |
| gcc(2)   | FreeBSD | arm64    |
| gcc(4)   | Linux   | aarch64  |
| gcc(4)   | Linux   | ppc64le  |
| gcc(5)   | Linux   | x86_64   |
| gcc(6)   | SunOS   | i86pc    |
| icx(7)   | Linux   | x86_64   |

Recent versions of the compilers have been povided by:
1. Apple,
2. FreeBSD,
3. Homebrew,
4. openSUSE Tumbleweed,
5. Oracle Linux,
6. Oracle Solaris,
7. Intel oneAPI.

Examples of building the library:
```bash
cd src
# query the building options (GNU make is necessary)
make help
# the output should be something like:
# make [COMPILER=clang|gcc|icx] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [NDEBUG=0|1|2|3|...] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [QUADMATH=...] [PROFILE=...] [all|clean|help]
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
```

## Using

Include the `pvn.h` header file in your C/C++ sources where needed and link with the `libpvn.a` static library, as shown in `GNUmakefile`.
There are no mutable global/static variables present, *unless profiling has been enabled*, but some static constant lookup tables are used.
The functions can safely be called from multiple threads on *unrelated* data without locking.
You might wish to tune the compiler flags to match yours, especially if you are building a dynamic library, a static executable, and/or want to use OpenMP.

Fortran (column-major) array order is assumed for the functions that operate on matrices.

A function with the name ending with an underscore (`_`) should be callable from Fortran without an explicit interface, just by declaring it `EXTERNAL` (without the underscore).

The correctly-rounded `cr_hypot[f]` and `cr_rsqrt[f]` functions might optionally be used if provided by, e.g., the [CORE-MATH](https://core-math.gitlabpages.inria.fr) project.
If their implementation is to be linked with, set the `CR_MATH` variable in a `[g]make` invocation to the cloned `core-math` source code directory path.
Note, the `hypot[f]_noerrno.c` and `rsqrt[f]_noerrno.c` files are not provided there but can be easily modified from the corresponding `hypot[f].c` and `rsqrt[f].c` files by conditionally eliminating all references to `errno` (see the `inc` subdirectory here for examples).
Alternatively, the modified implementations provided here will be used if `CR_MATH` is not set.
If the object files have not been prepared beforehand, the source files will be compiled in either case.
The object files in the `inc` subdirectory will be *deleted* by `[g]make clean`, so either back them up or edit `GNUmakefile` if that is not desirable.
Either way, the object files will be integrated into `libpvn.a` for easier re-use by other software linked with it.

The `OPENMP` option enables OpenMP and its content is appended to the compiler's flags.
Set it to `true` if no additional compiler flags are desired.
Bear in mind, however, that this will introduce a dependency on the OpenMP runtime library!

If `long double` is not the 128-bit floating-point datatype, the `PVN_QUADMATH` macro should be set automatically to the name of a library implementing quadruple precision arithmetic, unless `COMPILER=clang` is used.
If the autodetection fails, the relevant linker options can be set via the `QUADMATH` variable.

*Caveat*: certain parts of the library will *not* work on big-endian systems!
Also, several functions will not work on Windows, and probably on other untested systems as well.

## Examples

Several examples and tests are built as `*.exe` executables in the `src` subdirectory and are meant to be run from there.

## Documentation

...is a work in progress.

This work has been supported in part by Croatian Science Foundation under the project IP-2014-09-3670 ([MFBDA](https://web.math.pmf.unizg.hr/mfbda/)).

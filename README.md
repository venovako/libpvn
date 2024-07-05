# libpvn
A small portable C library with several utility functions.

(... work in progress ...)

This software is a supplementary material for:
- the paper doi:[10.1016/j.cam.2024.116003](https://doi.org/10.1016/j.cam.2024.116003 "Accurate complex Jacobi rotations").

Some functions have been adapted from those in the repositories [JACSD](https://github.com/venovako/JACSD) and [VecJac](https://github.com/venovako/VecJac).

## Building

The library has been successfully built using:
| compiler | `uname` | platform |
| -------- | ------- | -------- |
| clang(1) | Darwin  | x86_64   |
| clang(2) | FreeBSD | arm64    |
| gcc(3)   | Darwin  | x86_64   |
| gcc(2)   | FreeBSD | arm64    |
| gcc(4)   | Linux   | aarch64  |
| gcc(4)   | Linux   | ppc64le  |
| gcc(5)   | Linux   | x86_64   |
| gcc(6)   | SunOS   | i86pc    |
| icx(7)   | Linux   | x86_64   |

Recent versions of the compilers have been provided by or used on:
1. Apple (clang 13.0.0),
2. FreeBSD (clang 18.1.5 and GCC 13.2.0),
3. Homebrew (GCC 14.1.0),
4. openSUSE Tumbleweed (GCC 14.1.0),
5. Oracle Linux (GCC 13.2.1 and 14.1.1 (custom built)),
6. Oracle Solaris (GCC 11.2.0),
7. Intel oneAPI (2024.2).

Examples of building the library:
```bash
cd src
# query the building options (GNU make is necessary)
make help
# the output should be something like:
# make [COMPILER=clang|gcc|icx] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [NDEBUG=0|1|2|3|...] [PRINTOUT=ERR|OUT] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [QUADMATH=...] [PROFILE=...] [SAFE=...] [all|clean|help]
# where gcc is the default compiler to be used on Linux, and clang is otherwise
#
# a release build with icx on x86_64 Linux
make COMPILER=icx NDEBUG=3 clean all
#
# a release build with the Homebrew's gcc on x86_64 macOS
make COMPILER=gcc COMPILER_SUFFIX=-13 NDEBUG=3 clean all
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

The `SAFE` variable lists, as one or more comma-separated short names, the components for which a safe implementation is requested:
- `cma` makes the complex multiplication and FMA safe from unwarranted overflow;
- `ran` requests that all random numbers (not only those explicitly created as such) lie in the range `[xxx_MIN*2^p,xxx_MAX/4]`, where `p=0` by default;
- `sv2` indicates that `hypotf` and `hypot` are correctly rounded and can be used for the singular value decomposition.

So, `SAFE=cma,sv2`, e.g., requests the `cma` and `sv2` safe implementations, but not the `ran` one.

The other options are for testing, debugging, and profiling, and should not be set unless their effects are fully understood.

*Caveat*: certain parts of the library will *not* work on big-endian systems!
Also, several functions will not work on Windows, and probably on other untested systems as well.

## Examples

Several examples and tests are built as `*.exe` executables in the `src` subdirectory and are meant to be run from there.

## Documentation

...is a work in progress.

Run `doxygen` in the `doc` subdirectory to generate the HTML documentation.

This work has been supported in part by Croatian Science Foundation under the project IP-2014-09-3670 ([MFBDA](https://web.math.pmf.unizg.hr/mfbda/)).

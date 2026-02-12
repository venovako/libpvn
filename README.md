# libpvn
A small portable C library with several utility functions.

(... work in progress ...)

This software is a supplementary material for the papers:
- doi:[10.1016/j.cam.2024.116003](https://doi.org/10.1016/j.cam.2024.116003 "Accurate complex Jacobi rotations"),
- doi:[10.1007/s11075-025-02035-7](https://doi.org/10.1007/s11075-025-02035-7 "Arithmetical enhancements of the Kogbetliantz method for the SVD of order two"),
- doi:[10.1142/S0129626426500027](https://doi.org/10.1142/S0129626426500027 "Recursive vectorized computation of the vector p-norm"),
- doi:[10.1553/etna_vol65s26](https://doi.org/10.1553/etna_vol65s26 "On convergence and accuracy of the J-Jacobi method under the de Rijk pivot strategy").

Some functions have been adapted from those in the repositories [JACSD](https://github.com/venovako/JACSD) and [VecJac](https://github.com/venovako/VecJac).

A `getopt(3)`, `getopt_long(3)`, and `getopt_long_only(3)` implementation for Windows, in the `inc` subdirectory, has been modified from the [OpenBSD](https://www.openbsd.org) sources.

Several correctly-rounded mathematical functions provided by the [CORE-MATH](https://core-math.gitlabpages.inria.fr) project are used.
Their slightly modified implementations included here, in the `inc` subdirectory, will be taken if the `CR_MATH` make variable is not set.

Some norm-related testers, results, and documentation has been *moved* to the [VecNrmP](https://github.com/venovako/VecNrmP) repository.

## Building

The library has been successfully built using:
| compiler | `uname` | platform |
| -------- | ------- | -------- |
| clang(1) | Darwin  | x86_64   |
| clang(2) | FreeBSD | arm64    |
| gcc(3)   | Darwin  | x86_64   |
| gcc(4)   | FreeBSD | arm64    |
| gcc(5)   | Linux   | ppc64le  |
| gcc(6)   | Linux   | x86_64   |
| icx(7)   | Linux   | x86_64   |
| nvc(8)   | Linux   | x86_64   |

Recent versions of the compilers have been provided by or used on:
1. Apple (clang 13.0.0),
2. FreeBSD (clang 19.1.7),
3. gcc.gnu.org (GCC 15.2.1),
4. FreeBSD (GCC 14.2.0),
5. openSUSE Tumbleweed (GCC 15.2.1),
6. Oracle Linux (GCC 14.2.1-7),
7. Intel oneAPI (2025.3.1),
8. NVIDIA HPC SDK (26.1).

Examples of building the library:
```bash
cd src
# query the building options (GNU make is necessary)
make help
# the output should be something like:
# make [COMPILER=clang|gcc|icx|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [MARCH=...] [NDEBUG=0|1|2|3|...] [PRINTOUT=ERR|OUT] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [PROFILE=...] [SAFE=...] [QUADMATH=...] [LIB64=lib|lib64] [GMP=...] [MPFR=...] [MPC=...] [SLEEF=... [sleef=0|1|2]] [STATIC=-s] [all|clean|help]
# where gcc is the default compiler to be used on Linux, and clang is otherwise
#
# a release build with icx on x86_64 Linux with a high optimization level
make COMPILER=icx NDEBUG=3 clean all
#
# a release build with the Homebrew's gcc on x86_64 macOS with a low optimization level
make COMPILER=gcc COMPILER_SUFFIX=-15 NDEBUG=g clean all
#
# a debug build with clang on FreeBSD (note the usage of gmake instead of make)
gmake clean all
```

## Using

Include the `pvn.h` header file in your C/C++ sources where needed and link with the `libpvn.so` (dynamic) or `libpvn.a` (static) library, as shown in `GNUmakefile`.
There are no mutable global/static variables present, *unless profiling has been enabled or Windows are targeted*, but some static constant lookup tables are used.
The functions that do not interface other non-system libraries can safely be called from multiple threads on *unrelated* data without locking.
You might wish to tune the compiler flags to match yours, especially if you are building a static executable and/or want to use [OpenMP](https://openmp.org).

Fortran (column-major) array order is assumed for the functions that operate on matrices.

All functions declared with `PVN_FABI` and not taking their arguments by value should be callable from Fortran without an explicit interface, just by declaring it `EXTERNAL` (without the underscore).

Setting the `CR_MATH` variable in a `[g]make` invocation to the cloned `core-math` source code directory path overrides the included choice of the CORE-MATH functions.
If the object files have not been prepared beforehand, the source files will be compiled in either case.
The object files will be integrated into `libpvn.so` and `libpvn.a` for easier re-use by other software linked with it.

The `MARCH` option can be used to indicate another CPU architecture than `native` (for `clang` and `gcc`) or `Host` (for `icx`).

The `OPENMP` option enables OpenMP:
- `OPENMP=0` only enables the compiler's OpenMP support,
- `OPENMP=1` makes the functions OpenMP-safe, but they are not parallelized (some testers might be),
- `OPENMP>1` parallelizes certain functions.

Bear in mind, however, that this will introduce a dependency on the OpenMP runtime library!
If the [OpenCilk](https://www.opencilk.org) compiler is used, please set `OPENMP=cilk` instead.

If `long double` is not the 128-bit floating-point datatype (e.g., on Intel-compatible hardware), the `PVN_QUADMATH` macro should be set automatically to the name of a library implementing quadruple precision arithmetic, unless `COMPILER=clang` is used.

The `SAFE` variable lists, as one or more comma-separated short names, the components for which a safe implementation is requested:
- `nrm` selects the default norm computation backend (`cr_hypot` or `hypot`), and
- `sv2` uses a slightly more precise triangulation.

So, `SAFE=sv2,nrm`, e.g., requests the `sv2` and `nrm` safe implementations.
If in all-caps, `NRM` turns on the vector sorting before the final reductions where applicable.

The other options are for testing, debugging, and profiling, and should not be set unless their effects are fully understood.
For example, some numerical testers require that `GMP`, `MPFR`, and `SLEEF` are set to the installation prefixes of the respective libraries.
Assume, e.g., that [GMP](https://gmplib.org) is located at `/opt/gmp`, [MPFR](https://mpfr.org) at `/opt/mpfr`, and [SLEEF](https://sleef.org) at `/opt/sleef`.
Then, `GMP=/opt/gmp`, `MPFR=/opt/mpfr`, and `SLEEF=/opt/sleef`.
It is advisable to build those packages from their recent development sources, and the libraries' installation directories should be uniformly named, i.e., either `lib` or `lib64`.
Also, `VECLEN` should sometimes be set to the size in bytes of the widest vector datatype, as shown in the [VecNrmP](https://github.com/venovako/VecNrmP) examples.
As a side effect, some additional and often undesired optimizations are turned on by explicitly setting it!

*Caveat*: certain parts of the library will *not* work on big-endian systems!
Also, several functions will not work on Windows, and probably on other untested systems as well.

## Examples

Several examples and tests are built as `*.exe` executables in the `src` and the `etc` subdirectories and are meant to be run from there.

## Documentation

...is a work in progress.

Run `doxygen` in the `doc` subdirectory to generate the HTML documentation.

Some documentation can also be found in the `var` subdirectory.

This work has been supported in part by Croatian Science Foundation under the project IP-2014-09-3670 ([MFBDA](https://web.math.pmf.unizg.hr/mfbda/)).

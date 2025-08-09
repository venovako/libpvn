# libpvn
A small portable C library with several utility functions.

(... work in progress ...)

This software is a supplementary material for the papers:
- doi:[10.1016/j.cam.2024.116003](https://doi.org/10.1016/j.cam.2024.116003 "Accurate complex Jacobi rotations"),
- doi:[10.1007/s11075-025-02035-7](https://doi.org/10.1007/s11075-025-02035-7 "Arithmetical enhancements of the Kogbetliantz method for the SVD of order two").

Some functions have been adapted from those in the repositories [JACSD](https://github.com/venovako/JACSD) and [VecJac](https://github.com/venovako/VecJac).

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
| icc(7)   | Darwin  | x86_64   |
| icx(8)   | Linux   | x86_64   |
| nvc(9)   | Linux   | x86_64   |

Recent versions of the compilers have been provided by or used on:
1. Apple (clang 13.0.0),
2. FreeBSD (clang 19.1.7),
3. Homebrew (GCC 15.1.0),
4. FreeBSD (GCC 13.3.0),
5. openSUSE Tumbleweed (GCC 15.1.1),
6. Oracle Linux (GCC 14.2.1-7),
7. Intel Classic (2021.10.0),
8. Intel oneAPI (2025.1.1),
9. NVIDIA HPC SDK (25.7).

Examples of building the library:
```bash
cd src
# query the building options (GNU make is necessary)
make help
# the output should be something like:
# make [COMPILER=clang|gcc|icx|icc|nvc] [COMPILER_PREFIX=...] [COMPILER_SUFFIX=...] [MARCH=...] [NDEBUG=0|1|2|3|...] [PRINTOUT=ERR|OUT] [VECLEN=...] [CR_MATH=...] [OPENMP=...] [PROFILE=...] [SAFE=...] [DYNAMIC=dylib|so] [QUADMATH=-lquadmath] [GMP=...] [MPFR=...] [MPC=...] [all|clean|help]
# where gcc is the default compiler to be used on Linux, and clang is otherwise
#
# a release build with icx on x86_64 Linux with a high optimization level
make COMPILER=icx NDEBUG=3 clean all
#
# a release build with the Homebrew's gcc on x86_64 macOS with a low optimization level
make COMPILER=gcc COMPILER_SUFFIX=-14 NDEBUG=g clean all
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

Several correctly-rounded mathematical functions provided by the [CORE-MATH](https://core-math.gitlabpages.inria.fr) project are used.
Their slightly modified implementations included here will be taken if `CR_MATH` is not set.
Otherwise, set the `CR_MATH` variable in a `[g]make` invocation to the cloned `core-math` source code directory path.
If the object files have not been prepared beforehand, the source files will be compiled in either case.
The object files will be integrated into `libpvn.a` for easier re-use by other software linked with it.

The `MARCH` option can be used to indicate another CPU architecture than `native` (for `clang` and `gcc`) or `Host` (for `icx`).

The `OPENMP` option enables OpenMP and its content is appended to the compiler's flags.
Set it to `true` if no additional compiler flags are desired.
Bear in mind, however, that this will introduce a dependency on the OpenMP runtime library!

If `long double` is not the 128-bit floating-point datatype (e.g., on Intel-compatible hardware), the `PVN_QUADMATH` macro should be set automatically to the name of a library implementing quadruple precision arithmetic, unless `COMPILER=clang` is used.

The `SAFE` variable lists, as one or more comma-separated short names, the components for which a safe implementation is requested:
- `ev2` and `jv2` guard against a not-correctly-rounded `hypotq`;
- `ran` requests that all random numbers (not only those explicitly created as such) lie in the range `[xxx_MIN*2^p,xxx_MAX/4]`, where `p=0` by default;
- `sv2` uses a slightly more precise triangulation; and
- `vec` uses the `mpfr_hypot`-based instead of the `mpfr_fma`-based norm computation.

So, `SAFE=ev2,jv2,sv2`, e.g., requests the `ev2`, `jv2`, and `sv2` safe implementations, but not the `ran` one.

The other options are for testing, debugging, and profiling, and should not be set unless their effects are fully understood.

*Caveat*: certain parts of the library will *not* work on big-endian systems!
Also, several functions will not work on Windows, and probably on other untested systems as well.

## Examples

Several examples and tests are built as `*.exe` executables in the `src` and the `etc` subdirectories and are meant to be run from there.

## Documentation

...is a work in progress.

Run `doxygen` in the `doc` subdirectory to generate the HTML documentation.

Some documentation can also be found in the `var` subdirectory.

This work has been supported in part by Croatian Science Foundation under the project IP-2014-09-3670 ([MFBDA](https://web.math.pmf.unizg.hr/mfbda/)).

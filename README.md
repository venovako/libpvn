# libpvn
A small portable C library with several utility functions.

(... work in progress ...)

Some functions have been adapted from those in the repository [JACSD](https://github.com/venovako/JACSD).

The library has been successfully compiled using:
| compiler | `uname` | platform |
| -------- | ------- | -------- |
| clang(1) | Darwin  | x86_64   |
| clang(2) | Linux   | ppc64le  |
| clang(3) | FreeBSD | amd64    |
| clang(4) | OpenBSD | amd64    |
| gcc(5)   | Darwin  | x86_64   |
| gcc(6)   | Linux   | ppc64le  |
| gcc(7)   | Linux   | x86_64   |
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
# non-debug build with icx on x86_64
make COMPILER=icx NDEBUG=3 clean all
# debug build with clang on ppc64le
make COMPILER=clang COMPILER_PREFIX=ibm- COMPILER_SUFFIX=_r clean all
```

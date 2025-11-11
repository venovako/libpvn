#ifndef PVN_H
#define PVN_H

#ifdef _WIN32
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif /* !_USE_MATH_DEFINES */
#endif /* _WIN32 */

#if (defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER))
#include <mathimf.h>
#else /* !Intel */
#ifdef __cplusplus
#include <complex>
#include <cmath>
#else /* !__cplusplus */
#include <complex.h>
#include <math.h>
#endif /* ?__cplusplus */
#endif /* ?Intel */

#ifdef __cplusplus
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#else /* !__cplusplus */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#endif /* ?__cplusplus */

#include <stdalign.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#include "../inc/getopt.h"
#else /* !_WIN32 */
#include <dlfcn.h>
#include <getopt.h>
#include <sys/uio.h>
#include <execinfo.h>
#include <pthread.h>
#include <search.h>
#include <sys/time.h>
#include <unistd.h>
#endif /* ?_WIN32 */
#if (defined(PVN_OPENMP) && (PVN_OPENMP > 0))
#include <omp.h>
#endif /* PVN_OPENMP */
#ifndef PVN_CILK
#define cilk_for for
#define cilk_scope
#define cilk_spawn
#endif /* !PVN_CILK */
#ifdef PVN_MPI
#include "mpi.h"
#endif /* PVN_MPI */

#include "pvn_ext.h"

#ifndef PVN_NO_PROF
#ifdef PVN_PROFILE
#define PVN_NO_PROF __attribute__((no_instrument_function))
#else /* !PVN_PROFILE */
#define PVN_NO_PROF
#endif /* ?PVN_PROFILE */
#else /* PVN_NO_PROF */
#error PVN_NO_PROF already defined
#endif /* ?PVN_NO_PROF */

#ifndef PVN_ASSERT
#ifdef NDEBUG
#if (defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER))
#define PVN_ASSERT(cond) __assume(cond)
#elif (defined(__GNUC__) && !defined(__clang__) && !defined(__NVCOMPILER))
#if (__GNUC__ >= 13)
#define PVN_ASSERT(cond) __attribute__((assume(cond)))
#else /* __GNUC__ < 13 */
#define PVN_ASSERT(cond)
#endif /* ?__GNUC__ */
#elif (defined(__clang__) || defined(__NVCOMPILER))
#define PVN_ASSERT(cond) __builtin_assume(cond)
#else /* unsupported compiler */
#define PVN_ASSERT(cond) assert(cond)
#endif /* ?compiler */
#else /* !NDEBUG */
#define PVN_ASSERT(cond) assert(cond)
#endif /* ?NDEBUG */
#else /* PVN_ASSERT */
#error PVN_ASSERT already defined
#endif /* ?PVN_ASSERT */

#ifndef PVN_LF64
#ifdef _LARGEFILE64_SOURCE
#define PVN_LF64 O_LARGEFILE
#else /* !_LARGEFILE64_SOURCE */
#define PVN_LF64 0
#endif /* ?_LARGEFILE64_SOURCE */
#else /* PVN_LF64 */
#error PVN_LF64 already defined
#endif /* ?PVN_LF64 */

/* the constants have been taken from the GCC's quadmath.h and modified */
#ifdef PVN_QUADMATH
#ifdef __MATHIMF_H_INCLUDED
#ifndef FLT128_MAX
#define FLT128_MAX 1.18973149535723176508575932662800702E+4932q
#else /* FLT128_MAX */
#error FLT128_MAX already defined
#endif /* ?FLT128_MAX */
#ifndef FLT128_MIN
#define FLT128_MIN 3.36210314311209350626267781732175260E-4932q
#else /* FLT128_MIN */
#error FLT128_MIN already defined
#endif /* ?FLT128_MIN */
#ifndef FLT128_EPSILON
#define FLT128_EPSILON 1.92592994438723585305597794258492732E-34q
#else /* FLT128_EPSILON */
#error FLT128_EPSILON already defined
#endif /* ?FLT128_EPSILON */
#ifndef FLT128_TRUE_MIN
#define FLT128_TRUE_MIN 6.475175119438025110924438958227646552E-4966q
#else /* FLT128_TRUE_MIN */
#error FLT128_TRUE_MIN already defined
#endif /* !FLT128_TRUE_MIN */
#ifndef FLT128_MANT_DIG
#define FLT128_MANT_DIG 113
#else /* FLT128_MANT_DIG */
#error FLT128_MANT_DIG already defined
#endif /* ?FLT128_MANT_DIG */
#ifndef FLT128_MAX_EXP
#define FLT128_MAX_EXP 16384
#else /* FLT128_MAX_EXP */
#error FLT128_MAX_EXP already defined
#endif /* ?FLT128_MAX_EXP */
#ifndef FLT128_MIN_EXP
#define FLT128_MIN_EXP (-16381)
#else /* FLT128_MIN_EXP */
#error FLT128_MIN_EXP already defined
#endif /* ?FLT128_MIN_EXP */
EXTERN_C __float128 __copysignq(__float128, __float128);
EXTERN_C __float128 __fabsq(__float128);
EXTERN_C __float128 __fmaq(__float128, __float128, __float128);
EXTERN_C __float128 __fmaxq(__float128, __float128);
EXTERN_C __float128 __fminq(__float128, __float128);
EXTERN_C __float128 __frexpq(__float128, int*);
EXTERN_C __float128 __invsqrtq(__float128);
EXTERN_C int __isfiniteq(__float128);
EXTERN_C __float128 __hypotq(__float128, __float128);
EXTERN_C __float128 __powq(__float128, __float128);
EXTERN_C __float128 __scalbnq(__float128, int);
EXTERN_C __float128 __sqrtq(__float128);
#ifndef copysignq
#define copysignq __copysignq
#else /* copysignq */
#error copysignq already defined
#endif /* ?copysignq */
#ifndef fabsq
#define fabsq __fabsq
#else /* fabsq */
#error fabsq already defined
#endif /* ?fabsq */
#ifndef fmaq
#define fmaq __fmaq
#else /* fmaq */
#error fmaq already defined
#endif /* ?fmaq */
#ifndef fmaxq
#define fmaxq __fmaxq
#else /* fmaxq */
#error fmaxq already defined
#endif /* ?fmaxq */
#ifndef fminq
#define fminq __fminq
#else /* fminq */
#error fminq already defined
#endif /* ?fminq */
#ifndef frexpq
#define frexpq __frexpq
#else /* frexpq */
#error frexpq already defined
#endif /* ?frexpq */
#ifndef isfiniteq
#define isfiniteq __isfiniteq
#else /* isfiniteq */
#error isfiniteq already defined
#endif /* ?isfiniteq */
#ifndef powq
#define powq __powq
#else /* powq */
#error powq already defined
#endif /* ?powq */
#ifndef scalbnq
#define scalbnq __scalbnq
#else /* scalbnq */
#error scalbnq already defined
#endif /* ?scalbnq */
#if (!defined(_WIN32) || defined(_DLL))
EXTERN_C int quadmath_snprintf(char *str, size_t size, const char *format, ...);
EXTERN_C __float128 strtoflt128 (const char *s, char **sp);
#endif /* !_WIN32 || _DLL */
#else /* !__MATHIMF_H_INCLUDED */
#ifdef __GNUC__
#include <quadmath.h>
#ifndef FLT128_TRUE_MIN
#define FLT128_TRUE_MIN FLT128_DENORM_MIN
#else /* FLT128_TRUE_MIN */
#error FLT128_TRUE_MIN already defined
#endif /* !FLT128_TRUE_MIN */
#ifndef isfiniteq
#define isfiniteq finiteq
#else /* isfiniteq */
#error isfiniteq already defined
#endif /* ?isfiniteq */
#else /* !__GNUC__ */
#error quad-precision math not supported
#endif /* ?__GNUC__ */
#endif /* ?__MATHIMF_H_INCLUDED */
#endif /* PVN_QUADMATH */

#ifndef PVN_FABI
#ifdef _WIN32
#ifdef PVN_MINGW64
#define PVN_FABI(P,W) P##_
#else /* !PVN_MINGW64 */
#define PVN_FABI(P,W) W
#endif /* ?PVN_MINGW64 */
#else /* !_WIN32 */
#define PVN_FABI(P,W) P##_
#endif /* ?_WIN32 */
#endif /* !PVN_FABI */

#ifdef _WIN32
#ifndef ssize_t
#define ssize_t long long
#endif /* !ssize_t */
#ifndef off_t
#define off_t ssize_t
#endif /* !off_t */
#endif /* _WIN32 */

#include "pvn_aux.h"
#include "pvn_bio.h"
#include "pvn_bmp.h"
#include "pvn_cjs.h"
#include "pvn_cma.h"
#include "pvn_crm.h"
#include "pvn_det.h"
#include "pvn_djs.h"
#include "pvn_dot.h"
#include "pvn_err.h"
#include "pvn_ev2.h"
#include "pvn_fmt.h"
#include "pvn_jv2.h"
#include "pvn_mem.h"
#include "pvn_mm2.h"
#include "pvn_nrm.h"
#include "pvn_ran.h"
#include "pvn_sv2.h"
#include "pvn_tar.h"
#include "pvn_vec.h"
#include "pvn_vis.h"
#include "pvn_lock.h"
#ifndef _WIN32
#include "pvn_prof.h"
#endif /* !_WIN32 */
#include "pvn_timer.h"
#ifdef PVN_MPI
#include "pvn_mpi.h"
#endif /* PVN_MPI */

#ifdef PVN_GMP
#include "gmp.h"
#endif /* PVN_GMP */
#ifdef PVN_MPFR
#ifdef PVN_QUADMATH
#define MPFR_WANT_FLOAT128 __float128
#define mpfr_float128 __float128
#endif /* PVN_QUADMATH */
#include "mpfr.h"
#include "pvn_mpfr.h"
#endif /* PVN_MPFR */
#ifdef PVN_MPC
#include "mpc.h"
#endif /* PVN_MPC */

static inline int pvn_le()
{
  const int one = 1;
  return (int)*(const char*)&one;
}
PVN_EXTERN_C int PVN_FABI(pvn_le,PVN_LE)();

static inline int pvn_omp()
{
  return
#ifdef _OPENMP
    _OPENMP
#else /* !_OPENMP */
    0
#endif /* ?_OPENMP */
    ;
}
PVN_EXTERN_C int PVN_FABI(pvn_omp,PVN_OMP)();

PVN_EXTERN_C int PVN_FABI(c_math_err,C_MATH_ERR)();

#endif /* !PVN_H */

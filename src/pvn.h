#ifndef PVN_H
#define PVN_H

#if (defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER))
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
#ifndef _WIN32
#include <dlfcn.h>
#endif /* !_WIN32 */
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#else /* !_WIN32 */
#include <sys/uio.h>
#include <execinfo.h>
#include <pthread.h>
#include <search.h>
#include <sys/time.h>
#include <unistd.h>
#endif /* ?_WIN32 */
#ifdef _OPENMP
#include <omp.h>
#endif /* _OPENMP */

#ifndef PVN_EXTERN_C
#ifdef __cplusplus
#define PVN_EXTERN_C extern "C"
#else /* C */
#define PVN_EXTERN_C extern
#endif /* ?__cplusplus */
#else /* PVN_EXTERN_C */
#error PVN_EXTERN_C already defined
#endif /* ?PVN_EXTERN_C */

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
#if (defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER))
#define PVN_ASSERT(cond) __assume(cond)
#elif (defined(__GNUC__) && !defined(__clang__) && !defined(__NVCOMPILER))
#define PVN_ASSERT(cond) __attribute__((assume(cond)))
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
#define FLT128_MAX 1.18973149535723176508575932662800702E+4932q
#define FLT128_MIN 3.36210314311209350626267781732175260E-4932q
#define FLT128_EPSILON 1.92592994438723585305597794258492732E-34q
#define FLT128_TRUE_MIN 6.475175119438025110924438958227646552E-4966q
#define FLT128_MANT_DIG 113
#define FLT128_MAX_EXP 16384
#define FLT128_MIN_EXP (-16381)
PVN_EXTERN_C __float128 __copysignq(__float128, __float128);
PVN_EXTERN_C __float128 __fabsq(__float128);
PVN_EXTERN_C __float128 __fmaq(__float128, __float128, __float128);
PVN_EXTERN_C __float128 __fmaxq(__float128, __float128);
PVN_EXTERN_C __float128 __fminq(__float128, __float128);
PVN_EXTERN_C __float128 __frexpq(__float128, int*);
PVN_EXTERN_C __float128 __hypotq(__float128, __float128);
PVN_EXTERN_C int __isfiniteq(__float128);
PVN_EXTERN_C __float128 __scalbnq(__float128, int);
PVN_EXTERN_C __float128 __sqrtq(__float128);
PVN_EXTERN_C __float128 __invsqrtq(__float128);
#define copysignq __copysignq
#define fabsq __fabsq
#define fmaq __fmaq
#define fmaxq __fmaxq
#define fminq __fminq
#define frexpq __frexpq
#define hypotq __hypotq
#define isfiniteq __isfiniteq
#define scalbnq __scalbnq
#define sqrtq __sqrtq
#define rsqrtq __invsqrtq
#ifndef _WIN32
PVN_EXTERN_C int quadmath_snprintf(char *str, size_t size, const char *format, ...);
#endif /* !_WIN32 */
#else /* !__MATHIMF_H_INCLUDED */
#ifdef __GNUC__
#include <quadmath.h>
#ifndef FLT128_TRUE_MIN
#define FLT128_TRUE_MIN FLT128_DENORM_MIN
#endif /* !FLT128_TRUE_MIN */
#ifndef isfiniteq
#define isfiniteq finiteq
#endif /* !isfiniteq */
/* the GCC's libquadmath does not have rsqrtq or a similar function */
static inline __float128 rsqrtq(__float128 x)
{
  return (1.0q / sqrtq(x));
}
#else /* !__GNUC__ */
#error quad-precision math not supported
#endif /* ?__GNUC__ */
#endif /* ?__MATHIMF_H_INCLUDED */
#endif /* PVN_QUADMATH */

#ifndef PVN_FABI
#ifdef _WIN32
#define PVN_FABI(P,W) W
#else /* !_WIN32 */
#define PVN_FABI(P,W) P##_
#endif /* ?_WIN32 */
#else /* PVN_FABI */
#error PVN_FABI already defined
#endif /* ?PVN_FABI */

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
#include "pvn_vis.h"
#ifndef _WIN32
#include "pvn_lock.h"
#include "pvn_prof.h"
#endif /* !_WIN32 */
#include "pvn_timer.h"

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

#endif /* !PVN_H */

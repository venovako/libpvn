#ifndef PVN_H
#define PVN_H

#if (defined(__ICC) || defined(__INTEL_COMPILER) || defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER))
#include <mathimf.h>
#else /* !__ICC */
#ifdef __cplusplus
#include <complex>
#include <cmath>
#else /* !__cplusplus */
#include <complex.h>
#include <math.h>
#endif /* ?__cplusplus */
#endif /* ?__ICC */

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
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
#include <io.h>
#else /* !_WIN32 */
#include <sys/uio.h>
#include <execinfo.h>
#include <pthread.h>
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

/* the constants have been taken from the GCC's quadmath.h and modified */
#ifdef PVN_QUADMATH
#ifdef __MATHIMF_H_INCLUDED
#define FLT128_MAX 1.18973149535723176508575932662800702E+4932q
#define FLT128_MIN 3.36210314311209350626267781732175260E-4932q
#define FLT128_EPSILON 1.92592994438723585305597794258492732E-34q
#define FLT128_TRUE_MIN 6.475175119438025110924438958227646552E-4966q
#define FLT128_MAX_EXP 16384
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
PVN_EXTERN_C int quadmath_snprintf(char *str, size_t size, const char *format, ...);
#else /* !__MATHIMF_H_INCLUDED */
#ifdef __GNUC__
#include <quadmath.h>
#define FLT128_TRUE_MIN FLT128_DENORM_MIN
#define isfiniteq finiteq
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

#include "pvn_aux.h"
#include "pvn_bmp.h"
#include "pvn_cjs.h"
#include "pvn_crm.h"
#include "pvn_ev2.h"
#include "pvn_sv2.h"
#include "pvn_fmt.h"
#include "pvn_mem.h"
#include "pvn_mtx.h"
#ifndef _WIN32
#include "pvn_error.h"
#include "pvn_lock.h"
#include "pvn_timer.h"
#endif /* !_WIN32 */

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
PVN_EXTERN_C int
#ifdef _WIN32
PVN_OMP
#else /* !_WIN32 */
pvn_omp_
#endif /* ?_WIN32 */
();
#endif /* !PVN_H */

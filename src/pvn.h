#ifndef PVN_H
#define PVN_H

#if (!defined(__STDC__) || (__STDC__ != 1))
#error NON-CONFORMING C IMPLEMENTATION
#endif /* ?__STDC__ */

#if (!defined(__STDC_HOSTED__) || (__STDC_HOSTED__ != 1))
#error NOT A HOSTED IMPLEMENTATION
#endif /* ?__STDC_HOSTED__ */

#if (defined(__STDC_NO_COMPLEX__) && (__STDC_NO_COMPLEX__ == 1))
#error COMPLEX TYPES NOT SUPPORTED
#endif /* ?__STDC_NO_COMPLEX__ */

#if (defined(__STDC_NO_VLA__) && (__STDC_NO_VLA__ == 1))
#error VLA NOT SUPPORTED
#endif /* ?__STDC_NO_VLA__ */

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
#include <execinfo.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

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

#include "pvn_bmp.h"
#include "pvn_error.h"
#include "pvn_fmt.h"
#include "pvn_lock.h"
#include "pvn_timer.h"

#endif /* !PVN_H */

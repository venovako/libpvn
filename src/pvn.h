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

#include "pvn_aux.h"
#include "pvn_bmp.h"
#include "pvn_cjs.h"
#include "pvn_crm.h"
#include "pvn_ev2.h"
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
PVN_EXTERN_C int pvn_omp_();
#endif /* !PVN_H */

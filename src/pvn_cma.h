#ifndef PVN_CMA_H
#define PVN_CMA_H

#ifndef PVN_H
#error pvn_cma.h not intended for direct inclusion
#endif /* !PVN_H */

/* C = A * B */
/* rounding to nearest-even is assumed */

static inline void pvn_cmul(float *const cr, float *const ci, const float ar, const float ai, const float br, const float bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  float x;
  int t;
  *ci = -ai;
  *cr = PVN_FABI(pvn_sdet,PVN_SDET)(&ar, &ai, &bi, &br, &x, &t);
  *ci = PVN_FABI(pvn_sdet,PVN_SDET)(&ar,  ci, &br, &bi, &x, &t);
}

/* D = A * B + C */
/* rounding to nearest-even is assumed */

static inline float pvn_xtpyf(float x, int t, const float y)
{
  if (t >= FLT_MAX_EXP) {
    x = __builtin_scalbnf(x, t - FLT_MAX_EXP + 1);
    t = FLT_MAX_EXP - 1;
  }
  return __builtin_fmaf(__builtin_scalbnf(1.0f, t), x, y);
}

static inline void pvn_cfma(float *const dr, float *const di, const float ar, const float ai, const float br, const float bi, const float cr, const float ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  float x;
  int t;
  *di = -ai;
  *dr = PVN_FABI(pvn_sdet,PVN_SDET)(&ar, &ai, &bi, &br, &x, &t);
  *dr = pvn_xtpyf(x, t, cr);
  *di = PVN_FABI(pvn_sdet,PVN_SDET)(&ar,  di, &br, &bi, &x, &t);
  *di = pvn_xtpyf(x, t, ci);
}

static inline void pvn_zmul(double *const cr, double *const ci, const double ar, const double ai, const double br, const double bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  double x;
  int t;
  *ci = -ai;
  *cr = PVN_FABI(pvn_ddet,PVN_DDET)(&ar, &ai, &bi, &br, &x, &t);
  *ci = PVN_FABI(pvn_ddet,PVN_DDET)(&ar,  ci, &br, &bi, &x, &t);
}

static inline double pvn_xtpy(double x, int t, const double y)
{
  if (t >= DBL_MAX_EXP) {
    x = __builtin_scalbn(x, t - DBL_MAX_EXP + 1);
    t = DBL_MAX_EXP - 1;
  }
  return __builtin_fma(__builtin_scalbn(1.0, t), x, y);
}

static inline void pvn_zfma(double *const dr, double *const di, const double ar, const double ai, const double br, const double bi, const double cr, const double ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  double x;
  int t;
  *di = -ai;
  *dr = PVN_FABI(pvn_ddet,PVN_DDET)(&ar, &ai, &bi, &br, &x, &t);
  *dr = pvn_xtpy(x, t, cr);
  *di = PVN_FABI(pvn_ddet,PVN_DDET)(&ar,  di, &br, &bi, &x, &t);
  *di = pvn_xtpy(x, t, ci);
}

static inline void pvn_wmul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  long double x;
  int t;
  *ci = -ai;
  *cr = PVN_FABI(pvn_xdet,PVN_XDET)(&ar, &ai, &bi, &br, &x, &t);
  *ci = PVN_FABI(pvn_xdet,PVN_XDET)(&ar,  ci, &br, &bi, &x, &t);
}

static inline long double pvn_xtpyl(long double x, int t, const long double y)
{
  if (t >= LDBL_MAX_EXP) {
    x = __builtin_scalbnl(x, t - LDBL_MAX_EXP + 1);
    t = LDBL_MAX_EXP - 1;
  }
  return __builtin_fmal(__builtin_scalbnl(1.0L, t), x, y);
}

static inline void pvn_wfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  long double x;
  int t;
  *di = -ai;
  *dr = PVN_FABI(pvn_xdet,PVN_XDET)(&ar, &ai, &bi, &br, &x, &t);
  *dr = pvn_xtpyl(x, t, cr);
  *di = PVN_FABI(pvn_xdet,PVN_XDET)(&ar,  di, &br, &bi, &x, &t);
  *di = pvn_xtpyl(x, t, ci);
}

#ifdef PVN_QUADMATH
static inline void pvn_ymul(__float128 *const cr, __float128 *const ci, const __float128 ar, const __float128 ai, const __float128 br, const __float128 bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  __float128 x;
  int t;
  *ci = -ai;
  *cr = PVN_FABI(pvn_qdet,PVN_QDET)(&ar, &ai, &bi, &br, &x, &t);
  *ci = PVN_FABI(pvn_qdet,PVN_QDET)(&ar,  ci, &br, &bi, &x, &t);
}

static inline __float128 pvn_xtpyq(__float128 x, int t, const __float128 y)
{
  if (t >= FLT128_MAX_EXP) {
    x = scalbnq(x, t - FLT128_MAX_EXP + 1);
    t = FLT128_MAX_EXP - 1;
  }
  return fmaq(scalbnq(1.0q, t), x, y);
}

static inline void pvn_yfma(__float128 *const dr, __float128 *const di, const __float128 ar, const __float128 ai, const __float128 br, const __float128 bi, const __float128 cr, const __float128 ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  __float128 x;
  int t;
  *di = -ai;
  *dr = PVN_FABI(pvn_qdet,PVN_QDET)(&ar, &ai, &bi, &br, &x, &t);
  *dr = pvn_xtpyq(x, t, cr);
  *di = PVN_FABI(pvn_qdet,PVN_QDET)(&ar,  di, &br, &bi, &x, &t);
  *di = pvn_xtpyq(x, t, ci);
}
#else /* !PVN_QUADMATH */
static inline void pvn_ymul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi)
{
  pvn_wmul(cr, ci, ar, ai, br, bi);
}

static inline void pvn_yfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci)
{
  pvn_wfma(dr, di, ar, ai, br, bi, cr, ci);
}
#endif /* ?PVN_QUADMATH */

PVN_EXTERN_C void PVN_FABI(pvn_cmul,PVN_CMUL)(float *const cr, float *const ci, const float *const ar, const float *const ai, const float *const br, const float *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_cfma,PVN_CFMA)(float *const dr, float *const di, const float *const ar, const float *const ai, const float *const br, const float *const bi, const float *const cr, const float *const ci);
PVN_EXTERN_C void PVN_FABI(pvn_zmul,PVN_ZMUL)(double *const cr, double *const ci, const double *const ar, const double *const ai, const double *const br, const double *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_zfma,PVN_ZFMA)(double *const dr, double *const di, const double *const ar, const double *const ai, const double *const br, const double *const bi, const double *const cr, const double *const ci);
PVN_EXTERN_C void PVN_FABI(pvn_wmul,PVN_WMUL)(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_wfma,PVN_WFMA)(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void PVN_FABI(pvn_ymul,PVN_YMUL)(__float128 *const cr, __float128 *const ci, const __float128 *const ar, const __float128 *const ai, const __float128 *const br, const __float128 *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_yfma,PVN_YFMA)(__float128 *const dr, __float128 *const di, const __float128 *const ar, const __float128 *const ai, const __float128 *const br, const __float128 *const bi, const __float128 *const cr, const __float128 *const ci);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void PVN_FABI(pvn_ymul,PVN_YMUL)(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_yfma,PVN_YFMA)(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_CMA_H */

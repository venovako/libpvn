#ifndef PVN_DET_H
#define PVN_DET_H

#ifndef PVN_H
#error pvn_det.h not intended for direct inclusion
#endif /* !PVN_H */

/* Kahan's computation of ad - bc */

static inline float pvn_sdet(const float a, const float b, const float c, const float d)
{
  const float
    w = (b * c),
    e = fmaf(-b, c, w),
    f = fmaf(a, d, -w),
    x = (f + e);
  return x;
}

static inline double pvn_ddet(const double a, const double b, const double c, const double d)
{
  const double
    w = (b * c),
    e = fma(-b, c, w),
    f = fma(a, d, -w),
    x = (f + e);
  return x;
}

static inline long double pvn_xdet(const long double a, const long double b, const long double c, const long double d)
{
  const long double
    w = (b * c),
    e = fmal(-b, c, w),
    f = fmal(a, d, -w),
    x = (f + e);
  return x;
}

#ifdef PVN_QUADMATH
static inline __float128 pvn_qdet(const __float128 a, const __float128 b, const __float128 c, const __float128 d)
{
  const __float128
    w = (b * c),
    e = fmaq(-b, c, w),
    f = fmaq(a, d, -w),
    x = (f + e);
  return x;
}
#else /* !PVN_QUADMATH */
static inline long double pvn_qdet(const long double a, const long double b, const long double c, const long double d)
{
  const long double
    w = (b * c),
    e = fmal(-b, c, w),
    f = fmal(a, d, -w),
    x = (f + e);
  return x;
}
#endif /* ?PVN_QUADMATH */

PVN_EXTERN_C float PVN_FABI(pvn_sdet,PVN_SDET)(const float *const a, const float *const b, const float *const c, const float *const d, float *const x, int *const t);
PVN_EXTERN_C double PVN_FABI(pvn_ddet,PVN_DDET)(const double *const a, const double *const b, const double *const c, const double *const d, double *const x, int *const t);
PVN_EXTERN_C long double PVN_FABI(pvn_xdet,PVN_XDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d, long double *const x, int *const t);
#ifdef PVN_QUADMATH
PVN_EXTERN_C __float128 PVN_FABI(pvn_qdet,PVN_QDET)(const __float128 *const a, const __float128 *const b, const __float128 *const c, const __float128 *const d, __float128 *const x, int *const t);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C long double PVN_FABI(pvn_qdet,PVN_QDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d, long double *const x, int *const t);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_DET_H */

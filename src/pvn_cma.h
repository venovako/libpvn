#ifndef PVN_CMA_H
#define PVN_CMA_H

#ifndef PVN_H
#error pvn_cma.h not intended for direct inclusion
#endif /* !PVN_H */

#ifdef PVN_CMA_SAFE
/* !!! INCOMPLETE AND NOT EXHAUSTIVELY TESTED !!! */
PVN_EXTERN_C void pvn_cmul(float *const cr, float *const ci, const float ar, const float ai, const float br, const float bi);
PVN_EXTERN_C void pvn_cfma(float *const dr, float *const di, const float ar, const float ai, const float br, const float bi, const float cr, const float ci);
PVN_EXTERN_C void pvn_zmul(double *const cr, double *const ci, const double ar, const double ai, const double br, const double bi);
PVN_EXTERN_C void pvn_zfma(double *const dr, double *const di, const double ar, const double ai, const double br, const double bi, const double cr, const double ci);
PVN_EXTERN_C void pvn_wmul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi);
PVN_EXTERN_C void pvn_wfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void pvn_ymul(__float128 *const cr, __float128 *const ci, const __float128 ar, const __float128 ai, const __float128 br, const __float128 bi);
PVN_EXTERN_C void pvn_yfma(__float128 *const dr, __float128 *const di, const __float128 ar, const __float128 ai, const __float128 br, const __float128 bi, const __float128 cr, const __float128 ci);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void pvn_ymul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi);
PVN_EXTERN_C void pvn_yfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci);
#endif /* ?PVN_QUADMATH */
#else /* !PVN_CMA_SAFE */
/* COMPLEX MULTIPLICATION AND FMA OPERATIONS THAT */
/* !!! DO NOT YET AVOID UNNECESSARY OVERFLOWS !!! */
/* TO BE USED WHEN ALL PARTIAL RESULTS ARE NORMAL */

/* C = A * B */

static inline void pvn_cmul(float *const cr, float *const ci, const float ar, const float ai, const float br, const float bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  *cr = fmaf(ar, br, -ai * bi);
  *ci = fmaf(ar, bi,  ai * br);
}

/* D = A * B + C */
/* similar to the CUDA's complex FMA from cuComplex.h */

static inline void pvn_cfma(float *const dr, float *const di, const float ar, const float ai, const float br, const float bi, const float cr, const float ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  *dr = fmaf(ar, br, fmaf(-ai, bi, cr));
  *di = fmaf(ar, bi, fmaf( ai, br, ci));
}

static inline void pvn_zmul(double *const cr, double *const ci, const double ar, const double ai, const double br, const double bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  *cr = fma(ar, br, -ai * bi);
  *ci = fma(ar, bi,  ai * br);
}

static inline void pvn_zfma(double *const dr, double *const di, const double ar, const double ai, const double br, const double bi, const double cr, const double ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  *dr = fma(ar, br, fma(-ai, bi, cr));
  *di = fma(ar, bi, fma( ai, br, ci));
}

static inline void pvn_wmul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  *cr = fmal(ar, br, -ai * bi);
  *ci = fmal(ar, bi,  ai * br);
}

static inline void pvn_wfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  *dr = fmal(ar, br, fmal(-ai, bi, cr));
  *di = fmal(ar, bi, fmal( ai, br, ci));
}

#ifdef PVN_QUADMATH
static inline void pvn_ymul(__float128 *const cr, __float128 *const ci, const __float128 ar, const __float128 ai, const __float128 br, const __float128 bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  *cr = fmaq(ar, br, -ai * bi);
  *ci = fmaq(ar, bi,  ai * br);
}

static inline void pvn_yfma(__float128 *const dr, __float128 *const di, const __float128 ar, const __float128 ai, const __float128 br, const __float128 bi, const __float128 cr, const __float128 ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  *dr = fmaq(ar, br, fmaq(-ai, bi, cr));
  *di = fmaq(ar, bi, fmaq( ai, br, ci));
}
#else /* !PVN_QUADMATH */
static inline void pvn_ymul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  *cr = fmal(ar, br, -ai * bi);
  *ci = fmal(ar, bi,  ai * br);
}

static inline void pvn_yfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  *dr = fmal(ar, br, fmal(-ai, bi, cr));
  *di = fmal(ar, bi, fmal( ai, br, ci));
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_CMA_SAFE */

PVN_EXTERN_C void pvn_cmul_(float *const cr, float *const ci, const float *const ar, const float *const ai, const float *const br, const float *const bi);
PVN_EXTERN_C void pvn_cfma_(float *const dr, float *const di, const float *const ar, const float *const ai, const float *const br, const float *const bi, const float *const cr, const float *const ci);
PVN_EXTERN_C void pvn_zmul_(double *const cr, double *const ci, const double *const ar, const double *const ai, const double *const br, const double *const bi);
PVN_EXTERN_C void pvn_zfma_(double *const dr, double *const di, const double *const ar, const double *const ai, const double *const br, const double *const bi, const double *const cr, const double *const ci);
PVN_EXTERN_C void pvn_wmul_(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi);
PVN_EXTERN_C void pvn_wfma_(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void pvn_ymul_(__float128 *const cr, __float128 *const ci, const __float128 *const ar, const __float128 *const ai, const __float128 *const br, const __float128 *const bi);
PVN_EXTERN_C void pvn_yfma_(__float128 *const dr, __float128 *const di, const __float128 *const ar, const __float128 *const ai, const __float128 *const br, const __float128 *const bi, const __float128 *const cr, const __float128 *const ci);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void pvn_ymul_(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi);
PVN_EXTERN_C void pvn_yfma_(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_CMA_H */

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
/* C = A * B */
/* should be correctly rounded */

static inline void pvn_cmul(float *const cr, float *const ci, const float ar, const float ai, const float br, const float bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  const double
    ar_ = (double)ar,
    ai_ = (double)ai,
    br_ = (double)br,
    bi_ = (double)bi,
    cr_ = fma(ar_, br_, -ai_ * bi_),
    ci_ = fma(ar_, bi_,  ai_ * br_);
  *cr = (float)cr_;
  *ci = (float)ci_;
}

static inline void pvn_zmul(double *const cr, double *const ci, const double ar, const double ai, const double br, const double bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
#ifdef PVN_QUADMATH
  const __float128
    ar_ = (__float128)ar,
    ai_ = (__float128)ai,
    br_ = (__float128)br,
    bi_ = (__float128)bi,
    cr_ = fmaq(ar_, br_, -ai_ * bi_),
    ci_ = fmaq(ar_, bi_,  ai_ * br_);
#else /* !PVN_QUADMATH */
  const long double
    ar_ = (long double)ar,
    ai_ = (long double)ai,
    br_ = (long double)br,
    bi_ = (long double)bi,
    cr_ = fmal(ar_, br_, -ai_ * bi_),
    ci_ = fmal(ar_, bi_,  ai_ * br_);
#endif /* ?PVN_QUADMATH */
  *cr = (double)cr_;
  *ci = (double)ci_;
}

/* D = A * B + C */
/* similar to the CUDA's complex FMA from cuComplex.h */
/* possibly not correctly rounded */

static inline void pvn_cfma(float *const dr, float *const di, const float ar, const float ai, const float br, const float bi, const float cr, const float ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  const double
    ar_ = (double)ar,
    ai_ = (double)ai,
    br_ = (double)br,
    bi_ = (double)bi,
    cr_ = (double)cr,
    ci_ = (double)ci,
    dr_ = fma(ar_, br_, fma(-ai_, bi_, cr_)),
    di_ = fma(ar_, bi_, fma( ai_, br_, ci_));
  *dr = (float)dr_;
  *di = (float)di_;
}

static inline void pvn_zfma(double *const dr, double *const di, const double ar, const double ai, const double br, const double bi, const double cr, const double ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
#ifdef PVN_QUADMATH
  const __float128
    ar_ = (__float128)ar,
    ai_ = (__float128)ai,
    br_ = (__float128)br,
    bi_ = (__float128)bi,
    cr_ = (__float128)cr,
    ci_ = (__float128)ci,
    dr_ = fmaq(ar_, br_, fmaq(-ai_, bi_, cr_)),
    di_ = fmaq(ar_, bi_, fmaq( ai_, br_, ci_));
#else /* !PVN_QUADMATH */
  const long double
    ar_ = (long double)ar,
    ai_ = (long double)ai,
    br_ = (long double)br,
    bi_ = (long double)bi,
    cr_ = (long double)cr,
    ci_ = (long double)ci,
    dr_ = fmal(ar_, br_, fmal(-ai_, bi_, cr_)),
    di_ = fmal(ar_, bi_, fmal( ai_, br_, ci_));
#endif /* ?PVN_QUADMATH */
  *dr = (double)dr_;
  *di = (double)di_;
}

/* COMPLEX MULTIPLICATION AND FMA OPERATIONS THAT */
/* !!! DO NOT YET AVOID UNNECESSARY OVERFLOWS !!! */
/* TO BE USED WHEN ALL PARTIAL RESULTS ARE NORMAL */

static inline void pvn_wmul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
#ifdef PVN_QUADMATH
  *cr = fmaq(ar, br, -ai * bi);
  *ci = fmaq(ar, bi,  ai * br);
#else /* !PVN_QUADMATH */
  *cr = fmal(ar, br, -ai * bi);
  *ci = fmal(ar, bi,  ai * br);
#endif /* ?PVN_QUADMATH */
}

static inline void pvn_wfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
#ifdef PVN_QUADMATH
  *dr = fmaq(ar, br, fmaq(-ai, bi, cr));
  *di = fmaq(ar, bi, fmaq( ai, br, ci));
#else /* !PVN_QUADMATH */
  *dr = fmal(ar, br, fmal(-ai, bi, cr));
  *di = fmal(ar, bi, fmal( ai, br, ci));
#endif /* ?PVN_QUADMATH */
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

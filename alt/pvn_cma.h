#ifndef PVN_CMA_H
#define PVN_CMA_H

#ifndef PVN_H
#error pvn_cma.h not intended for direct inclusion
#endif /* !PVN_H */

/* !!! INCOMPLETE AND NOT EXHAUSTIVELY TESTED !!! */
PVN_EXTERN_C void pvn_cmul(float *const cr, float *const ci, const float ar, const float ai, const float br, const float bi);
PVN_EXTERN_C void pvn_cfma(float *const dr, float *const di, const float ar, const float ai, const float br, const float bi, const float cr, const float ci);
PVN_EXTERN_C void pvn_zmul(double *const cr, double *const ci, const double ar, const double ai, const double br, const double bi);
PVN_EXTERN_C void pvn_zfma(double *const dr, double *const di, const double ar, const double ai, const double br, const double bi, const double cr, const double ci);
PVN_EXTERN_C void pvn_wmul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi);
PVN_EXTERN_C void pvn_wfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void pvn_ymul(_Float128 *const cr, _Float128 *const ci, const _Float128 ar, const _Float128 ai, const _Float128 br, const _Float128 bi);
PVN_EXTERN_C void pvn_yfma(_Float128 *const dr, _Float128 *const di, const _Float128 ar, const _Float128 ai, const _Float128 br, const _Float128 bi, const _Float128 cr, const _Float128 ci);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void pvn_ymul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi);
PVN_EXTERN_C void pvn_yfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci);
#endif /* ?PVN_QUADMATH */

PVN_EXTERN_C void PVN_FABI(pvn_cmul,PVN_CMUL)(float *const cr, float *const ci, const float *const ar, const float *const ai, const float *const br, const float *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_cfma,PVN_CFMA)(float *const dr, float *const di, const float *const ar, const float *const ai, const float *const br, const float *const bi, const float *const cr, const float *const ci);
PVN_EXTERN_C void PVN_FABI(pvn_zmul,PVN_ZMUL)(double *const cr, double *const ci, const double *const ar, const double *const ai, const double *const br, const double *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_zfma,PVN_ZFMA)(double *const dr, double *const di, const double *const ar, const double *const ai, const double *const br, const double *const bi, const double *const cr, const double *const ci);
PVN_EXTERN_C void PVN_FABI(pvn_wmul,PVN_WMUL)(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_wfma,PVN_WFMA)(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void PVN_FABI(pvn_ymul,PVN_YMUL)(_Float128 *const cr, _Float128 *const ci, const _Float128 *const ar, const _Float128 *const ai, const _Float128 *const br, const _Float128 *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_yfma,PVN_YFMA)(_Float128 *const dr, _Float128 *const di, const _Float128 *const ar, const _Float128 *const ai, const _Float128 *const br, const _Float128 *const bi, const _Float128 *const cr, const _Float128 *const ci);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void PVN_FABI(pvn_ymul,PVN_YMUL)(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi);
PVN_EXTERN_C void PVN_FABI(pvn_yfma,PVN_YFMA)(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_CMA_H */

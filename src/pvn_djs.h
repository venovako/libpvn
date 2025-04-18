#ifndef PVN_DJS_H
#define PVN_DJS_H

#ifndef PVN_H
#error pvn_djs.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C void PVN_FABI(pvn_djs_denc,PVN_DJS_DENC)(double *const e, const float *const f, const unsigned *const p, const unsigned *const q);
PVN_EXTERN_C void PVN_FABI(pvn_djs_ddec,PVN_DJS_DDEC)(const double *const e, unsigned *const p, unsigned *const q);
PVN_EXTERN_C void PVN_FABI(pvn_djs_xenc,PVN_DJS_XENC)(long double *const e, const double *const d, const unsigned *const p, const unsigned *const q);
PVN_EXTERN_C void PVN_FABI(pvn_djs_xdec,PVN_DJS_XDEC)(const long double *const e, unsigned *const p, unsigned *const q);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void PVN_FABI(pvn_djs_qenc,PVN_DJS_QENC)(__float128 *const e, const double *const d, const unsigned *const p, const unsigned *const q);
PVN_EXTERN_C void PVN_FABI(pvn_djs_qdec,PVN_DJS_QDEC)(const __float128 *const e, unsigned *const p, unsigned *const q);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void PVN_FABI(pvn_djs_qenc,PVN_DJS_QENC)(long double *const e, const double *const d, const unsigned *const p, const unsigned *const q);
PVN_EXTERN_C void PVN_FABI(pvn_djs_qdec,PVN_DJS_QDEC)(const long double *const e, unsigned *const p, unsigned *const q);
#endif /* ?PVN_QUADMATH */

#endif /* !PVN_DJS_H */

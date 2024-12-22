#ifndef PVN_DJS_H
#define PVN_DJS_H

#ifndef PVN_H
#error pvn_djs.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C void pvn_djs_denc_(double *const e, const float *const f, const unsigned *const p, const unsigned *const q);
PVN_EXTERN_C void pvn_djs_ddec_(const double *const e, unsigned *const p, unsigned *const q);
PVN_EXTERN_C void pvn_djs_xenc_(long double *const e, const double *const d, const unsigned *const p, const unsigned *const q);
PVN_EXTERN_C void pvn_djs_xdec_(const long double *const e, unsigned *const p, unsigned *const q);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void pvn_djs_qenc_(__float128 *const e, const double *const d, const unsigned *const p, const unsigned *const q);
PVN_EXTERN_C void pvn_djs_qdec_(const __float128 *const e, unsigned *const p, unsigned *const q);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void pvn_djs_qenc_(long double *const e, const double *const d, const unsigned *const p, const unsigned *const q);
PVN_EXTERN_C void pvn_djs_qdec_(const long double *const e, unsigned *const p, unsigned *const q);
#endif /* ?PVN_QUADMATH */
PVN_EXTERN_C void pvn_djs_xmkd_(const unsigned *const n, const double *const g, const unsigned *const ldg, long double *const d, unsigned *const o, int *const info);
PVN_EXTERN_C void pvn_djs_wmkd_(const unsigned *const n, const double complex *const g, const unsigned *const ldg, long double *const d, unsigned *const o, int *const info);
PVN_EXTERN_C void pvn_djs_xmkdpq_(const unsigned *const n, const unsigned *const m, long double *const d, unsigned *const o, int *const info);

#endif /* !PVN_DJS_H */

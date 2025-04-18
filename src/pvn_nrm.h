#ifndef PVN_NRM_H
#define PVN_NRM_H

#ifndef PVN_H
#error pvn_nrm.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C void PVN_FABI(pvn_snrm2,PVN_SNRM2)(float *const f, const int *const m, const float *const x, const unsigned *const ix);
PVN_EXTERN_C void PVN_FABI(pvn_cnrm2,PVN_CNRM2)(float *const f, const int *const m, const float *const xr, const unsigned *const ixr, const float *const xi, const unsigned *const ixi);
PVN_EXTERN_C void PVN_FABI(pvn_dnrm2,PVN_DNRM2)(double *const f, const int *const m, const double *const x, const unsigned *const ix);
PVN_EXTERN_C void PVN_FABI(pvn_znrm2,PVN_ZNRM2)(double *const f, const int *const m, const double *const xr, const unsigned *const ixr, const double *const xi, const unsigned *const ixi);
PVN_EXTERN_C void PVN_FABI(pvn_xnrm2,PVN_XNRM2)(long double *const f, const int *const m, const long double *const x, const unsigned *const ix);
PVN_EXTERN_C void PVN_FABI(pvn_wnrm2,PVN_WNRM2)(long double *const f, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void PVN_FABI(pvn_qnrm2,PVN_QNRM2)(__float128 *const f, const int *const m, const __float128 *const x, const unsigned *const ix);
PVN_EXTERN_C void PVN_FABI(pvn_ynrm2,PVN_YNRM2)(__float128 *const f, const int *const m, const __float128 *const xr, const unsigned *const ixr, const __float128 *const xi, const unsigned *const ixi);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void PVN_FABI(pvn_qnrm2,PVN_QNRM2)(long double *const f, const int *const m, const long double *const x, const unsigned *const ix);
PVN_EXTERN_C void PVN_FABI(pvn_ynrm2,PVN_YNRM2)(long double *const f, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_NRM_H */

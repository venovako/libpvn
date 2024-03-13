#ifndef PVN_NRM_H
#define PVN_NRM_H

#ifndef PVN_H
#error pvn_nrm.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C void pvn_snrm2_(float *const f, const int *const m, const float *const x, const unsigned *const ix);
PVN_EXTERN_C void pvn_cnrm2_(float *const f, const int *const m, const float *const xr, const unsigned *const ixr, const float *const xi, const unsigned *const ixi);
PVN_EXTERN_C void pvn_dnrm2_(double *const f, const int *const m, const double *const x, const unsigned *const ix);
PVN_EXTERN_C void pvn_znrm2_(double *const f, const int *const m, const double *const xr, const unsigned *const ixr, const double *const xi, const unsigned *const ixi);
PVN_EXTERN_C void pvn_xnrm2_(long double *const f, const int *const m, const long double *const x, const unsigned *const ix);
PVN_EXTERN_C void pvn_wnrm2_(long double *const f, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void pvn_qnrm2_(__float128 *const f, const int *const m, const __float128 *const x, const unsigned *const ix);
PVN_EXTERN_C void pvn_ynrm2_(__float128 *const f, const int *const m, const __float128 *const xr, const unsigned *const ixr, const __float128 *const xi, const unsigned *const ixi);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void pvn_qnrm2_(long double *const f, const int *const m, const long double *const x, const unsigned *const ix);
PVN_EXTERN_C void pvn_ynrm2_(long double *const f, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_NRM_H */

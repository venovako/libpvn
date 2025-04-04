#ifndef PVN_JV2_H
#define PVN_JV2_H

#ifndef PVN_H
#error pvn_jv2.h not intended for direct inclusion
#endif /* !PVN_H */

/* on input, *es should be 0, unless the tangent is wanted instead of the sine */
/* on output, -*es is the power of two by which the input has been scaled */
/* the return value is negative on error, and non-negative on success */

PVN_EXTERN_C int pvn_sljv2_(const float *const a11, const float *const a22, const float *const a21, float *const ch, float *const sh, int *const es);
PVN_EXTERN_C int pvn_dljv2_(const double *const a11, const double *const a22, const double *const a21, double *const ch, double *const sh, int *const es);
PVN_EXTERN_C int pvn_cljv2_(const float *const a11, const float *const a22, const float *const a21r, const float *const a21i, float *const ch, float *const shr, float *const shi, int *const es);
PVN_EXTERN_C int pvn_zljv2_(const double *const a11, const double *const a22, const double *const a21r, const double *const a21i, double *const ch, double *const shr, double *const shi, int *const es);

PVN_EXTERN_C int pvn_xljv2_(const long double *const a11, const long double *const a22, const long double *const a21, long double *const ch, long double *const sh, int *const es);
PVN_EXTERN_C int pvn_wljv2_(const long double *const a11, const long double *const a22, const long double *const a21r, const long double *const a21i, long double *const ch, long double *const shr, long double *const shi, int *const es);

#ifdef PVN_QUADMATH
PVN_EXTERN_C int pvn_qljv2_(const __float128 *const a11, const __float128 *const a22, const __float128 *const a21, __float128 *const ch, __float128 *const sh, int *const es);
PVN_EXTERN_C int pvn_yljv2_(const __float128 *const a11, const __float128 *const a22, const __float128 *const a21r, const __float128 *const a21i, __float128 *const ch, __float128 *const shr, __float128 *const shi, int *const es);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C int pvn_qljv2_(const long double *const a11, const long double *const a22, const long double *const a21, long double *const ch, long double *const sh, int *const es);
PVN_EXTERN_C int pvn_yljv2_(const long double *const a11, const long double *const a22, const long double *const a21r, const long double *const a21i, long double *const ch, long double *const shr, long double *const shi, int *const es);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_JV2_H */

#ifndef PVN_JV2_H
#define PVN_JV2_H

#ifndef PVN_H
#error pvn_jv2.h not intended for direct inclusion
#endif /* !PVN_H */

/* on input, *es should be:
    0: ch, sh; th unbounded;
    1: ch, sh; |th| bounded by 4/5;
   -1: ch, th; |th| bounded by 4/5;
   -2: ch, th; th unbounded;
   with the bound proposed by Krešimir Veselić
   ( https://doi.org/10.1007/BF01388689 )
*/
/* on output, -*es is the power of two by which the input has been scaled */
/* the return value is negative on error, and non-negative on success */

PVN_EXTERN_C int PVN_FABI(pvn_sljv2,PVN_SLJV2)(const float *const a11, const float *const a22, const float *const a21, float *const ch, float *const sh, int *const es);
PVN_EXTERN_C int PVN_FABI(pvn_dljv2,PVN_DLJV2)(const double *const a11, const double *const a22, const double *const a21, double *const ch, double *const sh, int *const es);
PVN_EXTERN_C int PVN_FABI(pvn_cljv2,PVN_CLJV2)(const float *const a11, const float *const a22, const float *const a21r, const float *const a21i, float *const ch, float *const shr, float *const shi, int *const es);
PVN_EXTERN_C int PVN_FABI(pvn_zljv2,PVN_ZLJV2)(const double *const a11, const double *const a22, const double *const a21r, const double *const a21i, double *const ch, double *const shr, double *const shi, int *const es);

PVN_EXTERN_C int PVN_FABI(pvn_xljv2,PVN_XLJV2)(const long double *const a11, const long double *const a22, const long double *const a21, long double *const ch, long double *const sh, int *const es);
PVN_EXTERN_C int PVN_FABI(pvn_wljv2,PVN_WLJV2)(const long double *const a11, const long double *const a22, const long double *const a21r, const long double *const a21i, long double *const ch, long double *const shr, long double *const shi, int *const es);

#ifdef PVN_QUADMATH
PVN_EXTERN_C int PVN_FABI(pvn_qljv2,PVN_QLJV2)(const __float128 *const a11, const __float128 *const a22, const __float128 *const a21, __float128 *const ch, __float128 *const sh, int *const es);
PVN_EXTERN_C int PVN_FABI(pvn_yljv2,PVN_YLJV2)(const __float128 *const a11, const __float128 *const a22, const __float128 *const a21r, const __float128 *const a21i, __float128 *const ch, __float128 *const shr, __float128 *const shi, int *const es);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C int PVN_FABI(pvn_qljv2,PVN_QLJV2)(const long double *const a11, const long double *const a22, const long double *const a21, long double *const ch, long double *const sh, int *const es);
PVN_EXTERN_C int PVN_FABI(pvn_yljv2,PVN_YLJV2)(const long double *const a11, const long double *const a22, const long double *const a21r, const long double *const a21i, long double *const ch, long double *const shr, long double *const shi, int *const es);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_JV2_H */

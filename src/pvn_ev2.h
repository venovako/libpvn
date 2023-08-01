#ifndef PVN_EV2_H
#define PVN_EV2_H

#ifndef PVN_H
#error pvn_ev2.h not intended for direct inclusion
#endif /* !PVN_H */

/* these functions are fail-safe only when rounding to nearest and if the correctly rounded arithmetic is available */
/* on input, *es should be 0, unless the tangent is wanted instead of the sine */
/* on output, *es is the power of two by which the eigenvalues have to be scaled */
/* the return value is negative on error, and non-negative on success */

PVN_EXTERN_C int sljev2_(const float *const a11, const float *const a22, const float *const a21, float *const cs, float *const sn, float *const l1, float *const l2, int *const es);
PVN_EXTERN_C int dljev2_(const double *const a11, const double *const a22, const double *const a21, double *const cs, double *const sn, double *const l1, double *const l2, int *const es);
PVN_EXTERN_C int xljev2_(const long double *const a11, const long double *const a22, const long double *const a21, long double *const cs, long double *const sn, long double *const l1, long double *const l2, int *const es);

PVN_EXTERN_C int cljev2_(const float *const a11, const float *const a22, const float *const a21r, const float *const a21i, float *const cs, float *const snr, float *const sni, float *const l1, float *const l2, int *const es);
PVN_EXTERN_C int zljev2_(const double *const a11, const double *const a22, const double *const a21r, const double *const a21i, double *const cs, double *const snr, double *const sni, double *const l1, double *const l2, int *const es);
PVN_EXTERN_C int wljev2_(const long double *const a11, const long double *const a22, const long double *const a21r, const long double *const a21i, long double *const cs, long double *const snr, long double *const sni, long double *const l1, long double *const l2, int *const es);

#endif /* !PVN_EV2_H */

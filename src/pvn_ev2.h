#ifndef PVN_EV2_H
#define PVN_EV2_H

#ifndef PVN_H
#error pvn_ev2.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C int sljev2_(const float *const a11, const float *const a22, const float *const a21, float *const cs, float *const sn, float *const l1, float *const l2, int *const es);
PVN_EXTERN_C int dljev2_(const double *const a11, const double *const a22, const double *const a21, double *const cs, double *const sn, double *const l1, double *const l2, int *const es);

PVN_EXTERN_C int cljev2_(const float *const a11, const float *const a22, const float *const a21r, const float *const a21i, float *const cs, float *const snr, float *const sni, float *const l1, float *const l2, int *const es);
PVN_EXTERN_C int zljev2_(const double *const a11, const double *const a22, const double *const a21r, const double *const a21i, double *const cs, double *const snr, double *const sni, double *const l1, double *const l2, int *const es);

#endif /* !PVN_EV2_H */

#ifndef PVN_DOT_H
#define PVN_DOT_H

#ifndef PVN_H
#error pvn_dot.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C void PVN_FABI(pvn_sdot,PVN_SDOT)(float *const d, const int *const m, const float *const x, const unsigned *const ix, const float *const y, const unsigned *const iy);
PVN_EXTERN_C void PVN_FABI(pvn_ddot,PVN_DDOT)(double *const d, const int *const m, const double *const x, const unsigned *const ix, const double *const y, const unsigned *const iy);
PVN_EXTERN_C void PVN_FABI(pvn_xdot,PVN_XDOT)(long double *const d, const int *const m, const long double *const x, const unsigned *const ix, const long double *const y, const unsigned *const iy);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void PVN_FABI(pvn_qdot,PVN_QDOT)(__float128 *const d, const int *const m, const __float128 *const x, const unsigned *const ix, const __float128 *const y, const unsigned *const iy);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void PVN_FABI(pvn_qdot,PVN_QDOT)(long double *const d, const int *const m, const long double *const x, const unsigned *const ix, const long double *const y, const unsigned *const iy);
#endif /* ?PVN_QUADMATH */

/* x^H y */

PVN_EXTERN_C void PVN_FABI(pvn_cdot,PVN_CDOT)(float *const zr, float *const zi, const int *const m, const float *const xr, const unsigned *const ixr, const float *const xi, const unsigned *const ixi, const float *const yr, const unsigned *const iyr, const float *const yi, const unsigned *const iyi);
PVN_EXTERN_C void PVN_FABI(pvn_zdot,PVN_ZDOT)(double *const zr, double *const zi, const int *const m, const double *const xr, const unsigned *const ixr, const double *const xi, const unsigned *const ixi, const double *const yr, const unsigned *const iyr, const double *const yi, const unsigned *const iyi);
PVN_EXTERN_C void PVN_FABI(pvn_wdot,PVN_WDOT)(long double *const zr, long double *const zi, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi, const long double *const yr, const unsigned *const iyr, const long double *const yi, const unsigned *const iyi);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void PVN_FABI(pvn_ydot,PVN_YDOT)(__float128 *const zr, __float128 *const zi, const int *const m, const __float128 *const xr, const unsigned *const ixr, const __float128 *const xi, const unsigned *const ixi, const __float128 *const yr, const unsigned *const iyr, const __float128 *const yi, const unsigned *const iyi);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void PVN_FABI(pvn_ydot,PVN_YDOT)(long double *const zr, long double *const zi, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi, const long double *const yr, const unsigned *const iyr, const long double *const yi, const unsigned *const iyi);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_DOT_H */

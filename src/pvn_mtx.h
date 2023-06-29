#ifndef PVN_MTX_H
#define PVN_MTX_H

#ifndef PVN_H
#error pvn_mtx.h not intended for direct inclusion
#endif /* !PVN_H */

static inline unsigned pvn_umin(const unsigned a, const unsigned b)
{
  return ((a <= b) ? a : b);
}

static inline unsigned pvn_umax(const unsigned a, const unsigned b)
{
  return ((a >= b) ? a : b);
}

PVN_EXTERN_C int pvn_rop_idf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB);
PVN_EXTERN_C int pvn_rop_id(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB);
PVN_EXTERN_C int pvn_rop_idl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB);

PVN_EXTERN_C int pvn_rop_absf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB);
PVN_EXTERN_C int pvn_rop_abs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB);
PVN_EXTERN_C int pvn_rop_absl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB);

PVN_EXTERN_C int pvn_rop_lgabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB);
PVN_EXTERN_C int pvn_rop_lgabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB);
PVN_EXTERN_C int pvn_rop_lgabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB);

PVN_EXTERN_C int pvn_rop_logabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB);
PVN_EXTERN_C int pvn_rop_logabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB);
PVN_EXTERN_C int pvn_rop_logabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB);

PVN_EXTERN_C int pvn_cop_idf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC);
PVN_EXTERN_C int pvn_cop_id(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC);
PVN_EXTERN_C int pvn_cop_idl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC);

PVN_EXTERN_C int pvn_cop_absf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC);
PVN_EXTERN_C int pvn_cop_abs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC);
PVN_EXTERN_C int pvn_cop_absl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC);

PVN_EXTERN_C int pvn_cop_lgabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC);
PVN_EXTERN_C int pvn_cop_lgabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC);
PVN_EXTERN_C int pvn_cop_lgabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC);

PVN_EXTERN_C int pvn_cop_logabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC);
PVN_EXTERN_C int pvn_cop_logabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC);
PVN_EXTERN_C int pvn_cop_logabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC);

#endif /* !PVN_MTX_H */

#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_rop_idf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  float mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      float x = (B[i + offB] = A[i + offA]);
      if (isnan(x))
        x = INFINITY;
      mB = fminf(mB,  x);
      MB = fminf(MB, -x);
    }
  }
  *minB = fminf(*minB, mB);
  *maxB = fmaxf(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_id(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      double x = (B[i + offB] = A[i + offA]);
      if (isnan(x))
        x = INFINITY;
      mB = fmin(mB,  x);
      MB = fmin(MB, -x);
    }
  }
  *minB = fmin(*minB, mB);
  *maxB = fmax(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_idl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  long double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      long double x = (B[i + offB] = A[i + offA]);
      if (isnan(x))
        x = INFINITY;
      mB = fminl(mB,  x);
      MB = fminl(MB, -x);
    }
  }
  *minB = fminl(*minB, mB);
  *maxB = fmaxl(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_absf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  float mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      float x = (B[i + offB] = fabsf(A[i + offA]));
      if (isnan(x))
        x = INFINITY;
      mB = fminf(mB,  x);
      MB = fminf(MB, -x);
    }
  }
  *minB = fminf(*minB, mB);
  *maxB = fmaxf(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_abs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      double x = (B[i + offB] = fabs(A[i + offA]));
      if (isnan(x))
        x = INFINITY;
      mB = fmin(mB,  x);
      MB = fmin(MB, -x);
    }
  }
  *minB = fmin(*minB, mB);
  *maxB = fmax(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_absl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  long double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      long double x = (B[i + offB] = fabsl(A[i + offA]));
      if (isnan(x))
        x = INFINITY;
      mB = fminl(mB,  x);
      MB = fminl(MB, -x);
    }
  }
  *minB = fminl(*minB, mB);
  *maxB = fmaxl(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_lgabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  float mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      float x = (B[i + offB] = log2f(fabsf(A[i + offA])));
      if (isnan(x))
        x = INFINITY;
      mB = fminf(mB,  x);
      MB = fminf(MB, -x);
    }
  }
  *minB = fminf(*minB, mB);
  *maxB = fmaxf(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_lgabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      double x = (B[i + offB] = log2(fabs(A[i + offA])));
      if (isnan(x))
        x = INFINITY;
      mB = fmin(mB,  x);
      MB = fmin(MB, -x);
    }
  }
  *minB = fmin(*minB, mB);
  *maxB = fmax(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_lgabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  long double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      long double x = (B[i + offB] = log2l(fabsl(A[i + offA])));
      if (isnan(x))
        x = INFINITY;
      mB = fminl(mB,  x);
      MB = fminl(MB, -x);
    }
  }
  *minB = fminl(*minB, mB);
  *maxB = fmaxl(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_logabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  float mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      float x = (B[i + offB] = log10f(fabsf(A[i + offA])));
      if (isnan(x))
        x = INFINITY;
      mB = fminf(mB,  x);
      MB = fminf(MB, -x);
    }
  }
  *minB = fminf(*minB, mB);
  *maxB = fmaxf(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_logabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      double x = (B[i + offB] = log10(fabs(A[i + offA])));
      if (isnan(x))
        x = INFINITY;
      mB = fmin(mB,  x);
      MB = fmin(MB, -x);
    }
  }
  *minB = fmin(*minB, mB);
  *maxB = fmax(*maxB, (MB = -MB));
  return 0;
}

int pvn_rop_logabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  long double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      long double x = (B[i + offB] = log10l(fabsl(A[i + offA])));
      if (isnan(x))
        x = INFINITY;
      mB = fminl(mB,  x);
      MB = fminl(MB, -x);
    }
  }
  *minB = fminl(*minB, mB);
  *maxB = fmaxl(*maxB, (MB = -MB));
  return 0;
}

int pvn_cop_idf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  float mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      float x = (B[i + offB] = crealf(z));
      float y = (C[i + offC] = cimagf(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fminf(mB,  x);
      MB = fminf(MB, -x);
      mC = fminf(mC,  y);
      MC = fminf(MC, -y);
    }
  }
  *minB = fminf(*minB, mB);
  *maxB = fmaxf(*maxB, (MB = -MB));
  *minC = fminf(*minC, mC);
  *maxC = fmaxf(*maxC, (MC = -MC));
  return 0;
}

int pvn_cop_id(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      double x = (B[i + offB] = creal(z));
      double y = (C[i + offC] = cimag(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fmin(mB,  x);
      MB = fmin(MB, -x);
      mC = fmin(mC,  y);
      MC = fmin(MC, -y);
    }
  }
  *minB = fmin(*minB, mB);
  *maxB = fmax(*maxB, (MB = -MB));
  *minC = fmin(*minC, mC);
  *maxC = fmax(*maxC, (MC = -MC));
  return 0;
}

int pvn_cop_idl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  long double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      long double x = (B[i + offB] = creall(z));
      long double y = (C[i + offC] = cimagl(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fminl(mB,  x);
      MB = fminl(MB, -x);
      mC = fminl(mC,  y);
      MC = fminl(MC, -y);
    }
  }
  *minB = fminl(*minB, mB);
  *maxB = fmaxl(*maxB, (MB = -MB));
  *minC = fminl(*minC, mC);
  *maxC = fmaxl(*maxC, (MC = -MC));
  return 0;
}

int pvn_cop_absf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  float mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      float x = (B[i + offB] = cabsf(z));
      float y = (C[i + offC] = cargf(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fminf(mB,  x);
      MB = fminf(MB, -x);
      mC = fminf(mC,  y);
      MC = fminf(MC, -y);
    }
  }
  *minB = fminf(*minB, mB);
  *maxB = fmaxf(*maxB, (MB = -MB));
  *minC = fminf(*minC, mC);
  *maxC = fmaxf(*maxC, (MC = -MC));
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_abs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      double x = (B[i + offB] = cabs(z));
      double y = (C[i + offC] = carg(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fmin(mB,  x);
      MB = fmin(MB, -x);
      mC = fmin(mC,  y);
      MC = fmin(MC, -y);
    }
  }
  *minB = fmin(*minB, mB);
  *maxB = fmax(*maxB, (MB = -MB));
  *minC = fmin(*minC, mC);
  *maxC = fmax(*maxC, (MC = -MC));
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_absl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  long double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      long double x = (B[i + offB] = cabsl(z));
      long double y = (C[i + offC] = cargl(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fminl(mB,  x);
      MB = fminl(MB, -x);
      mC = fminl(mC,  y);
      MC = fminl(MC, -y);
    }
  }
  *minB = fminl(*minB, mB);
  *maxB = fmaxl(*maxB, (MB = -MB));
  *minC = fminl(*minC, mC);
  *maxC = fmaxl(*maxC, (MC = -MC));
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_lgabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  float mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      float x = (B[i + offB] = log2f(cabsf(z)));
      float y = (C[i + offC] = cargf(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fminf(mB,  x);
      MB = fminf(MB, -x);
      mC = fminf(mC,  y);
      MC = fminf(MC, -y);
    }
  }
  *minB = fminf(*minB, mB);
  *maxB = fmaxf(*maxB, (MB = -MB));
  *minC = fminf(*minC, mC);
  *maxC = fmaxf(*maxC, (MC = -MC));
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_lgabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      double x = (B[i + offB] = log2(cabs(z)));
      double y = (C[i + offC] = carg(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fmin(mB,  x);
      MB = fmin(MB, -x);
      mC = fmin(mC,  y);
      MC = fmin(MC, -y);
    }
  }
  *minB = fmin(*minB, mB);
  *maxB = fmax(*maxB, (MB = -MB));
  *minC = fmin(*minC, mC);
  *maxC = fmax(*maxC, (MC = -MC));
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_lgabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  long double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      long double x = (B[i + offB] = log2l(cabsl(z)));
      long double y = (C[i + offC] = cargl(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fminl(mB,  x);
      MB = fminl(MB, -x);
      mC = fminl(mC,  y);
      MC = fminl(MC, -y);
    }
  }
  *minB = fminl(*minB, mB);
  *maxB = fmaxl(*maxB, (MB = -MB));
  *minC = fminl(*minC, mC);
  *maxC = fmaxl(*maxC, (MC = -MC));
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_logabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  float mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      float x = (B[i + offB] = log10f(cabsf(z)));
      float y = (C[i + offC] = cargf(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fminf(mB,  x);
      MB = fminf(MB, -x);
      mC = fminf(mC,  y);
      MC = fminf(MC, -y);
    }
  }
  *minB = fminf(*minB, mB);
  *maxB = fmaxf(*maxB, (MB = -MB));
  *minC = fminf(*minC, mC);
  *maxC = fmaxf(*maxC, (MC = -MC));
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_logabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      double x = (B[i + offB] = log10(cabs(z)));
      double y = (C[i + offC] = carg(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fmin(mB,  x);
      MB = fmin(MB, -x);
      mC = fmin(mC,  y);
      MC = fmin(MC, -y);
    }
  }
  *minB = fmin(*minB, mB);
  *maxB = fmax(*maxB, (MB = -MB));
  *minC = fmin(*minC, mC);
  *maxC = fmax(*maxC, (MC = -MC));
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_logabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  long double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      long double x = (B[i + offB] = log10l(cabsl(z)));
      long double y = (C[i + offC] = cargl(z));
      if (isnan(x))
        x = INFINITY;
      if (isnan(y))
        y = INFINITY;
      mB = fminl(mB,  x);
      MB = fminl(MB, -x);
      mC = fminl(mC,  y);
      MC = fminl(MC, -y);
    }
  }
  *minB = fminl(*minB, mB);
  *maxB = fmaxl(*maxB, (MB = -MB));
  *minC = fminl(*minC, mC);
  *maxC = fmaxl(*maxC, (MC = -MC));
  /* check for overflow of |z| */
  return (MB == INFINITY);
}
#endif /* ?PVN_TEST */

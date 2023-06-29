#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_rop_idf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = A[i + offA];
  }
  return 0;
}

int pvn_rop_id(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = A[i + offA];
  }
  return 0;
}

int pvn_rop_idl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = A[i + offA];
  }
  return 0;
}

int pvn_rop_absf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = fabsf(A[i + offA]);
  }
  return 0;
}

int pvn_rop_abs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = fabs(A[i + offA]);
  }
  return 0;
}

int pvn_rop_absl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = fabsl(A[i + offA]);
  }
  return 0;
}

int pvn_rop_lgabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = log2f(fabsf(A[i + offA]));
  }
  return 0;
}

int pvn_rop_lgabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = log2(fabs(A[i + offA]));
  }
  return 0;
}

int pvn_rop_lgabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = log2l(fabsl(A[i + offA]));
  }
  return 0;
}

int pvn_rop_logabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = log10f(fabsf(A[i + offA]));
  }
  return 0;
}

int pvn_rop_logabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = log10(fabs(A[i + offA]));
  }
  return 0;
}

int pvn_rop_logabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB)
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
  if (m > pvn_umin(ldA, ldB))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
#pragma omp simd
    for (unsigned i = 0u; i < m; ++i)
      B[i + offB] = log10l(fabsl(A[i + offA]));
  }
  return 0;
}

int pvn_cop_idf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      B[i + offB] = crealf(z);
      C[i + offC] = cimagf(z);
    }
  }
  return 0;
}

int pvn_cop_id(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      B[i + offB] = creal(z);
      C[i + offC] = cimag(z);
    }
  }
  return 0;
}

int pvn_cop_idl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      B[i + offB] = creall(z);
      C[i + offC] = cimagl(z);
    }
  }
  return 0;
}

int pvn_cop_absf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      B[i + offB] = cabsf(z);
      C[i + offC] = cargf(z);
    }
  }
  return 0;
}

int pvn_cop_abs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      B[i + offB] = cabs(z);
      C[i + offC] = carg(z);
    }
  }
  return 0;
}

int pvn_cop_absl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      B[i + offB] = cabsl(z);
      C[i + offC] = cargl(z);
    }
  }
  return 0;
}

int pvn_cop_lgabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      B[i + offB] = log2f(cabsf(z));
      C[i + offC] = cargf(z);
    }
  }
  return 0;
}

int pvn_cop_lgabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      B[i + offB] = log2(cabs(z));
      C[i + offC] = carg(z);
    }
  }
  return 0;
}

int pvn_cop_lgabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      B[i + offB] = log2l(cabsl(z));
      C[i + offC] = cargl(z);
    }
  }
  return 0;
}

int pvn_cop_logabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      B[i + offB] = log10f(cabsf(z));
      C[i + offC] = cargf(z);
    }
  }
  return 0;
}

int pvn_cop_logabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      B[i + offB] = log10(cabs(z));
      C[i + offC] = carg(z);
    }
  }
  return 0;
}

int pvn_cop_logabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC)
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
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -9;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      B[i + offB] = log10l(cabsl(z));
      C[i + offC] = cargl(z);
    }
  }
  return 0;
}
#endif /* ?PVN_TEST */

#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  (void)printf("PVN_VECLEN = %u\n", PVN_FABI(pvn_vec_len,PVN_VEC_LEN)());
  (void)printf("PVN_SAFELEN(float) = %zu\n", PVN_SAFELEN(float));
  (void)printf("PVN_SAFELEN(double) = %zu\n", PVN_SAFELEN(double));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
unsigned PVN_FABI(pvn_vec_len,PVN_VEC_LEN)()
{
  return (PVN_VECLEN);
}

double PVN_FABI(pvn_v1d_nrmf,PVN_V1D_NRMF)(const size_t *const n, const double *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(h1d:double:omp_out=pvn_v1d_hypot(omp_out,omp_in)) initializer(omp_priv=0.0)
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  const size_t m = *n;
  double f = 0.0;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x) reduction(h1d:f)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i)
    f = pvn_v1d_hypot(f, x[i]);
  return f;
}
#if (defined(__AVX__) && defined(__FMA__))
double PVN_FABI(pvn_v2d_nrmf,PVN_V2D_NRMF)(const size_t *const n, const double *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(h2d:__m128d:omp_out=pvn_v2d_hypot(omp_out,omp_in)) initializer(omp_priv=_mm_setzero_pd())
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (*n & 1u)
    return -1.0;
  if (!*n)
    return -0.0;
  const size_t m = (*n >> 1u);
  register __m128d f = _mm_setzero_pd();
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x) reduction(h2d:f)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i)
    f = pvn_v2d_hypot(f, _mm_load_pd(x + (i << 1u)));
  return pvn_v2d_hypot_red(f);
}

double PVN_FABI(pvn_v4d_nrmf,PVN_V4D_NRMF)(const size_t *const n, const double *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(h4d:__m256d:omp_out=pvn_v4d_hypot(omp_out,omp_in)) initializer(omp_priv=_mm256_setzero_pd())
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (*n & 3u)
    return -1.0;
  if (!*n)
    return -0.0;
  const size_t m = (*n >> 2u);
  register __m256d f = _mm256_setzero_pd();
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x) reduction(h4d:f)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i)
    f = pvn_v4d_hypot(f, _mm256_load_pd(x + (i << 2u)));
  return pvn_v4d_hypot_red(f);
}
#ifdef __AVX512F__
double PVN_FABI(pvn_v8d_nrmf,PVN_V8D_NRMF)(const size_t *const n, const double *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(h8d:__m512d:omp_out=pvn_v8d_hypot(omp_out,omp_in)) initializer(omp_priv=_mm512_setzero_pd())
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (*n & 7u)
    return -1.0;
  if (!*n)
    return -0.0;
  const size_t m = (*n >> 3u);
  register __m512d f = _mm512_setzero_pd();
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x) reduction(h8d:f)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i)
    f = pvn_v8d_hypot(f, _mm512_load_pd(x + (i << 3u)));
  return pvn_v8d_hypot_red(f);
}
#endif /* __AVX512F__ */
#endif /* __AVX__ && __FMA__ */
#endif /* ?PVN_TEST */

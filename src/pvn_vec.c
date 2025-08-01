#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc > 3) {
    (void)fprintf(stderr, "%s [n [seed]]\n", *argv);
    return EXIT_FAILURE;
  }
  (void)printf("PVN_VECLEN = %u\n", PVN_VECLEN);
  (void)printf("PVN_SAFELEN(float) = %zu\n", PVN_SAFELEN(float));
  (void)printf("PVN_SAFELEN(double) = %zu\n", PVN_SAFELEN(double));
#if (defined(__AVX__) && defined(__FMA__))
  if (argc > 1) {
    const size_t n = pvn_atoz(argv[1]);
    if (!n)
      return EXIT_FAILURE;
    double *const x = (double*)aligned_alloc(64u, n * sizeof(double));
    if (!x)
      return EXIT_FAILURE;
#ifndef _WIN32
    if (argc > 2)
      srand48(atol(argv[2]));
    for (size_t i = 0u; i < n; ++i)
      x[i] = drand48();
#endif /* !_WIN32 */
    (void)printf("pvn_v2d_nrmf=");
    (void)fflush(stdout);
    long long t = pvn_time_mono_ns();
    double f = PVN_FABI(pvn_v2d_nrmf,PVN_V2D_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%#.17e in %21lld ns\n", f, t);
    (void)printf("pvn_v4d_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_v4d_nrmf,PVN_V4D_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%#.17e in %21lld ns\n", f, t);
#ifdef __AVX512F__
    (void)printf("pvn_v8d_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_v8d_nrmf,PVN_V8D_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%#.17e in %21lld ns\n", f, t);
#endif /* __AVX512F__ */
    free(x);
  }
#endif /* __AVX__ && __FMA__ */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
unsigned PVN_FABI(pvn_vec_len,PVN_VEC_LEN)()
{
  return (PVN_VECLEN);
}

double PVN_FABI(pvn_v1d_nrmf,PVN_V1D_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  const size_t m = *n;
  double f = 0.0;
  for (size_t i = 0u; i < m; ++i)
    f = pvn_v1d_hypot(f, x[i]);
  return f;
}
#if (defined(__AVX__) && defined(__FMA__))
double PVN_FABI(pvn_v2d_nrmf,PVN_V2D_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (*n & 1u)
    return -1.0;
  if (!*n)
    return -0.0;
  const size_t m = *n;
  register __m128d f = _mm_setzero_pd();
  for (size_t i = 0u; i < m; i += 2u)
    f = pvn_v2d_hypot(f, _mm_load_pd(x + i));
  return pvn_v2d_hypot_red(f);
}

double PVN_FABI(pvn_v4d_nrmf,PVN_V4D_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (*n & 3u)
    return -1.0;
  if (!*n)
    return -0.0;
  const size_t m = *n;
  register __m256d f = _mm256_setzero_pd();
  for (size_t i = 0u; i < m; i += 4u)
    f = pvn_v4d_hypot(f, _mm256_load_pd(x + i));
  return pvn_v4d_hypot_red(f);
}
#ifdef __AVX512F__
double PVN_FABI(pvn_v8d_nrmf,PVN_V8D_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (*n & 7u)
    return -1.0;
  if (!*n)
    return -0.0;
  const size_t m = *n;
  register __m512d f = _mm512_setzero_pd();
  for (size_t i = 0u; i < m; i += 8u)
    f = pvn_v8d_hypot(f, _mm512_load_pd(x + i));
  return pvn_v8d_hypot_red(f);
}
#endif /* __AVX512F__ */
#endif /* __AVX__ && __FMA__ */
#endif /* ?PVN_TEST */

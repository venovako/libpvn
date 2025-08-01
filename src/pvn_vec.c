#include "pvn.h"

#ifdef PVN_TEST
#ifdef PVN_LAPACK
extern double PVN_FABI(dnrm2,DNRM2)(const size_t *const n, const double *const x, const int64_t *const incx);

static double PVN_FABI(pvn_lad_nrmf,PVN_LAD_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  const int64_t incx = INT64_C(1);
  return PVN_FABI(dnrm2,DNRM2)(n, x, &incx);
}
#endif /* PVN_LAPACK */
#ifdef PVN_MPFR
static double PVN_FABI(pvn_mpd_nrmf,PVN_MPD_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  const size_t m = *n;
  mpfr_t mf, mx;
  (void)mpfr_init_set_d(mf, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(mx, 0.0, MPFR_RNDN);
  for (size_t i = 0u; i < m; ++i) {
    (void)mpfr_set_d(mx, x[i], MPFR_RNDN);
    (void)mpfr_hypot(mf, mf, mx, MPFR_RNDN);
  }
  const double f = mpfr_get_d(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}
#endif /* PVN_MPFR */

static double erelerr(const double e, const double f)
{
  return ((e == 0.0) ? -0.0 : (fabs(e - f) / scalbn(fabs(e), -53)));
}

int main(int argc, char *argv[])
{
  if (argc > 4) {
    (void)fprintf(stderr, "%s [n [seed [prec]]]\n", *argv);
    return EXIT_FAILURE;
  }
  (void)printf("PVN_VECLEN = %u\n", PVN_VECLEN);
  (void)printf("PVN_SAFELEN(float) = %zu\n", PVN_SAFELEN(float));
  (void)printf("PVN_SAFELEN(double) = %zu\n", PVN_SAFELEN(double));
#if (defined(__AVX__) && defined(__FMA__) && !defined(_WIN32))
  if (argc > 1) {
    const size_t n = pvn_atoz(argv[1]);
    if (!n)
      return EXIT_FAILURE;
    double *const x = (double*)aligned_alloc(64u, n * sizeof(double));
    if (!x)
      return EXIT_FAILURE;
    if (argc > 2) // e.g., the result of `echo "$RANDOM * $RANDOM" | bc`
      srand48(atol(argv[2]));
    for (size_t i = 0u; i < n; ++i)
      x[i] = drand48();
#ifdef PVN_MPFR
    mpfr_rnd_t rnd = MPFR_RNDN;
    mpfr_prec_t prec = ((argc > 3) ? atol(argv[3]) : 2048l);
    mpfr_exp_t emin = __MPFR_EXP_INVALID, emax = __MPFR_EXP_INVALID;
    if (PVN_FABI(pvn_mpfr_start,PVN_MPFR_START)(&rnd, &prec, &emin, &emax))
      return EXIT_FAILURE;
#endif /* PVN_MPFR */
    long long t = 0ll;
#ifdef PVN_MPFR
    (void)printf("pvn_mpd_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    const double e = PVN_FABI(pvn_mpd_nrmf,PVN_MPD_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", e, 0.0, t);
#else /* !PVN_MPFR */
    const double e = 0.0;
#endif /* ?PVN_MPFR */
    double f = 0.0;
#ifdef PVN_LAPACK
    (void)printf("pvn_lad_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_lad_nrmf,PVN_LAD_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
#endif /* PVN_LAPACK */
    (void)printf("pvn_crd_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_crd_nrmf,PVN_CRD_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
    (void)printf("pvn_v1d_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_v1d_nrmf,PVN_V1D_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
    (void)printf("pvn_v2d_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_v2d_nrmf,PVN_V2D_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
    (void)printf("pvn_v4d_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_v4d_nrmf,PVN_V4D_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
#ifdef __AVX512F__
    (void)printf("pvn_v8d_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_v8d_nrmf,PVN_V8D_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
#endif /* __AVX512F__ */
#ifdef PVN_MPFR
    (void)PVN_FABI(pvn_mpfr_stop,PVN_MPFR_STOP)();
#endif /* PVN_MPFR */
    free(x);
  }
#endif /* __AVX__ && __FMA__ && !_WIN32 */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
unsigned PVN_FABI(pvn_vec_len,PVN_VEC_LEN)()
{
  return (PVN_VECLEN);
}

double PVN_FABI(pvn_crd_nrmf,PVN_CRD_NRMF)(const size_t *const n, const double *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(hcd:double:omp_out=hypot(omp_out,omp_in)) initializer(omp_priv=0.0)
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  const size_t m = *n;
  double f = 0.0;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x) reduction(hcd:f)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i)
    f = hypot(f, x[i]);
  return f;
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

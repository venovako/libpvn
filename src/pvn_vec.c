#include "pvn.h"

#ifdef PVN_TEST
#if (defined(__AVX__) && defined(__FMA__) && !defined(_WIN32))
static double erelerr(const double e, const double f)
{
  return ((e == 0.0) ? -0.0 : (fabs(e - f) / scalbn(fabs(e), -53)));
}
#endif /* __AVX__ && __FMA__ && !_WIN32 */
int main(int argc, char *argv[])
{
#if (defined(__AVX__) && defined(__FMA__) && !defined(_WIN32))
  if (argc > 4) {
    (void)fprintf(stderr, "%s [n [seed [exact]]]\n", *argv);
    return EXIT_FAILURE;
  }
  (void)fprintf(stderr, "PVN_VECLEN = %u\n", PVN_VECLEN);
  (void)fprintf(stderr, "PVN_SAFELEN(float) = %zu\n", PVN_SAFELEN(float));
  (void)fprintf(stderr, "PVN_SAFELEN(double) = %zu\n", PVN_SAFELEN(double));
  if (argc > 1) {
    const size_t n = pvn_atoz(argv[1]);
    if (!n)
      return EXIT_FAILURE;
    if (argc > 2) {
      long s = atol(argv[2]);
      if (!seed48((unsigned short*)&s))
        return EXIT_FAILURE;
    }
    else {
      const int u = PVN_FABI(pvn_ran_open,PVN_RAN_OPEN)();
      if (u < 0)
        return EXIT_FAILURE;
      alignas(alignof(long)) unsigned short s[4] = { UINT16_C(0), UINT16_C(0), UINT16_C(0), UINT16_C(0) };
      while (!PVN_FABI(pvn_ran_16,PVN_RAN_16)(&u, s) || !*s) /* loop */;
      while (!PVN_FABI(pvn_ran_16,PVN_RAN_16)(&u, s + 1) || !s[1]) /* loop */;
      while (!PVN_FABI(pvn_ran_16,PVN_RAN_16)(&u, s + 2) || !s[2]) /* loop */;
      if (!seed48(s))
        return EXIT_FAILURE;
      if (PVN_FABI(pvn_ran_close,PVN_RAN_CLOSE)(&u))
        return EXIT_FAILURE;
      (void)fprintf(stderr, "SEED = %15ld\n", *(const long*)s);
    }
    double *const x = (double*)aligned_alloc(64u, n * sizeof(double));
    if (!x)
      return EXIT_FAILURE;
    for (size_t i = 0u; i < n; ++i)
      x[i] = drand48();
    long long t = 0ll;
    double e = ((argc > 3) ? atof(argv[3]) : 0.0);
#if (defined(PVN_MPFR) && !defined(_OPENMP))
    if (argc <= 3) {
      mpfr_rnd_t rnd = MPFR_RNDN;
      mpfr_prec_t prec = 2048l;
      mpfr_exp_t emin = __MPFR_EXP_INVALID, emax = __MPFR_EXP_INVALID;
      if (PVN_FABI(pvn_mpfr_start,PVN_MPFR_START)(&rnd, &prec, &emin, &emax))
        return EXIT_FAILURE;
      (void)printf("pvn_mpd_nrmf=");
      (void)fflush(stdout);
      t = pvn_time_mono_ns();
      e = PVN_FABI(pvn_mpd_nrmf,PVN_MPD_NRMF)(&n, x);
      t = pvn_time_mono_ns() - t;
      (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", e, 0.0, t);
    }
#endif /* PVN_MPFR && !_OPENMP */
    double f = 0.0;
#if (defined(PVN_LAPACK) && !defined(_OPENMP))
    (void)printf("pvn_lad_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_lad_nrmf,PVN_LAD_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
#endif /* PVN_LAPACK && !_OPENMP */
    (void)printf("pvn_red_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    f = PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&n, x);
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
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
#else /* !__AVX__ || !__FMA__ || _WIN32 */
  (void)printf("PVN_VECLEN = %u\n", PVN_VECLEN);
  (void)printf("PVN_SAFELEN(float) = %zu\n", PVN_SAFELEN(float));
  (void)printf("PVN_SAFELEN(double) = %zu\n", PVN_SAFELEN(double));
#endif /* __AVX__ && __FMA__ && !_WIN32 */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
unsigned PVN_FABI(pvn_vec_len,PVN_VEC_LEN)()
{
  return (PVN_VECLEN);
}
#ifdef PVN_MPFR
double PVN_FABI(pvn_mpd_nrmf,PVN_MPD_NRMF)(const size_t *const n, const double *const x)
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
#ifdef PVN_LAPACK
extern double PVN_FABI(dnrm2,DNRM2)(const size_t *const n, const double *const x, const int64_t *const incx);
double PVN_FABI(pvn_lad_nrmf,PVN_LAD_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  const int64_t incx = INT64_C(1);
  return PVN_FABI(dnrm2,DNRM2)(n, x, &incx);
}
#endif /* PVN_LAPACK */
double PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return hypot(x[0], x[1]);
  const size_t l = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t r = (*n - l);
  return hypot(PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&l, x), PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&r, (x + l)));
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
  /* the three Blue's accumulators as in DNRM2, plus the accumulator for subnormal inputs */
  const double tsml = __builtin_scalbn(1.0, -511);
  const double tbig = __builtin_scalbn(1.0,  486);
  double dnr = 0.0, sml = 0.0, med = 0.0, big = 0.0;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcd:dnr,sml,med,big)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const double y = __builtin_fabs(x[i]);
    if (y > 0.0) {
      if (y < DBL_MIN)
        dnr = hypot(dnr, __builtin_scalbn(y, 52));
      else if (y < tsml)
        sml = hypot(sml, y);
      else if (y > tbig)
        big = hypot(big, y);
      else /* med */
        med = hypot(med, y);
    }
  }
  if (dnr > 0.0)
    sml = hypot(sml, __builtin_scalbn(dnr, -52));
  if (sml > 0.0)
    med = hypot(med, sml);
  if (med > 0.0)
    big = hypot(big, med);
  return big;
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

#include "pvn.h"

#ifdef PVN_TEST
#if (defined(__AVX__) && defined(__FMA__) && !defined(_WIN32))
static double erelerr(const double e, const double f)
{
  return ((e == 0.0) ? -0.0 : (fabs(e - f) / scalbn(fabs(e), -53)));
}
int main(int argc, char *argv[])
{
  if ((argc < 2) || (argc > 4)) {
    (void)fprintf(stderr, "%s n [seed [exact]]\n", *argv);
    return EXIT_FAILURE;
  }
  const size_t n = pvn_atoz(argv[1]);
  if (!n)
    return EXIT_FAILURE;
  const size_t nb = (n * sizeof(double));
  if (nb % PVN_VECLEN)
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
  double *const x = (double*)aligned_alloc(64u, nb);
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
  (void)printf("pvn_dnrm2["
#ifdef _OPENMP
               "p"
#else /* !_OPENMP */
               "s"
#endif /* ?_OPENMP */
               "]=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = PVN_FABI(pvn_dnrm2,PVN_DNRM2)(&n, x);
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
#ifndef _OPENMP
  (void)printf("pvn_rfd_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(&n, x);
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
  (void)printf("pvn_rxd_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = PVN_FABI(pvn_rxd_nrmf,PVN_RXD_NRMF)(&n, x);
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
  (void)printf("pvn_ryd_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = PVN_FABI(pvn_ryd_nrmf,PVN_RYD_NRMF)(&n, x);
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
#ifdef __AVX512F__
  (void)printf("pvn_rzd_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = PVN_FABI(pvn_rzd_nrmf,PVN_RZD_NRMF)(&n, x);
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, erelerr(e, f), t);
#endif /* __AVX512F__ */
#endif /* !_OPENMP */
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
  return EXIT_SUCCESS;
}
#else /* !__AVX__ || !__FMA__ || _WIN32 */
int main(/* int argc, char *argv[] */)
{
  const double x[4][2] = {{-1.0, 1.0}, { 1.0,-1.0}, { 2.0, 2.0}, {-2.0,-2.0}};
  const size_t n = (size_t)4u;
  /* expected output: 4.472136 */
  (void)printf("%#F\n", PVN_FABI(pvn_znrm2,PVN_ZNRM2)(&n, &(x[0][0])));
  return EXIT_SUCCESS;
}
#endif /* __AVX__ && __FMA__ && !_WIN32 */
#else /* !PVN_TEST */
#ifdef PVN_MPFR
float PVN_FABI(pvn_mps_nrmf,PVN_MPS_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0f;
  const size_t m = *n;
  mpfr_t mf, mx;
  (void)mpfr_init_set_d(mf, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(mx, 0.0, MPFR_RNDN);
  for (size_t i = 0u; i < m; ++i) {
    (void)mpfr_set_flt(mx, x[i], MPFR_RNDN);
#ifdef PVN_NRM_SAFE
    (void)mpfr_hypot(mf, mf, mx, MPFR_RNDN);
#else /* !PVN_NRM_SAFE */
    (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
#endif /* ?PVN_NRM_SAFE */
  }
#ifndef PVN_NRM_SAFE
  (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
#endif /* !PVN_NRM_SAFE */
  const float f = mpfr_get_flt(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}

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
#ifdef PVN_NRM_SAFE
    (void)mpfr_hypot(mf, mf, mx, MPFR_RNDN);
#else /* !PVN_NRM_SAFE */
    (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
#endif /* ?PVN_NRM_SAFE */
  }
#ifndef PVN_NRM_SAFE
  (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
#endif /* !PVN_NRM_SAFE */
  const double f = mpfr_get_d(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}

long double PVN_FABI(pvn_mpx_nrmf,PVN_MPX_NRMF)(const size_t *const n, const long double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0L;
  const size_t m = *n;
  mpfr_t mf, mx;
  (void)mpfr_init_set_ld(mf, 0.0L, MPFR_RNDN);
  (void)mpfr_init_set_ld(mx, 0.0L, MPFR_RNDN);
  for (size_t i = 0u; i < m; ++i) {
    (void)mpfr_set_ld(mx, x[i], MPFR_RNDN);
#ifdef PVN_NRM_SAFE
    (void)mpfr_hypot(mf, mf, mx, MPFR_RNDN);
#else /* !PVN_NRM_SAFE */
    (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
#endif /* ?PVN_NRM_SAFE */
  }
#ifndef PVN_NRM_SAFE
  (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
#endif /* !PVN_NRM_SAFE */
  const long double f = mpfr_get_ld(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}
#endif /* PVN_MPFR */

#ifdef PVN_LAPACK
extern float PVN_FABI(snrm2,SNRM2)(const size_t *const n, const float *const x, const int64_t *const incx);
extern double PVN_FABI(dnrm2,DNRM2)(const size_t *const n, const double *const x, const int64_t *const incx);

float PVN_FABI(pvn_las_nrmf,PVN_LAS_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0f;
  const int64_t incx = INT64_C(1);
  return PVN_FABI(snrm2,SNRM2)(n, x, &incx);
}

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

float PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0f;
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return hypotf(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const float fl = PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&nl, x);
  const float fr = PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&nr, (x + nl));
  return hypotf(fl, fr);
}

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
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const double fl = PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&nl, x);
  const double fr = PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&nr, (x + nl));
  return hypot(fl, fr);
}

long double PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(const size_t *const n, const long double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0L;
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return hypotl(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const long double fl = PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&nl, x);
  const long double fr = PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&nr, (x + nl));
  return hypotl(fl, fr);
}

float PVN_FABI(pvn_crs_nrmf,PVN_CRS_NRMF)(const size_t *const n, const float *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(hcf:float:omp_out=hypotf(omp_out,omp_in)) initializer(omp_priv=0.0f)
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0f;
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return hypotf(x[0], x[1]);
  const size_t m = *n;
  /* the three Blue's accumulators as in SNRM2, plus the accumulator for subnormal inputs */
  const float tsml = __builtin_scalbnf(1.0f, -63);
  const float tbig = __builtin_scalbnf(1.0f,  52);
  float dnr = 0.0f, sml = 0.0f, med = 0.0f, big = 0.0f;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcf:dnr,sml,med,big)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const float y = __builtin_fabsf(x[i]);
    if (y > 0.0f) {
      if (y < FLT_MIN)
        dnr = hypotf(dnr, __builtin_scalbnf(y, 23));
      else if (y < tsml)
        sml = hypotf(sml, y);
      else if (y > tbig)
        big = hypotf(big, y);
      else /* med */
        med = hypotf(med, y);
    }
  }
  if (dnr > 0.0f)
    sml = hypotf(sml, __builtin_scalbnf(dnr, -23));
  if (sml > 0.0f)
    med = hypotf(med, sml);
  if (med > 0.0f)
    big = hypotf(big, med);
  return big;
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
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return hypot(x[0], x[1]);
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

long double PVN_FABI(pvn_crx_nrmf,PVN_CRX_NRMF)(const size_t *const n, const long double *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(hcx:long double:omp_out=hypotl(omp_out,omp_in)) initializer(omp_priv=0.0L)
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0L;
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return hypotl(x[0], x[1]);
  const size_t m = *n;
  /* the three Blue's accumulators, plus the accumulator for subnormal inputs */
  const long double tsml = __builtin_scalbnl(1.0L, -8191);
  const long double tbig = __builtin_scalbnl(1.0L,  8160);
  long double dnr = 0.0L, sml = 0.0L, med = 0.0L, big = 0.0L;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcx:dnr,sml,med,big)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const long double y = __builtin_fabsl(x[i]);
    if (y > 0.0L) {
      if (y < LDBL_MIN)
        dnr = hypotl(dnr, __builtin_scalbnl(y, 63));
      else if (y < tsml)
        sml = hypotl(sml, y);
      else if (y > tbig)
        big = hypotl(big, y);
      else /* med */
        med = hypotl(med, y);
    }
  }
  if (dnr > 0.0L)
    sml = hypotl(sml, __builtin_scalbnl(dnr, -63));
  if (sml > 0.0L)
    med = hypotl(med, sml);
  if (med > 0.0L)
    big = hypotl(big, med);
  return big;
}

float PVN_FABI(pvn_snrm2,PVN_SNRM2)(const size_t *const n, const float *const x)
{
#ifdef _OPENMP
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  const size_t mt = (size_t)omp_get_max_threads();
  float p[mt];
  for (size_t i = 0u; i < mt; ++i)
    p[i] = 0.0f;
#pragma omp parallel default(none) shared(n,x,p)
  {
    const size_t nt = (size_t)omp_get_num_threads();
    const size_t tn = (size_t)omp_get_thread_num();
    lldiv_t qr = lldiv((long long)*n, (long long)nt);
    const size_t q = (size_t)(qr.quot);
    const size_t r = (size_t)(qr.rem);
    size_t m = q, o = (tn * q);
    if (r) {
      if (tn < r) {
        ++m;
        o += tn;
      }
      else
        o += r;
    }
    if (m)
      p[tn] = PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&m, (x + o));
  }
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(n, x);
#endif /* _OPENMP */
}

float PVN_FABI(pvn_cnrm2,PVN_CNRM2)(const size_t *const n, const float *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_snrm2,PVN_SNRM2)(&m, x);
}

double PVN_FABI(pvn_dnrm2,PVN_DNRM2)(const size_t *const n, const double *const x)
{
#ifdef _OPENMP
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  const size_t mt = (size_t)omp_get_max_threads();
  double p[mt];
  for (size_t i = 0u; i < mt; ++i)
    p[i] = 0.0;
#pragma omp parallel default(none) shared(n,x,p)
  {
    const size_t nt = (size_t)omp_get_num_threads();
    const size_t tn = (size_t)omp_get_thread_num();
    lldiv_t qr = lldiv((long long)*n, (long long)nt);
    const size_t q = (size_t)(qr.quot);
    const size_t r = (size_t)(qr.rem);
    size_t m = q, o = (tn * q);
    if (r) {
      if (tn < r) {
        ++m;
        o += tn;
      }
      else
        o += r;
    }
    if (m)
      p[tn] = PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&m, (x + o));
  }
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(n, x);
#endif /* ?_OPENMP */
}

double PVN_FABI(pvn_znrm2,PVN_ZNRM2)(const size_t *const n, const double *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_dnrm2,PVN_DNRM2)(&m, x);
}

long double PVN_FABI(pvn_xnrm2,PVN_XNRM2)(const size_t *const n, const long double *const x)
{
#ifdef _OPENMP
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  const size_t mt = (size_t)omp_get_max_threads();
  long double p[mt];
  for (size_t i = 0u; i < mt; ++i)
    p[i] = 0.0L;
#pragma omp parallel default(none) shared(n,x,p)
  {
    const size_t nt = (size_t)omp_get_num_threads();
    const size_t tn = (size_t)omp_get_thread_num();
    lldiv_t qr = lldiv((long long)*n, (long long)nt);
    const size_t q = (size_t)(qr.quot);
    const size_t r = (size_t)(qr.rem);
    size_t m = q, o = (tn * q);
    if (r) {
      if (tn < r) {
        ++m;
        o += tn;
      }
      else
        o += r;
    }
    if (m)
      p[tn] = PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&m, (x + o));
  }
  return PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(n, x);
#endif /* ?_OPENMP */
}

long double PVN_FABI(pvn_wnrm2,PVN_WNRM2)(const size_t *const n, const long double *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_xnrm2,PVN_XNRM2)(&m, x);
}

#ifdef PVN_QUADMATH
#ifdef PVN_MPFR
__float128 PVN_FABI(pvn_mpq_nrmf,PVN_MPQ_NRMF)(const size_t *const n, const __float128 *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0q;
  const size_t m = *n;
  mpfr_t mf, mx;
  (void)mpfr_init_set_ld(mf, 0.0L, MPFR_RNDN);
  (void)mpfr_init_set_ld(mx, 0.0L, MPFR_RNDN);
  for (size_t i = 0u; i < m; ++i) {
    (void)mpfr_set_float128(mx, x[i], MPFR_RNDN);
#ifdef PVN_NRM_SAFE
    (void)mpfr_hypot(mf, mf, mx, MPFR_RNDN);
#else /* !PVN_NRM_SAFE */
    (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
#endif /* ?PVN_NRM_SAFE */
  }
#ifndef PVN_NRM_SAFE
  (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
#endif /* !PVN_NRM_SAFE */
  const __float128 f = mpfr_get_float128(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}
#endif /* PVN_MPFR */

__float128 PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(const size_t *const n, const __float128 *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0q;
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
    return hypotq(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const __float128 fl = PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&nl, x);
  const __float128 fr = PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&nr, (x + nl));
  return hypotq(fl, fr);
}

__float128 PVN_FABI(pvn_crq_nrmf,PVN_CRQ_NRMF)(const size_t *const n, const __float128 *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(hcq:__float128:omp_out=hypotq(omp_out,omp_in)) initializer(omp_priv=0.0q)
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0q;
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
    return hypotq(x[0], x[1]);
  const size_t m = *n;
  /* the three Blue's accumulators, plus the accumulator for subnormal inputs */
  const __float128 tsml = scalbnq(1.0L, -8191);
  const __float128 tbig = scalbnq(1.0L,  8136);
  __float128 dnr = 0.0q, sml = 0.0q, med = 0.0q, big = 0.0q;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcq:dnr,sml,med,big)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const __float128 y = fabsq(x[i]);
    if (y > 0.0q) {
      if (y < FLT128_MIN)
        dnr = hypotq(dnr, scalbnq(y, 112));
      else if (y < tsml)
        sml = hypotq(sml, y);
      else if (y > tbig)
        big = hypotq(big, y);
      else /* med */
        med = hypotq(med, y);
    }
  }
  if (dnr > 0.0q)
    sml = hypotq(sml, scalbnq(dnr, -112));
  if (sml > 0.0q)
    med = hypotq(med, sml);
  if (med > 0.0q)
    big = hypotq(big, med);
  return big;
}

__float128 PVN_FABI(pvn_qnrm2,PVN_QNRM2)(const size_t *const n, const __float128 *const x)
{
#ifdef _OPENMP
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  const size_t mt = (size_t)omp_get_max_threads();
  __float128 p[mt];
  for (size_t i = 0u; i < mt; ++i)
    p[i] = 0.0q;
#pragma omp parallel default(none) shared(n,x,p)
  {
    const size_t nt = (size_t)omp_get_num_threads();
    const size_t tn = (size_t)omp_get_thread_num();
    lldiv_t qr = lldiv((long long)*n, (long long)nt);
    const size_t q = (size_t)(qr.quot);
    const size_t r = (size_t)(qr.rem);
    size_t m = q, o = (tn * q);
    if (r) {
      if (tn < r) {
        ++m;
        o += tn;
      }
      else
        o += r;
    }
    if (m)
      p[tn] = PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&m, (x + o));
  }
  return PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(n, x);
#endif /* ?_OPENMP */
}

__float128 PVN_FABI(pvn_ynrm2,PVN_YNRM2)(const size_t *const n, const __float128 *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_qnrm2,PVN_QNRM2)(&m, x);
}
#else /* !PVN_QUADMATH */
#ifdef PVN_MPFR
long double PVN_FABI(pvn_mpq_nrmf,PVN_MPQ_NRMF)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_mpx_nrmf,PVN_MPX_NRMF)(n, x);
}
#endif /* PVN_MPFR */
long double PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(n, x);
}
long double PVN_FABI(pvn_crq_nrmf,PVN_CRQ_NRMF)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_crx_nrmf,PVN_CRX_NRMF)(n, x);
}
long double PVN_FABI(pvn_qnrm2,PVN_QNRM2)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_xnrm2,PVN_XNRM2)(n, x);
}
long double PVN_FABI(pvn_ynrm2,PVN_YNRM2)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_wnrm2,PVN_WNRM2)(n, x);
}
#endif /* ?PVN_QUADMATH */

float PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0f;
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return __builtin_hypotf(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const float fl = PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(&nl, x);
  const float fr = PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(&nr, (x + nl));
  return __builtin_hypotf(fl, fr);
}

double PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return __builtin_hypot(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const double fl = PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(&nl, x);
  const double fr = PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(&nr, (x + nl));
  return __builtin_hypot(fl, fr);
}

long double PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(const size_t *const n, const long double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0L;
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return __builtin_hypotl(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const long double fl = PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(&nl, x);
  const long double fr = PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(&nr, (x + nl));
  return __builtin_hypotl(fl, fr);
}

#if (defined(__AVX__) && defined(__FMA__))
static __m128 rxs_nrmf(const size_t n, const float *const x)
{
  register const __m128 z = _mm_set1_ps(-0.0f);
  if (!n)
    return z;
  const size_t m = (n >> 2u);
  if (m == (size_t)1u) {
    register const __m128 f = _mm_load_ps(x);
    return _mm_andnot_ps(z, f);
  }
  if (m == (size_t)2u) {
    register const __m128 fl = _mm_load_ps(x);
    register const __m128 fd = _mm_load_ps(x + 8u);
    return pvn_v4s_hypot(fl, fd);
  }
  const size_t nl = ((n >> 1u) + (n & (size_t)1u));
  const size_t nr = (n - nl);
  register const __m128 fl = rxs_nrmf(nl, x);
  register const __m128 fr = rxs_nrmf(nr, (x + nl));
  return pvn_v4s_hypot(fl, fr);
}

float PVN_FABI(pvn_rxs_nrmf,PVN_RXS_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (__builtin_popcountll((long long)*n) != 1)
    return -1.0f;
  alignas(16) float f[4];
  _mm_store_ps(f, rxs_nrmf(*n, x));
  const size_t m = (size_t)4u;
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&m, f);
}

static __m128d rxd_nrmf(const size_t n, const double *const x)
{
  register const __m128d z = _mm_set1_pd(-0.0);
  if (!n)
    return z;
  const size_t m = (n >> 1u);
  if (m == (size_t)1u) {
    register const __m128d f = _mm_load_pd(x);
    return _mm_andnot_pd(z, f);
  }
  if (m == (size_t)2u) {
    register const __m128d fl = _mm_load_pd(x);
    register const __m128d fd = _mm_load_pd(x + 4u);
    return pvn_v2d_hypot(fl, fd);
  }
  const size_t nl = ((n >> 1u) + (n & (size_t)1u));
  const size_t nr = (n - nl);
  register const __m128d fl = rxd_nrmf(nl, x);
  register const __m128d fr = rxd_nrmf(nr, (x + nl));
  return pvn_v2d_hypot(fl, fr);
}

double PVN_FABI(pvn_rxd_nrmf,PVN_RXD_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (__builtin_popcountll((long long)*n) != 1)
    return -1.0;
  alignas(16) double f[2];
  _mm_store_pd(f, rxd_nrmf(*n, x));
  const size_t m = (size_t)2u;
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&m, f);
}

static __m256 rys_nrmf(const size_t n, const float *const x)
{
  register const __m256 z = _mm256_set1_ps(-0.0f);
  if (!n)
    return z;
  const size_t m = (n >> 3u);
  if (m == (size_t)1u) {
    register const __m256 f = _mm256_load_ps(x);
    return _mm256_andnot_ps(z, f);
  }
  if (m == (size_t)2u) {
    register const __m256 fl = _mm256_load_ps(x);
    register const __m256 fd = _mm256_load_ps(x + 8u);
    return pvn_v8s_hypot(fl, fd);
  }
  const size_t nl = ((n >> 1u) + (n & (size_t)1u));
  const size_t nr = (n - nl);
  register const __m256 fl = rys_nrmf(nl, x);
  register const __m256 fr = rys_nrmf(nr, (x + nl));
  return pvn_v8s_hypot(fl, fr);
}

float PVN_FABI(pvn_rys_nrmf,PVN_RYS_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (__builtin_popcountll((long long)*n) != 1)
    return -1.0f;
  alignas(32) float f[8];
  _mm256_store_ps(f, rys_nrmf(*n, x));
  const size_t m = (size_t)8u;
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&m, f);
}

static __m256d ryd_nrmf(const size_t n, const double *const x)
{
  register const __m256d z = _mm256_set1_pd(-0.0);
  if (!n)
    return z;
  const size_t m = (n >> 2u);
  if (m == (size_t)1u) {
    register const __m256d f = _mm256_load_pd(x);
    return _mm256_andnot_pd(z, f);
  }
  if (m == (size_t)2u) {
    register const __m256d fl = _mm256_load_pd(x);
    register const __m256d fd = _mm256_load_pd(x + 4u);
    return pvn_v4d_hypot(fl, fd);
  }
  const size_t nl = ((n >> 1u) + (n & (size_t)1u));
  const size_t nr = (n - nl);
  register const __m256d fl = ryd_nrmf(nl, x);
  register const __m256d fr = ryd_nrmf(nr, (x + nl));
  return pvn_v4d_hypot(fl, fr);
}

double PVN_FABI(pvn_ryd_nrmf,PVN_RYD_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (__builtin_popcountll((long long)*n) != 1)
    return -1.0;
  alignas(32) double f[4];
  _mm256_store_pd(f, ryd_nrmf(*n, x));
  const size_t m = (size_t)4u;
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&m, f);
}
#ifdef __AVX512F__
static __m512 rzs_nrmf(const size_t n, const float *const x)
{
  register const __m512 z = _mm512_set1_ps(-0.0f);
  if (!n)
    return z;
  const size_t m = (n >> 4u);
  if (m == (size_t)1u) {
    register const __m512 f = _mm512_load_ps(x);
    return _mm512_castsi512_ps(_mm512_andnot_epi32(_mm512_castps_si512(z), _mm512_castps_si512(f)));
  }
  if (m == (size_t)2u) {
    register const __m512 fl = _mm512_load_ps(x);
    register const __m512 fd = _mm512_load_ps(x + 16u);
    return pvn_v16s_hypot(fl, fd);
  }
  const size_t nl = ((n >> 1u) + (n & (size_t)1u));
  const size_t nr = (n - nl);
  register const __m512 fl = rzs_nrmf(nl, x);
  register const __m512 fr = rzs_nrmf(nr, (x + nl));
  return pvn_v16s_hypot(fl, fr);
}

float PVN_FABI(pvn_rzs_nrmf,PVN_RZS_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (__builtin_popcountll((long long)*n) != 1)
    return -1.0f;
  alignas(64) float f[16];
  _mm512_store_ps(f, rzs_nrmf(*n, x));
  const size_t m = (size_t)16u;
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&m, f);
}

static __m512d rzd_nrmf(const size_t n, const double *const x)
{
  register const __m512d z = _mm512_set1_pd(-0.0);
  if (!n)
    return z;
  const size_t m = (n >> 3u);
  if (m == (size_t)1u) {
    register const __m512d f = _mm512_load_pd(x);
    return _mm512_castsi512_pd(_mm512_andnot_epi64(_mm512_castpd_si512(z), _mm512_castpd_si512(f)));
  }
  if (m == (size_t)2u) {
    register const __m512d fl = _mm512_load_pd(x);
    register const __m512d fd = _mm512_load_pd(x + 8u);
    return pvn_v8d_hypot(fl, fd);
  }
  const size_t nl = ((n >> 1u) + (n & (size_t)1u));
  const size_t nr = (n - nl);
  register const __m512d fl = rzd_nrmf(nl, x);
  register const __m512d fr = rzd_nrmf(nr, (x + nl));
  return pvn_v8d_hypot(fl, fr);
}

double PVN_FABI(pvn_rzd_nrmf,PVN_RZD_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (__builtin_popcountll((long long)*n) != 1)
    return -1.0;
  alignas(64) double f[8];
  _mm512_store_pd(f, rzd_nrmf(*n, x));
  const size_t m = (size_t)8u;
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&m, f);
}
#endif /* __AVX512F__ */
#endif /* __AVX__ && __FMA__ */
#endif /* ?PVN_TEST */

#include "pvn.h"

#ifdef PVN_TEST
#if (defined(PVN_LAPACK) && defined(__AVX__) && defined(__FMA__))
/* this assumes the Fortran integers are four-byte long, but only for this tester */
extern float PVN_FABI(slaran,SLARAN)(int *const iseed);
extern float PVN_FABI(slarnd,SLARND)(const int *const idist, int *const iseed);
extern double PVN_FABI(dlaran,DLARAN)(int *const iseed);
extern double PVN_FABI(dlarnd,DLARND)(const int *const idist, int *const iseed);
static double frelerr(const double e, const double f)
{
  return ((e == 0.0) ? -0.0 : (__builtin_fabs(e - f) / scalbn(__builtin_fabs(e), -24)));
}
static double erelerr(const double e, const double f)
{
  return ((e == 0.0) ? -0.0 : (double)(__builtin_fabsl((long double)e - (long double)f) / scalbnl(__builtin_fabsl((long double)e), -53)));
}
int main(int argc, char *argv[])
{
  if ((argc < 2) || (argc > 4)) {
    (void)fprintf(stderr, "%s n [seed [exact]]\n", *argv);
    return 1;
  }
  const long long n_ = atoll(argv[1u]);
  if (!n_)
    return 2;
  union {
    unsigned long long l;
    unsigned short s[4u];
  } ls;
  int idist = ((n_ < 0ll) ? 3 : 1);
  if (argc > 2) {
    const long long s_ = atoll(argv[2u]);
    if (s_ < 0ll) {
      ls.l = (unsigned long long)-s_;
      idist = -idist;
    }
    else
      ls.l = (unsigned long long)s_;
    if (!(ls.s[2u] & UINT16_C(16)))
        return 3;
    if (ls.s[3u]) /* longer than 48 bits */
      return 4;
  }
  else {
    const int u = PVN_FABI(pvn_ran_open,PVN_RAN_OPEN)();
    if (u < 0)
      return 5;
    while (!PVN_FABI(pvn_ran_16,PVN_RAN_16)(&u, ls.s)) /* loop */;
    while (!PVN_FABI(pvn_ran_16,PVN_RAN_16)(&u, ls.s + 1)) /* loop */;
    while (!PVN_FABI(pvn_ran_16,PVN_RAN_16)(&u, ls.s + 2) || !(ls.s[2u] & UINT16_C(16))) /* loop */;
    ls.s[3u] = UINT16_C(0);
    if (PVN_FABI(pvn_ran_close,PVN_RAN_CLOSE)(&u))
      return 6;
    (void)fprintf(stderr, "SEED = %15llu\n", ls.l);
  }
  const size_t n = (size_t)((n_ < 0ll) ? -n_ : n_);
  void *const x = aligned_alloc((size_t)64u, (n * ((idist < 0) ? sizeof(float) : sizeof(double))));
  if (!x)
    return 7;
  int iseed[4u] = { (int)(ls.l & 0xFFFull), (int)((ls.l >> 12u) & 0xFFFull), (int)((ls.l >> 24u) & 0xFFFull), (int)((ls.l >> 36u) & 0xFFFull) };
  const int adist = abs(idist);
  if (idist == -3) {
    for (size_t i = (size_t)0u; i < n; ++i)
      ((float*)x)[i] = PVN_FABI(slarnd,SLARND)(&adist, iseed);
  }
  else if (idist == -1) {
    for (size_t i = (size_t)0u; i < n; ++i)
      ((float*)x)[i] = PVN_FABI(slaran,SLARAN)(iseed);
  }
  else if (idist == 1) {
    for (size_t i = (size_t)0u; i < n; ++i)
      ((double*)x)[i] = PVN_FABI(dlaran,DLARAN)(iseed);
  }
  else if (idist == 3) {
    for (size_t i = (size_t)0u; i < n; ++i)
      ((double*)x)[i] = PVN_FABI(dlarnd,DLARND)(&adist, iseed);
  }
  else
    return 8;
  long long t = 0ll;
  double e = ((argc > 3) ? atof(argv[3u]) : 0.0), f = 0.0;
  double p = 0.0;
  if (idist < 0)
    *(float*)&p = 2.0f;
  else /* double */
    p = 2.0;
#ifndef PVN_CILK
#ifdef PVN_MPFR
  if (argc <= 3) {
    mpfr_rnd_t rnd = MPFR_RNDN;
    mpfr_prec_t prec = 2048l;
    mpfr_exp_t emin = __MPFR_EXP_INVALID, emax = __MPFR_EXP_INVALID;
    if (PVN_FABI(pvn_mpfr_start,PVN_MPFR_START)(&rnd, &prec, &emin, &emax))
      return 9;
    (void)printf((idist < 0) ? "pvn_mps_nrmf=" : "pvn_mpd_nrmf=");
    (void)fflush(stdout);
    t = pvn_time_mono_ns();
    e = ((idist < 0) ? PVN_FABI(pvn_mps_nrmp,PVN_MPS_NRMP)((const float*)&p, &n, (const float*)x) : PVN_FABI(pvn_mpd_nrmp,PVN_MPD_NRMP)(&p, &n, (const double*)x));
    t = pvn_time_mono_ns() - t;
    (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", e, 0.0, t);
  }
#endif /* PVN_MPFR */
  (void)printf((idist < 0) ? "pvn_las_nrmf=" : "pvn_lad_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = ((idist < 0) ? PVN_FABI(pvn_las_nrmf,PVN_LAS_NRMF)(&n, (const float*)x) : PVN_FABI(pvn_lad_nrmf,PVN_LAD_NRMF)(&n, (const double*)x));
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, ((idist < 0) ? frelerr(e, f) : erelerr(e, f)), t);
  (void)printf((idist < 0) ? "pvn_snrm2[s]=" : "pvn_dnrm2[s]=");
#else /* PVN_CILK */
  (void)printf((idist < 0) ? "pvn_snrm2[p]=" : "pvn_dnrm2[p]=");
#endif /* ?PVN_CILK */
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = ((idist < 0) ? PVN_FABI(pvn_snrm2,PVN_SNRM2)(&n, (const float*)x) : PVN_FABI(pvn_dnrm2,PVN_DNRM2)(&n, (const double*)x));
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, ((idist < 0) ? frelerr(e, f) : erelerr(e, f)), t);
#ifdef PVN_NRM_SAFE
  (void)printf((idist < 0) ? "pvn_rfs_nrmf=" : "pvn_rfd_nrmf=");
#else /* !PVN_NRM_SAFE */
  (void)printf((idist < 0) ? "pvn_res_nrmf=" : "pvn_red_nrmf=");
#endif /* ?PVN_NRM_SAFE */
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
#ifdef PVN_NRM_SAFE
  f = ((idist < 0) ? PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(&n, (const float*)x) : PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(&n, (const double*)x));
#else /* !PVN_NRM_SAFE */
  f = ((idist < 0) ? PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&n, (const float*)x) : PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&n, (const double*)x));
#endif /* ?PVN_NRM_SAFE */
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, ((idist < 0) ? frelerr(e, f) : erelerr(e, f)), t);
  (void)printf((idist < 0) ? "pvn_rhs_nrmf=" : "pvn_rhd_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = ((idist < 0) ? PVN_FABI(pvn_rhs_nrmf,PVN_RHS_NRMF)(&n, (const float*)x) : PVN_FABI(pvn_rhd_nrmf,PVN_RHD_NRMF)(&n, (const double*)x));
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, ((idist < 0) ? frelerr(e, f) : erelerr(e, f)), t);
  (void)printf((idist < 0) ? "pvn_rxs_nrmf=" : "pvn_rxd_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = ((idist < 0) ? PVN_FABI(pvn_rxs_nrmf,PVN_RXS_NRMF)(&n, (const float*)x) : PVN_FABI(pvn_rxd_nrmf,PVN_RXD_NRMF)(&n, (const double*)x));
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, ((idist < 0) ? frelerr(e, f) : erelerr(e, f)), t);
  (void)printf((idist < 0) ? "pvn_rys_nrmf=" : "pvn_ryd_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = ((idist < 0) ? PVN_FABI(pvn_rys_nrmf,PVN_RYS_NRMF)(&n, (const float*)x) : PVN_FABI(pvn_ryd_nrmf,PVN_RYD_NRMF)(&n, (const double*)x));
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, ((idist < 0) ? frelerr(e, f) : erelerr(e, f)), t);
#ifdef __AVX512F__
  (void)printf((idist < 0) ? "pvn_rzs_nrmf=" : "pvn_rzd_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = ((idist < 0) ? PVN_FABI(pvn_rzs_nrmf,PVN_RZS_NRMF)(&n, (const float*)x) : PVN_FABI(pvn_rzd_nrmf,PVN_RZD_NRMF)(&n, (const double*)x));
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, ((idist < 0) ? frelerr(e, f) : erelerr(e, f)), t);
#endif /* __AVX512F__ */
#ifndef PVN_CILK
  (void)printf((idist < 0) ? "pvn_crs_nrmf=" : "pvn_crd_nrmf=");
  (void)fflush(stdout);
  t = pvn_time_mono_ns();
  f = ((idist < 0) ? PVN_FABI(pvn_crs_nrmf,PVN_CRS_NRMF)(&n, (const float*)x) : PVN_FABI(pvn_crd_nrmf,PVN_CRD_NRMF)(&n, (const double*)x));
  t = pvn_time_mono_ns() - t;
  (void)printf("%# .17e relerr/ε %# .17e in %21lld ns\n", f, ((idist < 0) ? frelerr(e, f) : erelerr(e, f)), t);
#ifdef PVN_MPFR
  if (argc <= 3)
    (void)PVN_FABI(pvn_mpfr_stop,PVN_MPFR_STOP)();
#endif /* PVN_MPFR */
#endif /* !PVN_CILK */
  free(x);
  return 0;
}
#else /* !PVN_LAPACK || !__AVX__ || !__FMA__ */
int main(/* int argc, char *argv[] */)
{
  const double x[4][2] = {{-1.0, 1.0}, { 1.0,-1.0}, { 2.0, 2.0}, {-2.0,-2.0}};
  const size_t n = (size_t)4u;
  /* expected output: 4.472136 */
  (void)printf("%#F\n", PVN_FABI(pvn_znrm2,PVN_ZNRM2)(&n, &(x[0][0])));
  return EXIT_SUCCESS;
}
#endif /* PVN_LAPACK && __AVX__ && __FMA__ */
#else /* !PVN_TEST */
#ifdef PVN_MPFR
float PVN_FABI(pvn_mps_nrmp,PVN_MPS_NRMP)(const float *const p, const size_t *const n, const float *const x)
{
  if (!p || !(*p > 0.0f))
    return -1.0f;
  if (!n)
    return -2.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -3.0f;
  const size_t m = *n;
  float f = 0.0f;
  mpfr_t mf, mx;
  (void)mpfr_init_set_d(mf, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(mx, 0.0, MPFR_RNDN);
  if (*p == 1.0f) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_flt(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_add(mf, mf, mx, MPFR_RNDN);
    }
  }
  else if (*p == 2.0f) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_flt(mx, x[i], MPFR_RNDN);
      (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
    }
    (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
  }
  else if (isinf(*p)) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_flt(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_max(mf, mf, mx, MPFR_RNDN);
    }
  }
  else {
    mpfr_t mp;
    (void)mpfr_init_set_d(mf, (double)*p, MPFR_RNDN);
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_flt(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_pow(mx, mx, mp, MPFR_RNDN);
      (void)mpfr_add(mf, mf, mx, MPFR_RNDN);
    }
    (void)mpfr_d_div(mp, 1.0, mp, MPFR_RNDN);
    (void)mpfr_pow(mf, mf, mp, MPFR_RNDN);
    mpfr_clear(mp);
  }
  f = mpfr_get_flt(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}

double PVN_FABI(pvn_mpd_nrmp,PVN_MPD_NRMP)(const double *const p, const size_t *const n, const double *const x)
{
  if (!p || !(*p > 0.0))
    return -1.0;
  if (!n)
    return -2.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -3.0;
  const size_t m = *n;
  double f = 0.0;
  mpfr_t mf, mx;
  (void)mpfr_init_set_d(mf, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(mx, 0.0, MPFR_RNDN);
  if (*p == 1.0) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_d(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_add(mf, mf, mx, MPFR_RNDN);
    }
  }
  else if (*p == 2.0) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_d(mx, x[i], MPFR_RNDN);
      (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
    }
    (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
  }
  else if (isinf(*p)) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_d(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_max(mf, mf, mx, MPFR_RNDN);
    }
  }
  else {
    mpfr_t mp;
    (void)mpfr_init_set_d(mf, *p, MPFR_RNDN);
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_d(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_pow(mx, mx, mp, MPFR_RNDN);
      (void)mpfr_add(mf, mf, mx, MPFR_RNDN);
    }
    (void)mpfr_d_div(mp, 1.0, mp, MPFR_RNDN);
    (void)mpfr_pow(mf, mf, mp, MPFR_RNDN);
    mpfr_clear(mp);
  }
  f = mpfr_get_d(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}

long double PVN_FABI(pvn_mpx_nrmp,PVN_MPX_NRMP)(const long double *const p, const size_t *const n, const long double *const x)
{
  if (!p || !(*p > 0.0L))
    return -1.0L;
  if (!n)
    return -2.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -3.0L;
  const size_t m = *n;
  long double f = 0.0L;
  mpfr_t mf, mx;
  (void)mpfr_init_set_ld(mf, 0.0L, MPFR_RNDN);
  (void)mpfr_init_set_ld(mx, 0.0L, MPFR_RNDN);
  if (*p == 1.0L) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_ld(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_add(mf, mf, mx, MPFR_RNDN);
    }
  }
  else if (*p == 2.0L) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_ld(mx, x[i], MPFR_RNDN);
      (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
    }
    (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
  }
  else if (isinf(*p)) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_ld(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_max(mf, mf, mx, MPFR_RNDN);
    }
  }
  else {
    mpfr_t mp;
    (void)mpfr_init_set_ld(mf, *p, MPFR_RNDN);
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_ld(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_pow(mx, mx, mp, MPFR_RNDN);
      (void)mpfr_add(mf, mf, mx, MPFR_RNDN);
    }
    (void)mpfr_d_div(mp, 1.0, mp, MPFR_RNDN);
    (void)mpfr_pow(mf, mf, mp, MPFR_RNDN);
    mpfr_clear(mp);
  }
  f = mpfr_get_ld(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}
#endif /* PVN_MPFR */

#ifdef PVN_LAPACK
/* even if the Fortran integers are four-byte long, this should work on little endian machines */
extern float PVN_FABI(snrm2,SNRM2)(const size_t *const n, const float *const x, const int64_t *const incx);
extern double PVN_FABI(dnrm2,DNRM2)(const size_t *const n, const double *const x, const int64_t *const incx);

float PVN_FABI(pvn_las_nrmf,PVN_LAS_NRMF)(const size_t *const n, const float *const x)
{
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
  const int64_t incx = INT64_C(1);
  return PVN_FABI(snrm2,SNRM2)(n, x, &incx);
}

double PVN_FABI(pvn_lad_nrmf,PVN_LAD_NRMF)(const size_t *const n, const double *const x)
{
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
  const int64_t incx = INT64_C(1);
  return PVN_FABI(dnrm2,DNRM2)(n, x, &incx);
}
#endif /* PVN_LAPACK */

float PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(const size_t *const n, const float *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return hypotf(x[0u], x[1u]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  float fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&nr, (x + nl));
  }
  return hypotf(fl, fr);
}

double PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(const size_t *const n, const double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return hypot(x[0u], x[1u]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&nr, (x + nl));
  }
  return hypot(fl, fr);
}

long double PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(const size_t *const n, const long double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -2.0L;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return hypotl(x[0u], x[1u]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  long double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&nr, (x + nl));
  }
  return hypotl(fl, fr);
}

float PVN_FABI(pvn_res_nrm1,PVN_RES_NRM1)(const size_t *const n, const float *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return (__builtin_fabsf(x[0u]) + __builtin_fabsf(x[1u]));
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  float fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_res_nrm1,PVN_RES_NRM1)(&nl, x);
    fr = PVN_FABI(pvn_res_nrm1,PVN_RES_NRM1)(&nr, (x + nl));
  }
  return (fl + fr);
}

double PVN_FABI(pvn_red_nrm1,PVN_RED_NRM1)(const size_t *const n, const double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return (__builtin_fabs(x[0u]) + __builtin_fabs(x[1u]));
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_red_nrm1,PVN_RED_NRM1)(&nl, x);
    fr = PVN_FABI(pvn_red_nrm1,PVN_RED_NRM1)(&nr, (x + nl));
  }
  return (fl + fr);
}

long double PVN_FABI(pvn_rex_nrm1,PVN_REX_NRM1)(const size_t *const n, const long double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -2.0L;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return (__builtin_fabsl(x[0u]) + __builtin_fabsl(x[1u]));
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  long double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rex_nrm1,PVN_REX_NRM1)(&nl, x);
    fr = PVN_FABI(pvn_rex_nrm1,PVN_REX_NRM1)(&nr, (x + nl));
  }
  return (fl + fr);
}

float PVN_FABI(pvn_res_nrmI,PVN_RES_NRMI)(const size_t *const n, const float *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return __builtin_fmaxf(__builtin_fabsf(x[0u]), __builtin_fabsf(x[1u]));
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  float fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_res_nrmI,PVN_RES_NRMI)(&nl, x);
    fr = PVN_FABI(pvn_res_nrmI,PVN_RES_NRMI)(&nr, (x + nl));
  }
  return __builtin_fmaxf(fl, fr);
}

double PVN_FABI(pvn_red_nrmI,PVN_RED_NRMI)(const size_t *const n, const double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return __builtin_fmax(__builtin_fabs(x[0u]), __builtin_fabs(x[1u]));
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_red_nrmI,PVN_RED_NRMI)(&nl, x);
    fr = PVN_FABI(pvn_red_nrmI,PVN_RED_NRMI)(&nr, (x + nl));
  }
  return __builtin_fmax(fl, fr);
}

long double PVN_FABI(pvn_rex_nrmI,PVN_REX_NRMI)(const size_t *const n, const long double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -2.0L;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return __builtin_fmaxl(__builtin_fabsl(x[0u]), __builtin_fabsl(x[1u]));
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  long double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rex_nrmI,PVN_REX_NRMI)(&nl, x);
    fr = PVN_FABI(pvn_rex_nrmI,PVN_REX_NRMI)(&nr, (x + nl));
  }
  return __builtin_fmaxl(fl, fr);
}

float PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(const size_t *const n, const float *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return __builtin_hypotf(x[0u], x[1u]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  float fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(&nr, (x + nl));
  }
  return __builtin_hypotf(fl, fr);
}

double PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(const size_t *const n, const double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return __builtin_hypot(x[0u], x[1u]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(&nr, (x + nl));
  }
  return __builtin_hypot(fl, fr);
}

long double PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(const size_t *const n, const long double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -2.0L;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return __builtin_hypotl(x[0u], x[1u]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  long double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(&nr, (x + nl));
  }
  return __builtin_hypotl(fl, fr);
}

float PVN_FABI(pvn_rhs_nrmf,PVN_RHS_NRMF)(const size_t *const n, const float *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return pvn_v1s_hypot(x[0u], x[1u]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  float fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rhs_nrmf,PVN_RHS_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_rhs_nrmf,PVN_RHS_NRMF)(&nr, (x + nl));
  }
  return pvn_v1s_hypot(fl, fr);
}

double PVN_FABI(pvn_rhd_nrmf,PVN_RHD_NRMF)(const size_t *const n, const double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return pvn_v1d_hypot(x[0u], x[1u]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rhd_nrmf,PVN_RHD_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_rhd_nrmf,PVN_RHD_NRMF)(&nr, (x + nl));
  }
  return pvn_v1d_hypot(fl, fr);
}

long double PVN_FABI(pvn_rhx_nrmf,PVN_RHX_NRMF)(const size_t *const n, const long double *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -2.0L;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return pvn_v1x_hypot(x[0u], x[1u]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  long double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rhx_nrmf,PVN_RHX_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_rhx_nrmf,PVN_RHX_NRMF)(&nr, (x + nl));
  }
  return pvn_v1x_hypot(fl, fr);
}

float PVN_FABI(pvn_crs_nrmf,PVN_CRS_NRMF)(const size_t *const n, const float *const x)
{
#ifdef _OPENMP
#pragma omp declare reduction(hcf:float:omp_out=hypotf(omp_out,omp_in)) initializer(omp_priv=0.0f)
#endif /* _OPENMP */
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return hypotf(x[0u], x[1u]);
  const size_t m = *n;
  const float tsml = scalbnf(1.0f, -63);
  const float tbig = scalbnf(1.0f,  52);
  /* the three Blue's accumulators as in SNRM2 */
  float sml = 0.0f, med = 0.0f, big = 0.0f;
#ifndef PVN_LAPACK
  /* the accumulator for subnormal inputs */
  float dnr = 0.0f;
#endif /* !PVN_LAPACK */
#ifdef _OPENMP
#ifdef PVN_LAPACK
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcf:sml,med,big)
#else /* !PVN_LAPACK */
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcf:dnr,sml,med,big)
#endif /* ?PVN_LAPACK */
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const float y = __builtin_fabsf(x[i]);
    if (y > 0.0f) {
#ifdef PVN_LAPACK
      if (y < tsml)
#else /* !PVN_LAPACK */
      if (y < FLT_MIN)
        dnr = hypotf(dnr, scalbnf(y, 23));
      else if (y < tsml)
#endif /* PVN_LAPACK */
        sml = hypotf(sml, y);
      else if (y > tbig)
        big = hypotf(big, y);
      else /* med */
        med = hypotf(med, y);
    }
  }
#ifndef PVN_LAPACK
  if (dnr > 0.0f)
    sml = hypotf(sml, scalbnf(dnr, -23));
#endif /* !PVN_LAPACK */
  if (sml > 0.0f)
    med = hypotf(med, sml);
  if (med > 0.0f)
    big = hypotf(big, med);
  return big;
}

double PVN_FABI(pvn_crd_nrmf,PVN_CRD_NRMF)(const size_t *const n, const double *const x)
{
#ifdef _OPENMP
#pragma omp declare reduction(hcd:double:omp_out=hypot(omp_out,omp_in)) initializer(omp_priv=0.0)
#endif /* _OPENMP */
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return hypot(x[0u], x[1u]);
  const size_t m = *n;
  const double tsml = scalbn(1.0, -511);
  const double tbig = scalbn(1.0,  486);
  /* the three Blue's accumulators as in DNRM2 */
  double sml = 0.0, med = 0.0, big = 0.0;
#ifndef PVN_LAPACK
  /* the accumulator for subnormal inputs */
  double dnr = 0.0;
#endif /* !PVN_LAPACK */
#ifdef _OPENMP
#ifdef PVN_LAPACK
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcd:sml,med,big)
#else /* !PVN_LAPACK */
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcd:dnr,sml,med,big)
#endif /* ?PVN_LAPACK */
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const double y = __builtin_fabs(x[i]);
    if (y > 0.0) {
#ifdef PVN_LAPACK
      if (y < tsml)
#else /* !PVN_LAPACK */
      if (y < DBL_MIN)
        dnr = hypot(dnr, scalbn(y, 52));
      else if (y < tsml)
#endif /* ?PVN_LAPACK */
        sml = hypot(sml, y);
      else if (y > tbig)
        big = hypot(big, y);
      else /* med */
        med = hypot(med, y);
    }
  }
#ifndef PVN_LAPACK
  if (dnr > 0.0)
    sml = hypot(sml, scalbn(dnr, -52));
#endif /* !PVN_LAPACK */
  if (sml > 0.0)
    med = hypot(med, sml);
  if (med > 0.0)
    big = hypot(big, med);
  return big;
}

long double PVN_FABI(pvn_crx_nrmf,PVN_CRX_NRMF)(const size_t *const n, const long double *const x)
{
#ifdef _OPENMP
#pragma omp declare reduction(hcx:long double:omp_out=hypotl(omp_out,omp_in)) initializer(omp_priv=0.0L)
#endif /* _OPENMP */
  if (!n)
    return -1.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -2.0L;
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return hypotl(x[0u], x[1u]);
  const size_t m = *n;
  const long double tsml = scalbnl(1.0L, -8191);
  const long double tbig = scalbnl(1.0L,  8160);
  /* the three Blue's accumulators */
  long double sml = 0.0L, med = 0.0L, big = 0.0L;
#ifndef PVN_LAPACK
  /* the accumulator for subnormal inputs */
  long double dnr = 0.0L;
#endif /* !PVN_LAPACK */
#ifdef _OPENMP
#ifdef PVN_LAPACK
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcx:sml,med,big)
#else /* !PVN_LAPACK */
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcx:dnr,sml,med,big)
#endif /* ?PVN_LAPACK */
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const long double y = __builtin_fabsl(x[i]);
    if (y > 0.0L) {
#ifdef PVN_LAPACK
      if (y < tsml)
#else /* !PVN_LAPACK */
      if (y < LDBL_MIN)
        dnr = hypotl(dnr, scalbnl(y, 63));
      else if (y < tsml)
#endif /* ?PVN_LAPACK */
        sml = hypotl(sml, y);
      else if (y > tbig)
        big = hypotl(big, y);
      else /* med */
        med = hypotl(med, y);
    }
  }
#ifndef PVN_LAPACK
  if (dnr > 0.0L)
    sml = hypotl(sml, scalbnl(dnr, -63));
#endif /* !PVN_LAPACK */
  if (sml > 0.0L)
    med = hypotl(med, sml);
  if (med > 0.0L)
    big = hypotl(big, med);
  return big;
}

float PVN_FABI(pvn_snrm2,PVN_SNRM2)(const size_t *const n, const float *const x)
{
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
#ifdef _OPENMP
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
#ifdef PVN_NRM_SAFE
      p[tn] = PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&m, (x + o));
#else /* !PVN_NRM_SAFE */
      p[tn] = PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(&m, (x + o));
#endif /* ?PVN_NRM_SAFE */
  }
#ifdef PVN_NRM_SAFE
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&mt, p);
#else /* !PVN_NRM_SAFE */
  return PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(&mt, p);
#endif /* ?PVN_NRM_SAFE */
#else /* !_OPENMP */
#ifdef PVN_NRM_SAFE
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(n, x);
#else /* !PVN_NRM_SAFE */
  return PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(n, x);
#endif /* ?PVN_NRM_SAFE */
#endif /* _OPENMP */
}

float PVN_FABI(pvn_cnrm2,PVN_CNRM2)(const size_t *const n, const float *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_snrm2,PVN_SNRM2)(&m, x);
}

double PVN_FABI(pvn_dnrm2,PVN_DNRM2)(const size_t *const n, const double *const x)
{
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
#ifdef _OPENMP
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
#ifdef PVN_NRM_SAFE
      p[tn] = PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&m, (x + o));
#else /* !PVN_NRM_SAFE */
      p[tn] = PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(&m, (x + o));
#endif /* ?PVN_NRM_SAFE */
  }
#ifdef PVN_NRM_SAFE
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&mt, p);
#else /* !PVN_NRM_SAFE */
  return PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(&mt, p);
#endif /* ?PVN_NRM_SAFE */
#else /* !_OPENMP */
#ifdef PVN_NRM_SAFE
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(n, x);
#else /* !PVN_NRM_SAFE */
  return PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(n, x);
#endif /* ?PVN_NRM_SAFE */
#endif /* ?_OPENMP */
}

double PVN_FABI(pvn_znrm2,PVN_ZNRM2)(const size_t *const n, const double *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_dnrm2,PVN_DNRM2)(&m, x);
}

long double PVN_FABI(pvn_xnrm2,PVN_XNRM2)(const size_t *const n, const long double *const x)
{
  if (!n)
    return -1.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -2.0L;
#ifdef _OPENMP
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
#ifdef PVN_NRM_SAFE
      p[tn] = PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&m, (x + o));
#else /* !PVN_NRM_SAFE */
      p[tn] = PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(&m, (x + o));
#endif /* ?PVN_NRM_SAFE */
  }
#ifdef PVN_NRM_SAFE
  return PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&mt, p);
#else /* !PVN_NRM_SAFE */
  return PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(&mt, p);
#endif /* ?PVN_NRM_SAFE */
#else /* !_OPENMP */
#ifdef PVN_NRM_SAFE
  return PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(n, x);
#else /* !PVN_NRM_SAFE */
  return PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(n, x);
#endif /* ?PVN_NRM_SAFE */
#endif /* ?_OPENMP */
}

long double PVN_FABI(pvn_wnrm2,PVN_WNRM2)(const size_t *const n, const long double *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_xnrm2,PVN_XNRM2)(&m, x);
}

float PVN_FABI(pvn_snrm1,PVN_SNRM1)(const size_t *const n, const float *const x)
{
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
#ifdef _OPENMP
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
      p[tn] = PVN_FABI(pvn_res_nrm1,PVN_RES_NRM1)(&m, (x + o));
  }
  return PVN_FABI(pvn_res_nrm1,PVN_RES_NRM1)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_res_nrm1,PVN_RES_NRM1)(n, x);
#endif /* _OPENMP */
}

double PVN_FABI(pvn_dnrm1,PVN_DNRM1)(const size_t *const n, const double *const x)
{
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
#ifdef _OPENMP
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
      p[tn] = PVN_FABI(pvn_red_nrm1,PVN_RED_NRM1)(&m, (x + o));
  }
  return PVN_FABI(pvn_red_nrm1,PVN_RED_NRM1)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_red_nrm1,PVN_RED_NRM1)(n, x);
#endif /* ?_OPENMP */
}

long double PVN_FABI(pvn_xnrm1,PVN_XNRM1)(const size_t *const n, const long double *const x)
{
  if (!n)
    return -1.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -2.0L;
#ifdef _OPENMP
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
      p[tn] = PVN_FABI(pvn_rex_nrm1,PVN_REX_NRM1)(&m, (x + o));
  }
  return PVN_FABI(pvn_rex_nrm1,PVN_REX_NRM1)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_rex_nrm1,PVN_REX_NRM1)(n, x);
#endif /* ?_OPENMP */
}

float PVN_FABI(pvn_snrmI,PVN_SNRMI)(const size_t *const n, const float *const x)
{
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
#ifdef _OPENMP
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
      p[tn] = PVN_FABI(pvn_res_nrmI,PVN_RES_NRMI)(&m, (x + o));
  }
  return PVN_FABI(pvn_res_nrmI,PVN_RES_NRMI)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_res_nrmI,PVN_RES_NRMI)(n, x);
#endif /* _OPENMP */
}

double PVN_FABI(pvn_dnrmI,PVN_DNRMI)(const size_t *const n, const double *const x)
{
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
#ifdef _OPENMP
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
      p[tn] = PVN_FABI(pvn_red_nrmI,PVN_RED_NRMI)(&m, (x + o));
  }
  return PVN_FABI(pvn_red_nrmI,PVN_RED_NRMI)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_red_nrmI,PVN_RED_NRMI)(n, x);
#endif /* ?_OPENMP */
}

long double PVN_FABI(pvn_xnrmI,PVN_XNRMI)(const size_t *const n, const long double *const x)
{
  if (!n)
    return -1.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -2.0L;
#ifdef _OPENMP
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
      p[tn] = PVN_FABI(pvn_rex_nrmI,PVN_REX_NRMI)(&m, (x + o));
  }
  return PVN_FABI(pvn_rex_nrmI,PVN_REX_NRMI)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_rex_nrmI,PVN_REX_NRMI)(n, x);
#endif /* ?_OPENMP */
}

float PVN_FABI(pvn_snrmp,PVN_SNRMP)(const float *const p, const size_t *const n, const float *const x)
{
#ifndef NDEBUG
  if (!p || !(*p > 0.0f))
    return -1.0f;
  if (!n)
    return -2.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -3.0f;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return pvn_v1s_lp(*p, x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  float fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_snrmp,PVN_SNRMP)(p, &nl, x);
    fr = PVN_FABI(pvn_snrmp,PVN_SNRMP)(p, &nr, (x + nl));
  }
  return pvn_v1s_lp(*p, fl, fr);
}

double PVN_FABI(pvn_dnrmp,PVN_DNRMP)(const double *const p, const size_t *const n, const double *const x)
{
#ifndef NDEBUG
  if (!p || !(*p > 0.0))
    return -1.0;
  if (!n)
    return -2.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -3.0;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return pvn_v1d_lp(*p, x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_dnrmp,PVN_DNRMP)(p, &nl, x);
    fr = PVN_FABI(pvn_dnrmp,PVN_DNRMP)(p, &nr, (x + nl));
  }
  return pvn_v1d_lp(*p, fl, fr);
}

long double PVN_FABI(pvn_xnrmp,PVN_XNRMP)(const long double *const p, const size_t *const n, const long double *const x)
{
#ifndef NDEBUG
  if (!p || !(*p > 0.0L))
    return -1.0L;
  if (!n)
    return -2.0L;
  if (!*n)
    return -0.0L;
  if (!x)
    return -3.0L;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return pvn_v1x_lp(*p, x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  long double fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_xnrmp,PVN_XNRMP)(p, &nl, x);
    fr = PVN_FABI(pvn_xnrmp,PVN_XNRMP)(p, &nr, (x + nl));
  }
  return pvn_v1x_lp(*p, fl, fr);
}

#ifdef PVN_QUADMATH
#ifdef PVN_MPFR
__float128 PVN_FABI(pvn_mpq_nrmp,PVN_MPQ_NRMP)(const __float128 *const p, const size_t *const n, const __float128 *const x)
{
  if (!p || !(*p > 0.0q))
    return -1.0q;
  if (!n)
    return -2.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -3.0q;
  const size_t m = *n;
  __float128 f = 0.0q;
  mpfr_t mf, mx;
  (void)mpfr_init_set_ld(mf, 0.0L, MPFR_RNDN);
  (void)mpfr_init_set_ld(mx, 0.0L, MPFR_RNDN);
  if (*p == 1.0q) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_float128(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_add(mf, mf, mx, MPFR_RNDN);
    }
  }
  else if (*p == 2.0q) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_float128(mx, x[i], MPFR_RNDN);
      (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
    }
    (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
  }
  else if (!isfiniteq(*p)) {
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_float128(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_max(mf, mf, mx, MPFR_RNDN);
    }
  }
  else {
    mpfr_t mp;
    (void)mpfr_init_set_ld(mf, (long double)*p, MPFR_RNDN);
    for (size_t i = 0u; i < m; ++i) {
      (void)mpfr_set_float128(mx, x[i], MPFR_RNDN);
      (void)mpfr_abs(mx, mx, MPFR_RNDN);
      (void)mpfr_pow(mx, mx, mp, MPFR_RNDN);
      (void)mpfr_add(mf, mf, mx, MPFR_RNDN);
    }
    (void)mpfr_d_div(mp, 1.0, mp, MPFR_RNDN);
    (void)mpfr_pow(mf, mf, mp, MPFR_RNDN);
    mpfr_clear(mp);
  }
  f = mpfr_get_float128(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}
#endif /* PVN_MPFR */

__float128 PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(const size_t *const n, const __float128 *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -2.0q;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
    return hypotq(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  __float128 fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&nr, (x + nl));
  }
  return hypotq(fl, fr);
}

__float128 PVN_FABI(pvn_req_nrm1,PVN_REQ_NRM1)(const size_t *const n, const __float128 *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -2.0q;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
    return (fabsq(x[0]) + fabsq(x[1]));
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  __float128 fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_req_nrm1,PVN_REQ_NRM1)(&nl, x);
    fr = PVN_FABI(pvn_req_nrm1,PVN_REQ_NRM1)(&nr, (x + nl));
  }
  return (fl + fr);
}

__float128 PVN_FABI(pvn_req_nrmI,PVN_REQ_NRMI)(const size_t *const n, const __float128 *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -2.0q;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
    return fmaxq(fabsq(x[0]), fabsq(x[1]));
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  __float128 fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_req_nrmI,PVN_REQ_NRMI)(&nl, x);
    fr = PVN_FABI(pvn_req_nrmI,PVN_REQ_NRMI)(&nr, (x + nl));
  }
  return fmaxq(fl, fr);
}

__float128 PVN_FABI(pvn_rfq_nrmf,PVN_RFQ_NRMF)(const size_t *const n, const __float128 *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -2.0q;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
#ifdef __MATHIMF_H_INCLUDED
    return __hypotq(x[0], x[1]);
#else /* !__MATHIMF_H_INCLUDED */
#ifdef _GNU_SOURCE
    return __builtin_hypotf128(x[0], x[1]);
#else /* !_GNU_SOURCE */
    return hypotq(x[0], x[1]);
#endif /* ?_GNU_SOURCE */
#endif /* ?__MATHIMF_H_INCLUDED */
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  __float128 fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rfq_nrmf,PVN_RFQ_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_rfq_nrmf,PVN_RFQ_NRMF)(&nr, (x + nl));
  }
#ifdef __MATHIMF_H_INCLUDED
  return __hypotq(fl, fr);
#else /* !__MATHIMF_H_INCLUDED */
#ifdef _GNU_SOURCE
  return __builtin_hypotf128(fl, fr);
#else /* !_GNU_SOURCE */
  return hypotq(fl, fr);
#endif /* ?_GNU_SOURCE */
#endif /* ?__MATHIMF_H_INCLUDED */
}

__float128 PVN_FABI(pvn_rhq_nrmf,PVN_RHQ_NRMF)(const size_t *const n, const __float128 *const x)
{
#ifndef NDEBUG
  if (!n)
    return -1.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -2.0q;
#endif /* !NDEBUG */
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
    return pvn_v1q_hypot(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  __float128 fl, fr;
  cilk_scope {
    fl = cilk_spawn PVN_FABI(pvn_rhq_nrmf,PVN_RHQ_NRMF)(&nl, x);
    fr = PVN_FABI(pvn_rhq_nrmf,PVN_RHQ_NRMF)(&nr, (x + nl));
  }
  return pvn_v1q_hypot(fl, fr);
}

__float128 PVN_FABI(pvn_crq_nrmf,PVN_CRQ_NRMF)(const size_t *const n, const __float128 *const x)
{
#ifdef _OPENMP
#pragma omp declare reduction(hcq:__float128:omp_out=hypotq(omp_out,omp_in)) initializer(omp_priv=0.0q)
#endif /* _OPENMP */
  if (!n)
    return -1.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -2.0q;
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
    return hypotq(x[0], x[1]);
  const size_t m = *n;
  const __float128 tsml = scalbnq(1.0L, -8191);
  const __float128 tbig = scalbnq(1.0L,  8136);
  /* the three Blue's accumulators */
  __float128 sml = 0.0q, med = 0.0q, big = 0.0q;
#ifndef PVN_LAPACK
  /* the accumulator for subnormal inputs */
  __float128 dnr = 0.0q;
#endif /* !PVN_LAPACK */
#ifdef _OPENMP
#ifdef PVN_LAPACK
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcq:sml,med,big)
#else /* !PVN_LAPACK */
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcq:dnr,sml,med,big)
#endif /* ?PVN_LAPACK */
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const __float128 y = fabsq(x[i]);
    if (y > 0.0q) {
#ifdef PVN_LAPACK
      if (y < tsml)
#else /* !PVN_LAPACK */
      if (y < FLT128_MIN)
        dnr = hypotq(dnr, scalbnq(y, 112));
      else if (y < tsml)
#endif /* ?PVN_LAPACK */
        sml = hypotq(sml, y);
      else if (y > tbig)
        big = hypotq(big, y);
      else /* med */
        med = hypotq(med, y);
    }
  }
#ifndef PVN_LAPACK
  if (dnr > 0.0q)
    sml = hypotq(sml, scalbnq(dnr, -112));
#endif /* !PVN_LAPACK */
  if (sml > 0.0q)
    med = hypotq(med, sml);
  if (med > 0.0q)
    big = hypotq(big, med);
  return big;
}

__float128 PVN_FABI(pvn_qnrm2,PVN_QNRM2)(const size_t *const n, const __float128 *const x)
{
  if (!n)
    return -1.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -2.0q;
#ifdef _OPENMP
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
#ifdef PVN_NRM_SAFE
      p[tn] = PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&m, (x + o));
#else /* !PVN_NRM_SAFE */
      p[tn] = PVN_FABI(pvn_rfq_nrmf,PVN_RFQ_NRMF)(&m, (x + o));
#endif /* ?PVN_NRM_SAFE */
  }
#ifdef PVN_NRM_SAFE
  return PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&mt, p);
#else /* !PVN_NRM_SAFE */
  return PVN_FABI(pvn_rfq_nrmf,PVN_RFQ_NRMF)(&mt, p);
#endif /* ?PVN_NRM_SAFE */
#else /* !_OPENMP */
#ifdef PVN_NRM_SAFE
  return PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(n, x);
#else /* !PVN_NRM_SAFE */
  return PVN_FABI(pvn_rfq_nrmf,PVN_RFQ_NRMF)(n, x);
#endif /* ?PVN_NRM_SAFE */
#endif /* ?_OPENMP */
}

__float128 PVN_FABI(pvn_ynrm2,PVN_YNRM2)(const size_t *const n, const __float128 *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_qnrm2,PVN_QNRM2)(&m, x);
}

__float128 PVN_FABI(pvn_qnrm1,PVN_QNRM1)(const size_t *const n, const __float128 *const x)
{
  if (!n)
    return -1.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -2.0q;
#ifdef _OPENMP
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
      p[tn] = PVN_FABI(pvn_req_nrm1,PVN_REQ_NRM1)(&m, (x + o));
  }
  return PVN_FABI(pvn_req_nrm1,PVN_REQ_NRM1)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_req_nrm1,PVN_REQ_NRM1)(n, x);
#endif /* ?_OPENMP */
}

__float128 PVN_FABI(pvn_qnrmI,PVN_QNRMI)(const size_t *const n, const __float128 *const x)
{
  if (!n)
    return -1.0q;
  if (!*n)
    return -0.0q;
  if (!x)
    return -2.0q;
#ifdef _OPENMP
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
      p[tn] = PVN_FABI(pvn_req_nrmI,PVN_REQ_NRMI)(&m, (x + o));
  }
  return PVN_FABI(pvn_req_nrmI,PVN_REQ_NRMI)(&mt, p);
#else /* !_OPENMP */
  return PVN_FABI(pvn_req_nrmI,PVN_REQ_NRMI)(n, x);
#endif /* ?_OPENMP */
}
#else /* !PVN_QUADMATH */
#ifdef PVN_MPFR
long double PVN_FABI(pvn_mpq_nrmp,PVN_MPQ_NRMP)(const long double *const p, const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_mpx_nrmp,PVN_MPX_NRMP)(p, n, x);
}
#endif /* PVN_MPFR */
long double PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(n, x);
}
long double PVN_FABI(pvn_req_nrm1,PVN_REQ_NRM1)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_rex_nrm1,PVN_REX_NRM1)(n, x);
}
long double PVN_FABI(pvn_req_nrmI,PVN_REQ_NRMI)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_rex_nrmI,PVN_REX_NRMI)(n, x);
}
long double PVN_FABI(pvn_rfq_nrmf,PVN_RFQ_NRMF)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(n, x);
}
long double PVN_FABI(pvn_rhq_nrmf,PVN_RHQ_NRMF)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_rhx_nrmf,PVN_RHX_NRMF)(n, x);
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
long double PVN_FABI(pvn_qnrm1,PVN_QNRM1)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_xnrm1,PVN_XNRM1)(n, x);
}
long double PVN_FABI(pvn_qnrmI,PVN_QNRMI)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_xnrmI,PVN_XNRMI)(n, x);
}
#endif /* ?PVN_QUADMATH */

#if (defined(__AVX__) && defined(__FMA__))
static __m128 rxs_nrmf(const size_t n, const float *const x)
{
  register const __m128 z = _mm_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)3u),
    m = ((n >> 2u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm_andnot_ps(z, _mm_load_ps(x));
    case 1u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, -0.0f, x[0u]));
    case 2u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, x[1u], x[0u]));
    case 3u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, x[2u], x[1u], x[0u]));
    default: /* should never happen */
      return z;
    }
  }
  register __m128 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_load_ps(x);
    fr = (r ? rxs_nrmf(r, (x + 4u)) : _mm_load_ps(x + 4u));
    return pvn_v4s_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 2u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rxs_nrmf(nl, x);
    fr = rxs_nrmf(nr, (x + nl));
  }
  return pvn_v4s_hypot(fl, fr);
}

static __m128 rxsunrmf(const size_t n, const float *const x)
{
  register const __m128 z = _mm_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)3u),
    m = ((n >> 2u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm_andnot_ps(z, _mm_loadu_ps(x));
    case 1u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, -0.0f, x[0u]));
    case 2u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, x[1u], x[0u]));
    case 3u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, x[2u], x[1u], x[0u]));
    default: /* should never happen */
      return z;
    }
  }
  register __m128 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_loadu_ps(x);
    fr = (r ? rxsunrmf(r, (x + 4u)) : _mm_loadu_ps(x + 4u));
    return pvn_v4s_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 2u);
  const size_t nr = (n - nl);

  cilk_scope {
    fl = cilk_spawn rxsunrmf(nl, x);
    fr = rxsunrmf(nr, (x + nl));
  }
  return pvn_v4s_hypot(fl, fr);
}

float PVN_FABI(pvn_rxs_nrmf,PVN_RXS_NRMF)(const size_t *const n, const float *const x)
{
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
  register const __m128 r = (((uintptr_t)x & (uintptr_t)0x0Fu) ? rxsunrmf(*n, x) : rxs_nrmf(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(16) float f[4u];
  _mm_store_ps(f, r);
  const size_t m = (size_t)4u;
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&m, f);
#else /* !PVN_NRM_SAFE */
  return pvn_v4s_hypot_red(r);
#endif /* ?PVN_NRM_SAFE */
}

static __m128d rxd_nrmf(const size_t n, const double *const x)
{
  register const __m128d z = _mm_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)1u),
    m = ((n >> 1u) + r);
  if (m == (size_t)1u) {
    if (r)
      return _mm_andnot_pd(z, _mm_set_pd(-0.0, *x));
    else
      return _mm_andnot_pd(z, _mm_load_pd(x));
  }
  register __m128d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_load_pd(x);
    fr = (r ? rxd_nrmf(r, (x + 2u)) : _mm_load_pd(x + 2u));
    return pvn_v2d_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 1u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rxd_nrmf(nl, x);
    fr = rxd_nrmf(nr, (x + nl));
  }
  return pvn_v2d_hypot(fl, fr);
}

static __m128d rxdunrmf(const size_t n, const double *const x)
{
  register const __m128d z = _mm_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)1u),
    m = ((n >> 1u) + r);
  if (m == (size_t)1u) {
    if (r)
      return _mm_andnot_pd(z, _mm_set_pd(-0.0, *x));
    else
      return _mm_andnot_pd(z, _mm_loadu_pd(x));
  }
  register __m128d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_loadu_pd(x);
    fr = (r ? rxdunrmf(r, (x + 2u)) : _mm_loadu_pd(x + 2u));
    return pvn_v2d_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 1u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rxdunrmf(nl, x);
    fr = rxdunrmf(nr, (x + nl));
  }
  return pvn_v2d_hypot(fl, fr);
}

double PVN_FABI(pvn_rxd_nrmf,PVN_RXD_NRMF)(const size_t *const n, const double *const x)
{
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
  register const __m128d r = (((uintptr_t)x & (uintptr_t)0x0Fu) ? rxdunrmf(*n, x) : rxd_nrmf(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(16) double f[2u];
  _mm_store_pd(f, r);
  return hypot(f[0u], f[1u]);
#else /* !PVN_NRM_SAFE */
  return pvn_v2d_hypot_red(r);
#endif /* ?PVN_NRM_SAFE */
}

static __m128 rxs_nrm1(const size_t n, const float *const x)
{
  register const __m128 z = _mm_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)3u),
    m = ((n >> 2u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm_andnot_ps(z, _mm_load_ps(x));
    case 1u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, -0.0f, x[0u]));
    case 2u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, x[1u], x[0u]));
    case 3u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, x[2u], x[1u], x[0u]));
    default: /* should never happen */
      return z;
    }
  }
  register __m128 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_load_ps(x);
    fr = (r ? rxs_nrm1(r, (x + 4u)) : _mm_load_ps(x + 4u));
    return pvn_v4s_add_abs(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 2u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rxs_nrm1(nl, x);
    fr = rxs_nrm1(nr, (x + nl));
  }
  return pvn_v4s_add_abs(fl, fr);
}

static __m128 rxsunrm1(const size_t n, const float *const x)
{
  register const __m128 z = _mm_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)3u),
    m = ((n >> 2u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm_andnot_ps(z, _mm_loadu_ps(x));
    case 1u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, -0.0f, x[0u]));
    case 2u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, x[1u], x[0u]));
    case 3u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, x[2u], x[1u], x[0u]));
    default: /* should never happen */
      return z;
    }
  }
  register __m128 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_loadu_ps(x);
    fr = (r ? rxsunrm1(r, (x + 4u)) : _mm_loadu_ps(x + 4u));
    return pvn_v4s_add_abs(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 2u);
  const size_t nr = (n - nl);

  cilk_scope {
    fl = cilk_spawn rxsunrm1(nl, x);
    fr = rxsunrm1(nr, (x + nl));
  }
  return pvn_v4s_add_abs(fl, fr);
}

float PVN_FABI(pvn_rxs_nrm1,PVN_RXS_NRM1)(const size_t *const n, const float *const x)
{
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
  register const __m128 r = (((uintptr_t)x & (uintptr_t)0x0Fu) ? rxsunrm1(*n, x) : rxs_nrm1(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(16) float f[4u];
  _mm_store_ps(f, r);
  const size_t m = (size_t)4u;
  return PVN_FABI(pvn_res_nrm1,PVN_RES_NRM1)(&m, f);
#else /* !PVN_NRM_SAFE */
  return pvn_v4s_add_red(r);
#endif /* ?PVN_NRM_SAFE */
}

static __m128d rxd_nrm1(const size_t n, const double *const x)
{
  register const __m128d z = _mm_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)1u),
    m = ((n >> 1u) + r);
  if (m == (size_t)1u) {
    if (r)
      return _mm_andnot_pd(z, _mm_set_pd(-0.0, *x));
    else
      return _mm_andnot_pd(z, _mm_load_pd(x));
  }
  register __m128d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_load_pd(x);
    fr = (r ? rxd_nrm1(r, (x + 2u)) : _mm_load_pd(x + 2u));
    return pvn_v2d_add_abs(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 1u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rxd_nrm1(nl, x);
    fr = rxd_nrm1(nr, (x + nl));
  }
  return pvn_v2d_add_abs(fl, fr);
}

static __m128d rxdunrm1(const size_t n, const double *const x)
{
  register const __m128d z = _mm_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)1u),
    m = ((n >> 1u) + r);
  if (m == (size_t)1u) {
    if (r)
      return _mm_andnot_pd(z, _mm_set_pd(-0.0, *x));
    else
      return _mm_andnot_pd(z, _mm_loadu_pd(x));
  }
  register __m128d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_loadu_pd(x);
    fr = (r ? rxdunrm1(r, (x + 2u)) : _mm_loadu_pd(x + 2u));
    return pvn_v2d_add_abs(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 1u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rxdunrm1(nl, x);
    fr = rxdunrm1(nr, (x + nl));
  }
  return pvn_v2d_add_abs(fl, fr);
}

double PVN_FABI(pvn_rxd_nrm1,PVN_RXD_NRM1)(const size_t *const n, const double *const x)
{
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
  register const __m128d r = (((uintptr_t)x & (uintptr_t)0x0Fu) ? rxdunrm1(*n, x) : rxd_nrm1(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(16) double f[2u];
  _mm_store_pd(f, r);
  return pvn_v1d_add_abs(f[0u], f[1u]);
#else /* !PVN_NRM_SAFE */
  return pvn_v2d_add_red(r);
#endif /* ?PVN_NRM_SAFE */
}

static __m128 rxs_nrmI(const size_t n, const float *const x)
{
  register const __m128 z = _mm_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)3u),
    m = ((n >> 2u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm_andnot_ps(z, _mm_load_ps(x));
    case 1u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, -0.0f, x[0u]));
    case 2u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, x[1u], x[0u]));
    case 3u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, x[2u], x[1u], x[0u]));
    default: /* should never happen */
      return z;
    }
  }
  register __m128 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_load_ps(x);
    fr = (r ? rxs_nrmI(r, (x + 4u)) : _mm_load_ps(x + 4u));
    return pvn_v4s_max_abs(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 2u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rxs_nrmI(nl, x);
    fr = rxs_nrmI(nr, (x + nl));
  }
  return pvn_v4s_max_abs(fl, fr);
}

static __m128 rxsunrmI(const size_t n, const float *const x)
{
  register const __m128 z = _mm_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)3u),
    m = ((n >> 2u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm_andnot_ps(z, _mm_loadu_ps(x));
    case 1u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, -0.0f, x[0u]));
    case 2u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, -0.0f, x[1u], x[0u]));
    case 3u:
      return _mm_andnot_ps(z, _mm_set_ps(-0.0f, x[2u], x[1u], x[0u]));
    default: /* should never happen */
      return z;
    }
  }
  register __m128 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_loadu_ps(x);
    fr = (r ? rxsunrmI(r, (x + 4u)) : _mm_loadu_ps(x + 4u));
    return pvn_v4s_max_abs(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 2u);
  const size_t nr = (n - nl);

  cilk_scope {
    fl = cilk_spawn rxsunrmI(nl, x);
    fr = rxsunrmI(nr, (x + nl));
  }
  return pvn_v4s_max_abs(fl, fr);
}

float PVN_FABI(pvn_rxs_nrmI,PVN_RXS_NRMI)(const size_t *const n, const float *const x)
{
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
  register const __m128 r = (((uintptr_t)x & (uintptr_t)0x0Fu) ? rxsunrmI(*n, x) : rxs_nrmI(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(16) float f[4u];
  _mm_store_ps(f, r);
  const size_t m = (size_t)4u;
  return PVN_FABI(pvn_res_nrmI,PVN_RES_NRMI)(&m, f);
#else /* !PVN_NRM_SAFE */
  return pvn_v4s_max_red(r);
#endif /* ?PVN_NRM_SAFE */
}

static __m128d rxd_nrmI(const size_t n, const double *const x)
{
  register const __m128d z = _mm_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)1u),
    m = ((n >> 1u) + r);
  if (m == (size_t)1u) {
    if (r)
      return _mm_andnot_pd(z, _mm_set_pd(-0.0, *x));
    else
      return _mm_andnot_pd(z, _mm_load_pd(x));
  }
  register __m128d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_load_pd(x);
    fr = (r ? rxd_nrmI(r, (x + 2u)) : _mm_load_pd(x + 2u));
    return pvn_v2d_max_abs(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 1u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rxd_nrmI(nl, x);
    fr = rxd_nrmI(nr, (x + nl));
  }
  return pvn_v2d_max_abs(fl, fr);
}

static __m128d rxdunrmI(const size_t n, const double *const x)
{
  register const __m128d z = _mm_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)1u),
    m = ((n >> 1u) + r);
  if (m == (size_t)1u) {
    if (r)
      return _mm_andnot_pd(z, _mm_set_pd(-0.0, *x));
    else
      return _mm_andnot_pd(z, _mm_loadu_pd(x));
  }
  register __m128d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm_loadu_pd(x);
    fr = (r ? rxdunrmI(r, (x + 2u)) : _mm_loadu_pd(x + 2u));
    return pvn_v2d_max_abs(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 1u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rxdunrmI(nl, x);
    fr = rxdunrmI(nr, (x + nl));
  }
  return pvn_v2d_max_abs(fl, fr);
}

double PVN_FABI(pvn_rxd_nrmI,PVN_RXD_NRMI)(const size_t *const n, const double *const x)
{
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
  register const __m128d r = (((uintptr_t)x & (uintptr_t)0x0Fu) ? rxdunrmI(*n, x) : rxd_nrmI(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(16) double f[2u];
  _mm_store_pd(f, r);
  return pvn_v1d_max_abs(f[0u], f[1u]);
#else /* !PVN_NRM_SAFE */
  return pvn_v2d_max_red(r);
#endif /* ?PVN_NRM_SAFE */
}

static __m256 rys_nrmf(const size_t n, const float *const x)
{
  register const __m256 z = _mm256_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)7u),
    m = ((n >> 3u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm256_andnot_ps(z, _mm256_load_ps(x));
    case 1u:
      return _mm256_andnot_ps(z, _mm256_insertf128_ps(z, _mm_set_ps(-0.0f, -0.0f, -0.0f, x[0u]), 0));
    case 2u:
      return _mm256_andnot_ps(z, _mm256_insertf128_ps(z, _mm_set_ps(-0.0f, -0.0f, x[1u], x[0u]), 0));
    case 3u:
      return _mm256_andnot_ps(z, _mm256_insertf128_ps(z, _mm_set_ps(-0.0f, x[2u], x[1u], x[0u]), 0));
    case 4u:
      return _mm256_andnot_ps(z, _mm256_insertf128_ps(z, _mm_load_ps(x), 0));
    case 5u:
      return _mm256_andnot_ps(z, _mm256_set_m128(_mm_set_ps(-0.0f, -0.0f, -0.0f, x[4u]), _mm_load_ps(x)));
    case 6u:
      return _mm256_andnot_ps(z, _mm256_set_m128(_mm_set_ps(-0.0f, -0.0f, x[5u], x[4u]), _mm_load_ps(x)));
    case 7u:
      return _mm256_andnot_ps(z, _mm256_set_m128(_mm_set_ps(-0.0f, x[6u], x[5u], x[4u]), _mm_load_ps(x)));
    default: /* should never happen */
      return z;
    }
  }
  register __m256 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm256_load_ps(x);
    fr = (r ? rys_nrmf(r, (x + 8u)) : _mm256_load_ps(x + 8u));
    return pvn_v8s_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 3u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rys_nrmf(nl, x);
    fr = rys_nrmf(nr, (x + nl));
  }
  return pvn_v8s_hypot(fl, fr);
}

static __m256 rysunrmf(const size_t n, const float *const x)
{
  register const __m256 z = _mm256_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)7u),
    m = ((n >> 3u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm256_andnot_ps(z, _mm256_loadu_ps(x));
    case 1u:
      return _mm256_andnot_ps(z, _mm256_insertf128_ps(z, _mm_set_ps(-0.0f, -0.0f, -0.0f, x[0u]), 0));
    case 2u:
      return _mm256_andnot_ps(z, _mm256_insertf128_ps(z, _mm_set_ps(-0.0f, -0.0f, x[1u], x[0u]), 0));
    case 3u:
      return _mm256_andnot_ps(z, _mm256_insertf128_ps(z, _mm_set_ps(-0.0f, x[2u], x[1u], x[0u]), 0));
    case 4u:
      return _mm256_andnot_ps(z, _mm256_insertf128_ps(z, _mm_loadu_ps(x), 0));
    case 5u:
      return _mm256_andnot_ps(z, _mm256_set_m128(_mm_set_ps(-0.0f, -0.0f, -0.0f, x[4u]), _mm_loadu_ps(x)));
    case 6u:
      return _mm256_andnot_ps(z, _mm256_set_m128(_mm_set_ps(-0.0f, -0.0f, x[5u], x[4u]), _mm_loadu_ps(x)));
    case 7u:
      return _mm256_andnot_ps(z, _mm256_set_m128(_mm_set_ps(-0.0f, x[6u], x[5u], x[4u]), _mm_loadu_ps(x)));
    default: /* should never happen */
      return z;
    }
  }
  register __m256 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm256_loadu_ps(x);
    fr = (r ? rysunrmf(r, (x + 8u)) : _mm256_loadu_ps(x + 8u));
    return pvn_v8s_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 3u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rysunrmf(nl, x);
    fr = rysunrmf(nr, (x + nl));
  }
  return pvn_v8s_hypot(fl, fr);
}

float PVN_FABI(pvn_rys_nrmf,PVN_RYS_NRMF)(const size_t *const n, const float *const x)
{
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
  register const __m256 r = (((uintptr_t)x & (uintptr_t)0x1Fu) ? rysunrmf(*n, x) : rys_nrmf(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(32) float f[8u];
  _mm256_store_ps(f, r);
  const size_t m = (size_t)8u;
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&m, f);
#else /* !PVN_NRM_SAFE */
  return pvn_v8s_hypot_red(r);
#endif /* ?PVN_NRM_SAFE */
}

static __m256d ryd_nrmf(const size_t n, const double *const x)
{
  register const __m256d z = _mm256_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)3u),
    m = ((n >> 2u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm256_andnot_pd(z, _mm256_load_pd(x));
    case 1u:
      return _mm256_andnot_pd(z, _mm256_insertf128_pd(z, _mm_set_pd(-0.0, x[0u]), 0));
    case 2u:
      return _mm256_andnot_pd(z, _mm256_insertf128_pd(z, _mm_load_pd(x), 0));
    case 3u:
      return _mm256_andnot_pd(z, _mm256_set_m128d(_mm_set_pd(-0.0, x[2u]), _mm_load_pd(x)));
    default: /* should never happen */
      return z;
    }
  }
  register __m256d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm256_load_pd(x);
    fr = (r ? ryd_nrmf(r, (x + 4u)) : _mm256_load_pd(x + 4u));
    return pvn_v4d_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 2u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn ryd_nrmf(nl, x);
    fr = ryd_nrmf(nr, (x + nl));
  }
  return pvn_v4d_hypot(fl, fr);
}

static __m256d rydunrmf(const size_t n, const double *const x)
{
  register const __m256d z = _mm256_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)3u),
    m = ((n >> 2u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm256_andnot_pd(z, _mm256_loadu_pd(x));
    case 1u:
      return _mm256_andnot_pd(z, _mm256_insertf128_pd(z, _mm_set_pd(-0.0, x[0u]), 0));
    case 2u:
      return _mm256_andnot_pd(z, _mm256_insertf128_pd(z, _mm_loadu_pd(x), 0));
    case 3u:
      return _mm256_andnot_pd(z, _mm256_set_m128d(_mm_set_pd(-0.0, x[2u]), _mm_loadu_pd(x)));
    default: /* should never happen */
      return z;
    }
  }
  register __m256d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm256_loadu_pd(x);
    fr = (r ? rydunrmf(r, (x + 4u)) : _mm256_loadu_pd(x + 4u));
    return pvn_v4d_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 2u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rydunrmf(nl, x);
    fr = rydunrmf(nr, (x + nl));
  }
  return pvn_v4d_hypot(fl, fr);
}

double PVN_FABI(pvn_ryd_nrmf,PVN_RYD_NRMF)(const size_t *const n, const double *const x)
{
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
  register const __m256d r = (((uintptr_t)x & (uintptr_t)0x1Fu) ? rydunrmf(*n, x) : ryd_nrmf(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(32) double f[4u];
  _mm256_store_pd(f, r);
  const size_t m = (size_t)4u;
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&m, f);
#else /* !PVN_NRM_SAFE */
  return pvn_v4d_hypot_red(r);
#endif /* ?PVN_NRM_SAFE */
}
#ifdef __AVX512F__
static __m512 rzs_nrmf(const size_t n, const float *const x)
{
  register const __m512 z = _mm512_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)15u),
    m = ((n >> 4u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case  0u:
      return _mm512_andnot_ps(z, _mm512_load_ps(x));
    case  1u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,     (__mmask16)1u, x));
    case  2u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,     (__mmask16)3u, x));
    case  3u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,     (__mmask16)7u, x));
    case  4u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,    (__mmask16)15u, x));
    case  5u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,    (__mmask16)31u, x));
    case  6u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,    (__mmask16)63u, x));
    case  7u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,   (__mmask16)127u, x));
    case  8u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,   (__mmask16)255u, x));
    case  9u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,   (__mmask16)511u, x));
    case 10u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,  (__mmask16)1023u, x));
    case 11u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,  (__mmask16)2047u, x));
    case 12u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,  (__mmask16)4095u, x));
    case 13u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z,  (__mmask16)8191u, x));
    case 14u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z, (__mmask16)16383u, x));
    case 15u:
      return _mm512_andnot_ps(z, _mm512_mask_load_ps(z, (__mmask16)32767u, x));
    default: /* should never happen */
      return z;
    }
  }
  register __m512 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm512_load_ps(x);
    fr = (r ? rzs_nrmf(r, (x + 16u)) : _mm512_load_ps(x + 16u));
    return pvn_v16s_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 4u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rzs_nrmf(nl, x);
    fr = rzs_nrmf(nr, (x + nl));
  }
  return pvn_v16s_hypot(fl, fr);
}

static __m512 rzsunrmf(const size_t n, const float *const x)
{
  register const __m512 z = _mm512_set1_ps(-0.0f);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)15u),
    m = ((n >> 4u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case  0u:
      return _mm512_andnot_ps(z, _mm512_loadu_ps(x));
    case  1u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,     (__mmask16)1u, x));
    case  2u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,     (__mmask16)3u, x));
    case  3u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,     (__mmask16)7u, x));
    case  4u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,    (__mmask16)15u, x));
    case  5u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,    (__mmask16)31u, x));
    case  6u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,    (__mmask16)63u, x));
    case  7u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,   (__mmask16)127u, x));
    case  8u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,   (__mmask16)255u, x));
    case  9u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,   (__mmask16)511u, x));
    case 10u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,  (__mmask16)1023u, x));
    case 11u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,  (__mmask16)2047u, x));
    case 12u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,  (__mmask16)4095u, x));
    case 13u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z,  (__mmask16)8191u, x));
    case 14u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z, (__mmask16)16383u, x));
    case 15u:
      return _mm512_andnot_ps(z, _mm512_mask_loadu_ps(z, (__mmask16)32767u, x));
    default: /* should never happen */
      return z;
    }
  }
  register __m512 fl, fr;
  if (m == (size_t)2u) {
    fl = _mm512_loadu_ps(x);
    fr = (r ? rzsunrmf(r, (x + 16u)) : _mm512_loadu_ps(x + 16u));
    return pvn_v16s_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 4u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rzsunrmf(nl, x);
    fr = rzsunrmf(nr, (x + nl));
  }
  return pvn_v16s_hypot(fl, fr);
}

float PVN_FABI(pvn_rzs_nrmf,PVN_RZS_NRMF)(const size_t *const n, const float *const x)
{
  if (!n)
    return -1.0f;
  if (!*n)
    return -0.0f;
  if (!x)
    return -2.0f;
  register const __m512 r = (((uintptr_t)x & (uintptr_t)0x3Fu) ? rzsunrmf(*n, x) : rzs_nrmf(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(64) float f[16u];
  _mm512_store_ps(f, r);
  const size_t m = (size_t)16u;
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&m, f);
#else /* !PVN_NRM_SAFE */
  return pvn_v16s_hypot_red(r);
#endif /* ?PVN_NRM_SAFE */
}

static __m512d rzd_nrmf(const size_t n, const double *const x)
{
  register const __m512d z = _mm512_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  const size_t
    r = (n & (size_t)7u),
    m = ((n >> 3u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm512_andnot_pd(z, _mm512_load_pd(x));
    case 1u:
      return _mm512_andnot_pd(z, _mm512_mask_load_pd(z,   (__mmask8)1u, x));
    case 2u:
      return _mm512_andnot_pd(z, _mm512_mask_load_pd(z,   (__mmask8)3u, x));
    case 3u:
      return _mm512_andnot_pd(z, _mm512_mask_load_pd(z,   (__mmask8)7u, x));
    case 4u:
      return _mm512_andnot_pd(z, _mm512_mask_load_pd(z,  (__mmask8)15u, x));
    case 5u:
      return _mm512_andnot_pd(z, _mm512_mask_load_pd(z,  (__mmask8)31u, x));
    case 6u:
      return _mm512_andnot_pd(z, _mm512_mask_load_pd(z,  (__mmask8)63u, x));
    case 7u:
      return _mm512_andnot_pd(z, _mm512_mask_load_pd(z, (__mmask8)127u, x));
    default: /* should never happen */
      return z;
    }
  }
  register __m512d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm512_load_pd(x);
    fr = (r ? rzd_nrmf(r, (x + 8u)) : _mm512_load_pd(x + 8u));
    return pvn_v8d_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 3u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rzd_nrmf(nl, x);
    fr = rzd_nrmf(nr, (x + nl));
  }
  return pvn_v8d_hypot(fl, fr);
}

static __m512d rzdunrmf(const size_t n, const double *const x)
{
  register const __m512d z = _mm512_set1_pd(-0.0);
#ifndef NDEBUG
  if (!n)
    return z;
#endif /* !NDEBUG */
  size_t
    r = (n & (size_t)7u),
    m = ((n >> 3u) + (r != (size_t)0u));
  if (m == (size_t)1u) {
    switch ((unsigned)r) {
    case 0u:
      return _mm512_andnot_pd(z, _mm512_loadu_pd(x));
    case 1u:
      return _mm512_andnot_pd(z, _mm512_mask_loadu_pd(z,   (__mmask8)1u, x));
    case 2u:
      return _mm512_andnot_pd(z, _mm512_mask_loadu_pd(z,   (__mmask8)3u, x));
    case 3u:
      return _mm512_andnot_pd(z, _mm512_mask_loadu_pd(z,   (__mmask8)7u, x));
    case 4u:
      return _mm512_andnot_pd(z, _mm512_mask_loadu_pd(z,  (__mmask8)15u, x));
    case 5u:
      return _mm512_andnot_pd(z, _mm512_mask_loadu_pd(z,  (__mmask8)31u, x));
    case 6u:
      return _mm512_andnot_pd(z, _mm512_mask_loadu_pd(z,  (__mmask8)63u, x));
    case 7u:
      return _mm512_andnot_pd(z, _mm512_mask_loadu_pd(z, (__mmask8)127u, x));
    default: /* should never happen */
      return z;
    }
  }
  register __m512d fl, fr;
  if (m == (size_t)2u) {
    fl = _mm512_loadu_pd(x);
    fr = (r ? rzdunrmf(r, (x + 8u)) : _mm512_loadu_pd(x + 8u));
    return pvn_v8d_hypot(fl, fr);
  }
  const size_t nl = (((m >> 1u) + (m & (size_t)1u)) << 3u);
  const size_t nr = (n - nl);
  cilk_scope {
    fl = cilk_spawn rzdunrmf(nl, x);
    fr = rzdunrmf(nr, (x + nl));
  }
  return pvn_v8d_hypot(fl, fr);
}

double PVN_FABI(pvn_rzd_nrmf,PVN_RZD_NRMF)(const size_t *const n, const double *const x)
{
  if (!n)
    return -1.0;
  if (!*n)
    return -0.0;
  if (!x)
    return -2.0;
  register const __m512d r = (((uintptr_t)x & (uintptr_t)0x3Fu) ? rzdunrmf(*n, x) : rzd_nrmf(*n, x));
#ifdef PVN_NRM_SAFE
  alignas(64) double f[8u];
  _mm512_store_pd(f, r);
  const size_t m = (size_t)8u;
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&m, f);
#else /* !PVN_NRM_SAFE */
  return pvn_v8d_hypot_red(r);
#endif /* ?PVN_NRM_SAFE */
}
#endif /* __AVX512F__ */
#endif /* __AVX__ && __FMA__ */
#endif /* ?PVN_TEST */

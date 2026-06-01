#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
#ifdef PVN_MPFR
  if ((argc != 2) && (argc != 3)) {
    (void)fprintf(stderr, "%s n [prec]\n", *argv);
    return EXIT_FAILURE;
  }
  const size_t n = pvn_atoz(argv[1]);
  if (!n)
    return EXIT_SUCCESS;
  mpfr_rnd_t rnd = MPFR_RNDN;
  mpfr_exp_t emin = __MPFR_EXP_INVALID, emax = __MPFR_EXP_INVALID;
  mpfr_prec_t prec = ((argc == 3) ? atol(argv[2]) : 113l);
  int t = PVN_FABI(pvn_mpfr_start,PVN_MPFR_START)(&rnd, &prec, &emin, &emax);
  if (t) {
    (void)fprintf(stderr, "PVN_MPFR_START=%d\n", t);
    return EXIT_FAILURE;
  }
  double a = __builtin_nan("a"), b = __builtin_nan("b"), c = __builtin_nan("c"), d = __builtin_nan("d"), r = __builtin_nan("r"), x = __builtin_nan("x"), e = __builtin_inf(), E = 0.0;
  mpfr_t ma, mb, mc, md, mr, mx;
  t = mpfr_init_set_d(ma, a, MPFR_RNDN);
  t = mpfr_init_set_d(mb, b, MPFR_RNDN);
  t = mpfr_init_set_d(mc, c, MPFR_RNDN);
  t = mpfr_init_set_d(md, d, MPFR_RNDN);
  t = mpfr_init_set_d(mr, r, MPFR_RNDN);
  t = mpfr_init_set_d(mx, x, MPFR_RNDN);
  int u = PVN_FABI(pvn_ran_open,PVN_RAN_OPEN)();
  char s[26] = { '\0' };
  for (size_t i = 0u; i < n; ++i) {
    do {
      a = PVN_FABI(pvn_ran,PVN_RAN)(&u);
    } while (!__builtin_isfinite(a));
#ifndef NDEBUG
    (void)printf("%s,", pvn_dtoa(s, a));
#endif /* !NDEBUG */
    do {
      b = PVN_FABI(pvn_ran,PVN_RAN)(&u);
    } while (!__builtin_isfinite(b));
#ifndef NDEBUG
    (void)printf("%s,", pvn_dtoa(s, b));
#endif /* !NDEBUG */
    do {
      c = PVN_FABI(pvn_ran,PVN_RAN)(&u);
    } while (!__builtin_isfinite(c));
#ifndef NDEBUG
    (void)printf("%s,", pvn_dtoa(s, c));
#endif /* !NDEBUG */
    do {
      d = PVN_FABI(pvn_ran,PVN_RAN)(&u);
    } while (!__builtin_isfinite(d));
#ifndef NDEBUG
    (void)printf("%s;", pvn_dtoa(s, d));
#endif /* !NDEBUG */
    t = 0;
    r = PVN_FABI(pvn_ddet,PVN_DDET)(&a, &b, &c, &d, &x, &t);
#ifndef NDEBUG
    (void)printf("%s,%5d;", pvn_dtoa(s, x), t);
#endif /* !NDEBUG */
    (void)mpfr_set_d(ma, a, MPFR_RNDN);
    (void)mpfr_set_d(mb, b, MPFR_RNDN);
    (void)mpfr_set_d(mc, c, MPFR_RNDN);
    (void)mpfr_set_d(md, d, MPFR_RNDN);
    (void)mpfr_set_d(mx, x, MPFR_RNDN);
    (void)mpfr_mul_2si(mx, mx, (long)t, MPFR_RNDN);
    (void)mpfr_fmms(mr, ma, md, mb, mc, MPFR_RNDN);
    (void)mpfr_sub(mx, mr, mx, MPFR_RNDN);
    (void)mpfr_div(mx, mx, mr, MPFR_RNDN);
    (void)mpfr_abs(mx, mx, MPFR_RNDN);
    r = mpfr_get_d(mx, MPFR_RNDN);
#ifndef NDEBUG
    (void)printf("%s\n", pvn_dtoa(s, r));
#endif /* !NDEBUG */
    e = __builtin_fmin(e, r);
    E = __builtin_fmax(E, r);
  }
  u = PVN_FABI(pvn_ran_close,PVN_RAN_CLOSE)(&u);
  mpfr_clear(mx);
  mpfr_clear(mr);
  mpfr_clear(md);
  mpfr_clear(mc);
  mpfr_clear(mb);
  mpfr_clear(ma);
  t = PVN_FABI(pvn_mpfr_stop,PVN_MPFR_STOP)();
  (void)printf("min rel err=%s\n", pvn_dtoa(s, e));
  (void)printf("max rel err=%s\n", pvn_dtoa(s, E));
#else /* !PVN_MPFR */
  if (argc != 5) {
    (void)fprintf(stderr, "%s a b c d\n", *argv);
    return EXIT_FAILURE;
  }
  const double a = atof(argv[1]);
  const double b = atof(argv[2]);
  const double c = atof(argv[3]);
  const double d = atof(argv[4]);
  char s[26] = { '\0' };
  double x = __builtin_nan("x"),
    y = pvn_ddet(a, b, c, d);
  (void)printf("pvn_ddet=%s\n", pvn_dtoa(s, y));
  int t = 0;
  y = PVN_FABI(pvn_ddet,PVN_DDET)(&a, &b, &c, &d, &x, &t);
  (void)printf("PVN_DDET=%s (", pvn_dtoa(s, y));
  (void)printf("%s,%d)\n", pvn_dtoa(s, x), t);
  return EXIT_SUCCESS;
#endif /* ?PVN_MPFR */
}
#else /* !PVN_TEST */
float PVN_FABI(pvn_sdet,PVN_SDET)(const float *const a, const float *const b, const float *const c, const float *const d, float *const x, int *const t)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  int
    ea = 0,
    eb = 0,
    ec = 0,
    ed = 0;
  const float
    fb = __builtin_frexpf(*b, &eb),
    fc = __builtin_frexpf(*c, &ec);
  float
    fa = __builtin_frexpf(*a, &ea),
    fd = __builtin_frexpf(*d, &ed);
  const int
    u = (eb + ec),
    v = (ea + ed);
  int
    s = (u - v);
  *t = ((s > FLT_MAX_EXP) ? (s - FLT_MAX_EXP) : 0);
  *t += (*t & 1);
  const int
    t_2 = -(*t >> 1);
  fa = __builtin_scalbnf(fa, t_2);
  fd = __builtin_scalbnf(fd, t_2);
  s -= *t;
  *t += v;
  const float
    w = (fb * fc),
    e = __builtin_fmaf(-fb, fc, w),
    f = __builtin_fmaf(fa, fd, -__builtin_scalbnf(w, s));
  *x = __builtin_fmaf(__builtin_scalbnf(1.0f, s - 1), 2.0f * e, f);
  *x = __builtin_frexpf(*x, &s);
  *t += s;
  return __builtin_scalbnf(*x, *t);
}

double PVN_FABI(pvn_ddet,PVN_DDET)(const double *const a, const double *const b, const double *const c, const double *const d, double *const x, int *const t)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  int
    ea = 0,
    eb = 0,
    ec = 0,
    ed = 0;
  const double
    fb = __builtin_frexp(*b, &eb),
    fc = __builtin_frexp(*c, &ec);
  double
    fa = __builtin_frexp(*a, &ea),
    fd = __builtin_frexp(*d, &ed);
  const int
    u = (eb + ec),
    v = (ea + ed);
  int
    s = (u - v);
  *t = ((s > DBL_MAX_EXP) ? (s - DBL_MAX_EXP) : 0);
  *t += (*t & 1);
  const int
    t_2 = -(*t >> 1);
  fa = __builtin_scalbn(fa, t_2);
  fd = __builtin_scalbn(fd, t_2);
  s -= *t;
  *t += v;
  const double
    w = (fb * fc),
    e = __builtin_fma(-fb, fc, w),
    f = __builtin_fma(fa, fd, -__builtin_scalbn(w, s));
  *x = __builtin_fma(__builtin_scalbn(1.0, s - 1), 2.0 * e, f);
  *x = __builtin_frexp(*x, &s);
  *t += s;
  return __builtin_scalbn(*x, *t);
}

long double PVN_FABI(pvn_xdet,PVN_XDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d, long double *const x, int *const t)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  int
    ea = 0,
    eb = 0,
    ec = 0,
    ed = 0;
  const long double
    fb = __builtin_frexpl(*b, &eb),
    fc = __builtin_frexpl(*c, &ec);
  long double
    fa = __builtin_frexpl(*a, &ea),
    fd = __builtin_frexpl(*d, &ed);
  const int
    u = (eb + ec),
    v = (ea + ed);
  int
    s = (u - v);
  *t = ((s > LDBL_MAX_EXP) ? (s - LDBL_MAX_EXP) : 0);
  *t += (*t & 1);
  const int
    t_2 = -(*t >> 1);
  fa = __builtin_scalbnl(fa, t_2);
  fd = __builtin_scalbnl(fd, t_2);
  s -= *t;
  *t += v;
  const long double
    w = (fb * fc),
    e = __builtin_fmal(-fb, fc, w),
    f = __builtin_fmal(fa, fd, -__builtin_scalbnl(w, s));
  *x = __builtin_fmal(__builtin_scalbnl(1.0L, s - 1), 2.0L * e, f);
  *x = __builtin_frexpl(*x, &s);
  *t += s;
  return __builtin_scalbnl(*x, *t);
}
#ifdef PVN_QUADMATH
__float128 PVN_FABI(pvn_qdet,PVN_QDET)(const __float128 *const a, const __float128 *const b, const __float128 *const c, const __float128 *const d, __float128 *const x, int *const t)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  int
    ea = 0,
    eb = 0,
    ec = 0,
    ed = 0;
  const __float128
    fb = frexpq(*b, &eb),
    fc = frexpq(*c, &ec);
  __float128
    fa = frexpq(*a, &ea),
    fd = frexpq(*d, &ed);
  const int
    u = (eb + ec),
    v = (ea + ed);
  int
    s = (u - v);
  *t = ((s > FLT128_MAX_EXP) ? (s - FLT128_MAX_EXP) : 0);
  *t += (*t & 1);
  const int
    t_2 = -(*t >> 1);
  fa = scalbnq(fa, t_2);
  fd = scalbnq(fd, t_2);
  s -= *t;
  *t += v;
  const __float128
    w = (fb * fc),
    e = fmaq(-fb, fc, w),
    f = fmaq(fa, fd, -scalbnq(w, s));
  *x = fmaq(scalbnq(1.0q, s - 1), 2.0q * e, f);
  *x = frexpq(*x, &s);
  *t += s;
  return scalbnq(*x, *t);
}
#else /* !PVN_QUADMATH */
long double PVN_FABI(pvn_qdet,PVN_QDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d, long double *const x, int *const t)
{
  return PVN_FABI(pvn_xdet,PVN_XDET)(a, b, c, d, x, t);
}
#endif /* ?PVN_QUADMATH */

#ifdef __AVX512F__
#ifndef ZFREXPF
#define ZFREXPF(X,E,M)                \
  E = _mm512_getexp_ps(X);            \
  m = _mm512_cmplt_ps_mask(mI, E);    \
  E = _mm512_mask_add_ps(Z, m, E, O); \
  M = _mm512_mask_getmant_ps(Z, m, X, _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src)
#else /* ZFREXPF */
#error ZFREXPF already defined
#endif /* ?ZFREXPF */
void PVN_FABI(pvn_zdetf,PVN_ZDETF)(const float *const a, const float *const b, const float *const c, const float *const d, float *const x, int *const t, float *const y)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  PVN_ASSERT(y);
  register __m512 fA = _mm512_load_ps(a);
  register __m512 fB = _mm512_load_ps(b);
  register __m512 fC = _mm512_load_ps(c);
  register __m512 fD = _mm512_load_ps(d);
  register const __m512 Z = _mm512_setzero_ps();
  register const __m512 O = _mm512_set1_ps(1.0f);
  register const __m512 mI = _mm512_set1_ps(-__builtin_inff());
  register __m512 eA, eB, eC, eD;
  register __mmask16 m;
  ZFREXPF(fA, eA, fA);
  ZFREXPF(fB, eB, fB);
  ZFREXPF(fC, eC, fC);
  ZFREXPF(fD, eD, fD);
  register const __m512 U = _mm512_add_ps(eB, eC);
  register const __m512 V = _mm512_add_ps(eA, eD);
  register __m512 S = _mm512_sub_ps(U, V);
  register __m512 T = _mm512_set1_ps(FLT_MAX_EXP);
  T = _mm512_sub_ps(S, T);
  T = _mm512_max_ps(T, Z);
  register __m512 W = _mm512_cvtepi32_ps(_mm512_and_epi32(_mm512_cvtps_epi32(T), _mm512_set1_epi32(1)));
  T = _mm512_add_ps(T, W);
  W = _mm512_mul_ps(T, _mm512_set1_ps(-0.5f));
  fA = _mm512_scalef_ps(fA, W);
  fD = _mm512_scalef_ps(fD, W);
  S = _mm512_sub_ps(S, T);
  T = _mm512_add_ps(T, V);
  W = _mm512_mul_ps(fB, fC);
  register const __m512 E = _mm512_mul_ps(_mm512_fnmadd_ps(fB, fC, W), _mm512_set1_ps(2.0f));
  W = _mm512_scalef_ps(W, S);
  register const __m512 F = _mm512_fmsub_ps(fA, fD, W);
  W = _mm512_sub_ps(S, O);
  W = _mm512_scalef_ps(O, W);
  W = _mm512_fmadd_ps(W, E, F);
  ZFREXPF(W, S, W);
  _mm512_store_ps(x, W);
  T = _mm512_add_ps(T, S);
  _mm512_store_epi32(t, _mm512_cvtps_epi32(T));
  W = _mm512_scalef_ps(W, T);
  _mm512_store_ps(y, W);
}
#ifndef ZFREXP
#define ZFREXP(X,E,M)                 \
  E = _mm512_getexp_pd(X);            \
  m = _mm512_cmplt_pd_mask(mI, E);    \
  E = _mm512_mask_add_pd(Z, m, E, O); \
  M = _mm512_mask_getmant_pd(Z, m, X, _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src)
#else /* ZFREXP */
#error ZFREXP already defined
#endif /* ?ZFREXP */
void PVN_FABI(pvn_zdet,PVN_ZDET)(const double *const a, const double *const b, const double *const c, const double *const d, double *const x, int *const t, double *const y)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  PVN_ASSERT(y);
  register __m512d fA = _mm512_load_pd(a);
  register __m512d fB = _mm512_load_pd(b);
  register __m512d fC = _mm512_load_pd(c);
  register __m512d fD = _mm512_load_pd(d);
  register const __m512d Z = _mm512_setzero_pd();
  register const __m512d O = _mm512_set1_pd(1.0);
  register const __m512d mI = _mm512_set1_pd(-__builtin_inf());
  register __m512d eA, eB, eC, eD;
  register __mmask8 m;
  ZFREXP(fA, eA, fA);
  ZFREXP(fB, eB, fB);
  ZFREXP(fC, eC, fC);
  ZFREXP(fD, eD, fD);
  register const __m512d U = _mm512_add_pd(eB, eC);
  register const __m512d V = _mm512_add_pd(eA, eD);
  register __m512d S = _mm512_sub_pd(U, V);
  register __m512d T = _mm512_set1_pd(DBL_MAX_EXP);
  T = _mm512_sub_pd(S, T);
  T = _mm512_max_pd(T, Z);
  register __m512d W = _mm512_cvtepi32_pd(_mm256_and_si256(_mm512_cvtpd_epi32(T), _mm256_set1_epi32(1)));
  T = _mm512_add_pd(T, W);
  W = _mm512_mul_pd(T, _mm512_set1_pd(-0.5));
  fA = _mm512_scalef_pd(fA, W);
  fD = _mm512_scalef_pd(fD, W);
  S = _mm512_sub_pd(S, T);
  T = _mm512_add_pd(T, V);
  W = _mm512_mul_pd(fB, fC);
  register const __m512d E = _mm512_mul_pd(_mm512_fnmadd_pd(fB, fC, W), _mm512_set1_pd(2.0));
  W = _mm512_scalef_pd(W, S);
  register const __m512d F = _mm512_fmsub_pd(fA, fD, W);
  W = _mm512_sub_pd(S, O);
  W = _mm512_scalef_pd(O, W);
  W = _mm512_fmadd_pd(W, E, F);
  ZFREXP(W, S, W);
  _mm512_store_pd(x, W);
  T = _mm512_add_pd(T, S);
  _mm256_store_si256((__m256i*)t, _mm512_cvtpd_epi32(T));
  W = _mm512_scalef_pd(W, T);
  _mm512_store_pd(y, W);
}
#endif /* __AVX512F__ */
#endif /* ?PVN_TEST */

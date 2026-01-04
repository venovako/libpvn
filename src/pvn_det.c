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
  if (s > FLT_MAX_EXP) {
    *t = (s - FLT_MAX_EXP);
    *t += (*t & 1);
    const int
      t_2 = -(*t >> 1);
    fa = __builtin_scalbnf(fa, t_2);
    fd = __builtin_scalbnf(fd, t_2);
    s -= *t;
    *t += v;
  }
  else
    *t = v;
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
  if (s > DBL_MAX_EXP) {
    *t = (s - DBL_MAX_EXP);
    *t += (*t & 1);
    const int
      t_2 = -(*t >> 1);
    fa = __builtin_scalbn(fa, t_2);
    fd = __builtin_scalbn(fd, t_2);
    s -= *t;
    *t += v;
  }
  else
    *t = v;
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
  if (s > LDBL_MAX_EXP) {
    *t = (s - LDBL_MAX_EXP);
    *t += (*t & 1);
    const int
      t_2 = -(*t >> 1);
    fa = __builtin_scalbnl(fa, t_2);
    fd = __builtin_scalbnl(fd, t_2);
    s -= *t;
    *t += v;
  }
  else
    *t = v;
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
  if (s > FLT128_MAX_EXP) {
    *t = (s - FLT128_MAX_EXP);
    *t += (*t & 1);
    const int
      t_2 = -(*t >> 1);
    fa = scalbnq(fa, t_2);
    fd = scalbnq(fd, t_2);
    s -= *t;
    *t += v;
  }
  else
    *t = v;
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
#endif /* ?PVN_TEST */

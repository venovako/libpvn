#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc == 5) {
    const double a = atof(argv[1]);
    const double b = atof(argv[2]);
    const double c = atof(argv[3]);
    const double d = atof(argv[4]);
    char s[26] = { '\0' };
    double r = pvn_ddet(a, b, c, d);
    (void)printf("pvn_ddet=%s\n", pvn_dtoa(s, r));
    r = PVN_FABI(pvn_ddet,PVN_DDET)(&a, &b, &c, &d);
    (void)printf("PVN_DDET=%s\n", pvn_dtoa(s, r));
  }
  else {
    (void)fprintf(stderr, "%s a b c d\n", *argv);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
/* TODO: not yet exhaustively tested! */

float PVN_FABI(pvn_sdet,PVN_SDET)(const float *const a, const float *const b, const float *const c, const float *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(isfinite(*a));
  PVN_ASSERT(isfinite(*b));
  PVN_ASSERT(isfinite(*c));
  PVN_ASSERT(isfinite(*d));
  int
    ea = 0,
    eb = 0,
    ec = 0,
    ed = 0;
  const float
    fb = frexpf(*b, &eb),
    fc = frexpf(*c, &ec);
  float
    fa = frexpf(*a, &ea),
    fd = frexpf(*d, &ed);
  const int
    u = (eb + ec),
    v = (ea + ed);
  int
    s = (u - v),
    t = ((s > FLT_MAX_EXP) ? (s - FLT_MAX_EXP) : 0);
  if (t > 0) {
    t += (t & 1);
    const int
      t_2 = -(t >> 1);
    fa = scalbnf(fa, t_2);
    fd = scalbnf(fd, t_2);
    s -= t;
    t += v;
  }
  else
    t = v;
  const float
    w = (fb * fc),
    e = fmaf(-fb, fc, w),
    f = fmaf(fa, fd, scalbnf(-w, s)),
    x = fmaf(scalbnf(1.0f, s - 1), 2.0f * e, f);
  return scalbnf(x, t);
}

double PVN_FABI(pvn_ddet,PVN_DDET)(const double *const a, const double *const b, const double *const c, const double *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(isfinite(*a));
  PVN_ASSERT(isfinite(*b));
  PVN_ASSERT(isfinite(*c));
  PVN_ASSERT(isfinite(*d));
  int
    ea = 0,
    eb = 0,
    ec = 0,
    ed = 0;
  const double
    fb = frexp(*b, &eb),
    fc = frexp(*c, &ec);
  double
    fa = frexp(*a, &ea),
    fd = frexp(*d, &ed);
  const int
    u = (eb + ec),
    v = (ea + ed);
  int
    s = (u - v),
    t = ((s > DBL_MAX_EXP) ? (s - DBL_MAX_EXP) : 0);
  if (t > 0) {
    t += (t & 1);
    const int
      t_2 = -(t >> 1);
    fa = scalbn(fa, t_2);
    fd = scalbn(fd, t_2);
    s -= t;
    t += v;
  }
  else
    t = v;
  const double
    w = (fb * fc),
    e = fma(-fb, fc, w),
    f = fma(fa, fd, scalbn(-w, s)),
    x = fma(scalbn(1.0, s - 1), 2.0 * e, f);
  return scalbn(x, t);
}

long double PVN_FABI(pvn_xdet,PVN_XDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(isfinite(*a));
  PVN_ASSERT(isfinite(*b));
  PVN_ASSERT(isfinite(*c));
  PVN_ASSERT(isfinite(*d));
  int
    ea = 0,
    eb = 0,
    ec = 0,
    ed = 0;
  const long double
    fb = frexpl(*b, &eb),
    fc = frexpl(*c, &ec);
  long double
    fa = frexpl(*a, &ea),
    fd = frexpl(*d, &ed);
  const int
    u = (eb + ec),
    v = (ea + ed);
  int
    s = (u - v),
    t = ((s > LDBL_MAX_EXP) ? (s - LDBL_MAX_EXP) : 0);
  if (t > 0) {
    t += (t & 1);
    const int
      t_2 = -(t >> 1);
    fa = scalbnl(fa, t_2);
    fd = scalbnl(fd, t_2);
    s -= t;
    t += v;
  }
  else
    t = v;
  const long double
    w = (fb * fc),
    e = fmal(-fb, fc, w),
    f = fmal(fa, fd, scalbnl(-w, s)),
    x = fmal(scalbnl(1.0L, s - 1), 2.0L * e, f);
  return scalbnl(x, t);
}
#ifdef PVN_QUADMATH
__float128 PVN_FABI(pvn_qdet,PVN_QDET)(const __float128 *const a, const __float128 *const b, const __float128 *const c, const __float128 *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(isfiniteq(*a));
  PVN_ASSERT(isfiniteq(*b));
  PVN_ASSERT(isfiniteq(*c));
  PVN_ASSERT(isfiniteq(*d));
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
    s = (u - v),
    t = ((s > FLT128_MAX_EXP) ? (s - FLT128_MAX_EXP) : 0);
  if (t > 0) {
    t += (t & 1);
    const int
      t_2 = -(t >> 1);
    fa = scalbnq(fa, t_2);
    fd = scalbnq(fd, t_2);
    s -= t;
    t += v;
  }
  else
    t = v;
  const __float128
    w = (fb * fc),
    e = fmaq(-fb, fc, w),
    f = fmaq(fa, fd, scalbnq(-w, s)),
    x = fmaq(scalbnq(1.0q, s - 1), 2.0q * e, f);
  return scalbnq(x, t);
}
#else /* !PVN_QUADMATH */
long double PVN_FABI(pvn_qdet,PVN_QDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d)
{
  return PVN_FABI(pvn_xdet,PVN_XDET)(a, b, c, d);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

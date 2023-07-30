#include "pvn.h"

#ifdef FLT_BIG_EXP
#error FLT_BIG_EXP already defined
#else /* !FLT_BIG_EXP => 125 */
#define FLT_BIG_EXP (FLT_MAX_EXP - 3)
#endif /* ?FLT_BIG_EXP */

#ifdef DBL_BIG_EXP
#error DBL_BIG_EXP already defined
#else /* !DBL_BIG_EXP => 1021 */
#define DBL_BIG_EXP (DBL_MAX_EXP - 3)
#endif /* ?DBL_BIG_EXP */

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if ((argc < 4) || (argc > 5)) {
    (void)fprintf(stderr, "%s a11 a22 a21r [a21i]\n", *argv);
    return EXIT_FAILURE;
  }
  const double a11 = atof(argv[1]);
  const double a22 = atof(argv[2]);
  const double a21r = atof(argv[3]);
  const double a21i = ((5 == argc) ? atof(argv[4]) : 0.0);
  double cs, snr, sni, l1, l2;
  int es, lc;
  char s[26u];
  if (4 == argc) {
    lc = dljev2_(&a11, &a22, &a21r, &cs, &snr, &l1, &l2, &es);
    (void)printf("dljev2_ = %d, es = %d\n", lc, es);
    sni = 0.0;
  }
  else {
    lc = zljev2_(&a11, &a22, &a21r, &a21i, &cs, &snr, &sni, &l1, &l2, &es);
    (void)printf("zljev2_ = %d, es = %d\n", lc, es);
  }
  (void)printf("a11  = %s\n", pvn_dtoa(s, a11));
  (void)printf("a22  = %s\n", pvn_dtoa(s, a22));
  (void)printf("a21r = %s\n", pvn_dtoa(s, a21r));
  (void)printf("a21i = %s\n", pvn_dtoa(s, a21i));
  (void)printf("cs   = %s\n", pvn_dtoa(s, cs));
  (void)printf("snr  = %s\n", pvn_dtoa(s, snr));
  (void)printf("sni  = %s\n", pvn_dtoa(s, sni));
  (void)printf("l1   = %s\n", pvn_dtoa(s, l1));
  (void)printf("l2   = %s\n", pvn_dtoa(s, l2));
  if (es) {
    l1 = scalbn(l1, es);
    l2 = scalbn(l2, es);
    (void)printf("l11  = %s\n", pvn_dtoa(s, l1));
    (void)printf("l22  = %s\n", pvn_dtoa(s, l2));
  }
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int sljev2_(const float *const a11, const float *const a22, const float *const a21, float *const cs, float *const sn, float *const l1, float *const l2, int *const es)
{
  assert(a11);
  assert(a22);
  assert(a21);
  assert(cs);
  assert(sn);
  assert(l1);
  assert(l2);
  assert(es);
  if (!isfinite(*a11))
    return -1;
  if (!isfinite(*a22))
    return -2;
  if (!isfinite(*a21))
    return -3;
  int e1, e2, er;
  (void)frexpf(fmaxf(fabsf(*a11), FLT_TRUE_MIN), &e1); e1 = FLT_BIG_EXP - e1;
  (void)frexpf(fmaxf(fabsf(*a22), FLT_TRUE_MIN), &e2); e2 = FLT_BIG_EXP - e2;
  (void)frexpf(fmaxf(fabsf(*a21), FLT_TRUE_MIN), &er); er = FLT_BIG_EXP - er;
  e1 = ((e1 <= e2) ? e1 : e2);
  *es = ((e1 <= er) ? e1 : er);
  const float
    a1 = scalbnf(*a11, *es),
    a2 = scalbnf(*a22, *es),
    ar = scalbnf(*a21, *es),
    aa = fabsf(ar),
    as = copysignf(1.0f, ar),
    an = (aa * 2.0f),
    ad = (a1 - a2),
    t2 = copysignf(fmaxf(an / fabsf(ad), 0.0f), ad),
    t1 = (t2 / (1.0f + hypotf(t2, 1.0f))),
    s2 = fmaf(t1, t1, 1.0f),
    c1 = rsqrtf(s2),
    s1 = (t1 * c1);
  *cs = c1;
  *sn = as * s1;
  *l1 = fmaf(t1, fmaf(a2, t1,  an), a1) / s2;
  *l2 = fmaf(t1, fmaf(a1, t1, -an), a2) / s2;
  *es = -*es;
  return 0;
}

int dljev2_(const double *const a11, const double *const a22, const double *const a21, double *const cs, double *const sn, double *const l1, double *const l2, int *const es)
{
  assert(a11);
  assert(a22);
  assert(a21);
  assert(cs);
  assert(sn);
  assert(l1);
  assert(l2);
  assert(es);
  if (!isfinite(*a11))
    return -1;
  if (!isfinite(*a22))
    return -2;
  if (!isfinite(*a21))
    return -3;
  int e1, e2, er;
  (void)frexp(fmax(fabs(*a11), DBL_TRUE_MIN), &e1); e1 = DBL_BIG_EXP - e1;
  (void)frexp(fmax(fabs(*a22), DBL_TRUE_MIN), &e2); e2 = DBL_BIG_EXP - e2;
  (void)frexp(fmax(fabs(*a21), DBL_TRUE_MIN), &er); er = DBL_BIG_EXP - er;
  e1 = ((e1 <= e2) ? e1 : e2);
  *es = ((e1 <= er) ? e1 : er);
  const double
    a1 = scalbn(*a11, *es),
    a2 = scalbn(*a22, *es),
    ar = scalbn(*a21, *es),
    aa = fabs(ar),
    as = copysign(1.0, ar),
    an = (aa * 2.0),
    ad = (a1 - a2),
    t2 = copysign(fmax(an / fabs(ad), 0.0), ad),
    t1 = (t2 / (1.0 + hypot(t2, 1.0))),
    s2 = fma(t1, t1, 1.0),
    c1 = rsqrt(s2),
    s1 = (t1 * c1);
  *cs = c1;
  *sn = as * s1;
  *l1 = fma(t1, fma(a2, t1,  an), a1) / s2;
  *l2 = fma(t1, fma(a1, t1, -an), a2) / s2;
  *es = -*es;
  return 0;
}

int cljev2_(const float *const a11, const float *const a22, const float *const a21r, const float *const a21i, float *const cs, float *const snr, float *const sni, float *const l1, float *const l2, int *const es)
{
  assert(a11);
  assert(a22);
  assert(a21r);
  assert(a21i);
  assert(cs);
  assert(sn);
  assert(l1);
  assert(l2);
  assert(es);
  if (!isfinite(*a11))
    return -1;
  if (!isfinite(*a22))
    return -2;
  if (!isfinite(*a21r))
    return -3;
  if (!isfinite(*a21i))
    return -4;
  int e1, e2, er, ei;
  (void)frexpf(fmaxf(fabsf(*a11), FLT_TRUE_MIN), &e1); e1 = FLT_BIG_EXP - e1;
  (void)frexpf(fmaxf(fabsf(*a22), FLT_TRUE_MIN), &e2); e2 = FLT_BIG_EXP - e2;
  (void)frexpf(fmaxf(fabsf(*a21r), FLT_TRUE_MIN), &er); er = FLT_BIG_EXP - er;
  (void)frexpf(fmaxf(fabsf(*a21i), FLT_TRUE_MIN), &ei); ei = FLT_BIG_EXP - ei;
  e1 = ((e1 <= e2) ? e1 : e2);
  e2 = ((er <= ei) ? er : ei);
  *es = ((e1 <= e2) ? e1 : e2);
  const float
    a1 = scalbnf(*a11, *es),
    a2 = scalbnf(*a22, *es),
    ar = scalbnf(*a21r, *es),
    ai = scalbnf(*a21i, *es);
  float
    ar_ = fabsf(ar),
    ai_ = fabsf(ai);
  const float
    am = fminf(ar_, ai_),
    aM = fmaxf(ar_, ai_);
  float aa = fmaxf(am / aM, 0.0f);
  aa = hypotf(aa, 1.0f) * aM;
  ar_ = copysignf(fminf(ar_ / aa, 1.0f), ar);
  ai_ = ai / fmaxf(aa, FLT_TRUE_MIN);
  const float
    an = (aa * 2.0f),
    ad = (a1 - a2),
    t2 = copysignf(fmaxf(an / fabsf(ad), 0.0f), ad),
    t1 = (t2 / (1.0f + hypotf(t2, 1.0f))),
    s2 = fmaf(t1, t1, 1.0f),
    c1 = rsqrtf(s2),
    s1 = (t1 * c1);
  *cs = c1;
  *snr = ar_ * s1;
  *sni = ai_ * s1;
  *l1 = fmaf(t1, fmaf(a2, t1,  an), a1) / s2;
  *l2 = fmaf(t1, fmaf(a1, t1, -an), a2) / s2;
  *es = -*es;
  return 0;
}

int zljev2_(const double *const a11, const double *const a22, const double *const a21r, const double *const a21i, double *const cs, double *const snr, double *const sni, double *const l1, double *const l2, int *const es)
{
  assert(a11);
  assert(a22);
  assert(a21r);
  assert(a21i);
  assert(cs);
  assert(sn);
  assert(l1);
  assert(l2);
  assert(es);
  if (!isfinite(*a11))
    return -1;
  if (!isfinite(*a22))
    return -2;
  if (!isfinite(*a21r))
    return -3;
  if (!isfinite(*a21i))
    return -4;
  int e1, e2, er, ei;
  (void)frexp(fmax(fabs(*a11), DBL_TRUE_MIN), &e1); e1 = DBL_BIG_EXP - e1;
  (void)frexp(fmax(fabs(*a22), DBL_TRUE_MIN), &e2); e2 = DBL_BIG_EXP - e2;
  (void)frexp(fmax(fabs(*a21r), DBL_TRUE_MIN), &er); er = DBL_BIG_EXP - er;
  (void)frexp(fmax(fabs(*a21i), DBL_TRUE_MIN), &ei); ei = DBL_BIG_EXP - ei;
  e1 = ((e1 <= e2) ? e1 : e2);
  e2 = ((er <= ei) ? er : ei);
  *es = ((e1 <= e2) ? e1 : e2);
  const double
    a1 = scalbn(*a11, *es),
    a2 = scalbn(*a22, *es),
    ar = scalbn(*a21r, *es),
    ai = scalbn(*a21i, *es);
  double
    ar_ = fabs(ar),
    ai_ = fabs(ai);
  const double
    am = fmin(ar_, ai_),
    aM = fmax(ar_, ai_);
  double aa = fmax(am / aM, 0.0);
  aa = hypot(aa, 1.0) * aM;
  ar_ = copysign(fmin(ar_ / aa, 1.0), ar);
  ai_ = ai / fmax(aa, DBL_TRUE_MIN);
  const double
    an = (aa * 2.0),
    ad = (a1 - a2),
    t2 = copysign(fmax(an / fabs(ad), 0.0), ad),
    t1 = (t2 / (1.0 + hypot(t2, 1.0))),
    s2 = fma(t1, t1, 1.0),
    c1 = rsqrt(s2),
    s1 = (t1 * c1);
  *cs = c1;
  *snr = ar_ * s1;
  *sni = ai_ * s1;
  *l1 = fma(t1, fma(a2, t1,  an), a1) / s2;
  *l2 = fma(t1, fma(a1, t1, -an), a2) / s2;
  *es = -*es;
  return 0;
}
#endif /* ?PVN_TEST */

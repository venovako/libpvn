#include "pvn.h"

#ifdef FLT_BIG_EXP
#error FLT_BIG_EXP already defined
#else /* !FLT_BIG_EXP */
#define FLT_BIG_EXP (FLT_MAX_EXP - 3)
#endif /* ?FLT_BIG_EXP */

#ifdef DBL_BIG_EXP
#error DBL_BIG_EXP already defined
#else /* !DBL_BIG_EXP */
#define DBL_BIG_EXP (DBL_MAX_EXP - 3)
#endif /* ?DBL_BIG_EXP */

#ifdef LDBL_BIG_EXP
#error LDBL_BIG_EXP already defined
#else /* !LDBL_BIG_EXP */
#define LDBL_BIG_EXP (LDBL_MAX_EXP - 3)
#endif /* ?LDBL_BIG_EXP */

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
  double cs = 1.0, snr = 0.0, sni = 0.0, l1 = 0.0, l2 = 0.0;
  int es = 0, lc = 0;
  char s[26u] = { '\0' };
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
  float a1 = *a11;
  if (!isfinite(a1))
    return -1;
  float a2 = *a22;
  if (!isfinite(a2))
    return -2;
  float ar = *a21;
  if (!isfinite(ar))
    return -3;
  const int wt = (*es ? 1 : 0);
  int e1, e2, er;
  (void)frexpf(fmaxf(fabsf(a1), FLT_TRUE_MIN), &e1);
  (void)frexpf(fmaxf(fabsf(a2), FLT_TRUE_MIN), &e2);
  (void)frexpf(fmaxf(fabsf(ar), FLT_TRUE_MIN), &er);
  *es = FLT_BIG_EXP - pvn_imax3(e1, e2, er);
  if (*es) {
    a1 = scalbnf(a1, *es);
    a2 = scalbnf(a2, *es);
    ar = scalbnf(ar, *es);
    *es = -*es;
  }
  const float
    aa = fabsf(ar),
    as = copysignf(1.0f, ar),
    an = (aa * 2.0f),
    ad = (a1 - a2),
    t2 = copysignf(fminf(fmaxf(an / fabsf(ad), 0.0f), FLT_MAX), ad),
    t1 = (t2 / (1.0f + hypotf(t2, 1.0f))),
    s2 = fmaf(t1, t1, 1.0f),
    c1 = rsqrtf(s2);
  *cs = c1;
  if (wt)
    *sn = as * t1;
  else {
    const float s1 = t1 * c1;
    *sn = as * s1;
  }
  *l1 = fmaf(t1, fmaf(a2, t1,  an), a1) / s2;
  *l2 = fmaf(t1, fmaf(a1, t1, -an), a2) / s2;
  return wt;
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
  double a1 = *a11;
  if (!isfinite(a1))
    return -1;
  double a2 = *a22;
  if (!isfinite(a2))
    return -2;
  double ar = *a21;
  if (!isfinite(ar))
    return -3;
  const int wt = (*es ? 1 : 0);
  int e1, e2, er;
  (void)frexp(fmax(fabs(a1), DBL_TRUE_MIN), &e1);
  (void)frexp(fmax(fabs(a2), DBL_TRUE_MIN), &e2);
  (void)frexp(fmax(fabs(ar), DBL_TRUE_MIN), &er);
  *es = DBL_BIG_EXP - pvn_imax3(e1, e2, er);
  if (*es) {
    a1 = scalbn(a1, *es);
    a2 = scalbn(a2, *es);
    ar = scalbn(ar, *es);
    *es = -*es;
  }
  const double
    aa = fabs(ar),
    as = copysign(1.0, ar),
    an = (aa * 2.0),
    ad = (a1 - a2),
    t2 = copysign(fmin(fmax(an / fabs(ad), 0.0), DBL_MAX), ad),
    t1 = (t2 / (1.0 + hypot(t2, 1.0))),
    s2 = fma(t1, t1, 1.0),
    c1 = rsqrt(s2);
  *cs = c1;
  if (wt)
    *sn = as * t1;
  else {
    const double s1 = t1 * c1;
    *sn = as * s1;
  }
  *l1 = fma(t1, fma(a2, t1,  an), a1) / s2;
  *l2 = fma(t1, fma(a1, t1, -an), a2) / s2;
  return wt;
}

int xljev2_(const long double *const a11, const long double *const a22, const long double *const a21, long double *const cs, long double *const sn, long double *const l1, long double *const l2, int *const es)
{
  assert(a11);
  assert(a22);
  assert(a21);
  assert(cs);
  assert(sn);
  assert(l1);
  assert(l2);
  assert(es);
  long double a1 = *a11;
  if (!isfinite(a1))
    return -1;
  long double a2 = *a22;
  if (!isfinite(a2))
    return -2;
  long double ar = *a21;
  if (!isfinite(ar))
    return -3;
  const int wt = (*es ? 1 : 0);
  int e1, e2, er;
  (void)frexpl(fmaxl(fabsl(a1), LDBL_TRUE_MIN), &e1);
  (void)frexpl(fmaxl(fabsl(a2), LDBL_TRUE_MIN), &e2);
  (void)frexpl(fmaxl(fabsl(ar), LDBL_TRUE_MIN), &er);
  *es = LDBL_BIG_EXP - pvn_imax3(e1, e2, er);
  if (*es) {
    a1 = scalbnl(a1, *es);
    a2 = scalbnl(a2, *es);
    ar = scalbnl(ar, *es);
    *es = -*es;
  }
  const long double
    aa = fabsl(ar),
    as = copysignl(1.0L, ar),
    an = (aa * 2.0L),
    ad = (a1 - a2),
    t2 = copysignl(fminl(fmaxl(an / fabsl(ad), 0.0L), LDBL_MAX), ad),
    t1 = (t2 / (1.0L + hypotl(t2, 1.0L))),
    s2 = fmal(t1, t1, 1.0L),
    c1 = rsqrtl(s2);
  *cs = c1;
  if (wt)
    *sn = as * t1;
  else {
    const long double s1 = t1 * c1;
    *sn = as * s1;
  }
  *l1 = fmal(t1, fmal(a2, t1,  an), a1) / s2;
  *l2 = fmal(t1, fmal(a1, t1, -an), a2) / s2;
  return wt;
}

int cljev2_(const float *const a11, const float *const a22, const float *const a21r, const float *const a21i, float *const cs, float *const snr, float *const sni, float *const l1, float *const l2, int *const es)
{
  assert(a11);
  assert(a22);
  assert(a21r);
  assert(a21i);
  assert(cs);
  assert(snr);
  assert(sni);
  assert(l1);
  assert(l2);
  assert(es);
  float a1 = *a11;
  if (!isfinite(a1))
    return -1;
  float a2 = *a22;
  if (!isfinite(a2))
    return -2;
  float ar = *a21r;
  if (!isfinite(ar))
    return -3;
  float ai = *a21i;
  if (!isfinite(ai))
    return -4;
  const int wt = (*es ? 1 : 0);
  int e1, e2, er, ei;
  (void)frexpf(fmaxf(fabsf(a1), FLT_TRUE_MIN), &e1);
  (void)frexpf(fmaxf(fabsf(a2), FLT_TRUE_MIN), &e2);
  (void)frexpf(fmaxf(fabsf(ar), FLT_TRUE_MIN), &er);
  (void)frexpf(fmaxf(fabsf(ai), FLT_TRUE_MIN), &ei);
  *es = FLT_BIG_EXP - pvn_imax4(e1, e2, er, ei);
  if (*es) {
    a1 = scalbnf(a1, *es);
    a2 = scalbnf(a2, *es);
    ar = scalbnf(ar, *es);
    ai = scalbnf(ai, *es);
    *es = -*es;
  }
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
    t2 = copysignf(fminf(fmaxf(an / fabsf(ad), 0.0f), FLT_MAX), ad),
    t1 = (t2 / (1.0f + hypotf(t2, 1.0f))),
    s2 = fmaf(t1, t1, 1.0f),
    c1 = rsqrtf(s2);
  *cs = c1;
  if (wt) {
    *snr = ar_ * t1;
    *sni = ai_ * t1;
  }
  else {
    const float s1 = t1 * c1;
    *snr = ar_ * s1;
    *sni = ai_ * s1;
  }
  *l1 = fmaf(t1, fmaf(a2, t1,  an), a1) / s2;
  *l2 = fmaf(t1, fmaf(a1, t1, -an), a2) / s2;
  return wt;
}

int zljev2_(const double *const a11, const double *const a22, const double *const a21r, const double *const a21i, double *const cs, double *const snr, double *const sni, double *const l1, double *const l2, int *const es)
{
  assert(a11);
  assert(a22);
  assert(a21r);
  assert(a21i);
  assert(cs);
  assert(snr);
  assert(sni);
  assert(l1);
  assert(l2);
  assert(es);
  double a1 = *a11;
  if (!isfinite(a1))
    return -1;
  double a2 = *a22;
  if (!isfinite(a2))
    return -2;
  double ar = *a21r;
  if (!isfinite(ar))
    return -3;
  double ai = *a21i;
  if (!isfinite(ai))
    return -4;
  const int wt = (*es ? 1 : 0);
  int e1, e2, er, ei;
  (void)frexp(fmax(fabs(a1), DBL_TRUE_MIN), &e1);
  (void)frexp(fmax(fabs(a2), DBL_TRUE_MIN), &e2);
  (void)frexp(fmax(fabs(ar), DBL_TRUE_MIN), &er);
  (void)frexp(fmax(fabs(ai), DBL_TRUE_MIN), &ei);
  *es = DBL_BIG_EXP - pvn_imax4(e1, e2, er, ei);
  if (*es) {
    a1 = scalbn(a1, *es);
    a2 = scalbn(a2, *es);
    ar = scalbn(ar, *es);
    ai = scalbn(ai, *es);
    *es = -*es;
  }
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
    t2 = copysign(fmin(fmax(an / fabs(ad), 0.0), DBL_MAX), ad),
    t1 = (t2 / (1.0 + hypot(t2, 1.0))),
    s2 = fma(t1, t1, 1.0),
    c1 = rsqrt(s2);
  *cs = c1;
  if (wt) {
    *snr = ar_ * t1;
    *sni = ai_ * t1;
  }
  else {
    const double s1 = t1 * c1;
    *snr = ar_ * s1;
    *sni = ai_ * s1;
  }
  *l1 = fma(t1, fma(a2, t1,  an), a1) / s2;
  *l2 = fma(t1, fma(a1, t1, -an), a2) / s2;
  return wt;
}

int wljev2_(const long double *const a11, const long double *const a22, const long double *const a21r, const long double *const a21i, long double *const cs, long double *const snr, long double *const sni, long double *const l1, long double *const l2, int *const es)
{
  assert(a11);
  assert(a22);
  assert(a21r);
  assert(a21i);
  assert(cs);
  assert(snr);
  assert(sni);
  assert(l1);
  assert(l2);
  assert(es);
  long double a1 = *a11;
  if (!isfinite(a1))
    return -1;
  long double a2 = *a22;
  if (!isfinite(a2))
    return -2;
  long double ar = *a21r;
  if (!isfinite(ar))
    return -3;
  long double ai = *a21i;
  if (!isfinite(ai))
    return -4;
  const int wt = (*es ? 1 : 0);
  int e1, e2, er, ei;
  (void)frexpl(fmaxl(fabsl(a1), LDBL_TRUE_MIN), &e1);
  (void)frexpl(fmaxl(fabsl(a2), LDBL_TRUE_MIN), &e2);
  (void)frexpl(fmaxl(fabsl(ar), LDBL_TRUE_MIN), &er);
  (void)frexpl(fmaxl(fabsl(ai), LDBL_TRUE_MIN), &ei);
  *es = LDBL_BIG_EXP - pvn_imax4(e1, e2, er, ei);
  if (*es) {
    a1 = scalbnl(a1, *es);
    a2 = scalbnl(a2, *es);
    ar = scalbnl(ar, *es);
    ai = scalbnl(ai, *es);
    *es = -*es;
  }
  long double
    ar_ = fabsl(ar),
    ai_ = fabsl(ai);
  const long double
    am = fminl(ar_, ai_),
    aM = fmaxl(ar_, ai_);
  long double aa = fmaxl(am / aM, 0.0L);
  aa = hypotl(aa, 1.0L) * aM;
  ar_ = copysignl(fminl(ar_ / aa, 1.0L), ar);
  ai_ = ai / fmaxl(aa, LDBL_TRUE_MIN);
  const long double
    an = (aa * 2.0L),
    ad = (a1 - a2),
    t2 = copysignl(fminl(fmaxl(an / fabsl(ad), 0.0L), LDBL_MAX), ad),
    t1 = (t2 / (1.0L + hypotl(t2, 1.0L))),
    s2 = fmal(t1, t1, 1.0L),
    c1 = rsqrtl(s2);
  *cs = c1;
  if (wt) {
    *snr = ar_ * t1;
    *sni = ai_ * t1;
  }
  else {
    const long double s1 = t1 * c1;
    *snr = ar_ * s1;
    *sni = ai_ * s1;
  }
  *l1 = fmal(t1, fmal(a2, t1,  an), a1) / s2;
  *l2 = fmal(t1, fmal(a1, t1, -an), a2) / s2;
  return wt;
}
#endif /* ?PVN_TEST */

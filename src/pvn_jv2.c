#include "pvn.h"

#ifdef SUB_MAX_EXP
#error SUM_MAX_EXP already defined
#else /* !SUB_MAX_EXP */
#ifdef PVN_JV2_SAFE
#define SUB_MAX_EXP 3
#else /* !PVN_JV2_SAFE */
#define SUB_MAX_EXP 2
#endif /* ?PVN_JV2_SAFE */
#endif /* ?SUB_MAX_EXP */

#ifdef FLT_BIG_EXP
#error FLT_BIG_EXP already defined
#else /* !FLT_BIG_EXP */
#define FLT_BIG_EXP (FLT_MAX_EXP - SUB_MAX_EXP)
#endif /* ?FLT_BIG_EXP */

#ifdef DBL_BIG_EXP
#error DBL_BIG_EXP already defined
#else /* !DBL_BIG_EXP */
#define DBL_BIG_EXP (DBL_MAX_EXP - SUB_MAX_EXP)
#endif /* ?DBL_BIG_EXP */

#ifdef LDBL_BIG_EXP
#error LDBL_BIG_EXP already defined
#else /* !LDBL_BIG_EXP */
#define LDBL_BIG_EXP (LDBL_MAX_EXP - SUB_MAX_EXP)
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
  double cs = 1.0, snr = 0.0, sni = 0.0;
  int es = 0, lc = 0;
  if (4 == argc) {
    lc = pvn_dljv2_(&a11, &a22, &a21r, &cs, &snr, &es);
    (void)printf("pvn_dljv2_ = %d, es = %d\n", lc, es);
  }
  else {
    lc = pvn_zljv2_(&a11, &a22, &a21r, &a21i, &cs, &snr, &sni, &es);
    (void)printf("pvn_zljv2_ = %d, es = %d\n", lc, es);
  }
  if (lc < 0)
    return EXIT_FAILURE;
  char s[46u] = { '\0' };
  if (4 == argc) {
#ifdef PVN_QUADMATH
    __float128 c[2][2];
#else /* !PVN_QUADMATH */
    long double c[2][2];
#endif /* ?PVN_QUADMATH */
    (void)printf("A =\n");
    (void)printf("[%s,", pvn_dtoa(s, a11));
    (void)printf("%s]\n", pvn_dtoa(s, a21r));
    (void)printf("[%s,", pvn_dtoa(s, a21r));
    (void)printf("%s]\n", pvn_dtoa(s, a22));
    (void)printf("V =\n");
    (void)printf("[%s,", pvn_dtoa(s, cs));
    (void)printf("%s]\n", pvn_dtoa(s, snr));
    (void)printf("[%s,", pvn_dtoa(s, snr));
    (void)printf("%s]\n", pvn_dtoa(s, cs));
    (void)printf("V^T A V =\n");
    pvn_qmm2(&(c[0][0]), &(c[1][0]), &(c[0][1]), &(c[1][1]), cs, snr, snr, cs, a11, a21r, a21r, a22);
    pvn_qmm2(&(c[0][0]), &(c[1][0]), &(c[0][1]), &(c[1][1]), c[0][0], c[1][0], c[0][1], c[1][1], cs, snr, snr, cs);
    (void)printf("[%s,", pvn_qtoa(s, c[0][0]));
    (void)printf("%s]\n", pvn_qtoa(s, c[0][1]));
    (void)printf("[%s,", pvn_qtoa(s, c[1][0]));
    (void)printf("%s]\n", pvn_qtoa(s, c[1][1]));
    c[0][0] = -1.0;
    c[0][1] =  0.0;
    c[1][0] =  0.0;
    c[1][1] =  1.0;
    (void)printf("V^T J V - J =\n");
    pvn_qmma2(&(c[0][0]), &(c[1][0]), &(c[0][1]), &(c[1][1]), cs, snr, -snr, -cs, cs, snr, snr, cs);
    (void)printf("[%s,", pvn_qtoa(s, c[0][0]));
    (void)printf("%s]\n", pvn_qtoa(s, c[0][1]));
    (void)printf("[%s,", pvn_qtoa(s, c[1][0]));
    (void)printf("%s]\n", pvn_qtoa(s, c[1][1]));
  }
  else {
#ifdef PVN_QUADMATH
    __float128 c[2][4];
#else /* !PVN_QUADMATH */
    long double c[2][4];
#endif /* ?PVN_QUADMATH */
    (void)printf("A =\n");
    (void)printf("[(%s,", pvn_dtoa(s, a11));
    (void)printf("%s),", pvn_dtoa(s, 0.0));
    (void)printf("(%s,", pvn_dtoa(s, a21r));
    (void)printf("%s)]\n", pvn_dtoa(s, -a21i));
    (void)printf("[(%s,", pvn_dtoa(s, a21r));
    (void)printf("%s),", pvn_dtoa(s, a21i));
    (void)printf("(%s,", pvn_dtoa(s, a22));
    (void)printf("%s)]\n", pvn_dtoa(s, 0.0));
    (void)printf("V =\n");
    (void)printf("[(%s,", pvn_dtoa(s, cs));
    (void)printf("%s),", pvn_dtoa(s, 0.0));
    (void)printf("(%s,", pvn_dtoa(s, snr));
    (void)printf("%s)]\n", pvn_dtoa(s, -sni));
    (void)printf("[(%s,", pvn_dtoa(s, snr));
    (void)printf("%s),", pvn_dtoa(s, sni));
    (void)printf("(%s,", pvn_dtoa(s, cs));
    (void)printf("%s)]\n", pvn_dtoa(s, 0.0));
    pvn_ymm2(&(c[0][0]), &(c[0][1]), &(c[1][0]), &(c[1][1]), &(c[0][2]), &(c[0][3]), &(c[1][2]), &(c[1][3]), cs, 0.0, snr, sni, snr, -sni, cs, 0.0, a11, 0.0, a21r, a21i, a21r, -a21i, a22, 0.0);
    pvn_ymm2(&(c[0][0]), &(c[0][1]), &(c[1][0]), &(c[1][1]), &(c[0][2]), &(c[0][3]), &(c[1][2]), &(c[1][3]), c[0][0], c[0][1], c[1][0], c[1][1], c[0][2], c[0][3], c[1][2], c[1][3], cs, 0.0, snr, sni, snr, -sni, cs, 0.0);
    (void)printf("V^H A V =\n");
    (void)printf("[(%s,", pvn_qtoa(s, c[0][0]));
    (void)printf("%s),", pvn_qtoa(s, c[0][1]));
    (void)printf("(%s,", pvn_qtoa(s, c[0][2]));
    (void)printf("%s)]\n", pvn_qtoa(s, c[0][3]));
    (void)printf("[(%s,", pvn_qtoa(s, c[1][0]));
    (void)printf("%s),", pvn_qtoa(s, c[1][1]));
    (void)printf("(%s,", pvn_qtoa(s, c[1][2]));
    (void)printf("%s)]\n", pvn_qtoa(s, c[1][3]));
    c[0][0] = -1.0;
    c[0][1] =  0.0;
    c[0][2] =  0.0;
    c[0][3] =  0.0;
    c[1][0] =  0.0;
    c[1][1] =  0.0;
    c[1][2] =  1.0;
    c[1][3] =  0.0;
    pvn_ymma2(&(c[0][0]), &(c[0][1]), &(c[1][0]), &(c[1][1]), &(c[0][2]), &(c[0][3]), &(c[1][2]), &(c[1][3]), cs, 0.0, snr, sni, -snr, sni, -cs, -0.0, cs, 0.0, snr, sni, snr, -sni, cs, 0.0);
    (void)printf("V^H J V - J =\n");
    (void)printf("[(%s,", pvn_qtoa(s, c[0][0]));
    (void)printf("%s),", pvn_qtoa(s, c[0][1]));
    (void)printf("(%s,", pvn_qtoa(s, c[0][2]));
    (void)printf("%s)]\n", pvn_qtoa(s, c[0][3]));
    (void)printf("[(%s,", pvn_qtoa(s, c[1][0]));
    (void)printf("%s),", pvn_qtoa(s, c[1][1]));
    (void)printf("(%s,", pvn_qtoa(s, c[1][2]));
    (void)printf("%s)]\n", pvn_qtoa(s, c[1][3]));
  }
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_sljv2_(const float *const a11, const float *const a22, const float *const a21, float *const cs, float *const sn, int *const es)
{
  PVN_ASSERT(a11);
  PVN_ASSERT(a22);
  PVN_ASSERT(a21);
  PVN_ASSERT(cs);
  PVN_ASSERT(sn);
  PVN_ASSERT(es);
  float a1 = *a11;
  if (!isfinite(a1))
    return -1;
  if (a1 < 0.0f)
    return -1;
  float a2 = *a22;
  if (!isfinite(a2))
    return -2;
  if (a2 < 0.0f)
    return -2;
  float ar = *a21;
  if (!isfinite(ar))
    return -3;
  const int wt = (*es ? 1 : 0);
  int
    e1 = (a1 != 0.0f),
    e2 = (a2 != 0.0f),
    er = (ar != 0.0f);
  float t1 = 0.0f;
  if (er) {
    *es = (e1 | (e2 << 1) | (er << 2));
    if (*es) {
      (void)frexpf(fmaxf(a1, FLT_TRUE_MIN), &e1);
      (void)frexpf(fmaxf(a2, FLT_TRUE_MIN), &e2);
      (void)frexpf(fmaxf(fabsf(ar), FLT_TRUE_MIN), &er);
      e1 = pvn_imax3(e1, e2, er);
      e2 = *es;
      *es = (FLT_BIG_EXP - e1);
      a1 = scalbnf(a1, *es);
      a2 = scalbnf(a2, *es);
      ar = scalbnf(ar, *es);
      er = (ar != 0.0f);
      *es = -*es;
    }
    if (er) {
      const float
        aa = fabsf(ar);
      /* a non-zero element underflows due to scaling */
      e1 = ((((e2 & 1) && (a1 < FLT_MIN)) || ((e2 & 2) && (a2 < FLT_MIN)) || ((e2 & 4) && (aa < FLT_MIN))) << 1);
      ar = copysignf(1.0f, ar);
      const float
        an = (-2.0f * aa),
        ad = (a1 + a2),
        t2 = (an / ad);
      if (t2 <= -1.0f) {
        *cs = t2;
        return -7;
      }
      t1 = (t2 / (1.0f + sqrtf(fmaf(-t2, t2, 1.0f))));
      /* tangent underflows */
      e2 = ((fabsf(t1) < FLT_MIN) << 2);
    }
    else
      e2 = e1 = 0;
  }
  else
    *es = e2 = e1 = 0;
  *cs = rsqrtf(fmaf(-t1, t1, 1.0f));
  const float s1 = (wt ? t1 : (*cs * t1));
  *sn = (ar * s1);
  /* sine/tangent underflows with a non-zero aa */
  er = ((er && (fabsf(*sn) < FLT_MIN)) << 3);
  return (wt | e1 | e2 | er);
}

int pvn_dljv2_(const double *const a11, const double *const a22, const double *const a21, double *const cs, double *const sn, int *const es)
{
  PVN_ASSERT(a11);
  PVN_ASSERT(a22);
  PVN_ASSERT(a21);
  PVN_ASSERT(cs);
  PVN_ASSERT(sn);
  PVN_ASSERT(es);
  double a1 = *a11;
  if (!isfinite(a1))
    return -1;
  if (a1 < 0.0)
    return -1;
  double a2 = *a22;
  if (!isfinite(a2))
    return -2;
  if (a2 < 0.0)
    return -2;
  double ar = *a21;
  if (!isfinite(ar))
    return -3;
  const int wt = (*es ? 1 : 0);
  int
    e1 = (a1 != 0.0),
    e2 = (a2 != 0.0),
    er = (ar != 0.0);
  double t1 = 0.0;
  if (er) {
    *es = (e1 | (e2 << 1) | (er << 2));
    if (*es) {
      (void)frexp(fmax(a1, DBL_TRUE_MIN), &e1);
      (void)frexp(fmax(a2, DBL_TRUE_MIN), &e2);
      (void)frexp(fmax(fabs(ar), DBL_TRUE_MIN), &er);
      e1 = pvn_imax3(e1, e2, er);
      e2 = *es;
      *es = (DBL_BIG_EXP - e1);
      a1 = scalbn(a1, *es);
      a2 = scalbn(a2, *es);
      ar = scalbn(ar, *es);
      er = (ar != 0.0);
      *es = -*es;
    }
    if (er) {
      const double
        aa = fabs(ar);
      /* a non-zero element underflows due to scaling */
      e1 = ((((e2 & 1) && (a1 < DBL_MIN)) || ((e2 & 2) && (a2 < DBL_MIN)) || ((e2 & 4) && (aa < DBL_MIN))) << 1);
      ar = copysign(1.0, ar);
      const double
        an = (-2.0 * aa),
        ad = (a1 + a2),
        t2 = (an / ad);
      if (t2 <= -1.0) {
        *cs = t2;
        return -7;
      }
      t1 = (t2 / (1.0 + sqrt(fma(-t2, t2, 1.0))));
      /* tangent underflows */
      e2 = ((fabs(t1) < DBL_MIN) << 2);
    }
    else
      e2 = e1 = 0;
  }
  else
    *es = e2 = e1 = 0;
  *cs = rsqrt(fma(-t1, t1, 1.0));
  const double s1 = (wt ? t1 : (*cs * t1));
  *sn = (ar * s1);
  /* sine/tangent underflows with a non-zero aa */
  er = ((er && (fabs(*sn) < DBL_MIN)) << 3);
  return (wt | e1 | e2 | er);
}

int pvn_cljv2_(const float *const a11, const float *const a22, const float *const a21r, const float *const a21i, float *const cs, float *const snr, float *const sni, int *const es)
{
  PVN_ASSERT(a11);
  PVN_ASSERT(a22);
  PVN_ASSERT(a21r);
  PVN_ASSERT(a21i);
  PVN_ASSERT(cs);
  PVN_ASSERT(snr);
  PVN_ASSERT(sni);
  PVN_ASSERT(es);
  float a1 = *a11;
  if (!isfinite(a1))
    return -1;
  if (a1 < 0.0f)
    return -1;
  float a2 = *a22;
  if (!isfinite(a2))
    return -2;
  if (a2 < 0.0f)
    return -2;
  float ar = *a21r;
  if (!isfinite(ar))
    return -3;
  float ai = *a21i;
  if (!isfinite(ai))
    return -4;
  const int wt = (*es ? 1 : 0);
  int
    e1 = (a1 != 0.0f),
    e2 = (a2 != 0.0f),
    er = (ar != 0.0f),
    ei = (ai != 0.0f);
  float t1 = 0.0f;
  if (er || ei) {
    *es = (e1 | (e2 << 1) | (er << 2) | (ei << 3));
    if (*es) {
      (void)frexpf(fmaxf(a1, FLT_TRUE_MIN), &e1);
      (void)frexpf(fmaxf(a2, FLT_TRUE_MIN), &e2);
      (void)frexpf(fmaxf(fabsf(ar), FLT_TRUE_MIN), &er);
      (void)frexpf(fmaxf(fabsf(ai), FLT_TRUE_MIN), &ei);
      e1 = pvn_imax4(e1, e2, er, ei);
      e2 = *es;
      *es = (FLT_BIG_EXP - e1);
      a1 = scalbnf(a1, *es);
      a2 = scalbnf(a2, *es);
      ar = scalbnf(ar, *es);
      ai = scalbnf(ai, *es);
      er = (ar != 0.0f);
      ei = (ai != 0.0f);
      *es = -*es;
    }
    if (er || ei) {
      const float
        ar_ = fabsf(ar),
        ai_ = fabsf(ai),
        aa = hypotf(ar_, ai_);
      /* a non-zero element underflows due to scaling */
      e1 = ((((e2 & 1) && (a1 < FLT_MIN)) || ((e2 & 2) && (a2 < FLT_MIN)) || ((e2 & 4) && (ar_ < FLT_MIN)) || ((e2 & 8) && (ai_ < FLT_MIN))) << 1);
      ar = copysignf(fminf((ar_ / aa), 1.0f), ar);
      ai = (ai / fmaxf(aa, FLT_TRUE_MIN));
      const float
        an = (-2.0f * aa),
        ad = (a1 + a2),
        t2 = (an / ad);
      if (t2 <= -1.0f) {
        *cs = t2;
        return -9;
      }
      t1 = (t2 / (1.0f + sqrtf(fmaf(-t2, t2, 1.0f))));
      /* tangent underflows */
      e2 = ((fabsf(t1) < FLT_MIN) << 2);
    }
    else
      e2 = e1 = 0;
  }
  else
    *es = e2 = e1 = 0;
  *cs = rsqrtf(fmaf(-t1, t1, 1.0f));
  const float s1 = (wt ? t1 : (*cs * t1));
  *snr = (ar * s1);
  *sni = (ai * s1);
  /* sine/tangent underflows with a non-zero aa */
  er = ((er && (fabsf(*snr) < FLT_MIN)) << 3);
  ei = ((ei && (fabsf(*sni) < FLT_MIN)) << 4);
  return (wt | e1 | e2 | er | ei);
}

int pvn_zljv2_(const double *const a11, const double *const a22, const double *const a21r, const double *const a21i, double *const cs, double *const snr, double *const sni, int *const es)
{
  PVN_ASSERT(a11);
  PVN_ASSERT(a22);
  PVN_ASSERT(a21r);
  PVN_ASSERT(a21i);
  PVN_ASSERT(cs);
  PVN_ASSERT(snr);
  PVN_ASSERT(sni);
  PVN_ASSERT(es);
  double a1 = *a11;
  if (!isfinite(a1))
    return -1;
  if (a1 < 0.0)
    return -1;
  double a2 = *a22;
  if (!isfinite(a2))
    return -2;
  if (a2 < 0.0)
    return -2;
  double ar = *a21r;
  if (!isfinite(ar))
    return -3;
  double ai = *a21i;
  if (!isfinite(ai))
    return -4;
  const int wt = (*es ? 1 : 0);
  int
    e1 = (a1 != 0.0),
    e2 = (a2 != 0.0),
    er = (ar != 0.0),
    ei = (ai != 0.0);
  double t1 = 0.0;
  if (er || ei) {
    *es = (e1 | (e2 << 1) | (er << 2) | (ei << 3));
    if (*es) {
      (void)frexp(fmax(a1, DBL_TRUE_MIN), &e1);
      (void)frexp(fmax(a2, DBL_TRUE_MIN), &e2);
      (void)frexp(fmax(fabs(ar), DBL_TRUE_MIN), &er);
      (void)frexp(fmax(fabs(ai), DBL_TRUE_MIN), &ei);
      e1 = pvn_imax4(e1, e2, er, ei);
      e2 = *es;
      *es = (DBL_BIG_EXP - e1);
      a1 = scalbn(a1, *es);
      a2 = scalbn(a2, *es);
      ar = scalbn(ar, *es);
      ai = scalbn(ai, *es);
      er = (ar != 0.0);
      ei = (ai != 0.0);
      *es = -*es;
    }
    if (er || ei) {
      const double
        ar_ = fabs(ar),
        ai_ = fabs(ai),
        aa = hypot(ar_, ai_);
      /* a non-zero element underflows due to scaling */
      e1 = ((((e2 & 1) && (a1 < DBL_MIN)) || ((e2 & 2) && (a2 < DBL_MIN)) || ((e2 & 4) && (ar_ < DBL_MIN)) || ((e2 & 8) && (ai_ < DBL_MIN))) << 1);
      ar = copysign(fmin((ar_ / aa), 1.0), ar);
      ai = (ai / fmax(aa, DBL_TRUE_MIN));
      const double
        an = (-2.0 * aa),
        ad = (a1 + a2),
        t2 = (an / ad);
      if (t2 <= -1.0) {
        *cs = t2;
        return -9;
      }
      t1 = (t2 / (1.0 + sqrt(fma(-t2, t2, 1.0))));
      /* tangent underflows */
      e2 = ((fabs(t1) < DBL_MIN) << 2);
    }
    else
      e2 = e1 = 0;
  }
  else
    *es = e2 = e1 = 0;
  *cs = rsqrt(fma(-t1, t1, 1.0));
  const double s1 = (wt ? t1 : (*cs * t1));
  *snr = (ar * s1);
  *sni = (ai * s1);
  /* sine/tangent underflows with a non-zero aa */
  er = ((er && (fabs(*snr) < DBL_MIN)) << 3);
  ei = ((ei && (fabs(*sni) < DBL_MIN)) << 4);
  return (wt | e1 | e2 | er | ei);
}

int pvn_xljv2_(const long double *const a11, const long double *const a22, const long double *const a21, long double *const cs, long double *const sn, int *const es)
{
  PVN_ASSERT(a11);
  PVN_ASSERT(a22);
  PVN_ASSERT(a21);
  PVN_ASSERT(cs);
  PVN_ASSERT(sn);
  PVN_ASSERT(es);
  long double a1 = *a11;
  if (!isfinite(a1))
    return -1;
  if (a1 < 0.0L)
    return -1;
  long double a2 = *a22;
  if (!isfinite(a2))
    return -2;
  if (a2 < 0.0L)
    return -2;
  long double ar = *a21;
  if (!isfinite(ar))
    return -3;
  const int wt = (*es ? 1 : 0);
  int
    e1 = (a1 != 0.0L),
    e2 = (a2 != 0.0L),
    er = (ar != 0.0L);
  long double t1 = 0.0L;
  if (er) {
    *es = (e1 | (e2 << 1) | (er << 2));
    if (*es) {
      (void)frexpl(fmaxl(a1, LDBL_TRUE_MIN), &e1);
      (void)frexpl(fmaxl(a2, LDBL_TRUE_MIN), &e2);
      (void)frexpl(fmaxl(fabsl(ar), LDBL_TRUE_MIN), &er);
      e1 = pvn_imax3(e1, e2, er);
      e2 = *es;
      *es = (LDBL_BIG_EXP - e1);
      a1 = scalbnl(a1, *es);
      a2 = scalbnl(a2, *es);
      ar = scalbnl(ar, *es);
      er = (ar != 0.0L);
      *es = -*es;
    }
    if (er) {
      const long double
        aa = fabsl(ar);
      /* a non-zero element underflows due to scaling */
      e1 = ((((e2 & 1) && (a1 < LDBL_MIN)) || ((e2 & 2) && (a2 < LDBL_MIN)) || ((e2 & 4) && (aa < LDBL_MIN))) << 1);
      ar = copysignl(1.0L, ar);
      const long double
        an = (-2.0L * aa),
        ad = (a1 + a2),
        t2 = (an / ad);
      if (t2 <= -1.0L) {
        *cs = t2;
        return -7;
      }
      t1 = (t2 / (1.0L + sqrtl(fmal(-t2, t2, 1.0L))));
      /* tangent underflows */
      e2 = ((fabsl(t1) < LDBL_MIN) << 2);
    }
    else
      e2 = e1 = 0;
  }
  else
    *es = e2 = e1 = 0;
  *cs = rsqrtl(fmal(-t1, t1, 1.0L));
  const long double s1 = (wt ? t1 : (*cs * t1));
  *sn = (ar * s1);
  /* sine/tangent underflows with a non-zero aa */
  er = ((er && (fabsl(*sn) < LDBL_MIN)) << 3);
  return (wt | e1 | e2 | er);
}

int pvn_wljv2_(const long double *const a11, const long double *const a22, const long double *const a21r, const long double *const a21i, long double *const cs, long double *const snr, long double *const sni, int *const es)
{
  PVN_ASSERT(a11);
  PVN_ASSERT(a22);
  PVN_ASSERT(a21r);
  PVN_ASSERT(a21i);
  PVN_ASSERT(cs);
  PVN_ASSERT(snr);
  PVN_ASSERT(sni);
  PVN_ASSERT(es);
  long double a1 = *a11;
  if (!isfinite(a1))
    return -1;
  if (a1 < 0.0L)
    return -1;
  long double a2 = *a22;
  if (!isfinite(a2))
    return -2;
  if (a2 < 0.0L)
    return -2;
  long double ar = *a21r;
  if (!isfinite(ar))
    return -3;
  long double ai = *a21i;
  if (!isfinite(ai))
    return -4;
  const int wt = (*es ? 1 : 0);
  int
    e1 = (a1 != 0.0L),
    e2 = (a2 != 0.0L),
    er = (ar != 0.0L),
    ei = (ai != 0.0L);
  long double t1 = 0.0L;
  if (er || ei) {
    *es = (e1 | (e2 << 1) | (er << 2) | (ei << 3));
    if (*es) {
      (void)frexpl(fmaxl(a1, LDBL_TRUE_MIN), &e1);
      (void)frexpl(fmaxl(a2, LDBL_TRUE_MIN), &e2);
      (void)frexpl(fmaxl(fabsl(ar), LDBL_TRUE_MIN), &er);
      (void)frexpl(fmaxl(fabsl(ai), LDBL_TRUE_MIN), &ei);
      e1 = pvn_imax4(e1, e2, er, ei);
      e2 = *es;
      *es = (LDBL_BIG_EXP - e1);
      a1 = scalbnl(a1, *es);
      a2 = scalbnl(a2, *es);
      ar = scalbnl(ar, *es);
      ai = scalbnl(ai, *es);
      er = (ar != 0.0L);
      ei = (ai != 0.0L);
      *es = -*es;
    }
    if (er || ei) {
      const long double
        ar_ = fabsl(ar),
        ai_ = fabsl(ai),
        aa = hypotl(ar_, ai_);
      /* a non-zero element underflows due to scaling */
      e1 = ((((e2 & 1) && (a1 < LDBL_MIN)) || ((e2 & 2) && (a2 < LDBL_MIN)) || ((e2 & 4) && (ar_ < LDBL_MIN)) || ((e2 & 8) && (ai_ < LDBL_MIN))) << 1);
      ar = copysignl(fminl((ar_ / aa), 1.0L), ar);
      ai = (ai / fmaxl(aa, LDBL_TRUE_MIN));
      const long double
        an = (-2.0L * aa),
        ad = (a1 + a2),
        t2 = (an / ad);
      if (t2 <= -1.0L) {
        *cs = t2;
        return -9;
      }
      t1 = (t2 / (1.0L + sqrtl(fmal(-t2, t2, 1.0L))));
      /* tangent underflows */
      e2 = ((fabsl(t1) < LDBL_MIN) << 2);
    }
    else
      e2 = e1 = 0;
  }
  else
    *es = e2 = e1 = 0;
  *cs = rsqrtl(fmal(-t1, t1, 1.0L));
  const long double s1 = (wt ? t1 : (*cs * t1));
  *snr = (ar * s1);
  *sni = (ai * s1);
  /* sine/tangent underflows with a non-zero aa */
  er = ((er && (fabsl(*snr) < LDBL_MIN)) << 3);
  ei = ((ei && (fabsl(*sni) < LDBL_MIN)) << 4);
  return (wt | e1 | e2 | er | ei);
}

#ifdef PVN_QUADMATH
#ifdef FLT128_BIG_EXP
#error FLT128_BIG_EXP already defined
#else /* !FLT128_BIG_EXP */
#define FLT128_BIG_EXP (FLT128_MAX_EXP - 3)
#endif /* ?FLT128_BIG_EXP */

int pvn_qljv2_(const __float128 *const a11, const __float128 *const a22, const __float128 *const a21, __float128 *const cs, __float128 *const sn, int *const es)
{
  PVN_ASSERT(a11);
  PVN_ASSERT(a22);
  PVN_ASSERT(a21);
  PVN_ASSERT(cs);
  PVN_ASSERT(sn);
  PVN_ASSERT(es);
  __float128 a1 = *a11;
  if (!isfiniteq(a1))
    return -1;
  if (a1 < 0.0q)
    return -1;
  __float128 a2 = *a22;
  if (!isfiniteq(a2))
    return -2;
  if (a2 < 0.0q)
    return -2;
  __float128 ar = *a21;
  if (!isfiniteq(ar))
    return -3;
  const int wt = (*es ? 1 : 0);
  int
    e1 = (a1 != 0.0q),
    e2 = (a2 != 0.0q),
    er = (ar != 0.0q);
  __float128 t1 = 0.0q;
  if (er) {
    *es = (e1 | (e2 << 1) | (er << 2));
    if (*es) {
      (void)frexpq(fmaxq(a1, FLT128_TRUE_MIN), &e1);
      (void)frexpq(fmaxq(a2, FLT128_TRUE_MIN), &e2);
      (void)frexpq(fmaxq(fabsq(ar), FLT128_TRUE_MIN), &er);
      e1 = pvn_imax3(e1, e2, er);
      e2 = *es;
      *es = (FLT128_BIG_EXP - e1);
      a1 = scalbnq(a1, *es);
      a2 = scalbnq(a2, *es);
      ar = scalbnq(ar, *es);
      er = (ar != 0.0q);
      *es = -*es;
    }
    if (er) {
      const __float128
        aa = fabsq(ar);
      /* a non-zero element underflows due to scaling */
      e1 = ((((e2 & 1) && (a1 < FLT128_MIN)) || ((e2 & 2) && (a2 < FLT128_MIN)) || ((e2 & 4) && (aa < FLT128_MIN))) << 1);
      ar = copysignq(1.0q, ar);
      const __float128
        an = (-2.0q * aa),
        ad = (a1 + a2),
        t2 = (an / ad);
      if (t2 <= -1.0q) {
        *cs = t2;
        return -7;
      }
      t1 = (t2 / (1.0q + sqrtq(fmaq(-t2, t2, 1.0q))));
      /* tangent underflows */
      e2 = ((fabsq(t1) < FLT128_MIN) << 2);
    }
    else
      e2 = e1 = 0;
  }
  else
    *es = e2 = e1 = 0;
  *cs = rsqrtq(fmaq(-t1, t1, 1.0q));
  const __float128 s1 = (wt ? t1 : (*cs * t1));
  *sn = (ar * s1);
  /* sine/tangent underflows with a non-zero aa */
  er = ((er && (fabsq(*sn) < FLT128_MIN)) << 3);
  return (wt | e1 | e2 | er);
}

int pvn_yljv2_(const __float128 *const a11, const __float128 *const a22, const __float128 *const a21r, const __float128 *const a21i, __float128 *const cs, __float128 *const snr, __float128 *const sni, int *const es)
{
  PVN_ASSERT(a11);
  PVN_ASSERT(a22);
  PVN_ASSERT(a21r);
  PVN_ASSERT(a21i);
  PVN_ASSERT(cs);
  PVN_ASSERT(snr);
  PVN_ASSERT(sni);
  PVN_ASSERT(es);
  __float128 a1 = *a11;
  if (!isfiniteq(a1))
    return -1;
  if (a1 < 0.0q)
    return -1;
  __float128 a2 = *a22;
  if (!isfiniteq(a2))
    return -2;
  if (a2 < 0.0q)
    return -2;
  __float128 ar = *a21r;
  if (!isfiniteq(ar))
    return -3;
  __float128 ai = *a21i;
  if (!isfiniteq(ai))
    return -4;
  const int wt = (*es ? 1 : 0);
  int
    e1 = (a1 != 0.0q),
    e2 = (a2 != 0.0q),
    er = (ar != 0.0q),
    ei = (ai != 0.0q);
  __float128 t1 = 0.0q;
  if (er || ei) {
    *es = (e1 | (e2 << 1) | (er << 2) | (ei << 3));
    if (*es) {
      (void)frexpq(fmaxq(a1, FLT128_TRUE_MIN), &e1);
      (void)frexpq(fmaxq(a2, FLT128_TRUE_MIN), &e2);
      (void)frexpq(fmaxq(fabsq(ar), FLT128_TRUE_MIN), &er);
      (void)frexpq(fmaxq(fabsq(ai), FLT128_TRUE_MIN), &ei);
      e1 = pvn_imax4(e1, e2, er, ei);
      e2 = *es;
      *es = (FLT128_BIG_EXP - e1);
      a1 = scalbnq(a1, *es);
      a2 = scalbnq(a2, *es);
      ar = scalbnq(ar, *es);
      ai = scalbnq(ai, *es);
      er = (ar != 0.0q);
      ei = (ai != 0.0q);
      *es = -*es;
    }
    if (er || ei) {
      const __float128
        ar_ = fabsq(ar),
        ai_ = fabsq(ai),
        aa = hypotq(ar_, ai_);
      /* a non-zero element underflows due to scaling */
      e1 = ((((e2 & 1) && (a1 < FLT128_MIN)) || ((e2 & 2) && (a2 < FLT128_MIN)) || ((e2 & 4) && (ar_ < FLT128_MIN)) || ((e2 & 8) && (ai_ < FLT128_MIN))) << 1);
      ar = copysignq(fminq((ar_ / aa), 1.0q), ar);
      ai = (ai / fmaxq(aa, FLT128_TRUE_MIN));
      const __float128
        an = (-2.0q * aa),
        ad = (a1 + a2),
        t2 = (an / ad);
      if (t2 <= -1.0q) {
        *cs = t2;
        return -9;
      }
      t1 = (t2 / (1.0q + sqrtq(fmaq(-t2, t2, 1.0q))));
      /* tangent underflows */
      e2 = ((fabsq(t1) < FLT128_MIN) << 2);
    }
    else
      e2 = e1 = 0;
  }
  else
    *es = e2 = e1 = 0;
  *cs = rsqrtq(fmaq(-t1, t1, 1.0q));
  const __float128 s1 = (wt ? t1 : (*cs * t1));
  *snr = (ar * s1);
  *sni = (ai * s1);
  /* sine/tangent underflows with a non-zero aa */
  er = ((er && (fabsq(*snr) < FLT128_MIN)) << 3);
  ei = ((ei && (fabsq(*sni) < FLT128_MIN)) << 4);
  return (wt | e1 | e2 | er | ei);
}
#else /* !PVN_QUADMATH */
int pvn_qljv2_(const long double *const a11, const long double *const a22, const long double *const a21, long double *const cs, long double *const sn, int *const es)
{
  return pvn_xljv2_(a11, a22, a21, cs, sn, es);
}

int pvn_yljv2_(const long double *const a11, const long double *const a22, const long double *const a21r, const long double *const a21i, long double *const cs, long double *const snr, long double *const sni, int *const es)
{
  return pvn_wljv2_(a11, a22, a21r, a21i, cs, snr, sni, es);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if ((argc != 3) && (argc != 6)) {
    (void)fprintf(stderr, "%s (S|D) (N | a11 a21 a12 a22)\n", *argv);
    return EXIT_FAILURE;
  }
  if (argc == 6) {
    if (toupper(*(argv[1])) == 'S') {
      const float a11 = (float)atof(argv[2]);
      const float a21 = (float)atof(argv[3]);
      const float a12 = (float)atof(argv[4]);
      const float a22 = (float)atof(argv[5]);
      float u11 = -0.0f, u21 = -0.0f, u12 = -0.0f, u22 = -0.0f, v11 = -0.0f, v21 = -0.0f, v12 = -0.0f, v22 = -0.0f, s1 = -0.0f, s2 = -0.0f;
      int es[3] = { 0, 0, 0 };
      const int knd =
#ifdef _WIN32
        PVN_SLJSV2
#else /* !_WIN32 */
        pvn_sljsv2_
#endif /* ?_WIN32 */
        (&a11, &a21, &a12, &a22, &u11, &u21, &u12, &u22, &v11, &v21, &v12, &v22, &s1, &s2, es);
      (void)printf("knd=%d, es={%d,%d,%d}\n\ta =\n", knd, es[0], es[1], es[2]);
      char s[17] = { '\0' };
      (void)printf("%s ", pvn_stoa(s, a11));
      (void)printf("%s\n", pvn_stoa(s, a12));
      (void)printf("%s ", pvn_stoa(s, a21));
      (void)printf("%s\n\tu =\n", pvn_stoa(s, a22));
      (void)printf("%s ", pvn_stoa(s, u11));
      (void)printf("%s\n", pvn_stoa(s, u12));
      (void)printf("%s ", pvn_stoa(s, u21));
      (void)printf("%s\n\tv =\n", pvn_stoa(s, u22));
      (void)printf("%s ", pvn_stoa(s, v11));
      (void)printf("%s\n", pvn_stoa(s, v12));
      (void)printf("%s ", pvn_stoa(s, v21));
      (void)printf("%s\n\ts =\n", pvn_stoa(s, v22));
      (void)printf("%s ", pvn_stoa(s, s1));
      (void)printf("%s\n\tS =\n", pvn_stoa(s, s2));
      s1 = scalbnf(s1, (es[1] - es[0]));
      s2 = scalbnf(s2, (es[2] - es[0]));
      (void)printf("%s ", pvn_stoa(s, s1));
      (void)printf("%s\n", pvn_stoa(s, s2));
    }
    else if (toupper(*(argv[1])) == 'D') {
      const double a11 = atof(argv[2]);
      const double a21 = atof(argv[3]);
      const double a12 = atof(argv[4]);
      const double a22 = atof(argv[5]);
      double u11 = -0.0, u21 = -0.0, u12 = -0.0, u22 = -0.0, v11 = -0.0, v21 = -0.0, v12 = -0.0, v22 = -0.0, s1 = -0.0, s2 = -0.0;
      int es[3] = { 0, 0, 0 };
      const int knd =
#ifdef _WIN32
        PVN_DLJSV2
#else /* !_WIN32 */
        pvn_dljsv2_
#endif /* ?_WIN32 */
        (&a11, &a21, &a12, &a22, &u11, &u21, &u12, &u22, &v11, &v21, &v12, &v22, &s1, &s2, es);
      (void)printf("knd=%d, es={%d,%d,%d}\n\ta =\n", knd, es[0], es[1], es[2]);
      char s[26] = { '\0' };
      (void)printf("%s ", pvn_dtoa(s, a11));
      (void)printf("%s\n", pvn_dtoa(s, a12));
      (void)printf("%s ", pvn_dtoa(s, a21));
      (void)printf("%s\n\tu =\n", pvn_dtoa(s, a22));
      (void)printf("%s ", pvn_dtoa(s, u11));
      (void)printf("%s\n", pvn_dtoa(s, u12));
      (void)printf("%s ", pvn_dtoa(s, u21));
      (void)printf("%s\n\tv =\n", pvn_dtoa(s, u22));
      (void)printf("%s ", pvn_dtoa(s, v11));
      (void)printf("%s\n", pvn_dtoa(s, v12));
      (void)printf("%s ", pvn_dtoa(s, v21));
      (void)printf("%s\n\ts =\n", pvn_dtoa(s, v22));
      (void)printf("%s ", pvn_dtoa(s, s1));
      (void)printf("%s\n\tS =\n", pvn_dtoa(s, s2));
      s1 = scalbnf(s1, (es[1] - es[0]));
      s2 = scalbnf(s2, (es[2] - es[0]));
      (void)printf("%s ", pvn_dtoa(s, s1));
      (void)printf("%s\n", pvn_dtoa(s, s2));
    }
    else
      return EXIT_FAILURE;
  }
#ifdef _WIN32
  (void)fprintf(stderr, "NOT IMPLEMENTED YET!\n");
#else /* !_WIN32 */
  else {
    int n = atoi(argv[2]);
    if (!n)
      return EXIT_SUCCESS;
    const int upper = (n < 0);
    n = abs(n);
    int u = pvn_ran_open_();
    if (u < 0) {
      (void)fprintf(stderr, "open(/dev/random): %d\n", u);
      return EXIT_FAILURE;
    }
#ifdef __x86_64__
    char s[31] = { '\0' };
#else /* !__x86_64__ */
    char s[46] = { '\0' };
#endif /* ?__x86_64__ */
    long double EC = 0.0L, EU = 0.0L, EV = 0.0L, EG = 0.0L;
    if (toupper(*(argv[1])) == 'S') {
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(n,s,u,upper) reduction(max:EC,EU,EV,EG)
#endif /* _OPENMP */
      for (int i = 0u; i < n; ++i) {
        const float a11 = pvn_ran_safe_f_(&u);
        if (!(a11 != 0.0f))
          PVN_STOP("G(1,1)");
        const float a21 = (upper ? 0.0f : pvn_ran_safe_f_(&u));
        if (!upper && !(a21 != 0.0f))
          PVN_STOP("G(2,1)");
        const float a12 = pvn_ran_safe_f_(&u);
        if (!(a12 != 0.0f))
          PVN_STOP("G(1,2)");
        const float a22 = pvn_ran_safe_f_(&u);
        if (!(a22 != 0.0f))
          PVN_STOP("G(2,2)");
        float u11 = -0.0f, u21 = -0.0f, u12 = -0.0f, u22 = -0.0f, v11 = -0.0f, v21 = -0.0f, v12 = -0.0f, v22 = -0.0f, s1 = -0.0f, s2 = -0.0f;
        int es[3] = { 0, 0, 0 };
        const int knd = pvn_sljsv2_(&a11, &a21, &a12, &a22, &u11, &u21, &u12, &u22, &v11, &v21, &v12, &v22, &s1, &s2, es);
        if ((knd < 0) || ((knd != 13) && (knd != 15)))
          PVN_STOP("pvn_sljsv2_");
        long double E[4] = { 0.0L, 0.0L, 0.0L, 0.0L };
        pvn_sxljr2_(&a11, &a21, &a12, &a22, &u11, &u21, &u12, &u22, &v11, &v21, &v12, &v22, &s1, &s2, es, E);
        EC = fmaxl(EC, E[0]);
#ifndef NDEBUG
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
        {
          (void)printf("%10d: cond_2(G) =%s\n", i, pvn_xtoa(s, E[0]));
        }
#endif /* !NDEBUG */
        EU = fmaxl(EU, E[1]);
#ifndef NDEBUG
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
        {
          (void)printf("%10d: || U^T U - I ||_F =%s\n", i, pvn_xtoa(s, E[1]));
        }
#endif /* !NDEBUG */
        EV = fmaxl(EV, E[2]);
#ifndef NDEBUG
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
        {
          (void)printf("%10d: || V^T V - I ||_F =%s\n", i, pvn_xtoa(s, E[2]));
        }
#endif /* !NDEBUG */
        EG = fmaxl(EG, E[3]);
#ifndef NDEBUG
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
        {
          (void)printf("%10d: || U Σ V^T - G ||_F / || G ||_F =%s\n", i, pvn_xtoa(s, E[3]));
        }
#endif /* !NDEBUG */
      }
    }
    else if (toupper(*(argv[1])) == 'D') {
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(n,s,u,upper) reduction(max:EC,EU,EV,EG)
#endif /* _OPENMP */
      for (int i = 0u; i < n; ++i) {
        const double a11 = pvn_ran_safe_(&u);
        if (!(a11 != 0.0))
          PVN_STOP("G(1,1)");
        const double a21 = (upper ? 0.0 : pvn_ran_safe_(&u));
        if (!upper && !(a21 != 0.0))
          PVN_STOP("G(2,1)");
        const double a12 = pvn_ran_safe_(&u);
        if (!(a12 != 0.0))
          PVN_STOP("G(1,2)");
        const double a22 = pvn_ran_safe_(&u);
        if (!(a22 != 0.0))
          PVN_STOP("G(2,2)");
        double u11 = -0.0, u21 = -0.0, u12 = -0.0, u22 = -0.0, v11 = -0.0, v21 = -0.0, v12 = -0.0, v22 = -0.0, s1 = -0.0, s2 = -0.0;
        int es[3] = { 0, 0, 0 };
        const int knd = pvn_dljsv2_(&a11, &a21, &a12, &a22, &u11, &u21, &u12, &u22, &v11, &v21, &v12, &v22, &s1, &s2, es);
        if ((knd < 0) || ((knd != 13) && (knd != 15)))
          PVN_STOP("pvn_dljsv2_");
        long double E[4] = { 0.0L, 0.0L, 0.0L, 0.0L };
        pvn_dxljr2_(&a11, &a21, &a12, &a22, &u11, &u21, &u12, &u22, &v11, &v21, &v12, &v22, &s1, &s2, es, E);
        EC = fmaxl(EC, E[0]);
#ifndef NDEBUG
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
        {
          (void)printf("%10d: cond_2(G) =%s\n", i, pvn_xtoa(s, E[0]));
        }
#endif /* !NDEBUG */
        EU = fmaxl(EU, E[1]);
#ifndef NDEBUG
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
        {
          (void)printf("%10d: || U^T U - I ||_F =%s\n", i, pvn_xtoa(s, E[1]));
        }
#endif /* !NDEBUG */
        EV = fmaxl(EV, E[2]);
#ifndef NDEBUG
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
        {
          (void)printf("%10d: || V^T V - I ||_F =%s\n", i, pvn_xtoa(s, E[2]));
        }
#endif /* !NDEBUG */
        EG = fmaxl(EG, E[3]);
#ifndef NDEBUG
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
        {
          (void)printf("%10d: || U Σ V^T - G ||_F / || G ||_F =%s\n", i, pvn_xtoa(s, E[3]));
        }
#endif /* !NDEBUG */
      }
    }
    else
      return EXIT_FAILURE;
    u = pvn_ran_close_(&u);
#ifndef NDEBUG
    (void)printf("max(cond_2(G))=");
#endif /* !NDEBUG */
    (void)printf("%s", pvn_xtoa(s, EC));
#ifndef NDEBUG
    (void)printf("\nmax(|| U^T U - I ||_F)=");
#else /* NDEBUG */
    (void)putchar(',');
#endif /* ?NDEBUG */
    (void)printf("%s", pvn_xtoa(s, EU));
#ifndef NDEBUG
    (void)printf("\nmax(|| V^T V - I ||_F)=");
#else /* NDEBUG */
    (void)putchar(',');
#endif /* ?NDEBUG */
    (void)printf("%s", pvn_xtoa(s, EV));
#ifndef NDEBUG
    (void)printf("\nmax(|| U Σ V^T - G ||_F / || G ||_F)=");
#else /* NDEBUG */
    (void)putchar(',');
#endif /* ?NDEBUG */
    (void)printf("%s\n", pvn_xtoa(s, EG));
    if (u != 0) {
      (void)fprintf(stderr, "close(/dev/random): %d\n", u);
      return EXIT_FAILURE;
    }
  }
#endif /* ?_WIN32 */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#pragma message "DO NOT USE THE INCOMPLETE COMPLEX pvn_sv2 ROUTINES YET!!!"

static inline void ef_mulf(int *const e, float *const f, const int e1, const float f1, const int e2, const float f2)
{
  assert(e);
  assert(f);
  *f = (f1 * f2);
  *f = frexpf(*f, e);
  *e += (e1 + e2);
}

static inline void ef_divf(int *const e, float *const f, const int e1, const float f1, const int e2, const float f2)
{
  assert(e);
  assert(f);
  *f = (f1 / f2);
  *f = frexpf(*f, e);
  *e += (e1 - e2);
}

/* f1, f2 >= 0 */
static inline int ef_cmpf(const int e1, const float f1, const int e2, const float f2)
{
  if (f2 == 0.0f)
    return (f1 != 0.0f);
  if (f1 == 0.0f)
    return -1;
  if (e1 < e2)
    return -1;
  if (e2 < e1)
    return 1;
  if (f1 < f2)
    return -1;
  if (f2 < f1)
    return 1;
  return 0;
}

static void slpsv2(const float A11, const float A12, const float A22, float *const tf, float *const cf, float *const sf, float *const tp, float *const cp, float *const sp, float *const s1, float *const s2, int *const es
#ifndef NDEBUG
                   , char *const s
#endif /* !NDEBUG */
                   )
{
  assert(tf);
  assert(cf);
  assert(sf);
  assert(tp);
  assert(cp);
  assert(sp);
  assert(s1);
  assert(s2);
  assert(es);
  int ae = 0, be = 0, abe = 0, a_be = 0, de = 0, ne = 0, t2e = 0;
  float af = 0.0f, bf = 0.0f, abf = 0.0f, a_bf = 0.0f, df = 0.0f, nf = 0.0f, t2f = 0.0f, t2 = 0.0f;

  /* should never overflow */
  const float a = hypotf(A11, A12);
  float b = A22;

  if ((A11 / A12) < (FLT_EPSILON * 0.5f)) {
    t2 = ((2.0f * A22) / A12);
    b = -1.0f;
  }
  else if ((A11 < A12) && (a == A12)) {
    bf = frexpf(b, &be);
    t2 = ((2.0f * A22) / A12);
  }
  else {
    af = frexpf(a, &ae);
    bf = frexpf(b, &be);
    abf = (a + b);
    if (!isfinite(abf)) {
      abf = ((0.5f * a) + (0.5f * b));
      de = 1;
    }
    abf = frexpf(abf, &abe);
    abe += de;

    a_bf = (a - b);
    if (a == b)
      de = 0;
    else if (fabsf(a_bf) >= FLT_MIN) {
      a_bf = frexpf(a_bf, &a_be);
      de = 1;
    }
    else {
      de = ((FLT_MIN_EXP + FLT_MANT_DIG) - pvn_imin(ae, be));
      a_bf = (scalbnf(af, (ae + de)) - scalbnf(bf, (be + de)));
      a_bf = frexpf(a_bf, &a_be);
      a_be -= de;
      de = -1;
    }

    if (de)
      ef_mulf(&de, &df, a_be, a_bf, abe, abf);
    af = frexpf(A12, &ae);
    ef_mulf(&ne, &nf, ae, af, be, bf);
    ++ne;
    ef_divf(&t2e, &t2f, ne, nf, de, df);
    if (isfinite(t2f))
      t2 = scalbnf(t2f, t2e);
    else
      t2 = t2f;
  }

  if (isfinite(t2))
    *tf = (t2 / (1.0f + hypotf(t2, 1.0f)));
  else
    *tf = copysignf(1.0f, t2);
#ifndef NDEBUG
  if (s) (void)printf("tan(φ)=%s, ", pvn_stoa(s, *tf));
#endif /* !NDEBUG */
  *cf = hypotf(*tf, 1.0f);
  *sf = (*tf / *cf);

  *sp = fmaf(*tf, A22, A12);
  *tp = (*sp / A11);
#ifndef NDEBUG
  if (s) (void)printf("tan(ψ)=%s\n", pvn_stoa(s, *tp));
#endif /* !NDEBUG */

  if (b < 0.0f) {
    af = frexpf(A11, &ae);
    bf = frexpf(A22, &be);
    df = frexpf(*sp, &de);
    /* expect to be (A12 + tan(φ) * A22) == A12, but the LHS has already been computed */
    /* s2 ≈ (A11 * A22) / (A12 + tan(φ) * A22) */
    ef_mulf(&ne, &nf, ae, af, be, bf);
    ef_divf(&abe, &abf, ne, nf, de, df);
    /* s1 ≈ (A12 + tan(φ) * A22) */
    *s1 = *sp;
    a_bf = frexpf(*s1, &a_be);
    if (isfinite(*tp)) {
      /* 1 / cos */
      *cp = hypotf(*tp, 1.0f);
      *sp = (*tp / *cp);
      *cp = (1.0f / *cp);
    }
    else {
      ef_divf(&t2e, &t2f, de, df, ae, af);
      /* tan(ψ) so large that sec(ψ) ≈ |tan(ψ)| */
      t2f = fabsf(t2f);
      *sp = copysignf(1.0f, *tp);
      ef_divf(&ne, &nf, 1, 0.5f, t2e, t2f);
      *cp = scalbnf(nf, ne);
    }
  }
  else {
    /* 1 / cos */
    *cp = hypotf(*tp, 1.0f);
    nf = frexpf(*cf, &ne);
    df = frexpf(*cp, &de);
    ef_divf(&ae, &af, ne, nf, de, df);
    /* s2 = A22 * (cf / cp) */
    ef_mulf(&abe, &abf, be, bf, ae, af);
    bf = frexpf(A11, &be);
    /* s1 = A11 * (cp / cf) */
    ef_divf(&a_be, &a_bf, be, bf, ae, af);
    *sp = (*tp / *cp);
    *cp = (1.0f / *cp);
  }

  *cf = (1.0f / *cf);
#ifndef NDEBUG
  if (s) {
    (void)printf("cos(φ)=%s, ", pvn_stoa(s, *cf));
    (void)printf("sin(φ)=%s\n", pvn_stoa(s, *sf));
    (void)printf("cos(ψ)=%s, ", pvn_stoa(s, *cf));
    (void)printf("sin(ψ)=%s\n", pvn_stoa(s, *sf));
  }
#endif /* !NDEBUG */

  *s1 = a_bf;
  es[1] = a_be;
  *s2 = abf;
  es[2] = abe;
}

int
#ifdef _WIN32
PVN_SLJSV2
#else /* !_WIN32 */
pvn_sljsv2_
#endif /* ?_WIN32 */
(const float *const a11, const float *const a21, const float *const a12, const float *const a22,
 float *const u11, float *const u21, float *const u12, float *const u22,
 float *const v11, float *const v21, float *const v12, float *const v22,
 float *const s1, float *const s2, int *const es)
{
  assert(a11);
  assert(a21);
  assert(a12);
  assert(a22);
  assert(u11);
  assert(u21);
  assert(u12);
  assert(u22);
  assert(v11);
  assert(v21);
  assert(v12);
  assert(v22);
  assert(s1);
  assert(s2);
  assert(es);
  if (!isfinite(*a11))
    return -1;
  if (!isfinite(*a21))
    return -2;
  if (!isfinite(*a12))
    return -3;
  if (!isfinite(*a22))
    return -4;
  /* should not fail beyond this point when *es == 0 */
  int knd = 0, mxe = INT_MIN, e = INT_MIN;
  if (*a11 != 0.0f) {
    knd |= 1;
    (void)frexpf(*a11, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21 != 0.0f) {
    knd |= 2;
    (void)frexpf(*a21, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12 != 0.0f) {
    knd |= 4;
    (void)frexpf(*a12, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22 != 0.0f) {
    knd |= 8;
    (void)frexpf(*a22, &e);
    if (mxe < e)
      mxe = e;
  }
  switch (knd) {
  case  0:
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  3:
  case  5:
  case  7:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
    e = (FLT_MAX_EXP - mxe - 1);
    break;
  case 15:
    e = (FLT_MAX_EXP - mxe - 2);
    break;
  default:
    return INT_MIN;
  }
  mxe = *es;
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;
  es[2] = es[1] = 0;

  /* scaling by 2^(*es), can only fail when mxe != 0 */
  float A11 = *a11, A21 = *a21, A12 = *a12, A22 = *a22;
  if (*es) {
    A11 = scalbnf(A11, *es);
    A21 = scalbnf(A21, *es);
    A12 = scalbnf(A12, *es);
    A22 = scalbnf(A22, *es);
    if (mxe) {
      if (!isfinite(A11))
        return -5;
      if (!isfinite(A21))
        return -6;
      if (!isfinite(A12))
        return -7;
      if (!isfinite(A22))
        return -8;
    }
    if (*es < 0) {
      knd = 0;
      if (A11 != 0.0f)
        knd |= 1;
      if (A21 != 0.0f)
        knd |= 2;
      if (A12 != 0.0f)
        knd |= 4;
      if (A22 != 0.0f)
        knd |= 8;
    }
  }

  *u11 = 1.0f;
  *u21 = 0.0f;
  *u12 = 0.0f;
  *u22 = 1.0f;
  *v11 = 1.0f;
  *v21 = 0.0f;
  *v12 = 0.0f;
  *v22 = 1.0f;
  *s1 = 0.0f;
  *s2 = 0.0f;

  /* simplify the form of A */
  switch (knd) {
  case  0:
    /* [ 0 0 ] */
    /* [ 0 0 ] */
    *u11 = copysignf(1.0f, A11);
    *u22 = copysignf(1.0f, A22);
    A22 = A12 = A21 = A11 = 0.0f;
    break;
  case  1:
    /* [ * 0 ] */
    /* [ 0 0 ] */
    if (A11 < 0.0f) {
      *u11 = -1.0f;
      A11 = -A11;
    }
    *u22 = copysignf(1.0f, A22);
    A22 = A12 = A21 = 0.0f;
    *s1 = A11;
    break;
  case  2:
    /* [ 0 0 ] */
    /* [ * 0 ] */
    *u11 = 0.0f;
    *u22 = 0.0f;
    A11 = A21;
    A22 = A12;
    if (A11 < 0.0f) {
      *u12 = -1.0f;
      A11 = -A11;
    }
    else
      *u12 = 1.0f;
    *u21 = copysignf(1.0f, A12);
    A22 = A12 = A21 = 0.0f;
    *s1 = A11;
    break;
  case  3:
    /* [ * 0 ] */
    /* [ * 0 ] */
    A12 = fabsf(A11);
    A22 = fabsf(A21);
    if (A12 < A22) {
      *u11 = 0.0f;
      *u21 = copysignf(1.0f, A11);
      *u12 = copysignf(1.0f, A21);
      *u22 = 0.0f;
      A11 = A22;
      A21 = A12;
    }
    else {
      *u11 = copysignf(1.0f, A11);
      *u22 = copysignf(1.0f, A21);
      A11 = A12;
      A21 = A22;
    }
    A22 = A12 = 0.0f;
    *s1 = hypotf(A11, A21);
    e = 3;
    break;
  case  4:
    /* [ 0 * ] */
    /* [ 0 0 ] */
    *u11 = copysignf(1.0f, A12);
    *u22 = copysignf(1.0f, A21);
    A11 = fabsf(A12);
    A22 = A12 = A21 = 0.0f;
    *v11 = 0.0f;
    *v21 = 1.0f;
    *v12 = 1.0f;
    *v22 = 0.0f;
    *s1 = A11;
    break;
  case  5:
    /* [ * * ] */
    /* [ 0 0 ] */
    A21 = fabsf(A11);
    A22 = fabsf(A12);
    if (A21 < A22) {
      *v11 = 0.0f;
      *v21 = copysignf(1.0f, A12);
      *v12 = copysignf(1.0f, A11);
      *v22 = 0.0f;
      A11 = A22;
      A12 = A21;
    }
    else {
      *v11 = copysignf(1.0f, A11);
      *v22 = copysignf(1.0f, A12);
      A11 = A21;
      A12 = A22;
    }
    A22 = A21 = 0.0f;
    *s1 = hypotf(A11, A12);
    e = 5;
    break;
  case  6:
    /* [ 0 * ] */
    /* [ * 0 ] */
    *u11 = copysignf(1.0f, A12);
    *u22 = copysignf(1.0f, A21);
    A11 = fabsf(A12);
    A22 = fabsf(A21);
    A12 = A21 = 0.0f;
    *v11 = 0.0f;
    *v21 = 1.0f;
    *v12 = 1.0f;
    *v22 = 0.0f;
    *s1 = A11;
    *s2 = A22;
    break;
  case  7:
    /* [ * * ] */
    /* [ * 0 ] */
    A22 = A11;
    A11 = A12;
    A12 = A22;
    A22 = A21;
    A21 = 0.0f;
    *v11 = 0.0f;
    *v22 = 0.0f;
    if (A11 < 0.0f) {
      A11 = -A11;
      *v21 = -1.0f;
    }
    else
      *v21 = 1.0f;
    if (A12 < 0.0f) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -1.0f;
    }
    else
      *v12 = 1.0f;
    if (A22 < 0.0f) {
      *u22 = -1.0f;
      A22 = -A22;
    }
    e = 13;
    break;
  case  8:
    /* [ 0 0 ] */
    /* [ 0 * ] */
    *u11 = 0.0f;
    *u21 = copysignf(1.0f, A11);
    *u12 = copysignf(1.0f, A22);
    *u22 = 0.0f;
    A11 = fabsf(A22);
    A22 = A12 = A21 = 0.0f;
    *v11 = 0.0f;
    *v21 = 1.0f;
    *v12 = 1.0f;
    *v22 = 0.0f;
    *s1 = A11;
    break;
  case  9:
    /* [ * 0 ] */
    /* [ 0 * ] */
    if (A11 < 0.0f) {
      *u11 = -1.0f;
      A11 = -A11;
    }
    if (A22 < 0.0f) {
      *u22 = -1.0f;
      A22 = -A22;
    }
    A12 = A21 = 0.0f;
    *s1 = A11;
    *s2 = A22;
    break;
  case 10:
    /* [ 0 0 ] */
    /* [ * * ] */
    *u11 = 0.0f;
    *u21 = 1.0f;
    *u12 = 1.0f;
    *u22 = 0.0f;
    A11 = A21;
    A12 = A22;
    A21 = fabsf(A11);
    A22 = fabsf(A12);
    if (A21 < A22) {
      *v11 = 0.0f;
      *v21 = copysignf(1.0f, A12);
      *v12 = copysignf(1.0f, A11);
      *v22 = 0.0f;
      A11 = A22;
      A12 = A21;
    }
    else {
      *v11 = copysignf(1.0f, A11);
      *v22 = copysignf(1.0f, A12);
      A11 = A21;
      A12 = A22;
    }
    A22 = A21 = 0.0f;
    *s1 = hypotf(A11, A12);
    e = 5;
    break;
  case 11:
    /* [ * 0 ] */
    /* [ * * ] */
    *u11 = 0.0f;
    *u12 = 1.0f;
    *u22 = 0.0f;
    A12 = A11;
    A11 = A22;
    A22 = A12;
    A12 = A21;
    A21 = 0.0f;
    *v11 = 0.0f;
    *v22 = 0.0f;
    if (A11 < 0.0f) {
      A11 = -A11;
      *v21 = -1.0f;
    }
    else
      *v21 = 1.0f;
    if (A12 < 0.0f) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -1.0f;
    }
    else
      *v12 = 1.0f;
    if (A22 < 0.0f) {
      *u21 = -1.0f;
      A22 = -A22;
    }
    else
      *u21 = 1.0f;
    e = 13;
    break;
  case 12:
    /* [ 0 * ] */
    /* [ 0 * ] */
    A11 = A12;
    A21 = A22;
    *v11 = 0.0f;
    *v21 = 1.0f;
    *v12 = 1.0f;
    *v22 = 0.0f;
    A12 = fabsf(A11);
    A22 = fabsf(A21);
    if (A12 < A22) {
      *u11 = 0.0f;
      *u21 = copysignf(1.0f, A11);
      *u12 = copysignf(1.0f, A21);
      *u22 = 0.0f;
      A11 = A22;
      A21 = A12;
    }
    else {
      *u11 = copysignf(1.0f, A11);
      *u22 = copysignf(1.0f, A21);
      A11 = A12;
      A21 = A22;
    }
    A22 = A12 = 0.0f;
    *s1 = hypotf(A11, A21);
    e = 3;
    break;
  case 13:
    /* [ * * ] */
    /* [ 0 * ] */
    if (A11 < 0.0f) {
      A11 = -A11;
      *v11 = -1.0f;
    }
    if (A12 < 0.0f) {
      A12 = -A12;
      A22 = -A22;
      *v22 = -1.0f;
    }
    if (A22 < 0.0f) {
      *u22 = -1.0f;
      A22 = -A22;
    }
    A21 = 0.0f;
    e = 13;
    break;
  case 14:
    /* [ 0 * ] */
    /* [ * * ] */
    *u11 = 0.0f;
    *u12 = 1.0f;
    *u22 = 0.0f;
    A11 = A12;
    A12 = A22;
    A22 = A11;
    A11 = A21;
    A21 = 0.0f;
    if (A11 < 0.0f) {
      A11 = -A11;
      *v11 = -1.0f;
    }
    if (A12 < 0.0f) {
      A12 = -A12;
      A22 = -A22;
      *v22 = -1.0f;
    }
    if (A22 < 0.0f) {
      *u21 = -1.0f;
      A22 = -A22;
    }
    else
      *u21 = 1.0f;
    e = 13;
    break;
  case 15:
    /* [ * * ] */
    /* [ * * ] */
    e = 15;
    break;
  default:
    return INT_MIN;
  }

  if ((e == 13) && (A11 < A22))
    e = -13;

#ifndef NDEBUG
  char s[17] = { '\0' };
  (void)printf("\tA[knd=%d,e=%d] * 2^%d =\n", knd, e, *es);
  (void)printf("%s ", pvn_stoa(s, A11));
  (void)printf("%s\n", pvn_stoa(s, A12));
  (void)printf("%s ", pvn_stoa(s, A21));
  (void)printf("%s\n", pvn_stoa(s, A22));
#endif /* !NDEBUG */

  float tt = 0.0f, ct = 1.0f, st = 0.0f;

  if (e == 15) {
    /* [ * * ] */
    /* [ * * ] */
    *s1 = hypotf(A11, A21);
    *s2 = hypotf(A12, A22);
    if (*s1 < *s2) {
      pvn_fswp(&A11, &A12);
      pvn_fswp(&A21, &A22);
      pvn_fswp(v11, v12);
      pvn_fswp(v21, v22);
      pvn_fswp(s1, s2);
    }
    if (copysignf(1.0f, A11) != 1.0f) {
      *u11 = -*u11;
      *u12 = -*u12;
      A11 = -A11;
      A12 = -A12;
    }
    if (copysignf(1.0f, A21) != 1.0f) {
      *u21 = -*u21;
      *u22 = -*u22;
      A21 = -A21;
      A22 = -A22;
    }
    if (A11 < A21) {
      pvn_fswp(u11, u21);
      pvn_fswp(u12, u22);
      pvn_fswp(&A11, &A21);
      pvn_fswp(&A12, &A22);
    }
    /* U^T(ϑ):
        cos(ϑ)  sin(ϑ)
       -sin(ϑ)  cos(ϑ)
    */
    tt = (A21 / A11);
    /* 1 / cos */
    ct = hypotf(tt, 1.0f);
    /* apply the left Givens rotation to A (and maybe to U) */
    st = -tt;
    A21 = A12;
    if (ct == 1.0f) {
      A12 = fmaf(tt, A22, A12);
      A22 = fmaf(st, A21, A22);
      if ((A12 == 0.0f) || (A22 == 0.0f)) {
        A21 = *u11;
        *u11 = fmaf(tt, *u21, *u11);
        *u21 = fmaf(st,  A21, *u21);
        A21 = *u12;
        *u12 = fmaf(tt, *u22, *u12);
        *u22 = fmaf(st,  A21, *u22);
      }
      st = tt;
    }
    else {
      A12 = (fmaf(tt, A22, A12) / ct);
      A22 = (fmaf(st, A21, A22) / ct);
      if ((A12 == 0.0f) || (A22 == 0.0f)) {
        A21 = *u11;
        *u11 = (fmaf(tt, *u21, *u11) / ct);
        *u21 = (fmaf(st,  A21, *u21) / ct);
        A21 = *u12;
        *u12 = (fmaf(tt, *u22, *u12) / ct);
        *u22 = (fmaf(st,  A21, *u22) / ct);
      }
      st = (tt / ct);
      ct = (1.0f / ct);
    }
    A11 = *s1;
    A21 = 0.0f;
    if (A12 == 0.0f) {
      A12 = 0.0f;
      if (copysignf(1.0f, A22) != 1.0f) {
        *u21 = -*u21;
        *u22 = -*u22;
        A22 = -A22;
      }
      e = 0;
    }
    else if (A22 == 0.0f) {
      if (copysignf(1.0f, A12) != 1.0f) {
        A12 = -A12;
        A22 = -A22;
        *v12 = -*v12;
        *v22 = -*v22;
      }
      if (copysignf(1.0f, A22) != 1.0f) {
        *u21 = -*u21;
        *u22 = -*u22;
        A22 = 0.0f;
      }
      *s1 = hypotf(*s1, *s2);
      *s2 = 0.0f;
      e = 5;
    }
    else
      e = 13;
    if (A12 < 0.0f) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -*v12;
      *v22 = -*v22;
    }
    if (A22 < 0.0f) {
      A22 = -A22;
      /* sin(ϑ) is always non-negative by construction */
      /* this is just an extra bit of info, used later */
      st = -st;
    }
    if ((e == 13) && (A11 < A22))
      e = -13;
#ifndef NDEBUG
    (void)printf("tan(ϑ)=%s, ", pvn_stoa(s, tt));
    (void)printf("cos(ϑ)=%s, ", pvn_stoa(s, ct));
    (void)printf("sin(ϑ)=%s\n", pvn_stoa(s, st));
#endif /* !NDEBUG */
  }

  if (e == 3) {
    /* [ X 0 ] */
    /* [ x 0 ] */
    /* U^T(ϑ):
        cos(ϑ)  sin(ϑ)
       -sin(ϑ)  cos(ϑ)
    */
    tt = (A21 / A11);
    /* 1 / cos */
    ct = hypotf(tt, 1.0f);
    /* apply the left Givens rotation to U */
    st = -tt;
    A21 = *u11;
    if (ct == 1.0f) {
      *u11 = fmaf(tt, *u21, *u11);
      *u21 = fmaf(st,  A21, *u21);
      A21 = *u12;
      *u12 = fmaf(tt, *u22, *u12);
      *u22 = fmaf(st,  A21, *u22);
      st = tt;
    }
    else {
      *u11 = (fmaf(tt, *u21, *u11) / ct);
      *u21 = (fmaf(st,  A21, *u21) / ct);
      A21 = *u12;
      *u12 = (fmaf(tt, *u22, *u12) / ct);
      *u22 = (fmaf(st,  A21, *u22) / ct);
      st = (tt / ct);
      ct = (1.0f / ct);
    }
    A11 = *s1;
    A21 = 0.0f;
    e = 0;
#ifndef NDEBUG
    (void)printf("tan(ϑ)=%s, ", pvn_stoa(s, tt));
    (void)printf("cos(ϑ)=%s, ", pvn_stoa(s, ct));
    (void)printf("sin(ϑ)=%s\n", pvn_stoa(s, st));
#endif /* !NDEBUG */
  }

  if (e == 5) {
    /* [ X x ] */
    /* [ 0 0 ] */
    /* V(θ):
       cos(θ) -sin(θ)
       sin(θ)  cos(θ)
    */
    tt = (A12 / A11);
    /* 1 / cos */
    ct = hypotf(tt, 1.0f);
    /* apply the right Givens rotation to V */
    st = -tt;
    A12 = *v11;
    if (ct == 1.0f) {
      *v11 = fmaf(tt, *v12, *v11);
      *v12 = fmaf(st,  A12, *v12);
      A12 = *v21;
      *v21 = fmaf(tt, *v22, *v21);
      *v22 = fmaf(st,  A12, *v22);
      st = tt;
    }
    else {
      *v11 = (fmaf(tt, *v12, *v11) / ct);
      *v12 = (fmaf(st,  A12, *v12) / ct);
      A12 = *v21;
      *v21 = (fmaf(tt, *v22, *v21) / ct);
      *v22 = (fmaf(st,  A12, *v22) / ct);
      st = (tt / ct);
      ct = (1.0f / ct);
    }
    A11 = *s1;
    A12 = 0.0f;
    e = 0;
#ifndef NDEBUG
    (void)printf("tan(θ)=%s, ", pvn_stoa(s, tt));
    (void)printf("cos(θ)=%s, ", pvn_stoa(s, ct));
    (void)printf("sin(θ)=%s\n", pvn_stoa(s, st));
#endif /* !NDEBUG */
  }

#ifndef NDEBUG
  (void)printf("\tA =\n");
  (void)printf("%s ", pvn_stoa(s, scalbnf(A11, -*es)));
  (void)printf("%s\n", pvn_stoa(s, scalbnf(A12, -*es)));
  (void)printf("%s ", pvn_stoa(s, scalbnf(A21, -*es)));
  (void)printf("%s\n", pvn_stoa(s, scalbnf(A22, -*es)));
#endif /* !NDEBUG */

  if (abs(e) == 13) {
    /* [ x y ] */
    /* [ 0 z ] */

    float tf = 0.0f, cf = 1.0f, sf = 0.0f, tp = 0.0f, cp = 1.0f, sp = 0.0f;
    if (e == -13) {
      float tf_ = 0.0f, cf_ = 1.0f, sf_ = 0.0f, tp_ = 0.0f, cp_ = 1.0f, sp_ = 0.0f;
      slpsv2(A22, A12, A11, &tf_, &cf_, &sf_, &tp_, &cp_, &sp_, s1, s2, es
#ifndef NDEBUG
             , s
#endif /* !NDEBUG */
             );
      tf = (1.0f / tp_);
      cf = sp_;
      sf = cp_;
      tp = (1.0f / tf_);
      cp = sf_;
      sp = cf_;
    }
    else
      slpsv2(A11, A12, A22, &tf, &cf, &sf, &tp, &cp, &sp, s1, s2, es
#ifndef NDEBUG
             , s
#endif /* !NDEBUG */
             );

    /* update U */
    if (copysignf(1.0f, st) != 1.0f) {
      /* U^T(φ) * diag(1, -1) * U^T(ϑ):
          cos(φ - ϑ) -sin(φ - ϑ)
         -sin(φ - ϑ) -cos(φ - ϑ)
      */
      st = -st;
      float tf_t = (tf - tt), cf_t = 1.0f, sf_t = 0.0f;
      if (tf_t != 0.0f) {
        tf_t /= fmaf(tf, tt, 1.0f);
        if (isfinite(tf_t)) {
          /* 1 / cos */
          cf_t = hypotf(tf_t, 1.0f);
          sf_t = (tf_t / cf_t);
          cf_t = (1.0f / cf_t);
        }
        else {
          sf_t = copysignf(1.0f, tf_t);
          cf_t = 0.0f;
        }
      }
      else
        sf_t = tf_t;
      const float _sf_t = -sf_t;
      A21 = *u11;
      *u11 = (_sf_t * *u21 + cf_t * *u11);
      *u21 = (_sf_t *  A21 - cf_t * *u21);
      A21 = *u12;
      *u12 = (_sf_t * *u22 + cf_t * *u12);
      *u22 = (_sf_t *  A21 - cf_t * *u22);
      A21 = -1.0f;
    }
    else if (tt != 0.0f) {
      /* U^T(φ) * U^T(ϑ) = U^T(φ + ϑ):
          cos(φ + ϑ)  sin(φ + ϑ)
         -sin(φ + ϑ)  cos(φ + ϑ)
      */
      float tft = (tf + tt), cft = 1.0f, sft = 0.0f;
      if (tft != 0.0f) {
        tft /= fmaf(-tf, tt, 1.0f);
        if (isfinite(tft)) {
          /* 1 / cos */
          cft = hypotf(tft, 1.0f);
          sft = (tft / cft);
          cft = (1.0f / cft);
        }
        else {
          sft = copysignf(1.0f, tft);
          cft = 0.0f;
        }
      }
      else
        sft = tft;
      A21 = *u11;
      *u11 = (cft * *u11 + sft * *u21);
      *u21 = (cft * *u21 - sft *  A21);
      A21 = *u12;
      *u12 = (cft * *u12 + sft * *u22);
      *u22 = (cft * *u22 - sft *  A21);
      A21 = 1.0f;
    }
    else if (tf != 0.0f) {
      /* U^T(φ):
          cos(φ)  sin(φ)
         -sin(φ)  cos(φ)
       */
      A21 = *u11;
      *u11 = (cf * *u11 + sf * *u21);
      *u21 = (cf * *u21 - sf *  A21);
      A21 = *u12;
      *u12 = (cf * *u12 + sf * *u22);
      *u22 = (cf * *u22 - sf *  A21);
      A21 = -0.0f;
    }
    else /* U^T(φ) = I */
      A21 = 0.0f;
#ifndef NDEBUG
    (void)printf("U operation=%s\n", pvn_stoa(s, A21));
#endif /* !NDEBUG */

    /* update V */
    if (tp != 0.0f) {
      /* V(ψ):
         cos(ψ) -sin(ψ)
         sin(ψ)  cos(ψ)
      */
      A21 = *v11;
      *v11 = (*v11 * cp + *v12 * sp);
      *v12 = (*v12 * cp -  A21 * sp);
      A21 = *v21;
      *v21 = (*v21 * cp + *v22 * sp);
      *v22 = (*v22 * cp -  A21 * sp);
      A21 = -0.0f;
    }
    else /* V(ψ) = I */
      A21 = 0.0f;
#ifndef NDEBUG
    (void)printf("V operation=%s\n", pvn_stoa(s, A21));
#endif /* !NDEBUG */

    if (e == -13) {
      *u21 = -*u21;
      *u22 = -*u22;
      *v12 = -*v12;
      *v22 = -*v22;
    }
  }

  if (ef_cmpf(es[1], *s1, es[2], *s2) < 0) {
    pvn_fswp(u11, u21);
    pvn_fswp(u12, u22);
    pvn_fswp(v11, v12);
    pvn_fswp(v21, v22);
    pvn_fswp(s1, s2);
  }
  pvn_fswp(u21, u12);
  return knd;
}

static float cpolarf(const float r, const float i, float *const c, float *const s)
{
  assert(c);
  assert(s);
  const float a = hypotf(r, i);
  *c = copysignf(fminf((fabsf(r) / a), 1.0f), r);
  *s = (i / fmaxf(a, FLT_TRUE_MIN));
  return a;
}

int
#ifdef _WIN32
PVN_CLJSV2
#else /* !_WIN32 */
pvn_cljsv2_
#endif /* ?_WIN32 */
(const float *const a11r, const float *const a11i, const float *const a21r, const float *const a21i, const float *const a12r, const float *const a12i, const float *const a22r, const float *const a22i,
 float *const u11r, float *const u11i, float *const u21r, float *const u21i, float *const u12r, float *const u12i, float *const u22r, float *const u22i,
 float *const v11r, float *const v11i, float *const v21r, float *const v21i, float *const v12r, float *const v12i, float *const v22r, float *const v22i,
 float *const s1, float *const s2, int *const es)
{
  assert(a11r);
  assert(a11i);
  assert(a21r);
  assert(a21i);
  assert(a12r);
  assert(a12i);
  assert(a22r);
  assert(a22i);
  assert(u11r);
  assert(u11i);
  assert(u21r);
  assert(u21i);
  assert(u12r);
  assert(u12i);
  assert(u22r);
  assert(u22i);
  assert(v11r);
  assert(v11i);
  assert(v21r);
  assert(v21i);
  assert(v12r);
  assert(v12i);
  assert(v22r);
  assert(v22i);
  assert(s1);
  assert(s2);
  assert(es);
  if (!isfinite(*a11r))
    return -1;
  if (!isfinite(*a11i))
    return -2;
  if (!isfinite(*a21r))
    return -3;
  if (!isfinite(*a21i))
    return -4;
  if (!isfinite(*a12r))
    return -5;
  if (!isfinite(*a12i))
    return -6;
  if (!isfinite(*a22r))
    return -7;
  if (!isfinite(*a22i))
    return -8;
  /* should not fail beyond this point when *es == 0 */
  int kndi = 0, mxe = INT_MIN, e = 0;
  if (*a11i != 0.0f) {
    kndi |= 1;
    (void)frexpf(*a11i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21i != 0.0f) {
    kndi |= 2;
    (void)frexpf(*a21i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12i != 0.0f) {
    kndi |= 4;
    (void)frexpf(*a12i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22i != 0.0f) {
    kndi |= 8;
    (void)frexpf(*a22i, &e);
    if (mxe < e)
      mxe = e;
  }
  *u11i = 0.0f;
  *u21i = 0.0f;
  *u12i = 0.0f;
  *u22i = 0.0f;
  *v11i = 0.0f;
  *v21i = 0.0f;
  *v12i = 0.0f;
  *v22i = 0.0f;
  if (!kndi)
    return
#ifdef _WIN32
      PVN_SLJSV2
#else /* !_WIN32 */
      pvn_sljsv2_
#endif /* ?_WIN32 */
      (a11r, a21r, a12r, a22r, u11r, u21r, u12r, u22r, v11r, v21r, v12r, v22r, s1, s2, es);
  int kndr = 0;
  if (*a11r != 0.0f) {
    kndr |= 1;
    (void)frexpf(*a11r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21r != 0.0f) {
    kndr |= 2;
    (void)frexpf(*a21r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12r != 0.0f) {
    kndr |= 4;
    (void)frexpf(*a12r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22r != 0.0f) {
    kndr |= 8;
    (void)frexpf(*a22r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (!kndr) {
    *u11r = 0.0f;
    *u21r = 0.0f;
    *u12r = 0.0f;
    *u22r = 0.0f;
    return
#ifdef _WIN32
      PVN_SLJSV2
#else /* !_WIN32 */
      pvn_sljsv2_
#endif /* ?_WIN32 */
      (a11i, a21i, a12i, a22i, u11i, u21i, u12i, u22i, v11r, v21r, v12r, v22r, s1, s2, es);
  }

  int knd = (kndr | kndi);
  switch (knd) {
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  3:
  case  5:
  case  7:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    e = (FLT_MAX_EXP - mxe - 2);
    break;
  default:
    return INT_MIN;
  }
  mxe = *es;
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;
  es[2] = es[1] = 0;

  /* scaling by 2^(*es), can only fail when mxe != 0 */
  float A11r = *a11r, A11i = *a11i, A21r = *a21r, A21i = *a21i, A12r = *a12r, A12i = *a12i, A22r = *a22r, A22i = *a22i;
  if (*es) {
    A11r = scalbnf(A11r, *es);
    A11i = scalbnf(A11i, *es);
    A21r = scalbnf(A21r, *es);
    A21i = scalbnf(A21i, *es);
    A12r = scalbnf(A12r, *es);
    A12i = scalbnf(A12i, *es);
    A22r = scalbnf(A22r, *es);
    A22i = scalbnf(A22i, *es);
    if (mxe) {
      if (!isfinite(A11r))
        return -9;
      if (!isfinite(A11i))
        return -10;
      if (!isfinite(A21r))
        return -11;
      if (!isfinite(A21i))
        return -12;
      if (!isfinite(A12r))
        return -13;
      if (!isfinite(A12i))
        return -14;
      if (!isfinite(A22r))
        return -15;
      if (!isfinite(A22i))
        return -16;
    }
    if (*es < 0) {
      kndr = 0;
      if (A11r != 0.0f)
        kndr |= 1;
      if (A21r != 0.0f)
        kndr |= 2;
      if (A12r != 0.0f)
        kndr |= 4;
      if (A22r != 0.0f)
        kndr |= 8;
      kndi = 0;
      if (A11i != 0.0f)
        kndi |= 1;
      if (A21i != 0.0f)
        kndi |= 2;
      if (A12i != 0.0f)
        kndi |= 4;
      if (A22i != 0.0f)
        kndi |= 8;
      knd = (kndr | kndi);
    }
  }

  *u11r = 1.0f;
  *u21r = 0.0f;
  *u12r = 0.0f;
  *u22r = 1.0f;
  *v11r = 1.0f;
  *v21r = 0.0f;
  *v12r = 0.0f;
  *v22r = 1.0f;
  *s1 = 0.0f;
  *s2 = 0.0f;

  /* simplify the form of A */
  switch (knd) {
  case  0:
  case  1:
  case  8:
  case  9:
    /* [ ? 0 ] */
    /* [ 0 ? ] */
    *s1 = cpolarf(A11r, A11i, u11r, u11i); *u11i = -*u11i;
    A11r = *s1; A11i = 0.0f;
    *s2 = cpolarf(A22r, A22i, u22r, u22i); *u22i = -*u22i;
    A22r = *s2; A22i = 0.0f;
    A12i = A12r = A21i = A21r = 0.0f;
    break;
  case  2:
  case  4:
  case  6:
    /* [ 0 ? ] */
    /* [ ? 0 ] */
    *u11r = 0.0f;
    *u22r = 0.0f;
    A11r = A21r; A11i = A21i;
    A22r = A12r; A22i = A12i;
    *s1 = cpolarf(A11r, A11i, u12r, u12i); *u12i = -*u12i;
    A11r = *s1; A11i = 0.0f;
    *s2 = cpolarf(A22r, A22i, u21r, u21i); *u21i = -*u21i;
    A22r = *s2; A22i = 0.0f;
    A12i = A12r = A21i = A21r = 0.0f;
    break;
  case 12:
    /* [ 0 * ] */
    /* [ 0 * ] */
    *v11r = 0.0f;
    *v21r = 1.0f;
    *v12r = 1.0f;
    *v22r = 0.0f;
    A11r = A12r; A11i = A12i;
    A21r = A22r; A21i = A22i;
    /* FALLTHRU */
  case  3:
    /* [ * 0 ] */
    /* [ * 0 ] */
    *s1 = cpolarf(A11r, A11i, u11r, u11i); *u11i = -*u11i;
    A11r = *s1; A11i = 0.0f;
    *s2 = cpolarf(A21r, A21i, u22r, u22i); *u22i = -*u22i;
    A21r = *s2; A21i = 0.0f;
    if (A11r < A21r) {
      pvn_fswp(u11r, u21r);
      pvn_fswp(u11i, u21i);
      pvn_fswp(u22r, u12r);
      pvn_fswp(u22i, u12i);
      pvn_fswp(&A11r, &A21r);
    }
    A22i = A22r = A12i = A12r = 0.0f;
    *s1 = hypotf(A11r, A21r);
    *s2 = 0.0f;
    e = 3;
    break;
  case 10:
    /* [ 0 0 ] */
    /* [ * * ] */
    *u11r = 0.0f;
    *u21r = 1.0f;
    *u12r = 1.0f;
    *u22r = 0.0f;
    A11r = A21r; A11i = A21i;
    A12r = A22r; A12i = A22i;
    /* FALLTHRU */
  case  5:
    /* [ * * ] */
    /* [ 0 0 ] */
    *s1 = cpolarf(A11r, A11i, v11r, v11i); *v11i = -*v11i;
    A11r = *s1; A11i = 0.0f;
    *s2 = cpolarf(A12r, A12i, v22r, v22i); *v22i = -*v22i;
    A12r = *s2; A12i = 0.0f;
    if (A11r < A12r) {
      pvn_fswp(&A11r, &A12r);
      pvn_fswp(v11r, v12r);
      pvn_fswp(v11i, v12i);
      pvn_fswp(v22r, v21r);
      pvn_fswp(v22i, v21i);
    }
    A22i = A22r = A21i = A21r = 0.0f;
    *s1 = hypotf(A11r, A12r);
    *s2 = 0.0f;
    e = 5;
    break;
  case  7:
    /* [ * * ] */
    /* [ * 0 ] */
    pvn_fswp(&A11r, &A12r);
    pvn_fswp(&A11i, &A12i);
    A22r = A21r; A22i = A21i;
    *v11r = 0.0f;
    *v22r = 0.0f;
    *s1 = cpolarf(A11r, A11i, v21r, v21i); *v21i = -*v21i;
    A11r = *s1; A11i = 0.0f;
    *s1 = cpolarf(A12r, A12i, v12r, v12i); *v12i = -*v12i;
    A12r = *s1; A12i = 0.0f;
    pvn_cmul(&A21r, &A21i, A22r, A22i, *v12r, *v12i);
    A22r = A21r; A22i = A21i;
    *s2 = cpolarf(A22r, A22i, u22r, u22i); *u22i = -*u22i;
    A22r = *s2; A22i = 0.0f;
    *s2 = *s1 = A21i = A21r = 0.0f;
    e = 13;
    break;
  case 11:
    /* [ * 0 ] */
    /* [ * * ] */
    *u11r = 0.0f;
    *u12r = 1.0f;
    *u22r = 0.0f;
    pvn_fswp(&A11r, &A22r);
    pvn_fswp(&A11i, &A22i);
    A12r = A21r; A12i = A21i;
    *v11r = 0.0f;
    *v22r = 0.0f;
    *s1 = cpolarf(A11r, A11i, v21r, v21i); *v21i = -*v21i;
    A11r = *s1; A11i = 0.0f;
    *s1 = cpolarf(A12r, A12i, v12r, v12i); *v12i = -*v12i;
    A12r = *s1; A12i = 0.0f;
    pvn_cmul(&A21r, &A21i, A22r, A22i, *v12r, *v12i);
    A22r = A21r; A22i = A21i;
    *s2 = cpolarf(A22r, A22i, u21r, u21i); *u21i = -*u21i;
    A22r = *s2; A22i = 0.0f;
    *s2 = *s1 = A21i = A21r = 0.0f;
    e = 13;
    break;
  case 13:
    /* [ * * ] */
    /* [ 0 * ] */
    *s1 = cpolarf(A11r, A11i, v11r, v11i); *v11i = -*v11i;
    A11r = *s1; A11i = 0.0f;
    *s1 = cpolarf(A12r, A12i, v22r, v22i); *v22i = -*v22i;
    A12r = *s1; A12i = 0.0f;
    pvn_cmul(&A21r, &A21i, A22r, A22i, *v22r, *v22i);
    A22r = A21r; A22i = A21i;
    *s2 = cpolarf(A22r, A22i, u22r, u22i); *u22i = -*u22i;
    A22r = *s2; A22i = 0.0f;
    *s2 = *s1 = A21i = A21r = 0.0f;
    e = 13;
    break;
  case 14:
    /* [ 0 * ] */
    /* [ * * ] */
    *u11r = 0.0f;
    *u12r = 1.0f;
    *u22r = 0.0f;
    A11r = A21r; A11i = A21i;
    pvn_fswp(&A12r, &A22r);
    pvn_fswp(&A12i, &A22i);
    *s1 = cpolarf(A11r, A11i, v11r, v11i); *v11i = -*v11i;
    A11r = *s1; A11i = 0.0f;
    *s1 = cpolarf(A12r, A12i, v22r, v22i); *v22i = -*v22i;
    A12r = *s1; A12i = 0.0f;
    pvn_cmul(&A21r, &A21i, A22r, A22i, *v22r, *v22i);
    A22r = A21r; A22i = A21i;
    *s2 = cpolarf(A22r, A22i, u21r, u21i); *u21i = -*u21i;
    A22r = *s2; A22i = 0.0f;
    *s2 = *s1 = A21i = A21r = 0.0f;
    e = 13;
    break;
  case 15:
    /* [ * * ] */
    /* [ * * ] */
    e = 15;
    break;
  default:
    return INT_MIN;
  }

  if ((e == 13) && (A11r < A22r))
    e = -13;

  float tt = 0.0f, ct = 1.0f, st = 0.0f;

  if (e == 15) {
    float B11, B21, B12, B22, C11, C21, C12, C22, S11, S21, S12, S22;
    B11 = cpolarf(A11r, A11i, &C11, &S11); S11 = -S11;
    B21 = cpolarf(A21r, A21i, &C21, &S21); S21 = -S21;
    B12 = cpolarf(A12r, A12i, &C12, &S12); S12 = -S12;
    B22 = cpolarf(A22r, A22i, &C22, &S22); S22 = -S22;
    *s1 = hypotf(B11, B21);
    *s2 = hypotf(B12, B22);
    if (*s1 < *s2) {
      pvn_fswp(&A11r, &A12r);
      pvn_fswp(&A11i, &A12i);
      pvn_fswp(&A21r, &A22r);
      pvn_fswp(&A21i, &A22i);
      pvn_fswp(&B11, &B12);
      pvn_fswp(&B21, &B22);
      pvn_fswp(&C11, &C12);
      pvn_fswp(&C21, &C22);
      pvn_fswp(&S11, &S12);
      pvn_fswp(&S21, &S22);
      pvn_fswp(s1, s2);
      pvn_fswp(v11r, v12r);
      pvn_fswp(v11i, v12i);
      pvn_fswp(v21r, v22r);
      pvn_fswp(v21i, v22i);
    }
    if (B11 < B21) {
      pvn_fswp(u11r, u21r);
      pvn_fswp(u11i, u21i);
      pvn_fswp(u12r, u22r);
      pvn_fswp(u12i, u22i);
      pvn_fswp(&A11r, &A21r);
      pvn_fswp(&A11i, &A21i);
      pvn_fswp(&A12r, &A22r);
      pvn_fswp(&A12i, &A22i);
      pvn_fswp(&B11, &B21);
      pvn_fswp(&B12, &B22);
      pvn_fswp(&C11, &C21);
      pvn_fswp(&C12, &C22);
      pvn_fswp(&S11, &S21);
      pvn_fswp(&S12, &S22);
    }
    pvn_cmul(u11r, u11i, C11, S11, *u11r, *u11i);
    pvn_cmul(u12r, u12i, C11, S11, *u12r, *u12i);
    A11r = B11; A11i = 0.0f;
    pvn_cmul(&A12r, &A12i, C11, S11, A12r, A12i);
    pvn_cmul(u21r, u21i, C21, S21, *u21r, *u21i);
    pvn_cmul(u22r, u22i, C21, S21, *u22r, *u22i);
    A21r = B21; A21i = 0.0f;
    pvn_cmul(&A22r, &A22i, C21, S21, A22r, A22i);
    /* rotate */
    tt = (A21r / A11r);
    /* 1 / cos */
    ct = hypotf(tt, 1.0f);
    /* apply the left Givens rotation to A and U */
    st = -tt;
    A21r = A12r;
    A21i = A12i;
    if (ct == 1.0f) {
      A12r = fmaf(tt, A22r, A12r);
      A12i = fmaf(tt, A22i, A12i);
      A22r = fmaf(st, A21r, A22r);
      A22i = fmaf(st, A21i, A22i);
      A21r = *u11r;
      A21i = *u11i;
      *u11r = fmaf(tt, *u21r, *u11r);
      *u11i = fmaf(tt, *u21i, *u11i);
      *u21r = fmaf(st,  A21r, *u21r);
      *u21i = fmaf(st,  A21i, *u21i);
      A21r = *u12r;
      A21i = *u12i;
      *u12r = fmaf(tt, *u22r, *u12r);
      *u12i = fmaf(tt, *u22i, *u12i);
      *u22r = fmaf(st,  A21r, *u22r);
      *u22i = fmaf(st,  A21i, *u22i);
      st = tt;
    }
    else {
      A12r = (fmaf(tt, A22r, A12r) / ct);
      A12i = (fmaf(tt, A22i, A12i) / ct);
      A22r = (fmaf(st, A21r, A22r) / ct);
      A22i = (fmaf(st, A21i, A22i) / ct);
      A21r = *u11r;
      A21i = *u11i;
      *u11r = (fmaf(tt, *u21r, *u11r) / ct);
      *u11i = (fmaf(tt, *u21i, *u11i) / ct);
      *u21r = (fmaf(st,  A21r, *u21r) / ct);
      *u21i = (fmaf(st,  A21i, *u21i) / ct);
      A21r = *u12r;
      A21i = *u12i;
      *u12r = (fmaf(tt, *u22r, *u12r) / ct);
      *u12i = (fmaf(tt, *u22i, *u12i) / ct);
      *u22r = (fmaf(st,  A21r, *u22r) / ct);
      *u22i = (fmaf(st,  A21i, *u22i) / ct);
      st = (tt / ct);
      ct = (1.0f / ct);
    }
    A11r = *s1;
    A21r = cpolarf(A12r, A12i, &C12, &S12); S12 = -S12;
    A12r = B12; A12i = 0.0f;
    pvn_cmul(&A21r, &A21i, A22r, A22i, C12, S12);
    A22r = A21r; A22i = A21i;
    pvn_cmul(&A21r, &A21i, *v12r, *v12i, C12, S12);
    *v12r = A21r; *v12i = A21i;
    pvn_cmul(&A21r, &A21i, *v22r, *v22i, C12, S12);
    *v22r = A21r; *v22i = A21i;
    A21i = cpolarf(A22r, A22i, &C22, &S22); S22 = -S22;
    A22r = B22; A22i = 0.0f;
    pvn_cmul(&A21r, &A21i, C22, S22, *u21r, *u21i);
    *u21r = A21r; *u21i = A21i;
    pvn_cmul(&A21r, &A21i, C22, S22, *u22r, *u22i);
    *u22r = A21r; *u22i = A21i;
    A21i = A21r = 0.0f;
    if (A12r == 0.0f)
      e = 0;
    else if (A22r == 0.0f) {
      *s1 = hypotf(*s1, *s2);
      *s2 = 0.0f;
      e = 5;
    }
    else if (A11r < A22r)
      e = -13;
    else
      e = 13;
  }

  if (e == 3) {
    tt = (A21r / A11r);
    /* 1 / cos */
    ct = hypotf(tt, 1.0f);
    /* apply the left Givens rotation to U */
    st = -tt;
    A21r = *u11r;
    A21i = *u11i;
    if (ct == 1.0f) {
      *u11r = fmaf(tt, *u21r, *u11r);
      *u11i = fmaf(tt, *u21i, *u11i);
      *u21r = fmaf(st,  A21r, *u21r);
      *u21i = fmaf(st,  A21i, *u21i);
      A21r = *u12r;
      A21i = *u12i;
      *u12r = fmaf(tt, *u22r, *u12r);
      *u12i = fmaf(tt, *u22i, *u12i);
      *u22r = fmaf(st,  A21r, *u22r);
      *u22i = fmaf(st,  A21i, *u22i);
      st = tt;
    }
    else {
      *u11r = (fmaf(tt, *u21r, *u11r) / ct);
      *u11i = (fmaf(tt, *u21i, *u11i) / ct);
      *u21r = (fmaf(st,  A21r, *u21r) / ct);
      *u21i = (fmaf(st,  A21i, *u21i) / ct);
      A21r = *u12r;
      A21i = *u12i;
      *u12r = (fmaf(tt, *u22r, *u12r) / ct);
      *u12i = (fmaf(tt, *u22i, *u12i) / ct);
      *u22r = (fmaf(st,  A21r, *u22r) / ct);
      *u22i = (fmaf(st,  A21i, *u22i) / ct);
      st = (tt / ct);
      ct = (1.0f / ct);
    }
    A11r = *s1;
    A21i = A21r = 0.0f;
    e = 0;
  }

  if (e == 5) {
    tt = (A12r / A11r);
    /* 1 / cos */
    ct = hypotf(tt, 1.0f);
    /* apply the right Givens rotation to V */
    st = -tt;
    A12r = *v11r;
    A12i = *v11i;
    if (ct == 1.0f) {
      *v11r = fmaf(tt, *v12r, *v11r);
      *v11i = fmaf(tt, *v12i, *v11i);
      *v12r = fmaf(st,  A12r, *v12r);
      *v12i = fmaf(st,  A12i, *v12i);
      A12r = *v21r;
      A12i = *v21i;
      *v21r = fmaf(tt, *v22r, *v21r);
      *v21i = fmaf(tt, *v22i, *v21i);
      *v22r = fmaf(st,  A12r, *v22r);
      *v22i = fmaf(st,  A12i, *v22i);
      st = tt;
    }
    else {
      *v11r = (fmaf(tt, *v12r, *v11r) / ct);
      *v11i = (fmaf(tt, *v12i, *v11i) / ct);
      *v12r = (fmaf(st,  A12r, *v12r) / ct);
      *v12i = (fmaf(st,  A12i, *v12i) / ct);
      A12r = *v21r;
      A12i = *v21i;
      *v21r = (fmaf(tt, *v22r, *v21r) / ct);
      *v21i = (fmaf(tt, *v22i, *v21i) / ct);
      *v22r = (fmaf(st,  A12r, *v22r) / ct);
      *v22i = (fmaf(st,  A12i, *v22i) / ct);
      st = (tt / ct);
      ct = (1.0f / ct);
    }
    A11r = *s1;
    A12i = A12r = 0.0f;
    e = 0;
  }

  if (abs(e) == 13) {
    float tf = 0.0f, cf = 1.0f, sf = 0.0f, tp = 0.0f, cp = 1.0f, sp = 0.0f;
    if (e == -13) {
      float tf_ = 0.0f, cf_ = 1.0f, sf_ = 0.0f, tp_ = 0.0f, cp_ = 1.0f, sp_ = 0.0f;
      slpsv2(A22r, A12r, A11r, &tf_, &cf_, &sf_, &tp_, &cp_, &sp_, s1, s2, es
#ifndef NDEBUG
             , (char*)NULL
#endif /* !NDEBUG */
             );
      tf = (1.0f / tp_);
      cf = sp_;
      sf = cp_;
      tp = (1.0f / tf_);
      cp = sf_;
      sp = cf_;
    }
    else
      slpsv2(A11r, A12r, A22r, &tf, &cf, &sf, &tp, &cp, &sp, s1, s2, es
#ifndef NDEBUG
             , (char*)NULL
#endif /* !NDEBUG */
             );

    if (tf != 0.0f) {
      A21r = *u11r;
      A21i = *u11i;
      *u11r = (cf * *u11r + sf * *u21r);
      *u11i = (cf * *u11i + sf * *u21i);
      *u21r = (cf * *u21r - sf *  A21r);
      *u21i = (cf * *u21i - sf *  A21i);
      A21r = *u12r;
      A21i = *u12i;
      *u12r = (cf * *u12r + sf * *u22r);
      *u12i = (cf * *u12i + sf * *u22i);
      *u22r = (cf * *u22r - sf *  A21r);
      *u22i = (cf * *u22i - sf *  A21i);
      A21i = A21r = -0.0f;
    }
    else
      A21i = A21r = 0.0f;

    if (tp != 0.0f) {
      /* V(ψ):
         cos(ψ) -sin(ψ)
         sin(ψ)  cos(ψ)
      */
      A21r = *v11r;
      A21i = *v11i;
      *v11r = (*v11r * cp + *v12r * sp);
      *v11i = (*v11i * cp + *v12i * sp);
      *v12r = (*v12r * cp -  A21r * sp);
      *v12i = (*v12i * cp -  A21i * sp);
      A21r = *v21r;
      A21i = *v21i;
      *v21r = (*v21r * cp + *v22r * sp);
      *v21i = (*v21i * cp + *v22i * sp);
      *v22r = (*v22r * cp -  A21r * sp);
      *v22i = (*v22i * cp -  A21i * sp);
      A21i = A21r = -0.0f;
    }
    else
      A21i = A21r = 0.0f;

    if (e == -13) {
      *u21r = -*u21r;
      *u21i = -*u21i;
      *u22r = -*u22r;
      *u22i = -*u22i;
      *v12r = -*v12r;
      *v12i = -*v12i;
      *v22r = -*v22r;
      *v22i = -*v22i;
    }
  }

  if (ef_cmpf(es[1], *s1, es[2], *s2) < 0) {
    pvn_fswp(u11r, u21r);
    pvn_fswp(u11i, u21i);
    pvn_fswp(u12r, u22r);
    pvn_fswp(u12i, u22i);
    pvn_fswp(v11r, v12r);
    pvn_fswp(v11i, v12i);
    pvn_fswp(v21r, v22r);
    pvn_fswp(v21i, v22i);
    pvn_fswp(s1, s2);
  }
  pvn_fswp(u21r, u12r);
  pvn_fswp(u21i, u12i);
  *u11i = -*u11i;
  *u21i = -*u21i;
  *u12i = -*u12i;
  *u22i = -*u22i;
  return knd;
}

static inline void ef_mul(int *const e, double *const f, const int e1, const double f1, const int e2, const double f2)
{
  assert(e);
  assert(f);
  *f = (f1 * f2);
  *f = frexp(*f, e);
  *e += (e1 + e2);
}

static inline void ef_div(int *const e, double *const f, const int e1, const double f1, const int e2, const double f2)
{
  assert(e);
  assert(f);
  *f = (f1 / f2);
  *f = frexp(*f, e);
  *e += (e1 - e2);
}

/* f1, f2 >= 0 */
static inline int ef_cmp(const int e1, const double f1, const int e2, const double f2)
{
  if (f2 == 0.0)
    return (f1 != 0.0);
  if (f1 == 0.0)
    return -1;
  if (e1 < e2)
    return -1;
  if (e2 < e1)
    return 1;
  if (f1 < f2)
    return -1;
  if (f2 < f1)
    return 1;
  return 0;
}

static void dlpsv2(const double A11, const double A12, const double A22, double *const tf, double *const cf, double *const sf, double *const tp, double *const cp, double *const sp, double *const s1, double *const s2, int *const es
#ifndef NDEBUG
                   , char *const s
#endif /* !NDEBUG */
                   )
{
  assert(tf);
  assert(cf);
  assert(sf);
  assert(tp);
  assert(cp);
  assert(sp);
  assert(s1);
  assert(s2);
  assert(es);
  int ae = 0, be = 0, abe = 0, a_be = 0, de = 0, ne = 0, t2e = 0;
  double af = 0.0, bf = 0.0, abf = 0.0, a_bf = 0.0, df = 0.0, nf = 0.0, t2f = 0.0, t2 = 0.0;

  /* should never overflow */
  const double a = hypot(A11, A12);
  double b = A22;

  if ((A11 / A12) < (DBL_EPSILON * 0.5)) {
    t2 = ((2.0 * A22) / A12);
    b = -1.0;
  }
  else if ((A11 < A12) && (a == A12)) {
    bf = frexp(b, &be);
    t2 = ((2.0 * A22) / A12);
  }
  else {
    af = frexp(a, &ae);
    bf = frexp(b, &be);
    abf = (a + b);
    if (!isfinite(abf)) {
      abf = ((0.5 * a) + (0.5 * b));
      de = 1;
    }
    abf = frexp(abf, &abe);
    abe += de;

    a_bf = (a - b);
    if (a == b)
      de = 0;
    else if (fabs(a_bf) >= DBL_MIN) {
      a_bf = frexp(a_bf, &a_be);
      de = 1;
    }
    else {
      de = ((DBL_MIN_EXP + DBL_MANT_DIG) - pvn_imin(ae, be));
      a_bf = (scalbn(af, (ae + de)) - scalbn(bf, (be + de)));
      a_bf = frexp(a_bf, &a_be);
      a_be -= de;
      de = -1;
    }

    if (de)
      ef_mul(&de, &df, a_be, a_bf, abe, abf);
    af = frexp(A12, &ae);
    ef_mul(&ne, &nf, ae, af, be, bf);
    ++ne;
    ef_div(&t2e, &t2f, ne, nf, de, df);
    if (isfinite(t2f))
      t2 = scalbn(t2f, t2e);
    else
      t2 = t2f;
  }

  if (isfinite(t2))
    *tf = (t2 / (1.0 + hypot(t2, 1.0)));
  else
    *tf = copysign(1.0, t2);
#ifndef NDEBUG
  if (s) (void)printf("tan(φ)=%s, ", pvn_dtoa(s, *tf));
#endif /* !NDEBUG */
  *cf = hypot(*tf, 1.0);
  *sf = (*tf / *cf);

  *sp = fma(*tf, A22, A12);
  *tp = (*sp / A11);
#ifndef NDEBUG
  if (s) (void)printf("tan(ψ)=%s\n", pvn_dtoa(s, *tp));
#endif /* !NDEBUG */

  if (b < 0.0) {
    af = frexp(A11, &ae);
    bf = frexp(A22, &be);
    df = frexp(*sp, &de);
    /* expect to be (A12 + tan(φ) * A22) == A12, but the LHS has already been computed */
    /* s2 ≈ (A11 * A22) / (A12 + tan(φ) * A22) */
    ef_mul(&ne, &nf, ae, af, be, bf);
    ef_div(&abe, &abf, ne, nf, de, df);
    /* s1 ≈ (A12 + tan(φ) * A22) */
    *s1 = *sp;
    a_bf = frexp(*s1, &a_be);
    if (isfinite(*tp)) {
      /* 1 / cos */
      *cp = hypot(*tp, 1.0);
      *sp = (*tp / *cp);
      *cp = (1.0 / *cp);
    }
    else {
      ef_div(&t2e, &t2f, de, df, ae, af);
      /* tan(ψ) so large that sec(ψ) ≈ |tan(ψ)| */
      t2f = fabs(t2f);
      *sp = copysign(1.0, *tp);
      ef_div(&ne, &nf, 1, 0.5, t2e, t2f);
      *cp = scalbn(nf, ne);
    }
  }
  else {
    /* 1 / cos */
    *cp = hypot(*tp, 1.0);
    nf = frexp(*cf, &ne);
    df = frexp(*cp, &de);
    ef_div(&ae, &af, ne, nf, de, df);
    /* s2 = A22 * (cf / cp) */
    ef_mul(&abe, &abf, be, bf, ae, af);
    bf = frexp(A11, &be);
    /* s1 = A11 * (cp / cf) */
    ef_div(&a_be, &a_bf, be, bf, ae, af);
    *sp = (*tp / *cp);
    *cp = (1.0 / *cp);
  }

  *cf = (1.0 / *cf);
#ifndef NDEBUG
  if (s) {
    (void)printf("cos(φ)=%s, ", pvn_dtoa(s, *cf));
    (void)printf("sin(φ)=%s\n", pvn_dtoa(s, *sf));
    (void)printf("cos(ψ)=%s, ", pvn_dtoa(s, *cf));
    (void)printf("sin(ψ)=%s\n", pvn_dtoa(s, *sf));
  }
#endif /* !NDEBUG */

  *s1 = a_bf;
  es[1] = a_be;
  *s2 = abf;
  es[2] = abe;
}

int
#ifdef _WIN32
PVN_DLJSV2
#else /* !_WIN32 */
pvn_dljsv2_
#endif /* ?_WIN32 */
(const double *const a11, const double *const a21, const double *const a12, const double *const a22,
 double *const u11, double *const u21, double *const u12, double *const u22,
 double *const v11, double *const v21, double *const v12, double *const v22,
 double *const s1, double *const s2, int *const es)
{
  assert(a11);
  assert(a21);
  assert(a12);
  assert(a22);
  assert(u11);
  assert(u21);
  assert(u12);
  assert(u22);
  assert(v11);
  assert(v21);
  assert(v12);
  assert(v22);
  assert(s1);
  assert(s2);
  assert(es);
  if (!isfinite(*a11))
    return -1;
  if (!isfinite(*a21))
    return -2;
  if (!isfinite(*a12))
    return -3;
  if (!isfinite(*a22))
    return -4;
  /* should not fail beyond this point when *es == 0 */
  int knd = 0, mxe = INT_MIN, e = INT_MIN;
  if (*a11 != 0.0) {
    knd |= 1;
    (void)frexp(*a11, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21 != 0.0) {
    knd |= 2;
    (void)frexp(*a21, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12 != 0.0) {
    knd |= 4;
    (void)frexp(*a12, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22 != 0.0) {
    knd |= 8;
    (void)frexp(*a22, &e);
    if (mxe < e)
      mxe = e;
  }
  switch (knd) {
  case  0:
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  3:
  case  5:
  case  7:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
    e = (DBL_MAX_EXP - mxe - 1);
    break;
  case 15:
    e = (DBL_MAX_EXP - mxe - 2);
    break;
  default:
    return INT_MIN;
  }
  mxe = *es;
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;
  es[2] = es[1] = 0;

  /* scaling by 2^(*es), can only fail when mxe != 0 */
  double A11 = *a11, A21 = *a21, A12 = *a12, A22 = *a22;
  if (*es) {
    A11 = scalbn(A11, *es);
    A21 = scalbn(A21, *es);
    A12 = scalbn(A12, *es);
    A22 = scalbn(A22, *es);
    if (mxe) {
      if (!isfinite(A11))
        return -5;
      if (!isfinite(A21))
        return -6;
      if (!isfinite(A12))
        return -7;
      if (!isfinite(A22))
        return -8;
    }
    if (*es < 0) {
      knd = 0;
      if (A11 != 0.0)
        knd |= 1;
      if (A21 != 0.0)
        knd |= 2;
      if (A12 != 0.0)
        knd |= 4;
      if (A22 != 0.0)
        knd |= 8;
    }
  }

  *u11 = 1.0;
  *u21 = 0.0;
  *u12 = 0.0;
  *u22 = 1.0;
  *v11 = 1.0;
  *v21 = 0.0;
  *v12 = 0.0;
  *v22 = 1.0;
  *s1 = 0.0;
  *s2 = 0.0;

  /* simplify the form of A */
  switch (knd) {
  case  0:
    /* [ 0 0 ] */
    /* [ 0 0 ] */
    *u11 = copysign(1.0, A11);
    *u22 = copysign(1.0, A22);
    A22 = A12 = A21 = A11 = 0.0;
    break;
  case  1:
    /* [ * 0 ] */
    /* [ 0 0 ] */
    if (A11 < 0.0) {
      *u11 = -1.0;
      A11 = -A11;
    }
    *u22 = copysign(1.0, A22);
    A22 = A12 = A21 = 0.0;
    *s1 = A11;
    break;
  case  2:
    /* [ 0 0 ] */
    /* [ * 0 ] */
    *u11 = 0.0;
    *u22 = 0.0;
    A11 = A21;
    A22 = A12;
    if (A11 < 0.0) {
      *u12 = -1.0;
      A11 = -A11;
    }
    else
      *u12 = 1.0;
    *u21 = copysign(1.0, A12);
    A22 = A12 = A21 = 0.0;
    *s1 = A11;
    break;
  case  3:
    /* [ * 0 ] */
    /* [ * 0 ] */
    A12 = fabs(A11);
    A22 = fabs(A21);
    if (A12 < A22) {
      *u11 = 0.0;
      *u21 = copysign(1.0, A11);
      *u12 = copysign(1.0, A21);
      *u22 = 0.0;
      A11 = A22;
      A21 = A12;
    }
    else {
      *u11 = copysign(1.0, A11);
      *u22 = copysign(1.0, A21);
      A11 = A12;
      A21 = A22;
    }
    A22 = A12 = 0.0;
    *s1 = hypot(A11, A21);
    e = 3;
    break;
  case  4:
    /* [ 0 * ] */
    /* [ 0 0 ] */
    *u11 = copysign(1.0, A12);
    *u22 = copysign(1.0, A21);
    A11 = fabs(A12);
    A22 = A12 = A21 = 0.0;
    *v11 = 0.0;
    *v21 = 1.0;
    *v12 = 1.0;
    *v22 = 0.0;
    *s1 = A11;
    break;
  case  5:
    /* [ * * ] */
    /* [ 0 0 ] */
    A21 = fabs(A11);
    A22 = fabs(A12);
    if (A21 < A22) {
      *v11 = 0.0;
      *v21 = copysign(1.0, A12);
      *v12 = copysign(1.0, A11);
      *v22 = 0.0;
      A11 = A22;
      A12 = A21;
    }
    else {
      *v11 = copysign(1.0, A11);
      *v22 = copysign(1.0, A12);
      A11 = A21;
      A12 = A22;
    }
    A22 = A21 = 0.0;
    *s1 = hypot(A11, A12);
    e = 5;
    break;
  case  6:
    /* [ 0 * ] */
    /* [ * 0 ] */
    *u11 = copysign(1.0, A12);
    *u22 = copysign(1.0, A21);
    A11 = fabs(A12);
    A22 = fabs(A21);
    A12 = A21 = 0.0;
    *v11 = 0.0;
    *v21 = 1.0;
    *v12 = 1.0;
    *v22 = 0.0;
    *s1 = A11;
    *s2 = A22;
    break;
  case  7:
    /* [ * * ] */
    /* [ * 0 ] */
    A22 = A11;
    A11 = A12;
    A12 = A22;
    A22 = A21;
    A21 = 0.0;
    *v11 = 0.0;
    *v22 = 0.0;
    if (A11 < 0.0) {
      A11 = -A11;
      *v21 = -1.0;
    }
    else
      *v21 = 1.0;
    if (A12 < 0.0) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -1.0;
    }
    else
      *v12 = 1.0;
    if (A22 < 0.0) {
      *u22 = -1.0;
      A22 = -A22;
    }
    e = 13;
    break;
  case  8:
    /* [ 0 0 ] */
    /* [ 0 * ] */
    *u11 = 0.0;
    *u21 = copysign(1.0, A11);
    *u12 = copysign(1.0, A22);
    *u22 = 0.0;
    A11 = fabs(A22);
    A22 = A12 = A21 = 0.0;
    *v11 = 0.0;
    *v21 = 1.0;
    *v12 = 1.0;
    *v22 = 0.0;
    *s1 = A11;
    break;
  case  9:
    /* [ * 0 ] */
    /* [ 0 * ] */
    if (A11 < 0.0) {
      *u11 = -1.0;
      A11 = -A11;
    }
    if (A22 < 0.0) {
      *u22 = -1.0;
      A22 = -A22;
    }
    A12 = A21 = 0.0;
    *s1 = A11;
    *s2 = A22;
    break;
  case 10:
    /* [ 0 0 ] */
    /* [ * * ] */
    *u11 = 0.0;
    *u21 = 1.0;
    *u12 = 1.0;
    *u22 = 0.0;
    A11 = A21;
    A12 = A22;
    A21 = fabs(A11);
    A22 = fabs(A12);
    if (A21 < A22) {
      *v11 = 0.0;
      *v21 = copysign(1.0, A12);
      *v12 = copysign(1.0, A11);
      *v22 = 0.0;
      A11 = A22;
      A12 = A21;
    }
    else {
      *v11 = copysign(1.0, A11);
      *v22 = copysign(1.0, A12);
      A11 = A21;
      A12 = A22;
    }
    A22 = A21 = 0.0;
    *s1 = hypot(A11, A12);
    e = 5;
    break;
  case 11:
    /* [ * 0 ] */
    /* [ * * ] */
    *u11 = 0.0;
    *u12 = 1.0;
    *u22 = 0.0;
    A12 = A11;
    A11 = A22;
    A22 = A12;
    A12 = A21;
    A21 = 0.0;
    *v11 = 0.0;
    *v22 = 0.0;
    if (A11 < 0.0) {
      A11 = -A11;
      *v21 = -1.0;
    }
    else
      *v21 = 1.0;
    if (A12 < 0.0) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -1.0;
    }
    else
      *v12 = 1.0;
    if (A22 < 0.0) {
      *u21 = -1.0;
      A22 = -A22;
    }
    else
      *u21 = 1.0;
    e = 13;
    break;
  case 12:
    /* [ 0 * ] */
    /* [ 0 * ] */
    A11 = A12;
    A21 = A22;
    *v11 = 0.0;
    *v21 = 1.0;
    *v12 = 1.0;
    *v22 = 0.0;
    A12 = fabs(A11);
    A22 = fabs(A21);
    if (A12 < A22) {
      *u11 = 0.0;
      *u21 = copysign(1.0, A11);
      *u12 = copysign(1.0, A21);
      *u22 = 0.0;
      A11 = A22;
      A21 = A12;
    }
    else {
      *u11 = copysign(1.0, A11);
      *u22 = copysign(1.0, A21);
      A11 = A12;
      A21 = A22;
    }
    A22 = A12 = 0.0;
    *s1 = hypot(A11, A21);
    e = 3;
    break;
  case 13:
    /* [ * * ] */
    /* [ 0 * ] */
    if (A11 < 0.0) {
      A11 = -A11;
      *v11 = -1.0;
    }
    if (A12 < 0.0) {
      A12 = -A12;
      A22 = -A22;
      *v22 = -1.0;
    }
    if (A22 < 0.0) {
      *u22 = -1.0;
      A22 = -A22;
    }
    A21 = 0.0;
    e = 13;
    break;
  case 14:
    /* [ 0 * ] */
    /* [ * * ] */
    *u11 = 0.0;
    *u12 = 1.0;
    *u22 = 0.0;
    A11 = A12;
    A12 = A22;
    A22 = A11;
    A11 = A21;
    A21 = 0.0;
    if (A11 < 0.0) {
      A11 = -A11;
      *v11 = -1.0;
    }
    if (A12 < 0.0) {
      A12 = -A12;
      A22 = -A22;
      *v22 = -1.0;
    }
    if (A22 < 0.0) {
      *u21 = -1.0;
      A22 = -A22;
    }
    else
      *u21 = 1.0;
    e = 13;
    break;
  case 15:
    /* [ * * ] */
    /* [ * * ] */
    e = 15;
    break;
  default:
    return INT_MIN;
  }

  if ((e == 13) && (A11 < A22))
    e = -13;

#ifndef NDEBUG
  char s[26] = { '\0' };
  (void)printf("\tA[knd=%d,e=%d] * 2^%d =\n", knd, e, *es);
  (void)printf("%s ", pvn_dtoa(s, A11));
  (void)printf("%s\n", pvn_dtoa(s, A12));
  (void)printf("%s ", pvn_dtoa(s, A21));
  (void)printf("%s\n", pvn_dtoa(s, A22));
#endif /* !NDEBUG */

  double tt = 0.0, ct = 1.0, st = 0.0;

  if (e == 15) {
    /* [ * * ] */
    /* [ * * ] */
    *s1 = hypot(A11, A21);
    *s2 = hypot(A12, A22);
    if (*s1 < *s2) {
      pvn_dswp(&A11, &A12);
      pvn_dswp(&A21, &A22);
      pvn_dswp(v11, v12);
      pvn_dswp(v21, v22);
      pvn_dswp(s1, s2);
    }
    if (copysign(1.0, A11) != 1.0) {
      *u11 = -*u11;
      *u12 = -*u12;
      A11 = -A11;
      A12 = -A12;
    }
    if (copysign(1.0, A21) != 1.0) {
      *u21 = -*u21;
      *u22 = -*u22;
      A21 = -A21;
      A22 = -A22;
    }
    if (A11 < A21) {
      pvn_dswp(u11, u21);
      pvn_dswp(u12, u22);
      pvn_dswp(&A11, &A21);
      pvn_dswp(&A12, &A22);
    }
    /* U^T(ϑ):
        cos(ϑ)  sin(ϑ)
       -sin(ϑ)  cos(ϑ)
    */
    tt = (A21 / A11);
    /* 1 / cos */
    ct = hypot(tt, 1.0);
    /* apply the left Givens rotation to A (and maybe to U) */
    st = -tt;
    A21 = A12;
    if (ct == 1.0) {
      A12 = fma(tt, A22, A12);
      A22 = fma(st, A21, A22);
      if ((A12 == 0.0) || (A22 == 0.0)) {
        A21 = *u11;
        *u11 = fma(tt, *u21, *u11);
        *u21 = fma(st,  A21, *u21);
        A21 = *u12;
        *u12 = fma(tt, *u22, *u12);
        *u22 = fma(st,  A21, *u22);
      }
      st = tt;
    }
    else {
      A12 = (fma(tt, A22, A12) / ct);
      A22 = (fma(st, A21, A22) / ct);
      if ((A12 == 0.0) || (A22 == 0.0)) {
        A21 = *u11;
        *u11 = (fma(tt, *u21, *u11) / ct);
        *u21 = (fma(st,  A21, *u21) / ct);
        A21 = *u12;
        *u12 = (fma(tt, *u22, *u12) / ct);
        *u22 = (fma(st,  A21, *u22) / ct);
      }
      st = (tt / ct);
      ct = (1.0 / ct);
    }
    A11 = *s1;
    A21 = 0.0;
    if (A12 == 0.0) {
      A12 = 0.0;
      if (copysign(1.0, A22) != 1.0) {
        *u21 = -*u21;
        *u22 = -*u22;
        A22 = -A22;
      }
      e = 0;
    }
    else if (A22 == 0.0) {
      if (copysign(1.0, A12) != 1.0) {
        A12 = -A12;
        A22 = -A22;
        *v12 = -*v12;
        *v22 = -*v22;
      }
      if (copysign(1.0, A22) != 1.0) {
        *u21 = -*u21;
        *u22 = -*u22;
        A22 = 0.0;
      }
      *s1 = hypot(*s1, *s2);
      *s2 = 0.0;
      e = 5;
    }
    else
      e = 13;
    if (A12 < 0.0) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -*v12;
      *v22 = -*v22;
    }
    if (A22 < 0.0) {
      A22 = -A22;
      /* sin(ϑ) is always non-negative by construction */
      /* this is just an extra bit of info, used later */
      st = -st;
    }
    if ((e == 13) && (A11 < A22))
      e = -13;
#ifndef NDEBUG
    (void)printf("tan(ϑ)=%s, ", pvn_dtoa(s, tt));
    (void)printf("cos(ϑ)=%s, ", pvn_dtoa(s, ct));
    (void)printf("sin(ϑ)=%s\n", pvn_dtoa(s, st));
#endif /* !NDEBUG */
  }

  if (e == 3) {
    /* [ X 0 ] */
    /* [ x 0 ] */
    /* U^T(ϑ):
        cos(ϑ)  sin(ϑ)
       -sin(ϑ)  cos(ϑ)
    */
    tt = (A21 / A11);
    /* 1 / cos */
    ct = hypot(tt, 1.0);
    /* apply the left Givens rotation to U */
    st = -tt;
    A21 = *u11;
    if (ct == 1.0) {
      *u11 = fma(tt, *u21, *u11);
      *u21 = fma(st,  A21, *u21);
      A21 = *u12;
      *u12 = fma(tt, *u22, *u12);
      *u22 = fma(st,  A21, *u22);
      st = tt;
    }
    else {
      *u11 = (fma(tt, *u21, *u11) / ct);
      *u21 = (fma(st,  A21, *u21) / ct);
      A21 = *u12;
      *u12 = (fma(tt, *u22, *u12) / ct);
      *u22 = (fma(st,  A21, *u22) / ct);
      st = (tt / ct);
      ct = (1.0 / ct);
    }
    A11 = *s1;
    A21 = 0.0;
    e = 0;
#ifndef NDEBUG
    (void)printf("tan(ϑ)=%s, ", pvn_dtoa(s, tt));
    (void)printf("cos(ϑ)=%s, ", pvn_dtoa(s, ct));
    (void)printf("sin(ϑ)=%s\n", pvn_dtoa(s, st));
#endif /* !NDEBUG */
  }

  if (e == 5) {
    /* [ X x ] */
    /* [ 0 0 ] */
    /* V(θ):
       cos(θ) -sin(θ)
       sin(θ)  cos(θ)
    */
    tt = (A12 / A11);
    /* 1 / cos */
    ct = hypot(tt, 1.0);
    /* apply the right Givens rotation to V */
    st = -tt;
    A12 = *v11;
    if (ct == 1.0) {
      *v11 = fma(tt, *v12, *v11);
      *v12 = fma(st,  A12, *v12);
      A12 = *v21;
      *v21 = fma(tt, *v22, *v21);
      *v22 = fma(st,  A12, *v22);
      st = tt;
    }
    else {
      *v11 = (fma(tt, *v12, *v11) / ct);
      *v12 = (fma(st,  A12, *v12) / ct);
      A12 = *v21;
      *v21 = (fma(tt, *v22, *v21) / ct);
      *v22 = (fma(st,  A12, *v22) / ct);
      st = (tt / ct);
      ct = (1.0 / ct);
    }
    A11 = *s1;
    A12 = 0.0;
    e = 0;
#ifndef NDEBUG
    (void)printf("tan(θ)=%s, ", pvn_dtoa(s, tt));
    (void)printf("cos(θ)=%s, ", pvn_dtoa(s, ct));
    (void)printf("sin(θ)=%s\n", pvn_dtoa(s, st));
#endif /* !NDEBUG */
  }

#ifndef NDEBUG
  (void)printf("\tA =\n");
  (void)printf("%s ", pvn_dtoa(s, scalbn(A11, -*es)));
  (void)printf("%s\n", pvn_dtoa(s, scalbn(A12, -*es)));
  (void)printf("%s ", pvn_dtoa(s, scalbn(A21, -*es)));
  (void)printf("%s\n", pvn_dtoa(s, scalbn(A22, -*es)));
#endif /* !NDEBUG */

  if (abs(e) == 13) {
    /* [ x y ] */
    /* [ 0 z ] */

    double tf = 0.0, cf = 1.0, sf = 0.0, tp = 0.0, cp = 1.0, sp = 0.0;
    if (e == -13) {
      double tf_ = 0.0, cf_ = 1.0, sf_ = 0.0, tp_ = 0.0, cp_ = 1.0, sp_ = 0.0;
      dlpsv2(A22, A12, A11, &tf_, &cf_, &sf_, &tp_, &cp_, &sp_, s1, s2, es
#ifndef NDEBUG
             , s
#endif /* !NDEBUG */
             );
      tf = (1.0 / tp_);
      cf = sp_;
      sf = cp_;
      tp = (1.0 / tf_);
      cp = sf_;
      sp = cf_;
    }
    else
      dlpsv2(A11, A12, A22, &tf, &cf, &sf, &tp, &cp, &sp, s1, s2, es
#ifndef NDEBUG
             , s
#endif /* !NDEBUG */
             );

    /* update U */
    if (copysign(1.0, st) != 1.0) {
      /* U^T(φ) * diag(1, -1) * U^T(ϑ):
          cos(φ - ϑ) -sin(φ - ϑ)
         -sin(φ - ϑ) -cos(φ - ϑ)
      */
      st = -st;
      double tf_t = (tf - tt), cf_t = 1.0, sf_t = 0.0;
      if (tf_t != 0.0) {
        tf_t /= fma(tf, tt, 1.0);
        if (isfinite(tf_t)) {
          /* 1 / cos */
          cf_t = hypot(tf_t, 1.0);
          sf_t = (tf_t / cf_t);
          cf_t = (1.0 / cf_t);
        }
        else {
          sf_t = copysign(1.0, tf_t);
          cf_t = 0.0;
        }
      }
      else
        sf_t = tf_t;
      const double _sf_t = -sf_t;
      A21 = *u11;
      *u11 = (_sf_t * *u21 + cf_t * *u11);
      *u21 = (_sf_t *  A21 - cf_t * *u21);
      A21 = *u12;
      *u12 = (_sf_t * *u22 + cf_t * *u12);
      *u22 = (_sf_t *  A21 - cf_t * *u22);
      A21 = -1.0;
    }
    else if (tt != 0.0) {
      /* U^T(φ) * U^T(ϑ) = U^T(φ + ϑ):
          cos(φ + ϑ)  sin(φ + ϑ)
         -sin(φ + ϑ)  cos(φ + ϑ)
      */
      double tft = (tf + tt), cft = 1.0, sft = 0.0;
      if (tft != 0.0) {
        tft /= fma(-tf, tt, 1.0);
        if (isfinite(tft)) {
          /* 1 / cos */
          cft = hypot(tft, 1.0);
          sft = (tft / cft);
          cft = (1.0 / cft);
        }
        else {
          sft = copysign(1.0, tft);
          cft = 0.0;
        }
      }
      else
        sft = tft;
      A21 = *u11;
      *u11 = (cft * *u11 + sft * *u21);
      *u21 = (cft * *u21 - sft *  A21);
      A21 = *u12;
      *u12 = (cft * *u12 + sft * *u22);
      *u22 = (cft * *u22 - sft *  A21);
      A21 = 1.0;
    }
    else if (tf != 0.0) {
      /* U^T(φ):
          cos(φ)  sin(φ)
         -sin(φ)  cos(φ)
       */
      A21 = *u11;
      *u11 = (cf * *u11 + sf * *u21);
      *u21 = (cf * *u21 - sf *  A21);
      A21 = *u12;
      *u12 = (cf * *u12 + sf * *u22);
      *u22 = (cf * *u22 - sf *  A21);
      A21 = -0.0;
    }
    else /* U^T(φ) = I */
      A21 = 0.0;
#ifndef NDEBUG
    (void)printf("U operation=%s\n", pvn_dtoa(s, A21));
#endif /* !NDEBUG */

    /* update V */
    if (tp != 0.0) {
      /* V(ψ):
         cos(ψ) -sin(ψ)
         sin(ψ)  cos(ψ)
      */
      A21 = *v11;
      *v11 = (*v11 * cp + *v12 * sp);
      *v12 = (*v12 * cp -  A21 * sp);
      A21 = *v21;
      *v21 = (*v21 * cp + *v22 * sp);
      *v22 = (*v22 * cp -  A21 * sp);
      A21 = -0.0;
    }
    else /* V(ψ) = I */
      A21 = 0.0;
#ifndef NDEBUG
    (void)printf("V operation=%s\n", pvn_dtoa(s, A21));
#endif /* !NDEBUG */

    if (e == -13) {
      *u21 = -*u21;
      *u22 = -*u22;
      *v12 = -*v12;
      *v22 = -*v22;
    }
  }

  if (ef_cmp(es[1], *s1, es[2], *s2) < 0) {
    pvn_dswp(u11, u21);
    pvn_dswp(u12, u22);
    pvn_dswp(v11, v12);
    pvn_dswp(v21, v22);
    pvn_dswp(s1, s2);
  }
  pvn_dswp(u21, u12);
  return knd;
}

static double cpolar(const double r, const double i, double *const c, double *const s)
{
  assert(c);
  assert(s);
  const double a = hypot(r, i);
  *c = copysign(fmin((fabs(r) / a), 1.0), r);
  *s = (i / fmax(a, DBL_TRUE_MIN));
  return a;
}

int
#ifdef _WIN32
PVN_ZLJSV2
#else /* !_WIN32 */
pvn_zljsv2_
#endif /* ?_WIN32 */
(const double *const a11r, const double *const a11i, const double *const a21r, const double *const a21i, const double *const a12r, const double *const a12i, const double *const a22r, const double *const a22i,
 double *const u11r, double *const u11i, double *const u21r, double *const u21i, double *const u12r, double *const u12i, double *const u22r, double *const u22i,
 double *const v11r, double *const v11i, double *const v21r, double *const v21i, double *const v12r, double *const v12i, double *const v22r, double *const v22i,
 double *const s1, double *const s2, int *const es)
{
  assert(a11r);
  assert(a11i);
  assert(a21r);
  assert(a21i);
  assert(a12r);
  assert(a12i);
  assert(a22r);
  assert(a22i);
  assert(u11r);
  assert(u11i);
  assert(u21r);
  assert(u21i);
  assert(u12r);
  assert(u12i);
  assert(u22r);
  assert(u22i);
  assert(v11r);
  assert(v11i);
  assert(v21r);
  assert(v21i);
  assert(v12r);
  assert(v12i);
  assert(v22r);
  assert(v22i);
  assert(s1);
  assert(s2);
  assert(es);
  if (!isfinite(*a11r))
    return -1;
  if (!isfinite(*a11i))
    return -2;
  if (!isfinite(*a21r))
    return -3;
  if (!isfinite(*a21i))
    return -4;
  if (!isfinite(*a12r))
    return -5;
  if (!isfinite(*a12i))
    return -6;
  if (!isfinite(*a22r))
    return -7;
  if (!isfinite(*a22i))
    return -8;
  /* should not fail beyond this point when *es == 0 */
  int kndi = 0, mxe = INT_MIN, e = 0;
  if (*a11i != 0.0) {
    kndi |= 1;
    (void)frexp(*a11i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21i != 0.0) {
    kndi |= 2;
    (void)frexp(*a21i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12i != 0.0) {
    kndi |= 4;
    (void)frexp(*a12i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22i != 0.0) {
    kndi |= 8;
    (void)frexp(*a22i, &e);
    if (mxe < e)
      mxe = e;
  }
  *u11i = 0.0;
  *u21i = 0.0;
  *u12i = 0.0;
  *u22i = 0.0;
  *v11i = 0.0;
  *v21i = 0.0;
  *v12i = 0.0;
  *v22i = 0.0;
  if (!kndi)
    return
#ifdef _WIN32
      PVN_DLJSV2
#else /* !_WIN32 */
      pvn_dljsv2_
#endif /* ?_WIN32 */
      (a11r, a21r, a12r, a22r, u11r, u21r, u12r, u22r, v11r, v21r, v12r, v22r, s1, s2, es);
  int kndr = 0;
  if (*a11r != 0.0) {
    kndr |= 1;
    (void)frexp(*a11r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21r != 0.0) {
    kndr |= 2;
    (void)frexp(*a21r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12r != 0.0) {
    kndr |= 4;
    (void)frexp(*a12r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22r != 0.0) {
    kndr |= 8;
    (void)frexp(*a22r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (!kndr) {
    *u11r = 0.0;
    *u21r = 0.0;
    *u12r = 0.0;
    *u22r = 0.0;
    return
#ifdef _WIN32
      PVN_DLJSV2
#else /* !_WIN32 */
      pvn_dljsv2_
#endif /* ?_WIN32 */
      (a11i, a21i, a12i, a22i, u11i, u21i, u12i, u22i, v11r, v21r, v12r, v22r, s1, s2, es);
  }
  int knd = (kndr | kndi);
  switch (knd) {
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  3:
  case  5:
  case  7:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    e = (DBL_MAX_EXP - mxe - 2);
    break;
  default:
    return INT_MIN;
  }
  mxe = *es;
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;
  es[2] = es[1] = 0;

  /* scaling by 2^(*es), can only fail when mxe != 0 */
  double A11r = *a11r, A11i = *a11i, A21r = *a21r, A21i = *a21i, A12r = *a12r, A12i = *a12i, A22r = *a22r, A22i = *a22i;
  if (*es) {
    A11r = scalbn(A11r, *es);
    A11i = scalbn(A11i, *es);
    A21r = scalbn(A21r, *es);
    A21i = scalbn(A21i, *es);
    A12r = scalbn(A12r, *es);
    A12i = scalbn(A12i, *es);
    A22r = scalbn(A22r, *es);
    A22i = scalbn(A22i, *es);
    if (mxe) {
      if (!isfinite(A11r))
        return -9;
      if (!isfinite(A11i))
        return -10;
      if (!isfinite(A21r))
        return -11;
      if (!isfinite(A21i))
        return -12;
      if (!isfinite(A12r))
        return -13;
      if (!isfinite(A12i))
        return -14;
      if (!isfinite(A22r))
        return -15;
      if (!isfinite(A22i))
        return -16;
    }
    if (*es < 0) {
      kndr = 0;
      if (A11r != 0.0)
        kndr |= 1;
      if (A21r != 0.0)
        kndr |= 2;
      if (A12r != 0.0)
        kndr |= 4;
      if (A22r != 0.0)
        kndr |= 8;
      kndi = 0;
      if (A11i != 0.0)
        kndi |= 1;
      if (A21i != 0.0)
        kndi |= 2;
      if (A12i != 0.0)
        kndi |= 4;
      if (A22i != 0.0)
        kndi |= 8;
      knd = (kndr | kndi);
    }
  }

  *u11r = 1.0;
  *u21r = 0.0;
  *u12r = 0.0;
  *u22r = 1.0;
  *v11r = 1.0;
  *v21r = 0.0;
  *v12r = 0.0;
  *v22r = 1.0;
  *s1 = 0.0;
  *s2 = 0.0;

  if ((e == 13) && (A11r < A22r))
    e = -13;

  /* TODO */

  if (ef_cmp(es[1], *s1, es[2], *s2) < 0) {
    pvn_dswp(u11r, u21r);
    pvn_dswp(u11i, u21i);
    pvn_dswp(u12r, u22r);
    pvn_dswp(u12i, u22i);
    pvn_dswp(v11r, v12r);
    pvn_dswp(v11i, v12i);
    pvn_dswp(v21r, v22r);
    pvn_dswp(v21i, v22i);
    pvn_dswp(s1, s2);
  }
  pvn_dswp(u21r, u12r);
  pvn_dswp(u21i, u12i);
  *u11i = -*u11i;
  *u21i = -*u21i;
  *u12i = -*u12i;
  *u22i = -*u22i;
  return knd;
}

#ifndef _WIN32
#ifdef XLJR2
#error XLJR2 already defined
#else /* !XLJR2 */
#define XLJR2                                                   \
  assert(a11);                                                  \
  assert(a21);                                                  \
  assert(a12);                                                  \
  assert(a22);                                                  \
  assert(u11);                                                  \
  assert(u21);                                                  \
  assert(u12);                                                  \
  assert(u22);                                                  \
  assert(v11);                                                  \
  assert(v21);                                                  \
  assert(v12);                                                  \
  assert(v22);                                                  \
  assert(s1);                                                   \
  assert(s2);                                                   \
  assert(es);                                                   \
  assert(E);                                                    \
  long double                                                   \
    u11l = *u11, u21l = *u21, u12l = *u12, u22l = *u22,         \
    v11l = *v11, v21l = *v21, v12l = *v12, v22l = *v22,         \
    s1l = scalbnl(*s1, es[1] - es[0]),                          \
    s2l = scalbnl(*s2, es[2] - es[0]);                          \
  /* cond_2(G) */                                               \
  E[0] = fminl((s1l / s2l), INFINITY);                          \
  /* U^T U - I */                                               \
  long double T11 = fmal(u11l, u11l, fmal(u21l, u21l, -1.0L));  \
  long double T21 = fmal(u12l, u11l, u22l * u21l);              \
  long double T12 = T21;                                        \
  long double T22 = fmal(u12l, u12l, fmal(u22l, u22l, -1.0L));  \
  E[1] = hypotl(hypotl(T11, T21), hypotl(T12, T22));            \
  T11 = fmal(v11l, v11l, fmal(v21l, v21l, -1.0L));              \
  T21 = fmal(v12l, v11l, v22l * v21l);                          \
  T12 = T21;                                                    \
  T22 = fmal(v12l, v12l, fmal(v22l, v22l, -1.0L));              \
  E[2] = hypotl(hypotl(T11, T21), hypotl(T12, T22));            \
  u11l *= s1l;                                                  \
  u21l *= s1l;                                                  \
  u12l *= s2l;                                                  \
  u22l *= s2l;                                                  \
  E[3] = hypotl(hypotl(*a11, *a21), hypotl(*a12, *a22));        \
  T11 = fmal(u11l, v11l, fmal(u12l, v12l, -*a11));              \
  T21 = fmal(u21l, v11l, fmal(u22l, v12l, -*a21));              \
  T12 = fmal(u11l, v21l, fmal(u12l, v22l, -*a12));              \
  T22 = fmal(u21l, v21l, fmal(u22l, v22l, -*a22));              \
  E[3] = (hypotl(hypotl(T11, T21), hypotl(T12, T22)) / E[3])
#endif /* ?XLJR2 */
void pvn_sxljr2_
(const float *const a11, const float *const a21, const float *const a12, const float *const a22,
 const float *const u11, const float *const u21, const float *const u12, const float *const u22,
 const float *const v11, const float *const v21, const float *const v12, const float *const v22,
 const float *const s1, const float *const s2, const int *const es, long double *const E)
{
  XLJR2;
}

void pvn_dxljr2_
(const double *const a11, const double *const a21, const double *const a12, const double *const a22,
 const double *const u11, const double *const u21, const double *const u12, const double *const u22,
 const double *const v11, const double *const v21, const double *const v12, const double *const v22,
 const double *const s1, const double *const s2, const int *const es, long double *const E)
{
  XLJR2;
}

static inline void ef_mull(int *const e, long double *const f, const int e1, const long double f1, const int e2, const long double f2)
{
  assert(e);
  assert(f);
  *f = (f1 * f2);
  *f = frexpl(*f, e);
  *e += (e1 + e2);
}

static inline void ef_divl(int *const e, long double *const f, const int e1, const long double f1, const int e2, const long double f2)
{
  assert(e);
  assert(f);
  *f = (f1 / f2);
  *f = frexpl(*f, e);
  *e += (e1 - e2);
}

/* f1, f2 >= 0 */
static inline int ef_cmpl(const int e1, const long double f1, const int e2, const long double f2)
{
  if (f2 == 0.0L)
    return (f1 != 0.0L);
  if (f1 == 0.0L)
    return -1;
  if (e1 < e2)
    return -1;
  if (e2 < e1)
    return 1;
  if (f1 < f2)
    return -1;
  if (f2 < f1)
    return 1;
  return 0;
}

static void xlpsv2(const long double A11, const long double A12, const long double A22, long double *const tf, long double *const cf, long double *const sf, long double *const tp, long double *const cp, long double *const sp, long double *const s1, long double *const s2, int *const es
#ifndef NDEBUG
                   , char *const s
#endif /* !NDEBUG */
                   )
{
  assert(tf);
  assert(cf);
  assert(sf);
  assert(tp);
  assert(cp);
  assert(sp);
  assert(s1);
  assert(s2);
  assert(es);
  int ae = 0, be = 0, abe = 0, a_be = 0, de = 0, ne = 0, t2e = 0;
  long double af = 0.0L, bf = 0.0L, abf = 0.0L, a_bf = 0.0L, df = 0.0L, nf = 0.0L, t2f = 0.0L, t2 = 0.0L;

  /* should never overflow */
  const long double a = hypotl(A11, A12);
  long double b = A22;

  if ((A11 / A12) < (LDBL_EPSILON * 0.5L)) {
    t2 = ((2.0L * A22) / A12);
    b = -1.0L;
  }
  else if ((A11 < A12) && (a == A12)) {
    bf = frexpl(b, &be);
    t2 = ((2.0L * A22) / A12);
  }
  else {
    af = frexpl(a, &ae);
    bf = frexpl(b, &be);
    abf = (a + b);
    if (!isfinite(abf)) {
      abf = ((0.5L * a) + (0.5L * b));
      de = 1;
    }
    abf = frexpl(abf, &abe);
    abe += de;

    a_bf = (a - b);
    if (a == b)
      de = 0;
    else if (fabsl(a_bf) >= LDBL_MIN) {
      a_bf = frexpl(a_bf, &a_be);
      de = 1;
    }
    else {
      de = ((LDBL_MIN_EXP + LDBL_MANT_DIG) - pvn_imin(ae, be));
      a_bf = (scalbnl(af, (ae + de)) - scalbnl(bf, (be + de)));
      a_bf = frexpl(a_bf, &a_be);
      a_be -= de;
      de = -1;
    }

    if (de)
      ef_mull(&de, &df, a_be, a_bf, abe, abf);
    af = frexpl(A12, &ae);
    ef_mull(&ne, &nf, ae, af, be, bf);
    ++ne;
    ef_divl(&t2e, &t2f, ne, nf, de, df);
    if (isfinite(t2f))
      t2 = scalbnl(t2f, t2e);
    else
      t2 = t2f;
  }

  if (isfinite(t2))
    *tf = (t2 / (1.0L + hypotl(t2, 1.0L)));
  else
    *tf = copysignl(1.0L, t2);
#ifndef NDEBUG
  if (s) (void)printf("tan(φ)=%s, ", pvn_xtoa(s, *tf));
#endif /* !NDEBUG */
  *cf = hypotl(*tf, 1.0L);
  *sf = (*tf / *cf);

  *sp = fmal(*tf, A22, A12);
  *tp = (*sp / A11);
#ifndef NDEBUG
  if (s) (void)printf("tan(ψ)=%s\n", pvn_dtoa(s, *tp));
#endif /* !NDEBUG */

  if (b < 0.0L) {
    af = frexpl(A11, &ae);
    bf = frexpl(A22, &be);
    df = frexpl(*sp, &de);
    /* expect to be (A12 + tan(φ) * A22) == A12, but the LHS has already been computed */
    /* s2 ≈ (A11 * A22) / (A12 + tan(φ) * A22) */
    ef_mull(&ne, &nf, ae, af, be, bf);
    ef_divl(&abe, &abf, ne, nf, de, df);
    /* s1 ≈ (A12 + tan(φ) * A22) */
    *s1 = *sp;
    a_bf = frexpl(*s1, &a_be);
    if (isfinite(*tp)) {
      /* 1 / cos */
      *cp = hypotl(*tp, 1.0L);
      *sp = (*tp / *cp);
      *cp = (1.0L / *cp);
    }
    else {
      ef_divl(&t2e, &t2f, de, df, ae, af);
      /* tan(ψ) so large that sec(ψ) ≈ |tan(ψ)| */
      t2f = fabsl(t2f);
      *sp = copysignl(1.0L, *tp);
      ef_divl(&ne, &nf, 1, 0.5L, t2e, t2f);
      *cp = scalbnl(nf, ne);
    }
  }
  else {
    /* 1 / cos */
    *cp = hypotl(*tp, 1.0L);
    nf = frexpl(*cf, &ne);
    df = frexpl(*cp, &de);
    ef_divl(&ae, &af, ne, nf, de, df);
    /* s2 = A22 * (cf / cp) */
    ef_mull(&abe, &abf, be, bf, ae, af);
    bf = frexpl(A11, &be);
    /* s1 = A11 * (cp / cf) */
    ef_divl(&a_be, &a_bf, be, bf, ae, af);
    *sp = (*tp / *cp);
    *cp = (1.0L / *cp);
  }

  *cf = (1.0L / *cf);
#ifndef NDEBUG
  if (s) {
    (void)printf("cos(φ)=%s, ", pvn_xtoa(s, *cf));
    (void)printf("sin(φ)=%s\n", pvn_xtoa(s, *sf));
    (void)printf("cos(ψ)=%s, ", pvn_xtoa(s, *cf));
    (void)printf("sin(ψ)=%s\n", pvn_xtoa(s, *sf));
  }
#endif /* !NDEBUG */

  *s1 = a_bf;
  es[1] = a_be;
  *s2 = abf;
  es[2] = abe;
}

int pvn_xljsv2_
(const long double *const a11, const long double *const a21, const long double *const a12, const long double *const a22,
 long double *const u11, long double *const u21, long double *const u12, long double *const u22,
 long double *const v11, long double *const v21, long double *const v12, long double *const v22,
 long double *const s1, long double *const s2, int *const es)
{
  assert(a11);
  assert(a21);
  assert(a12);
  assert(a22);
  assert(u11);
  assert(u21);
  assert(u12);
  assert(u22);
  assert(v11);
  assert(v21);
  assert(v12);
  assert(v22);
  assert(s1);
  assert(s2);
  assert(es);
  if (!isfinite(*a11))
    return -1;
  if (!isfinite(*a21))
    return -2;
  if (!isfinite(*a12))
    return -3;
  if (!isfinite(*a22))
    return -4;
  int knd = 0, mxe = INT_MIN, e = INT_MIN;
  if (*a11 != 0.0L) {
    knd |= 1;
    (void)frexpl(*a11, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21 != 0.0L) {
    knd |= 2;
    (void)frexpl(*a21, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12 != 0.0L) {
    knd |= 4;
    (void)frexpl(*a12, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22 != 0.0L) {
    knd |= 8;
    (void)frexpl(*a22, &e);
    if (mxe < e)
      mxe = e;
  }
  switch (knd) {
  case  0:
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  3:
  case  5:
  case  7:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
    e = (LDBL_MAX_EXP - mxe - 1);
    break;
  case 15:
    e = (LDBL_MAX_EXP - mxe - 2);
    break;
  default:
    return INT_MIN;
  }
  mxe = *es;
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;
  es[2] = es[1] = 0;

  /* scaling by 2^(*es), can only fail when mxe != 0 */
  long double A11 = *a11, A21 = *a21, A12 = *a12, A22 = *a22;
  if (*es) {
    A11 = scalbnl(A11, *es);
    A21 = scalbnl(A21, *es);
    A12 = scalbnl(A12, *es);
    A22 = scalbnl(A22, *es);
    if (mxe) {
      if (!isfinite(A11))
        return -5;
      if (!isfinite(A21))
        return -6;
      if (!isfinite(A12))
        return -7;
      if (!isfinite(A22))
        return -8;
    }
    if (*es < 0) {
      knd = 0;
      if (A11 != 0.0L)
        knd |= 1;
      if (A21 != 0.0L)
        knd |= 2;
      if (A12 != 0.0L)
        knd |= 4;
      if (A22 != 0.0L)
        knd |= 8;
    }
  }

  *u11 = 1.0L;
  *u21 = 0.0L;
  *u12 = 0.0L;
  *u22 = 1.0L;
  *v11 = 1.0L;
  *v21 = 0.0L;
  *v12 = 0.0L;
  *v22 = 1.0L;
  *s1 = 0.0L;
  *s2 = 0.0L;

  /* simplify the form of A */
  switch (knd) {
  case  0:
    /* [ 0 0 ] */
    /* [ 0 0 ] */
    *u11 = copysignl(1.0L, A11);
    *u22 = copysignl(1.0L, A22);
    A22 = A12 = A21 = A11 = 0.0L;
    break;
  case  1:
    /* [ * 0 ] */
    /* [ 0 0 ] */
    if (A11 < 0.0L) {
      *u11 = -1.0L;
      A11 = -A11;
    }
    *u22 = copysignl(1.0L, A22);
    A22 = A12 = A21 = 0.0L;
    *s1 = A11;
    break;
  case  2:
    /* [ 0 0 ] */
    /* [ * 0 ] */
    *u11 = 0.0L;
    *u22 = 0.0L;
    A11 = A21;
    A22 = A12;
    if (A11 < 0.0L) {
      *u12 = -1.0L;
      A11 = -A11;
    }
    else
      *u12 = 1.0L;
    *u21 = copysignl(1.0L, A12);
    A22 = A12 = A21 = 0.0L;
    *s1 = A11;
    break;
  case  3:
    /* [ * 0 ] */
    /* [ * 0 ] */
    A12 = fabsl(A11);
    A22 = fabsl(A21);
    if (A12 < A22) {
      *u11 = 0.0L;
      *u21 = copysignl(1.0L, A11);
      *u12 = copysignl(1.0L, A21);
      *u22 = 0.0L;
      A11 = A22;
      A21 = A12;
    }
    else {
      *u11 = copysignl(1.0L, A11);
      *u22 = copysignl(1.0L, A21);
      A11 = A12;
      A21 = A22;
    }
    A22 = A12 = 0.0L;
    *s1 = hypotl(A11, A21);
    e = 3;
    break;
  case  4:
    /* [ 0 * ] */
    /* [ 0 0 ] */
    *u11 = copysignl(1.0L, A12);
    *u22 = copysignl(1.0L, A21);
    A11 = fabsl(A12);
    A22 = A12 = A21 = 0.0L;
    *v11 = 0.0L;
    *v21 = 1.0L;
    *v12 = 1.0L;
    *v22 = 0.0L;
    *s1 = A11;
    break;
  case  5:
    /* [ * * ] */
    /* [ 0 0 ] */
    A21 = fabsl(A11);
    A22 = fabsl(A12);
    if (A21 < A22) {
      *v11 = 0.0L;
      *v21 = copysignl(1.0L, A12);
      *v12 = copysignl(1.0L, A11);
      *v22 = 0.0L;
      A11 = A22;
      A12 = A21;
    }
    else {
      *v11 = copysignl(1.0L, A11);
      *v22 = copysignl(1.0L, A12);
      A11 = A21;
      A12 = A22;
    }
    A22 = A21 = 0.0L;
    *s1 = hypotl(A11, A12);
    e = 5;
    break;
  case  6:
    /* [ 0 * ] */
    /* [ * 0 ] */
    *u11 = copysignl(1.0L, A12);
    *u22 = copysignl(1.0L, A21);
    A11 = fabsl(A12);
    A22 = fabsl(A21);
    A12 = A21 = 0.0L;
    *v11 = 0.0L;
    *v21 = 1.0L;
    *v12 = 1.0L;
    *v22 = 0.0L;
    *s1 = A11;
    *s2 = A22;
    break;
  case  7:
    /* [ * * ] */
    /* [ * 0 ] */
    A22 = A11;
    A11 = A12;
    A12 = A22;
    A22 = A21;
    A21 = 0.0L;
    *v11 = 0.0L;
    *v22 = 0.0L;
    if (A11 < 0.0L) {
      A11 = -A11;
      *v21 = -1.0L;
    }
    else
      *v21 = 1.0L;
    if (A12 < 0.0L) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -1.0L;
    }
    else
      *v12 = 1.0L;
    if (A22 < 0.0L) {
      *u22 = -1.0L;
      A22 = -A22;
    }
    e = 13;
    break;
  case  8:
    /* [ 0 0 ] */
    /* [ 0 * ] */
    *u11 = 0.0L;
    *u21 = copysignl(1.0L, A11);
    *u12 = copysignl(1.0L, A22);
    *u22 = 0.0L;
    A11 = fabsl(A22);
    A22 = A12 = A21 = 0.0L;
    *v11 = 0.0L;
    *v21 = 1.0L;
    *v12 = 1.0L;
    *v22 = 0.0L;
    *s1 = A11;
    break;
  case  9:
    /* [ * 0 ] */
    /* [ 0 * ] */
    if (A11 < 0.0L) {
      *u11 = -1.0L;
      A11 = -A11;
    }
    if (A22 < 0.0L) {
      *u22 = -1.0L;
      A22 = -A22;
    }
    A12 = A21 = 0.0L;
    *s1 = A11;
    *s2 = A22;
    break;
  case 10:
    /* [ 0 0 ] */
    /* [ * * ] */
    *u11 = 0.0L;
    *u21 = 1.0L;
    *u12 = 1.0L;
    *u22 = 0.0L;
    A11 = A21;
    A12 = A22;
    A21 = fabsl(A11);
    A22 = fabsl(A12);
    if (A21 < A22) {
      *v11 = 0.0L;
      *v21 = copysignl(1.0L, A12);
      *v12 = copysignl(1.0L, A11);
      *v22 = 0.0L;
      A11 = A22;
      A12 = A21;
    }
    else {
      *v11 = copysignl(1.0L, A11);
      *v22 = copysignl(1.0L, A12);
      A11 = A21;
      A12 = A22;
    }
    A22 = A21 = 0.0L;
    *s1 = hypotl(A11, A12);
    e = 5;
    break;
  case 11:
    /* [ * 0 ] */
    /* [ * * ] */
    *u11 = 0.0L;
    *u12 = 1.0L;
    *u22 = 0.0L;
    A12 = A11;
    A11 = A22;
    A22 = A12;
    A12 = A21;
    A21 = 0.0L;
    *v11 = 0.0L;
    *v22 = 0.0L;
    if (A11 < 0.0L) {
      A11 = -A11;
      *v21 = -1.0L;
    }
    else
      *v21 = 1.0L;
    if (A12 < 0.0L) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -1.0L;
    }
    else
      *v12 = 1.0L;
    if (A22 < 0.0L) {
      *u21 = -1.0L;
      A22 = -A22;
    }
    else
      *u21 = 1.0L;
    e = 13;
    break;
  case 12:
    /* [ 0 * ] */
    /* [ 0 * ] */
    A11 = A12;
    A21 = A22;
    *v11 = 0.0L;
    *v21 = 1.0L;
    *v12 = 1.0L;
    *v22 = 0.0L;
    A12 = fabsl(A11);
    A22 = fabsl(A21);
    if (A12 < A22) {
      *u11 = 0.0L;
      *u21 = copysignl(1.0L, A11);
      *u12 = copysignl(1.0L, A21);
      *u22 = 0.0L;
      A11 = A22;
      A21 = A12;
    }
    else {
      *u11 = copysignl(1.0L, A11);
      *u22 = copysignl(1.0L, A21);
      A11 = A12;
      A21 = A22;
    }
    A22 = A12 = 0.0L;
    *s1 = hypotl(A11, A21);
    e = 3;
    break;
  case 13:
    /* [ * * ] */
    /* [ 0 * ] */
    if (A11 < 0.0L) {
      A11 = -A11;
      *v11 = -1.0L;
    }
    if (A12 < 0.0L) {
      A12 = -A12;
      A22 = -A22;
      *v22 = -1.0L;
    }
    if (A22 < 0.0L) {
      *u22 = -1.0L;
      A22 = -A22;
    }
    A21 = 0.0L;
    e = 13;
    break;
  case 14:
    /* [ 0 * ] */
    /* [ * * ] */
    *u11 = 0.0L;
    *u12 = 1.0L;
    *u22 = 0.0L;
    A11 = A12;
    A12 = A22;
    A22 = A11;
    A11 = A21;
    A21 = 0.0L;
    if (A11 < 0.0L) {
      A11 = -A11;
      *v11 = -1.0L;
    }
    if (A12 < 0.0L) {
      A12 = -A12;
      A22 = -A22;
      *v22 = -1.0L;
    }
    if (A22 < 0.0L) {
      *u21 = -1.0L;
      A22 = -A22;
    }
    else
      *u21 = 1.0L;
    e = 13;
    break;
  case 15:
    /* [ * * ] */
    /* [ * * ] */
    e = 15;
    break;
  default:
    return INT_MIN;
  }

  if ((e == 13) && (A11 < A22))
    e = -13;

#ifndef NDEBUG
#ifdef __x86_64__
  char s[31] = { '\0' };
#else /* !__x86_64__ */
  char s[46] = { '\0' };
#endif /* ?__x86_64__ */
  (void)printf("\tA[knd=%d,e=%d] * 2^%d =\n", knd, e, *es);
  (void)printf("%s ", pvn_xtoa(s, A11));
  (void)printf("%s\n", pvn_xtoa(s, A12));
  (void)printf("%s ", pvn_xtoa(s, A21));
  (void)printf("%s\n", pvn_xtoa(s, A22));
#endif /* !NDEBUG */

  long double tt = 0.0L, ct = 1.0L, st = 0.0L;

  if (e == 15) {
    /* [ * * ] */
    /* [ * * ] */
    *s1 = hypotl(A11, A21);
    *s2 = hypotl(A12, A22);
    if (*s1 < *s2) {
      pvn_Lswp(&A11, &A12);
      pvn_Lswp(&A21, &A22);
      pvn_Lswp(v11, v12);
      pvn_Lswp(v21, v22);
      pvn_Lswp(s1, s2);
    }
    if (copysignl(1.0L, A11) != 1.0L) {
      *u11 = -*u11;
      *u12 = -*u12;
      A11 = -A11;
      A12 = -A12;
    }
    if (copysignl(1.0L, A21) != 1.0L) {
      *u21 = -*u21;
      *u22 = -*u22;
      A21 = -A21;
      A22 = -A22;
    }
    if (A11 < A21) {
      pvn_Lswp(u11, u21);
      pvn_Lswp(u12, u22);
      pvn_Lswp(&A11, &A21);
      pvn_Lswp(&A12, &A22);
    }
    /* U^T(ϑ):
        cos(ϑ)  sin(ϑ)
       -sin(ϑ)  cos(ϑ)
    */
    tt = (A21 / A11);
    /* 1 / cos */
    ct = hypotl(tt, 1.0L);
    /* apply the left Givens rotation to A (and maybe to U) */
    st = -tt;
    A21 = A12;
    if (ct == 1.0L) {
      A12 = fmal(tt, A22, A12);
      A22 = fmal(st, A21, A22);
      if ((A12 == 0.0L) || (A22 == 0.0L)) {
        A21 = *u11;
        *u11 = fmal(tt, *u21, *u11);
        *u21 = fmal(st,  A21, *u21);
        A21 = *u12;
        *u12 = fmal(tt, *u22, *u12);
        *u22 = fmal(st,  A21, *u22);
      }
      st = tt;
    }
    else {
      A12 = (fmal(tt, A22, A12) / ct);
      A22 = (fmal(st, A21, A22) / ct);
      if ((A12 == 0.0L) || (A22 == 0.0L)) {
        A21 = *u11;
        *u11 = (fmal(tt, *u21, *u11) / ct);
        *u21 = (fmal(st,  A21, *u21) / ct);
        A21 = *u12;
        *u12 = (fmal(tt, *u22, *u12) / ct);
        *u22 = (fmal(st,  A21, *u22) / ct);
      }
      st = (tt / ct);
      ct = (1.0L / ct);
    }
    A11 = *s1;
    A21 = 0.0L;
    if (A12 == 0.0L) {
      A12 = 0.0L;
      if (copysignl(1.0L, A22) != 1.0L) {
        *u21 = -*u21;
        *u22 = -*u22;
        A22 = -A22;
      }
      e = 0;
    }
    else if (A22 == 0.0L) {
      if (copysignl(1.0L, A12) != 1.0L) {
        A12 = -A12;
        A22 = -A22;
        *v12 = -*v12;
        *v22 = -*v22;
      }
      if (copysignl(1.0L, A22) != 1.0L) {
        *u21 = -*u21;
        *u22 = -*u22;
        A22 = 0.0L;
      }
      *s1 = hypotl(*s1, *s2);
      *s2 = 0.0L;
      e = 5;
    }
    else
      e = 13;
    if (A12 < 0.0L) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -*v12;
      *v22 = -*v22;
    }
    if (A22 < 0.0L) {
      A22 = -A22;
      /* sin(ϑ) is always non-negative by construction */
      /* this is just an extra bit of info, used later */
      st = -st;
    }
    if ((e == 13) && (A11 < A22))
      e = -13;
#ifndef NDEBUG
    (void)printf("tan(ϑ)=%s, ", pvn_xtoa(s, tt));
    (void)printf("cos(ϑ)=%s, ", pvn_xtoa(s, ct));
    (void)printf("sin(ϑ)=%s\n", pvn_xtoa(s, st));
#endif /* !NDEBUG */
  }

  if (e == 3) {
    /* [ X 0 ] */
    /* [ x 0 ] */
    /* U^T(ϑ):
        cos(ϑ)  sin(ϑ)
       -sin(ϑ)  cos(ϑ)
    */
    tt = (A21 / A11);
    /* 1 / cos */
    ct = hypotl(tt, 1.0L);
    /* apply the left Givens rotation to U */
    st = -tt;
    A21 = *u11;
    if (ct == 1.0L) {
      *u11 = fmal(tt, *u21, *u11);
      *u21 = fmal(st,  A21, *u21);
      A21 = *u12;
      *u12 = fmal(tt, *u22, *u12);
      *u22 = fmal(st,  A21, *u22);
      st = tt;
    }
    else {
      *u11 = (fmal(tt, *u21, *u11) / ct);
      *u21 = (fmal(st,  A21, *u21) / ct);
      A21 = *u12;
      *u12 = (fmal(tt, *u22, *u12) / ct);
      *u22 = (fmal(st,  A21, *u22) / ct);
      st = (tt / ct);
      ct = (1.0L / ct);
    }
    A11 = *s1;
    A21 = 0.0L;
    e = 0;
#ifndef NDEBUG
    (void)printf("tan(ϑ)=%s, ", pvn_xtoa(s, tt));
    (void)printf("cos(ϑ)=%s, ", pvn_xtoa(s, ct));
    (void)printf("sin(ϑ)=%s\n", pvn_xtoa(s, st));
#endif /* !NDEBUG */
  }

  if (e == 5) {
    /* [ X x ] */
    /* [ 0 0 ] */
    /* V(θ):
       cos(θ) -sin(θ)
       sin(θ)  cos(θ)
    */
    tt = (A12 / A11);
    /* 1 / cos */
    ct = hypotl(tt, 1.0L);
    /* apply the right Givens rotation to V */
    st = -tt;
    A12 = *v11;
    if (ct == 1.0L) {
      *v11 = fmal(tt, *v12, *v11);
      *v12 = fmal(st,  A12, *v12);
      A12 = *v21;
      *v21 = fmal(tt, *v22, *v21);
      *v22 = fmal(st,  A12, *v22);
      st = tt;
    }
    else {
      *v11 = (fmal(tt, *v12, *v11) / ct);
      *v12 = (fmal(st,  A12, *v12) / ct);
      A12 = *v21;
      *v21 = (fmal(tt, *v22, *v21) / ct);
      *v22 = (fmal(st,  A12, *v22) / ct);
      st = (tt / ct);
      ct = (1.0L / ct);
    }
    A11 = *s1;
    A12 = 0.0L;
    e = 0;
#ifndef NDEBUG
    (void)printf("tan(θ)=%s, ", pvn_xtoa(s, tt));
    (void)printf("cos(θ)=%s, ", pvn_xtoa(s, ct));
    (void)printf("sin(θ)=%s\n", pvn_xtoa(s, st));
#endif /* !NDEBUG */
  }

#ifndef NDEBUG
  (void)printf("\tA =\n");
  (void)printf("%s ", pvn_xtoa(s, scalbnl(A11, -*es)));
  (void)printf("%s\n", pvn_xtoa(s, scalbnl(A12, -*es)));
  (void)printf("%s ", pvn_xtoa(s, scalbnl(A21, -*es)));
  (void)printf("%s\n", pvn_xtoa(s, scalbnl(A22, -*es)));
#endif /* !NDEBUG */

  if (abs(e) == 13) {
    /* [ x y ] */
    /* [ 0 z ] */

    long double tf = 0.0L, cf = 1.0L, sf = 0.0L, tp = 0.0L, cp = 1.0L, sp = 0.0L;
    if (e == -13) {
      long double tf_ = 0.0L, cf_ = 1.0L, sf_ = 0.0L, tp_ = 0.0L, cp_ = 1.0L, sp_ = 0.0L;
      xlpsv2(A22, A12, A11, &tf_, &cf_, &sf_, &tp_, &cp_, &sp_, s1, s2, es
#ifndef NDEBUG
             , s
#endif /* !NDEBUG */
             );
      tf = (1.0L / tp_);
      cf = sp_;
      sf = cp_;
      tp = (1.0L / tf_);
      cp = sf_;
      sp = cf_;
    }
    else
      xlpsv2(A11, A12, A22, &tf, &cf, &sf, &tp, &cp, &sp, s1, s2, es
#ifndef NDEBUG
             , s
#endif /* !NDEBUG */
             );

    /* update U */
    if (copysignl(1.0L, st) != 1.0L) {
      /* U^T(φ) * diag(1, -1) * U^T(ϑ):
          cos(φ - ϑ) -sin(φ - ϑ)
         -sin(φ - ϑ) -cos(φ - ϑ)
      */
      st = -st;
      long double tf_t = (tf - tt), cf_t = 1.0L, sf_t = 0.0L;
      if (tf_t != 0.0L) {
        tf_t /= fmal(tf, tt, 1.0L);
        if (isfinite(tf_t)) {
          /* 1 / cos */
          cf_t = hypotl(tf_t, 1.0L);
          sf_t = (tf_t / cf_t);
          cf_t = (1.0L / cf_t);
        }
        else {
          sf_t = copysignl(1.0L, tf_t);
          cf_t = 0.0L;
        }
      }
      else
        sf_t = tf_t;
      const long double _sf_t = -sf_t;
      A21 = *u11;
      *u11 = (_sf_t * *u21 + cf_t * *u11);
      *u21 = (_sf_t *  A21 - cf_t * *u21);
      A21 = *u12;
      *u12 = (_sf_t * *u22 + cf_t * *u12);
      *u22 = (_sf_t *  A21 - cf_t * *u22);
      A21 = -1.0L;
    }
    else if (tt != 0.0L) {
      /* U^T(φ) * U^T(ϑ) = U^T(φ + ϑ):
          cos(φ + ϑ)  sin(φ + ϑ)
         -sin(φ + ϑ)  cos(φ + ϑ)
      */
      long double tft = (tf + tt), cft = 1.0L, sft = 0.0L;
      if (tft != 0.0L) {
        tft /= fmal(-tf, tt, 1.0L);
        if (isfinite(tft)) {
          /* 1 / cos */
          cft = hypotl(tft, 1.0L);
          sft = (tft / cft);
          cft = (1.0L / cft);
        }
        else {
          sft = copysignl(1.0L, tft);
          cft = 0.0L;
        }
      }
      else
        sft = tft;
      A21 = *u11;
      *u11 = (cft * *u11 + sft * *u21);
      *u21 = (cft * *u21 - sft *  A21);
      A21 = *u12;
      *u12 = (cft * *u12 + sft * *u22);
      *u22 = (cft * *u22 - sft *  A21);
      A21 = 1.0L;
    }
    else if (tf != 0.0L) {
      /* U^T(φ):
          cos(φ)  sin(φ)
         -sin(φ)  cos(φ)
       */
      A21 = *u11;
      *u11 = (cf * *u11 + sf * *u21);
      *u21 = (cf * *u21 - sf *  A21);
      A21 = *u12;
      *u12 = (cf * *u12 + sf * *u22);
      *u22 = (cf * *u22 - sf *  A21);
      A21 = -0.0L;
    }
    else /* U^T(φ) = I */
      A21 = 0.0L;
#ifndef NDEBUG
    (void)printf("U operation=%s\n", pvn_xtoa(s, A21));
#endif /* !NDEBUG */

    /* update V */
    if (tp != 0.0L) {
      /* V(ψ):
         cos(ψ) -sin(ψ)
         sin(ψ)  cos(ψ)
      */
      A21 = *v11;
      *v11 = (*v11 * cp + *v12 * sp);
      *v12 = (*v12 * cp -  A21 * sp);
      A21 = *v21;
      *v21 = (*v21 * cp + *v22 * sp);
      *v22 = (*v22 * cp -  A21 * sp);
      A21 = -0.0L;
    }
    else /* V(ψ) = I */
      A21 = 0.0L;
#ifndef NDEBUG
    (void)printf("V operation=%s\n", pvn_xtoa(s, A21));
#endif /* !NDEBUG */

    if (e == -13) {
      *u21 = -*u21;
      *u22 = -*u22;
      *v12 = -*v12;
      *v22 = -*v22;
    }
  }

  if (ef_cmpl(es[1], *s1, es[2], *s2) < 0) {
    pvn_Lswp(u11, u21);
    pvn_Lswp(u12, u22);
    pvn_Lswp(v11, v12);
    pvn_Lswp(v21, v22);
    pvn_Lswp(s1, s2);
  }
  pvn_Lswp(u21, u12);
  return knd;
}

static long double cpolarl(const long double r, const long double i, long double *const c, long double *const s)
{
  assert(c);
  assert(s);
  const long double a = hypotl(r, i);
  *c = copysignl(fminl((fabsl(r) / a), 1.0L), r);
  *s = (i / fmaxl(a, LDBL_TRUE_MIN));
  return a;
}

int pvn_wljsv2_
(const long double *const a11r, const long double *const a11i, const long double *const a21r, const long double *const a21i, const long double *const a12r, const long double *const a12i, const long double *const a22r, const long double *const a22i,
 long double *const u11r, long double *const u11i, long double *const u21r, long double *const u21i, long double *const u12r, long double *const u12i, long double *const u22r, long double *const u22i,
 long double *const v11r, long double *const v11i, long double *const v21r, long double *const v21i, long double *const v12r, long double *const v12i, long double *const v22r, long double *const v22i,
 long double *const s1, long double *const s2, int *const es)
{
  assert(a11r);
  assert(a11i);
  assert(a21r);
  assert(a21i);
  assert(a12r);
  assert(a12i);
  assert(a22r);
  assert(a22i);
  assert(u11r);
  assert(u11i);
  assert(u21r);
  assert(u21i);
  assert(u12r);
  assert(u12i);
  assert(u22r);
  assert(u22i);
  assert(v11r);
  assert(v11i);
  assert(v21r);
  assert(v21i);
  assert(v12r);
  assert(v12i);
  assert(v22r);
  assert(v22i);
  assert(s1);
  assert(s2);
  assert(es);
  if (!isfinite(*a11r))
    return -1;
  if (!isfinite(*a11i))
    return -2;
  if (!isfinite(*a21r))
    return -3;
  if (!isfinite(*a21i))
    return -4;
  if (!isfinite(*a12r))
    return -5;
  if (!isfinite(*a12i))
    return -6;
  if (!isfinite(*a22r))
    return -7;
  if (!isfinite(*a22i))
    return -8;
  int kndi = 0, mxe = INT_MIN, e = 0;
  if (*a11i != 0.0L) {
    kndi |= 1;
    (void)frexpl(*a11i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21i != 0.0L) {
    kndi |= 2;
    (void)frexpl(*a21i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12i != 0.0L) {
    kndi |= 4;
    (void)frexpl(*a12i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22i != 0.0L) {
    kndi |= 8;
    (void)frexpl(*a22i, &e);
    if (mxe < e)
      mxe = e;
  }
  *u11i = 0.0L;
  *u21i = 0.0L;
  *u12i = 0.0L;
  *u22i = 0.0L;
  *v11i = 0.0L;
  *v21i = 0.0L;
  *v12i = 0.0L;
  *v22i = 0.0L;
  if (!kndi)
    return pvn_xljsv2_(a11r, a21r, a12r, a22r, u11r, u21r, u12r, u22r, v11r, v21r, v12r, v22r, s1, s2, es);
  int kndr = 0;
  if (*a11r != 0.0L) {
    kndr |= 1;
    (void)frexpl(*a11r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21r != 0.0L) {
    kndr |= 2;
    (void)frexpl(*a21r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12r != 0.0L) {
    kndr |= 4;
    (void)frexpl(*a12r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22r != 0.0L) {
    kndr |= 8;
    (void)frexpl(*a22r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (!kndr) {
    *u11r = 0.0L;
    *u21r = 0.0L;
    *u12r = 0.0L;
    *u22r = 0.0L;
    return pvn_xljsv2_(a11i, a21i, a12i, a22i, u11i, u21i, u12i, u22i, v11r, v21r, v12r, v22r, s1, s2, es);
  }
  int knd = (kndr | kndi);
  switch (knd) {
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  3:
  case  5:
  case  7:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    e = (LDBL_MAX_EXP - mxe - 2);
    break;
  default:
    return INT_MIN;
  }
  mxe = *es;
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;
  es[2] = es[1] = 0;

  /* scaling by 2^(*es), can only fail when mxe != 0 */
  long double A11r = *a11r, A11i = *a11i, A21r = *a21r, A21i = *a21i, A12r = *a12r, A12i = *a12i, A22r = *a22r, A22i = *a22i;
  if (*es) {
    A11r = scalbnl(A11r, *es);
    A11i = scalbnl(A11i, *es);
    A21r = scalbnl(A21r, *es);
    A21i = scalbnl(A21i, *es);
    A12r = scalbnl(A12r, *es);
    A12i = scalbnl(A12i, *es);
    A22r = scalbnl(A22r, *es);
    A22i = scalbnl(A22i, *es);
    if (mxe) {
      if (!isfinite(A11r))
        return -9;
      if (!isfinite(A11i))
        return -10;
      if (!isfinite(A21r))
        return -11;
      if (!isfinite(A21i))
        return -12;
      if (!isfinite(A12r))
        return -13;
      if (!isfinite(A12i))
        return -14;
      if (!isfinite(A22r))
        return -15;
      if (!isfinite(A22i))
        return -16;
    }
    if (*es < 0) {
      kndr = 0;
      if (A11r != 0.0L)
        kndr |= 1;
      if (A21r != 0.0L)
        kndr |= 2;
      if (A12r != 0.0L)
        kndr |= 4;
      if (A22r != 0.0L)
        kndr |= 8;
      kndi = 0;
      if (A11i != 0.0L)
        kndi |= 1;
      if (A21i != 0.0L)
        kndi |= 2;
      if (A12i != 0.0L)
        kndi |= 4;
      if (A22i != 0.0L)
        kndi |= 8;
      knd = (kndr | kndi);
    }
  }

  *u11r = 1.0L;
  *u21r = 0.0L;
  *u12r = 0.0L;
  *u22r = 1.0L;
  *v11r = 1.0L;
  *v21r = 0.0L;
  *v12r = 0.0L;
  *v22r = 1.0L;
  *s1 = 0.0L;
  *s2 = 0.0L;

  if ((e == 13) && (A11r < A22r))
    e = -13;

  /* TODO */

  if (ef_cmpl(es[1], *s1, es[2], *s2) < 0) {
    pvn_Lswp(u11r, u21r);
    pvn_Lswp(u11i, u21i);
    pvn_Lswp(u12r, u22r);
    pvn_Lswp(u12i, u22i);
    pvn_Lswp(v11r, v12r);
    pvn_Lswp(v11i, v12i);
    pvn_Lswp(v21r, v22r);
    pvn_Lswp(v21i, v22i);
    pvn_Lswp(s1, s2);
  }
  pvn_Lswp(u21r, u12r);
  pvn_Lswp(u21i, u12i);
  *u11i = -*u11i;
  *u21i = -*u21i;
  *u12i = -*u12i;
  *u22i = -*u22i;
  return knd;
}

#ifdef PVN_QUADMATH
static inline void ef_mulq(int *const e, __float128 *const f, const int e1, const __float128 f1, const int e2, const __float128 f2)
{
  assert(e);
  assert(f);
  *f = (f1 * f2);
  *f = frexpq(*f, e);
  *e += (e1 + e2);
}

static inline void ef_divq(int *const e, __float128 *const f, const int e1, const __float128 f1, const int e2, const __float128 f2)
{
  assert(e);
  assert(f);
  *f = (f1 / f2);
  *f = frexpq(*f, e);
  *e += (e1 - e2);
}

/* f1, f2 >= 0 */
static inline int ef_cmpq(const int e1, const __float128 f1, const int e2, const __float128 f2)
{
  if (f2 == 0.0q)
    return (f1 != 0.0q);
  if (f1 == 0.0q)
    return -1;
  if (e1 < e2)
    return -1;
  if (e2 < e1)
    return 1;
  if (f1 < f2)
    return -1;
  if (f2 < f1)
    return 1;
  return 0;
}

static void qlpsv2(const __float128 A11, const __float128 A12, const __float128 A22, __float128 *const tf, __float128 *const cf, __float128 *const sf, __float128 *const tp, __float128 *const cp, __float128 *const sp, __float128 *const s1, __float128 *const s2, int *const es
#ifndef NDEBUG
                   , char *const s
#endif /* !NDEBUG */
                   )
{
  assert(tf);
  assert(cf);
  assert(sf);
  assert(tp);
  assert(cp);
  assert(sp);
  assert(s1);
  assert(s2);
  assert(es);
  int ae = 0, be = 0, abe = 0, a_be = 0, de = 0, ne = 0, t2e = 0;
  __float128 af = 0.0q, bf = 0.0q, abf = 0.0q, a_bf = 0.0q, df = 0.0q, nf = 0.0q, t2f = 0.0q, t2 = 0.0q;

  /* should never overflow */
  const __float128 a = hypotq(A11, A12);
  __float128 b = A22;

  if ((A11 / A12) < (FLT128_EPSILON * 0.5q)) {
    t2 = ((2.0q * A22) / A12);
    b = -1.0q;
  }
  else if ((A11 < A12) && (a == A12)) {
    bf = frexpq(b, &be);
    t2 = ((2.0q * A22) / A12);
  }
  else {
    af = frexpq(a, &ae);
    bf = frexpq(b, &be);
    abf = (a + b);
    if (!isfiniteq(abf)) {
      abf = ((0.5q * a) + (0.5q * b));
      de = 1;
    }
    abf = frexpq(abf, &abe);
    abe += de;

    a_bf = (a - b);
    if (a == b)
      de = 0;
    else if (fabsq(a_bf) >= FLT128_MIN) {
      a_bf = frexpq(a_bf, &a_be);
      de = 1;
    }
    else {
      de = ((FLT128_MIN_EXP + FLT128_MANT_DIG) - pvn_imin(ae, be));
      a_bf = (scalbnq(af, (ae + de)) - scalbnq(bf, (be + de)));
      a_bf = frexpq(a_bf, &a_be);
      a_be -= de;
      de = -1;
    }

    if (de)
      ef_mulq(&de, &df, a_be, a_bf, abe, abf);
    af = frexpq(A12, &ae);
    ef_mulq(&ne, &nf, ae, af, be, bf);
    ++ne;
    ef_divq(&t2e, &t2f, ne, nf, de, df);
    if (isfiniteq(t2f))
      t2 = scalbnq(t2f, t2e);
    else
      t2 = t2f;
  }

  if (isfiniteq(t2))
    *tf = (t2 / (1.0q + hypotq(t2, 1.0q)));
  else
    *tf = copysignq(1.0q, t2);
#ifndef NDEBUG
  if (s) (void)printf("tan(φ)=%s, ", pvn_qtoa(s, *tf));
#endif /* !NDEBUG */
  *cf = hypotq(*tf, 1.0q);
  *sf = (*tf / *cf);

  *sp = fmaq(*tf, A22, A12);
  *tp = (*sp / A11);
#ifndef NDEBUG
  if (s) (void)printf("tan(ψ)=%s\n", pvn_qtoa(s, *tp));
#endif /* !NDEBUG */

  if (b < 0.0q) {
    af = frexpq(A11, &ae);
    bf = frexpq(A22, &be);
    df = frexpq(*sp, &de);
    /* expect to be (A12 + tan(φ) * A22) == A12, but the LHS has already been computed */
    /* s2 ≈ (A11 * A22) / (A12 + tan(φ) * A22) */
    ef_mulq(&ne, &nf, ae, af, be, bf);
    ef_divq(&abe, &abf, ne, nf, de, df);
    /* s1 ≈ (A12 + tan(φ) * A22) */
    *s1 = *sp;
    a_bf = frexpq(*s1, &a_be);
    if (isfinite(*tp)) {
      /* 1 / cos */
      *cp = hypotq(*tp, 1.0q);
      *sp = (*tp / *cp);
      *cp = (1.0q / *cp);
    }
    else {
      ef_divq(&t2e, &t2f, de, df, ae, af);
      /* tan(ψ) so large that sec(ψ) ≈ |tan(ψ)| */
      t2f = fabsq(t2f);
      *sp = copysignq(1.0q, *tp);
      ef_divq(&ne, &nf, 1, 0.5q, t2e, t2f);
      *cp = scalbnq(nf, ne);
    }
  }
  else {
    /* 1 / cos */
    *cp = hypotq(*tp, 1.0q);
    nf = frexpq(*cf, &ne);
    df = frexpq(*cp, &de);
    ef_divq(&ae, &af, ne, nf, de, df);
    /* s2 = A22 * (cf / cp) */
    ef_mulq(&abe, &abf, be, bf, ae, af);
    bf = frexpq(A11, &be);
    /* s1 = A11 * (cp / cf) */
    ef_divq(&a_be, &a_bf, be, bf, ae, af);
    *sp = (*tp / *cp);
    *cp = (1.0q / *cp);
  }

  *cf = (1.0q / *cf);
#ifndef NDEBUG
  if (s) {
    (void)printf("cos(φ)=%s, ", pvn_qtoa(s, *cf));
    (void)printf("sin(φ)=%s\n", pvn_qtoa(s, *sf));
    (void)printf("cos(ψ)=%s, ", pvn_qtoa(s, *cf));
    (void)printf("sin(ψ)=%s\n", pvn_qtoa(s, *sf));
  }
#endif /* !NDEBUG */

  *s1 = a_bf;
  es[1] = a_be;
  *s2 = abf;
  es[2] = abe;
}

int pvn_qljsv2_
(const __float128 *const a11, const __float128 *const a21, const __float128 *const a12, const __float128 *const a22,
 __float128 *const u11, __float128 *const u21, __float128 *const u12, __float128 *const u22,
 __float128 *const v11, __float128 *const v21, __float128 *const v12, __float128 *const v22,
 __float128 *const s1, __float128 *const s2, int *const es)
{
  assert(a11);
  assert(a21);
  assert(a12);
  assert(a22);
  assert(u11);
  assert(u21);
  assert(u12);
  assert(u22);
  assert(v11);
  assert(v21);
  assert(v12);
  assert(v22);
  assert(s1);
  assert(s2);
  assert(es);
  if (!isfiniteq(*a11))
    return -1;
  if (!isfiniteq(*a21))
    return -2;
  if (!isfiniteq(*a12))
    return -3;
  if (!isfiniteq(*a22))
    return -4;
  int knd = 0, mxe = INT_MIN, e = INT_MIN;
  if (*a11 != 0.0q) {
    knd |= 1;
    (void)frexpq(*a11, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21 != 0.0q) {
    knd |= 2;
    (void)frexpq(*a21, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12 != 0.0q) {
    knd |= 4;
    (void)frexpq(*a12, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22 != 0.0q) {
    knd |= 8;
    (void)frexpq(*a22, &e);
    if (mxe < e)
      mxe = e;
  }
  switch (knd) {
  case  0:
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  3:
  case  5:
  case  7:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
    e = (FLT128_MAX_EXP - mxe - 1);
    break;
  case 15:
    e = (FLT128_MAX_EXP - mxe - 2);
    break;
  default:
    return INT_MIN;
  }
  mxe = *es;
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;
  es[2] = es[1] = 0;

  /* scaling by 2^(*es), can only fail when mxe != 0 */
  __float128 A11 = *a11, A21 = *a21, A12 = *a12, A22 = *a22;
  if (*es) {
    A11 = scalbnq(A11, *es);
    A21 = scalbnq(A21, *es);
    A12 = scalbnq(A12, *es);
    A22 = scalbnq(A22, *es);
    if (mxe) {
      if (!isfiniteq(A11))
        return -5;
      if (!isfiniteq(A21))
        return -6;
      if (!isfiniteq(A12))
        return -7;
      if (!isfiniteq(A22))
        return -8;
    }
    if (*es < 0) {
      knd = 0;
      if (A11 != 0.0q)
        knd |= 1;
      if (A21 != 0.0q)
        knd |= 2;
      if (A12 != 0.0q)
        knd |= 4;
      if (A22 != 0.0q)
        knd |= 8;
    }
  }

  *u11 = 1.0q;
  *u21 = 0.0q;
  *u12 = 0.0q;
  *u22 = 1.0q;
  *v11 = 1.0q;
  *v21 = 0.0q;
  *v12 = 0.0q;
  *v22 = 1.0q;
  *s1 = 0.0q;
  *s2 = 0.0q;

  /* simplify the form of A */
  switch (knd) {
  case  0:
    /* [ 0 0 ] */
    /* [ 0 0 ] */
    *u11 = copysignq(1.0q, A11);
    *u22 = copysignq(1.0q, A22);
    A22 = A12 = A21 = A11 = 0.0q;
    break;
  case  1:
    /* [ * 0 ] */
    /* [ 0 0 ] */
    if (A11 < 0.0q) {
      *u11 = -1.0q;
      A11 = -A11;
    }
    *u22 = copysignq(1.0q, A22);
    A22 = A12 = A21 = 0.0q;
    *s1 = A11;
    break;
  case  2:
    /* [ 0 0 ] */
    /* [ * 0 ] */
    *u11 = 0.0q;
    *u22 = 0.0q;
    A11 = A21;
    A22 = A12;
    if (A11 < 0.0q) {
      *u12 = -1.0q;
      A11 = -A11;
    }
    else
      *u12 = 1.0q;
    *u21 = copysignq(1.0q, A12);
    A22 = A12 = A21 = 0.0q;
    *s1 = A11;
    break;
  case  3:
    /* [ * 0 ] */
    /* [ * 0 ] */
    A12 = fabsq(A11);
    A22 = fabsq(A21);
    if (A12 < A22) {
      *u11 = 0.0q;
      *u21 = copysignq(1.0q, A11);
      *u12 = copysignq(1.0q, A21);
      *u22 = 0.0q;
      A11 = A22;
      A21 = A12;
    }
    else {
      *u11 = copysignq(1.0q, A11);
      *u22 = copysignq(1.0q, A21);
      A11 = A12;
      A21 = A22;
    }
    A22 = A12 = 0.0q;
    *s1 = hypotq(A11, A21);
    e = 3;
    break;
  case  4:
    /* [ 0 * ] */
    /* [ 0 0 ] */
    *u11 = copysignq(1.0q, A12);
    *u22 = copysignq(1.0q, A21);
    A11 = fabsq(A12);
    A22 = A12 = A21 = 0.0q;
    *v11 = 0.0q;
    *v21 = 1.0q;
    *v12 = 1.0q;
    *v22 = 0.0q;
    *s1 = A11;
    break;
  case  5:
    /* [ * * ] */
    /* [ 0 0 ] */
    A21 = fabsq(A11);
    A22 = fabsq(A12);
    if (A21 < A22) {
      *v11 = 0.0q;
      *v21 = copysignq(1.0q, A12);
      *v12 = copysignq(1.0q, A11);
      *v22 = 0.0q;
      A11 = A22;
      A12 = A21;
    }
    else {
      *v11 = copysignq(1.0q, A11);
      *v22 = copysignq(1.0q, A12);
      A11 = A21;
      A12 = A22;
    }
    A22 = A21 = 0.0q;
    *s1 = hypotq(A11, A12);
    e = 5;
    break;
  case  6:
    /* [ 0 * ] */
    /* [ * 0 ] */
    *u11 = copysignq(1.0q, A12);
    *u22 = copysignq(1.0q, A21);
    A11 = fabsq(A12);
    A22 = fabsq(A21);
    A12 = A21 = 0.0q;
    *v11 = 0.0q;
    *v21 = 1.0q;
    *v12 = 1.0q;
    *v22 = 0.0q;
    *s1 = A11;
    *s2 = A22;
    break;
  case  7:
    /* [ * * ] */
    /* [ * 0 ] */
    A22 = A11;
    A11 = A12;
    A12 = A22;
    A22 = A21;
    A21 = 0.0f;
    *v11 = 0.0f;
    *v22 = 0.0f;
    if (A11 < 0.0f) {
      A11 = -A11;
      *v21 = -1.0f;
    }
    else
      *v21 = 1.0f;
    if (A12 < 0.0f) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -1.0f;
    }
    else
      *v12 = 1.0f;
    if (A22 < 0.0f) {
      *u22 = -1.0f;
      A22 = -A22;
    }
    e = 13;
    break;
  case  8:
    /* [ 0 0 ] */
    /* [ 0 * ] */
    *u11 = 0.0q;
    *u21 = copysignq(1.0q, A11);
    *u12 = copysignq(1.0q, A22);
    *u22 = 0.0q;
    A11 = fabsq(A22);
    A22 = A12 = A21 = 0.0q;
    *v11 = 0.0q;
    *v21 = 1.0q;
    *v12 = 1.0q;
    *v22 = 0.0q;
    *s1 = A11;
    break;
  case  9:
    /* [ * 0 ] */
    /* [ 0 * ] */
    if (A11 < 0.0q) {
      *u11 = -1.0q;
      A11 = -A11;
    }
    if (A22 < 0.0q) {
      *u22 = -1.0q;
      A22 = -A22;
    }
    A12 = A21 = 0.0q;
    *s1 = A11;
    *s2 = A22;
    break;
  case 10:
    /* [ 0 0 ] */
    /* [ * * ] */
    *u11 = 0.0q;
    *u21 = 1.0q;
    *u12 = 1.0q;
    *u22 = 0.0q;
    A11 = A21;
    A12 = A22;
    A21 = fabsq(A11);
    A22 = fabsq(A12);
    if (A21 < A22) {
      *v11 = 0.0q;
      *v21 = copysignq(1.0q, A12);
      *v12 = copysignq(1.0q, A11);
      *v22 = 0.0q;
      A11 = A22;
      A12 = A21;
    }
    else {
      *v11 = copysignq(1.0q, A11);
      *v22 = copysignq(1.0q, A12);
      A11 = A21;
      A12 = A22;
    }
    A22 = A21 = 0.0q;
    *s1 = hypotq(A11, A12);
    e = 5;
    break;
  case 11:
    /* [ * 0 ] */
    /* [ * * ] */
    *u11 = 0.0f;
    *u12 = 1.0f;
    *u22 = 0.0f;
    A12 = A11;
    A11 = A22;
    A22 = A12;
    A12 = A21;
    A21 = 0.0f;
    *v11 = 0.0f;
    *v22 = 0.0f;
    if (A11 < 0.0f) {
      A11 = -A11;
      *v21 = -1.0f;
    }
    else
      *v21 = 1.0f;
    if (A12 < 0.0f) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -1.0f;
    }
    else
      *v12 = 1.0f;
    if (A22 < 0.0f) {
      *u21 = -1.0f;
      A22 = -A22;
    }
    else
      *u21 = 1.0f;
    e = 13;
    break;
  case 12:
    /* [ 0 * ] */
    /* [ 0 * ] */
    A11 = A12;
    A21 = A22;
    *v11 = 0.0q;
    *v21 = 1.0q;
    *v12 = 1.0q;
    *v22 = 0.0q;
    A12 = fabsq(A11);
    A22 = fabsq(A21);
    if (A12 < A22) {
      *u11 = 0.0q;
      *u21 = copysignq(1.0q, A11);
      *u12 = copysignq(1.0q, A21);
      *u22 = 0.0q;
      A11 = A22;
      A21 = A12;
    }
    else {
      *u11 = copysignq(1.0q, A11);
      *u22 = copysignq(1.0q, A21);
      A11 = A12;
      A21 = A22;
    }
    A22 = A12 = 0.0q;
    *s1 = hypotq(A11, A21);
    e = 3;
    break;
  case 13:
    /* [ * * ] */
    /* [ 0 * ] */
    if (A11 < 0.0f) {
      A11 = -A11;
      *v11 = -1.0f;
    }
    if (A12 < 0.0f) {
      A12 = -A12;
      A22 = -A22;
      *v22 = -1.0f;
    }
    if (A22 < 0.0f) {
      *u22 = -1.0f;
      A22 = -A22;
    }
    A21 = 0.0f;
    e = 13;
    break;
  case 14:
    /* [ 0 * ] */
    /* [ * * ] */
    *u11 = 0.0f;
    *u12 = 1.0f;
    *u22 = 0.0f;
    A11 = A12;
    A12 = A22;
    A22 = A11;
    A11 = A21;
    A21 = 0.0f;
    if (A11 < 0.0f) {
      A11 = -A11;
      *v11 = -1.0f;
    }
    if (A12 < 0.0f) {
      A12 = -A12;
      A22 = -A22;
      *v22 = -1.0f;
    }
    if (A22 < 0.0f) {
      *u21 = -1.0f;
      A22 = -A22;
    }
    else
      *u21 = 1.0f;
    e = 13;
    break;
  case 15:
    /* [ * * ] */
    /* [ * * ] */
    e = 15;
    break;
  default:
    return INT_MIN;
  }

  if ((e == 13) && (A11 < A22))
    e = -13;

#ifndef NDEBUG
  char s[46] = { '\0' };
  (void)printf("\tA[knd=%d,e=%d] * 2^%d =\n", knd, e, *es);
  (void)printf("%s ", pvn_qtoa(s, A11));
  (void)printf("%s\n", pvn_qtoa(s, A12));
  (void)printf("%s ", pvn_qtoa(s, A21));
  (void)printf("%s\n", pvn_qtoa(s, A22));
#endif /* !NDEBUG */

  __float128 tt = 0.0q, ct = 1.0q, st = 0.0q;

  if (e == 15) {
    /* [ * * ] */
    /* [ * * ] */
    *s1 = hypotq(A11, A21);
    *s2 = hypotq(A12, A22);
    if (*s1 < *s2) {
      pvn_qswp(&A11, &A12);
      pvn_qswp(&A21, &A22);
      pvn_qswp(v11, v12);
      pvn_qswp(v21, v22);
      pvn_qswp(s1, s2);
    }
    if (copysignq(1.0q, A11) != 1.0q) {
      *u11 = -*u11;
      *u12 = -*u12;
      A11 = -A11;
      A12 = -A12;
    }
    if (copysignq(1.0q, A21) != 1.0q) {
      *u21 = -*u21;
      *u22 = -*u22;
      A21 = -A21;
      A22 = -A22;
    }
    if (A11 < A21) {
      pvn_qswp(u11, u21);
      pvn_qswp(u12, u22);
      pvn_qswp(&A11, &A21);
      pvn_qswp(&A12, &A22);
    }
    /* U^T(ϑ):
        cos(ϑ)  sin(ϑ)
       -sin(ϑ)  cos(ϑ)
    */
    tt = (A21 / A11);
    /* 1 / cos */
    ct = hypotq(tt, 1.0q);
    /* apply the left Givens rotation to A (and maybe to U) */
    st = -tt;
    A21 = A12;
    if (ct == 1.0q) {
      A12 = fmaq(tt, A22, A12);
      A22 = fmaq(st, A21, A22);
      if ((A12 == 0.0q) || (A22 == 0.0q)) {
        A21 = *u11;
        *u11 = fmaq(tt, *u21, *u11);
        *u21 = fmaq(st,  A21, *u21);
        A21 = *u12;
        *u12 = fmaq(tt, *u22, *u12);
        *u22 = fmaq(st,  A21, *u22);
      }
      st = tt;
    }
    else {
      A12 = (fmaq(tt, A22, A12) / ct);
      A22 = (fmaq(st, A21, A22) / ct);
      if ((A12 == 0.0q) || (A22 == 0.0q)) {
        A21 = *u11;
        *u11 = (fmaq(tt, *u21, *u11) / ct);
        *u21 = (fmaq(st,  A21, *u21) / ct);
        A21 = *u12;
        *u12 = (fmaq(tt, *u22, *u12) / ct);
        *u22 = (fmaq(st,  A21, *u22) / ct);
      }
      st = (tt / ct);
      ct = (1.0q / ct);
    }
    A11 = *s1;
    A21 = 0.0q;
    if (A12 == 0.0q) {
      A12 = 0.0q;
      if (copysignq(1.0q, A22) != 1.0q) {
        *u21 = -*u21;
        *u22 = -*u22;
        A22 = -A22;
      }
      e = 0;
    }
    else if (A22 == 0.0q) {
      if (copysignq(1.0q, A12) != 1.0q) {
        A12 = -A12;
        A22 = -A22;
        *v12 = -*v12;
        *v22 = -*v22;
      }
      if (copysignq(1.0q, A22) != 1.0q) {
        *u21 = -*u21;
        *u22 = -*u22;
        A22 = 0.0q;
      }
      *s1 = hypotq(*s1, *s2);
      *s2 = 0.0q;
      e = 5;
    }
    else
      e = 13;
    if (A12 < 0.0q) {
      A12 = -A12;
      A22 = -A22;
      *v12 = -*v12;
      *v22 = -*v22;
    }
    if (A22 < 0.0q) {
      A22 = -A22;
      /* sin(ϑ) is always non-negative by construction */
      /* this is just an extra bit of info, used later */
      st = -st;
    }
    if ((e == 13) && (A11 < A22))
      e = -13;
#ifndef NDEBUG
    (void)printf("tan(ϑ)=%s, ", pvn_qtoa(s, tt));
    (void)printf("cos(ϑ)=%s, ", pvn_qtoa(s, ct));
    (void)printf("sin(ϑ)=%s\n", pvn_qtoa(s, st));
#endif /* !NDEBUG */
  }

  if (e == 3) {
    /* [ X 0 ] */
    /* [ x 0 ] */
    /* U^T(ϑ):
        cos(ϑ)  sin(ϑ)
       -sin(ϑ)  cos(ϑ)
    */
    tt = (A21 / A11);
    /* 1 / cos */
    ct = hypotq(tt, 1.0q);
    /* apply the left Givens rotation to U */
    st = -tt;
    A21 = *u11;
    if (ct == 1.0q) {
      *u11 = fmaq(tt, *u21, *u11);
      *u21 = fmaq(st,  A21, *u21);
      A21 = *u12;
      *u12 = fmaq(tt, *u22, *u12);
      *u22 = fmaq(st,  A21, *u22);
      st = tt;
    }
    else {
      *u11 = (fmaq(tt, *u21, *u11) / ct);
      *u21 = (fmaq(st,  A21, *u21) / ct);
      A21 = *u12;
      *u12 = (fmaq(tt, *u22, *u12) / ct);
      *u22 = (fmaq(st,  A21, *u22) / ct);
      st = (tt / ct);
      ct = (1.0q / ct);
    }
    A11 = *s1;
    A21 = 0.0q;
    e = 0;
#ifndef NDEBUG
    (void)printf("tan(ϑ)=%s, ", pvn_qtoa(s, tt));
    (void)printf("cos(ϑ)=%s, ", pvn_qtoa(s, ct));
    (void)printf("sin(ϑ)=%s\n", pvn_qtoa(s, st));
#endif /* !NDEBUG */
  }

  if (e == 5) {
    /* [ X x ] */
    /* [ 0 0 ] */
    /* V(θ):
       cos(θ) -sin(θ)
       sin(θ)  cos(θ)
    */
    tt = (A12 / A11);
    /* 1 / cos */
    ct = hypotq(tt, 1.0q);
    /* apply the right Givens rotation to V */
    st = -tt;
    A12 = *v11;
    if (ct == 1.0q) {
      *v11 = fmaq(tt, *v12, *v11);
      *v12 = fmaq(st,  A12, *v12);
      A12 = *v21;
      *v21 = fmaq(tt, *v22, *v21);
      *v22 = fmaq(st,  A12, *v22);
      st = tt;
    }
    else {
      *v11 = (fmaq(tt, *v12, *v11) / ct);
      *v12 = (fmaq(st,  A12, *v12) / ct);
      A12 = *v21;
      *v21 = (fmaq(tt, *v22, *v21) / ct);
      *v22 = (fmaq(st,  A12, *v22) / ct);
      st = (tt / ct);
      ct = (1.0q / ct);
    }
    A11 = *s1;
    A12 = 0.0q;
    e = 0;
#ifndef NDEBUG
    (void)printf("tan(θ)=%s, ", pvn_qtoa(s, tt));
    (void)printf("cos(θ)=%s, ", pvn_qtoa(s, ct));
    (void)printf("sin(θ)=%s\n", pvn_qtoa(s, st));
#endif /* !NDEBUG */
  }

#ifndef NDEBUG
  (void)printf("\tA =\n");
  (void)printf("%s ", pvn_qtoa(s, scalbnq(A11, -*es)));
  (void)printf("%s\n", pvn_qtoa(s, scalbnq(A12, -*es)));
  (void)printf("%s ", pvn_qtoa(s, scalbnq(A21, -*es)));
  (void)printf("%s\n", pvn_qtoa(s, scalbnq(A22, -*es)));
#endif /* !NDEBUG */

  if (abs(e) == 13) {
    /* [ x y ] */
    /* [ 0 z ] */

    __float128 tf = 0.0q, cf = 1.0q, sf = 0.0q, tp = 0.0q, cp = 1.0q, sp = 0.0q;
    if (e == -13) {
      __float128 tf_ = 0.0q, cf_ = 1.0q, sf_ = 0.0q, tp_ = 0.0q, cp_ = 1.0q, sp_ = 0.0q;
      qlpsv2(A22, A12, A11, &tf_, &cf_, &sf_, &tp_, &cp_, &sp_, s1, s2, es
#ifndef NDEBUG
             , s
#endif /* !NDEBUG */
             );
      tf = (1.0q / tp_);
      cf = sp_;
      sf = cp_;
      tp = (1.0q / tf_);
      cp = sf_;
      sp = cf_;
    }
    else
      qlpsv2(A11, A12, A22, &tf, &cf, &sf, &tp, &cp, &sp, s1, s2, es
#ifndef NDEBUG
             , s
#endif /* !NDEBUG */
             );

    /* update U */
    if (copysignq(1.0q, st) != 1.0q) {
      /* U^T(φ) * diag(1, -1) * U^T(ϑ):
          cos(φ - ϑ) -sin(φ - ϑ)
         -sin(φ - ϑ) -cos(φ - ϑ)
      */
      st = -st;
      __float128 tf_t = (tf - tt), cf_t = 1.0q, sf_t = 0.0q;
      if (tf_t != 0.0q) {
        tf_t /= fmaq(tf, tt, 1.0q);
        if (isfiniteq(tf_t)) {
          /* 1 / cos */
          cf_t = hypotq(tf_t, 1.0q);
          sf_t = (tf_t / cf_t);
          cf_t = (1.0q / cf_t);
        }
        else {
          sf_t = copysignq(1.0q, tf_t);
          cf_t = 0.0q;
        }
      }
      else
        sf_t = tf_t;
      const __float128 _sf_t = -sf_t;
      A21 = *u11;
      *u11 = (_sf_t * *u21 + cf_t * *u11);
      *u21 = (_sf_t *  A21 - cf_t * *u21);
      A21 = *u12;
      *u12 = (_sf_t * *u22 + cf_t * *u12);
      *u22 = (_sf_t *  A21 - cf_t * *u22);
      A21 = -1.0q;
    }
    else if (tt != 0.0q) {
      /* U^T(φ) * U^T(ϑ) = U^T(φ + ϑ):
          cos(φ + ϑ)  sin(φ + ϑ)
         -sin(φ + ϑ)  cos(φ + ϑ)
      */
      __float128 tft = (tf + tt), cft = 1.0q, sft = 0.0q;
      if (tft != 0.0q) {
        tft /= fmaq(-tf, tt, 1.0q);
        if (isfiniteq(tft)) {
          /* 1 / cos */
          cft = hypotq(tft, 1.0q);
          sft = (tft / cft);
          cft = (1.0q / cft);
        }
        else {
          sft = copysignq(1.0q, tft);
          cft = 0.0q;
        }
      }
      else
        sft = tft;
      A21 = *u11;
      *u11 = (cft * *u11 + sft * *u21);
      *u21 = (cft * *u21 - sft *  A21);
      A21 = *u12;
      *u12 = (cft * *u12 + sft * *u22);
      *u22 = (cft * *u22 - sft *  A21);
      A21 = 1.0q;
    }
    else if (tf != 0.0q) {
      /* U^T(φ):
          cos(φ)  sin(φ)
         -sin(φ)  cos(φ)
       */
      A21 = *u11;
      *u11 = (cf * *u11 + sf * *u21);
      *u21 = (cf * *u21 - sf *  A21);
      A21 = *u12;
      *u12 = (cf * *u12 + sf * *u22);
      *u22 = (cf * *u22 - sf *  A21);
      A21 = -0.0q;
    }
    else /* U^T(φ) = I */
      A21 = 0.0q;
#ifndef NDEBUG
    (void)printf("U operation=%s\n", pvn_qtoa(s, A21));
#endif /* !NDEBUG */

    /* update V */
    if (tp != 0.0q) {
      /* V(ψ):
         cos(ψ) -sin(ψ)
         sin(ψ)  cos(ψ)
      */
      A21 = *v11;
      *v11 = (*v11 * cp + *v12 * sp);
      *v12 = (*v12 * cp -  A21 * sp);
      A21 = *v21;
      *v21 = (*v21 * cp + *v22 * sp);
      *v22 = (*v22 * cp -  A21 * sp);
      A21 = -0.0q;
    }
    else /* V(ψ) = I */
      A21 = 0.0q;
#ifndef NDEBUG
    (void)printf("V operation=%s\n", pvn_qtoa(s, A21));
#endif /* !NDEBUG */

    if (e == -13) {
      *u21 = -*u21;
      *u22 = -*u22;
      *v12 = -*v12;
      *v22 = -*v22;
    }
  }

  if (ef_cmpq(es[1], *s1, es[2], *s2) < 0) {
    pvn_qswp(u11, u21);
    pvn_qswp(u12, u22);
    pvn_qswp(v11, v12);
    pvn_qswp(v21, v22);
    pvn_qswp(s1, s2);
  }
  pvn_qswp(u21, u12);
  return knd;
}

static __float128 cpolarq(const __float128 r, const __float128 i, __float128 *const c, __float128 *const s)
{
  assert(c);
  assert(s);
  const __float128 a = hypotq(r, i);
  *c = copysignq(fminq((fabsq(r) / a), 1.0q), r);
  *s = (i / fmaxq(a, FLT128_TRUE_MIN));
  return a;
}

int pvn_yljsv2_
(const __float128 *const a11r, const __float128 *const a11i, const __float128 *const a21r, const __float128 *const a21i, const __float128 *const a12r, const __float128 *const a12i, const __float128 *const a22r, const __float128 *const a22i,
 __float128 *const u11r, __float128 *const u11i, __float128 *const u21r, __float128 *const u21i, __float128 *const u12r, __float128 *const u12i, __float128 *const u22r, __float128 *const u22i,
 __float128 *const v11r, __float128 *const v11i, __float128 *const v21r, __float128 *const v21i, __float128 *const v12r, __float128 *const v12i, __float128 *const v22r, __float128 *const v22i,
 __float128 *const s1, __float128 *const s2, int *const es)
{
  assert(a11r);
  assert(a11i);
  assert(a21r);
  assert(a21i);
  assert(a12r);
  assert(a12i);
  assert(a22r);
  assert(a22i);
  assert(u11r);
  assert(u11i);
  assert(u21r);
  assert(u21i);
  assert(u12r);
  assert(u12i);
  assert(u22r);
  assert(u22i);
  assert(v11r);
  assert(v11i);
  assert(v21r);
  assert(v21i);
  assert(v12r);
  assert(v12i);
  assert(v22r);
  assert(v22i);
  assert(s1);
  assert(s2);
  assert(es);
  if (!isfiniteq(*a11r))
    return -1;
  if (!isfiniteq(*a11i))
    return -2;
  if (!isfiniteq(*a21r))
    return -3;
  if (!isfiniteq(*a21i))
    return -4;
  if (!isfiniteq(*a12r))
    return -5;
  if (!isfiniteq(*a12i))
    return -6;
  if (!isfiniteq(*a22r))
    return -7;
  if (!isfiniteq(*a22i))
    return -8;
  int kndi = 0, mxe = INT_MIN, e = 0;
  if (*a11i != 0.0q) {
    kndi |= 1;
    (void)frexpq(*a11i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21i != 0.0q) {
    kndi |= 2;
    (void)frexpq(*a21i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12i != 0.0q) {
    kndi |= 4;
    (void)frexpq(*a12i, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22i != 0.0q) {
    kndi |= 8;
    (void)frexpq(*a22i, &e);
    if (mxe < e)
      mxe = e;
  }
  *u11i = 0.0q;
  *u21i = 0.0q;
  *u12i = 0.0q;
  *u22i = 0.0q;
  *v11i = 0.0q;
  *v21i = 0.0q;
  *v12i = 0.0q;
  *v22i = 0.0q;
  if (!kndi)
    return pvn_qljsv2_(a11r, a21r, a12r, a22r, u11r, u21r, u12r, u22r, v11r, v21r, v12r, v22r, s1, s2, es);
  int kndr = 0;
  if (*a11r != 0.0q) {
    kndr |= 1;
    (void)frexpq(*a11r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a21r != 0.0q) {
    kndr |= 2;
    (void)frexpq(*a21r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12r != 0.0q) {
    kndr |= 4;
    (void)frexpq(*a12r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22r != 0.0q) {
    kndr |= 8;
    (void)frexpq(*a22r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (!kndr) {
    *u11r = 0.0q;
    *u21r = 0.0q;
    *u12r = 0.0q;
    *u22r = 0.0q;
    return pvn_qljsv2_(a11i, a21i, a12i, a22i, u11i, u21i, u12i, u22i, v11r, v21r, v12r, v22r, s1, s2, es);
  }
  int knd = (kndr | kndi);
  switch (knd) {
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  3:
  case  5:
  case  7:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    e = (FLT128_MAX_EXP - mxe - 2);
    break;
  default:
    return INT_MIN;
  }
  mxe = *es;
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;
  es[2] = es[1] = 0;

  /* scaling by 2^(*es), can only fail when mxe != 0 */
  __float128 A11r = *a11r, A11i = *a11i, A21r = *a21r, A21i = *a21i, A12r = *a12r, A12i = *a12i, A22r = *a22r, A22i = *a22i;
  if (*es) {
    A11r = scalbnq(A11r, *es);
    A11i = scalbnq(A11i, *es);
    A21r = scalbnq(A21r, *es);
    A21i = scalbnq(A21i, *es);
    A12r = scalbnq(A12r, *es);
    A12i = scalbnq(A12i, *es);
    A22r = scalbnq(A22r, *es);
    A22i = scalbnq(A22i, *es);
    if (mxe) {
      if (!isfiniteq(A11r))
        return -9;
      if (!isfiniteq(A11i))
        return -10;
      if (!isfiniteq(A21r))
        return -11;
      if (!isfiniteq(A21i))
        return -12;
      if (!isfiniteq(A12r))
        return -13;
      if (!isfiniteq(A12i))
        return -14;
      if (!isfiniteq(A22r))
        return -15;
      if (!isfiniteq(A22i))
        return -16;
    }
    if (*es < 0) {
      kndr = 0;
      if (A11r != 0.0q)
        kndr |= 1;
      if (A21r != 0.0q)
        kndr |= 2;
      if (A12r != 0.0q)
        kndr |= 4;
      if (A22r != 0.0q)
        kndr |= 8;
      kndi = 0;
      if (A11i != 0.0q)
        kndi |= 1;
      if (A21i != 0.0q)
        kndi |= 2;
      if (A12i != 0.0q)
        kndi |= 4;
      if (A22i != 0.0q)
        kndi |= 8;
      knd = (kndr | kndi);
    }
  }

  *u11r = 1.0q;
  *u21r = 0.0q;
  *u12r = 0.0q;
  *u22r = 1.0q;
  *v11r = 1.0q;
  *v21r = 0.0q;
  *v12r = 0.0q;
  *v22r = 1.0q;
  *s1 = 0.0q;
  *s2 = 0.0q;

  if ((e == 13) && (A11r < A22r))
    e = -13;

  /* TODO */

  if (ef_cmpq(es[1], *s1, es[2], *s2) < 0) {
    pvn_qswp(u11r, u21r);
    pvn_qswp(u11i, u21i);
    pvn_qswp(u12r, u22r);
    pvn_qswp(u12i, u22i);
    pvn_qswp(v11r, v12r);
    pvn_qswp(v11i, v12i);
    pvn_qswp(v21r, v22r);
    pvn_qswp(v21i, v22i);
    pvn_qswp(s1, s2);
  }
  pvn_qswp(u21r, u12r);
  pvn_qswp(u21i, u12i);
  *u11i = -*u11i;
  *u21i = -*u21i;
  *u12i = -*u12i;
  *u22i = -*u22i;
  return knd;
}
#else /* !PVN_QUADMATH */
int pvn_qljsv2_
(const long double *const a11, const long double *const a21, const long double *const a12, const long double *const a22,
 long double *const u11, long double *const u21, long double *const u12, long double *const u22,
 long double *const v11, long double *const v21, long double *const v12, long double *const v22,
 long double *const s1, long double *const s2, int *const es)
{
  return pvn_xljsv2_(a11, a21, a12, a22, u11, u21, u12, u22, v11, v21, v12, v22, s1, s2, es);
}

int pvn_yljsv2_
(const long double *const a11r, const long double *const a11i, const long double *const a21r, const long double *const a21i, const long double *const a12r, const long double *const a12i, const long double *const a22r, const long double *const a22i,
 long double *const u11r, long double *const u11i, long double *const u21r, long double *const u21i, long double *const u12r, long double *const u12i, long double *const u22r, long double *const u22i,
 long double *const v11r, long double *const v11i, long double *const v21r, long double *const v21i, long double *const v12r, long double *const v12i, long double *const v22r, long double *const v22i,
 long double *const s1, long double *const s2, int *const es)
{
  return pvn_wljsv2_(a11r, a11i, a21r, a21i, a12r, a12i, a22r, a22i, u11r, u11i, u21r, u21i, u12r, u12i, u22r, u22i, v11r, v11i, v21r, v21i, v12r, v12i, v22r, v22i, s1, s2, es);
}
#endif /* ?PVN_QUADMATH */
#endif /* !_WIN32 */
#endif /* ?PVN_TEST */

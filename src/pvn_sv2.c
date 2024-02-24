#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc != 5) {
    (void)fprintf(stderr, "%s a11 a21 a12 a22\n", *argv);
    return EXIT_FAILURE;
  }
  const float a11 = (float)atof(argv[1]);
  const float a21 = (float)atof(argv[2]);
  const float a12 = (float)atof(argv[3]);
  const float a22 = (float)atof(argv[4]);
  float u11 = -0.0f, u21 = -0.0f, u12 = -0.0f, u22 = -0.0f, v11 = -0.0f, v21 = -0.0f, v12 = -0.0f, v22 = -0.0f, s1 = -0.0f, s2 = -0.0f;
  int es = 0;
  const int knd =
#ifdef _WIN32
PVN_SLJSV2
#else /* !_WIN32 */
pvn_sljsv2_
#endif /* ?_WIN32 */
    (&a11, &a21, &a12, &a22, &u11, &u21, &u12, &u22, &v11, &v21, &v12, &v22, &s1, &s2, &es);
  (void)printf("knd=%d, es=%d\n\ta =\n", knd, es);
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
  if (es) {
    es = -es;
    s1 = scalbnf(s1, es);
    s2 = scalbnf(s2, es);
  }
  (void)printf("%s ", pvn_stoa(s, s1));
  (void)printf("%s\n", pvn_stoa(s, s2));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
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
  /* TODO: TEST THE ROUTINE EXHAUSTIVELY */
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
    *s1 = hypotf(A11, A21);
    *s2 = hypotf(A12, A22);
    if (*s1 < *s2) {
      pvn_fswp(&A11, &A12);
      pvn_fswp(&A21, &A22);
      *v11 = 0.0f;
      *v21 = 1.0f;
      *v12 = 1.0f;
      *v22 = 0.0f;
      pvn_fswp(s1, s2);
    }
    if (A11 < 0.0f) {
      *u11 = -1.0f;
      A11 = -A11;
      A12 = -A12;
    }
    if (A21 < 0.0f) {
      *u22 = -1.0f;
      A21 = -A21;
      A22 = -A22;
    }
    if (A11 < A21) {
      pvn_fswp(u11, u21);
      pvn_fswp(u12, u22);
      pvn_fswp(&A11, &A21);
      pvn_fswp(&A12, &A22);
    }
    e = 15;
    break;
  default:
    return INT_MIN;
  }

#ifndef NDEBUG
  (void)printf("knd=%d, e=%d\n\tA * 2^(*es) =\n", knd, e);
  char s[17] = { '\0' };
  (void)printf("%s ", pvn_stoa(s, A11));
  (void)printf("%s\n", pvn_stoa(s, A12));
  (void)printf("%s ", pvn_stoa(s, A21));
  (void)printf("%s\n", pvn_stoa(s, A22));
#endif /* !NDEBUG */

  float tt = 0.0f, ct = 1.0f, st = 0.0f;

  if (e == 15) {
    /* [ X y ] */
    /* [ x z ] */
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
        if (*u21 != 0.0f)
          *u21 = -*u21;
        if (*u22 != 0.0f)
          *u22 = -*u22;
        A22 = -A22;
      }
      *s2 = A22;
      e = 0;
    }
    else if (A22 == 0.0f) {
      if (A12 < 0.0f) {
        A12 = -A12;
        A22 = -A22;
        if (*v12 != 0.0f)
          *v12 = -*v12;
        else
          *v22 = -*v22;
      }
      if (copysignf(1.0f, A22) != 1.0f) {
        if (*u21 != 0.0f)
          *u21 = -*u21;
        if (*u22 != 0.0f)
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
      if (*v12 != 0.0f)
        *v12 = -*v12;
      else
        *v22 = -*v22;
    }
    if (A22 < 0.0f) {
      A22 = -A22;
      /* sin(ϑ) is always non-negative by construction */
      /* this is just an extra bit of info, used later */
      st = -st;
    }
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

  if (e == 13) {
    /* [ x y ] */
    /* [ 0 z ] */

    /* should never overflow */
    const float a = hypotf(A11, A12);
    const float b = A22;

    int ae = 0, be = 0;
    float af = frexpf(a, &ae);
    float bf = frexpf(b, &be);

    float abf = (a + b);
    int abe = 0, de = 0;
    if (!isfinite(abf)) {
      abf = ((0.5f * a) + (0.5f * b));
      de = 1;
    }
    abf = frexpf(abf, &abe);
    abe += de;

    int a_be = 0;
    float a_bf = (a - b), df = 0.0f;
    if (a == b)
      de = 0;
    else if (isnormal(a_bf)) {
      de = -1;
      a_bf = frexpf(a_bf, &a_be);
    }
    else {
      de = ((FLT_MIN_EXP + FLT_MANT_DIG) - pvn_imin(ae, be));
      a_bf = (scalbnf(af, (ae + de)) - scalbnf(bf, (be + de)));
      a_bf = frexpf(a_bf, &a_be);
      a_be -= de;
    }

    if (de)
      ef_mulf(&de, &df, a_be, a_bf, abe, abf);

    af = frexpf(A12, &ae);
    int ne = 0;
    float nf = 0.0f;
    ef_mulf(&ne, &nf, ae, af, be, bf);
    ++ne;

    int t2e = 0;
    float t2f = 0.0f;
    ef_divf(&t2e, &t2f, ne, nf, de, df);
    const float t2 = (isfinite(t2f) ? scalbnf(t2f, t2e) : t2f);

    float tf = 0.0f, cf = 1.0f, sf = 0.0f;    
    if (isfinite(t2))
      tf = (t2 / (1.0f + hypotf(t2, 1.0f)));
    else
      tf = copysignf(1.0f, t2);
#ifndef NDEBUG
    (void)printf("tan(φ)=%s, ", pvn_stoa(s, tf));
#endif /* !NDEBUG */
    cf = hypotf(tf, 1.0f);
    sf = (tf / cf);

    float tp = 0.0f, cp = 1.0f, sp = 0.0f;
    /* this should never overflow... */
    tp = fmaf(tf, A22, A12);
    /* ...but this might */
    tp /= A11;
#ifndef NDEBUG
    (void)printf("tan(ψ)=%s\n", pvn_stoa(s, tp));
#endif /* !NDEBUG */

    if (isfinite(tp)) {
      /* 1 / cos */
      cp = hypotf(tp, 1.0f);
      sp = (tp / cp);
      nf = frexpf(cf, &ne);
      df = frexpf(cp, &de);
      ef_divf(&ae, &af, ne, nf, de, df);
      /* s2 = z * (cf / cp) */
      ef_mulf(&abe, &abf, be, bf, ae, af);
      bf = frexpf(A11, &be);
      /* s1 = x * (cp / cf) */
      ef_divf(&a_be, &a_bf, be, bf, ae, af);
      if (!mxe) {
        if (abe > FLT_MAX_EXP) {
          ne = (FLT_MAX_EXP - abe);
          abe += ne;
          a_be += ne;
          *es += ne;
        }
        if (a_be > FLT_MAX_EXP) {
          de = (FLT_MAX_EXP - a_be);
          abe += de;
          a_be += de;
          *es += de;
        }
      }
      *s1 = scalbnf(a_bf, a_be);
      *s2 = scalbnf(abf, abe);
      cp = (1.0f / cp);
    }
    else {
      *s1 = (fmaf(tf, A22, A12) / cf);
      *s2 = (A11 * sf);
      cp = 0.0f;
      sp = 1.0f;
    }
    cf = (1.0f / cf);

    /* update U */
#ifndef NDEBUG
    (void)printf("cos(φ)=%s, ", pvn_stoa(s, cf));
    (void)printf("sin(φ)=%s\n", pvn_stoa(s, sf));
#endif /* !NDEBUG */
    if (copysignf(1.0f, st) != 1.0f) {
      /* U^T(φ) * diag(1, -1) * U^T(ϑ):
          cos(φ - ϑ) -sin(φ - ϑ)
         -sin(φ - ϑ) -cos(φ - ϑ)
      */
      st = -st;
      float tf_t = (tf - tt), cf_t = 1.0f, sf_t = 0.0f;
      if (tf_t != 0.0f) {
        tf_t /= fmaf(tf, tt, 1.0f);
        /* 1 / cos */
        cf_t = hypotf(tf_t, 1.0f);
        sf_t = (tf_t / cf_t);
        cf_t = (1.0f / cf_t);
      }
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
          sft = 1.0f;
          cft = 0.0f;
        }
      }
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
#ifndef NDEBUG
    (void)printf("cos(ψ)=%s, ", pvn_stoa(s, cf));
    (void)printf("sin(ψ)=%s\n", pvn_stoa(s, sf));
#endif /* !NDEBUG */
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

    A21 = 0.0f;
    e = 0;
  }

  if (*s1 < *s2) {
    pvn_fswp(u11, u21);
    pvn_fswp(u12, u22);
    pvn_fswp(v11, v12);
    pvn_fswp(v21, v22);
    pvn_fswp(s1, s2);
  }
  pvn_fswp(u21, u12);
  return knd;
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

  /* TODO */
  switch (knd) {
  case  0:
    break;
  case  1:
    break;
  case  2:
    break;
  case  3:
    break;
  case  4:
    break;
  case  5:
    break;
  case  6:
    break;
  case  7:
    break;
  case  8:
    break;
  case  9:
    break;
  case 10:
    break;
  case 11:
    break;
  case 12:
    break;
  case 13:
    break;
  case 14:
    break;
  case 15:
    break;
  default:
    return INT_MIN;
  }

  if (*s1 < *s2) {
    pvn_dswp(u11, u21);
    pvn_dswp(u12, u22);
    pvn_dswp(v11, v12);
    pvn_dswp(v21, v22);
    pvn_dswp(s1, s2);
  }
  pvn_dswp(u21, u12);
  return knd;
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
    kndr |= 1;
    (void)frexpf(*a21r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12r != 0.0f) {
    kndr |= 1;
    (void)frexpf(*a12r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22r != 0.0f) {
    kndr |= 1;
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

  const int knd = (kndr | kndi);
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

  /* TODO */
  switch (knd) {
  case  1:
    break;
  case  2:
    break;
  case  3:
    break;
  case  4:
    break;
  case  5:
    break;
  case  6:
    break;
  case  7:
    break;
  case  8:
    break;
  case  9:
    break;
  case 10:
    break;
  case 11:
    break;
  case 12:
    break;
  case 13:
    break;
  case 14:
    break;
  case 15:
    break;
  default:
    return INT_MIN;
  }

  if (*s1 < *s2) {
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
    kndr |= 1;
    (void)frexp(*a21r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12r != 0.0) {
    kndr |= 1;
    (void)frexp(*a12r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22r != 0.0) {
    kndr |= 1;
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
  const int knd = (kndr | kndi);
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

  /* TODO */
  switch (knd) {
  case  1:
    break;
  case  2:
    break;
  case  3:
    break;
  case  4:
    break;
  case  5:
    break;
  case  6:
    break;
  case  7:
    break;
  case  8:
    break;
  case  9:
    break;
  case 10:
    break;
  case 11:
    break;
  case 12:
    break;
  case 13:
    break;
  case 14:
    break;
  case 15:
    break;
  default:
    return INT_MIN;
  }

  if (*s1 < *s2) {
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

  /* TODO */
  switch (knd) {
  case  0:
    break;
  case  1:
    break;
  case  2:
    break;
  case  3:
    break;
  case  4:
    break;
  case  5:
    break;
  case  6:
    break;
  case  7:
    break;
  case  8:
    break;
  case  9:
    break;
  case 10:
    break;
  case 11:
    break;
  case 12:
    break;
  case 13:
    break;
  case 14:
    break;
  case 15:
    break;
  default:
    return INT_MIN;
  }

  if (*s1 < *s2) {
    pvn_Lswp(u11, u21);
    pvn_Lswp(u12, u22);
    pvn_Lswp(v11, v12);
    pvn_Lswp(v21, v22);
    pvn_Lswp(s1, s2);
  }
  pvn_Lswp(u21, u12);
  return knd;
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
    kndr |= 1;
    (void)frexpl(*a21r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12r != 0.0L) {
    kndr |= 1;
    (void)frexpl(*a12r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22r != 0.0L) {
    kndr |= 1;
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
  const int knd = (kndr | kndi);
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

  /* TODO */
  switch (knd) {
  case  1:
    break;
  case  2:
    break;
  case  3:
    break;
  case  4:
    break;
  case  5:
    break;
  case  6:
    break;
  case  7:
    break;
  case  8:
    break;
  case  9:
    break;
  case 10:
    break;
  case 11:
    break;
  case 12:
    break;
  case 13:
    break;
  case 14:
    break;
  case 15:
    break;
  default:
    return INT_MIN;
  }

  if (*s1 < *s2) {
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

  /* TODO */
  switch (knd) {
  case  0:
    break;
  case  1:
    break;
  case  2:
    break;
  case  3:
    break;
  case  4:
    break;
  case  5:
    break;
  case  6:
    break;
  case  7:
    break;
  case  8:
    break;
  case  9:
    break;
  case 10:
    break;
  case 11:
    break;
  case 12:
    break;
  case 13:
    break;
  case 14:
    break;
  case 15:
    break;
  default:
    return INT_MIN;
  }

  if (*s1 < *s2) {
    pvn_qswp(u11, u21);
    pvn_qswp(u12, u22);
    pvn_qswp(v11, v12);
    pvn_qswp(v21, v22);
    pvn_qswp(s1, s2);
  }
  pvn_qswp(u21, u12);
  return knd;
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
    kndr |= 1;
    (void)frexpq(*a21r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a12r != 0.0q) {
    kndr |= 1;
    (void)frexpq(*a12r, &e);
    if (mxe < e)
      mxe = e;
  }
  if (*a22r != 0.0q) {
    kndr |= 1;
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
  const int knd = (kndr | kndi);
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

  /* TODO */
  switch (knd) {
  case  1:
    break;
  case  2:
    break;
  case  3:
    break;
  case  4:
    break;
  case  5:
    break;
  case  6:
    break;
  case  7:
    break;
  case  8:
    break;
  case  9:
    break;
  case 10:
    break;
  case 11:
    break;
  case 12:
    break;
  case 13:
    break;
  case 14:
    break;
  case 15:
    break;
  default:
    return INT_MIN;
  }

  if (*s1 < *s2) {
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

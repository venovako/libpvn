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
  /* should not fail beyond this point */
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
  case  7:
  case 11:
  case 13:
  case 14:
    /* QR not needed, extra scaling later */
    e = FLT_MAX_EXP - mxe;
    break;
  case  3:
  case  5:
  case 10:
  case 12:
    /* QR needed, extra scaling later */
    e = FLT_MAX_EXP - mxe - 1;
    break;
  case 15:
    /* at least one QR needed, extra scaling later */
    e = FLT_MAX_EXP - mxe - 2;
    break;
  default:
    return INT_MIN;
  }
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;

  float A11 = *a11, A21 = *a21, A12 = *a12, A22 = *a22;
  if (*es) {
    A11 = scalbnf(A11, *es);
    A21 = scalbnf(A21, *es);
    A12 = scalbnf(A12, *es);
    A22 = scalbnf(A22, *es);
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

  (void)printf("knd=%d, e=%d\n\tA =\n", knd, e);
  
  char s[17] = { '\0' };
  (void)printf("%s ", pvn_stoa(s, A11));
  (void)printf("%s\n", pvn_stoa(s, A12));
  (void)printf("%s ", pvn_stoa(s, A21));
  (void)printf("%s\n", pvn_stoa(s, A22));

  float tt = 0.0f, ct = 1.0f, st = 0.0f;
  if (e == 15) {
    /* [ X y ] */
    /* [ x z ] */
    tt = A21 / A11;
    /* 1 / cos */
    ct = hypotf(tt, 1.0f);
    /* apply the left Givens rotation to A (and maybe to u) */
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
      A12 = fmaf(tt, A22, A12) / ct;
      A22 = fmaf(st, A21, A22) / ct;
      if ((A12 == 0.0f) || (A22 == 0.0f)) {
        A21 = *u11;
        *u11 = fmaf(tt, *u21, *u11) / ct;
        *u21 = fmaf(st,  A21, *u21) / ct;
        A21 = *u12;
        *u12 = fmaf(tt, *u22, *u12) / ct;
        *u22 = fmaf(st,  A21, *u22) / ct;
      }
      st = tt / ct;
      ct = 1.0f / ct;
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
      /* sin(θ) is always non-negative by construction */
      /* this is just an extra bit of info, used later */
      st = -st;
    }
  }
  if (e == 5) {
    /* [ X x ] */
    /* [ 0 0 ] */
    tt = A12 / A11;
    /* 1 / cos */
    ct = hypotf(tt, 1.0f);
    /* apply the right Givens rotation */
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
      *v11 = fmaf(tt, *v12, *v11) / ct;
      *v12 = fmaf(st,  A12, *v12) / ct;
      A12 = *v21;
      *v21 = fmaf(tt, *v22, *v21) / ct;
      *v22 = fmaf(st,  A12, *v22) / ct;
      st = tt / ct;
      ct = 1.0f / ct;
    }
    A11 = *s1;
    A12 = 0.0f;
    e = 0;
  }
  if (e == 3) {
    /* [ X 0 ] */
    /* [ x 0 ] */
    tt = A21 / A11;
    /* 1 / cos */
    ct = hypotf(tt, 1.0f);
    /* apply the left Givens rotation */
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
      *u11 = fmaf(tt, *u21, *u11) / ct;
      *u21 = fmaf(st,  A21, *u21) / ct;
      A21 = *u12;
      *u12 = fmaf(tt, *u22, *u12) / ct;
      *u22 = fmaf(st,  A21, *u22) / ct;
      st = tt / ct;
      ct = 1.0f / ct;
    }
    A11 = *s1;
    A21 = 0.0f;
    e = 0;
  }

  (void)printf("tan(θ)=%s, ", pvn_stoa(s, tt));
  (void)printf("cos(θ)=%s, ", pvn_stoa(s, ct));
  (void)printf("sin(θ)=%s\n\tA =\n", pvn_stoa(s, st));
  (void)printf("%s ", pvn_stoa(s, A11));
  (void)printf("%s\n", pvn_stoa(s, A12));
  (void)printf("%s ", pvn_stoa(s, A21));
  (void)printf("%s\n", pvn_stoa(s, A22));

  float
    tf = 0.0f, cf = 1.0f, sf = 0.0f,
    tp = 0.0f, cp = 1.0f, sp = 0.0f;
  if (e == 13) {
    /* [ x y ] */
    /* [ 0 z ] */
    int xe = 0, ye = 0, ze = 0;
    const float xf = frexpf(A11, &xe);
    const float yf = frexpf(A12, &ye);
    const float zf = frexpf(A22, &ze);

    /* TODO: compute tf and tp */

    /* TODO: update S, scaling back A if necessary, such that all scaled singular values are finite */

    (void)printf("tan(φ)=%s, ", pvn_stoa(s, tf));
    (void)printf("tan(ψ)=%s\n", pvn_stoa(s, tp));

    /* update U */
    if (isfinite(tf)) {
      sf = (tf / cf);
      cf = (1.0f / cf);
    }
    else {
      sf = copysignf(1.0f, tf);
      cf = 0.0f;
    }
    (void)printf("cos(φ)=%s, ", pvn_stoa(s, cf));
    (void)printf("sin(φ)=%s\n", pvn_stoa(s, sf));
    if (copysignf(1.0f, st) != 1.0f) {
      st = -st;
      float tf_t = (tf - tt), cf_t = fmaf(tf, tt, 1.0f), sf_t = 0.0f;
      if (tf_t == 0.0f)
        tf_t *= copysignf(1.0f, cf_t);
      else
        tf_t /= cf_t;
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
      const float _sf_t = -sf_t;
      A21 = *u11;
      *u11 = _sf_t * *u21 + cf_t * *u11;
      *u21 = _sf_t *  A21 - cf_t * *u21;
      A21 = *u12;
      *u12 = _sf_t * *u22 + cf_t * *u12;
      *u22 = _sf_t *  A21 - cf_t * *u22;
      A21 = -1.0f;
    }
    else if (tt != 0.0f) {
      float tft = (tf + tt), cft = fmaf(-tf, tt, 1.0f), sft = 0.0f;
      if (tft == 0.0f)
        tft *= copysignf(1.0f, cft);
      else
        tft /= cft;
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
      A21 = *u11;
      *u11 = cft * *u11 + sft * *u21;
      *u21 = cft * *u21 - sft *  A21;
      A21 = *u12;
      *u12 = cft * *u12 + sft * *u22;
      *u22 = cft * *u22 - sft *  A21;
      A21 = 1.0f;
    }
    else if (tf != 0.0f) {
      A21 = *u11;
      *u11 = cf * *u11 + sf * *u21;
      *u21 = cf * *u21 - sf *  A21;
      A21 = *u12;
      *u12 = cf * *u12 + sf * *u22;
      *u22 = cf * *u22 - sf *  A21;
      A21 = -0.0f;
    }
    else
      A21 = 0.0f;
    (void)printf("U operation=%s\n", pvn_stoa(s, A21));

    /* update V */
    if (isfinite(tp)) {
      sp = (tp / cp);
      cp = (1.0f / cp);
    }
    else {
      sp = copysignf(1.0f, tp);
      cp = 0.0f;
    }
    (void)printf("cos(ψ)=%s, ", pvn_stoa(s, cf));
    (void)printf("sin(ψ)=%s\n", pvn_stoa(s, sf));
    if (tp != 0.0f) {
      A21 = *v11;
      *v11 = *v11 * cp + *v12 * sp;
      *v12 = *v12 * cp -  A21 * sp;
      A21 = *v21;
      *v21 = *v21 * cp + *v22 * sp;
      *v22 = *v22 * cp -  A21 * sp;
      A21 = -0.0f;
    }
    else
      A21 = 0.0f;
    (void)printf("V operation=%s\n", pvn_stoa(s, A21));
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
  /* should not fail beyond this point */
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
  case  7:
  case 11:
  case 13:
  case 14:
    /* QR not needed, extra scaling later */
    e = DBL_MAX_EXP - mxe;
    break;
  case  3:
  case  5:
  case 10:
  case 12:
    /* QR needed, extra scaling later */
    e = DBL_MAX_EXP - mxe - 1;
    break;
  case 15:
    /* at least one QR needed, extra scaling later */
    e = DBL_MAX_EXP - mxe - 2;
    break;
  default:
    return INT_MIN;
  }
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;

  double A11 = *a11, A21 = *a21, A12 = *a12, A22 = *a22;
  if (*es) {
    A11 = scalbn(A11, *es);
    A21 = scalbn(A21, *es);
    A12 = scalbn(A12, *es);
    A22 = scalbn(A22, *es);
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
  /* should not fail beyond this point */
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

  const int knd = kndr | kndi;
  switch (knd) {
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  7:
  case 11:
  case 13:
  case 14:
    /* QR not needed, extra scaling later */
    e = FLT_MAX_EXP - mxe - 1;
    break;
  case  3:
  case  5:
  case 10:
  case 12:
  case 15:
    /* QR needed, extra scaling later */
    e = FLT_MAX_EXP - mxe - 2;
    break;
  default:
    return INT_MIN;
  }
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;

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
  /* should not fail beyond this point */
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
  const int knd = kndr | kndi;
  switch (knd) {
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  7:
  case 11:
  case 13:
  case 14:
    /* QR not needed, extra scaling later */
    e = DBL_MAX_EXP - mxe - 1;
    break;
  case  3:
  case  5:
  case 10:
  case 12:
  case 15:
    /* QR needed, extra scaling later */
    e = DBL_MAX_EXP - mxe - 2;
    break;
  default:
    return INT_MIN;
  }
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;

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
  case  7:
  case 11:
  case 13:
  case 14:
    /* QR not needed, extra scaling later */
    e = LDBL_MAX_EXP - mxe;
    break;
  case  3:
  case  5:
  case 10:
  case 12:
    /* QR needed, extra scaling later */
    e = LDBL_MAX_EXP - mxe - 1;
    break;
  case 15:
    /* at least one QR needed, extra scaling later */
    e = LDBL_MAX_EXP - mxe - 2;
    break;
  default:
    return INT_MIN;
  }
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;

  long double A11 = *a11, A21 = *a21, A12 = *a12, A22 = *a22;
  if (*es) {
    A11 = scalbnl(A11, *es);
    A21 = scalbnl(A21, *es);
    A12 = scalbnl(A12, *es);
    A22 = scalbnl(A22, *es);
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
  const int knd = kndr | kndi;
  switch (knd) {
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  7:
  case 11:
  case 13:
  case 14:
    /* QR not needed, extra scaling later */
    e = LDBL_MAX_EXP - mxe - 1;
    break;
  case  3:
  case  5:
  case 10:
  case 12:
  case 15:
    /* QR needed, extra scaling later */
    e = LDBL_MAX_EXP - mxe - 2;
    break;
  default:
    return INT_MIN;
  }
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;

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
  case  7:
  case 11:
  case 13:
  case 14:
    /* QR not needed, extra scaling later */
    e = FLT128_MAX_EXP - mxe;
    break;
  case  3:
  case  5:
  case 10:
  case 12:
    /* QR needed, extra scaling later */
    e = FLT128_MAX_EXP - mxe - 1;
    break;
  case 15:
    /* at least one QR needed, extra scaling later */
    e = FLT128_MAX_EXP - mxe - 2;
    break;
  default:
    return INT_MIN;
  }
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;

  __float128 A11 = *a11, A21 = *a21, A12 = *a12, A22 = *a22;
  if (*es) {
    A11 = scalbnq(A11, *es);
    A21 = scalbnq(A21, *es);
    A12 = scalbnq(A12, *es);
    A22 = scalbnq(A22, *es);
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
  const int knd = kndr | kndi;
  switch (knd) {
  case  1:
  case  2:
  case  4:
  case  6:
  case  8:
  case  9:
    e = 0;
    break;
  case  7:
  case 11:
  case 13:
  case 14:
    /* QR not needed, extra scaling later */
    e = FLT128_MAX_EXP - mxe - 1;
    break;
  case  3:
  case  5:
  case 10:
  case 12:
  case 15:
    /* QR needed, extra scaling later */
    e = FLT128_MAX_EXP - mxe - 2;
    break;
  default:
    return INT_MIN;
  }
  if (!*es)
    *es = e;
  else if (*es < 0)
    ++*es;

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

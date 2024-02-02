#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
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
  *es = 0;
  /* TODO */
  return 0;
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
  *es = 0;
  /* TODO */
  return 0;
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
  *u11r = 1.0f;
  *u11i = 0.0f;
  *u21r = 0.0f;
  *u21i = 0.0f;
  *u12r = 0.0f;
  *u12i = 0.0f;
  *u22r = 1.0f;
  *u22i = 0.0f;
  *v11r = 1.0f;
  *v11i = 0.0f;
  *v21r = 0.0f;
  *v21i = 0.0f;
  *v12r = 0.0f;
  *v12i = 0.0f;
  *v22r = 1.0f;
  *v22i = 0.0f;
  *s1 = 0.0f;
  *s2 = 0.0f;
  *es = 0;
  int kndi = 0;
  if (*a11i != 0.0f)
    kndi |= 1;
  if (*a21i != 0.0f)
    kndi |= 2;
  if (*a12i != 0.0f)
    kndi |= 4;
  if (*a22i != 0.0f)
    kndi |= 8;
  if (!kndi)
    return
#ifdef _WIN32
      PVN_SLJSV2
#else /* !_WIN32 */
      pvn_sljsv2_
#endif /* ?_WIN32 */
      (a11r, a21r, a12r, a22r, u11r, u21r, u12r, u22r, v11r, v21r, v12r, v22r, s1, s2, es);
  /* TODO */
  return 0;
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
  *u11r = 1.0;
  *u11i = 0.0;
  *u21r = 0.0;
  *u21i = 0.0;
  *u12r = 0.0;
  *u12i = 0.0;
  *u22r = 1.0;
  *u22i = 0.0;
  *v11r = 1.0;
  *v11i = 0.0;
  *v21r = 0.0;
  *v21i = 0.0;
  *v12r = 0.0;
  *v12i = 0.0;
  *v22r = 1.0;
  *v22i = 0.0;
  *s1 = 0.0;
  *s2 = 0.0;
  *es = 0;
  int kndi = 0;
  if (*a11i != 0.0)
    kndi |= 1;
  if (*a21i != 0.0)
    kndi |= 2;
  if (*a12i != 0.0)
    kndi |= 4;
  if (*a22i != 0.0)
    kndi |= 8;
  if (!kndi)
    return
#ifdef _WIN32
      PVN_DLJSV2
#else /* !_WIN32 */
      pvn_dljsv2_
#endif /* ?_WIN32 */
      (a11r, a21r, a12r, a22r, u11r, u21r, u12r, u22r, v11r, v21r, v12r, v22r, s1, s2, es);
  /* TODO */
  return 0;
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
  *es = 0;
  /* TODO */
  return 0;
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
  *u11r = 1.0L;
  *u11i = 0.0L;
  *u21r = 0.0L;
  *u21i = 0.0L;
  *u12r = 0.0L;
  *u12i = 0.0L;
  *u22r = 1.0L;
  *u22i = 0.0L;
  *v11r = 1.0L;
  *v11i = 0.0L;
  *v21r = 0.0L;
  *v21i = 0.0L;
  *v12r = 0.0L;
  *v12i = 0.0L;
  *v22r = 1.0L;
  *v22i = 0.0L;
  *s1 = 0.0L;
  *s2 = 0.0L;
  *es = 0;
  int kndi = 0;
  if (*a11i != 0.0L)
    kndi |= 1;
  if (*a21i != 0.0L)
    kndi |= 2;
  if (*a12i != 0.0L)
    kndi |= 4;
  if (*a22i != 0.0L)
    kndi |= 8;
  if (!kndi)
    return pvn_xljsv2_(a11r, a21r, a12r, a22r, u11r, u21r, u12r, u22r, v11r, v21r, v12r, v22r, s1, s2, es);
  /* TODO */
  return 0;
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
  *es = 0;
  /* TODO */
  return 0;
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
  *u11r = 1.0q;
  *u11i = 0.0q;
  *u21r = 0.0q;
  *u21i = 0.0q;
  *u12r = 0.0q;
  *u12i = 0.0q;
  *u22r = 1.0q;
  *u22i = 0.0q;
  *v11r = 1.0q;
  *v11i = 0.0q;
  *v21r = 0.0q;
  *v21i = 0.0q;
  *v12r = 0.0q;
  *v12i = 0.0q;
  *v22r = 1.0q;
  *v22i = 0.0q;
  *s1 = 0.0q;
  *s2 = 0.0q;
  *es = 0;
  int kndi = 0;
  if (*a11i != 0.0q)
    kndi |= 1;
  if (*a21i != 0.0q)
    kndi |= 2;
  if (*a12i != 0.0q)
    kndi |= 4;
  if (*a22i != 0.0q)
    kndi |= 8;
  if (!kndi)
    return pvn_qljsv2_(a11r, a21r, a12r, a22r, u11r, u21r, u12r, u22r, v11r, v21r, v12r, v22r, s1, s2, es);
  /* TODO */
  return 0;
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

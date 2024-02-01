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
#else /* !PVN_QUADMATH */
int pvn_qljsv2_
(const long double *const a11, const long double *const a21, const long double *const a12, const long double *const a22,
 long double *const u11, long double *const u21, long double *const u12, long double *const u22,
 long double *const v11, long double *const v21, long double *const v12, long double *const v22,
 long double *const s1, long double *const s2, int *const es)
{
  return pvn_xljsv2_(a11, a21, a12, a22, u11, u21, u12, u22, v11, v21, v12, v22, s1, s2, es);
}
#endif /* ?PVN_QUADMATH */
#endif /* !_WIN32 */
#endif /* ?PVN_TEST */

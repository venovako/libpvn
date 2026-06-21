#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc != 6) {
#if (!defined(_WIN32) || defined(_DLL))
    (void)fprintf(stderr, "%s (S|D|X) a b c d\n", *argv);
#else /* _WIN32 && !_DLL */
    (void)fprintf(stderr, "%s (S|D) a b c d\n", *argv);
#endif /* ?(!_WIN32 || _DLL) */
    return EXIT_FAILURE;
  }
  const char T = (char)toupper(*(argv[1]));
  if (T == 'S') {
    const float a = pvn_atos(argv[2]);
    const float b = pvn_atos(argv[3]);
    const float c = pvn_atos(argv[4]);
    const float d = pvn_atos(argv[5]);
    char s[17] = { '\0' };
    float x = __builtin_nanf("x"),
      y = pvn_sdet(a, b, c, d);
    (void)printf("pvn_sdet=%s\n", pvn_stoa(s, y));
    int t = 0;
    y = PVN_FABI(pvn_sdet,PVN_SDET)(&a, &b, &c, &d, &x, &t);
    (void)printf("PVN_SDET=%s (", pvn_stoa(s, y));
    (void)printf("%s,%d)\n", pvn_stoa(s, x), t);
  }
  else if (T == 'D') {
    const double a = pvn_atod(argv[2]);
    const double b = pvn_atod(argv[3]);
    const double c = pvn_atod(argv[4]);
    const double d = pvn_atod(argv[5]);
    char s[26] = { '\0' };
    double x = __builtin_nan("x"),
      y = pvn_ddet(a, b, c, d);
    (void)printf("pvn_ddet=%s\n", pvn_dtoa(s, y));
    int t = 0;
    y = PVN_FABI(pvn_ddet,PVN_DDET)(&a, &b, &c, &d, &x, &t);
    (void)printf("PVN_DDET=%s (", pvn_dtoa(s, y));
    (void)printf("%s,%d)\n", pvn_dtoa(s, x), t);
  }
#if (!defined(_WIN32) || defined(_DLL))
  else if (T == 'X') {
    const long double a = pvn_atox(argv[2]);
    const long double b = pvn_atox(argv[3]);
    const long double c = pvn_atox(argv[4]);
    const long double d = pvn_atox(argv[5]);
    char s[46] = { '\0' };
    long double x = __builtin_nanl("x"),
      y = pvn_xdet(a, b, c, d);
    (void)printf("pvn_xdet=%s\n", pvn_xtoa(s, y));
    int t = 0;
    y = PVN_FABI(pvn_xdet,PVN_XDET)(&a, &b, &c, &d, &x, &t);
    (void)printf("PVN_XDET=%s (", pvn_xtoa(s, y));
    (void)printf("%s,%d)\n", pvn_xtoa(s, x), t);
  }
#endif /* !_WIN32 || _DLL */
  else
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
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
  float r
#if (!defined(PVN_DET_SAFE) || ((PVN_DET_SAFE & 1) == 0))
    = pvn_sdet(*a, *b, *c, *d);
  if (__builtin_isfinite(r))
    *x = __builtin_frexpf(r, t);
  else {
#else /* safe */
    ;
#endif /* ?safe */
#ifdef NDEBUG
    int ea, eb, ec, ed;
#else /* !NDEBUG */
    int ea = 0, eb = 0, ec = 0, ed = 0;
#endif /* ?NDEBUG */
    const float
      fb = __builtin_frexpf(*b, &eb),
      fc = __builtin_frexpf(*c, &ec);
    float
      fa = __builtin_frexpf(*a, &ea),
      fd = __builtin_frexpf(*d, &ed);
    const int
      u = (eb + ec),
      v = (ea + ed);
    const float w = (fb * fc);
    *x = (fa * fd);
    if (w == 0.0f) {
      if (*x == 0.0f) {
        r = *x;
        *t = 0;
      }
      else {
        *x = __builtin_frexpf(*x, t);
        *t += v;
        r = __builtin_scalbnf(*x, *t);
      }
    }
    else if (*x == 0.0f) {
      *x = __builtin_frexpf(-w, t);
      *t += u;
      r = __builtin_scalbnf(*x, *t);
    }
    else {
      int
        s = (u - v);
      *t = (int)__builtin_fmaxf((float)(s - FLT_MAX_EXP), 0.0f);
      *t += (*t & 1);
      const int
        t_2 = -(*t >> 1);
      fa = __builtin_scalbnf(fa, t_2);
      fd = __builtin_scalbnf(fd, t_2);
      s -= *t;
      *t += v;
      const float
        e = __builtin_fmaf(-fb, fc, w),
        f = __builtin_fmaf(fa, fd, -__builtin_scalbnf(w, s));
      *x = __builtin_fmaf(__builtin_scalbnf(0.5f, s), 2.0f * e, f);
      *x = __builtin_frexpf(*x, &s);
      *t += s;
      r = __builtin_scalbnf(*x, *t);
    }
#if (!defined(PVN_DET_SAFE) || ((PVN_DET_SAFE & 1) == 0))
  }
#endif /* !safe */
  return r;
}

double PVN_FABI(pvn_ddet,PVN_DDET)(const double *const a, const double *const b, const double *const c, const double *const d, double *const x, int *const t)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  double r
#if (!defined(PVN_DET_SAFE) || ((PVN_DET_SAFE & 2) == 0))
    = pvn_ddet(*a, *b, *c, *d);
  if (__builtin_isfinite(r))
    *x = __builtin_frexp(r, t);
  else {
#else /* safe */
    ;
#endif /* ?safe */
#ifdef NDEBUG
    int ea, eb, ec, ed;
#else /* !NDEBUG */
    int ea = 0, eb = 0, ec = 0, ed = 0;
#endif /* ?NDEBUG */
    const double
      fb = __builtin_frexp(*b, &eb),
      fc = __builtin_frexp(*c, &ec);
    double
      fa = __builtin_frexp(*a, &ea),
      fd = __builtin_frexp(*d, &ed);
    const int
      u = (eb + ec),
      v = (ea + ed);
    const double w = (fb * fc);
    *x = (fa * fd);
    if (w == 0.0) {
      if (*x == 0.0) {
        r = *x;
        *t = 0;
      }
      else {
        *x = __builtin_frexp(*x, t);
        *t += v;
        r = __builtin_scalbn(*x, *t);
      }
    }
    else if (*x == 0.0) {
      *x = __builtin_frexp(-w, t);
      *t += u;
      r = __builtin_scalbn(*x, *t);
    }
    else {
      int
        s = (u - v);
      *t = (int)__builtin_fmax((double)(s - DBL_MAX_EXP), 0.0);
      *t += (*t & 1);
      const int
        t_2 = -(*t >> 1);
      fa = __builtin_scalbn(fa, t_2);
      fd = __builtin_scalbn(fd, t_2);
      s -= *t;
      *t += v;
      const double
        e = __builtin_fma(-fb, fc, w),
        f = __builtin_fma(fa, fd, -__builtin_scalbn(w, s));
      *x = __builtin_fma(__builtin_scalbn(0.5, s), 2.0 * e, f);
      *x = __builtin_frexp(*x, &s);
      *t += s;
      r = __builtin_scalbn(*x, *t);
    }
#if (!defined(PVN_DET_SAFE) || ((PVN_DET_SAFE & 2) == 0))
  }
#endif /* !safe */
  return r;
}

long double PVN_FABI(pvn_xdet,PVN_XDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d, long double *const x, int *const t)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  long double r
#if (!defined(PVN_DET_SAFE) || ((PVN_DET_SAFE & 4) == 0))
    = pvn_xdet(*a, *b, *c, *d);
  if (__builtin_isfinite(r))
    *x = __builtin_frexpl(r, t);
  else {
#else /* safe */
    ;
#endif /* ?safe */
#ifdef NDEBUG
    int ea, eb, ec, ed;
#else /* !NDEBUG */
    int ea = 0, eb = 0, ec = 0, ed = 0;
#endif /* ?NDEBUG */
    const long double
      fb = __builtin_frexpl(*b, &eb),
      fc = __builtin_frexpl(*c, &ec);
    long double
      fa = __builtin_frexpl(*a, &ea),
      fd = __builtin_frexpl(*d, &ed);
    const int
      u = (eb + ec),
      v = (ea + ed);
    const long double w = (fb * fc);
    *x = (fa * fd);
    if (w == 0.0L) {
      if (*x == 0.0L) {
        r = *x;
        *t = 0;
      }
      else {
        *x = __builtin_frexpl(*x, t);
        *t += v;
        r = __builtin_scalbnl(*x, *t);
      }
    }
    else if (*x == 0.0L) {
      *x = __builtin_frexpl(-w, t);
      *t += u;
      r = __builtin_scalbnl(*x, *t);
    }
    else {
      int
        s = (u - v);
      *t = (int)__builtin_fmaxl((long double)(s - LDBL_MAX_EXP), 0.0L);
      *t += (*t & 1);
      const int
        t_2 = -(*t >> 1);
      fa = __builtin_scalbnl(fa, t_2);
      fd = __builtin_scalbnl(fd, t_2);
      s -= *t;
      *t += v;
      const long double
        e = __builtin_fmal(-fb, fc, w),
        f = __builtin_fmal(fa, fd, -__builtin_scalbnl(w, s));
      *x = __builtin_fmal(__builtin_scalbnl(0.5L, s), 2.0L * e, f);
      *x = __builtin_frexpl(*x, &s);
      *t += s;
      r = __builtin_scalbnl(*x, *t);
    }
#if (!defined(PVN_DET_SAFE) || ((PVN_DET_SAFE & 4) == 0))
  }
#endif /* !safe */
  return r;
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
  __float128 r
#if (!defined(PVN_DET_SAFE) || ((PVN_DET_SAFE & 8) == 0))
    = pvn_qdet(*a, *b, *c, *d);
  if (isfiniteq(r))
    *x = frexpq(r, t);
  else {
#else /* safe */
    ;
#endif /* ?safe */
#ifdef NDEBUG
    int ea, eb, ec, ed;
#else /* !NDEBUG */
    int ea = 0, eb = 0, ec = 0, ed = 0;
#endif /* ?NDEBUG */
    const __float128
      fb = frexpq(*b, &eb),
      fc = frexpq(*c, &ec);
    __float128
      fa = frexpq(*a, &ea),
      fd = frexpq(*d, &ed);
    const int
      u = (eb + ec),
      v = (ea + ed);
    const __float128 w = (fb * fc);
    *x = (fa * fd);
    if (w == 0.0q) {
      if (*x == 0.0q) {
        r = *x;
        *t = 0;
      }
      else {
        *x = frexpq(*x, t);
        *t += v;
        r = scalbnq(*x, *t);
      }
    }
    else if (*x == 0.0q) {
      *x = frexpq(-w, t);
      *t += u;
      r = scalbnq(*x, *t);
    }
    else {
      int
        s = (u - v);
      *t = ((s > FLT128_MAX_EXP) ? (s - FLT128_MAX_EXP) : 0);
      *t += (*t & 1);
      const int
        t_2 = -(*t >> 1);
      fa = scalbnq(fa, t_2);
      fd = scalbnq(fd, t_2);
      s -= *t;
      *t += v;
      const __float128
        e = fmaq(-fb, fc, w),
        f = fmaq(fa, fd, -scalbnq(w, s));
      *x = fmaq(scalbnq(0.5q, s), 2.0q * e, f);
      *x = frexpq(*x, &s);
      *t += s;
      r = scalbnq(*x, *t);
    }
#if (!defined(PVN_DET_SAFE) || ((PVN_DET_SAFE & 8) == 0))
  }
#endif /* !safe */
  return r;
}
#else /* !PVN_QUADMATH */
long double PVN_FABI(pvn_qdet,PVN_QDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d, long double *const x, int *const t)
{
  return PVN_FABI(pvn_xdet,PVN_XDET)(a, b, c, d, x, t);
}
#endif /* ?PVN_QUADMATH */

#ifdef __AVX512F__
#ifndef ZFREXPF
#define ZFREXPF(X,E,M,m)            \
  E = _mm512_getexp_ps(X);          \
  m = _mm512_cmplt_ps_mask(mI, E);  \
  E = _mm512_maskz_add_ps(m, E, O); \
  M = _mm512_maskz_getmant_ps(m, X, _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src)
#else /* ZFREXPF */
#error ZFREXPF already defined
#endif /* ?ZFREXPF */
void PVN_FABI(pvn_zdetf,PVN_ZDETF)(const float *const a, const float *const b, const float *const c, const float *const d, float *const x, int *const t, float *const y)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  PVN_ASSERT(y);
  register __m512 fA = _mm512_load_ps(a);
  register __m512 fB = _mm512_load_ps(b);
  register __m512 fC = _mm512_load_ps(c);
  register __m512 fD = _mm512_load_ps(d);
  register __m512 Z = _mm512_set1_pd(-0.0f);
  register const __m512 O = _mm512_set1_ps(1.0f);
  register const __m512 mI = _mm512_set1_ps(-__builtin_inff());
  register __m512 eA, eB, eC, eD;
  register __mmask16 mA, mB, mC, mD;
  ZFREXPF(fA, eA, fA, mA);
  ZFREXPF(fB, eB, fB, mB);
  ZFREXPF(fC, eC, fC, mC);
  ZFREXPF(fD, eD, fD, mD);
  mB = _kand_mask16(mB, mC);
  mA = _kand_mask16(mA, mD);
  mC = _kand_mask16(mA, mB);
  register const __m512 U = _mm512_maskz_add_ps(mB, eB, eC);
  register const __m512 V = _mm512_maskz_add_ps(mA, eA, eD);
  register __m512 S = _mm512_maskz_sub_ps(mC, U, V);
  register __m512 T = _mm512_set1_ps(FLT_MAX_EXP);
  T = _mm512_maskz_sub_ps(mC, S, T);
  T = _mm512_maskz_max_ps(mC, T, Z);
  register __m512 W = _mm512_maskz_cvtepi32_ps(mC, _mm512_maskz_and_epi32(mC, _mm512_maskz_cvtps_epi32(mC, T), _mm512_set1_epi32(1)));
  T = _mm512_maskz_add_ps(mC, T, W);
  W = _mm512_maskz_mul_ps(mC, T, _mm512_set1_ps(-0.5f));
  fA = _mm512_mask_scalef_ps(fA, mC, fA, W);
  fD = _mm512_mask_scalef_ps(fD, mC, fD, W);
  S = _mm512_maskz_sub_ps(mC, S, T);
  T = _mm512_maskz_add_ps(mC, T, V);
  W = _mm512_maskz_mul_ps(mC, fB, fC);
  register const __m512 E = _mm512_maskz_mul_ps(mC, _mm512_maskz_fnmadd_ps(mC, fB, fC, W), _mm512_set1_ps(2.0f));
  W = _mm512_maskz_scalef_ps(mC, W, S);
  register const __m512 F = _mm512_maskz_fmsub_ps(mC, fA, fD, W);
  W = _mm512_maskz_scalef_ps(mC, _mm512_set1_ps(0.5f), S);
  W = _mm512_maskz_fmadd_ps(mC, W, E, F);
  /* !mA & mB */
  mD = _kandn_mask16(mA, mB);
  Z = _mm512_castsi512_ps(_mm512_mask_xor_epi32(_mm512_castps_si512(fB), mD, _mm512_castps_si512(fB), _mm512_castps_si512(Z)));
  W = _mm512_mask_mul_ps(W, mD, Z, fC);
  T = _mm512_mask_mov_ps(T, mD, U);
  /* !mB & mA */
  mD = _kandn_mask16(mB, mA);
  W = _mm512_mask_mul_ps(W, mD, fA, fD);
  T = _mm512_mask_mov_ps(T, mD, V);
  ZFREXPF(W, S, W, mD);
  T = _mm512_add_ps(T, S);
  _mm512_store_ps(x, W);
  _mm512_store_epi32(t, _mm512_cvtps_epi32(T));
  W = _mm512_scalef_ps(W, T);
  _mm512_store_ps(y, W);
}
#ifndef ZFREXP
#define ZFREXP(X,E,M,m)             \
  E = _mm512_getexp_pd(X);          \
  m = _mm512_cmplt_pd_mask(mI, E);  \
  E = _mm512_maskz_add_pd(m, E, O); \
  M = _mm512_maskz_getmant_pd(m, X, _MM_MANT_NORM_p5_1, _MM_MANT_SIGN_src)
#else /* ZFREXP */
#error ZFREXP already defined
#endif /* ?ZFREXP */
void PVN_FABI(pvn_zdet,PVN_ZDET)(const double *const a, const double *const b, const double *const c, const double *const d, double *const x, int *const t, double *const y)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  PVN_ASSERT(y);
  register __m512d fA = _mm512_load_pd(a);
  register __m512d fB = _mm512_load_pd(b);
  register __m512d fC = _mm512_load_pd(c);
  register __m512d fD = _mm512_load_pd(d);
  register __m512d Z = _mm512_set1_pd(-0.0);
  register const __m512d O = _mm512_set1_pd(1.0);
  register const __m512d mI = _mm512_set1_pd(-__builtin_inf());
  register __m512d eA, eB, eC, eD;
  register __mmask8 mA, mB, mC, mD;
  ZFREXP(fA, eA, fA, mA);
  ZFREXP(fB, eB, fB, mB);
  ZFREXP(fC, eC, fC, mC);
  ZFREXP(fD, eD, fD, mD);
  mB = (__mmask8)_kand_mask16((__mmask16)mB, (__mmask16)mC);
  mA = (__mmask8)_kand_mask16((__mmask16)mA, (__mmask16)mD);
  mC = (__mmask8)_kand_mask16((__mmask16)mA, (__mmask16)mB);
  register const __m512d U = _mm512_maskz_add_pd(mB, eB, eC);
  register const __m512d V = _mm512_maskz_add_pd(mA, eA, eD);
  register __m512d S = _mm512_maskz_sub_pd(mC, U, V);
  register __m512d T = _mm512_set1_pd(DBL_MAX_EXP);
  T = _mm512_maskz_sub_pd(mC, S, T);
  T = _mm512_maskz_max_pd(mC, T, Z);
  register __m512d W = _mm512_maskz_cvtepi32_pd(mC, _mm256_and_si256(_mm512_maskz_cvtpd_epi32(mC, T), _mm256_set1_epi32(1)));
  T = _mm512_maskz_add_pd(mC, T, W);
  W = _mm512_maskz_mul_pd(mC, T, _mm512_set1_pd(-0.5));
  fA = _mm512_mask_scalef_pd(fA, mC, fA, W);
  fD = _mm512_mask_scalef_pd(fD, mC, fD, W);
  S = _mm512_maskz_sub_pd(mC, S, T);
  T = _mm512_maskz_add_pd(mC, T, V);
  W = _mm512_maskz_mul_pd(mC, fB, fC);
  register const __m512d E = _mm512_maskz_mul_pd(mC, _mm512_maskz_fnmadd_pd(mC, fB, fC, W), _mm512_set1_pd(2.0));
  W = _mm512_maskz_scalef_pd(mC, W, S);
  register const __m512d F = _mm512_maskz_fmsub_pd(mC, fA, fD, W);
  W = _mm512_maskz_scalef_pd(mC, _mm512_set1_pd(0.5), S);
  W = _mm512_maskz_fmadd_pd(mC, W, E, F);
  /* !mA & mB */
  mD = (__mmask8)_kandn_mask16((__mmask16)mA, (__mmask16)mB);
  Z = _mm512_castsi512_pd(_mm512_mask_xor_epi64(_mm512_castpd_si512(fB), mD, _mm512_castpd_si512(fB), _mm512_castpd_si512(Z)));
  W = _mm512_mask_mul_pd(W, mD, Z, fC);
  T = _mm512_mask_mov_pd(T, mD, U);
  /* !mB & mA */
  mD = (__mmask8)_kandn_mask16((__mmask16)mB, (__mmask16)mA);
  W = _mm512_mask_mul_pd(W, mD, fA, fD);
  T = _mm512_mask_mov_pd(T, mD, V);
  ZFREXP(W, S, W, mD);
  T = _mm512_add_pd(T, S);
  _mm512_store_pd(x, W);
  _mm256_store_si256((__m256i*)t, _mm512_cvtpd_epi32(T));
  W = _mm512_scalef_pd(W, T);
  _mm512_store_pd(y, W);
}
#endif /* __AVX512F__ */
#endif /* ?PVN_TEST */

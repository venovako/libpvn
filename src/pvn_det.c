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
  if (__builtin_isnormal(r))
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
      *t = ((s > FLT_MAX_EXP) ? (s - FLT_MAX_EXP) : 0);
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
      *x = __builtin_fmaf(__builtin_scalbnf(0.5f, s), (e + e), f);
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
  if (__builtin_isnormal(r))
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
      *t = ((s > DBL_MAX_EXP) ? (s - DBL_MAX_EXP) : 0);
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
      *x = __builtin_fma(__builtin_scalbn(0.5, s), (e + e), f);
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
  if (__builtin_isnormal(r))
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
      *t = ((s > LDBL_MAX_EXP) ? (s - LDBL_MAX_EXP) : 0);
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
      *x = __builtin_fmal(__builtin_scalbnl(0.5L, s), (e + e), f);
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
  if (isnormalq(r))
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
      *x = fmaq(scalbnq(0.5q, s), (e + e), f);
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
int PVN_FABI(pvn_zdetf,PVN_ZDETF)(const __m512 *const a, const __m512 *const b, const __m512 *const c, const __m512 *const d, __m512 *const x, __m512i *const t, __m512 *const h)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  PVN_ASSERT(h);
  register const __m512
    mI = _mm512_set1_ps(-__builtin_inff()),
    Z = _mm512_set1_ps(-0.0f),
    O = _mm512_set1_ps(1.0f);
  register __m512 eA, eB, eC, eD, U, V, S, T, W,
    fA = _mm512_load_ps(a),
    fB = _mm512_load_ps(b),
    fC = _mm512_load_ps(c),
    fD = _mm512_load_ps(d),
    E = _mm512_set1_ps(FLT_MAX_EXP),
    F = _mm512_set1_ps(-0.5f),
    X = _mm512_set1_ps(0.5f);
  register __m512i J = _mm512_set1_epi32(1);
  register __mmask16 mA, mB, mC, mD;
  ZFREXPF(fB, eB, fB, mB);
  ZFREXPF(fC, eC, fC, mC);
  ZFREXPF(fA, eA, fA, mA);
  ZFREXPF(fD, eD, fD, mD);
  mB = _kand_mask16(mB, mC);
  mA = _kand_mask16(mA, mD);
  mC = _kand_mask16(mA, mB);
  U = _mm512_maskz_add_ps(mB, eB, eC);
  V = _mm512_maskz_add_ps(mA, eA, eD);
  S = _mm512_maskz_sub_ps(mC, U, V);
  W = _mm512_maskz_sub_ps(mC, S, E);
  T = _mm512_maskz_max_ps(mC, W, Z);
  J = _mm512_maskz_and_epi32(mC, _mm512_maskz_cvtps_epi32(mC, T), J);
  W = _mm512_maskz_cvtepi32_ps(mC, J); // W = T & 1 {mC}
  T = _mm512_maskz_add_ps(mC, T, W);
  W = _mm512_maskz_mul_ps(mC, T, F);
  fA = _mm512_mask_scalef_ps(fA, mC, fA, W);
  fD = _mm512_mask_scalef_ps(fD, mC, fD, W);
  S = _mm512_maskz_sub_ps(mC, S, T);
  T = _mm512_maskz_add_ps(mC, T, V);
  W = _mm512_maskz_mul_ps(mC, fB, fC);
  E = _mm512_maskz_fnmadd_ps(mC, fB, fC, W);
  W = _mm512_maskz_scalef_ps(mC, W, S);
  E = _mm512_maskz_add_ps(mC, E, E);
  F = _mm512_maskz_fmsub_ps(mC, fA, fD, W);
  W = _mm512_maskz_scalef_ps(mC, X, S);
  X = _mm512_maskz_fmadd_ps(mC, W, E, F);
  mD = _kandn_mask16(mA, mB); // !mA & mB
  W = _mm512_maskz_xor_ps(mD, fB, Z); // W = -fB {mD}
  X = _mm512_mask_mul_ps(X, mD, W, fC);
  T = _mm512_mask_mov_ps(T, mD, U);
  mD = _kandn_mask16(mB, mA); // !mB & mA
  X = _mm512_mask_mul_ps(X, mD, fA, fD);
  T = _mm512_mask_mov_ps(T, mD, V);
  // !mA & !mB should have already been implicitly handled
  ZFREXPF(X, S, X, mD);
  T = _mm512_add_ps(T, S);
  J = _mm512_cvtps_epi32(T);
  _mm512_store_epi32(t, J);
  _mm512_store_ps(x, X);
  W = _mm512_scalef_ps(X, T);
  _mm512_store_ps(h, W);
  return _mm_popcnt_u32(_cvtmask16_u32(mD));
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
int PVN_FABI(pvn_zdet,PVN_ZDET)(const __m512d *const a, const __m512d *const b, const __m512d *const c, const __m512d *const d, __m512d *const x, __m256i *const t, __m512d *const h)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(x);
  PVN_ASSERT(t);
  PVN_ASSERT(h);
  register const __m512d
    mI = _mm512_set1_pd(-__builtin_inf()),
    Z = _mm512_set1_pd(-0.0),
    O = _mm512_set1_pd(1.0);
  register __m512d eA, eB, eC, eD, U, V, S, T, W,
    fA = _mm512_load_pd(a),
    fB = _mm512_load_pd(b),
    fC = _mm512_load_pd(c),
    fD = _mm512_load_pd(d),
    E = _mm512_set1_pd(DBL_MAX_EXP),
    F = _mm512_set1_pd(-0.5),
    X = _mm512_set1_pd(0.5);
  register __m256i J = _mm256_set1_epi32(1);
  register __mmask8 mA, mB, mC, mD;
  ZFREXP(fB, eB, fB, mB);
  ZFREXP(fC, eC, fC, mC);
  ZFREXP(fA, eA, fA, mA);
  ZFREXP(fD, eD, fD, mD);
  mB = _kand_mask8(mB, mC);
  mA = _kand_mask8(mA, mD);
  mC = _kand_mask8(mA, mB);
  U = _mm512_maskz_add_pd(mB, eB, eC);
  V = _mm512_maskz_add_pd(mA, eA, eD);
  S = _mm512_maskz_sub_pd(mC, U, V);
  W = _mm512_maskz_sub_pd(mC, S, E);
  T = _mm512_maskz_max_pd(mC, W, Z);
  J = _mm256_and_si256(_mm512_maskz_cvtpd_epi32(mC, T), J);
  W = _mm512_maskz_cvtepi32_pd(mC, J); // W = T & 1 {mC}
  T = _mm512_maskz_add_pd(mC, T, W);
  W = _mm512_maskz_mul_pd(mC, T, F);
  fA = _mm512_mask_scalef_pd(fA, mC, fA, W);
  fD = _mm512_mask_scalef_pd(fD, mC, fD, W);
  S = _mm512_maskz_sub_pd(mC, S, T);
  T = _mm512_maskz_add_pd(mC, T, V);
  W = _mm512_maskz_mul_pd(mC, fB, fC);
  E = _mm512_maskz_fnmadd_pd(mC, fB, fC, W);
  W = _mm512_maskz_scalef_pd(mC, W, S);
  E = _mm512_maskz_add_pd(mC, E, E);
  F = _mm512_maskz_fmsub_pd(mC, fA, fD, W);
  W = _mm512_maskz_scalef_pd(mC, X, S);
  X = _mm512_maskz_fmadd_pd(mC, W, E, F);
  mD = _kandn_mask8(mA, mB); // !mA & mB
  W = _mm512_maskz_xor_pd(mD, fB, Z); // Z = -fB {mD}
  X = _mm512_mask_mul_pd(X, mD, W, fC);
  T = _mm512_mask_mov_pd(T, mD, U);
  mD = _kandn_mask8(mB, mA); // !mB & mA
  X = _mm512_mask_mul_pd(X, mD, fA, fD);
  T = _mm512_mask_mov_pd(T, mD, V);
  // !mA & !mB should have already been implicitly handled
  ZFREXP(X, S, X, mD);
  T = _mm512_add_pd(T, S);
  J = _mm512_cvtpd_epi32(T);
  _mm256_store_si256(t, J);
  _mm512_store_pd(x, X);
  W = _mm512_scalef_pd(X, T);
  _mm512_store_pd(h, W);
  return _mm_popcnt_u32(_cvtmask8_u32(mD));
}
#endif /* __AVX512F__ */
#endif /* ?PVN_TEST */

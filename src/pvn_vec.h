#ifndef PVN_VEC_H
#define PVN_VEC_H

#ifndef PVN_H
#error pvn_vec.h not intended for direct inclusion
#endif /* !PVN_H */

/* imperfect detection of the maximal vector length */
#ifndef PVN_VECLEN
#ifdef __AVX512F__
#define PVN_VECLEN 64u
#else /* !__AVX512F__ */
#ifdef __x86_64__
/* assume AVX2 */
#define PVN_VECLEN 32u
#else /* !__x86_64__ */
#define PVN_VECLEN 16u
#endif /* ?__x86_64__ */
#endif /* ?__AVX512F__ */
#endif /* !PVN_VECLEN */

#ifndef PVN_SAFELEN
#define PVN_SAFELEN(x) ((PVN_VECLEN) / sizeof(x))
#else /* PVN_SAFELEN */
#error PVN_SAFELEN already defined
#endif /* ?PVN_SAFELEN */

static inline float pvn_v1s_rsqrt(const float x)
{
  return (1.0f / __builtin_sqrtf(x));
}

static inline float pvn_v1s_hypot(const float x, const float y)
{
  const float X = __builtin_fabsf(x);
  const float Y = __builtin_fabsf(y);
  const float m = __builtin_fminf(X, Y);
  const float M = __builtin_fmaxf(X, Y);
  const float q = (m / M);
  const float Q = __builtin_fmaxf(q, 0.0f);
  const float S = __builtin_fmaf(Q, Q, 1.0f);
  const float s = __builtin_sqrtf(S);
  return (M * s);
}

static inline double pvn_v1d_rsqrt(const double x)
{
  return (1.0 / __builtin_sqrt(x));
}

static inline double pvn_v1d_hypot(const double x, const double y)
{
  const double X = __builtin_fabs(x);
  const double Y = __builtin_fabs(y);
  const double m = __builtin_fmin(X, Y);
  const double M = __builtin_fmax(X, Y);
  const double q = (m / M);
  const double Q = __builtin_fmax(q, 0.0);
  const double S = __builtin_fma(Q, Q, 1.0);
  const double s = __builtin_sqrt(S);
  return (M * s);
}

static inline long double pvn_v1x_rsqrt(const long double x)
{
  return (1.0L / __builtin_sqrtl(x));
}

static inline long double pvn_v1x_hypot(const long double x, const long double y)
{
  const long double X = __builtin_fabsl(x);
  const long double Y = __builtin_fabsl(y);
  const long double m = __builtin_fminl(X, Y);
  const long double M = __builtin_fmaxl(X, Y);
  const long double q = (m / M);
  const long double Q = __builtin_fmaxl(q, 0.0L);
  const long double S = __builtin_fmal(Q, Q, 1.0L);
  const long double s = __builtin_sqrtl(S);
  return (M * s);
}

#ifdef PVN_QUADMATH
static inline __float128 pvn_v1q_rsqrt(const __float128 x)
{
  return (1.0q / sqrtq(x));
}

static inline __float128 pvn_v1q_hypot(const __float128 x, const __float128 y)
{
  const __float128 X = fabsq(x);
  const __float128 Y = fabsq(y);
  const __float128 m = fminq(X, Y);
  const __float128 M = fmaxq(X, Y);
  const __float128 q = (m / M);
  const __float128 Q = fmaxq(q, 0.0q);
  const __float128 S = fmaq(Q, Q, 1.0q);
  const __float128 s = sqrtq(S);
  return (M * s);
}
#endif /* PVN_QUADMATH */

#if (defined(__AVX__) && defined(__FMA__))
#include <immintrin.h>

static inline __m128 pvn_v4s_rsqrt(register const __m128 x)
{
  return _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(x)); //_mm_invsqrt_ps(x)
}

static inline float pvn_v4s_hypot_red(register const __m128 x)
{
  alignas(16) float v[4];
  _mm_store_ps(v, x);
  return pvn_v1s_hypot(pvn_v1s_hypot(v[0], v[2]), pvn_v1s_hypot(v[1], v[3]));
}

static inline __m128 pvn_v4s_hypot(register const __m128 x, register const __m128 y)
{
  register const __m128 z = _mm_set1_ps(-0.0f);
  register const __m128 o = _mm_set1_ps(1.0f);
  register const __m128 X = _mm_andnot_ps(z, x);
  register const __m128 Y = _mm_andnot_ps(z, y);
  register const __m128 m = _mm_min_ps(X, Y);
  register const __m128 M = _mm_max_ps(X, Y);
  register const __m128 q = _mm_div_ps(m, M);
  register const __m128 Q = _mm_max_ps(q, z);
  register const __m128 S = _mm_fmadd_ps(Q, Q, o);
  register const __m128 s = _mm_sqrt_ps(S);
  register const __m128 h = _mm_mul_ps(M, s);
  return h;
}

static inline __m128d pvn_v2d_rsqrt(register const __m128d x)
{
  return _mm_div_pd(_mm_set1_pd(1.0), _mm_sqrt_pd(x)); //_mm_invsqrt_pd(x)
}

static inline double pvn_v2d_hypot_red(register const __m128d x)
{
  alignas(16) double v[2];
  _mm_store_pd(v, x);
  return pvn_v1d_hypot(v[0], v[1]);
}

static inline __m128d pvn_v2d_hypot(register const __m128d x, register const __m128d y)
{
  register const __m128d z = _mm_set1_pd(-0.0);
  register const __m128d o = _mm_set1_pd(1.0);
  register const __m128d X = _mm_andnot_pd(z, x);
  register const __m128d Y = _mm_andnot_pd(z, y);
  register const __m128d m = _mm_min_pd(X, Y);
  register const __m128d M = _mm_max_pd(X, Y);
  register const __m128d q = _mm_div_pd(m, M);
  register const __m128d Q = _mm_max_pd(q, z);
  register const __m128d S = _mm_fmadd_pd(Q, Q, o);
  register const __m128d s = _mm_sqrt_pd(S);
  register const __m128d h = _mm_mul_pd(M, s);
  return h;
}

static inline __m256 pvn_v8s_rsqrt(register const __m256 x)
{
  return _mm256_div_ps(_mm256_set1_ps(1.0f), _mm256_sqrt_ps(x)); //_mm256_invsqrt_ps(x)
}

static inline float pvn_v8s_hypot_red(register const __m256 x)
{
  return pvn_v4s_hypot_red(pvn_v4s_hypot(_mm256_extractf128_ps(x, 0), _mm256_extractf128_ps(x, 1)));
}

static inline __m256 pvn_v8s_hypot(register const __m256 x, register const __m256 y)
{
  register const __m256 z = _mm256_set1_ps(-0.0f);
  register const __m256 o = _mm256_set1_ps(1.0f);
  register const __m256 X = _mm256_andnot_ps(z, x);
  register const __m256 Y = _mm256_andnot_ps(z, y);
  register const __m256 m = _mm256_min_ps(X, Y);
  register const __m256 M = _mm256_max_ps(X, Y);
  register const __m256 q = _mm256_div_ps(m, M);
  register const __m256 Q = _mm256_max_ps(q, z);
  register const __m256 S = _mm256_fmadd_ps(Q, Q, o);
  register const __m256 s = _mm256_sqrt_ps(S);
  register const __m256 h = _mm256_mul_ps(M, s);
  return h;
}

static inline __m256d pvn_v4d_rsqrt(register const __m256d x)
{
  return _mm256_div_pd(_mm256_set1_pd(1.0), _mm256_sqrt_pd(x)); //_mm256_invsqrt_pd(x)
}

static inline double pvn_v4d_hypot_red(register const __m256d x)
{
  return pvn_v2d_hypot_red(pvn_v2d_hypot(_mm256_extractf128_pd(x, 0), _mm256_extractf128_pd(x, 1)));
}

static inline __m256d pvn_v4d_hypot(register const __m256d x, register const __m256d y)
{
  register const __m256d z = _mm256_set1_pd(-0.0);
  register const __m256d o = _mm256_set1_pd(1.0);
  register const __m256d X = _mm256_andnot_pd(z, x);
  register const __m256d Y = _mm256_andnot_pd(z, y);
  register const __m256d m = _mm256_min_pd(X, Y);
  register const __m256d M = _mm256_max_pd(X, Y);
  register const __m256d q = _mm256_div_pd(m, M);
  register const __m256d Q = _mm256_max_pd(q, z);
  register const __m256d S = _mm256_fmadd_pd(Q, Q, o);
  register const __m256d s = _mm256_sqrt_pd(S);
  register const __m256d h = _mm256_mul_pd(M, s);
  return h;
}

#ifdef __AVX512F__
static inline __m512 pvn_v16s_rsqrt(register const __m512 x)
{
  return _mm512_div_ps(_mm512_set1_ps(1.0f), _mm512_sqrt_ps(x)); //_mm512_invsqrt_ps(x)
}

static inline float pvn_v16s_hypot_red(register const __m512 x)
{
  return pvn_v8s_hypot_red(pvn_v8s_hypot(_mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(x), 0)), _mm256_castpd_ps(_mm512_extractf64x4_pd(_mm512_castps_pd(x), 1))));
}

static inline __m512 pvn_v16s_hypot(register const __m512 x, register const __m512 y)
{
  register const __m512 z = _mm512_set1_ps(-0.0f);
  register const __m512 o = _mm512_set1_ps(1.0f);
  register const __m512 X = _mm512_castsi512_ps(_mm512_andnot_epi32(_mm512_castps_si512(z), _mm512_castps_si512(x)));
  register const __m512 Y = _mm512_castsi512_ps(_mm512_andnot_epi32(_mm512_castps_si512(z), _mm512_castps_si512(y)));
  register const __m512 m = _mm512_min_ps(X, Y);
  register const __m512 M = _mm512_max_ps(X, Y);
  register const __m512 q = _mm512_div_ps(m, M);
  register const __m512 Q = _mm512_max_ps(q, z);
  register const __m512 S = _mm512_fmadd_ps(Q, Q, o);
  register const __m512 s = _mm512_sqrt_ps(S);
  register const __m512 h = _mm512_mul_ps(M, s);
  return h;
}

static inline __m512d pvn_v8d_rsqrt(register const __m512d x)
{
  return _mm512_div_pd(_mm512_set1_pd(1.0), _mm512_sqrt_pd(x)); //_mm512_invsqrt_pd(x)
}

static inline double pvn_v8d_hypot_red(register const __m512d x)
{
  return pvn_v4d_hypot_red(pvn_v4d_hypot(_mm512_extractf64x4_pd(x, 0), _mm512_extractf64x4_pd(x, 1)));
}

static inline __m512d pvn_v8d_hypot(register const __m512d x, register const __m512d y)
{
  register const __m512d z = _mm512_set1_pd(-0.0);
  register const __m512d o = _mm512_set1_pd(1.0);
  register const __m512d X = _mm512_castsi512_pd(_mm512_andnot_epi64(_mm512_castpd_si512(z), _mm512_castpd_si512(x)));
  register const __m512d Y = _mm512_castsi512_pd(_mm512_andnot_epi64(_mm512_castpd_si512(z), _mm512_castpd_si512(y)));
  register const __m512d m = _mm512_min_pd(X, Y);
  register const __m512d M = _mm512_max_pd(X, Y);
  register const __m512d q = _mm512_div_pd(m, M);
  register const __m512d Q = _mm512_max_pd(q, z);
  register const __m512d S = _mm512_fmadd_pd(Q, Q, o);
  register const __m512d s = _mm512_sqrt_pd(S);
  register const __m512d h = _mm512_mul_pd(M, s);
  return h;
}
#endif /* __AVX512F__ */
#endif /* __AVX__ && __FMA__ */

PVN_EXTERN_C unsigned PVN_FABI(pvn_vec_len,PVN_VEC_LEN)();

PVN_EXTERN_C double PVN_FABI(pvn_v1d_nrmf,PVN_V1D_NRMF)(const size_t *const n, const double *const x);
#if (defined(__AVX__) && defined(__FMA__))
PVN_EXTERN_C double PVN_FABI(pvn_v2d_nrmf,PVN_V2D_NRMF)(const size_t *const n, const double *const x);
PVN_EXTERN_C double PVN_FABI(pvn_v4d_nrmf,PVN_V4D_NRMF)(const size_t *const n, const double *const x);
#ifdef __AVX512F__
PVN_EXTERN_C double PVN_FABI(pvn_v8d_nrmf,PVN_V8D_NRMF)(const size_t *const n, const double *const x);
#endif /* __AVX512F__ */
#endif /* __AVX__ && __FMA__ */

#endif /* !PVN_VEC_H */

#ifndef PVN_VEC_H
#define PVN_VEC_H

#ifndef PVN_H
#error pvn_vec.h not intended for direct inclusion
#endif /* !PVN_H */

static inline float pvn_v1s_rsqrt(const float x)
{
  return (1.0f / sqrtf(x));
}

static inline float pvn_v1s_hypot(const float x, const float y)
{
  const float X = fabsf(x);
  const float Y = fabsf(y);
  const float m = fminf(X, Y);
  const float M = fmaxf(X, Y);
  const float q = (m / M);
  const float Q = fmaxf(q, 0.0f);
  const float S = fmaf(Q, Q, 1.0f);
  const float s = sqrtf(S);
  const float h = (M * s);
  return h;
}

static inline double pvn_v1d_rsqrt(const double x)
{
  return (1.0 / sqrt(x));
}

static inline double pvn_v1d_hypot(const double x, const double y)
{
  const double X = fabs(x);
  const double Y = fabs(y);
  const double m = fmin(X, Y);
  const double M = fmax(X, Y);
  const double q = (m / M);
  const double Q = fmax(q, 0.0);
  const double S = fma(Q, Q, 1.0);
  const double s = sqrt(S);
  const double h = (M * s);
  return h;
}

static inline long double pvn_v1x_rsqrt(const long double x)
{
  return (1.0L / sqrtl(x));
}

static inline long double pvn_v1x_hypot(const long double x, const long double y)
{
  const long double X = fabsl(x);
  const long double Y = fabsl(y);
  const long double m = fminl(X, Y);
  const long double M = fmaxl(X, Y);
  const long double q = (m / M);
  const long double Q = fmaxl(q, 0.0L);
  const long double S = fmal(Q, Q, 1.0L);
  const long double s = sqrtl(S);
  const long double h = (M * s);
  return h;
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
  const __float128 h = (M * s);
  return h;
}
#endif /* PVN_QUADMATH */

#if (defined(__AVX__) && defined(__FMA__))
#include <immintrin.h>

static inline __m256 pvn_v8s_hypot(register const __m256 x, register const __m256 y)
{
  register const __m256 z = _mm256_set1_ps(-0.0f);
  register const __m256 o = _mm256_set1_ps(1.0f);
  register const __m256 X = _mm256_xor_ps(x, z);
  register const __m256 Y = _mm256_xor_ps(y, z);
  register const __m256 m = _mm256_min_ps(X, Y);
  register const __m256 M = _mm256_max_ps(X, Y);
  register const __m256 q = _mm256_div_ps(m, M);
  register const __m256 Q = _mm256_max_ps(q, z);
  register const __m256 S = _mm256_fmadd_ps(Q, Q, o);
  register const __m256 s = _mm256_sqrt_ps(S);
  register const __m256 h = _mm256_mul_ps(M, s);
  return h;
}

static inline __m256d pvn_v4d_hypot(register const __m256d x, register const __m256d y)
{
  register const __m256d z = _mm256_set1_pd(-0.0);
  register const __m256d o = _mm256_set1_pd(1.0);
  register const __m256d X = _mm256_xor_pd(x, z);
  register const __m256d Y = _mm256_xor_pd(y, z);
  register const __m256d m = _mm256_min_pd(X, Y);
  register const __m256d M = _mm256_max_pd(X, Y);
  register const __m256d q = _mm256_div_pd(m, M);
  register const __m256d Q = _mm256_max_pd(q, z);
  register const __m256d S = _mm256_fmadd_pd(Q, Q, o);
  register const __m256d s = _mm256_sqrt_pd(S);
  register const __m256d h = _mm256_mul_pd(M, s);
  return h;
}
#endif /* __AVX__ && __FMA__ */

#ifdef __AVX512F__
static inline __m512 pvn_v16s_hypot(register const __m512 x, register const __m512 y)
{
  register const __m512 z = _mm512_set1_ps(-0.0f);
  register const __m512 o = _mm512_set1_ps(1.0f);
  register const __m512 X = _mm512_castsi512_ps(_mm512_xor_epi32(_mm512_castps_si512(x), _mm512_castps_si512(z)));
  register const __m512 Y = _mm512_castsi512_ps(_mm512_xor_epi32(_mm512_castps_si512(y), _mm512_castps_si512(z)));
  register const __m512 m = _mm512_min_ps(X, Y);
  register const __m512 M = _mm512_max_ps(X, Y);
  register const __m512 q = _mm512_div_ps(m, M);
  register const __m512 Q = _mm512_max_ps(q, z);
  register const __m512 S = _mm512_fmadd_ps(Q, Q, o);
  register const __m512 s = _mm512_sqrt_ps(S);
  register const __m512 h = _mm512_mul_ps(M, s);
  return h;
}

static inline __m512d pvn_v8d_hypot(register const __m512d x, register const __m512d y)
{
  register const __m512d z = _mm512_set1_pd(-0.0);
  register const __m512d o = _mm512_set1_pd(1.0);
  register const __m512d X = _mm512_castsi512_pd(_mm512_xor_epi64(_mm512_castpd_si512(x), _mm512_castpd_si512(z)));
  register const __m512d Y = _mm512_castsi512_pd(_mm512_xor_epi64(_mm512_castpd_si512(y), _mm512_castpd_si512(z)));
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
#endif /* !PVN_VEC_H */

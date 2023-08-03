#ifndef PVN_AUX_H
#define PVN_AUX_H

#ifndef PVN_H
#error pvn_aux.h not intended for direct inclusion
#endif /* !PVN_H */

#ifndef PVN_LF64
#ifdef _LARGEFILE64_SOURCE
#define PVN_LF64 O_LARGEFILE
#else /* !_LARGEFILE64_SOURCE */
#define PVN_LF64 0
#endif /* ?_LARGEFILE64_SOURCE */
#else /* PVN_LF64 */
#error PVN_LF64 already defined
#endif /* ?PVN_LF64 */

static inline int pvn_imin(const int a, const int b)
{
  return ((a <= b) ? a : b);
}

static inline int pvn_imin3(const int a, const int b, const int c)
{
  return pvn_imin(pvn_imin(a, b), c);
}

static inline int pvn_imin4(const int a, const int b, const int c, const int d)
{
  return pvn_imin(pvn_imin(a, b), pvn_imin(c, d));
}

static inline int pvn_imax(const int a, const int b)
{
  return ((a >= b) ? a : b);
}

static inline int pvn_imax3(const int a, const int b, const int c)
{
  return pvn_imax(pvn_imax(a, b), c);
}

static inline int pvn_imax4(const int a, const int b, const int c, const int d)
{
  return pvn_imax(pvn_imax(a, b), pvn_imax(c, d));
}

static inline long pvn_lmin(const long a, const long b)
{
  return ((a <= b) ? a : b);
}

static inline long pvn_lmax(const long a, const long b)
{
  return ((a >= b) ? a : b);
}

static inline unsigned pvn_umin(const unsigned a, const unsigned b)
{
  return ((a <= b) ? a : b);
}

static inline unsigned pvn_umax(const unsigned a, const unsigned b)
{
  return ((a >= b) ? a : b);
}

static inline size_t pvn_zmin(const size_t a, const size_t b)
{
  return ((a <= b) ? a : b);
}

static inline size_t pvn_zmax(const size_t a, const size_t b)
{
  return ((a >= b) ? a : b);
}

PVN_EXTERN_C size_t pvn_gcd(const size_t a, const size_t b);
PVN_EXTERN_C size_t pvn_gcd_(const size_t *const a, const size_t *const b);
PVN_EXTERN_C size_t pvn_lcm(const size_t a, const size_t b);
PVN_EXTERN_C size_t pvn_lcm_(const size_t *const a, const size_t *const b);

#endif /* !PVN_AUX_H */

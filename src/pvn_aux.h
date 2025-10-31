#ifndef PVN_AUX_H
#define PVN_AUX_H

#ifndef PVN_H
#error pvn_aux.h not intended for direct inclusion
#endif /* !PVN_H */

#ifndef pvn_Tswp
#define pvn_Tswp(T,t)                                   \
static inline void pvn_##t##swp(T *const a, T *const b) \
{                                                       \
  PVN_ASSERT(a);                                        \
  PVN_ASSERT(b);                                        \
  const T c = *a;                                       \
  *a = *b;                                              \
  *b = c;                                               \
}
#else /* pvn_Tswp */
#error pvn_Tswp already defined
#endif /* ?pvn_Tswp */

pvn_Tswp(int,i)
pvn_Tswp(long,l)
pvn_Tswp(long long,ll)
pvn_Tswp(unsigned,u)
pvn_Tswp(unsigned long,ul)
pvn_Tswp(unsigned long long,ull)
pvn_Tswp(size_t,z)
pvn_Tswp(float,f)
pvn_Tswp(double,d)
pvn_Tswp(long double,L)
#ifdef PVN_QUADMATH
pvn_Tswp(__float128,q)
#endif /* PVN_QUADMATH */

#ifndef pvn_Tmin
#define pvn_Tmin(T,t)                              \
static inline T pvn_##t##min(const T a, const T b) \
{                                                  \
  return ((a <= b) ? a : b);                       \
}
#else /* pvn_Tmin */
#error pvn_Tmin already defined
#endif /* ?pvn_Tmin */

pvn_Tmin(int,i)
pvn_Tmin(long,l)
pvn_Tmin(long long,ll)
pvn_Tmin(unsigned,u)
pvn_Tmin(unsigned long,ul)
pvn_Tmin(unsigned long long,ull)
pvn_Tmin(size_t,z)

#ifndef pvn_Tmin3
#define pvn_Tmin3(T,t)                                         \
static inline T pvn_##t##min3(const T a, const T b, const T c) \
{                                                              \
  return pvn_##t##min(pvn_##t##min(a, b), c);                  \
}
#else /* pvn_Tmin3 */
#error pvn_Tmin3 already defined
#endif /* ?pvn_Tmin3 */

pvn_Tmin3(int,i)
pvn_Tmin3(long,l)
pvn_Tmin3(long long,ll)
pvn_Tmin3(unsigned,u)
pvn_Tmin3(unsigned long,ul)
pvn_Tmin3(unsigned long long,ull)
pvn_Tmin3(size_t,z)

#ifndef pvn_Tmin4
#define pvn_Tmin4(T,t)                                                    \
static inline T pvn_##t##min4(const T a, const T b, const T c, const T d) \
{                                                                         \
  return pvn_##t##min(pvn_##t##min(a, b), pvn_##t##min(c, d));            \
}
#else /* pvn_Tmin4 */
#error pvn_Tmin4 already defined
#endif /* ?pvn_Tmin4 */

pvn_Tmin4(int,i)
pvn_Tmin4(long,l)
pvn_Tmin4(long long,ll)
pvn_Tmin4(unsigned,u)
pvn_Tmin4(unsigned long,ul)
pvn_Tmin4(unsigned long long,ull)
pvn_Tmin4(size_t,z)

#ifndef pvn_Tmax
#define pvn_Tmax(T,t)                              \
static inline T pvn_##t##max(const T a, const T b) \
{                                                  \
  return ((a >= b) ? a : b);                       \
}
#else /* pvn_Tmax */
#error pvn_Tmax already defined
#endif /* ?pvn_Tmax */

pvn_Tmax(int,i)
pvn_Tmax(long,l)
pvn_Tmax(long long,ll)
pvn_Tmax(unsigned,u)
pvn_Tmax(unsigned long,ul)
pvn_Tmax(unsigned long long,ull)
pvn_Tmax(size_t,z)

#ifndef pvn_Tmax3
#define pvn_Tmax3(T,t)                                         \
static inline T pvn_##t##max3(const T a, const T b, const T c) \
{                                                              \
  return pvn_##t##max(pvn_##t##max(a, b), c);                  \
}
#else /* pvn_Tmax3 */
#error pvn_Tmax3 already defined
#endif /* ?pvn_Tmax3 */

pvn_Tmax3(int,i)
pvn_Tmax3(long,l)
pvn_Tmax3(long long,ll)
pvn_Tmax3(unsigned,u)
pvn_Tmax3(unsigned long,ul)
pvn_Tmax3(unsigned long long,ull)
pvn_Tmax3(size_t,z)

#ifndef pvn_Tmax4
#define pvn_Tmax4(T,t)                                                    \
static inline T pvn_##t##max4(const T a, const T b, const T c, const T d) \
{                                                                         \
  return pvn_##t##max(pvn_##t##max(a, b), pvn_##t##max(c, d));            \
}
#else /* pvn_Tmax4 */
#error pvn_Tmax4 already defined
#endif /* ?pvn_Tmax4 */

pvn_Tmax4(int,i)
pvn_Tmax4(long,l)
pvn_Tmax4(long long,ll)
pvn_Tmax4(unsigned,u)
pvn_Tmax4(unsigned long,ul)
pvn_Tmax4(unsigned long long,ull)
pvn_Tmax4(size_t,z)

PVN_EXTERN_C size_t pvn_gcd(const size_t a, const size_t b);
PVN_EXTERN_C size_t PVN_FABI(pvn_gcd,PVN_GCD)(const size_t *const a, const size_t *const b);
PVN_EXTERN_C size_t pvn_lcm(const size_t a, const size_t b);
PVN_EXTERN_C size_t PVN_FABI(pvn_lcm,PVN_LCM)(const size_t *const a, const size_t *const b);

PVN_EXTERN_C char *pvn_hexify(char *const s, const void *const x, const size_t z);
PVN_EXTERN_C size_t PVN_FABI(pvn_hexify,PVN_HEXIFY)(char *const s, const void *const x, const size_t *const z, ...);

PVN_EXTERN_C int PVN_FABI(pvn_signbitf,PVN_SIGNBITF)(const float *const x);
PVN_EXTERN_C int PVN_FABI(pvn_signbit,PVN_SIGNBIT)(const double *const x);
PVN_EXTERN_C int PVN_FABI(pvn_signbitl,PVN_SIGNBITL)(const long double *const x);
#ifdef PVN_QUADMATH
PVN_EXTERN_C int PVN_FABI(pvn_signbitq,PVN_SIGNBITQ)(const __float128 *const x);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C int PVN_FABI(pvn_signbitq,PVN_SIGNBITQ)(const long double *const x);
#endif /* ?PVN_QUADMATH */

PVN_EXTERN_C void PVN_FABI(pvn_qsort,PVN_QSORT)(void *const b, const size_t *const n, const size_t *const w, int (*const c)(const void*, const void*));

PVN_EXTERN_C void* PVN_FABI(pvn_pack80,PVN_PACK80)(long double *const a, const size_t *const n);
PVN_EXTERN_C long double* PVN_FABI(pvn_unpack80,PVN_UNPACK80)(void *const a, const size_t *const n);

#ifndef _WIN32
PVN_EXTERN_C void PVN_FABI(pvn_whoami,PVN_WHOAMI)(const char **const w);
#endif /* !_WIN32 */
#endif /* !PVN_AUX_H */

#ifndef PVN_CRM_H
#define PVN_CRM_H

#ifndef PVN_H
#error pvn_crm.h not intended for direct inclusion
#endif /* !PVN_H */

#ifdef PVN_CR_MATH
/* redirect certain math functions to the correctly rounded implementations from core-math */
PVN_EXTERN_C float cr_hypotf(float x, float y);
#define hypotf cr_hypotf
#define cabsf(z) hypotf(crealf(z), cimagf(z))
PVN_EXTERN_C float cr_rsqrtf(float x);
#define rsqrtf cr_rsqrtf
PVN_EXTERN_C void cr_sincosf(float x, float *s, float *c);
#define sincosf cr_sincosf
PVN_EXTERN_C double cr_hypot(double x, double y);
#define hypot cr_hypot
#define cabs(z) hypot(creal(z), cimag(z))
PVN_EXTERN_C double cr_rsqrt(double x);
#define rsqrt cr_rsqrt
PVN_EXTERN_C void cr_sincos(double x, double *s, double *c);
#define sincos cr_sincos
/* cr_hypotl and cr_rsqrtl in core-math assume the 80-bit double-extended arithmetic */
#ifdef __x86_64__
PVN_EXTERN_C long double cr_hypotl(long double x, long double y);
#define hypotl cr_hypotl
#define cabsl(z) hypotl(creall(z), cimagl(z))
PVN_EXTERN_C long double cr_rsqrtl(long double x);
#define rsqrtl cr_rsqrtl
#else /* !__x86_64__ */
#define rsqrtl(x) (1.0L / sqrtl(x))
#endif /* ?__x86_64__ */
#ifdef PVN_QUADMATH
PVN_EXTERN_C __float128 cr_rsqrtq(__float128 x);
#define rsqrtq cr_rsqrtq
PVN_EXTERN_C __float128 cr_sqrtq(__float128 x);
#define sqrtq cr_sqrtq
#else /* !PVN_QUADMATH */
#endif /* ?PVN_QUADMATH */
#else /* !PVN_CR_MATH */
#ifdef __MATHIMF_H_INCLUDED
/* almost correctly rounded Intel-specific functions */
#define rsqrtf invsqrtf
#define rsqrt invsqrt
#define rsqrtl invsqrtl
#ifdef PVN_QUADMATH
#define rsqrtq __invsqrtq
#define sqrtq __sqrtq
#endif /* PVN_QUADMATH */
#else /* !__MATHIMF_H_INCLUDED */
static inline float rsqrtf(float x)
{
  return (1.0f / sqrtf(x));
}
static inline double rsqrt(double x)
{
  return (1.0 / sqrt(x));
}
static inline long double rsqrtl(long double x)
{
  return (1.0L / sqrtl(x));
}
#ifdef PVN_QUADMATH
static inline __float128 rsqrtq(__float128 x)
{
  return (1.0q / sqrtq(x));
}
#endif /* PVN_QUADMATH */
#endif /* ?__MATHIMF_H_INCLUDED */
#endif /* ?PVN_CR_MATH */

#endif /* !PVN_CRM_H */

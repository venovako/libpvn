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
#if (defined(__PPC64__) && defined(__LITTLE_ENDIAN__) && defined(_ARCH_PWR9))
#define cr_hypotl cr_hypotq
#define cr_rsqrtl cr_rsqrtq
/* sqrtl should be correctly rounded */
#else /* !(__PPC64__ && __LITTLE_ENDIAN__ && _ARCH_PWR9) */
PVN_EXTERN_C long double cr_hypotl(long double x, long double y);
PVN_EXTERN_C long double cr_rsqrtl(long double x);
#endif /* ?(__PPC64__ && __LITTLE_ENDIAN__ && _ARCH_PWR9) */
#define hypotl cr_hypotl
#define rsqrtl cr_rsqrtl
#if (defined(PVN_QUADMATH) || (defined(__PPC64__) && defined(__LITTLE_ENDIAN__) && defined(_ARCH_PWR9)))
#define cabsl(z) hypotl(creall(z), cimagl(z))
PVN_EXTERN_C __float128 cr_hypotq(__float128 x, __float128 y);
#define hypotq cr_hypotq
PVN_EXTERN_C __float128 cr_rsqrtq(__float128 x);
#define rsqrtq cr_rsqrtq
PVN_EXTERN_C __float128 cr_sqrtq(__float128 x);
#define sqrtq cr_sqrtq
#endif /* PVN_QUADMATH || (__PPC64__ && __LITTLE_ENDIAN__ && _ARCH_PWR9) */
#else /* !PVN_CR_MATH */
#ifdef __MATHIMF_H_INCLUDED
/* almost correctly rounded Intel-specific functions */
#define rsqrtf invsqrtf
#define rsqrt invsqrt
#define rsqrtl invsqrtl
#ifdef PVN_QUADMATH
#define hypotq __hypotq
#define rsqrtq __invsqrtq
#define sqrtq __sqrtq
#endif /* PVN_QUADMATH */
#else /* !__MATHIMF_H_INCLUDED */
#define rsqrtf(x) pvn_v1s_rsqrt(x)
#define rsqrt(x) pvn_v1d_rsqrt(x)
#define rsqrtl(x) pvn_v1x_rsqrt(x)
#ifdef PVN_QUADMATH
#define rsqrtq(x) pvn_v1q_rsqrt(x)
#endif /* PVN_QUADMATH */
#endif /* ?__MATHIMF_H_INCLUDED */
#endif /* ?PVN_CR_MATH */

#endif /* !PVN_CRM_H */

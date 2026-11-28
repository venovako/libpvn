#ifndef PVN_CRM_H
#define PVN_CRM_H

#ifndef PVN_H
#error pvn_crm.h not intended for direct inclusion
#endif /* !PVN_H */

#ifdef PVN_CR_MATH
/* redirect certain math functions to the correctly rounded implementations from core-math */
/* single */
PVN_EXTERN_C float cr_hypotf(float x, float y);
#define hypotf cr_hypotf
#define cabsf(z) hypotf(crealf(z), cimagf(z))
PVN_EXTERN_C float cr_powf(float x, float y);
#define powf cr_powf
PVN_EXTERN_C float cr_rsqrtf(float x);
#define rsqrtf cr_rsqrtf
PVN_EXTERN_C void cr_sincosf(float x, float *s, float *c);
#define sincosf cr_sincosf
/* double */
PVN_EXTERN_C double cr_hypot(double x, double y);
#define hypot cr_hypot
#define cabs(z) hypot(creal(z), cimag(z))
PVN_EXTERN_C double cr_pow(double x, double y);
#define pow cr_pow
PVN_EXTERN_C double cr_rsqrt(double x);
#define rsqrt cr_rsqrt
PVN_EXTERN_C void cr_sincos(double x, double *s, double *c);
#define sincos cr_sincos
/* extended/quadruple */
#define cr_sqrtl sqrtl
/* cr_hypotl, cr_powl, and cr_rsqrtl in core-math assume the 80-bit double-extended arithmetic */
#if ((PVN_CR_MATH & 8) == 8)
PVN_EXTERN_C long double cr_hypotl(long double x, long double y);
PVN_EXTERN_C long double cr_powl(long double x, long double y);
#define powl cr_powl
PVN_EXTERN_C long double cr_rsqrtl(long double x);
#else /* !extended */
/* TODO: might not be correctly rounded */
#define cr_powl powl
#if ((PVN_CR_MATH & 4) == 4)
#define cr_hypotl cr_hypotq
#define cr_rsqrtl cr_rsqrtq
#else /* !quadruple */
/* TODO: might not be correctly rounded */
#define cr_hypotl pvn_v1x_hypot
/* TODO: might not be correctly rounded */
#define cr_rsqrtl pvn_v1x_rsqrt
#endif /* ?quadruple */
#endif /* ?extended */
#define hypotl cr_hypotl
#define cabsl(z) hypotl(creall(z), cimagl(z))
#define rsqrtl cr_rsqrtl
#if ((PVN_CR_MATH & 4) == 4)
PVN_EXTERN_C _Float128 cr_hypotq(_Float128 x, _Float128 y);
#define hypotq cr_hypotq
PVN_EXTERN_C _Float128 cr_rsqrtq(_Float128 x);
#define rsqrtq cr_rsqrtq
PVN_EXTERN_C _Float128 cr_sqrtq(_Float128 x);
#define sqrtq cr_sqrtq
#endif /* quadruple */
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
#define rsqrtf pvn_v1s_rsqrt
#define rsqrt pvn_v1d_rsqrt
#define rsqrtl pvn_v1x_rsqrt
#ifdef PVN_QUADMATH
#define rsqrtq pvn_v1q_rsqrt
#endif /* PVN_QUADMATH */
#endif /* ?__MATHIMF_H_INCLUDED */
#endif /* ?PVN_CR_MATH */

/* TODO: waiting for the following to be correctly rounded */
PVN_EXTERN_C float rhypotf(float x, float y);
PVN_EXTERN_C double rhypot(double x, double y);
PVN_EXTERN_C long double rhypotl(long double x, long double y);
#ifdef PVN_QUADMATH
PVN_EXTERN_C _Float128 rhypotq(_Float128 x, _Float128 y);
#endif /* PVN_QUADMATH */

#endif /* !PVN_CRM_H */

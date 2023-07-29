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
PVN_EXTERN_C double cr_hypot(double x, double y);
#define hypot cr_hypot
#define cabs(z) hypot(creal(z), cimag(z))
PVN_EXTERN_C double cr_rsqrt(double x);
#define rsqrt cr_rsqrt
/* cr_hypotl is not yet present in core-math, so use the C library's hypotl instead */
#define cabsl(z) hypotl(creall(z), cimagl(z))
/* cr_rsqrtl is not yet present in core-math, so a dummy implementation is provided for completeness */
PVN_EXTERN_C long double cr_rsqrtl(long double x);
#define rsqrtl cr_rsqrtl
#else /* !PVN_CR_MATH */
#ifdef __MATHIMF_H_INCLUDED
/* almost correctly rounded Intel-specific functions */
#define rsqrtf invsqrtf
#define rsqrt invsqrt
#define rsqrtl invsqrtl
#else /* !__MATHIMF_H_INCLUDED */
/* TODO: define rsqrt* functions if needed */
#endif /* ?__MATHIMF_H_INCLUDED */
#endif /* ?PVN_CR_MATH */

#endif /* !PVN_CRM_H */

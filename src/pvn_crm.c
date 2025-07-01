#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  (void)printf("PVN_CR_MATH=%s\n",
#ifdef PVN_CR_MATH
               PVN_CR_MATH
#else /* !PVN_CR_MATH */
               "<undefined>"
#endif /* ?PVN_CR_MATH */
               );
#ifdef PVN_CR_MATH
  /* check the linkage */
  (void)printf("cr_hypotf =%18p\n", cr_hypotf);
  (void)printf("cr_rsqrtf =%18p\n", cr_rsqrtf);
  (void)printf("cr_sincosf=%18p\n", cr_sincosf);
  (void)printf("cr_hypot  =%18p\n", cr_hypot);
  (void)printf("cr_rsqrt  =%18p\n", cr_rsqrt);
  (void)printf("cr_sincos =%18p\n", cr_sincos);
  (void)printf("cr_hypotl =%18p\n", cr_hypotl);
  (void)printf("cr_rsqrtl =%18p\n", cr_rsqrtl);
#if (defined(PVN_QUADMATH) || (defined(__PPC64__) && defined(__LITTLE_ENDIAN__) && defined(_ARCH_PWR9)))
  (void)printf("cr_hypotq =%18p\n", cr_hypotq);
  (void)printf("cr_rsqrtq =%18p\n", cr_rsqrtq);
  (void)printf("cr_sqrtq  =%18p\n", cr_sqrtq);
#endif /* PVN_QUADMATH */
#endif /* PVN_CR_MATH */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#ifdef PVN_CR_MATH
#if !(defined(PVN_QUADMATH) || (defined(__PPC64__) && defined(__LITTLE_ENDIAN__) && defined(_ARCH_PWR9)))
/* This is not correctly rounded, but is at least reproducible. */

long double cr_hypotl(long double x, long double y)
{
  const long double
    X = fabsl(x),
    Y = fabsl(y),
    m = fminl(X, Y),
    M = fmaxl(X, Y),
    q = (m / M),
    Q = fmaxl(q, 0.0L),
    S = fmal(Q, Q, 1.0L),
    s = sqrtl(S),
    h = (M * s);
  return h /* hypotl(x, y) */;
}

long double cr_rsqrtl(long double x)
{
  return (1.0L / sqrtl(x));
}
#endif /* !__x86_64__ */
#endif /* PVN_CR_MATH */
#endif /* ?PVN_TEST */

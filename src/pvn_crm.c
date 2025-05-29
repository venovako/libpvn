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
#ifdef __x86_64__
  (void)printf("cr_hypotl =%18p\n", cr_hypotl);
  (void)printf("cr_rsqrtl =%18p\n", cr_rsqrtl);
#endif /* __x86_64__ */
#ifdef PVN_QUADMATH
  (void)printf("cr_rsqrtq =%18p\n", cr_rsqrtq);
  (void)printf("cr_sqrtq  =%18p\n", cr_sqrtq);
#endif /* PVN_QUADMATH */
#endif /* PVN_CR_MATH */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#endif /* ?PVN_TEST */

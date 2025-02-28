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
  (void)printf("cr_hypotf=%18p\n", cr_hypotf);
  (void)printf("cr_rsqrtf=%18p\n", cr_rsqrtf);
  (void)printf("cr_hypot =%18p\n", cr_hypot);
  (void)printf("cr_rsqrt =%18p\n", cr_rsqrt);
  (void)printf("cr_hypotl=%18p\n", cr_hypotl);
  (void)printf("cr_rsqrtl=%18p\n", cr_rsqrtl);
#endif /* PVN_CR_MATH */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#ifndef __x86_64__
#ifdef hypotl
#undef hypotl
#endif /* hypotl */
/* might not be correctly rounded */
long double cr_hypotl(long double x, long double y)
{
  return hypotl(x, y);
}

long double cr_rsqrtl(long double x)
{
  return (1.0L / sqrtl(x));
}
#endif /* !__x86_64__ */
#endif /* ?PVN_TEST */

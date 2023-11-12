#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
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
  (void)printf("cr_hypotf=%018p\n", cr_hypotf);
  (void)printf("cr_rsqrtf=%018p\n", cr_rsqrtf);
  (void)printf("cr_hypot =%018p\n", cr_hypot);
  (void)printf("cr_rsqrt =%018p\n", cr_rsqrt);
#endif /* PVN_CR_MATH */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#ifdef PVN_CR_MATH
/* not correctly rounded */
long double cr_rsqrtl(long double x)
{
#ifdef __MATHIMF_H_INCLUDED
  return invsqrtl(x);
#else /* !__MATHIMF_H_INCLUDED */
  return (1.0L / sqrtl(x));
#endif /* ?__MATHIMF_H_INCLUDED */
}
#endif /* PVN_CR_MATH */
#endif /* ?PVN_TEST */

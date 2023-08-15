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

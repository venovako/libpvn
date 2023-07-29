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
  return (1.0L / sqrtl(x));
}
#endif /* PVN_CR_MATH */
#endif /* ?PVN_TEST */

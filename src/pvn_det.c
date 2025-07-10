#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc == 5) {
    const double a = atof(argv[1]);
    const double b = atof(argv[2]);
    const double c = atof(argv[3]);
    const double d = atof(argv[4]);
    char s[26] = { '\0' };
    (void)printf("pvn_ddet=%s\n", pvn_dtoa(s, PVN_FABI(pvn_ddet,PVN_DDET)(&a, &b, &c, &d)));
  }
  else {
    (void)fprintf(stderr, "%s a b c d\n", *argv);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
float PVN_FABI(pvn_sdet,PVN_SDET)(const float *const a, const float *const b, const float *const c, const float *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(isfinite(*a));
  PVN_ASSERT(isfinite(*b));
  PVN_ASSERT(isfinite(*c));
  PVN_ASSERT(isfinite(*d));
  return pvn_sdet(*a, *b, *c, *d);
}

double PVN_FABI(pvn_ddet,PVN_DDET)(const double *const a, const double *const b, const double *const c, const double *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(isfinite(*a));
  PVN_ASSERT(isfinite(*b));
  PVN_ASSERT(isfinite(*c));
  PVN_ASSERT(isfinite(*d));
  return pvn_ddet(*a, *b, *c, *d);
}

long double PVN_FABI(pvn_xdet,PVN_XDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(isfinite(*a));
  PVN_ASSERT(isfinite(*b));
  PVN_ASSERT(isfinite(*c));
  PVN_ASSERT(isfinite(*d));
  return pvn_xdet(*a, *b, *c, *d);
}
#ifdef PVN_QUADMATH
__float128 PVN_FABI(pvn_qdet,PVN_QDET)(const __float128 *const a, const __float128 *const b, const __float128 *const c, const __float128 *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  PVN_ASSERT(isfiniteq(*a));
  PVN_ASSERT(isfiniteq(*b));
  PVN_ASSERT(isfiniteq(*c));
  PVN_ASSERT(isfiniteq(*d));
  return pvn_qdet(*a, *b, *c, *d);
}
#else /* !PVN_QUADMATH */
long double PVN_FABI(pvn_qdet,PVN_QDET)(const long double *const a, const long double *const b, const long double *const c, const long double *const d)
{
  return PVN_FABI(pvn_xdet,PVN_XDET)(a, b, c, d);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

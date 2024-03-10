#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  const int u = pvn_ran_open_();
  (void)printf("open=%d\n", pvn_ran_open_());
  char s[46] = { '\0' };
  (void)printf("safe_f=%s\n", pvn_stoa(s, pvn_ran_safe_f_(&u)));
  (void)printf("safe_d=%s\n", pvn_dtoa(s, pvn_ran_safe_(&u)));
  (void)printf("safe_l=%s\n", pvn_xtoa(s, pvn_ran_safe_l_(&u)));
  (void)printf("safe_q=%s\n", pvn_qtoa(s, pvn_ran_safe_q_(&u)));
  (void)printf("close=%d\n", pvn_ran_close_(&u));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_ran_open_()
{
  return open("/dev/random", (O_RDONLY | PVN_LF64));
}

int pvn_ran_close_(const int *const u)
{
  assert(u);
  return close(*u);
}

float pvn_ran_safe_f_(const int *const u)
{
  assert(u);
  const float rmin = FLT_MIN;
  const float rmax = (FLT_MAX * 0.25f);
  float a = FLT_MAX, r = 0.0f;
  while (!(a >= rmin) || !(a <= rmax)) {
    ssize_t s = 0;
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
    {
      s = read(*u, &r, sizeof(r));
    }
    if (s == (ssize_t)sizeof(r))
      a = fabsf(r);
    else if (s < 0)
      return -0.0f;
    else /* s >= 0 */
      return 0.0f;
  }
  return r;
}

double pvn_ran_safe_(const int *const u)
{
  assert(u);
  const double rmin = DBL_MIN;
  const double rmax = (DBL_MAX * 0.25);
  double a = DBL_MAX, r = 0.0;
  while (!(a >= rmin) || !(a <= rmax)) {
    ssize_t s = 0;
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
    {
      s = read(*u, &r, sizeof(r));
    }
    if (s == (ssize_t)sizeof(r))
      a = fabs(r);
    else if (s < 0)
      return -0.0;
    else /* s >= 0 */
      return 0.0;
  }
  return r;
}

long double pvn_ran_safe_l_(const int *const u)
{
  assert(u);
  const long double rmin = LDBL_MIN;
  const long double rmax = (LDBL_MAX * 0.25L);
  long double a = LDBL_MAX, r = 0.0L;
  while (!(a >= rmin) || !(a <= rmax)) {
    ssize_t s = 0;
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
    {
      s = read(*u, &r, sizeof(r));
    }
    if (s == (ssize_t)sizeof(r))
      a = fabsl(r);
    else if (s < 0)
      return -0.0L;
    else /* s >= 0 */
      return 0.0L;
  }
  return r;
}

#ifdef PVN_QUADMATH
__float128 pvn_ran_safe_q_(const int *const u)
{
  assert(u);
  const __float128 rmin = FLT128_MIN;
  const __float128 rmax = (FLT128_MAX * 0.25q);
  __float128 a = FLT128_MAX, r = 0.0q;
  while (!(a >= rmin) || !(a <= rmax)) {
    ssize_t s = 0;
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
    {
      s = read(*u, &r, sizeof(r));
    }
    if (s == (ssize_t)sizeof(r))
      a = fabsq(r);
    else if (s < 0)
      return -0.0q;
    else /* s >= 0 */
      return 0.0q;
  }
  return r;
}
#else /* !PVN_QUADMATH */
long double pvn_ran_safe_q_(const int *const u)
{
  return pvn_ran_safe_l_(u);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

#include "pvn.h"

#ifdef __RDRND__
#include <immintrin.h>
#endif /* __RDRND__ */

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  const int u = pvn_ran_open_();
  (void)printf("open=%d\n", pvn_ran_open_());
  char s[46] = { '\0' };
  (void)printf("safe_f=%s\n", pvn_stoa(s, pvn_ran_safe_f_(&u, (const int*)NULL)));
  (void)printf("safe_d=%s\n", pvn_dtoa(s, pvn_ran_safe_(&u, (const int*)NULL)));
  (void)printf("safe_l=%s\n", pvn_xtoa(s, pvn_ran_safe_l_(&u, (const int*)NULL)));
  (void)printf("safe_q=%s\n", pvn_qtoa(s, pvn_ran_safe_q_(&u, (const int*)NULL)));
  (void)printf("close=%d\n", pvn_ran_close_(&u));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_ran_open_()
{
#ifdef __RDRND__
  return __RDRND__;
#else /* !__RDRND__ */
  return open("/dev/random", (O_RDONLY | PVN_LF64));
#endif /* ?__RDRND__ */
}

int pvn_ran_close_(const int *const u)
{
  PVN_ASSERT(u);
#ifdef __RDRND__
  return ((*u < 0) ? -1 : 0);
#else /* !__RDRND__ */
  return close(*u);
#endif /* ?__RDRND__ */
}

float pvn_ran_safe_f_(const int *const u, const int *const p)
{
  PVN_ASSERT(u);
  const float rmin = (p ? scalbnf(FLT_MIN, *p) : FLT_MIN);
  const float rmax = (FLT_MAX * 0.25f);
  float a = FLT_MAX, r = 0.0f;
  if (*u < 0)
    return r;
  while (!(a >= rmin) || !(a <= rmax)) {
#ifdef __RDRND__
    while (!_rdrand32_step((unsigned*)&r)) /**/;
    a = fabsf(r);
#else /* !__RDRND__ */
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
#endif /* ?__RDRND__ */
  }
  return r;
}

float pvn_ran_f_(const int *const u)
{
#ifdef PVN_RAN_SAFE
  return pvn_ran_safe_f_(u, (const int*)NULL);
#else /* !PVN_RAN_SAFE */
  PVN_ASSERT(u);
  float r = ((*u < 0) ? NAN : 0.0f);
#ifdef __RDRND__
  while (!_rdrand32_step((unsigned*)&r)) /**/;
#else /* !__RDRND__ */
  ssize_t s = 0;
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
  {
    s = read(*u, &r, sizeof(r));
  }
#endif /* ?__RDRND__ */
  return r;
#endif /* ?PVN_RAN_SAFE */
}

double pvn_ran_safe_(const int *const u, const int *const p)
{
  PVN_ASSERT(u);
  const double rmin = (p ? scalbn(DBL_MIN, *p) : DBL_MIN);
  const double rmax = (DBL_MAX * 0.25);
  double a = DBL_MAX, r = 0.0;
  if (*u < 0)
    return r;
  while (!(a >= rmin) || !(a <= rmax)) {
#ifdef __RDRND__
    while (!_rdrand64_step((unsigned long long*)&r)) /**/;
    a = fabs(r);
#else /* !__RDRND__ */
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
#endif /* ?__RDRND__ */
  }
  return r;
}

double pvn_ran_(const int *const u)
{
#ifdef PVN_RAN_SAFE
  return pvn_ran_safe_(u, (const int*)NULL);
#else /* !PVN_RAN_SAFE */
  PVN_ASSERT(u);
  double r = ((*u < 0) ? (double)NAN : 0.0);
#ifdef __RDRND__
  while (!_rdrand64_step((unsigned long long*)&r)) /**/;
#else /* !__RDRND__ */
  ssize_t s = 0;
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
  {
    s = read(*u, &r, sizeof(r));
  }
#endif /* ?__RDRND__ */
  return r;
#endif /* ?PVN_RAN_SAFE */
}

long double pvn_ran_safe_l_(const int *const u, const int *const p)
{
  PVN_ASSERT(u);
  const long double rmin = (p ? scalbnl(LDBL_MIN, *p) : LDBL_MIN);
  const long double rmax = (LDBL_MAX * 0.25L);
  long double a = LDBL_MAX, r = 0.0L;
  if (*u < 0)
    return r;
  while (!(a >= rmin) || !(a <= rmax)) {
#ifdef __RDRND__
    while (!_rdrand64_step((unsigned long long*)&r)) /**/;
    while (!_rdrand16_step((unsigned short*)((unsigned long long*)&r + 1))) /**/;
    a = fabsl(r);
#else /* !__RDRND__ */
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
#endif /* ?__RDRND__ */
  }
  return r;
}

long double pvn_ran_l_(const int *const u)
{
#ifdef PVN_RAN_SAFE
  return pvn_ran_safe_l_(u, (const int*)NULL);
#else /* !PVN_RAN_SAFE */
  PVN_ASSERT(u);
  long double r = ((*u < 0) ? (long double)NAN : 0.0L);
#ifdef __RDRND__
  while (!_rdrand64_step((unsigned long long*)&r)) /**/;
  while (!_rdrand16_step((unsigned short*)((unsigned long long*)&r + 1))) /**/;
#else /* !__RDRND__ */
  ssize_t s = 0;
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
  {
    s = read(*u, &r, sizeof(r));
  }
#endif /* ?__RDRND__ */
  return r;
#endif /* ?PVN_RAN_SAFE */
}

#ifdef PVN_QUADMATH
__float128 pvn_ran_safe_q_(const int *const u, const int *const p)
{
  PVN_ASSERT(u);
  const __float128 rmin = (p ? scalbnq(FLT128_MIN, *p) : FLT128_MIN);
  const __float128 rmax = (FLT128_MAX * 0.25q);
  __float128 a = FLT128_MAX, r = 0.0q;
  if (*u < 0)
    return r;
  while (!(a >= rmin) || !(a <= rmax)) {
#ifdef __RDRND__
    while (!_rdrand64_step((unsigned long long*)&r)) /**/;
    while (!_rdrand64_step((unsigned long long*)&r + 1)) /**/;
    a = fabsq(r);
#else /* !__RDRND__ */
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
#endif /* ?__RDRND__ */
  }
  return r;
}

__float128 pvn_ran_q_(const int *const u)
{
#ifdef PVN_RAN_SAFE
  return pvn_ran_safe_q_(u, (const int*)NULL);
#else /* !PVN_RAN_SAFE */
  PVN_ASSERT(u);
  __float128 r = ((*u < 0) ? (__float128)NAN : 0.0q);
#ifdef __RDRND__
  while (!_rdrand64_step((unsigned long long*)&r)) /**/;
  while (!_rdrand64_step((unsigned long long*)&r + 1)) /**/;
#else /* !__RDRND__ */
  ssize_t s = 0;
#ifdef _OPENMP
#pragma omp critical
#endif /* _OPENMP */
  {
    s = read(*u, &r, sizeof(r));
  }
#endif /* ?__RDRND__ */
  return r;
#endif /* ?PVN_RAN_SAFE */
}
#else /* !PVN_QUADMATH */
long double pvn_ran_safe_q_(const int *const u, const int *const p)
{
  return pvn_ran_safe_l_(u, p);
}

long double pvn_ran_q_(const int *const u)
{
  return pvn_ran_l_(u);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc != 2) {
    fprintf(stderr, "%s size_t\n", argv[0]);
    return EXIT_FAILURE;
  }
  (void)printf(" size_t      = %zu\n", pvn_atoz(argv[1]));

#ifdef _WIN32
  char s[31u];
#else /* !_WIN32 */
  char s[46u];
#endif /* ?_WIN32 */
  (void)printf(" FLT_TRUE_MIN=%s\n", pvn_stoa(s, FLT_TRUE_MIN));
  (void)printf(" FLT_MIN     =%s\n", pvn_stoa(s, FLT_MIN));
  (void)printf(" FLT_EPSILON =%s\n", pvn_stoa(s, FLT_EPSILON));
  (void)printf(" FLT_MAX     =%s\n", pvn_stoa(s, FLT_MAX));
  (void)printf(" DBL_TRUE_MIN=%s\n", pvn_dtoa(s, DBL_TRUE_MIN));
  (void)printf(" DBL_MIN     =%s\n", pvn_dtoa(s, DBL_MIN));
  (void)printf(" DBL_EPSILON =%s\n", pvn_dtoa(s, DBL_EPSILON));
  (void)printf(" DBL_MAX     =%s\n", pvn_dtoa(s, DBL_MAX));
#ifndef _WIN32
  (void)printf("LDBL_TRUE_MIN=%s\n", pvn_xtoa(s, LDBL_TRUE_MIN));
  (void)printf("LDBL_MIN     =%s\n", pvn_xtoa(s, LDBL_MIN));
  (void)printf("LDBL_EPSILON =%s\n", pvn_xtoa(s, LDBL_EPSILON));
  (void)printf("LDBL_MAX     =%s\n", pvn_xtoa(s, LDBL_MAX));
#endif /* !_WIN32 */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
size_t pvn_atoz(const char *const s)
{
  char *e = (char*)NULL;
  const size_t z = ((s && *s) ? (size_t)strtoull(s, &e, 0) : (size_t)0u);
  return ((e && *e) ? (size_t)0u : z);
}

char *pvn_stoa(char *const s, const float x)
{
  if (s) {
    int l = sprintf((char*)memset(s, 0, (size_t)17u), "%# -16.9E", (double)x);
    if (l <= 0)
      return (char*)NULL;
    char *d = s + 16;
    char *e = strrchr(s, 'E');
    if (e) {
      for (--d; isblank(*d); --d)
        *d = '\0';
      e += 2;
      l = (int)(strchr(e, '\0') - e);
      if (l >= 2)
        return s;
      d = s + 16;
      e += l;
      for (int i = 0; i < l; ++i)
        *--d = *--e;
      for (--d; isdigit(*d); --d)
        *d = '0';
    }
    else
      for (--d; !*d; --d)
        *d = ' ';
  }
  return s;
}

char *pvn_dtoa(char *const s, const double x)
{
  if (s) {
    int l = sprintf((char*)memset(s, 0, (size_t)26u), "%# -25.17E", x);
    if (l <= 0)
      return (char*)NULL;
    char *d = s + 25;
    char *e = strrchr(s, 'E');
    if (e) {
      for (--d; isblank(*d); --d)
        *d = '\0';
      e += 2;
      l = (int)(strchr(e, '\0') - e);
      if (l >= 3)
        return s;
      d = s + 25;
      e += l;
      for (int i = 0; i < l; ++i)
        *--d = *--e;
      for (--d; isdigit(*d); --d)
        *d = '0';
    }
    else
      for (--d; !*d; --d)
        *d = ' ';
  }
  return s;
}

#ifndef _WIN32
char *pvn_xtoa(char *const s, const long double x)
{
  if (s) {
#ifdef __x86_64__
    int l = sprintf((char*)memset(s, 0, (size_t)31u), "%# -30.21LE", x);
    if (l <= 0)
      return (char*)NULL;
    char *d = s + 30;
    char *e = strrchr(s, 'E');
    if (e) {
      for (--d; isblank(*d); --d)
        *d = '\0';
      e += 2;
      l = (int)(strchr(e, '\0') - e);
      if (l >= 4)
        return s;
      d = s + 30;
      e += l;
      for (int i = 0; i < l; ++i)
        *--d = *--e;
      for (--d; isdigit(*d); --d)
        *d = '0';
    }
#else /* !__x86_64__ */
    int l = sprintf((char*)memset(s, 0, (size_t)46u), "%# -45.36LE", x);
    if (l <= 0)
      return (char*)NULL;
    char *d = s + 45;
    char *e = strrchr(s, 'E');
    if (e) {
      for (--d; isblank(*d); --d)
        *d = '\0';
      e += 2;
      l = (int)(strchr(e, '\0') - e);
      if (l >= 4)
        return s;
      d = s + 45;
      e += l;
      for (int i = 0; i < l; ++i)
        *--d = *--e;
      for (--d; isdigit(*d); --d)
        *d = '0';
    }
#endif /* ?__x86_64__ */
    else
      for (--d; !*d; --d)
        *d = ' ';
  }
  return s;
}

#ifdef PVN_QUADMATH
char *pvn_qtoa(char *const s, const __float128 x)
{
#ifdef __MATHIMF_H_INCLUDED
  /* TODO: how to print out __float128 with Intel compilers? */
  return pvn_xtoa(s, (long double)x);
#else /* !__MATHIMF_H_INCLUDED */
  if (s) {
    int l = quadmath_snprintf((char*)memset(s, 0, (size_t)46u), (size_t)46u, "%# -45.36QE", x);
    if (l <= 0)
      return (char*)NULL;
    char *d = s + 45;
    char *e = strrchr(s, 'E');
    if (e) {
      for (--d; isblank(*d); --d)
        *d = '\0';
      e += 2;
      l = (int)(strchr(e, '\0') - e);
      if (l >= 4)
        return s;
      d = s + 45;
      e += l;
      for (int i = 0; i < l; ++i)
        *--d = *--e;
      for (--d; isdigit(*d); --d)
        *d = '0';
    }
    else
      for (--d; !*d; --d)
        *d = ' ';
  }
  return s;
#endif /* ?__MATHIMF_H_INCLUDED */
}
#else /* !PVN_QUADMATH */
char *pvn_qtoa(char *const s, const long double x)
{
  return pvn_xtoa(s, x);
}
#endif /* ?PVN_QUADMATH */
#endif /* !_WIN32 */
#endif /* ?PVN_TEST */

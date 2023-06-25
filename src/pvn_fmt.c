#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc != 3) {
    fprintf(stderr, "%s size_t long_double\n", argv[0]);
    return EXIT_FAILURE;
  }

  (void)printf("size_t=%zu\n", pvn_atoz(argv[1]));

  char *e = (char*)NULL;
  const long double ld = strtold(argv[2], &e);
  if (!e || *e)
    return EXIT_FAILURE;

  char s[46u];
  (void)printf("long double=%s\n", pvn_xtoa(s, ld));
  (void)printf("double=%s\n", pvn_dtoa(s, (double)ld));
  (void)printf("float=%s\n", pvn_stoa(s, (float)ld));

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
#endif /* ?PVN_TEST */

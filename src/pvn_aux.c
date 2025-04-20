#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (4 != argc) {
#ifdef _WIN32
    (void)fprintf(stderr, "%s a b d\n", *argv);
#else /* !_WIN32 */
    (void)fprintf(stderr, "%s a b ld\n", *argv);
#endif /* ?_WIN32 */
    return EXIT_FAILURE;
  }
  const size_t a = pvn_atoz(argv[1]);
  const size_t b = pvn_atoz(argv[2]);
  (void)printf("gcd(%zu, %zu) = %zu\n", a, b, pvn_gcd(a, b));
  (void)printf("lcm(%zu, %zu) = %zu\n", a, b, pvn_lcm(a, b));
#ifdef _WIN32
  char s[17] = { '\0' };
  char *e = (char*)NULL;
  double d = 0.0;
  *(double*)memset(&d, 0, sizeof(d)) = strtod(argv[3], &e);
  if (e && *e)
    return EXIT_FAILURE;
  (void)printf("hexify(%s) = 0x%s\n", argv[3], pvn_hexify(s, &d, sizeof(d)));
#else /* !_WIN32 */
  char s[33] = { '\0' };
  char *e = (char*)NULL;
  long double ld = 0.0L;
  *(long double*)memset(&ld, 0, sizeof(ld)) = strtold(argv[3], &e);
  if (e && *e)
    return EXIT_FAILURE;
  (void)printf("hexify(%s) = 0x%s\n", argv[3], pvn_hexify(s, &ld, sizeof(ld)));
#endif /* ?_WIN32 */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
size_t pvn_gcd(const size_t a, const size_t b)
{
  size_t aa, bb;
  if (a < b) {
    aa = b;
    bb = a;
  }
  else {
    aa = a;
    bb = b;
  }
  while (bb) {
    aa = aa - bb;
    if (aa < bb) {
      const size_t cc = aa;
      aa = bb;
      bb = cc;
    }
  }
  return aa;
}

size_t PVN_FABI(pvn_gcd,PVN_GCD)(const size_t *const a, const size_t *const b)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  return pvn_gcd(*a, *b);
}

size_t pvn_lcm(const size_t a, const size_t b)
{
  const size_t g = pvn_gcd(a, b);
  return (g ? ((a / g) * b) : (size_t)0u);
}

size_t PVN_FABI(pvn_lcm,PVN_LCM)(const size_t *const a, const size_t *const b)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  return pvn_lcm(*a, *b);
}

char *pvn_hexify(char *const s, const void *const x, const size_t z)
{
  if (!s)
    return s;
  *s = '\0';
  if (!x || !z)
    return s;
  const unsigned char *const b = (const unsigned char*)x;
  const unsigned char *e = b + z;
  char *o = s;
  do {
    const unsigned char c = *--e;
    if (2 != sprintf(o, "%02hhX", c)) {
      *o = '\0';
      break;
    }
    o += 2;
  } while (e != b);
  return s;
}

size_t PVN_FABI(pvn_hexify,PVN_HEXIFY)(char *const s, const void *const x, const size_t *const z, ...)
{
  PVN_ASSERT(s);
  PVN_ASSERT(x);
  PVN_ASSERT(z);
  return strlen(pvn_hexify(s, x, *z));
}

int PVN_FABI(pvn_signbitf,PVN_SIGNBITF)(const float *const x)
{
  PVN_ASSERT(x);
  return ((*(const unsigned*)x & 0x80000000u) != 0u);
}

int PVN_FABI(pvn_signbit,PVN_SIGNBIT)(const double *const x)
{
  PVN_ASSERT(x);
  return ((((const unsigned*)x)[1u] & 0x80000000u) != 0u);
}

int PVN_FABI(pvn_signbitl,PVN_SIGNBITL)(const long double *const x)
{
  PVN_ASSERT(x);
#ifdef __x86_64__
  return ((((const unsigned*)x)[2u] & 0x00008000u) != 0u);
#else /* !__x86_64__ */
  return ((((const unsigned*)x)[3u] & 0x80000000u) != 0u);
#endif /* ?__x86_64__ */
}

#ifdef PVN_QUADMATH
int PVN_FABI(pvn_signbitq,PVN_SIGNBITQ)(const __float128 *const x)
{
  PVN_ASSERT(x);
  return ((((const unsigned*)x)[3u] & 0x80000000u) != 0u);
}
#else /* !PVN_QUADMATH */
int PVN_FABI(pvn_signbitq,PVN_SIGNBITQ)(const long double *const x)
{
  return PVN_FABI(pvn_signbitl,PVN_SIGNBITL)(x);
}
#endif /* ?PVN_QUADMATH */

void PVN_FABI(pvn_qsort,PVN_QSORT)(void *const b, const size_t *const n, const size_t *const w, int (*const c)(const void*, const void*))
{
  PVN_ASSERT(b);
  PVN_ASSERT(n);
  PVN_ASSERT(w);
  PVN_ASSERT(c);
#if (defined(_OPENMP) && defined(__APPLE__))
  /* if threading is desired and psort(3) is available */
  psort(b, *n, *w, c);
#else /* !_OPENMP || !__APPLE__ */
  qsort(b, *n, *w, c);
#endif /* ?_OPENMP && ?__APPLE__ */
}
#endif /* ?PVN_TEST */

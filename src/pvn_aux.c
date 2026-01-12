#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (4 != argc) {
    (void)fprintf(stderr, "%s a b x\n", *argv);
    return EXIT_FAILURE;
  }
  const size_t a = pvn_atoz(argv[1]);
  const size_t b = pvn_atoz(argv[2]);
  (void)printf("gcd(%zu, %zu) = %zu\n", a, b, pvn_gcd(a, b));
  (void)printf("lcm(%zu, %zu) = %zu\n", a, b, pvn_lcm(a, b));
  size_t n = (size_t)0u, *p = (size_t*)NULL, *m = (size_t*)NULL;
  (void)printf("%zu =", a);
  if ((n = pvn_factorize(a, &p, &m)))
    for (size_t i = (size_t)0u; i < n; ++i)
      (void)printf(" %zu^%zu", p[i], m[i]);
  else
    (void)printf(" %zu^1", a);
  (void)printf("\n");
  (void)printf("%zu =", b);
  if ((n = pvn_factorize(b, &p, &m)))
    for (size_t i = (size_t)0u; i < n; ++i)
      (void)printf(" %zu^%zu", p[i], m[i]);
  else
    (void)printf(" %zu^1", b);
  (void)printf("\n");
  free(m);
  free(p);
#if (!defined(_WIN32) || defined(_DLL))
  char s[33] = { '\0' };
  long double x = 0.0L;
  *(long double*)memset(&x, 0, sizeof(x)) = pvn_atox(argv[3]);
#else /* _WIN32 && !_DLL */
  char s[17] = { '\0' };
  char *e = (char*)NULL;
  const double x = strtod(argv[3], &e);
  if (e && *e)
    return EXIT_FAILURE;
#endif /* ?(!_WIN32 || _DLL) */
  (void)printf("hexify(%s) = 0x%s\n", argv[3], pvn_hexify(s, &x, sizeof(x)));
#ifdef _WIN32
  char *line = (char*)NULL;
  size_t linecap = (size_t)0u;
  (void)printf("getline = %lld\n", getline(&line, &linecap, stdin));
  (void)printf("line back (with size %zu): %s", linecap, line);
  free(line);
#else /* !_WIN32 */
  const char *w = (const char*)NULL;
  (void)printf("pvn_whoami = %s\n", ((PVN_FABI(pvn_whoami,PVN_WHOAMI)(&w) > 0) ? w : "?"));
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
  return ((a && b) ? pvn_gcd(*a, *b) : (size_t)0u);
}

size_t pvn_lcm(const size_t a, const size_t b)
{
  const size_t g = pvn_gcd(a, b);
  return (g ? ((a / g) * b) : (size_t)0u);
}

size_t PVN_FABI(pvn_lcm,PVN_LCM)(const size_t *const a, const size_t *const b)
{
  return ((a && b) ? pvn_lcm(*a, *b) : (size_t)0u);
}

static size_t rec_factorize(size_t x, size_t f, size_t l, size_t **const p, size_t **const m)
{
  const size_t y = (size_t)__builtin_sqrtl((long double)x);
  while ((f <= y) && (x % f))
    ++f;
  if (f > y)
    f = x;
  size_t n = (size_t)0u;
  do {
    x /= f;
    ++n;
  } while (!(x % f));
  size_t r = (size_t)0u;
  if (x < f) {
    r = l * sizeof(size_t);
    if (p && !(*p = (size_t*)realloc(*p, r)))
      return (size_t)0u;
    if (m && !(*m = (size_t*)realloc(*m, r)))
      return (size_t)0u;
    r = l;
  }
  else
    r = rec_factorize(x, (f + 1u), (l + 1u), p, m);
  if (r) {
    --l;
    if (p)
      (*p)[l] = f;
    if (m)
      (*m)[l] = n;
  }
  return r;
}

size_t pvn_factorize(const size_t x, size_t **const p, size_t **const m)
{
  return ((x <= (size_t)1u) ? (size_t)0u : rec_factorize(x, 2u, 1u, p, m));
}

size_t PVN_FABI(pvn_factorize,PVN_FACTORIZE)(const size_t *const x, size_t **const p, size_t **const m)
{
  return (x ? pvn_factorize(*x, p, m) : (size_t)0u);
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
#if (defined(__APPLE__) && defined(PVN_OPENMP) && (PVN_OPENMP > 1))
  /* if the nested threading is desired and psort(3) is available */
  psort(b, *n, *w, c);
#else /* !__APPLE__ || !PVN_OPENMP */
  qsort(b, *n, *w, c);
#endif /* ?__APPLE__ && ?PVN_OPENMP */
}

void* PVN_FABI(pvn_pack80,PVN_PACK80)(long double *const a, const size_t *const n)
{
  PVN_ASSERT(!((uintptr_t)a & 1u));
  if (!n)
    return NULL;
  if (!a || !*n)
    return a;
  uint16_t *const x = (uint16_t*)a;
  for (size_t i = (size_t)1u; i < *n; ++i) {
    const uint16_t *const s = x + i * 8u;
    uint16_t *const d = x + i * 5u;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    d[4] = s[4];
  }
  return a;
}

long double* PVN_FABI(pvn_unpack80,PVN_UNPACK80)(void *const a, const size_t *const n)
{
  PVN_ASSERT(!((uintptr_t)a & 1u));
  if (!n)
    return (long double*)NULL;
  if (!a || !*n)
    return (long double*)a;
  uint16_t *const x = (uint16_t*)a;
  for (size_t i = *n - (size_t)1u; i >= (size_t)1u; --i) {
    const uint16_t *const s = x + i * 5u;
    uint16_t *const d = x + i * 8u;
    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    d[4] = s[4];
    d[5] = UINT16_C(0);
    d[6] = UINT16_C(0);
    d[7] = UINT16_C(0);
  }
  x[5] = UINT16_C(0);
  x[6] = UINT16_C(0);
  x[7] = UINT16_C(0);
  return (long double*)a;
}

#ifndef _WIN32
int PVN_FABI(pvn_whoami,PVN_WHOAMI)(const char **const w)
{
  PVN_ASSERT(w);
  Dl_info info;
  *w = (dladdr(__builtin_extract_return_addr(__builtin_return_address(0u)), &info) ? info.dli_sname : (const char*)NULL);
  return (*w ? (int)strlen(*w) : -1);
}
#endif /* !_WIN32 */
#endif /* ?PVN_TEST */

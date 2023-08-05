#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (4 != argc) {
    (void)fprintf(stderr, "%s a b ld\n", *argv);
    return EXIT_FAILURE;
  }
  const size_t a = pvn_atoz(argv[1]);
  const size_t b = pvn_atoz(argv[2]);
  (void)printf("gcd(%zu, %zu) = %zu\n", a, b, pvn_gcd(a, b));
  (void)printf("lcm(%zu, %zu) = %zu\n", a, b, pvn_lcm(a, b));
  char s[33] = { '\0' };
  char *e = (char*)NULL;
  const long double ld = strtold(argv[3], &e);
  if (e && *e)
    return EXIT_FAILURE;
  (void)printf("hexify(%s) = 0x%s\n", argv[3], pvn_hexify(s, &ld, sizeof(ld)));
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

size_t pvn_gcd_(const size_t *const a, const size_t *const b)
{
  assert(a);
  assert(b);
  return pvn_gcd(*a, *b);
}

size_t pvn_lcm(const size_t a, const size_t b)
{
  const size_t g = pvn_gcd(a, b);
  return (g ? ((a / g) * b) : (size_t)0u);
}

size_t pvn_lcm_(const size_t *const a, const size_t *const b)
{
  assert(a);
  assert(b);
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
#endif /* ?PVN_TEST */

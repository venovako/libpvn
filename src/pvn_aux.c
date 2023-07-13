#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (3 != argc) {
    (void)fprintf(stderr, "%s a b\n", *argv);
    return EXIT_FAILURE;
  }
  const size_t a = pvn_atoz(argv[1]);
  const size_t b = pvn_atoz(argv[2]);
  (void)printf("gcd(%zu, %zu) = %zu\n", a, b, pvn_gcd(a, b));
  (void)printf("lcm(%zu, %zu) = %zu\n", a, b, pvn_lcm(a, b));
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
#endif /* ?PVN_TEST */

#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (3 != argc) {
    (void)fprintf(stderr, "%s a b\n", *argv);
    return EXIT_FAILURE;
  }
  const int a = atoi(argv[1]);
  const int b = atoi(argv[2]);
  (void)printf("gcd(%d, %d) = %d\n", a, b, pvn_gcd(a, b));
  (void)printf("lcm(%d, %d) = %d\n", a, b, pvn_lcm(a, b));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_gcd(const int a, const int b)
{
  int aa = abs(a);
  int bb = abs(b);
  if (aa < bb) {
    const int cc = aa;
    aa = bb;
    bb = cc;
  }
  while (bb) {
    aa = aa - bb;
    if (aa < bb) {
      const int cc = aa;
      aa = bb;
      bb = cc;
    }
  }
  return aa;
}

int pvn_gcd_(const int *const a, const int *const b)
{
  assert(a);
  assert(b);
  return pvn_gcd(*a, *b);
}

int pvn_lcm(const int a, const int b)
{
  const int g = pvn_gcd(a, b);
  return (g ? ((abs(a) / g) * abs(b)) : 0);
}

int pvn_lcm_(const int *const a, const int *const b)
{
  assert(a);
  assert(b);
  return pvn_lcm(*a, *b);
}
#endif /* ?PVN_TEST */

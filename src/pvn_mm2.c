#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  const double
    a11r = -1.0, a11i = 1.0, a21r = 1.0, a21i = -1.0, a12r = 2.0, a12i = 2.0, a22r = -2.0, a22i = - 2.0;
  double
    c11r = 0.0, c11i = 0.0, c21r = 0.0, c21i = 0.0, c12r = 0.0, c12i = 0.0, c22r = 0.0, c22i = 0.0;
  pvn_zmm2(&c11r, &c11i, &c21r, &c21i, &c12r, &c12i, &c22r, &c22i, a11r, a11i, a21r, a21i, a12r, a12i, a22r, a22i, a11r, a11i, a21r, a21i, a12r, a12i, a22r, a22i);
  (void)printf("(% 3.0F,% 3.0F) (% 3.0F,% 3.0F)\n", c11r, c11i, c12r, c12i);
  (void)printf("(% 3.0F,% 3.0F) (% 3.0F,% 3.0F)\n", c21r, c21i, c22r, c22i);
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#endif /* ?PVN_TEST */
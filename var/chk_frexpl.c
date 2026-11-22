/* icx on Windows */
#include <mathimf.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int e = 0;
  const long double m = __builtin_frexpl(1.0L, &e);
  (void)printf("m=%lF\n", (double)m);
  (void)printf("e=%d\n", e);
  return EXIT_SUCCESS;
}

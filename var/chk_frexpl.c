/* icx.exe /nologo /Qstd:gnu2y /Qlong-double /Qoption,cpp,--extended_float_types /EHsc /O3 /QxHost /Ob2 /fp:precise /Qfma /Qftz- /Qopenmp-simd /Qvec-threshold:0 /traceback /MT chk_frexpl.c /link /RELEASE */
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

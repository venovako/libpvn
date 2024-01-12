#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  (void)printf("libpvn built on %s with %s for %s on %s ", __DATE__, PVN_COMPILER, PVN_OS, PVN_ARCH);
#ifdef NDEBUG
  (void)printf("with optimization level %d ", NDEBUG);
#else /* !NDEBUG */
  (void)printf("for debugging ");
#endif /* ?NDEBUG */
  (void)printf("and with OpenMP ");
#ifdef _OPENMP
  (void)printf("%d\n", _OPENMP);
#else /* !_OPENMP */
  (void)printf("disabled\n");
#endif /* ?_OPENMP */
#ifdef PVN_CR_MATH
  (void)printf("CR_MATH : %s\n", PVN_CR_MATH);
#endif /* PVN_CR_MATH */
#ifdef PVN_QUADMATH
  (void)printf("QUADMATH: %s\n", PVN_QUADMATH);
#endif /* PVN_QUADMATH */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_omp_()
{
  return pvn_omp();
}
#endif /* ?PVN_TEST */

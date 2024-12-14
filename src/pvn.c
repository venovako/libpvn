#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  (void)printf("libpvn.");
#ifdef PVN_DYNAMIC
  (void)printf("%s ", PVN_DYNAMIC);
#else /* !PVN_DYNAMIC */
  (void)printf("a ");
#endif /* ?PVN_DYNAMIC */
  (void)printf("built on %s with %s for %s on %s ", __DATE__, PVN_COMPILER, PVN_OS, PVN_ARCH);
#ifdef NDEBUG
  (void)printf("with optimizations ");
#else /* !NDEBUG */
  (void)printf("for debugging ");
#endif /* ?NDEBUG */
  (void)printf("and with OpenMP ");
#ifdef _OPENMP
  (void)printf("%d\n", _OPENMP);
#else /* !_OPENMP */
  (void)printf("disabled\n");
#endif /* ?_OPENMP */
  (void)printf("endianness  : %s\n", (pvn_le() ? "little" : "big"));
#ifdef PVN_CR_MATH
  (void)printf("PVN_CR_MATH : %s\n", PVN_CR_MATH);
#endif /* PVN_CR_MATH */
#ifdef PVN_QUADMATH
  (void)printf("PVN_QUADMATH: %s\n", PVN_QUADMATH);
#endif /* PVN_QUADMATH */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_le_()
{
  return pvn_le();
}

int pvn_omp_()
{
  return pvn_omp();
}
#endif /* ?PVN_TEST */

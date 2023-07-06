#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  (void)printf("libpvn built on %s with %s for %s on %s ", __DATE__, PVN_COMPILER, PVN_OS, PVN_ARCH);
#ifdef NDEBUG
  (void)printf("with optimization level %d\n", NDEBUG);
#else /* !NDEBUG */
  (void)printf("for debugging\n");
#endif /* ?NDEBUG */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_le_()
{
  return pvn_le();
}
#endif /* ?PVN_TEST */

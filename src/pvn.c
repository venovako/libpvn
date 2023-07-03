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
int pvn_le()
{
  return
#ifdef __LITTLE_ENDIAN__
    __LITTLE_ENDIAN__
#else /* !__LITTLE_ENDIAN__ */
    0
#endif /* ?__LITTLE_ENDIAN__ */
    ;
}
#endif /* ?PVN_TEST */

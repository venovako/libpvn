#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  (void)printf("libpvn built on %s ", __DATE__);
#ifdef NDEBUG
  (void)printf("with optimization level %d\n", NDEBUG);
#else /* !NDEBUG */
  (void)printf("for debugging\n");
#endif /* ?NDEBUG */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#endif /* ?PVN_TEST */

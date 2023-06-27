#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  struct timespec t;
  PVN_SYSI_CALL(clock_getres(PVN_CLOCK_MONOTONIC, &t));
  (void)printf("%s resolution: %ld ns.\n", ((PVN_CLOCK_MONOTONIC == CLOCK_MONOTONIC) ? "CLOCK_MONOTONIC" : "CLOCK_MONOTONIC_RAW"), pvn_t2ns(&t));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#endif /* ?PVN_TEST */

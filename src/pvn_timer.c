#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  (void)printf("%s resolution: %ld ns.\n", ((PVN_CLOCK_MONOTONIC == CLOCK_MONOTONIC) ? "CLOCK_MONOTONIC" : "CLOCK_MONOTONIC_RAW"), pvn_time_sys_res());
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
long pvn_time_thread_res()
{
  struct timespec t;
  PVN_SYSI_CALL(clock_getres(CLOCK_THREAD_CPUTIME_ID, &t));
  return pvn_t2ns(&t);
}

long pvn_time_sys_res()
{
  struct timespec t;
  PVN_SYSI_CALL(clock_getres(PVN_CLOCK_MONOTONIC, &t));
  return pvn_t2ns(&t);
}
#endif /* ?PVN_TEST */

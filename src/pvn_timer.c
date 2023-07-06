#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  (void)printf("%s resolution: %ld ns.\n", ((PVN_CLOCK_MONOTONIC == CLOCK_MONOTONIC) ? "CLOCK_MONOTONIC" : "CLOCK_MONOTONIC_RAW"), pvn_time_mono_res());
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
long pvn_time_real_us_()
{
  return pvn_time_real_us();
}

long pvn_time_thread_ns_()
{
  return pvn_time_thread_ns();
}

long pvn_time_mono_ns_()
{
  return pvn_time_mono_ns();
}

long pvn_time_thread_res_()
{
  return pvn_time_thread_res();
}

long pvn_time_mono_res_()
{
  return pvn_time_mono_res();
}
#endif /* ?PVN_TEST */

#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  (void)printf("%s resolution: %ld ns.\n", ((PVN_CLOCK_MONOTONIC == CLOCK_MONOTONIC) ? "CLOCK_MONOTONIC" : "CLOCK_MONOTONIC_RAW"), pvn_time_mono_res());
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
long PVN_FABI(pvn_time_real_us,PVN_TIME_REAL_US)()
{
  return pvn_time_real_us();
}

long PVN_FABI(pvn_time_thread_ns,PVN_TIME_THREAD_NS)()
{
  return pvn_time_thread_ns();
}

long PVN_FABI(pvn_time_mono_ns,PVN_TIME_MONO_NS)()
{
  return pvn_time_mono_ns();
}

long PVN_FABI(pvn_time_thread_res,PVN_TIME_THREAD_RES)()
{
  return pvn_time_thread_res();
}

long PVN_FABI(pvn_time_mono_res,PVN_TIME_MONO_RES)()
{
  return pvn_time_mono_res();
}
#endif /* ?PVN_TEST */

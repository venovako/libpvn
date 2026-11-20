#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (1 != argc) {
    (void)fprintf(stderr, "%s takes no arguments\n", *argv);
    return EXIT_FAILURE;
  }
#ifdef _WIN32
  (void)printf("QueryPerformanceFrequency: %ld Hz.\n", (long)PVN_FABI(pvn_time_mono_freq,PVN_TIME_MONO_FREQ)());
#else /* !_WIN32 */
  (void)printf("%s resolution: %lld ns.\n", ((PVN_CLOCK_MONOTONIC == CLOCK_MONOTONIC) ? "CLOCK_MONOTONIC" : "CLOCK_MONOTONIC_RAW"), pvn_time_mono_res());
#if !defined(__NVCOMPILER) && defined(__x86_64__)
  unsigned long long rem = 0ull;
  const unsigned long long hz = pvn_tsc_get_freq_Hz(&rem);
  unsigned baseF = 0u, maxF = 0u, busF = 0u, res = 0u;
  pvn_cpu_get_freq_MHz(&baseF, &maxF, &busF, &res);
  (void)printf("Base freq [MHz] = %u\n", baseF);
  (void)printf("Max. freq [MHz] = %u\n", maxF);
  (void)printf("Bus  freq [MHz] = %u\n", busF);
  (void)printf("CPUID 0x16 = %u\n", res);
  (void)printf("CPUID 0x15 [Hz] = %llu (%llu)\n", hz, rem);
  const unsigned long long beg = pvn_rdtsc_beg(&res);
  (void)printf("beg = %llu (%u)\n", beg, res);
  const unsigned long long end = pvn_rdtsc_end(&res);
  (void)printf("end = %llu (%u)\n", end, res);
  const long double lap = pvn_tsc_lap(hz, beg, end);
  (void)printf("lap = %#.9LF [s]\n", lap);
#endif /* !__NVCOMPILER && __x86_64__ */
#endif /* ?_WIN32 */
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#ifdef _WIN32
#include <Windows.h>

long long PVN_FABI(pvn_time_mono_ticks,PVN_TIME_MONO_TICKS)()
{
  long long r = 0ll;
  return (QueryPerformanceCounter((LARGE_INTEGER*)&r) ? r : -1ll);
}

long long PVN_FABI(pvn_time_mono_freq,PVN_TIME_MONO_FREQ)()
{
  long long r = 0ll;
  return (QueryPerformanceFrequency((LARGE_INTEGER*)&r) ? r : -1ll);
}
#else /* !_WIN32 */
long long PVN_FABI(pvn_time_real_us,PVN_TIME_REAL_US)()
{
  return pvn_time_real_us();
}

long long PVN_FABI(pvn_time_thread_ns,PVN_TIME_THREAD_NS)()
{
  return pvn_time_thread_ns();
}

long long PVN_FABI(pvn_time_thread_res,PVN_TIME_THREAD_RES)()
{
  return pvn_time_thread_res();
}

long long PVN_FABI(pvn_time_mono_ns,PVN_TIME_MONO_NS)()
{
  return pvn_time_mono_ns();
}

long long PVN_FABI(pvn_time_mono_res,PVN_TIME_MONO_RES)()
{
  return pvn_time_mono_res();
}

long long PVN_FABI(pvn_time_mono_ticks,PVN_TIME_MONO_TICKS)()
{
  return pvn_time_mono_ns();
}

long long PVN_FABI(pvn_time_mono_freq,PVN_TIME_MONO_FREQ)()
{
  return 1000000000ll;
}
#endif /* ?_WIN32 */
#endif /* ?PVN_TEST */

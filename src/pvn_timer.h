#ifndef PVN_TIMER_H
#define PVN_TIMER_H

#ifndef PVN_H
#error pvn_timer.h not intended for direct inclusion
#endif /* !PVN_H */

#ifndef _WIN32
#ifndef PVN_CLOCK_MONOTONIC
#ifdef CLOCK_MONOTONIC_RAW
#define PVN_CLOCK_MONOTONIC CLOCK_MONOTONIC_RAW
#else /* !CLOCK_MONOTONIC_RAW */
#define PVN_CLOCK_MONOTONIC CLOCK_MONOTONIC
#endif /* ?CLOCK_MONOTONIC_RAW */
#else /* PVN_CLOCK_MONOTONIC */
#error PVN_CLOCK_MONOTONIC already defined
#endif /* ?PVN_CLOCK_MONOTONIC */

static inline long long pvn_t2us(const struct timeval *const tp)
{
  return (tp ? (tp->tv_sec * 1000000ll + tp->tv_usec) : -1ll);
}

static inline long long pvn_t2ns(const struct timespec *const tp)
{
  return (tp ? (tp->tv_sec * 1000000000ll + tp->tv_nsec) : -1ll);
}

static inline lldiv_t pvn_us2s(const long long us)
{
  return lldiv(us, 1000000ll);
}

static inline lldiv_t pvn_ns2s(const long long ns)
{
  return lldiv(ns, 1000000000ll);
}

static inline long long pvn_ns2us(const long long ns)
{
  const lldiv_t qr = lldiv(ns, 1000ll);
  return ((qr.rem >= 500ll) ? (qr.quot + 1ll) : qr.quot);
}

static inline long long pvn_time_real_us()
{
  struct timeval t = { (time_t)0, (suseconds_t)0 };
  return (gettimeofday(&t, NULL) ? -1ll : pvn_t2us(&t));
}

static inline long long pvn_time_proc_ns()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t) ? -1ll : pvn_t2ns(&t));
}

static inline long long pvn_time_proc_res()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_getres(CLOCK_PROCESS_CPUTIME_ID, &t) ? -1ll : pvn_t2ns(&t));
}

static inline long long pvn_time_thread_ns()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t) ? -1ll : pvn_t2ns(&t));
}

static inline long long pvn_time_thread_res()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_getres(CLOCK_THREAD_CPUTIME_ID, &t) ? -1ll : pvn_t2ns(&t));
}

static inline long long pvn_time_mono_ns()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_gettime(PVN_CLOCK_MONOTONIC, &t) ? -1ll : pvn_t2ns(&t));
}

static inline long long pvn_time_mono_res()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_getres(PVN_CLOCK_MONOTONIC, &t) ? -1ll : pvn_t2ns(&t));
}

#if !defined(__NVCOMPILER) && defined(__x86_64__)
#include <cpuid.h>
#if defined(__clang__) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER))
#include <x86intrin.h>
#endif /* Clang */

static inline unsigned long long pvn_rdtsc_beg(unsigned aux[static restrict 1])
{
  _mm_mfence();
  return (unsigned long long)__rdtscp(aux);
}

static inline unsigned long long pvn_rdtsc_end(unsigned aux[static restrict 1])
{
  const unsigned long long tsc = (unsigned long long)__rdtscp(aux);
  _mm_lfence();
  return tsc;
}

static inline void pvn_cpu_get_freq_MHz(unsigned baseF[static restrict 1], unsigned maxF[static restrict 1], unsigned busF[static restrict 1], unsigned res[static restrict 1])
{
  *res = *busF = *maxF = *baseF = 0u;
  *res = (unsigned)__get_cpuid(0x16u, baseF, maxF, busF, res);
}

static inline unsigned long long pvn_tsc_get_freq_Hz(unsigned long long rem[static restrict 1])
{
  unsigned long long hz = 0ull;
  unsigned eax = 0u, ebx = 0u, ecx = 0u, edx = 0u;
  if ((*rem = (unsigned long long)__get_cpuid(0x15u, &eax, &ebx, &ecx, &edx)) && eax && ebx && ecx) {
    /* ecx * (ebx / eax) = (ecx * ebx) / eax */
    const unsigned long long num = (unsigned long long)ebx * (unsigned long long)ecx;
    *rem = (unsigned long long)eax;
    hz = num / *rem;
    *rem = num % *rem;
  }
  else {
    pvn_cpu_get_freq_MHz(&eax, &ebx, &ecx, &edx);
    if (edx) {
      hz = (unsigned long long)eax * 1000000ull;
      *rem = (unsigned long long)ebx * 1000000ull + (unsigned long long)ecx;
    }
    else /* set the TSC_FREQ_HZ environment variable to the refined TSC clocksource calibration value from `dmesg` */
      hz = (unsigned long long)pvn_atoz(getenv("TSC_FREQ_HZ"));
  }
  return hz;
}
#elif defined(__aarch64__)
static inline unsigned long long pvn_rdtsc_beg(unsigned *const aux)
{ // Google & https://learn.arm.com/learning-paths/servers-and-cloud-computing/arm_pmu/assembly/
  uint64_t val;
  asm volatile("isb; mrs %0, cntvct_el0" : "=r"(val));
  if (aux)
    *aux = 0u;
  return (unsigned long long)val;
}

static inline unsigned long long pvn_rdtsc_end(unsigned *const aux)
{ // Google & https://learn.arm.com/learning-paths/servers-and-cloud-computing/arm_pmu/assembly/
  uint64_t val;
  asm volatile("isb; mrs %0, cntvct_el0" : "=r"(val));
  if (aux)
    *aux = 0u;
  return (unsigned long long)val;
}

static inline void pvn_cpu_get_freq_MHz(unsigned baseF[static restrict 1], unsigned maxF[static restrict 1], unsigned busF[static restrict 1], unsigned res[static restrict 1])
{
  *busF = (unsigned)(pvn_atoz(getenv("TSC_FREQ_HZ")) / 1000000u);
  *res = *maxF = *baseF = 0u;
}

static inline unsigned long long pvn_tsc_get_freq_Hz(unsigned long long *const rem)
{ // Google & https://learn.arm.com/learning-paths/servers-and-cloud-computing/arm_pmu/assembly/
  uint64_t freq;
  asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
  if (rem)
    *rem = 0ull;
  return (unsigned long long)freq;
}
#elif defined(__PPC__)
#include <sys/platform/ppc.h>

static inline unsigned long long pvn_rdtsc_beg(unsigned *const aux)
{ // Google
  const uint64_t val = __ppc_get_timebase();
  if (aux)
    *aux = 0u;
  return (unsigned long long)val;
}

static inline unsigned long long pvn_rdtsc_end(unsigned *const aux)
{ // Google
  const uint64_t val = __ppc_get_timebase();
  if (aux)
    *aux = 0u;
  return (unsigned long long)val;
}

static inline void pvn_cpu_get_freq_MHz(unsigned baseF[static restrict 1], unsigned maxF[static restrict 1], unsigned busF[static restrict 1], unsigned res[static restrict 1])
{
  *busF = (unsigned)(pvn_atoz(getenv("TSC_FREQ_HZ")) / 1000000u);
  *res = *maxF = *baseF = 0u;
}

static inline unsigned long long pvn_tsc_get_freq_Hz(unsigned long long *const rem)
{ // Google
  const uint64_t freq = __ppc_get_timebase_freq();
  if (rem)
    *rem = 0ull;
  return (unsigned long long)freq;
}
#else /* arch unknown */
static inline unsigned long long pvn_rdtsc_beg(unsigned *const aux)
{
  if (aux)
    *aux = 0u;
  return 0ull;
}

static inline unsigned long long pvn_rdtsc_end(unsigned *const aux)
{
  if (aux)
    *aux = 0u;
  return 0ull;
}

static inline void pvn_cpu_get_freq_MHz(unsigned baseF[static restrict 1], unsigned maxF[static restrict 1], unsigned busF[static restrict 1], unsigned res[static restrict 1])
{
  *busF = (unsigned)(pvn_atoz(getenv("TSC_FREQ_HZ")) / 1000000u);
  *res = *maxF = *baseF = 0u;
}

static inline unsigned long long pvn_tsc_get_freq_Hz(unsigned long long *const rem)
{
  if (rem)
    *rem = 0ull;
  return (unsigned long long)pvn_atoz(getenv("TSC_FREQ_HZ"));
}
#endif /* arch */

static inline long double pvn_tsc_sec(const unsigned long long freq_hz, const unsigned long long dif)
{
  return (dif ? ((long double)dif / (long double)freq_hz) : 0.0L);
}

static inline long double pvn_tsc_lap(const unsigned long long freq_hz, const unsigned long long beg, const unsigned long long end)
{
  return ((end >= beg) ? pvn_tsc_sec(freq_hz, (end - beg)) : __builtin_nanl(""));
}

PVN_EXTERN_C long long PVN_FABI(pvn_time_real_us,PVN_TIME_REAL_US)();
PVN_EXTERN_C long long PVN_FABI(pvn_time_thread_ns,PVN_TIME_THREAD_NS)();
PVN_EXTERN_C long long PVN_FABI(pvn_time_thread_res,PVN_TIME_THREAD_RES)();
PVN_EXTERN_C long long PVN_FABI(pvn_time_mono_ns,PVN_TIME_MONO_NS)();
PVN_EXTERN_C long long PVN_FABI(pvn_time_mono_res,PVN_TIME_MONO_RES)();
#endif /* !_WIN32 */
PVN_EXTERN_C long long PVN_FABI(pvn_time_mono_ticks,PVN_TIME_MONO_TICKS)();
PVN_EXTERN_C long long PVN_FABI(pvn_time_mono_freq,PVN_TIME_MONO_FREQ)();

#endif /* !PVN_TIMER_H */

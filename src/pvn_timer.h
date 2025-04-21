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

static inline long long pvn_time_thread_ns()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t) ? -1ll : pvn_t2ns(&t));
}

static inline long long pvn_time_mono_ns()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_gettime(PVN_CLOCK_MONOTONIC, &t) ? -1ll : pvn_t2ns(&t));
}

static inline long long pvn_time_thread_res()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_getres(CLOCK_THREAD_CPUTIME_ID, &t) ? -1ll : pvn_t2ns(&t));
}

static inline long long pvn_time_mono_res()
{
  struct timespec t = { (time_t)0, 0l };
  return (clock_getres(PVN_CLOCK_MONOTONIC, &t) ? -1ll : pvn_t2ns(&t));
}

PVN_EXTERN_C long long PVN_FABI(pvn_time_real_us,PVN_TIME_REAL_US)();
PVN_EXTERN_C long long PVN_FABI(pvn_time_thread_ns,PVN_TIME_THREAD_NS)();
PVN_EXTERN_C long long PVN_FABI(pvn_time_thread_res,PVN_TIME_THREAD_RES)();
#endif /* !_WIN32 */
PVN_EXTERN_C long long PVN_FABI(pvn_time_mono_ns,PVN_TIME_MONO_NS)();
PVN_EXTERN_C long long PVN_FABI(pvn_time_mono_res,PVN_TIME_MONO_RES)();

#endif /* !PVN_TIMER_H */

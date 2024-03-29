#ifndef PVN_TIMER_H
#define PVN_TIMER_H

#ifndef PVN_H
#error pvn_timer.h not intended for direct inclusion
#endif /* !PVN_H */

#ifndef PVN_CLOCK_MONOTONIC
#ifdef CLOCK_MONOTONIC_RAW
#define PVN_CLOCK_MONOTONIC CLOCK_MONOTONIC_RAW
#else /* !CLOCK_MONOTONIC_RAW */
#define PVN_CLOCK_MONOTONIC CLOCK_MONOTONIC
#endif /* ?CLOCK_MONOTONIC_RAW */
#else /* PVN_CLOCK_MONOTONIC */
#error PVN_CLOCK_MONOTONIC already defined
#endif /* ?PVN_CLOCK_MONOTONIC */

static inline long PVN_NO_PROF pvn_t2us(const struct timeval *const tp)
{
  return (tp ? (tp->tv_sec * 1000000L + tp->tv_usec) : -1L);
}

static inline long PVN_NO_PROF pvn_t2ns(const struct timespec *const tp)
{
  return (tp ? (tp->tv_sec * 1000000000L + tp->tv_nsec) : -1L);
}

static inline ldiv_t PVN_NO_PROF pvn_us2s(const long us)
{
  return ldiv(us, 1000000L);
}

static inline ldiv_t PVN_NO_PROF pvn_ns2s(const long ns)
{
  return ldiv(ns, 1000000000L);
}

static inline long PVN_NO_PROF pvn_ns2us(const long ns)
{
  const ldiv_t qr = ldiv(ns, 1000L);
  return ((qr.rem >= 500L) ? (qr.quot + 1L) : qr.quot);
}

static inline long PVN_NO_PROF pvn_time_real_us()
{
  struct timeval t;
  return (gettimeofday(&t, NULL) ? -1L : pvn_t2us(&t));
}

static inline long PVN_NO_PROF pvn_time_thread_ns()
{
  struct timespec t;
  return (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t) ? -1L : pvn_t2ns(&t));
}

static inline long PVN_NO_PROF pvn_time_mono_ns()
{
  struct timespec t;
  return (clock_gettime(PVN_CLOCK_MONOTONIC, &t) ? -1L : pvn_t2ns(&t));
}

static inline long PVN_NO_PROF pvn_time_thread_res()
{
  struct timespec t;
  return (clock_getres(CLOCK_THREAD_CPUTIME_ID, &t) ? -1L : pvn_t2ns(&t));
}

static inline long PVN_NO_PROF pvn_time_mono_res()
{
  struct timespec t;
  return (clock_getres(PVN_CLOCK_MONOTONIC, &t) ? -1L : pvn_t2ns(&t));
}

PVN_EXTERN_C long pvn_time_real_us_();
PVN_EXTERN_C long pvn_time_thread_ns_();
PVN_EXTERN_C long pvn_time_mono_ns_();
PVN_EXTERN_C long pvn_time_thread_res_();
PVN_EXTERN_C long pvn_time_mono_res_();

#endif /* !PVN_TIMER_H */

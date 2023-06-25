#ifndef PVN_TIMER_H
#define PVN_TIMER_H

#ifndef PVN_H
#error pvn_timer.h not intended for direct inclusion
#endif /* !PVN_H */

static inline long pvn_t2ns(const struct timespec tp[static 1])
{
  return (tp->tv_sec * 1000000000L + tp->tv_nsec);
}

static inline long pvn_t2us(const struct timeval tp[static 1])
{
  return (tp->tv_sec * 1000000L + tp->tv_usec);
}

static inline long pvn_ns2us(const long ns)
{
  const ldiv_t qr = ldiv(ns, 1000L);
  return ((qr.rem >= 500L) ? (qr.quot + 1L) : qr.quot);
}

static inline ldiv_t pvn_ns2s(const long ns)
{
  return ldiv(ns, 1000000000L);
}

static inline ldiv_t pvn_us2s(const long us)
{
  return ldiv(us, 1000000L);
}

static inline long pvn_time_thread_ns()
{
  struct timespec t;
  return (clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t) ? -1L : pvn_t2ns(&t));
}

static inline long pvn_time_sys_ns()
{
  struct timespec t;
  return (clock_gettime(CLOCK_MONOTONIC_RAW, &t) ? -1L : pvn_t2ns(&t));
}

static inline long pvn_time_sys_us()
{
  struct timeval t;
  return (gettimeofday(&t, NULL) ? -1L : pvn_t2us(&t));
}

#endif /* !PVN_TIMER_H */

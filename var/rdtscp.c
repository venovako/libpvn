#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpuid.h>

#ifndef __RDTSC__
#error __RDTSC__ not defined
#endif /* !__RDTSC__ */

static inline unsigned long long rdtsc_beg(unsigned aux[static restrict 1])
{
  _mm_mfence();
  return (unsigned long long)__rdtscp(aux);
}

static inline unsigned long long rdtsc_end(unsigned aux[static restrict 1])
{
  const unsigned long long tsc = (unsigned long long)__rdtscp(aux);
  _mm_lfence();
  return tsc;
}

static inline void cpu_get_freq_MHz(int baseF[static restrict 1], int maxF[static restrict 1], int busF[static restrict 1], int res[static restrict 1])
{
  *res = *busF = *maxF = *baseF = 0;
  *res = __get_cpuid(0x16, baseF, maxF, busF, res);
}

static inline long long tsc_get_freq_Hz(lldiv_t qr[static restrict 1])
{
  int eax = 0, ebx = 0, ecx = 0, edx = 0;
  if ((qr->quot = -__get_cpuid(0x15, &eax, &ebx, &ecx, &edx)) && eax)
    *qr = lldiv(((long long)ebx * (long long)ecx), (long long)eax);
  else {
    qr->rem = eax;
    cpu_get_freq_MHz(&eax, &ebx, &ecx, &edx);
    if (edx) {
      qr->quot = (long long)eax * 1000000ll;
      qr->rem = (long long)ebx * 1000000ll + (long long)ecx;
    }
  }
  return (qr->quot);
}

static inline long double tsc_lap(const long long freq_hz, const unsigned long long beg, const unsigned long long end)
{
  return ((freq_hz > 0ll) ? ((end >= beg) ? ((long double)(end - beg) / (long double)freq_hz) : __builtin_nanl("")) : __builtin_nanl(""));
}

int main()
{
  lldiv_t qr;
  const long long hz = tsc_get_freq_Hz(&qr);
  int baseF = 0, maxF = 0, busF = 0, res = 0;
  cpu_get_freq_MHz(&baseF, &maxF, &busF, &res);
  (void)printf("Base freq [MHz] = %d\n", baseF);
  (void)printf("Max. freq [MHz] = %d\n", maxF);
  (void)printf("Bus  freq [MHz] = %d\n", busF);
  (void)printf("CPUID 0x16 = %d\n", res);
  (void)printf("CPUID 0x15 [Hz] = %lld (%lld)\n", hz, qr.rem);
  const unsigned long long beg = rdtsc_beg((unsigned*)&res);
  (void)printf("beg = %llu (%u)\n", beg, res);
  const unsigned long long end = rdtsc_end((unsigned*)&res);
  (void)printf("end = %llu (%u)\n", end, res);
  const long double lap = tsc_lap(hz, beg, end);
  (void)printf("lap = %#.9LF [s]\n", lap);
  return EXIT_SUCCESS;
}

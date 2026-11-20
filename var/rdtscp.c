#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <cpuid.h>

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

static inline long long tsc_get_freq_hz(lldiv_t qr[static restrict 1])
{
  int eax = 0, ebx = 0, ecx = 0, edx = 0;
  if ((qr->quot = -__get_cpuid(0x15, &eax, &ebx, &ecx, &edx)) && eax)
    *qr = lldiv(((long long)ebx * (long long)ecx), (long long)eax);
  else
    qr->rem = eax;
  return (qr->quot);
}

int main()
{
  lldiv_t qr;
  const long long hz = tsc_get_freq_hz(&qr);
  (void)printf("Hz = %lld (%lld / %lld)\n", hz, qr.quot, qr.rem);
  unsigned aux = 0u;
  (void)printf("beg = %llu (", rdtsc_beg(&aux));
  (void)printf("%u)\n", aux);
  (void)printf("end = %llu (", rdtsc_end(&aux));
  (void)printf("%u)\n", aux);
  return EXIT_SUCCESS;
}

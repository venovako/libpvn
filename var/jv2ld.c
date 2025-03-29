#include "pvn.h"
#include "gmp.h"
#include "mpfr.h"

#ifdef LDBL_BIG_EXP
#error LDBL_BIG_EXP already defined
#else /* !LDBL_BIG_EXP */
#define LDBL_BIG_EXP (LDBL_MAX_EXP - 2)
#endif /* ?LDBL_BIG_EXP */

static mpfr_t A1, A2, AR, AI, CS, SNR, SNI, AA, AN, AD, T2, T1;

static int init_mpfr(const mpfr_prec_t *const p)
{
#ifndef NDEBUG
  assert(p);
#endif /* !NDEBUG */
  if (*p < MPFR_PREC_MIN)
    return -1;
  if (*p > MPFR_PREC_MAX)
    return -1;
  if (*p != mpfr_get_default_prec())
    mpfr_set_default_prec(*p);
  const mpfr_exp_t emin = mpfr_get_emin_min();
  if ((emin < mpfr_get_emin()) && mpfr_set_emin(emin))
    return 1;
  const mpfr_exp_t emax = mpfr_get_emax_max();
  if ((emax > mpfr_get_emax()) && mpfr_set_emax(emax))
    return 2;
  if (mpfr_init_set_ld(A1, 0.0L, MPFR_RNDN))
    return 3;
  if (mpfr_init_set_ld(A2, 0.0L, MPFR_RNDN))
    return 4;
  if (mpfr_init_set_ld(AR, 0.0L, MPFR_RNDN))
    return 5;
  if (mpfr_init_set_ld(AI, 0.0L, MPFR_RNDN))
    return 6;
  if (mpfr_init_set_ld(CS, 1.0L, MPFR_RNDN))
    return 7;
  if (mpfr_init_set_ld(SNR, 0.0L, MPFR_RNDN))
    return 8;
  if (mpfr_init_set_ld(SNI, 0.0L, MPFR_RNDN))
    return 9;
  if (mpfr_init_set_ld(AA, 0.0L, MPFR_RNDN))
    return 10;
  if (mpfr_init_set_ld(AN, 0.0L, MPFR_RNDN))
    return 11;
  if (mpfr_init_set_ld(AD, 0.0L, MPFR_RNDN))
    return 12;
  if (mpfr_init_set_ld(T2, 0.0L, MPFR_RNDN))
    return 13;
  if (mpfr_init_set_ld(T1, 0.0L, MPFR_RNDN))
    return 14;
  return 0;
}

static void fini_mpfr()
{
  mpfr_clear(T1);
  mpfr_clear(T2);
  mpfr_clear(AD);
  mpfr_clear(AN);
  mpfr_clear(AA);
  mpfr_clear(SNI);
  mpfr_clear(SNR);
  mpfr_clear(CS);
  mpfr_clear(AI);
  mpfr_clear(AR);
  mpfr_clear(A2);
  mpfr_clear(A1);
  mpfr_free_cache();
}

/* e.g., ./jv2ld.exe 1024 4.895968119607223484400E-01 6.543925533503869600887E-01 -5.231534659497175174378E-01 1.309017422649569065629E-01 */
int main(int argc, char *argv[])
{
  if (argc != 6) {
    (void)fprintf(stderr, "%s p a11 a22 a21r a21i\n", *argv);
    return EXIT_FAILURE;
  }
  const mpfr_prec_t p = (mpfr_prec_t)atoi(argv[1]);
#ifndef NDEBUG
  (void)printf("MPFR_PREC_MIN=%zu\n", (size_t)MPFR_PREC_MIN);
  (void)printf("MPFR_PREC_MAX=%zu\n", (size_t)MPFR_PREC_MAX);
  (void)printf("mpfr_get_emin=%zd\n", (ssize_t)mpfr_get_emin());
  (void)printf("mpfr_get_emax=%zd\n", (ssize_t)mpfr_get_emax());
#endif /* !NDEBUG */
  (void)printf("init_mpfr=%d\n", init_mpfr(&p));
#ifndef NDEBUG
  (void)printf("mpfr_get_emin=%zd\n", (ssize_t)mpfr_get_emin());
  (void)printf("mpfr_get_emax=%zd\n", (ssize_t)mpfr_get_emax());
#endif /* !NDEBUG */
  char s[31] = { '\0' };
  long double a1 = strtold(argv[2], (char**)NULL);
  if (!isfinite(a1))
    return EXIT_FAILURE;
  if (a1 < 0.0L)
    return EXIT_FAILURE;
  long double a2 = strtold(argv[3], (char**)NULL);
  if (!isfinite(a2))
    return EXIT_FAILURE;
  if (a2 < 0.0L)
    return EXIT_FAILURE;
  long double ar = strtold(argv[4], (char**)NULL);
  if (!isfinite(ar))
    return EXIT_FAILURE;
  long double ai = strtold(argv[5], (char**)NULL);
  if (!isfinite(ai))
    return EXIT_FAILURE;
  int
    e1 = (a1 != 0.0L),
    e2 = (a2 != 0.0L),
    er = (ar != 0.0L),
    ei = (ai != 0.0L),
    es = 0;
  long double cs = 0.0L, snr = 0.0L, sni = 0.0L, t1 = 0.0L;
  if (er || ei) {
    es = (e1 | (e2 << 1) | (er << 2) | (ei << 3));
    if (es) {
      (void)frexpl(fmaxl(a1, LDBL_TRUE_MIN), &e1);
      (void)frexpl(fmaxl(a2, LDBL_TRUE_MIN), &e2);
      (void)frexpl(fmaxl(fabsl(ar), LDBL_TRUE_MIN), &er);
      (void)frexpl(fmaxl(fabsl(ai), LDBL_TRUE_MIN), &ei);
      e1 = pvn_imax4(e1, e2, er, ei);
      e2 = es;
      es = (LDBL_BIG_EXP - e1);
      a1 = scalbnl(a1, es);
      a2 = scalbnl(a2, es);
      ar = scalbnl(ar, es);
      ai = scalbnl(ai, es);
      er = (ar != 0.0L);
      ei = (ai != 0.0L);
      es = -es;
    }
    if (er || ei) {
      (void)mpfr_set_ld(A1, a1, MPFR_RNDN);
      (void)mpfr_set_ld(A2, a2, MPFR_RNDN);
      (void)mpfr_set_ld(AR, ar, MPFR_RNDN);
      (void)mpfr_set_ld(AI, ai, MPFR_RNDN);
      (void)printf("a11   =%s\n", pvn_xtoa(s, a1));
      (void)printf("a22   =%s\n", pvn_xtoa(s, a2));
      (void)printf("a21r  =%s\n", pvn_xtoa(s, ar));
      (void)printf("a21i  =%s\n", pvn_xtoa(s, ai));
      const long double
        ar_ = fabsl(ar),
        ai_ = fabsl(ai),
        aa = hypotl(ar_, ai_); /* aa cannot be zero here if hypotl is correctly rounded */
      (void)printf("|a21| =%s\n", pvn_xtoa(s, aa));
      (void)mpfr_hypot(AA, AR, AI, MPFR_RNDN);
      (void)printf("|A21| =%s\n", pvn_xtoa(s, mpfr_get_ld(AA, MPFR_RNDN)));
      /* a non-zero element underflows due to scaling */
      e1 = ((((e2 & 1) && (a1 < LDBL_MIN)) || ((e2 & 2) && (a2 < LDBL_MIN)) || ((e2 & 4) && (ar_ < LDBL_MIN)) || ((e2 & 8) && (ai_ < LDBL_MIN))) << 1);
      ar = (ar / aa);
      (void)printf("a21r' =%s\n", pvn_xtoa(s, ar));
      (void)mpfr_div(AR, AR, AA, MPFR_RNDN);
      ai = (ai / aa);
      (void)printf("a21i' =%s\n", pvn_xtoa(s, ai));
      (void)mpfr_div(AI, AI, AA, MPFR_RNDN);
      const long double
        an = (2.0L * aa),
        ad = (a1 + a2),
        t2 = -((an >= ad) ? 1.0L : (an / ad));
      (void)printf("t2    =%s\n", pvn_xtoa(s, t2));
      (void)mpfr_set_ld(AN, an, MPFR_RNDN);
      (void)mpfr_add(AD, A1, A2, MPFR_RNDN);
      if (an >= ad)
        (void)mpfr_set_ld(T2, 1.0L, MPFR_RNDN);
      else
        (void)mpfr_div(T2, AN, AD, MPFR_RNDN);
      t1 = (t2 / (1.0L + sqrtl(fmal(-t2, t2, 1.0L))));
      (void)printf("t1    =%s\n", pvn_xtoa(s, t1));
      (void)mpfr_neg(T1, T2, MPFR_RNDN);
      (void)mpfr_set_ld(CS, 1.0L, MPFR_RNDN);
      (void)mpfr_fma(T1, T1, T2, CS, MPFR_RNDN);
      (void)mpfr_sqrt(T1, T1, MPFR_RNDN);
      (void)mpfr_add(T1, CS, T1, MPFR_RNDN);
      (void)mpfr_div(T1, T2, T1, MPFR_RNDN);
      (void)printf("T1    =%s\n", pvn_xtoa(s, mpfr_get_ld(T1, MPFR_RNDN)));
      /* tangent underflows */
      e2 = ((fabsl(t1) < LDBL_MIN) << 2);
    }
    else
      e2 = e1 = 0;
  }
  else
    es = e2 = e1 = 0;
  cs = fmal(-t1, t1, 1.0L);
  (void)printf("t1^2+1=%s\n", pvn_xtoa(s, cs));
  (void)mpfr_neg(SNR, T1, MPFR_RNDN);
  (void)mpfr_fma(CS, SNR, T1, CS, MPFR_RNDN);
  (void)printf("T1^2+1=%s\n", pvn_xtoa(s, mpfr_get_ld(CS, MPFR_RNDN)));
  (void)printf("cs'   =%s\n", pvn_xtoa(s, 1.0L / sqrtl(cs)));
  cs = rsqrtl(cs);
  (void)printf("cs    =%s\n", pvn_xtoa(s, cs));
  (void)mpfr_rec_sqrt(CS, CS, MPFR_RNDN);
  (void)printf("CS    =%s\n", pvn_xtoa(s, mpfr_get_ld(CS, MPFR_RNDN)));
  const long double s1 = (cs * t1);
  (void)printf("s1    =%s\n", pvn_xtoa(s, s1));
  (void)mpfr_mul(SNI, CS, T1, MPFR_RNDN);
  snr = (ar * s1);
  (void)mpfr_mul(SNR, AR, SNI, MPFR_RNDN);
  sni = (ai * s1);
  (void)mpfr_mul(SNI, AI, SNI, MPFR_RNDN);
  /* sine/tangent underflows with a non-zero aa */
  er = ((er && (fabsl(snr) < LDBL_MIN)) << 3);
  ei = ((ei && (fabsl(sni) < LDBL_MIN)) << 4);
  fini_mpfr();
  return EXIT_SUCCESS;
}

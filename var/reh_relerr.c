#include "pvn.h"

typedef struct {
  mpfr_t m, p;
} re_t;

static mpfr_t ed;
static char fmtm[32], fmtp[32];

static re_t relerr(const size_t m)
{
  re_t o;
  (void)mpfr_init_set_d(o.m, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(o.p, 0.0, MPFR_RNDN);
  if (m <= (size_t)1u)
    return o;
  if (m == (size_t)2u) {
    (void)mpfr_neg(o.m, ed, MPFR_RNDN);
    (void)mpfr_set(o.p, ed, MPFR_RNDN);
    return o;
  }

  const size_t n = ((m >> 1u) + (m & (size_t)1u));
  const size_t k = (m - n);
  re_t l = relerr(n);
#ifndef NDEBUG
  (void)mpfr_printf(fmtm, n, l.m);
  (void)mpfr_printf(fmtp, n, l.p);
#endif /* !NDEBUG */
  re_t r = relerr(k);
#ifndef NDEBUG
  (void)mpfr_printf(fmtm, k, r.m);
  (void)mpfr_printf(fmtp, k, r.p);
#endif /* !NDEBUG */

  mpfr_t e1, e2, eq;
  (void)mpfr_init(e1);
  (void)mpfr_init(e2);
  (void)mpfr_init(eq);

  (void)mpfr_add_d(e1, l.m, 1.0, MPFR_RNDN);
  (void)mpfr_add_d(e2, r.m, 1.0, MPFR_RNDN);
  (void)mpfr_div(eq, e1, e2, MPFR_RNDN);
  (void)mpfr_add_d(e1, eq, 1.0, MPFR_RNDN);
  (void)mpfr_sub_d(eq, eq, 1.0, MPFR_RNDN);
  (void)mpfr_mul(e1, eq, e1, MPFR_RNDN);
  (void)mpfr_add_d(e1, e1, 1.0, MPFR_RNDN);
  (void)mpfr_sqrt(e1, e1, MPFR_RNDN);
  (void)mpfr_mul(e1, e1, e2, MPFR_RNDN);
  (void)mpfr_d_sub(e2, 1.0, ed, MPFR_RNDN);
  (void)mpfr_mul(e1, e1, e2, MPFR_RNDN);
  (void)mpfr_sub_d(o.m, e1, 1.0, MPFR_RNDN);

  (void)mpfr_add_d(e1, r.p, 1.0, MPFR_RNDN);
  (void)mpfr_add_d(e2, l.p, 1.0, MPFR_RNDN);
  (void)mpfr_div(eq, e1, e2, MPFR_RNDN);
  (void)mpfr_add_d(e1, eq, 1.0, MPFR_RNDN);
  (void)mpfr_sub_d(eq, eq, 1.0, MPFR_RNDN);
  (void)mpfr_mul(e1, eq, e1, MPFR_RNDN);
  (void)mpfr_add_d(e1, e1, 1.0, MPFR_RNDN);
  (void)mpfr_sqrt(e1, e1, MPFR_RNDN);
  (void)mpfr_mul(e1, e1, e2, MPFR_RNDN);
  (void)mpfr_add_d(e2, ed, 1.0, MPFR_RNDN);
  (void)mpfr_mul(e1, e1, e2, MPFR_RNDN);
  (void)mpfr_sub_d(o.p, e1, 1.0, MPFR_RNDN);

  mpfr_clear(eq);
  mpfr_clear(e2);
  mpfr_clear(e1);
  mpfr_clear(r.p);
  mpfr_clear(r.m);
  mpfr_clear(l.p);
  mpfr_clear(l.m);
  return o;
}

int main(int argc, char *argv[])
{
  if (argc != 4) {
    (void)fprintf(stderr, "%s {S|D|X} prec m\n", *argv);
    return EXIT_FAILURE;
  }
  double e = 0.5;
  int f = 0;
  const char p = toupper(*(argv[1]));
  switch (p) {
  case 'S':
    e *= FLT_EPSILON;
    f = 9;
    (void)fprintf(stderr, "ε=%#.9e\n", e);
    break;
  case 'D':
    e *= DBL_EPSILON;
    f = 17;
    (void)fprintf(stderr, "ε=%#.17e\n", e);
    break;
  case 'X':
    e *= LDBL_EPSILON;
    f = 21;
    (void)fprintf(stderr, "ε=%#.21e\n", e);
    break;
  default:
    (void)fprintf(stderr, "%c not in {S,D,X}\n", p);
    return EXIT_FAILURE;
  }
  const size_t m = pvn_atoz(argv[3]);
  if (!m)
    return EXIT_FAILURE;
  mpfr_rnd_t rnd = MPFR_RNDN;
  mpfr_prec_t prec = atol(argv[2]);
  mpfr_exp_t emin = __MPFR_EXP_INVALID, emax = __MPFR_EXP_INVALID;
  if (PVN_FABI(pvn_mpfr_start,PVN_MPFR_START)(&rnd, &prec, &emin, &emax))
    return EXIT_FAILURE;
  const size_t d = (size_t)floorl(log10l(m) + 1.0L);
  (void)sprintf(fmtm, "ϵ%%0%zuzu-/ε=%%# .%dRe\n", d, f);
  (void)sprintf(fmtp, "ϵ%%0%zuzu+/ε=%%# .%dRe\n", d, f);
  (void)mpfr_init_set_d(ed, e, MPFR_RNDN);
  (void)fprintf(stderr, "(%c) relative error bounds for the recursive evaluation:\n", p);
  re_t r = relerr(m);
  (void)mpfr_div(r.m, r.m, ed, MPFR_RNDN);
  (void)mpfr_printf(fmtm, m, r.m);
  (void)mpfr_div(r.p, r.p, ed, MPFR_RNDN);
  (void)mpfr_printf(fmtp, m, r.p);
  mpfr_clear(r.p);
  mpfr_clear(r.m);
  mpfr_clear(ed);
  return (PVN_FABI(pvn_mpfr_stop,PVN_MPFR_STOP)() ? EXIT_FAILURE : EXIT_SUCCESS);
}

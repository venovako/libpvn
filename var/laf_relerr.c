#include "pvn.h"

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
  char
    fmtm[32] = { '\0' },
    fmtp[32] = { '\0' };
  const size_t d = (size_t)floorl(log10l(m) + 1.0L);
  (void)sprintf(fmtm, "ϵ%%0%zuzu-/ε=%%# .%dRe\n", d, f);
  (void)sprintf(fmtp, "ϵ%%0%zuzu+/ε=%%# .%dRe\n", d, f);
  mpfr_t em, ep, et, ef, ed;
  (void)mpfr_init_set_d(em,-0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(ep, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(et, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(ef, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(ed, e, MPFR_RNDN);
  size_t i = (size_t)1u;
  (void)fprintf(stderr, "(%c) relative error bounds for the sequential evaluation:\n", p);
  (void)mpfr_printf(fmtm, i, em);
  (void)mpfr_printf(fmtm, i, ep);
  for (++i; i <= m; ++i) {
    (void)mpfr_add_d(em, em, 1.0, MPFR_RNDN);
    (void)mpfr_d_sub(et, 1.0, ed, MPFR_RNDN);
    (void)mpfr_mul(em, em, et, MPFR_RNDN);
    (void)mpfr_sqrt(ef, em, MPFR_RNDN);
    (void)mpfr_mul(ef, ef, et, MPFR_RNDN);
    (void)mpfr_sub_d(em, em, 1.0, MPFR_RNDN);
    (void)mpfr_sub_d(ef, ef, 1.0, MPFR_RNDN);
    (void)mpfr_div(ef, ef, ed, MPFR_RNDN);
    (void)mpfr_printf(fmtm, i, ef);
    (void)mpfr_add_d(ep, ep, 1.0, MPFR_RNDN);
    (void)mpfr_add_d(et, ed, 1.0, MPFR_RNDN);
    (void)mpfr_mul(ep, ep, et, MPFR_RNDN);
    (void)mpfr_sqrt(ef, ep, MPFR_RNDN);
    (void)mpfr_mul(ef, ef, et, MPFR_RNDN);
    (void)mpfr_sub_d(ep, ep, 1.0, MPFR_RNDN);
    (void)mpfr_sub_d(ef, ef, 1.0, MPFR_RNDN);
    (void)mpfr_div(ef, ef, ed, MPFR_RNDN);
    (void)mpfr_printf(fmtp, i, ef);
  }
  mpfr_clear(ed);
  mpfr_clear(ef);
  mpfr_clear(et);
  mpfr_clear(ep);
  mpfr_clear(em);
  return (PVN_FABI(pvn_mpfr_stop,PVN_MPFR_STOP)() ? EXIT_FAILURE : EXIT_SUCCESS);
}

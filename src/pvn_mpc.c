#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (1 != argc) {
    (void)fprintf(stderr, "%s takes no arguments\n", *argv);
    return EXIT_FAILURE;
  }
  mpfr_rnd_t rnd = __MPFR_RND_INVALID;
  mpfr_prec_t prec = __MPFR_PREC_INVALID;
  mpfr_exp_t emin = __MPFR_EXP_INVALID, emax = __MPFR_EXP_INVALID;
  const int r = PVN_FABI(pvn_mpc_start,PVN_MPC_START)(&rnd, &prec, &emin, &emax);
  (void)printf("PVN_MPC_START=%d\n", r);
  (void)printf("rnd=%d\n", (int)rnd);
  (void)printf("prec=%ld\n", (long)prec);
  (void)printf("emin=%ld\n", (long)emin);
  (void)printf("emax=%ld\n", (long)emax);
  return (r ? EXIT_FAILURE : PVN_FABI(pvn_mpc_stop,PVN_MPC_STOP)());
}
#else /* !PVN_TEST */
int PVN_FABI(pvn_mpc_start,PVN_MPC_START)(mpfr_rnd_t *const rnd, mpfr_prec_t *const prec, mpfr_exp_t *const emin, mpfr_exp_t *const emax)
{
  return PVN_FABI(pvn_mpfr_start,PVN_MPFR_START)(rnd, prec, emin, emax);
}

int PVN_FABI(pvn_mpc_stop,PVN_MPC_STOP)()
{
  return PVN_FABI(pvn_mpfr_stop,PVN_MPFR_STOP)();
}
#endif /* ?PVN_TEST */

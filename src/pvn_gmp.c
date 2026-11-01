#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (1 != argc) {
    (void)fprintf(stderr, "%s takes no arguments\n", *argv);
    return EXIT_FAILURE;
  }
  mp_bitcnt_t prec = mpf_get_default_prec();
  const int r = PVN_FABI(pvn_gmp_start,PVN_GMP_START)(&prec);
  (void)printf("PVN_GMP_START=%d\n", r);
  (void)printf("prec=%llu\n", (unsigned long long)prec);
  return (r ? EXIT_FAILURE : PVN_FABI(pvn_gmp_stop,PVN_GMP_STOP)());
}
#else /* !PVN_TEST */
int PVN_FABI(pvn_gmp_start,PVN_GMP_START)(mp_bitcnt_t *const prec)
{
  if (prec) {
    if (*prec)
      mpf_set_default_prec(*prec);
    else
      *prec = mpf_get_default_prec();
  }
  return 0;
}

int PVN_FABI(pvn_gmp_stop,PVN_GMP_STOP)()
{
  return 0;
}
#endif /* ?PVN_TEST */

#include "pvn.h"

#ifdef PVN_TEST
#ifdef PVN_MPFR
int main(int argc, char *argv[])
{
  if ((argc < 3) || (argc > 4)) {
    (void)fprintf(stderr, "%s m {S|D|X} [prec]\n", *argv);
    return EXIT_FAILURE;
  }
  const size_t m = pvn_atoz(argv[1]);
  if (!m)
    return EXIT_SUCCESS;
  double e = 0.5;
  int f = 0;
  const char p = toupper(*(argv[2]));
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
  mpfr_rnd_t rnd = MPFR_RNDN;
  mpfr_prec_t prec = ((argc == 4) ? atol(argv[3]) : __MPFR_PREC_INVALID);
  mpfr_exp_t emin = __MPFR_EXP_INVALID, emax = __MPFR_EXP_INVALID;
  if (PVN_FABI(pvn_mpfr_start,PVN_MPFR_START)(&rnd, &prec, &emin, &emax))
    return EXIT_FAILURE;
  const size_t d = (size_t)floorl(log10l(m) + 1.0L);
  char
    fmtm[22] = { '\0' },
    fmtp[22] = { '\0' };
  (void)sprintf(fmtm, "ϵ%%0%zuzu-/ε=%%# .%dRe\n", d, f);
  (void)sprintf(fmtp, "ϵ%%0%zuzu+/ε=%%# .%dRe\n", d, f);
  mpfr_t em, ep, et, ed;
  (void)mpfr_init_set_d(em,-0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(ep, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(et, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(ed, e, MPFR_RNDN);
  (void)fprintf(stderr, "relative error bounds for the sequential evaluation of %c-precision ||x||_F:\n", p);
  size_t i = (size_t)1u;
  (void)mpfr_printf(fmtm, i, em);
  (void)mpfr_printf(fmtm, i, ep);
  for (++i; i <= m; ++i) {
    (void)mpfr_add_d(et, em, 2.0, MPFR_RNDN);
    (void)mpfr_mul(et, et, em, MPFR_RNDN);
    (void)mpfr_add_d(et, et, 1.0, MPFR_RNDN);
    (void)mpfr_sqrt(em, et, MPFR_RNDN);
    (void)mpfr_d_sub(et, 1.0, ed, MPFR_RNDN);
    (void)mpfr_mul(em, em, et, MPFR_RNDN);
    (void)mpfr_sub_d(em, em, 1.0, MPFR_RNDN);
    (void)mpfr_div(et, em, ed, MPFR_RNDN);
    (void)mpfr_printf(fmtm, i, et);
    (void)mpfr_add_d(et, ep, 2.0, MPFR_RNDN);
    (void)mpfr_mul(et, et, ep, MPFR_RNDN);
    (void)mpfr_add_d(et, et, 1.0, MPFR_RNDN);
    (void)mpfr_sqrt(ep, et, MPFR_RNDN);
    (void)mpfr_add_d(et, ed, 1.0, MPFR_RNDN);
    (void)mpfr_mul(ep, ep, et, MPFR_RNDN);
    (void)mpfr_sub_d(ep, ep, 1.0, MPFR_RNDN);
    (void)mpfr_div(et, ep, ed, MPFR_RNDN);
    (void)mpfr_printf(fmtp, i, et);
  }
  mpfr_clear(ed);
  mpfr_clear(et);
  mpfr_clear(ep);
  mpfr_clear(em);
  return (PVN_FABI(pvn_mpfr_stop,PVN_MPFR_STOP)() ? EXIT_FAILURE : EXIT_SUCCESS);
}
#else /* !PVN_MPFR */
int main(/* int argc, char *argv[] */)
{
  const double x[4][2] = {{-1.0, 1.0}, { 1.0,-1.0}, { 2.0, 2.0}, {-2.0,-2.0}};
  double f = 0.0;
  const int m = 4;
  const unsigned i = 2u;
  PVN_FABI(pvn_znrm2,PVN_ZNRM2)(&f, &m, &(x[0][0]), &i, &(x[0][1]), &i);
  /* expected output: 4.472136 */
  (void)printf("%#F\n", f);
  return EXIT_SUCCESS;
}
#endif /* ?PVN_MPFR */
#else /* !PVN_TEST */
void PVN_FABI(pvn_snrm2,PVN_SNRM2)(float *const f, const int *const m, const float *const x, const unsigned *const ix)
{
  PVN_ASSERT(f);
  PVN_ASSERT(m);
  PVN_ASSERT(x);
  PVN_ASSERT(ix);
  unsigned n = 0u;
  if (*m >= 0) {
    *f = 0.0f;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  const float *x_ = x;
  for (unsigned i = 0u; i < n; ++i) {
    *f = hypotf(*f, *x_);
    x_ += *ix;
  }
}

void PVN_FABI(pvn_cnrm2,PVN_CNRM2)(float *const f, const int *const m, const float *const xr, const unsigned *const ixr, const float *const xi, const unsigned *const ixi)
{
  PVN_ASSERT(f);
  PVN_ASSERT(m);
  PVN_ASSERT(xr);
  PVN_ASSERT(ixr);
  PVN_ASSERT(xi);
  PVN_ASSERT(ixi);
  unsigned n = 0u;
  if (*m >= 0) {
    *f = 0.0f;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  if (*ixi) {
    const float *xr_ = xr;
    const float *xi_ = xi;
    for (unsigned i = 0u; i < n; ++i) {
      *f = hypotf(*f, hypotf(*xr_, *xi_));
      xr_ += *ixr;
      xi_ += *ixi;
    }
  }
  else {
    const float *xr_ = xr;
    const float *xi_ = (xi + 1);
    for (unsigned i = 0u; i < n; ++i) {
      *f = hypotf(*f, hypotf(*xr_, *xi_));
      xr_ += *ixr;
      xi_ += *ixr;
    }
  }
}

void PVN_FABI(pvn_dnrm2,PVN_DNRM2)(double *const f, const int *const m, const double *const x, const unsigned *const ix)
{
  PVN_ASSERT(f);
  PVN_ASSERT(m);
  PVN_ASSERT(x);
  PVN_ASSERT(ix);
  unsigned n = 0u;
  if (*m >= 0) {
    *f = 0.0;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  const double *x_ = x;
  for (unsigned i = 0u; i < n; ++i) {
    *f = hypot(*f, *x_);
    x_ += *ix;
  }
}

void PVN_FABI(pvn_znrm2,PVN_ZNRM2)(double *const f, const int *const m, const double *const xr, const unsigned *const ixr, const double *const xi, const unsigned *const ixi)
{
  PVN_ASSERT(f);
  PVN_ASSERT(m);
  PVN_ASSERT(xr);
  PVN_ASSERT(ixr);
  PVN_ASSERT(xi);
  PVN_ASSERT(ixi);
  unsigned n = 0u;
  if (*m >= 0) {
    *f = 0.0;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  if (*ixi) {
    const double *xr_ = xr;
    const double *xi_ = xi;
    for (unsigned i = 0u; i < n; ++i) {
      *f = hypot(*f, hypot(*xr_, *xi_));
      xr_ += *ixr;
      xi_ += *ixi;
    }
  }
  else {
    const double *xr_ = xr;
    const double *xi_ = (xi + 1);
    for (unsigned i = 0u; i < n; ++i) {
      *f = hypot(*f, hypot(*xr_, *xi_));
      xr_ += *ixr;
      xi_ += *ixr;
    }
  }
}

void PVN_FABI(pvn_xnrm2,PVN_XNRM2)(long double *const f, const int *const m, const long double *const x, const unsigned *const ix)
{
  PVN_ASSERT(f);
  PVN_ASSERT(m);
  PVN_ASSERT(x);
  PVN_ASSERT(ix);
  unsigned n = 0u;
  if (*m >= 0) {
    *f = 0.0L;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  const long double *x_ = x;
  for (unsigned i = 0u; i < n; ++i) {
    *f = hypotl(*f, *x_);
    x_ += *ix;
  }
}

void PVN_FABI(pvn_wnrm2,PVN_WNRM2)(long double *const f, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi)
{
  PVN_ASSERT(f);
  PVN_ASSERT(m);
  PVN_ASSERT(xr);
  PVN_ASSERT(ixr);
  PVN_ASSERT(xi);
  PVN_ASSERT(ixi);
  unsigned n = 0u;
  if (*m >= 0) {
    *f = 0.0L;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  if (*ixi) {
    const long double *xr_ = xr;
    const long double *xi_ = xi;
    for (unsigned i = 0u; i < n; ++i) {
      *f = hypotl(*f, hypotl(*xr_, *xi_));
      xr_ += *ixr;
      xi_ += *ixi;
    }
  }
  else {
    const long double *xr_ = xr;
    const long double *xi_ = (xi + 1);
    for (unsigned i = 0u; i < n; ++i) {
      *f = hypotl(*f, hypotl(*xr_, *xi_));
      xr_ += *ixr;
      xi_ += *ixr;
    }
  }
}

#ifdef PVN_QUADMATH
void PVN_FABI(pvn_qnrm2,PVN_QNRM2)(__float128 *const f, const int *const m, const __float128 *const x, const unsigned *const ix)
{
  PVN_ASSERT(f);
  PVN_ASSERT(m);
  PVN_ASSERT(x);
  PVN_ASSERT(ix);
  unsigned n = 0u;
  if (*m >= 0) {
    *f = 0.0q;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  const __float128 *x_ = x;
  for (unsigned i = 0u; i < n; ++i) {
    *f = hypotq(*f, *x_);
    x_ += *ix;
  }
}

void PVN_FABI(pvn_ynrm2,PVN_YNRM2)(__float128 *const f, const int *const m, const __float128 *const xr, const unsigned *const ixr, const __float128 *const xi, const unsigned *const ixi)
{
  PVN_ASSERT(f);
  PVN_ASSERT(m);
  PVN_ASSERT(xr);
  PVN_ASSERT(ixr);
  PVN_ASSERT(xi);
  PVN_ASSERT(ixi);
  unsigned n = 0u;
  if (*m >= 0) {
    *f = 0.0q;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  if (*ixi) {
    const __float128 *xr_ = xr;
    const __float128 *xi_ = xi;
    for (unsigned i = 0u; i < n; ++i) {
      *f = hypotq(*f, hypotq(*xr_, *xi_));
      xr_ += *ixr;
      xi_ += *ixi;
    }
  }
  else {
    const __float128 *xr_ = xr;
    const __float128 *xi_ = (xi + 1);
    for (unsigned i = 0u; i < n; ++i) {
      *f = hypotq(*f, hypotq(*xr_, *xi_));
      xr_ += *ixr;
      xi_ += *ixr;
    }
  }
}
#else /* !PVN_QUADMATH */
void PVN_FABI(pvn_qnrm2,PVN_QNRM2)(long double *const f, const int *const m, const long double *const x, const unsigned *const ix)
{
  PVN_FABI(pvn_xnrm2,PVN_XNRM2)(f, m, x, ix);
}

void PVN_FABI(pvn_ynrm2,PVN_YNRM2)(long double *const f, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi)
{
  PVN_FABI(pvn_wnrm2,PVN_WNRM2)(f, m, xr, ixr, xi, ixi);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

#include "pvn.h"

#ifdef PVN_TEST
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

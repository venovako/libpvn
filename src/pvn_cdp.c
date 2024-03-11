#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  const double x[4][2] = {{-1.0, 1.0}, { 1.0,-1.0}, { 2.0, 2.0}, {-2.0,-2.0}};
  double zr = 0.0, zi = 0.0;
  const int m = 4;
  const unsigned i = 2u;
  pvn_zdp_(&zr, &zi, &m, &(x[0][0]), &i, &(x[0][1]), &i, &(x[0][0]), &i, &(x[0][1]), &i);
  /* expected output: ( 20.0,  0.0) */
  (void)printf("(%# 5.1F,%# 5.1F)\n", zr, zi);
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
void pvn_cdp_(float *const zr, float *const zi, const int *const m, const float *const xr, const unsigned *const ixr, const float *const xi, const unsigned *const ixi, const float *const yr, const unsigned *const iyr, const float *const yi, const unsigned *const iyi)
{
  assert(zr);
  assert(zi);
  assert(m);
  assert(xr);
  assert(ixr);
  assert(xi);
  assert(ixi);
  assert(yr);
  assert(iyr);
  assert(yi);
  assert(iyi);
  unsigned n = 0u;
  if (*m >= 0) {
    *zr = 0.0f;
    *zi = 0.0f;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  const float *xr_ = xr;
  const float *xi_ = xi;
  const float *yr_ = yr;
  const float *yi_ = yi;
  for (unsigned i = 0u; i < n; ++i) {
    pvn_cfma(zr, zi, *xr_, -*xi_, *yr_, *yi_, *zr, *zi);
    xr_ += *ixr;
    xi_ += *ixi;
    yr_ += *iyr;
    yi_ += *iyi;
  }
}

void pvn_zdp_(double *const zr, double *const zi, const int *const m, const double *const xr, const unsigned *const ixr, const double *const xi, const unsigned *const ixi, const double *const yr, const unsigned *const iyr, const double *const yi, const unsigned *const iyi)
{
  assert(zr);
  assert(zi);
  assert(m);
  assert(xr);
  assert(ixr);
  assert(xi);
  assert(ixi);
  assert(yr);
  assert(iyr);
  assert(yi);
  assert(iyi);
  unsigned n = 0u;
  if (*m >= 0) {
    *zr = 0.0;
    *zi = 0.0;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  const double *xr_ = xr;
  const double *xi_ = xi;
  const double *yr_ = yr;
  const double *yi_ = yi;
  for (unsigned i = 0u; i < n; ++i) {
    pvn_zfma(zr, zi, *xr_, -*xi_, *yr_, *yi_, *zr, *zi);
    xr_ += *ixr;
    xi_ += *ixi;
    yr_ += *iyr;
    yi_ += *iyi;
  }
}

void pvn_wdp_(long double *const zr, long double *const zi, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi, const long double *const yr, const unsigned *const iyr, const long double *const yi, const unsigned *const iyi)
{
  assert(zr);
  assert(zi);
  assert(m);
  assert(xr);
  assert(ixr);
  assert(xi);
  assert(ixi);
  assert(yr);
  assert(iyr);
  assert(yi);
  assert(iyi);
  unsigned n = 0u;
  if (*m >= 0) {
    *zr = 0.0L;
    *zi = 0.0L;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  const long double *xr_ = xr;
  const long double *xi_ = xi;
  const long double *yr_ = yr;
  const long double *yi_ = yi;
  for (unsigned i = 0u; i < n; ++i) {
    pvn_wfma(zr, zi, *xr_, -*xi_, *yr_, *yi_, *zr, *zi);
    xr_ += *ixr;
    xi_ += *ixi;
    yr_ += *iyr;
    yi_ += *iyi;
  }
}

#ifdef PVN_QUADMATH
void pvn_ydp_(__float128 *const zr, __float128 *const zi, const int *const m, const __float128 *const xr, const unsigned *const ixr, const __float128 *const xi, const unsigned *const ixi, const __float128 *const yr, const unsigned *const iyr, const __float128 *const yi, const unsigned *const iyi)
{
  assert(zr);
  assert(zi);
  assert(m);
  assert(xr);
  assert(ixr);
  assert(xi);
  assert(ixi);
  assert(yr);
  assert(iyr);
  assert(yi);
  assert(iyi);
  unsigned n = 0u;
  if (*m >= 0) {
    *zr = 0.0q;
    *zi = 0.0q;
    n = (unsigned)*m;
  }
  else
    n = (unsigned)-*m;
  if (!n)
    return;
  const __float128 *xr_ = xr;
  const __float128 *xi_ = xi;
  const __float128 *yr_ = yr;
  const __float128 *yi_ = yi;
  for (unsigned i = 0u; i < n; ++i) {
    pvn_yfma(zr, zi, *xr_, -*xi_, *yr_, *yi_, *zr, *zi);
    xr_ += *ixr;
    xi_ += *ixi;
    yr_ += *iyr;
    yi_ += *iyi;
  }
}
#else /* !PVN_QUADMATH */
void pvn_ydp_(long double *const zr, long double *const zi, const int *const m, const long double *const xr, const unsigned *const ixr, const long double *const xi, const unsigned *const ixi, const long double *const yr, const unsigned *const iyr, const long double *const yi, const unsigned *const iyi)
{
  pvn_wdp_(zr, zi, m, xr, ixr, xi, ixi, yr, iyr, yi, iyi);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

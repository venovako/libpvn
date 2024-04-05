#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
void pvn_cmul_(float *const cr, float *const ci, const float *const ar, const float *const ai, const float *const br, const float *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_cmul(cr, ci, *ar, *ai, *br, *bi);
}

void pvn_cfma_(float *const dr, float *const di, const float *const ar, const float *const ai, const float *const br, const float *const bi, const float *const cr, const float *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_cfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}

void pvn_zmul_(double *const cr, double *const ci, const double *const ar, const double *const ai, const double *const br, const double *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_zmul(cr, ci, *ar, *ai, *br, *bi);
}

void pvn_zfma_(double *const dr, double *const di, const double *const ar, const double *const ai, const double *const br, const double *const bi, const double *const cr, const double *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_zfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}

void pvn_wmul_(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_wmul(cr, ci, *ar, *ai, *br, *bi);
}

void pvn_wfma_(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_wfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}

#ifdef PVN_QUADMATH
void pvn_ymul_(__float128 *const cr, __float128 *const ci, const __float128 *const ar, const __float128 *const ai, const __float128 *const br, const __float128 *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_ymul(cr, ci, *ar, *ai, *br, *bi);
}

void pvn_yfma_(__float128 *const dr, __float128 *const di, const __float128 *const ar, const __float128 *const ai, const __float128 *const br, const __float128 *const bi, const __float128 *const cr, const __float128 *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_yfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}
#else /* !PVN_QUADMATH */
void pvn_ymul_(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_wmul(cr, ci, *ar, *ai, *br, *bi);
}

void pvn_yfma_(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_wfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

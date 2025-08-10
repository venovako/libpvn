#include "pvn.h"

#ifdef PVN_TEST
#ifdef PVN_MPFR
int main(int argc, char *argv[])
{
  if ((argc < 3) || (argc > 4)) {
    (void)fprintf(stderr, "%s {S|D|X} prec [m]\n", *argv);
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
  mpfr_rnd_t rnd = MPFR_RNDN;
  mpfr_prec_t prec = atol(argv[2]);
  mpfr_exp_t emin = __MPFR_EXP_INVALID, emax = __MPFR_EXP_INVALID;
  if (PVN_FABI(pvn_mpfr_start,PVN_MPFR_START)(&rnd, &prec, &emin, &emax))
    return EXIT_FAILURE;
  char
    fmtm[24] = { '\0' },
    fmtp[24] = { '\0' };
  size_t m = ~0ul - 1ul;
  if (argc == 4) {
    m = pvn_atoz(argv[3]);
    const size_t d = (size_t)floorl(log10l(m) + 1.0L);
    (void)sprintf(fmtm, "ϵ%%0%zuzu-/ε=%%# .%dRe\n", d, f);
    (void)sprintf(fmtp, "ϵ%%0%zuzu+/ε=%%# .%dRe\n", d, f);
  }
  mpfr_t em, ep, et, ed;
  (void)mpfr_init_set_d(em,-0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(ep, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(et, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(ed, e, MPFR_RNDN);
  size_t i = (size_t)1u;
  if (argc == 4) {
    (void)fprintf(stderr, "relative error bounds for the sequential evaluation of %c-precision ||x||_F:\n", p);
    (void)mpfr_printf(fmtm, i, em);
    (void)mpfr_printf(fmtm, i, ep);
  }
  else
    (void)fprintf(stderr, "min(i) such that ϵi+ ≥ i*ε in %c-precision: ", p);
  for (++i; i <= m; ++i) {
    (void)mpfr_add_d(et, em, 2.0, MPFR_RNDN);
    (void)mpfr_mul(et, et, em, MPFR_RNDN);
    (void)mpfr_add_d(et, et, 1.0, MPFR_RNDN);
    (void)mpfr_sqrt(em, et, MPFR_RNDN);
    (void)mpfr_d_sub(et, 1.0, ed, MPFR_RNDN);
    (void)mpfr_mul(em, em, et, MPFR_RNDN);
    (void)mpfr_sub_d(em, em, 1.0, MPFR_RNDN);
    (void)mpfr_div(et, em, ed, MPFR_RNDN);
    if (argc == 4)
      (void)mpfr_printf(fmtm, i, et);
    (void)mpfr_add_d(et, ep, 2.0, MPFR_RNDN);
    (void)mpfr_mul(et, et, ep, MPFR_RNDN);
    (void)mpfr_add_d(et, et, 1.0, MPFR_RNDN);
    (void)mpfr_sqrt(ep, et, MPFR_RNDN);
    (void)mpfr_add_d(et, ed, 1.0, MPFR_RNDN);
    (void)mpfr_mul(ep, ep, et, MPFR_RNDN);
    (void)mpfr_sub_d(ep, ep, 1.0, MPFR_RNDN);
    (void)mpfr_div(et, ep, ed, MPFR_RNDN);
    if (argc == 4)
      (void)mpfr_printf(fmtp, i, et);
    else {
      (void)mpfr_ui_sub(et, i, et, MPFR_RNDN);
      if (mpfr_sgn(et) <= 0) {
        (void)fprintf(stderr, "%zu\n", i);
        break;
      }
    }
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
  const size_t n = (size_t)4u;
  /* expected output: 4.472136 */
  (void)printf("%#F\n", PVN_FABI(pvn_znrm2,PVN_ZNRM2)(&n, &(x[0][0])));
  return EXIT_SUCCESS;
}
#endif /* ?PVN_MPFR */
#else /* !PVN_TEST */
#ifdef PVN_MPFR
float PVN_FABI(pvn_mpf_nrmf,PVN_MPF_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0f;
  const size_t m = *n;
  mpfr_t mf, mx;
  (void)mpfr_init_set_d(mf, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(mx, 0.0, MPFR_RNDN);
  for (size_t i = 0u; i < m; ++i) {
    (void)mpfr_set_flt(mx, x[i], MPFR_RNDN);
#ifdef PVN_NRM_SAFE
    (void)mpfr_hypot(mf, mf, mx, MPFR_RNDN);
#else /* !PVN_NRM_SAFE */
    (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
#endif /* ?PVN_NRM_SAFE */
  }
#ifndef PVN_NRM_SAFE
  (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
#endif /* !PVN_NRM_SAFE */
  const float f = mpfr_get_flt(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}

double PVN_FABI(pvn_mpd_nrmf,PVN_MPD_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  const size_t m = *n;
  mpfr_t mf, mx;
  (void)mpfr_init_set_d(mf, 0.0, MPFR_RNDN);
  (void)mpfr_init_set_d(mx, 0.0, MPFR_RNDN);
  for (size_t i = 0u; i < m; ++i) {
    (void)mpfr_set_d(mx, x[i], MPFR_RNDN);
#ifdef PVN_NRM_SAFE
    (void)mpfr_hypot(mf, mf, mx, MPFR_RNDN);
#else /* !PVN_NRM_SAFE */
    (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
#endif /* ?PVN_NRM_SAFE */
  }
#ifndef PVN_NRM_SAFE
  (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
#endif /* !PVN_NRM_SAFE */
  const double f = mpfr_get_d(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}

long double PVN_FABI(pvn_mpx_nrmf,PVN_MPX_NRMF)(const size_t *const n, const long double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0L;
  const size_t m = *n;
  mpfr_t mf, mx;
  (void)mpfr_init_set_ld(mf, 0.0L, MPFR_RNDN);
  (void)mpfr_init_set_ld(mx, 0.0L, MPFR_RNDN);
  for (size_t i = 0u; i < m; ++i) {
    (void)mpfr_set_ld(mx, x[i], MPFR_RNDN);
#ifdef PVN_NRM_SAFE
    (void)mpfr_hypot(mf, mf, mx, MPFR_RNDN);
#else /* !PVN_NRM_SAFE */
    (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
#endif /* ?PVN_NRM_SAFE */
  }
#ifndef PVN_NRM_SAFE
  (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
#endif /* !PVN_NRM_SAFE */
  const long double f = mpfr_get_ld(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}
#endif /* PVN_MPFR */

#ifdef PVN_LAPACK
extern float PVN_FABI(snrm2,SNRM2)(const size_t *const n, const float *const x, const int64_t *const incx);
extern double PVN_FABI(dnrm2,DNRM2)(const size_t *const n, const double *const x, const int64_t *const incx);

float PVN_FABI(pvn_las_nrmf,PVN_LAS_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0f;
  const int64_t incx = INT64_C(1);
  return PVN_FABI(snrm2,SNRM2)(n, x, &incx);
}

double PVN_FABI(pvn_lad_nrmf,PVN_LAD_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  const int64_t incx = INT64_C(1);
  return PVN_FABI(dnrm2,DNRM2)(n, x, &incx);
}
#endif /* PVN_LAPACK */

float PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(const size_t *const n, const float *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0f;
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return hypotf(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const float fl = PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&nl, x);
  const float fr = PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(&nr, (x + nl));
  return hypotf(fl, fr);
}

double PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(const size_t *const n, const double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return hypot(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const double fl = PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&nl, x);
  const double fr = PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(&nr, (x + nl));
  return hypot(fl, fr);
}

long double PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(const size_t *const n, const long double *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0L;
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return hypotl(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const long double fl = PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&nl, x);
  const long double fr = PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(&nr, (x + nl));
  return hypotl(fl, fr);
}

float PVN_FABI(pvn_crs_nrmf,PVN_CRS_NRMF)(const size_t *const n, const float *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(hcf:float:omp_out=hypotf(omp_out,omp_in)) initializer(omp_priv=0.0f)
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0f;
  if (*n == (size_t)1u)
    return __builtin_fabsf(*x);
  if (*n == (size_t)2u)
    return hypotf(x[0], x[1]);
  const size_t m = *n;
  /* the three Blue's accumulators as in SNRM2, plus the accumulator for subnormal inputs */
  const float tsml = __builtin_scalbnf(1.0f, -63);
  const float tbig = __builtin_scalbnf(1.0f,  52);
  float dnr = 0.0f, sml = 0.0f, med = 0.0f, big = 0.0f;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcf:dnr,sml,med,big)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const float y = __builtin_fabsf(x[i]);
    if (y > 0.0f) {
      if (y < FLT_MIN)
        dnr = hypotf(dnr, __builtin_scalbnf(y, 23));
      else if (y < tsml)
        sml = hypotf(sml, y);
      else if (y > tbig)
        big = hypotf(big, y);
      else /* med */
        med = hypotf(med, y);
    }
  }
  if (dnr > 0.0f)
    sml = hypotf(sml, __builtin_scalbnf(dnr, -23));
  if (sml > 0.0f)
    med = hypotf(med, sml);
  if (med > 0.0f)
    big = hypotf(big, med);
  return big;
}

double PVN_FABI(pvn_crd_nrmf,PVN_CRD_NRMF)(const size_t *const n, const double *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(hcd:double:omp_out=hypot(omp_out,omp_in)) initializer(omp_priv=0.0)
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0;
  if (*n == (size_t)1u)
    return __builtin_fabs(*x);
  if (*n == (size_t)2u)
    return hypot(x[0], x[1]);
  const size_t m = *n;
  /* the three Blue's accumulators as in DNRM2, plus the accumulator for subnormal inputs */
  const double tsml = __builtin_scalbn(1.0, -511);
  const double tbig = __builtin_scalbn(1.0,  486);
  double dnr = 0.0, sml = 0.0, med = 0.0, big = 0.0;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcd:dnr,sml,med,big)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const double y = __builtin_fabs(x[i]);
    if (y > 0.0) {
      if (y < DBL_MIN)
        dnr = hypot(dnr, __builtin_scalbn(y, 52));
      else if (y < tsml)
        sml = hypot(sml, y);
      else if (y > tbig)
        big = hypot(big, y);
      else /* med */
        med = hypot(med, y);
    }
  }
  if (dnr > 0.0)
    sml = hypot(sml, __builtin_scalbn(dnr, -52));
  if (sml > 0.0)
    med = hypot(med, sml);
  if (med > 0.0)
    big = hypot(big, med);
  return big;
}

long double PVN_FABI(pvn_crx_nrmf,PVN_CRX_NRMF)(const size_t *const n, const long double *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(hcx:long double:omp_out=hypotl(omp_out,omp_in)) initializer(omp_priv=0.0L)
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0L;
  if (*n == (size_t)1u)
    return __builtin_fabsl(*x);
  if (*n == (size_t)2u)
    return hypotl(x[0], x[1]);
  const size_t m = *n;
  /* the three Blue's accumulators, plus the accumulator for subnormal inputs */
  const long double tsml = __builtin_scalbnl(1.0L, -8191);
  const long double tbig = __builtin_scalbnl(1.0L,  8160);
  long double dnr = 0.0L, sml = 0.0L, med = 0.0L, big = 0.0L;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcx:dnr,sml,med,big)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const long double y = __builtin_fabsl(x[i]);
    if (y > 0.0L) {
      if (y < LDBL_MIN)
        dnr = hypotl(dnr, __builtin_scalbnl(y, 63));
      else if (y < tsml)
        sml = hypotl(sml, y);
      else if (y > tbig)
        big = hypotl(big, y);
      else /* med */
        med = hypotl(med, y);
    }
  }
  if (dnr > 0.0L)
    sml = hypotl(sml, __builtin_scalbnl(dnr, -63));
  if (sml > 0.0L)
    med = hypotl(med, sml);
  if (med > 0.0L)
    big = hypotl(big, med);
  return big;
}

float PVN_FABI(pvn_snrm2,PVN_SNRM2)(const size_t *const n, const float *const x)
{
  return PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(n, x);
}
float PVN_FABI(pvn_cnrm2,PVN_CNRM2)(const size_t *const n, const float *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_snrm2,PVN_SNRM2)(&m, x);
}

double PVN_FABI(pvn_dnrm2,PVN_DNRM2)(const size_t *const n, const double *const x)
{
  return PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(n, x);
}
double PVN_FABI(pvn_znrm2,PVN_ZNRM2)(const size_t *const n, const double *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_dnrm2,PVN_DNRM2)(&m, x);
}

long double PVN_FABI(pvn_xnrm2,PVN_XNRM2)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(n, x);
}
long double PVN_FABI(pvn_wnrm2,PVN_WNRM2)(const size_t *const n, const long double *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_xnrm2,PVN_XNRM2)(&m, x);
}

#ifdef PVN_QUADMATH
#ifdef PVN_MPFR
__float128 PVN_FABI(pvn_mpq_nrmf,PVN_MPQ_NRMF)(const size_t *const n, const __float128 *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0q;
  const size_t m = *n;
  mpfr_t mf, mx;
  (void)mpfr_init_set_ld(mf, 0.0L, MPFR_RNDN);
  (void)mpfr_init_set_ld(mx, 0.0L, MPFR_RNDN);
  for (size_t i = 0u; i < m; ++i) {
    (void)mpfr_set_float128(mx, x[i], MPFR_RNDN);
#ifdef PVN_NRM_SAFE
    (void)mpfr_hypot(mf, mf, mx, MPFR_RNDN);
#else /* !PVN_NRM_SAFE */
    (void)mpfr_fma(mf, mx, mx, mf, MPFR_RNDN);
#endif /* ?PVN_NRM_SAFE */
  }
#ifndef PVN_NRM_SAFE
  (void)mpfr_sqrt(mf, mf, MPFR_RNDN);
#endif /* !PVN_NRM_SAFE */
  const __float128 f = mpfr_get_float128(mf, MPFR_RNDN);
  mpfr_clear(mx);
  mpfr_clear(mf);
  return f;
}
#endif /* PVN_MPFR */

__float128 PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(const size_t *const n, const __float128 *const x)
{
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0q;
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
    return hypotq(x[0], x[1]);
  const size_t nl = ((*n >> 1u) + (*n & (size_t)1u));
  const size_t nr = (*n - nl);
  const __float128 fl = PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&nl, x);
  const __float128 fr = PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(&nr, (x + nl));
  return hypotq(fl, fr);
}

__float128 PVN_FABI(pvn_crq_nrmf,PVN_CRQ_NRMF)(const size_t *const n, const __float128 *const x)
{
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp declare reduction(hcq:__float128:omp_out=hypotq(omp_out,omp_in)) initializer(omp_priv=0.0q)
#endif /* _OPENMP */
  PVN_ASSERT(n);
  PVN_ASSERT(x);
  if (!*n)
    return -0.0q;
  if (*n == (size_t)1u)
    return fabsq(*x);
  if (*n == (size_t)2u)
    return hypotq(x[0], x[1]);
  const size_t m = *n;
  /* the three Blue's accumulators, plus the accumulator for subnormal inputs */
  const __float128 tsml = scalbnq(1.0L, -8191);
  const __float128 tbig = scalbnq(1.0L,  8136);
  __float128 dnr = 0.0q, sml = 0.0q, med = 0.0q, big = 0.0q;
#if (defined(_OPENMP) && !(defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER)))
#pragma omp parallel for default(none) shared(m,x,tsml,tbig) reduction(hcq:dnr,sml,med,big)
#endif /* _OPENMP */
  for (size_t i = 0u; i < m; ++i) {
    const __float128 y = fabsq(x[i]);
    if (y > 0.0q) {
      if (y < FLT128_MIN)
        dnr = hypotq(dnr, scalbnq(y, 112));
      else if (y < tsml)
        sml = hypotq(sml, y);
      else if (y > tbig)
        big = hypotq(big, y);
      else /* med */
        med = hypotq(med, y);
    }
  }
  if (dnr > 0.0q)
    sml = hypotq(sml, scalbnq(dnr, -112));
  if (sml > 0.0q)
    med = hypotq(med, sml);
  if (med > 0.0q)
    big = hypotq(big, med);
  return big;
}

__float128 PVN_FABI(pvn_qnrm2,PVN_QNRM2)(const size_t *const n, const __float128 *const x)
{
  return PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(n, x);
}
__float128 PVN_FABI(pvn_ynrm2,PVN_YNRM2)(const size_t *const n, const __float128 *const x)
{
  const size_t m = (n ? (*n << 1u) : (size_t)0u);
  return PVN_FABI(pvn_qnrm2,PVN_QNRM2)(&m, x);
}
#else /* !PVN_QUADMATH */
#ifdef PVN_MPFR
long double PVN_FABI(pvn_mpq_nrmf,PVN_MPQ_NRMF)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_mpx_nrmf,PVN_MPX_NRMF)(n, x);
}
#endif /* PVN_MPFR */
long double PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(n, x);
}
long double PVN_FABI(pvn_crq_nrmf,PVN_CRQ_NRMF)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_crx_nrmf,PVN_CRX_NRMF)(n, x);
}
long double PVN_FABI(pvn_qnrm2,PVN_QNRM2)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_xnrm2,PVN_XNRM2)(n, x);
}
long double PVN_FABI(pvn_ynrm2,PVN_YNRM2)(const size_t *const n, const long double *const x)
{
  return PVN_FABI(pvn_wnrm2,PVN_WNRM2)(n, x);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

#ifndef PVN_MM2_H
#define PVN_MM2_H

#ifndef PVN_H
#error pvn_mm2.h not intended for direct inclusion
#endif /* !PVN_H */

/* D = A * B + C */

static inline void pvn_cfma(float *const dr, float *const di, const float ar, const float ai, const float br, const float bi, const float cr, const float ci)
{
  assert(dr);
  assert(di);
  *dr = fmaf(ar, br, fmaf(-ai, bi, cr));
  *di = fmaf(ar, bi, fmaf( ai, br, ci));
}

static inline void pvn_zfma(double *const dr, double *const di, const double ar, const double ai, const double br, const double bi, const double cr, const double ci)
{
  assert(dr);
  assert(di);
  *dr = fma(ar, br, fma(-ai, bi, cr));
  *di = fma(ar, bi, fma( ai, br, ci));
}

static inline void pvn_wfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci)
{
  assert(dr);
  assert(di);
  *dr = fmal(ar, br, fmal(-ai, bi, cr));
  *di = fmal(ar, bi, fmal( ai, br, ci));
}

#ifdef PVN_QUADMATH
static inline void pvn_yfma(__float128 *const dr, __float128 *const di, const __float128 ar, const __float128 ai, const __float128 br, const __float128 bi, const __float128 cr, const __float128 ci)
{
  assert(dr);
  assert(di);
  *dr = fmaq(ar, br, fmaq(-ai, bi, cr));
  *di = fmaq(ar, bi, fmaq( ai, br, ci));
}
#endif /* PVN_QUADMATH */

/* C += A * B */

static inline void pvn_smm2(float *const c11, float *const c21, float *const c12, float *const c22, const float a11, const float a21, const float a12, const float a22, const float b11, const float b21, const float b12, const float b22)
{
  assert(c11);
  assert(c21);
  assert(c12);
  assert(c22);
  *c11 = fmaf(a11, b11, fmaf(a12, b21, *c11));
  *c21 = fmaf(a21, b11, fmaf(a22, b21, *c21));
  *c12 = fmaf(a11, b12, fmaf(a12, b22, *c12));
  *c22 = fmaf(a21, b12, fmaf(a22, b22, *c22));
}

static inline void pvn_cmm2(float *const c11r, float *const c11i, float *const c21r, float *const c21i, float *const c12r, float *const c12i, float *const c22r, float *const c22i, const float a11r, const float a11i, const float a21r, const float a21i, const float a12r, const float a12i, const float a22r, const float a22i, const float b11r, const float b11i, const float b21r, const float b21i, const float b12r, const float b12i, const float b22r, const float b22i)
{
  assert(c11r);
  assert(c11i);
  assert(c21r);
  assert(c21i);
  assert(c12r);
  assert(c12i);
  assert(c22r);
  assert(c22i);
  pvn_cfma(c11r, c11i, a12r, a12i, b21r, b21i, *c11r, *c11i);
  pvn_cfma(c11r, c11i, a11r, a11i, b11r, b11i, *c11r, *c11i);
  pvn_cfma(c21r, c21i, a22r, a22i, b21r, b21i, *c21r, *c21i);
  pvn_cfma(c21r, c21i, a21r, a21i, b11r, b11i, *c21r, *c21i);
  pvn_cfma(c12r, c12i, a12r, a12i, b22r, b22i, *c12r, *c12i);
  pvn_cfma(c12r, c12i, a11r, a11i, b12r, b12i, *c12r, *c12i);
  pvn_cfma(c22r, c22i, a22r, a22i, b22r, b22i, *c22r, *c22i);
  pvn_cfma(c22r, c22i, a21r, a21i, b12r, b12i, *c22r, *c22i);
}

static inline void pvn_dmm2(double *const c11, double *const c21, double *const c12, double *const c22, const double a11, const double a21, const double a12, const double a22, const double b11, const double b21, const double b12, const double b22)
{
  assert(c11);
  assert(c21);
  assert(c12);
  assert(c22);
  *c11 = fma(a11, b11, fma(a12, b21, *c11));
  *c21 = fma(a21, b11, fma(a22, b21, *c21));
  *c12 = fma(a11, b12, fma(a12, b22, *c12));
  *c22 = fma(a21, b12, fma(a22, b22, *c22));
}

static inline void pvn_zmm2(double *const c11r, double *const c11i, double *const c21r, double *const c21i, double *const c12r, double *const c12i, double *const c22r, double *const c22i, const double a11r, const double a11i, const double a21r, const double a21i, const double a12r, const double a12i, const double a22r, const double a22i, const double b11r, const double b11i, const double b21r, const double b21i, const double b12r, const double b12i, const double b22r, const double b22i)
{
  assert(c11r);
  assert(c11i);
  assert(c21r);
  assert(c21i);
  assert(c12r);
  assert(c12i);
  assert(c22r);
  assert(c22i);
  pvn_zfma(c11r, c11i, a12r, a12i, b21r, b21i, *c11r, *c11i);
  pvn_zfma(c11r, c11i, a11r, a11i, b11r, b11i, *c11r, *c11i);
  pvn_zfma(c21r, c21i, a22r, a22i, b21r, b21i, *c21r, *c21i);
  pvn_zfma(c21r, c21i, a21r, a21i, b11r, b11i, *c21r, *c21i);
  pvn_zfma(c12r, c12i, a12r, a12i, b22r, b22i, *c12r, *c12i);
  pvn_zfma(c12r, c12i, a11r, a11i, b12r, b12i, *c12r, *c12i);
  pvn_zfma(c22r, c22i, a22r, a22i, b22r, b22i, *c22r, *c22i);
  pvn_zfma(c22r, c22i, a21r, a21i, b12r, b12i, *c22r, *c22i);
}

static inline void pvn_xmm2(long double *const c11, long double *const c21, long double *const c12, long double *const c22, const long double a11, const long double a21, const long double a12, const long double a22, const long double b11, const long double b21, const long double b12, const long double b22)
{
  assert(c11);
  assert(c21);
  assert(c12);
  assert(c22);
  *c11 = fmal(a11, b11, fmal(a12, b21, *c11));
  *c21 = fmal(a21, b11, fmal(a22, b21, *c21));
  *c12 = fmal(a11, b12, fmal(a12, b22, *c12));
  *c22 = fmal(a21, b12, fmal(a22, b22, *c22));
}

static inline void pvn_wmm2(long double *const c11r, long double *const c11i, long double *const c21r, long double *const c21i, long double *const c12r, long double *const c12i, long double *const c22r, long double *const c22i, const long double a11r, const long double a11i, const long double a21r, const long double a21i, const long double a12r, const long double a12i, const long double a22r, const long double a22i, const long double b11r, const long double b11i, const long double b21r, const long double b21i, const long double b12r, const long double b12i, const long double b22r, const long double b22i)
{
  assert(c11r);
  assert(c11i);
  assert(c21r);
  assert(c21i);
  assert(c12r);
  assert(c12i);
  assert(c22r);
  assert(c22i);
  pvn_wfma(c11r, c11i, a12r, a12i, b21r, b21i, *c11r, *c11i);
  pvn_wfma(c11r, c11i, a11r, a11i, b11r, b11i, *c11r, *c11i);
  pvn_wfma(c21r, c21i, a22r, a22i, b21r, b21i, *c21r, *c21i);
  pvn_wfma(c21r, c21i, a21r, a21i, b11r, b11i, *c21r, *c21i);
  pvn_wfma(c12r, c12i, a12r, a12i, b22r, b22i, *c12r, *c12i);
  pvn_wfma(c12r, c12i, a11r, a11i, b12r, b12i, *c12r, *c12i);
  pvn_wfma(c22r, c22i, a22r, a22i, b22r, b22i, *c22r, *c22i);
  pvn_wfma(c22r, c22i, a21r, a21i, b12r, b12i, *c22r, *c22i);
}

#ifdef PVN_QUADMATH
static inline void pvn_qmm2(__float128 *const c11, __float128 *const c21, __float128 *const c12, __float128 *const c22, const __float128 a11, const __float128 a21, const __float128 a12, const __float128 a22, const __float128 b11, const __float128 b21, const __float128 b12, const __float128 b22)
{
  assert(c11);
  assert(c21);
  assert(c12);
  assert(c22);
  *c11 = fmaq(a11, b11, fmaq(a12, b21, *c11));
  *c21 = fmaq(a21, b11, fmaq(a22, b21, *c21));
  *c12 = fmaq(a11, b12, fmaq(a12, b22, *c12));
  *c22 = fmaq(a21, b12, fmaq(a22, b22, *c22));
}

static inline void pvn_ymm2(__float128 *const c11r, __float128 *const c11i, __float128 *const c21r, __float128 *const c21i, __float128 *const c12r, __float128 *const c12i, __float128 *const c22r, __float128 *const c22i, const __float128 a11r, const __float128 a11i, const __float128 a21r, const __float128 a21i, const __float128 a12r, const __float128 a12i, const __float128 a22r, const __float128 a22i, const __float128 b11r, const __float128 b11i, const __float128 b21r, const __float128 b21i, const __float128 b12r, const __float128 b12i, const __float128 b22r, const __float128 b22i)
{
  assert(c11r);
  assert(c11i);
  assert(c21r);
  assert(c21i);
  assert(c12r);
  assert(c12i);
  assert(c22r);
  assert(c22i);
  pvn_yfma(c11r, c11i, a12r, a12i, b21r, b21i, *c11r, *c11i);
  pvn_yfma(c11r, c11i, a11r, a11i, b11r, b11i, *c11r, *c11i);
  pvn_yfma(c21r, c21i, a22r, a22i, b21r, b21i, *c21r, *c21i);
  pvn_yfma(c21r, c21i, a21r, a21i, b11r, b11i, *c21r, *c21i);
  pvn_yfma(c12r, c12i, a12r, a12i, b22r, b22i, *c12r, *c12i);
  pvn_yfma(c12r, c12i, a11r, a11i, b12r, b12i, *c12r, *c12i);
  pvn_yfma(c22r, c22i, a22r, a22i, b22r, b22i, *c22r, *c22i);
  pvn_yfma(c22r, c22i, a21r, a21i, b12r, b12i, *c22r, *c22i);
}
#endif /* PVN_QUADMATH */
#endif /* !PVN_MM2_H */

#ifndef PVN_SV2_H
#define PVN_SV2_H

#ifndef PVN_H
#error pvn_sv2.h not intended for direct inclusion
#endif /* !PVN_H */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* ! THESE FUNCTIONS ARE FAIL-SAFE ONLY WHEN ROUNDING TO NEAREST, ! */
/* !     AND IF THE CORRECTLY ROUNDED ARITHMETIC IS AVAILABLE     ! */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

PVN_EXTERN_C int pvn_sljsv2_
(const float *const a11, const float *const a21, const float *const a12, const float *const a22,
 float *const u11, float *const u21, float *const u12, float *const u22,
 float *const v11, float *const v21, float *const v12, float *const v22,
 float *const s1, float *const s2, int *const es);

PVN_EXTERN_C int pvn_cljsv2_
(const float *const a11r, const float *const a11i, const float *const a21r, const float *const a21i, const float *const a12r, const float *const a12i, const float *const a22r, const float *const a22i,
 float *const u11r, float *const u11i, float *const u21r, float *const u21i, float *const u12r, float *const u12i, float *const u22r, float *const u22i,
 float *const v11r, float *const v11i, float *const v21r, float *const v21i, float *const v12r, float *const v12i, float *const v22r, float *const v22i,
 float *const s1, float *const s2, int *const es);

PVN_EXTERN_C int pvn_dljsv2_
(const double *const a11, const double *const a21, const double *const a12, const double *const a22,
 double *const u11, double *const u21, double *const u12, double *const u22,
 double *const v11, double *const v21, double *const v12, double *const v22,
 double *const s1, double *const s2, int *const es);

PVN_EXTERN_C int pvn_zljsv2_
(const double *const a11r, const double *const a11i, const double *const a21r, const double *const a21i, const double *const a12r, const double *const a12i, const double *const a22r, const double *const a22i,
 double *const u11r, double *const u11i, double *const u21r, double *const u21i, double *const u12r, double *const u12i, double *const u22r, double *const u22i,
 double *const v11r, double *const v11i, double *const v21r, double *const v21i, double *const v12r, double *const v12i, double *const v22r, double *const v22i,
 double *const s1, double *const s2, int *const es);

/* at present, the functions declared below might not be fail-safe */

PVN_EXTERN_C void pvn_sxljr2_
(const float *const a11, const float *const a21, const float *const a12, const float *const a22,
 const float *const u11, const float *const u21, const float *const u12, const float *const u22,
 const float *const v11, const float *const v21, const float *const v12, const float *const v22,
 const float *const s1, const float *const s2, const int *const es, long double *const E);

PVN_EXTERN_C void pvn_dxljr2_
(const double *const a11, const double *const a21, const double *const a12, const double *const a22,
 const double *const u11, const double *const u21, const double *const u12, const double *const u22,
 const double *const v11, const double *const v21, const double *const v12, const double *const v22,
 const double *const s1, const double *const s2, const int *const es, long double *const E);

PVN_EXTERN_C void pvn_cwljr2_
(const float *const a11r, const float *const a11i, const float *const a21r, const float *const a21i,
 const float *const a12r, const float *const a12i, const float *const a22r, const float *const a22i,
 const float *const u11r, const float *const u11i, const float *const u21r, const float *const u21i,
 const float *const u12r, const float *const u12i, const float *const u22r, const float *const u22i,
 const float *const v11r, const float *const v11i, const float *const v21r, const float *const v21i,
 const float *const v12r, const float *const v12i, const float *const v22r, const float *const v22i,
 const float *const s1, const float *const s2, const int *const es, long double *const E);

PVN_EXTERN_C void pvn_zwljr2_
(const double *const a11r, const double *const a11i, const double *const a21r, const double *const a21i,
 const double *const a12r, const double *const a12i, const double *const a22r, const double *const a22i,
 const double *const u11r, const double *const u11i, const double *const u21r, const double *const u21i,
 const double *const u12r, const double *const u12i, const double *const u22r, const double *const u22i,
 const double *const v11r, const double *const v11i, const double *const v21r, const double *const v21i,
 const double *const v12r, const double *const v12i, const double *const v22r, const double *const v22i,
 const double *const s1, const double *const s2, const int *const es, long double *const E);

PVN_EXTERN_C int pvn_xljsv2_
(const long double *const a11, const long double *const a21, const long double *const a12, const long double *const a22,
 long double *const u11, long double *const u21, long double *const u12, long double *const u22,
 long double *const v11, long double *const v21, long double *const v12, long double *const v22,
 long double *const s1, long double *const s2, int *const es);

PVN_EXTERN_C int pvn_wljsv2_
(const long double *const a11r, const long double *const a11i, const long double *const a21r, const long double *const a21i, const long double *const a12r, const long double *const a12i, const long double *const a22r, const long double *const a22i,
 long double *const u11r, long double *const u11i, long double *const u21r, long double *const u21i, long double *const u12r, long double *const u12i, long double *const u22r, long double *const u22i,
 long double *const v11r, long double *const v11i, long double *const v21r, long double *const v21i, long double *const v12r, long double *const v12i, long double *const v22r, long double *const v22i,
 long double *const s1, long double *const s2, int *const es);
#ifdef PVN_QUADMATH
PVN_EXTERN_C void pvn_sqljr2_
(const float *const a11, const float *const a21, const float *const a12, const float *const a22,
 const float *const u11, const float *const u21, const float *const u12, const float *const u22,
 const float *const v11, const float *const v21, const float *const v12, const float *const v22,
 const float *const s1, const float *const s2, const int *const es, __float128 *const E);

PVN_EXTERN_C void pvn_dqljr2_
(const double *const a11, const double *const a21, const double *const a12, const double *const a22,
 const double *const u11, const double *const u21, const double *const u12, const double *const u22,
 const double *const v11, const double *const v21, const double *const v12, const double *const v22,
 const double *const s1, const double *const s2, const int *const es, __float128 *const E);

PVN_EXTERN_C void pvn_xqljr2_
(const long double *const a11, const long double *const a21, const long double *const a12, const long double *const a22,
 const long double *const u11, const long double *const u21, const long double *const u12, const long double *const u22,
 const long double *const v11, const long double *const v21, const long double *const v12, const long double *const v22,
 const long double *const s1, const long double *const s2, const int *const es, __float128 *const E);

PVN_EXTERN_C int pvn_qljsv2_
(const __float128 *const a11, const __float128 *const a21, const __float128 *const a12, const __float128 *const a22,
 __float128 *const u11, __float128 *const u21, __float128 *const u12, __float128 *const u22,
 __float128 *const v11, __float128 *const v21, __float128 *const v12, __float128 *const v22,
 __float128 *const s1, __float128 *const s2, int *const es);

PVN_EXTERN_C int pvn_yljsv2_
(const __float128 *const a11r, const __float128 *const a11i, const __float128 *const a21r, const __float128 *const a21i, const __float128 *const a12r, const __float128 *const a12i, const __float128 *const a22r, const __float128 *const a22i,
 __float128 *const u11r, __float128 *const u11i, __float128 *const u21r, __float128 *const u21i, __float128 *const u12r, __float128 *const u12i, __float128 *const u22r, __float128 *const u22i,
 __float128 *const v11r, __float128 *const v11i, __float128 *const v21r, __float128 *const v21i, __float128 *const v12r, __float128 *const v12i, __float128 *const v22r, __float128 *const v22i,
 __float128 *const s1, __float128 *const s2, int *const es);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C void pvn_sqljr2_
(const float *const a11, const float *const a21, const float *const a12, const float *const a22,
 const float *const u11, const float *const u21, const float *const u12, const float *const u22,
 const float *const v11, const float *const v21, const float *const v12, const float *const v22,
 const float *const s1, const float *const s2, const int *const es, long double *const E);

PVN_EXTERN_C void pvn_dqljr2_
(const double *const a11, const double *const a21, const double *const a12, const double *const a22,
 const double *const u11, const double *const u21, const double *const u12, const double *const u22,
 const double *const v11, const double *const v21, const double *const v12, const double *const v22,
 const double *const s1, const double *const s2, const int *const es, long double *const E);

PVN_EXTERN_C void pvn_xqljr2_
(const long double *const a11, const long double *const a21, const long double *const a12, const long double *const a22,
 const long double *const u11, const long double *const u21, const long double *const u12, const long double *const u22,
 const long double *const v11, const long double *const v21, const long double *const v12, const long double *const v22,
 const long double *const s1, const long double *const s2, const int *const es, long double *const E);

PVN_EXTERN_C int pvn_qljsv2_
(const long double *const a11, const long double *const a21, const long double *const a12, const long double *const a22,
 long double *const u11, long double *const u21, long double *const u12, long double *const u22,
 long double *const v11, long double *const v21, long double *const v12, long double *const v22,
 long double *const s1, long double *const s2, int *const es);

PVN_EXTERN_C int pvn_yljsv2_
(const long double *const a11r, const long double *const a11i, const long double *const a21r, const long double *const a21i, const long double *const a12r, const long double *const a12i, const long double *const a22r, const long double *const a22i,
 long double *const u11r, long double *const u11i, long double *const u21r, long double *const u21i, long double *const u12r, long double *const u12i, long double *const u22r, long double *const u22i,
 long double *const v11r, long double *const v11i, long double *const v21r, long double *const v21i, long double *const v12r, long double *const v12i, long double *const v22r, long double *const v22i,
 long double *const s1, long double *const s2, int *const es);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_SV2_H */

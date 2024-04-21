#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if ((argc == 5) || (argc == 7)) {
    const double ar = atof(argv[1]);
    const double ai = atof(argv[2]);
    const double br = atof(argv[3]);
    const double bi = atof(argv[4]);
    char s[26] = { '\0' };
    if (argc == 7) {
      const double cr = atof(argv[5]);
      const double ci = atof(argv[6]);
      double dr = 0.0, di = 0.0;
      pvn_zfma(&dr, &di, ar, ai, br, bi, cr, ci);
#ifdef PVN_CMA_SAFE
      (void)printf("PVN_ZFMA=(%s,", pvn_dtoa(s, dr));
#else /* !PVN_CMA_SAFE */
      (void)printf("pvn_zfma=(%s,", pvn_dtoa(s, dr));
#endif /* ?PVN_CMA_SAFE */
      (void)printf("%s)\n", pvn_dtoa(s, di));
    }
    else {
      double cr = 0.0, ci = 0.0;
      pvn_zmul(&cr, &ci, ar, ai, br, bi);
#ifdef PVN_CMA_SAFE
      (void)printf("PVN_ZMUL=(%s,", pvn_dtoa(s, cr));
#else /* !PVN_CMA_SAFE */
      (void)printf("pvn_zmul=(%s,", pvn_dtoa(s, cr));
#endif /* ?PVN_CMA_SAFE */
      (void)printf("%s)\n", pvn_dtoa(s, ci));
    }
  }
  else {
    (void)fprintf(stderr, "%s ar ai br bi [cr ci]\n", *argv);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#ifdef PVN_CMA_SAFE
#ifdef t_ab_cd
#error t_ab_cd already defined
#else /* !t_ab_cd */
#define t_ab_cd(t,T,TYP)                                                                                                                            \
static T t##_ab_cd(const int arbre, const int are, const T arf, const int bre, const T brf, const int aie, const T aif, const int bie, const T bif) \
{                                                                                                                                                   \
  const int s = (TYP##_MAX_EXP - arbre - 1);                                                                                                        \
  int ars = are, brs = bre, ais = aie, bis = bie;                                                                                                   \
  if (s > 0) {                                                                                                                                      \
    if (are >= bre) {                                                                                                                               \
      brs = (bre + s);                                                                                                                              \
      ars = (brs - TYP##_MAX_EXP);                                                                                                                  \
      if (ars > 0) {                                                                                                                                \
        brs -= ars;                                                                                                                                 \
        ars += are;                                                                                                                                 \
      }                                                                                                                                             \
      else                                                                                                                                          \
        ars = are;                                                                                                                                  \
    }                                                                                                                                               \
    else {                                                                                                                                          \
      ars = (are + s);                                                                                                                              \
      brs = (ars - TYP##_MAX_EXP);                                                                                                                  \
      if (brs > 0) {                                                                                                                                \
        ars -= brs;                                                                                                                                 \
        brs += bre;                                                                                                                                 \
      }                                                                                                                                             \
      else                                                                                                                                          \
        brs = bre;                                                                                                                                  \
    }                                                                                                                                               \
    if (aie >= bie) {                                                                                                                               \
      bis = (bie + s);                                                                                                                              \
      ais = (bis - TYP##_MAX_EXP);                                                                                                                  \
      if (ais > 0) {                                                                                                                                \
        bis -= ais;                                                                                                                                 \
        ais += aie;                                                                                                                                 \
      }                                                                                                                                             \
      else                                                                                                                                          \
        ais = aie;                                                                                                                                  \
    }                                                                                                                                               \
    else {                                                                                                                                          \
      ais = (aie + s);                                                                                                                              \
      bis = (ais - TYP##_MAX_EXP);                                                                                                                  \
      if (bis > 0) {                                                                                                                                \
        ais -= bis;                                                                                                                                 \
        bis += bie;                                                                                                                                 \
      }                                                                                                                                             \
      else                                                                                                                                          \
        bis = bie;                                                                                                                                  \
    }                                                                                                                                               \
  }                                                                                                                                                 \
  else if (s < 0) {                                                                                                                                 \
    if (are >= bre) {                                                                                                                               \
      ars = (are + s);                                                                                                                              \
      brs = (ars - TYP##_MIN_EXP);                                                                                                                  \
      if (brs < 0) {                                                                                                                                \
        ars -= brs;                                                                                                                                 \
        brs += bre;                                                                                                                                 \
      }                                                                                                                                             \
      else                                                                                                                                          \
        brs = bre;                                                                                                                                  \
    }                                                                                                                                               \
    else {                                                                                                                                          \
      brs = (bre + s);                                                                                                                              \
      ars = (brs - TYP##_MIN_EXP);                                                                                                                  \
      if (ars < 0) {                                                                                                                                \
        brs -= ars;                                                                                                                                 \
        ars += are;                                                                                                                                 \
      }                                                                                                                                             \
      else                                                                                                                                          \
        ars = are;                                                                                                                                  \
    }                                                                                                                                               \
    if (aie >= bie) {                                                                                                                               \
      ais = (aie + s);                                                                                                                              \
      bis = (ais - TYP##_MIN_EXP);                                                                                                                  \
      if (bis < 0) {                                                                                                                                \
        ais -= bis;                                                                                                                                 \
        bis += bie;                                                                                                                                 \
      }                                                                                                                                             \
      else                                                                                                                                          \
        bis = bie;                                                                                                                                  \
    }                                                                                                                                               \
    else {                                                                                                                                          \
      bis = (bie + s);                                                                                                                              \
      ais = (bis - TYP##_MIN_EXP);                                                                                                                  \
      if (ais < 0) {                                                                                                                                \
        bis -= ais;                                                                                                                                 \
        ais += aie;                                                                                                                                 \
      }                                                                                                                                             \
      else                                                                                                                                          \
        ais = aie;                                                                                                                                  \
    }                                                                                                                                               \
  }                                                                                                                                                 \
  const T ret = fma##t(scalbn##t(arf, ars), scalbn##t(brf, brs), scalbn##t(aif, ais) * scalbn##t(bif, bis));                                        \
  return (s ? scalbn##t(ret, -s) : ret);                                                                                                            \
}
#endif /* ?t_ab_cd */
t_ab_cd(f,float,FLT)

void pvn_cmul(float *const cr, float *const ci, const float ar, const float ai, const float br, const float bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  PVN_ASSERT(isfinite(ar));
  PVN_ASSERT(isfinite(ai));
  PVN_ASSERT(isfinite(br));
  PVN_ASSERT(isfinite(bi));
  int are = 0, aie = 0, bre = 0, bie = 0;
  const float arf = frexpf(ar, &are);
  const float aif = frexpf(ai, &aie);
  const float brf = frexpf(br, &bre);
  const float bif = frexpf(bi, &bie);
  const int arbre = (are + bre);
  const int aibie = (aie + bie);
  *cr = (((arbre > aibie) || ((arbre == aibie) && (fabsf(arf * brf) >= fabsf(aif * bif)))) ?
         f_ab_cd(arbre, are, arf, bre, brf, aie, -aif, bie, bif) :
         f_ab_cd(aibie, aie, -aif, bie, bif, are, arf, bre, brf));
  const int arbie = (are + bie);
  const int aibre = (aie + bre);
  *ci = (((arbie > aibre) || ((arbie == aibre) && (fabsf(arf * bif) >= fabsf(aif * brf)))) ?
         f_ab_cd(arbie, are, arf, bie, bif, aie,  aif, bre, brf) :
         f_ab_cd(aibre, aie,  aif, bre, brf, are, arf, bie, bif));
}

#ifdef t_ab_cd_e
#error t_ab_cd_e already defined
#else /* !t_ab_cd_e */
#define t_ab_cd_e(t,T,TYP)                                                                                                           \
static T t##_ab_cd_e(const int emax,                                                                                                 \
                     const int are, const T arf, const int bre, const T brf, const int aie, const T aif, const int bie, const T bif, \
                     const int cre, const T crf)                                                                                     \
{                                                                                                                                    \
  const int s = (TYP##_MAX_EXP - emax - 2);                                                                                          \
  int ars = are, brs = bre, ais = aie, bis = bie, crs = (cre + s);                                                                   \
  if (s > 0) {                                                                                                                       \
    if (are >= bre) {                                                                                                                \
      brs = (bre + s);                                                                                                               \
      ars = (brs - TYP##_MAX_EXP);                                                                                                   \
      if (ars > 0) {                                                                                                                 \
        brs -= ars;                                                                                                                  \
        ars += are;                                                                                                                  \
      }                                                                                                                              \
      else                                                                                                                           \
        ars = are;                                                                                                                   \
    }                                                                                                                                \
    else {                                                                                                                           \
      ars = (are + s);                                                                                                               \
      brs = (ars - TYP##_MAX_EXP);                                                                                                   \
      if (brs > 0) {                                                                                                                 \
        ars -= brs;                                                                                                                  \
        brs += bre;                                                                                                                  \
      }                                                                                                                              \
      else                                                                                                                           \
        brs = bre;                                                                                                                   \
    }                                                                                                                                \
    if (aie >= bie) {                                                                                                                \
      bis = (bie + s);                                                                                                               \
      ais = (bis - TYP##_MAX_EXP);                                                                                                   \
      if (ais > 0) {                                                                                                                 \
        bis -= ais;                                                                                                                  \
        ais += aie;                                                                                                                  \
      }                                                                                                                              \
      else                                                                                                                           \
        ais = aie;                                                                                                                   \
    }                                                                                                                                \
    else {                                                                                                                           \
      ais = (aie + s);                                                                                                               \
      bis = (ais - TYP##_MAX_EXP);                                                                                                   \
      if (bis > 0) {                                                                                                                 \
        ais -= bis;                                                                                                                  \
        bis += bie;                                                                                                                  \
      }                                                                                                                              \
      else                                                                                                                           \
        bis = bie;                                                                                                                   \
    }                                                                                                                                \
  }                                                                                                                                  \
  else if (s < 0) {                                                                                                                  \
    if (are >= bre) {                                                                                                                \
      ars = (are + s);                                                                                                               \
      brs = (ars - TYP##_MIN_EXP);                                                                                                   \
      if (brs < 0) {                                                                                                                 \
        ars -= brs;                                                                                                                  \
        brs += bre;                                                                                                                  \
      }                                                                                                                              \
      else                                                                                                                           \
        brs = bre;                                                                                                                   \
    }                                                                                                                                \
    else {                                                                                                                           \
      brs = (bre + s);                                                                                                               \
      ars = (brs - TYP##_MIN_EXP);                                                                                                   \
      if (ars < 0) {                                                                                                                 \
        brs -= ars;                                                                                                                  \
        ars += are;                                                                                                                  \
      }                                                                                                                              \
      else                                                                                                                           \
        ars = are;                                                                                                                   \
    }                                                                                                                                \
    if (aie >= bie) {                                                                                                                \
      ais = (aie + s);                                                                                                               \
      bis = (ais - TYP##_MIN_EXP);                                                                                                   \
      if (bis < 0) {                                                                                                                 \
        ais -= bis;                                                                                                                  \
        bis += bie;                                                                                                                  \
      }                                                                                                                              \
      else                                                                                                                           \
        bis = bie;                                                                                                                   \
    }                                                                                                                                \
    else {                                                                                                                           \
      bis = (bie + s);                                                                                                               \
      ais = (bis - TYP##_MIN_EXP);                                                                                                   \
      if (ais < 0) {                                                                                                                 \
        bis -= ais;                                                                                                                  \
        ais += aie;                                                                                                                  \
      }                                                                                                                              \
      else                                                                                                                           \
        ais = aie;                                                                                                                   \
    }                                                                                                                                \
  }                                                                                                                                  \
  const T ret = fma##t(scalbn##t(arf, ars), scalbn##t(brf, brs),                                                                     \
                       fma##t(scalbn##t(aif, ais), scalbn##t(bif, bis), scalbn##t(crf, crs)));                                       \
  return (s ? scalbn##t(ret, -s) : ret);                                                                                             \
}
#endif /* ?t_ab_cd_e */
t_ab_cd_e(f,float,FLT)

void pvn_cfma(float *const dr, float *const di, const float ar, const float ai, const float br, const float bi, const float cr, const float ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  PVN_ASSERT(isfinite(ar));
  PVN_ASSERT(isfinite(ai));
  PVN_ASSERT(isfinite(br));
  PVN_ASSERT(isfinite(bi));
  PVN_ASSERT(isfinite(cr));
  PVN_ASSERT(isfinite(ci));
  int are = 0, aie = 0, bre = 0, bie = 0, cre = 0, cie = 0;
  const float arf = frexpf(ar, &are);
  const float aif = frexpf(ai, &aie);
  const float brf = frexpf(br, &bre);
  const float bif = frexpf(bi, &bie);
  const float crf = frexpf(cr, &cre);
  const float cif = frexpf(ci, &cie);
  const int arbre = (are + bre);
  const int aibie = (aie + bie);
  const int ermax = pvn_imax3(arbre, aibie, cre);
  *dr = (((arbre > aibie) || ((arbre == aibie) && (fabsf(arf * brf) >= fabsf(aif * bif)))) ?
         f_ab_cd_e(ermax, are, arf, bre, brf, aie, -aif, bie, bif, cre, crf) :
         f_ab_cd_e(ermax, aie, -aif, bie, bif, are, arf, bre, brf, cre, crf));
  const int arbie = (are + bie);
  const int aibre = (aie + bre);
  const int eimax = pvn_imax3(arbie, aibre, cie);
  *di = (((arbie > aibre) || ((arbie == aibre) && (fabsf(arf * bif) >= fabsf(aif * brf)))) ?
         f_ab_cd_e(eimax, are, arf, bie, bif, aie,  aif, bre, brf, cie, cif) :
         f_ab_cd_e(eimax, aie,  aif, bre, brf, are, arf, bie, bif, cie, cif));
}

t_ab_cd(,double,DBL)

void pvn_zmul(double *const cr, double *const ci, const double ar, const double ai, const double br, const double bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  PVN_ASSERT(isfinite(ar));
  PVN_ASSERT(isfinite(ai));
  PVN_ASSERT(isfinite(br));
  PVN_ASSERT(isfinite(bi));
  int are = 0, aie = 0, bre = 0, bie = 0;
  const double arf = frexp(ar, &are);
  const double aif = frexp(ai, &aie);
  const double brf = frexp(br, &bre);
  const double bif = frexp(bi, &bie);
  const int arbre = (are + bre);
  const int aibie = (aie + bie);
  *cr = (((arbre > aibie) || ((arbre == aibie) && (fabs(arf * brf) >= fabs(aif * bif)))) ?
         _ab_cd(arbre, are, arf, bre, brf, aie, -aif, bie, bif) :
         _ab_cd(aibie, aie, -aif, bie, bif, are, arf, bre, brf));
  const int arbie = (are + bie);
  const int aibre = (aie + bre);
  *ci = (((arbie > aibre) || ((arbie == aibre) && (fabs(arf * bif) >= fabs(aif * brf)))) ?
         _ab_cd(arbie, are, arf, bie, bif, aie,  aif, bre, brf) :
         _ab_cd(aibre, aie,  aif, bre, brf, are, arf, bie, bif));
}

t_ab_cd_e(,double,DBL)

void pvn_zfma(double *const dr, double *const di, const double ar, const double ai, const double br, const double bi, const double cr, const double ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  PVN_ASSERT(isfinite(ar));
  PVN_ASSERT(isfinite(ai));
  PVN_ASSERT(isfinite(br));
  PVN_ASSERT(isfinite(bi));
  PVN_ASSERT(isfinite(cr));
  PVN_ASSERT(isfinite(ci));
  int are = 0, aie = 0, bre = 0, bie = 0, cre = 0, cie = 0;
  const double arf = frexp(ar, &are);
  const double aif = frexp(ai, &aie);
  const double brf = frexp(br, &bre);
  const double bif = frexp(bi, &bie);
  const double crf = frexp(cr, &cre);
  const double cif = frexp(ci, &cie);
  const int arbre = (are + bre);
  const int aibie = (aie + bie);
  const int ermax = pvn_imax3(arbre, aibie, cre);
  *dr = (((arbre > aibie) || ((arbre == aibie) && (fabs(arf * brf) >= fabs(aif * bif)))) ?
         _ab_cd_e(ermax, are, arf, bre, brf, aie, -aif, bie, bif, cre, crf) :
         _ab_cd_e(ermax, aie, -aif, bie, bif, are, arf, bre, brf, cre, crf));
  const int arbie = (are + bie);
  const int aibre = (aie + bre);
  const int eimax = pvn_imax3(arbie, aibre, cie);
  *di = (((arbie > aibre) || ((arbie == aibre) && (fabs(arf * bif) >= fabs(aif * brf)))) ?
         _ab_cd_e(eimax, are, arf, bie, bif, aie,  aif, bre, brf, cie, cif) :
         _ab_cd_e(eimax, aie,  aif, bre, brf, are, arf, bie, bif, cie, cif));
}

t_ab_cd(l,long double,LDBL)

void pvn_wmul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  PVN_ASSERT(isfinite(ar));
  PVN_ASSERT(isfinite(ai));
  PVN_ASSERT(isfinite(br));
  PVN_ASSERT(isfinite(bi));
  int are = 0, aie = 0, bre = 0, bie = 0;
  const long double arf = frexpl(ar, &are);
  const long double aif = frexpl(ai, &aie);
  const long double brf = frexpl(br, &bre);
  const long double bif = frexpl(bi, &bie);
  const int arbre = (are + bre);
  const int aibie = (aie + bie);
  *cr = (((arbre > aibie) || ((arbre == aibie) && (fabsl(arf * brf) >= fabsl(aif * bif)))) ?
         l_ab_cd(arbre, are, arf, bre, brf, aie, -aif, bie, bif) :
         l_ab_cd(aibie, aie, -aif, bie, bif, are, arf, bre, brf));
  const int arbie = (are + bie);
  const int aibre = (aie + bre);
  *ci = (((arbie > aibre) || ((arbie == aibre) && (fabsl(arf * bif) >= fabsl(aif * brf)))) ?
         l_ab_cd(arbie, are, arf, bie, bif, aie,  aif, bre, brf) :
         l_ab_cd(aibre, aie,  aif, bre, brf, are, arf, bie, bif));
}

t_ab_cd_e(l,long double,LDBL)

void pvn_wfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  PVN_ASSERT(isfinite(ar));
  PVN_ASSERT(isfinite(ai));
  PVN_ASSERT(isfinite(br));
  PVN_ASSERT(isfinite(bi));
  PVN_ASSERT(isfinite(cr));
  PVN_ASSERT(isfinite(ci));
  int are = 0, aie = 0, bre = 0, bie = 0, cre = 0, cie = 0;
  const long double arf = frexpl(ar, &are);
  const long double aif = frexpl(ai, &aie);
  const long double brf = frexpl(br, &bre);
  const long double bif = frexpl(bi, &bie);
  const long double crf = frexpl(cr, &cre);
  const long double cif = frexpl(ci, &cie);
  const int arbre = (are + bre);
  const int aibie = (aie + bie);
  const int ermax = pvn_imax3(arbre, aibie, cre);
  *dr = (((arbre > aibie) || ((arbre == aibie) && (fabsl(arf * brf) >= fabsl(aif * bif)))) ?
         l_ab_cd_e(ermax, are, arf, bre, brf, aie, -aif, bie, bif, cre, crf) :
         l_ab_cd_e(ermax, aie, -aif, bie, bif, are, arf, bre, brf, cre, crf));
  const int arbie = (are + bie);
  const int aibre = (aie + bre);
  const int eimax = pvn_imax3(arbie, aibre, cie);
  *di = (((arbie > aibre) || ((arbie == aibre) && (fabsl(arf * bif) >= fabsl(aif * brf)))) ?
         l_ab_cd_e(eimax, are, arf, bie, bif, aie,  aif, bre, brf, cie, cif) :
         l_ab_cd_e(eimax, aie,  aif, bre, brf, are, arf, bie, bif, cie, cif));
}

#ifdef PVN_QUADMATH
t_ab_cd(q,__float128,FLT128)

void pvn_ymul(__float128 *const cr, __float128 *const ci, const __float128 ar, const __float128 ai, const __float128 br, const __float128 bi)
{
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  PVN_ASSERT(isfiniteq(ar));
  PVN_ASSERT(isfiniteq(ai));
  PVN_ASSERT(isfiniteq(br));
  PVN_ASSERT(isfiniteq(bi));
  int are = 0, aie = 0, bre = 0, bie = 0;
  const __float128 arf = frexpq(ar, &are);
  const __float128 aif = frexpq(ai, &aie);
  const __float128 brf = frexpq(br, &bre);
  const __float128 bif = frexpq(bi, &bie);
  const int arbre = (are + bre);
  const int aibie = (aie + bie);
  *cr = (((arbre > aibie) || ((arbre == aibie) && (fabsq(arf * brf) >= fabsq(aif * bif)))) ?
         q_ab_cd(arbre, are, arf, bre, brf, aie, -aif, bie, bif) :
         q_ab_cd(aibie, aie, -aif, bie, bif, are, arf, bre, brf));
  const int arbie = (are + bie);
  const int aibre = (aie + bre);
  *ci = (((arbie > aibre) || ((arbie == aibre) && (fabsq(arf * bif) >= fabsq(aif * brf)))) ?
         q_ab_cd(arbie, are, arf, bie, bif, aie,  aif, bre, brf) :
         q_ab_cd(aibre, aie,  aif, bre, brf, are, arf, bie, bif));
}

t_ab_cd_e(q,__float128,FLT128)

void pvn_yfma(__float128 *const dr, __float128 *const di, const __float128 ar, const __float128 ai, const __float128 br, const __float128 bi, const __float128 cr, const __float128 ci)
{
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  PVN_ASSERT(isfiniteq(ar));
  PVN_ASSERT(isfiniteq(ai));
  PVN_ASSERT(isfiniteq(br));
  PVN_ASSERT(isfiniteq(bi));
  PVN_ASSERT(isfiniteq(cr));
  PVN_ASSERT(isfiniteq(ci));
  int are = 0, aie = 0, bre = 0, bie = 0, cre = 0, cie = 0;
  const __float128 arf = frexpl(ar, &are);
  const __float128 aif = frexpl(ai, &aie);
  const __float128 brf = frexpl(br, &bre);
  const __float128 bif = frexpl(bi, &bie);
  const __float128 crf = frexpl(cr, &cre);
  const __float128 cif = frexpl(ci, &cie);
  const int arbre = (are + bre);
  const int aibie = (aie + bie);
  const int ermax = pvn_imax3(arbre, aibie, cre);
  *dr = (((arbre > aibie) || ((arbre == aibie) && (fabsq(arf * brf) >= fabsq(aif * bif)))) ?
         q_ab_cd_e(ermax, are, arf, bre, brf, aie, -aif, bie, bif, cre, crf) :
         q_ab_cd_e(ermax, aie, -aif, bie, bif, are, arf, bre, brf, cre, crf));
  const int arbie = (are + bie);
  const int aibre = (aie + bre);
  const int eimax = pvn_imax3(arbie, aibre, cie);
  *di = (((arbie > aibre) || ((arbie == aibre) && (fabsq(arf * bif) >= fabsq(aif * brf)))) ?
         q_ab_cd_e(eimax, are, arf, bie, bif, aie,  aif, bre, brf, cie, cif) :
         q_ab_cd_e(eimax, aie,  aif, bre, brf, are, arf, bie, bif, cie, cif));
}
#else /* !PVN_QUADMATH */
void pvn_ymul(long double *const cr, long double *const ci, const long double ar, const long double ai, const long double br, const long double bi)
{
  pvn_wmul(cr, ci, ar, ai, br, bi);
}

void pvn_yfma(long double *const dr, long double *const di, const long double ar, const long double ai, const long double br, const long double bi, const long double cr, const long double ci)
{
  pvn_wfma(dr, di, ar, ai, br, bi, cr, ci);
}
#endif /* ?PVN_QUADMATH */
#endif /* PVN_CMA_SAFE */

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

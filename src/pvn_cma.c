#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if ((argc != 5) && (argc != 7)) {
    (void)fprintf(stderr, "%s ar ai br bi [cr ci]\n", *argv);
    return EXIT_FAILURE;
  }
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
    (void)printf("pvn_zfma=(%s,", pvn_dtoa(s, dr));
    (void)printf("%s)\n", pvn_dtoa(s, di));
  }
  else {
    double cr = 0.0, ci = 0.0;
    pvn_zmul(&cr, &ci, ar, ai, br, bi);
    (void)printf("pvn_zmul=(%s,", pvn_dtoa(s, cr));
    (void)printf("%s)\n", pvn_dtoa(s, ci));
  }
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
void PVN_FABI(pvn_cmul,PVN_CMUL)(float *const cr, float *const ci, const float *const ar, const float *const ai, const float *const br, const float *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_cmul(cr, ci, *ar, *ai, *br, *bi);
}

float complex PVN_FABI(pvn_mulc,PVN_MULC)(const float complex *const a, const float complex *const b)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  float complex c;
  PVN_FABI(pvn_cmul,PVN_CMUL)((float*)&c, (float*)&c + 1, (const float*)a, (const float*)a + 1, (const float*)b, (const float*)b + 1);
  return c;
}

void PVN_FABI(pvn_cfma,PVN_CFMA)(float *const dr, float *const di, const float *const ar, const float *const ai, const float *const br, const float *const bi, const float *const cr, const float *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_cfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}

float complex PVN_FABI(pvn_fmac,PVN_FMAC)(const float complex *const a, const float complex *const b, const float complex *const c)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  float complex d;
  PVN_FABI(pvn_cfma,PVN_CFMA)((float*)&d, (float*)&d + 1, (const float*)a, (const float*)a + 1, (const float*)b, (const float*)b + 1, (const float*)c, (const float*)c + 1);
  return d;
}

void PVN_FABI(pvn_cfmma,PVN_CFMMA)(float *const er, float *const ei, const float *const ar, const float *const ai, const float *const br, const float *const bi, const float *const cr, const float *const ci, const float *const dr, const float *const di)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  pvn_cfmma(er, ei, *ar, *ai, *br, *bi, *cr, *ci, *dr, *di);
}

float complex PVN_FABI(pvn_fmmac,PVN_FMMAC)(const float complex *const a, const float complex *const b, const float complex *const c, const float complex *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  float complex e;
  PVN_FABI(pvn_cfmma,PVN_CFMMA)((float*)&e, (float*)&e + 1, (const float*)a, (const float*)a + 1, (const float*)b, (const float*)b + 1, (const float*)c, (const float*)c + 1, (const float*)d, (const float*)d + 1);
  return e;
}

void PVN_FABI(pvn_zmul,PVN_ZMUL)(double *const cr, double *const ci, const double *const ar, const double *const ai, const double *const br, const double *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_zmul(cr, ci, *ar, *ai, *br, *bi);
}

double complex PVN_FABI(pvn_mulz,PVN_MULZ)(const double complex *const a, const double complex *const b)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  double complex c;
  PVN_FABI(pvn_zmul,PVN_ZMUL)((double*)&c, (double*)&c + 1, (const double*)a, (const double*)a + 1, (const double*)b, (const double*)b + 1);
  return c;
}

void PVN_FABI(pvn_zfma,PVN_ZFMA)(double *const dr, double *const di, const double *const ar, const double *const ai, const double *const br, const double *const bi, const double *const cr, const double *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_zfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}

double complex PVN_FABI(pvn_fmaz,PVN_FMAZ)(const double complex *const a, const double complex *const b, const double complex *const c)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  double complex d;
  PVN_FABI(pvn_zfma,PVN_ZFMA)((double*)&d, (double*)&d + 1, (const double*)a, (const double*)a + 1, (const double*)b, (const double*)b + 1, (const double*)c, (const double*)c + 1);
  return d;
}

void PVN_FABI(pvn_zfmma,PVN_ZFMMA)(double *const er, double *const ei, const double *const ar, const double *const ai, const double *const br, const double *const bi, const double *const cr, const double *const ci, const double *const dr, const double *const di)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  pvn_zfmma(er, ei, *ar, *ai, *br, *bi, *cr, *ci, *dr, *di);
}

double complex PVN_FABI(pvn_fmmaz,PVN_FMMAZ)(const double complex *const a, const double complex *const b, const double complex *const c, const double complex *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  double complex e;
  PVN_FABI(pvn_zfmma,PVN_ZFMMA)((double*)&e, (double*)&e + 1, (const double*)a, (const double*)a + 1, (const double*)b, (const double*)b + 1, (const double*)c, (const double*)c + 1, (const double*)d, (const double*)d + 1);
  return e;
}

void PVN_FABI(pvn_wmul,PVN_WMUL)(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_wmul(cr, ci, *ar, *ai, *br, *bi);
}

long double complex PVN_FABI(pvn_mulw,PVN_MULW)(const long double complex *const a, const long double complex *const b)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  long double complex c;
  PVN_FABI(pvn_wmul,PVN_WMUL)((long double*)&c, (long double*)&c + 1, (const long double*)a, (const long double*)a + 1, (const long double*)b, (const long double*)b + 1);
  return c;
}

void PVN_FABI(pvn_wfma,PVN_WFMA)(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_wfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}

long double complex PVN_FABI(pvn_fmaw,PVN_FMAW)(const long double complex *const a, const long double complex *const b, const long double complex *const c)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  long double complex d;
  PVN_FABI(pvn_wfma,PVN_WFMA)((long double*)&d, (long double*)&d + 1, (const long double*)a, (const long double*)a + 1, (const long double*)b, (const long double*)b + 1, (const long double*)c, (const long double*)c + 1);
  return d;
}

void PVN_FABI(pvn_wfmma,PVN_WFMMA)(long double *const er, long double *const ei, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci, const long double *const dr, const long double *const di)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  pvn_wfmma(er, ei, *ar, *ai, *br, *bi, *cr, *ci, *dr, *di);
}

long double complex PVN_FABI(pvn_fmmaw,PVN_FMMAW)(const long double complex *const a, const long double complex *const b, const long double complex *const c, const long double complex *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  long double complex e;
  PVN_FABI(pvn_wfmma,PVN_WFMMA)((long double*)&e, (long double*)&e + 1, (const long double*)a, (const long double*)a + 1, (const long double*)b, (const long double*)b + 1, (const long double*)c, (const long double*)c + 1, (const long double*)d, (const long double*)d + 1);
  return e;
}
#ifdef PVN_QUADMATH
void PVN_FABI(pvn_ymul,PVN_YMUL)(__float128 *const cr, __float128 *const ci, const __float128 *const ar, const __float128 *const ai, const __float128 *const br, const __float128 *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_ymul(cr, ci, *ar, *ai, *br, *bi);
}

__complex128 PVN_FABI(pvn_muly,PVN_MULY)(const __complex128 *const a, const __complex128 *const b)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  __complex128 c;
  PVN_FABI(pvn_ymul,PVN_YMUL)((__float128*)&c, (__float128*)&c + 1, (const __float128*)a, (const __float128*)a + 1, (const __float128*)b, (const __float128*)b + 1);
  return c;
}

void PVN_FABI(pvn_yfma,PVN_YFMA)(__float128 *const dr, __float128 *const di, const __float128 *const ar, const __float128 *const ai, const __float128 *const br, const __float128 *const bi, const __float128 *const cr, const __float128 *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_yfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}

__complex128 PVN_FABI(pvn_fmay,PVN_FMAY)(const __complex128 *const a, const __complex128 *const b, const __complex128 *const c)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  __complex128 d;
  PVN_FABI(pvn_yfma,PVN_YFMA)((__float128*)&d, (__float128*)&d + 1, (const __float128*)a, (const __float128*)a + 1, (const __float128*)b, (const __float128*)b + 1, (const __float128*)c, (const __float128*)c + 1);
  return d;
}

void PVN_FABI(pvn_yfmma,PVN_YFMMA)(__float128 *const er, __float128 *const ei, const __float128 *const ar, const __float128 *const ai, const __float128 *const br, const __float128 *const bi, const __float128 *const cr, const __float128 *const ci, const __float128 *const dr, const __float128 *const di)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  pvn_yfmma(er, ei, *ar, *ai, *br, *bi, *cr, *ci, *dr, *di);
}

__complex128 PVN_FABI(pvn_fmmay,PVN_FMMAY)(const __complex128 *const a, const __complex128 *const b, const __complex128 *const c, const __complex128 *const d)
{
  PVN_ASSERT(a);
  PVN_ASSERT(b);
  PVN_ASSERT(c);
  PVN_ASSERT(d);
  __complex128 e;
  PVN_FABI(pvn_yfmma,PVN_YFMMA)((__float128*)&e, (__float128*)&e + 1, (const __float128*)a, (const __float128*)a + 1, (const __float128*)b, (const __float128*)b + 1, (const __float128*)c, (const __float128*)c + 1, (const __float128*)d, (const __float128*)d + 1);
  return e;
}
#else /* !PVN_QUADMATH */
void PVN_FABI(pvn_ymul,PVN_YMUL)(long double *const cr, long double *const ci, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  pvn_wmul(cr, ci, *ar, *ai, *br, *bi);
}

long double complex PVN_FABI(pvn_muly,PVN_MULY)(const long double complex *const a, const long double complex *const b)
{
  return PVN_FABI(pvn_mulw,PVN_MULW)(a, b);
}

void PVN_FABI(pvn_yfma,PVN_YFMA)(long double *const dr, long double *const di, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  pvn_wfma(dr, di, *ar, *ai, *br, *bi, *cr, *ci);
}

long double complex PVN_FABI(pvn_fmay,PVN_FMAY)(const long double complex *const a, const long double complex *const b, const long double complex *const c)
{
  return PVN_FABI(pvn_fmaw,PVN_FMAW)(a, b, c);
}

void PVN_FABI(pvn_yfmma,PVN_YFMMA)(long double *const er, long double *const ei, const long double *const ar, const long double *const ai, const long double *const br, const long double *const bi, const long double *const cr, const long double *const ci, const long double *const dr, const long double *const di)
{
  PVN_ASSERT(ar);
  PVN_ASSERT(ai);
  PVN_ASSERT(br);
  PVN_ASSERT(bi);
  PVN_ASSERT(cr);
  PVN_ASSERT(ci);
  PVN_ASSERT(dr);
  PVN_ASSERT(di);
  pvn_wfmma(er, ei, *ar, *ai, *br, *bi, *cr, *ci, *dr, *di);
}

long double complex PVN_FABI(pvn_fmmay,PVN_FMMAY)(const long double complex *const a, const long double complex *const b, const long double complex *const c, const long double complex *const d)
{
  return PVN_FABI(pvn_fmmaw,PVN_FMMAW)(a, b, c, d);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

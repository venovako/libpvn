#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc != 3) {
    (void)fprintf(stderr, "%s p q\n", argv[0]);
    return EXIT_FAILURE;
  }
  const unsigned p = (unsigned)atoi(argv[1]);
  const unsigned q = (unsigned)atoi(argv[2]);
  const float f = 0.0f;
  double d = 0.0;
  pvn_djs_denc_(&d, &f, &p, &q);
  char s[46] = { '\0' };
  (void)fprintf(stdout, "d=%s\n", pvn_dtoa(s, d));
  d = 0.0;
#ifdef PVN_QUADMATH
  __float128 e = 0.0q;
#else /* !PVN_QUADMATH */
  long double e = 0.0L;
#endif /* ?PVN_QUADMATH */
  pvn_djs_qenc_(&e, &d, &p, &q);
  (void)fprintf(stdout, "q=%s\n", pvn_qtoa(s, e));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
void pvn_djs_denc_(double *const e, const float *const f, const unsigned *const p, const unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(f);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  const double x = (double)*f;
  *(unsigned long*)e = (*(const unsigned long*)&x | ((unsigned long)((*p - 1u) & 16383u) << 14u) | (unsigned long)((*q - 1u) & 16383u));
}

void pvn_djs_ddec_(const double *const e, unsigned *const p, unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  unsigned long u = *(const unsigned long*)e;
  *q = ((unsigned)(u & 16383ul) + 1u);
  u >>= 14u;
  *p = ((unsigned)(u & 16383ul) + 1u);
}
#ifdef PVN_QUADMATH
void pvn_djs_qenc_(__float128 *const e, const double *const d, const unsigned *const p, const unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(d);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  const __float128 x = (__float128)*d;
  *(unsigned __int128*)e = (*(const unsigned __int128*)&x | ((unsigned __int128)((*p - 1u) & 1073741823u) << 30u) | (unsigned __int128)((*q - 1u) & 1073741823u));
}

void pvn_djs_qdec_(const __float128 *const e, unsigned *const p, unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  unsigned __int128 u = *(const unsigned __int128*)e;
  *q = ((unsigned)(u & (unsigned __int128)1073741823u) + 1u);
  u >>= 30u;
  *p = ((unsigned)(u & (unsigned __int128)1073741823u) + 1u);
}
#else /* !PVN_QUADMATH */
void pvn_djs_qenc_(long double *const e, const double *const d, const unsigned *const p, const unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(d);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  const long double x = (long double)*d;
  *(unsigned __int128*)e = (*(const unsigned __int128*)&x | ((unsigned __int128)((*p - 1u) & 1073741823u) << 30u) | ((unsigned __int128)((*q - 1u) & 1073741823u) << 30u));
}

void pvn_djs_qdec_(const long double *const e, unsigned *const p, unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  unsigned __int128 u = *(const unsigned __int128*)e;
  *q = ((unsigned)(u & (unsigned __int128)1073741823u) + 1u);
  u >>= 30u;
  *p = ((unsigned)(u & (unsigned __int128)1073741823u) + 1u);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

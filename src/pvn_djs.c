#include "pvn.h"

/* !!! LITTLE ENDIAN ONLY !!! */

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
#ifdef __x86_64__
  long double x = 0.0L;
  pvn_djs_xenc_(&x, &d, &p, &q);
  (void)fprintf(stdout, "x=%s\n", pvn_xtoa(s, x));
#endif /* __x86_64__ */
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
  PVN_ASSERT(*p);
  PVN_ASSERT(*q);
  *e = (double)*f;
#ifdef NDEBUG
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)(*p - 1u) << 14u) | (unsigned long)(*q - 1u));
#else /* !NDEBUG */
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)((*p - 1u) & 16383u) << 14u) | (unsigned long)((*q - 1u) & 16383u));
#endif /* ?NDEBUG */
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

void pvn_djs_xenc_(long double *const e, const double *const d, const unsigned *const p, const unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(d);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  PVN_ASSERT(*p);
  PVN_ASSERT(*q);
  *e = (long double)*d;
#ifdef __x86_64__
#ifdef NDEBUG
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)(*p - 1u) << 5u) | (unsigned long)(*q - 1u));
#else /* !NDEBUG */
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)((*p - 1u) & 31u) << 5u) | (unsigned long)((*q - 1u) & 31u));
#endif /* ?NDEBUG */
#else /* !__x86_64__ */
#ifdef NDEBUG
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)(*p - 1u) << 30u) | (unsigned long)(*q - 1u));
#else /* !NDEBUG */
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)((*p - 1u) & 1073741823u) << 30u) | (unsigned long)((*q - 1u) & 1073741823u));
#endif /* ?NDEBUG */
#endif /* ?__x86_64__ */
}

void pvn_djs_xdec_(const long double *const e, unsigned *const p, unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  unsigned long u = *(const unsigned long*)e;
#ifdef __x86_64__
  *q = ((unsigned)(u & 31ul) + 1u);
  u >>= 5u;
  *p = ((unsigned)(u & 31ul) + 1u);
#else /* !__x86_64__ */
  *q = ((unsigned)(u & 1073741823ul) + 1u);
  u >>= 30u;
  *p = ((unsigned)(u & 1073741823ul) + 1u);
#endif /* ?__x86_64__ */
}

#ifdef PVN_QUADMATH
void pvn_djs_qenc_(__float128 *const e, const double *const d, const unsigned *const p, const unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(d);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  PVN_ASSERT(*p);
  PVN_ASSERT(*q);
  *e = (__float128)*d;
#ifdef NDEBUG
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)(*p - 1u) << 30u) | (unsigned long)(*q - 1u));
#else /* !NDEBUG */
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)((*p - 1u) & 1073741823u) << 30u) | (unsigned long)((*q - 1u) & 1073741823u));
#endif /* ?NDEBUG */
}

void pvn_djs_qdec_(const __float128 *const e, unsigned *const p, unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  unsigned long u = *(const unsigned long*)e;
  *q = ((unsigned)(u & 1073741823ul) + 1u);
  u >>= 30u;
  *p = ((unsigned)(u & 1073741823ul) + 1u);
}
#else /* !PVN_QUADMATH */
void pvn_djs_qenc_(long double *const e, const double *const d, const unsigned *const p, const unsigned *const q)
{
  pvn_djs_xenc_(e, d, p, q);
}

void pvn_djs_qdec_(const long double *const e, unsigned *const p, unsigned *const q)
{
  pvn_djs_xdec_(e, p, q);
}
#endif /* ?PVN_QUADMATH */
#endif /* ?PVN_TEST */

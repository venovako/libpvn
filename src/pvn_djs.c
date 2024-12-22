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

void pvn_djs_xmkd_(const unsigned *const n, const double *const g, const unsigned *const ldg, long double *const d, const unsigned *const o, int *const info)
{
  PVN_ASSERT(n);
  PVN_ASSERT(g);
  PVN_ASSERT(ldg);
  PVN_ASSERT(d);
  PVN_ASSERT(o);
  PVN_ASSERT(info);

  const int l = *info;
  *info = 0;
  if (*ldg < *n)
    *info = -3;
#ifdef __x86_64__
  if (*n > 32u)
#else /* !__x86_64__ */
  if (*n > 1073741824u)
#endif /* ?__x86_64__ */
    *info = -1;
  if (*info)
    return;
  if (!*n)
    return;

  /* build D and determine its largest element */
  const unsigned m = ((*n & 1u) ? (*n * ((*n - 1u) >> 1u)) : ((*n >> 1u) * (*n - 1u)));
  long double w = 0.0L;
  if (l) {
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(g,d,o,m,n,ldg) reduction(max:w)
#endif /* _OPENMP */
    for (unsigned k = 0u; k < m; ++k) {
      const unsigned k_ = (k << 1u);
      const unsigned p = o[k_];
      const unsigned q = o[k_ + 1u];
      const unsigned p_ = (p - 1u);
      const unsigned q_ = (q - 1u);
      const unsigned p_ldg = (*ldg * p_);
      const unsigned q_ldg = (*ldg * q_);
      const double gpp = g[p_ldg + p_];
      const double gqp = g[p_ldg + q_];
      const double gpq = g[q_ldg + p_];
      const double gqq = g[q_ldg + q_];
      const double h = ((gqp == 0.0) ? fabs(gpq) : ((gpq == 0.0) ? fabs(gqp) : hypot(gqp, gpq)));
      if ((h > 0.0) || (gpp < gqq) || (copysign(1.0, gpp) != 1.0) || (copysign(1.0, gqq) != 1.0)) {
        pvn_djs_xenc_((d + k), &h, &p, &q);
        w = fmaxl(w, d[k]);
      }
      else /* no transformation */
        d[k] = (long double)-h;
    }
    d[*info = (int)m] = w;
  }
  else {
    unsigned K = 0u;
    for (unsigned k = 0u; k < m; ++k) {
      const unsigned k_ = (k << 1u);
      const unsigned p = o[k_];
      const unsigned q = o[k_ + 1u];
      const unsigned p_ = (p - 1u);
      const unsigned q_ = (q - 1u);
      const unsigned p_ldg = (*ldg * p_);
      const unsigned q_ldg = (*ldg * q_);
      const double gpp = g[p_ldg + p_];
      const double gqp = g[p_ldg + q_];
      const double gpq = g[q_ldg + p_];
      const double gqq = g[q_ldg + q_];
      const double h = ((gqp == 0.0) ? fabs(gpq) : ((gpq == 0.0) ? fabs(gqp) : hypot(gqp, gpq)));
      if ((h > 0.0) || (gpp < gqq) || (copysign(1.0, gpp) != 1.0) || (copysign(1.0, gqq) != 1.0)) {
        pvn_djs_xenc_((d + K), &h, &p, &q);
        w = fmaxl(w, d[K++]);
      }
    }
    d[*info = (int)K] = w;
  }
}

void pvn_djs_wmkd_(const unsigned *const n, const double complex *const g, const unsigned *const ldg, long double *const d, const unsigned *const o, int *const info)
{
  PVN_ASSERT(n);
  PVN_ASSERT(g);
  PVN_ASSERT(ldg);
  PVN_ASSERT(d);
  PVN_ASSERT(o);
  PVN_ASSERT(info);

  const int l = *info;
  *info = 0;
  if (*ldg < *n)
    *info = -3;
#ifdef __x86_64__
  if (*n > 32u)
#else /* !__x86_64__ */
  if (*n > 1073741824u)
#endif /* ?__x86_64__ */
    *info = -1;
  if (*info)
    return;
  if (!*n)
    return;

  /* build D and determine its largest element */
  const unsigned m = ((*n & 1u) ? (*n * ((*n - 1u) >> 1u)) : ((*n >> 1u) * (*n - 1u)));
  long double w = 0.0L;
  if (l) {
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(g,d,o,m,n,ldg) reduction(max:w)
#endif /* _OPENMP */
    for (unsigned k = 0u; k < m; ++k) {
      const unsigned k_ = (k << 1u);
      const unsigned p = o[k_];
      const unsigned q = o[k_ + 1u];
      const unsigned p_ = (p - 1u);
      const unsigned q_ = (q - 1u);
      const unsigned p_ldg = (*ldg * p_);
      const unsigned q_ldg = (*ldg * q_);
      const double complex gpp = g[p_ldg + p_];
      const double complex gqp = g[p_ldg + q_];
      const double complex gpq = g[q_ldg + p_];
      const double complex gqq = g[q_ldg + q_];
      const double h = hypot(cabs(gqp), cabs(gpq));
      if ((h > 0.0) || (creal(gpp) < creal(gqq)) || (cimag(gpp) != 0.0) || (cimag(gqq) != 0.0) || (copysign(1.0, creal(gpp)) != 1.0) || (copysign(1.0, creal(gqq)) != 1.0)) {
        pvn_djs_xenc_((d + k), &h, &p, &q);
        w = fmaxl(w, d[k]);
      }
      else /* no transformation */
        d[k] = (long double)-h;
    }
    d[*info = (int)m] = w;
  }
  else {
    unsigned K = 0u;
    /* build D and determine its largest element */
    for (unsigned k = 0u; k < m; ++k) {
      const unsigned k_ = (k << 1u);
      const unsigned p = o[k_];
      const unsigned q = o[k_ + 1u];
      const unsigned p_ = (p - 1u);
      const unsigned q_ = (q - 1u);
      const unsigned p_ldg = (*ldg * p_);
      const unsigned q_ldg = (*ldg * q_);
      const double complex gpp = g[p_ldg + p_];
      const double complex gqp = g[p_ldg + q_];
      const double complex gpq = g[q_ldg + p_];
      const double complex gqq = g[q_ldg + q_];
      const double h = hypot(cabs(gqp), cabs(gpq));
      if ((h > 0.0) || (creal(gpp) < creal(gqq)) || (cimag(gpp) != 0.0) || (cimag(gqq) != 0.0) || (copysign(1.0, creal(gpp)) != 1.0) || (copysign(1.0, creal(gqq)) != 1.0)) {
        pvn_djs_xenc_((d + K), &h, &p, &q);
        w = fmaxl(w, d[K++]);
      }
    }
    d[*info = (int)K] = w;
  }
}

void pvn_djs_xmkdpq_(const unsigned *const n, const unsigned *const m, long double *const d, unsigned *const o, int *const info)
{
  PVN_ASSERT(n);
  PVN_ASSERT(m);
  PVN_ASSERT(d);
  PVN_ASSERT(o);
  PVN_ASSERT(info);

  const int l = *info;
  *info = 0;
#ifdef __x86_64__
  if (*n > 32u)
#else /* !__x86_64__ */
  if (*n > 1073741824u)
#endif /* ?__x86_64__ */
    *info = -1;
  if (*info)
    return;
  if (!*n)
    return;
  if (!*m)
    return;

  /* find the remaining pivots */
  const unsigned b = (*n >> 1u);
  long double w = d[*m];
  if (w <= 0.0L)
    return;
  if (l) {
    const unsigned K = *m;
    for (unsigned a = 0u; a < b; ) {
      ++*info;
      unsigned p = 0u, q = 0u;
      pvn_djs_xdec_(&w, &p, &q);
      const unsigned k_ = (a << 1u);
      o[k_] = p;
      o[k_ + 1u] = q;
      if (++a >= b)
        break;
      w = 0.0L;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(d,n,K,p,q) reduction(max:w)
#endif /* _OPENMP */
      for (unsigned k = 0u; k < K; ++k) {
        if (d[k] > 0.0L) {
          unsigned i = 0u, j = 0u;
          pvn_djs_xdec_((d + k), &i, &j);
          if ((i != p) && (i != q) && (j != p) && (j != q))
            w = fmaxl(w, d[k]);
          else /* colliding */
            d[k] = -d[k];
        }
      }
      if (w <= 0.0L)
        break;
    }
  }
  else {
    unsigned K = *m;
    for (unsigned a = 0u; a < b; ) {
      ++*info;
      unsigned p = 0u, q = 0u;
      pvn_djs_xdec_(&w, &p, &q);
      const unsigned k_ = (a << 1u);
      o[k_] = p;
      o[k_ + 1u] = q;
      if (++a >= b)
        break;
      w = 0.0L;
      for (int k = 0; k < (int)K; ++k) {
        unsigned i = 0u, j = 0u;
        pvn_djs_xdec_((d + k), &i, &j);
        if ((i != p) && (i != q) && (j != p) && (j != q))
          w = fmaxl(w, d[k]);
        else /* colliding */
          d[k--] = d[--K];
      }
      if (w <= 0.0L)
        break;
    }
  }
}
#endif /* ?PVN_TEST */

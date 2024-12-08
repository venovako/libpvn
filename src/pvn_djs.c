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
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)((*p - 1u) & 16383u) << 14u) | (unsigned long)((*q - 1u) & 16383u));
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
#ifdef __x86_64__
void pvn_djs_xenc_(long double *const e, const double *const d, const unsigned *const p, const unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(d);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  PVN_ASSERT(*p);
  PVN_ASSERT(*q);
  *e = (long double)*d;
  *(unsigned long*)e = (*(const unsigned long*)e | ((unsigned long)((*p - 1u) & 31u) << 5u) | (unsigned long)((*q - 1u) & 31u));
}

void pvn_djs_xdec_(const long double *const e, unsigned *const p, unsigned *const q)
{
  PVN_ASSERT(e);
  PVN_ASSERT(p);
  PVN_ASSERT(q);
  unsigned long u = *(const unsigned long*)e;
  *q = ((unsigned)(u & 31ul) + 1u);
  u >>= 5u;
  *p = ((unsigned)(u & 31ul) + 1u);
}

void pvn_djs_xmkdpq_(const unsigned *const n, const double *const g, const unsigned *const ldg, long double *const d, unsigned *const o, int *const info)
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
  if (*n > 32u)
    *info = -1;
  if (*info)
    return;
  if (!*n)
    return;

  const unsigned m = ((*n * (*n - 1u)) >> 1u);

  /* build D and determine its largest element */
  long double w = -1.0L;
  if (l) {
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(g,d,o,m,n,ldg,info) reduction(max:w)
#endif /* _OPENMP */
    for (unsigned k = 0u; k < m; ++k) {
      const unsigned k_ = (k << 1u);
      const unsigned p = o[k_];
      const unsigned q = o[k_ + 1u];
      if (!p || (p > *n) || !q || (q > *n))
        *info = -5;
      else {
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
          d[k] = -1.0L;
      }
    }
  }
  else {
    for (unsigned k = 0u; k < m; ++k) {
      const unsigned k_ = (k << 1u);
      const unsigned p = o[k_];
      const unsigned q = o[k_ + 1u];
      if (!p || (p > *n) || !q || (q > *n))
        *info = -5;
      else {
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
          d[k] = -1.0L;
      }
    }
  }
  if (*info || (w <= 0.0L))
    return;

  /* find the remaining pivots */
  const unsigned b = (*n >> 1u);
  if (l) {
    for (unsigned a = 0u; a < b; ++a) {
      ++*info;
      unsigned p = 0u;
      unsigned q = 0u;
      pvn_djs_xdec_(&w, &p, &q);
      if (!p || (p > *n) || !q || (q > *n)) {
        *info = -4;
        return;
      }
      const unsigned k_ = ((m + a) << 1u);
      o[k_] = p;
      o[k_ + 1u] = q;
      if ((a + 1u) < b) {
        w = -1.0L;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(d,m,n,p,q,info) reduction(max:w)
#endif /* _OPENMP */
        for (unsigned k = 0u; k < m; ++k) {
          if (d[k] > 0.0L) {
            unsigned i = 0u;
            unsigned j = 0u;
            pvn_djs_xdec_((d + k), &i, &j);
            if (!i || (i > *n) || !j || (j > *n))
              *info = -4;
            else if ((i != p) && (i != q) && (j != p) && (j != q))
              w = fmaxl(w, d[k]);
            else /* colliding */
              d[k] = -1.0L;
          }
        }
        if ((*info < 0) || (w <= 0.0L))
          break;
      }
    }
  }
  else {
    for (unsigned a = 0u; a < b; ++a) {
      ++*info;
      unsigned p = 0u;
      unsigned q = 0u;
      pvn_djs_xdec_(&w, &p, &q);
      if (!p || (p > *n) || !q || (q > *n)) {
        *info = -4;
        return;
      }
      const unsigned k_ = ((m + a) << 1u);
      o[k_] = p;
      o[k_ + 1u] = q;
      if ((a + 1u) < b) {
        w = -1.0L;
        for (unsigned k = 0u; k < m; ++k) {
          if (d[k] > 0.0L) {
            unsigned i = 0u;
            unsigned j = 0u;
            pvn_djs_xdec_((d + k), &i, &j);
            if (!i || (i > *n) || !j || (j > *n))
              *info = -4;
            else if ((i != p) && (i != q) && (j != p) && (j != q))
              w = fmaxl(w, d[k]);
            else /* colliding */
              d[k] = -1.0L;
          }
        }
        if ((*info < 0) || (w <= 0.0L))
          break;
      }
    }
  }
}

void pvn_djs_wmkdpq_(const unsigned *const n, const double complex *const g, const unsigned *const ldg, long double *const d, unsigned *const o, int *const info)
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
  if (*n > 32u)
    *info = -1;
  if (*info)
    return;
  if (!*n)
    return;

  const unsigned m = ((*n * (*n - 1u)) >> 1u);

  /* build D and determine its largest element */
  long double w = -1.0L;
  if (l) {
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(g,d,o,m,n,ldg,info) reduction(max:w)
#endif /* _OPENMP */
    for (unsigned k = 0u; k < m; ++k) {
      const unsigned k_ = (k << 1u);
      const unsigned p = o[k_];
      const unsigned q = o[k_ + 1u];
      if (!p || (p > *n) || !q || (q > *n))
        *info = -5;
      else {
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
          d[k] = -1.0L;
      }
    }
  }
  else {
    for (unsigned k = 0u; k < m; ++k) {
      const unsigned k_ = (k << 1u);
      const unsigned p = o[k_];
      const unsigned q = o[k_ + 1u];
      if (!p || (p > *n) || !q || (q > *n))
        *info = -5;
      else {
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
          d[k] = -1.0L;
      }
    }
  }
  if (*info || (w <= 0.0L))
    return;

  /* find the remaining pivots */
  const unsigned b = (*n >> 1u);
  if (l) {
    for (unsigned a = 0u; a < b; ++a) {
      ++*info;
      unsigned p = 0u;
      unsigned q = 0u;
      pvn_djs_xdec_(&w, &p, &q);
      if (!p || (p > *n) || !q || (q > *n)) {
        *info = -4;
        return;
      }
      const unsigned k_ = ((m + a) << 1u);
      o[k_] = p;
      o[k_ + 1u] = q;
      if ((a + 1u) < b) {
        w = -1.0L;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(d,m,n,p,q,info) reduction(max:w)
#endif /* _OPENMP */
        for (unsigned k = 0u; k < m; ++k) {
          if (d[k] > 0.0L) {
            unsigned i = 0u;
            unsigned j = 0u;
            pvn_djs_xdec_((d + k), &i, &j);
            if (!i || (i > *n) || !j || (j > *n))
              *info = -4;
            else if ((i != p) && (i != q) && (j != p) && (j != q))
              w = fmaxl(w, d[k]);
            else /* colliding */
              d[k] = -1.0L;
          }
        }
        if ((*info < 0) || (w <= 0.0L))
          break;
      }
    }
  }
  else {
    for (unsigned a = 0u; a < b; ++a) {
      ++*info;
      unsigned p = 0u;
      unsigned q = 0u;
      pvn_djs_xdec_(&w, &p, &q);
      if (!p || (p > *n) || !q || (q > *n)) {
        *info = -4;
        return;
      }
      const unsigned k_ = ((m + a) << 1u);
      o[k_] = p;
      o[k_ + 1u] = q;
      if ((a + 1u) < b) {
        w = -1.0L;
        for (unsigned k = 0u; k < m; ++k) {
          if (d[k] > 0.0L) {
            unsigned i = 0u;
            unsigned j = 0u;
            pvn_djs_xdec_((d + k), &i, &j);
            if (!i || (i > *n) || !j || (j > *n))
              *info = -4;
            else if ((i != p) && (i != q) && (j != p) && (j != q))
              w = fmaxl(w, d[k]);
            else /* colliding */
              d[k] = -1.0L;
          }
        }
        if ((*info < 0) || (w <= 0.0L))
          break;
      }
    }
  }
}
#endif /* __x86_64__ */
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
  *(unsigned __int128*)e = (*(const unsigned __int128*)e | ((unsigned __int128)((*p - 1u) & 1073741823u) << 30u) | (unsigned __int128)((*q - 1u) & 1073741823u));
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
  PVN_ASSERT(*p);
  PVN_ASSERT(*q);
  *e = (long double)*d;
  *(unsigned __int128*)e = (*(const unsigned __int128*)e | ((unsigned __int128)((*p - 1u) & 1073741823u) << 30u) | ((unsigned __int128)((*q - 1u) & 1073741823u) << 30u));
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

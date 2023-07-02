#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_rvis_start_f(pvn_rvis_ctx_f **const ctx, const unsigned m, const unsigned n, const pvn_rop_f op, const char *const fnB)
{
  if (!ctx)
    return -1;
  if (!m)
    return -2;
  if (!n)
    return -3;
  if (!op)
    return -4;
  if (!fnB)
    return -5;
  if (!*fnB)
    return -5;

  *ctx = (pvn_rvis_ctx_f*)malloc(sizeof(pvn_rvis_ctx_f));
  PVN_SYSP_CALL(*ctx);
  (*ctx)->minB =  INFINITY;
  (*ctx)->maxB = -INFINITY;
  (*ctx)->op = op;
  (*ctx)->B = (float*)malloc(m * (n * sizeof(float)));
  PVN_SYSP_CALL((*ctx)->B);
  (*ctx)->m = m;
  (*ctx)->n = n;
  (*ctx)->cnt = 0u;
  (*ctx)->fdB = open(fnB, (O_RDWR | O_CREAT | O_TRUNC
#ifdef _LARGEFILE64_SOURCE
                           | O_LARGEFILE
#endif /* _LARGEFILE64_SOURCE */
                           ), (S_IRUSR | S_IWUSR));
  PVN_SYSI_CALL(-1 == (*ctx)->fdB);
  return 0;
}

int pvn_rvis_start(pvn_rvis_ctx **const ctx, const unsigned m, const unsigned n, const pvn_rop op, const char *const fnB)
{
  if (!ctx)
    return -1;
  if (!m)
    return -2;
  if (!n)
    return -3;
  if (!op)
    return -4;
  if (!fnB)
    return -5;
  if (!*fnB)
    return -5;

  *ctx = (pvn_rvis_ctx*)malloc(sizeof(pvn_rvis_ctx));
  PVN_SYSP_CALL(*ctx);
  (*ctx)->minB =  INFINITY;
  (*ctx)->maxB = -INFINITY;
  (*ctx)->op = op;
  (*ctx)->B = (double*)malloc(m * (n * sizeof(double)));
  PVN_SYSP_CALL((*ctx)->B);
  (*ctx)->m = m;
  (*ctx)->n = n;
  (*ctx)->cnt = 0u;
  (*ctx)->fdB = open(fnB, (O_RDWR | O_CREAT | O_TRUNC
#ifdef _LARGEFILE64_SOURCE
                           | O_LARGEFILE
#endif /* _LARGEFILE64_SOURCE */
                           ), (S_IRUSR | S_IWUSR));
  PVN_SYSI_CALL(-1 == (*ctx)->fdB);
  return 0;
}

int pvn_rvis_start_l(pvn_rvis_ctx_l **const ctx, const unsigned m, const unsigned n, const pvn_rop_l op, const char *const fnB)
{
  if (!ctx)
    return -1;
  if (!m)
    return -2;
  if (!n)
    return -3;
  if (!op)
    return -4;
  if (!fnB)
    return -5;
  if (!*fnB)
    return -5;

  *ctx = (pvn_rvis_ctx_l*)malloc(sizeof(pvn_rvis_ctx_l));
  PVN_SYSP_CALL(*ctx);
  (*ctx)->minB =  INFINITY;
  (*ctx)->maxB = -INFINITY;
  (*ctx)->op = op;
  (*ctx)->B = (long double*)malloc(m * (n * sizeof(long double)));
  PVN_SYSP_CALL((*ctx)->B);
  (*ctx)->m = m;
  (*ctx)->n = n;
  (*ctx)->cnt = 0u;
  (*ctx)->fdB = open(fnB, (O_RDWR | O_CREAT | O_TRUNC
#ifdef _LARGEFILE64_SOURCE
                           | O_LARGEFILE
#endif /* _LARGEFILE64_SOURCE */
                           ), (S_IRUSR | S_IWUSR));
  PVN_SYSI_CALL(-1 == (*ctx)->fdB);
  return 0;
}

int pvn_rvis_frame_f(pvn_rvis_ctx_f *const ctx, const float *const restrict A, const size_t ldA)
{
  if (!ctx)
    return -1;
  if (!A)
    return -2;
  if (!ldA)
    return -3;
  size_t sz = (size_t)(ctx->m);
  const int ret = (ctx->op)(ctx->m, ctx->n, A, ldA, ctx->B, sz, &(ctx->minB), &(ctx->maxB));
  if (ret)
    return ret;
  sz *= ((ctx->n) * sizeof(float));
  PVN_SYSI_CALL((ssize_t)sz != write(ctx->fdB, ctx->B, sz));
  ++(ctx->cnt);
  return ret;
}

int pvn_rvis_frame(pvn_rvis_ctx *const ctx, const double *const restrict A, const size_t ldA)
{
  if (!ctx)
    return -1;
  if (!A)
    return -2;
  if (!ldA)
    return -3;
  size_t sz = (size_t)(ctx->m);
  const int ret = (ctx->op)(ctx->m, ctx->n, A, ldA, ctx->B, sz, &(ctx->minB), &(ctx->maxB));
  if (ret)
    return ret;
  sz *= ((ctx->n) * sizeof(double));
  PVN_SYSI_CALL((ssize_t)sz != write(ctx->fdB, ctx->B, sz));
  ++(ctx->cnt);
  return ret;
}

int pvn_rvis_frame_l(pvn_rvis_ctx_l *const ctx, const long double *const restrict A, const size_t ldA)
{
  if (!ctx)
    return -1;
  if (!A)
    return -2;
  if (!ldA)
    return -3;
  size_t sz = (size_t)(ctx->m);
  const int ret = (ctx->op)(ctx->m, ctx->n, A, ldA, ctx->B, sz, &(ctx->minB), &(ctx->maxB));
  if (ret)
    return ret;
  sz *= ((ctx->n) * sizeof(long double));
  PVN_SYSI_CALL((ssize_t)sz != write(ctx->fdB, ctx->B, sz));
  ++(ctx->cnt);
  return ret;
}

int pvn_rvis_stop_f(pvn_rvis_ctx_f *const ctx, const unsigned sx, const unsigned sy, const unsigned bppB, const char *const bnB)
{
  if (!ctx)
    return -1;
  if (!sx)
    return -2;
  if (!sy)
    return -3;
  if (!bnB)
    return -5;
  if (!*bnB)
    return -5;

  float ubc, mid, shf;
  if (bppB == 4u) {
    ubc = 15.0f;
    mid = 7.0f;
    shf = 13.0f;
  }
  else if (bppB == 8u) {
    ubc = 255.0f;
    mid = 127.0f;
    shf = 253.0f;
  }
  else
    return -4;
  const float wid = ctx->maxB - ctx->minB;

  if (!(ctx->cnt))
    return 0;
  const char *fmt = "-%010u.bmp";
  if (ctx->cnt <= 10u)
    fmt = "-%1u.bmp";
  else if (ctx->cnt <= 100u)
    fmt = "-%02u.bmp";
  else if (ctx->cnt <= 1000u)
    fmt = "-%03u.bmp";
  else if (ctx->cnt <= 10000u)
    fmt = "-%04u.bmp";
  else if (ctx->cnt <= 100000u)
    fmt = "-%05u.bmp";
  else if (ctx->cnt <= 1000000u)
    fmt = "-%06u.bmp";
  else if (ctx->cnt <= 10000000u)
    fmt = "-%07u.bmp";
  else if (ctx->cnt <= 100000000u)
    fmt = "-%08u.bmp";
  else if (ctx->cnt <= 1000000000u)
    fmt = "-%09u.bmp";

  const size_t bnl = strlen(bnB);
  char fn[bnl + 16u];
  strcpy(fn, bnB)[bnl] = '.';
  fn[bnl + 1u] = 'p';
  fn[bnl + 2u] = 'l';
  fn[bnl + 3u] = 't';
  fn[bnl + 4u] = '\0';

  pvn_bmp_t bmp = (pvn_bmp_t)NULL;
  if (pvn_bmp_create(&bmp, (ctx->n * sx), ((int)(ctx->m) * -(int)sy), bppB)) {
    PVN_STOP("pvn_bmp_create");
  }
  if (pvn_bmp_read_cmap(bmp, fn)) {
    PVN_STOP("pvn_bmp_read_cmap");
  }
  const pvn_bmp_pixel_setter_t ps = pvn_bmp_get_pixel_setter(bmp);
  if (!ps) {
    PVN_STOP("pvn_bmp_get_pixel_setter");
  }
  const size_t ldB = (size_t)(ctx->m);
  const size_t sz = ldB * ((ctx->n) * sizeof(float));
  PVN_SYSI_CALL(lseek(ctx->fdB, 0, SEEK_SET));
  for (unsigned f = 0u; f < ctx->cnt; ++f) {
    PVN_SYSI_CALL((ssize_t)sz != read(ctx->fdB, ctx->B, sz));
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(ctx,ldB,ubc,wid,mid,shf)
#endif /* _OPENMP */
    for (unsigned j = 0u; j < ctx->n; ++j) {
      float *const cB = ctx->B + j * ldB;
      for (unsigned i = 0u; i < ctx->m; ++i) {
        float *const b = cB + i;
        if (*b == -INFINITY)
          *b = 0.0f;
        else if (!isfinite(*b)) /* +Inf || NaN */
          *b = ubc;
        else if (wid == 0.0f)
          *b = mid;
        else
          *b = roundf(fmaf((*b - ctx->minB) / wid, shf, 1.0f));
      }
    }
    for (unsigned j = 0u; j < ctx->n; ++j) {
      const float *const cB = ctx->B + j * ldB;
      const unsigned jsx = j * sx;
      for (unsigned i = 0u; i < ctx->m; ++i) {
        const unsigned isy = i * sy;
        const unsigned c = (unsigned)(cB[i]);
        for (unsigned ii = 0u; ii < sy; ++ii) {
          const unsigned iisy = isy + ii;
          for (unsigned jj = 0u; jj < sx; ++jj)
            ps(bmp, (jsx + jj), iisy, c);
        }
      }
    }
    PVN_SYSI_CALL(sprintf((fn + bnl), fmt, f) <= 0);
    if (pvn_bmp_fwrite(bmp, fn)) {
      PVN_STOP("pvn_bmp_fwrite");
    }
  }
  pvn_bmp_destroy(bmp);

  PVN_SYSI_CALL(lseek(ctx->fdB, 0, SEEK_SET));
  PVN_SYSI_CALL(ftruncate(ctx->fdB, 0));
  PVN_SYSI_CALL(dprintf(ctx->fdB, "m: %u\nn: %u\nsx: %u\nsy: %u\nbpp: %u\ncnt: %u\nmin: %s\nmax: ", ctx->m, ctx->n, sx, sy, bppB, ctx->cnt, pvn_stoa(fn, ctx->minB)) <= 0);
  PVN_SYSI_CALL(dprintf(ctx->fdB, "%s\n", pvn_stoa(fn, ctx->maxB)) <= 0);
  PVN_SYSI_CALL(close(ctx->fdB));
  free(ctx->B);
  free(ctx);
  return 0;
}

int pvn_rvis_stop(pvn_rvis_ctx *const ctx, const unsigned sx, const unsigned sy, const unsigned bppB, const char *const bnB)
{
  if (!ctx)
    return -1;
  if (!sx)
    return -2;
  if (!sy)
    return -3;
  if (!bnB)
    return -5;
  if (!*bnB)
    return -5;

  double ubc, mid, shf;
  if (bppB == 4u) {
    ubc = 15.0;
    mid = 7.0;
    shf = 13.0;
  }
  else if (bppB == 8u) {
    ubc = 255.0;
    mid = 127.0;
    shf = 253.0;
  }
  else
    return -4;
  const double wid = ctx->maxB - ctx->minB;

  if (!(ctx->cnt))
    return 0;
  const char *fmt = "-%010u.bmp";
  if (ctx->cnt <= 10u)
    fmt = "-%1u.bmp";
  else if (ctx->cnt <= 100u)
    fmt = "-%02u.bmp";
  else if (ctx->cnt <= 1000u)
    fmt = "-%03u.bmp";
  else if (ctx->cnt <= 10000u)
    fmt = "-%04u.bmp";
  else if (ctx->cnt <= 100000u)
    fmt = "-%05u.bmp";
  else if (ctx->cnt <= 1000000u)
    fmt = "-%06u.bmp";
  else if (ctx->cnt <= 10000000u)
    fmt = "-%07u.bmp";
  else if (ctx->cnt <= 100000000u)
    fmt = "-%08u.bmp";
  else if (ctx->cnt <= 1000000000u)
    fmt = "-%09u.bmp";

  const size_t bnl = strlen(bnB);
  char fn[bnl + 25u];
  strcpy(fn, bnB)[bnl] = '.';
  fn[bnl + 1u] = 'p';
  fn[bnl + 2u] = 'l';
  fn[bnl + 3u] = 't';
  fn[bnl + 4u] = '\0';

  pvn_bmp_t bmp = (pvn_bmp_t)NULL;
  if (pvn_bmp_create(&bmp, (ctx->n * sx), ((int)(ctx->m) * -(int)sy), bppB)) {
    PVN_STOP("pvn_bmp_create");
  }
  if (pvn_bmp_read_cmap(bmp, fn)) {
    PVN_STOP("pvn_bmp_read_cmap");
  }
  const pvn_bmp_pixel_setter_t ps = pvn_bmp_get_pixel_setter(bmp);
  if (!ps) {
    PVN_STOP("pvn_bmp_get_pixel_setter");
  }
  const size_t ldB = (size_t)(ctx->m);
  const size_t sz = ldB * ((ctx->n) * sizeof(double));
  PVN_SYSI_CALL(lseek(ctx->fdB, 0, SEEK_SET));
  for (unsigned f = 0u; f < ctx->cnt; ++f) {
    PVN_SYSI_CALL((ssize_t)sz != read(ctx->fdB, ctx->B, sz));
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(ctx,ldB,ubc,wid,mid,shf)
#endif /* _OPENMP */
    for (unsigned j = 0u; j < ctx->n; ++j) {
      double *const cB = ctx->B + j * ldB;
      for (unsigned i = 0u; i < ctx->m; ++i) {
        double *const b = cB + i;
        if (*b == -INFINITY)
          *b = 0.0;
        else if (!isfinite(*b)) /* +Inf || NaN */
          *b = ubc;
        else if (wid == 0.0)
          *b = mid;
        else
          *b = round(fma((*b - ctx->minB) / wid, shf, 1.0));
      }
    }
    for (unsigned j = 0u; j < ctx->n; ++j) {
      const double *const cB = ctx->B + j * ldB;
      const unsigned jsx = j * sx;
      for (unsigned i = 0u; i < ctx->m; ++i) {
        const unsigned isy = i * sy;
        const unsigned c = (unsigned)(cB[i]);
        for (unsigned ii = 0u; ii < sy; ++ii) {
          const unsigned iisy = isy + ii;
          for (unsigned jj = 0u; jj < sx; ++jj)
            ps(bmp, (jsx + jj), iisy, c);
        }
      }
    }
    PVN_SYSI_CALL(sprintf((fn + bnl), fmt, f) <= 0);
    if (pvn_bmp_fwrite(bmp, fn)) {
      PVN_STOP("pvn_bmp_fwrite");
    }
  }
  pvn_bmp_destroy(bmp);

  PVN_SYSI_CALL(lseek(ctx->fdB, 0, SEEK_SET));
  PVN_SYSI_CALL(ftruncate(ctx->fdB, 0));
  PVN_SYSI_CALL(dprintf(ctx->fdB, "m: %u\nn: %u\nsx: %u\nsy: %u\nbpp: %u\ncnt: %u\nmin: %s\nmax: ", ctx->m, ctx->n, sx, sy, bppB, ctx->cnt, pvn_stoa(fn, ctx->minB)) <= 0);
  PVN_SYSI_CALL(dprintf(ctx->fdB, "%s\n", pvn_dtoa(fn, ctx->maxB)) <= 0);
  PVN_SYSI_CALL(close(ctx->fdB));
  free(ctx->B);
  free(ctx);
  return 0;
}

int pvn_rvis_stop_l(pvn_rvis_ctx_l *const ctx, const unsigned sx, const unsigned sy, const unsigned bppB, const char *const bnB)
{
  if (!ctx)
    return -1;
  if (!sx)
    return -2;
  if (!sy)
    return -3;
  if (!bnB)
    return -5;
  if (!*bnB)
    return -5;

  long double ubc, mid, shf;
  if (bppB == 4u) {
    ubc = 15.0L;
    mid = 7.0L;
    shf = 13.0L;
  }
  else if (bppB == 8u) {
    ubc = 255.0L;
    mid = 127.0L;
    shf = 253.0L;
  }
  else
    return -4;
  const long double wid = ctx->maxB - ctx->minB;

  if (!(ctx->cnt))
    return 0;
  const char *fmt = "-%010u.bmp";
  if (ctx->cnt <= 10u)
    fmt = "-%1u.bmp";
  else if (ctx->cnt <= 100u)
    fmt = "-%02u.bmp";
  else if (ctx->cnt <= 1000u)
    fmt = "-%03u.bmp";
  else if (ctx->cnt <= 10000u)
    fmt = "-%04u.bmp";
  else if (ctx->cnt <= 100000u)
    fmt = "-%05u.bmp";
  else if (ctx->cnt <= 1000000u)
    fmt = "-%06u.bmp";
  else if (ctx->cnt <= 10000000u)
    fmt = "-%07u.bmp";
  else if (ctx->cnt <= 100000000u)
    fmt = "-%08u.bmp";
  else if (ctx->cnt <= 1000000000u)
    fmt = "-%09u.bmp";

  const size_t bnl = strlen(bnB);
  char fn[bnl +
#ifdef __x86_64__
          30u
#else /* !__x86_64__ */
          45
#endif /* ?__x86_64__ */
          ];
  strcpy(fn, bnB)[bnl] = '.';
  fn[bnl + 1u] = 'p';
  fn[bnl + 2u] = 'l';
  fn[bnl + 3u] = 't';
  fn[bnl + 4u] = '\0';

  pvn_bmp_t bmp = (pvn_bmp_t)NULL;
  if (pvn_bmp_create(&bmp, (ctx->n * sx), ((int)(ctx->m) * -(int)sy), bppB)) {
    PVN_STOP("pvn_bmp_create");
  }
  if (pvn_bmp_read_cmap(bmp, fn)) {
    PVN_STOP("pvn_bmp_read_cmap");
  }
  const pvn_bmp_pixel_setter_t ps = pvn_bmp_get_pixel_setter(bmp);
  if (!ps) {
    PVN_STOP("pvn_bmp_get_pixel_setter");
  }
  const size_t ldB = (size_t)(ctx->m);
  const size_t sz = ldB * ((ctx->n) * sizeof(long double));
  PVN_SYSI_CALL(lseek(ctx->fdB, 0, SEEK_SET));
  for (unsigned f = 0u; f < ctx->cnt; ++f) {
    PVN_SYSI_CALL((ssize_t)sz != read(ctx->fdB, ctx->B, sz));
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(ctx,ldB,ubc,wid,mid,shf)
#endif /* _OPENMP */
    for (unsigned j = 0u; j < ctx->n; ++j) {
      long double *const cB = ctx->B + j * ldB;
      for (unsigned i = 0u; i < ctx->m; ++i) {
        long double *const b = cB + i;
        if (*b == -INFINITY)
          *b = 0.0L;
        else if (!isfinite(*b)) /* +Inf || NaN */
          *b = ubc;
        else if (wid == 0.0L)
          *b = mid;
        else
          *b = roundl(fmal((*b - ctx->minB) / wid, shf, 1.0L));
      }
    }
    for (unsigned j = 0u; j < ctx->n; ++j) {
      const long double *const cB = ctx->B + j * ldB;
      const unsigned jsx = j * sx;
      for (unsigned i = 0u; i < ctx->m; ++i) {
        const unsigned isy = i * sy;
        const unsigned c = (unsigned)(cB[i]);
        for (unsigned ii = 0u; ii < sy; ++ii) {
          const unsigned iisy = isy + ii;
          for (unsigned jj = 0u; jj < sx; ++jj)
            ps(bmp, (jsx + jj), iisy, c);
        }
      }
    }
    PVN_SYSI_CALL(sprintf((fn + bnl), fmt, f) <= 0);
    if (pvn_bmp_fwrite(bmp, fn)) {
      PVN_STOP("pvn_bmp_fwrite");
    }
  }
  pvn_bmp_destroy(bmp);

  PVN_SYSI_CALL(lseek(ctx->fdB, 0, SEEK_SET));
  PVN_SYSI_CALL(ftruncate(ctx->fdB, 0));
  PVN_SYSI_CALL(dprintf(ctx->fdB, "m: %u\nn: %u\nsx: %u\nsy: %u\nbpp: %u\ncnt: %u\nmin: %s\nmax: ", ctx->m, ctx->n, sx, sy, bppB, ctx->cnt, pvn_stoa(fn, ctx->minB)) <= 0);
  PVN_SYSI_CALL(dprintf(ctx->fdB, "%s\n", pvn_xtoa(fn, ctx->maxB)) <= 0);
  PVN_SYSI_CALL(close(ctx->fdB));
  free(ctx->B);
  free(ctx);
  return 0;
}

int pvn_cvis_start_f(pvn_cvis_ctx_f **const ctx, const unsigned m, const unsigned n, const pvn_cop_f op, const char *const fnB, const char *const fnC)
{
  if (!ctx)
    return -1;
  if (!m)
    return -2;
  if (!n)
    return -3;
  if (!op)
    return -4;
  if (!fnB)
    return -5;
  if (!*fnB)
    return -5;
  if (!fnC)
    return -6;
  if (!*fnC)
    return -6;

  *ctx = (pvn_cvis_ctx_f*)malloc(sizeof(pvn_cvis_ctx_f));
  PVN_SYSP_CALL(*ctx);
  (*ctx)->minB =  INFINITY;
  (*ctx)->maxB = -INFINITY;
  (*ctx)->op = op;
  const size_t sz = m * (n * sizeof(float));
  (*ctx)->B = (float*)malloc(sz);
  PVN_SYSP_CALL((*ctx)->B);
  (*ctx)->C = (float*)malloc(sz);
  PVN_SYSP_CALL((*ctx)->C);
  (*ctx)->m = m;
  (*ctx)->n = n;
  (*ctx)->cnt = 0u;
  (*ctx)->fdB = open(fnB, (O_RDWR | O_CREAT | O_TRUNC
#ifdef _LARGEFILE64_SOURCE
                           | O_LARGEFILE
#endif /* _LARGEFILE64_SOURCE */
                           ), (S_IRUSR | S_IWUSR));
  PVN_SYSI_CALL(-1 == (*ctx)->fdB);
  (*ctx)->fdC = open(fnC, (O_RDWR | O_CREAT | O_TRUNC
#ifdef _LARGEFILE64_SOURCE
                           | O_LARGEFILE
#endif /* _LARGEFILE64_SOURCE */
                           ), (S_IRUSR | S_IWUSR));
  PVN_SYSI_CALL(-1 == (*ctx)->fdC);
  return ((*ctx)->err = 0);
}

int pvn_cvis_start(pvn_cvis_ctx **const ctx, const unsigned m, const unsigned n, const pvn_cop op, const char *const fnB, const char *const fnC)
{
  if (!ctx)
    return -1;
  if (!m)
    return -2;
  if (!n)
    return -3;
  if (!op)
    return -4;
  if (!fnB)
    return -5;
  if (!*fnB)
    return -5;
  if (!fnC)
    return -6;
  if (!*fnC)
    return -6;

  *ctx = (pvn_cvis_ctx*)malloc(sizeof(pvn_cvis_ctx));
  PVN_SYSP_CALL(*ctx);
  (*ctx)->minB =  INFINITY;
  (*ctx)->maxB = -INFINITY;
  (*ctx)->op = op;
  const size_t sz = m * (n * sizeof(double));
  (*ctx)->B = (double*)malloc(sz);
  PVN_SYSP_CALL((*ctx)->B);
  (*ctx)->C = (double*)malloc(sz);
  PVN_SYSP_CALL((*ctx)->C);
  (*ctx)->m = m;
  (*ctx)->n = n;
  (*ctx)->cnt = 0u;
  (*ctx)->fdB = open(fnB, (O_RDWR | O_CREAT | O_TRUNC
#ifdef _LARGEFILE64_SOURCE
                           | O_LARGEFILE
#endif /* _LARGEFILE64_SOURCE */
                           ), (S_IRUSR | S_IWUSR));
  PVN_SYSI_CALL(-1 == (*ctx)->fdB);
  (*ctx)->fdC = open(fnC, (O_RDWR | O_CREAT | O_TRUNC
#ifdef _LARGEFILE64_SOURCE
                           | O_LARGEFILE
#endif /* _LARGEFILE64_SOURCE */
                           ), (S_IRUSR | S_IWUSR));
  PVN_SYSI_CALL(-1 == (*ctx)->fdC);
  return ((*ctx)->err = 0);
}

int pvn_cvis_start_l(pvn_cvis_ctx_l **const ctx, const unsigned m, const unsigned n, const pvn_cop_l op, const char *const fnB, const char *const fnC)
{
  if (!ctx)
    return -1;
  if (!m)
    return -2;
  if (!n)
    return -3;
  if (!op)
    return -4;
  if (!fnB)
    return -5;
  if (!*fnB)
    return -5;
  if (!fnC)
    return -6;
  if (!*fnC)
    return -6;

  *ctx = (pvn_cvis_ctx_l*)malloc(sizeof(pvn_cvis_ctx_l));
  PVN_SYSP_CALL(*ctx);
  (*ctx)->minB =  INFINITY;
  (*ctx)->maxB = -INFINITY;
  (*ctx)->op = op;
  const size_t sz = m * (n * sizeof(long double));
  (*ctx)->B = (long double*)malloc(sz);
  PVN_SYSP_CALL((*ctx)->B);
  (*ctx)->C = (long double*)malloc(sz);
  PVN_SYSP_CALL((*ctx)->C);
  (*ctx)->m = m;
  (*ctx)->n = n;
  (*ctx)->cnt = 0u;
  (*ctx)->fdB = open(fnB, (O_RDWR | O_CREAT | O_TRUNC
#ifdef _LARGEFILE64_SOURCE
                           | O_LARGEFILE
#endif /* _LARGEFILE64_SOURCE */
                           ), (S_IRUSR | S_IWUSR));
  PVN_SYSI_CALL(-1 == (*ctx)->fdB);
  (*ctx)->fdC = open(fnC, (O_RDWR | O_CREAT | O_TRUNC
#ifdef _LARGEFILE64_SOURCE
                           | O_LARGEFILE
#endif /* _LARGEFILE64_SOURCE */
                           ), (S_IRUSR | S_IWUSR));
  PVN_SYSI_CALL(-1 == (*ctx)->fdC);
  return ((*ctx)->err = 0);
}

int pvn_cvis_frame_f(pvn_cvis_ctx_f *const ctx, const float complex *const restrict A, const size_t ldA)
{
  if (!ctx)
    return -1;
  if (!A)
    return -2;
  if (!ldA)
    return -3;
  size_t sz = (size_t)(ctx->m);
  ctx->err = (ctx->op)(ctx->m, ctx->n, A, ldA, ctx->B, sz, ctx->C, sz, &(ctx->minB), &(ctx->maxB), &(ctx->minC), &(ctx->maxC));
  if (ctx->err < 0)
    return (ctx->err);
  sz *= ((ctx->n) * sizeof(float));
  PVN_SYSI_CALL((ssize_t)sz != write(ctx->fdB, ctx->B, sz));
  PVN_SYSI_CALL((ssize_t)sz != write(ctx->fdC, ctx->C, sz));
  ++(ctx->cnt);
  return (ctx->err);
}

int pvn_cvis_frame(pvn_cvis_ctx *const ctx, const double complex *const restrict A, const size_t ldA)
{
  if (!ctx)
    return -1;
  if (!A)
    return -2;
  if (!ldA)
    return -3;
  size_t sz = (size_t)(ctx->m);
  ctx->err = (ctx->op)(ctx->m, ctx->n, A, ldA, ctx->B, sz, ctx->C, sz, &(ctx->minB), &(ctx->maxB), &(ctx->minC), &(ctx->maxC));
  if (ctx->err < 0)
    return (ctx->err);
  sz *= ((ctx->n) * sizeof(double));
  PVN_SYSI_CALL((ssize_t)sz != write(ctx->fdB, ctx->B, sz));
  PVN_SYSI_CALL((ssize_t)sz != write(ctx->fdC, ctx->C, sz));
  ++(ctx->cnt);
  return (ctx->err);
}

int pvn_cvis_frame_l(pvn_cvis_ctx_l *const ctx, const long double complex *const restrict A, const size_t ldA)
{
  if (!ctx)
    return -1;
  if (!A)
    return -2;
  if (!ldA)
    return -3;
  size_t sz = (size_t)(ctx->m);
  ctx->err = (ctx->op)(ctx->m, ctx->n, A, ldA, ctx->B, sz, ctx->C, sz, &(ctx->minB), &(ctx->maxB), &(ctx->minC), &(ctx->maxC));
  if (ctx->err < 0)
    return (ctx->err);
  sz *= ((ctx->n) * sizeof(long double));
  PVN_SYSI_CALL((ssize_t)sz != write(ctx->fdB, ctx->B, sz));
  PVN_SYSI_CALL((ssize_t)sz != write(ctx->fdC, ctx->C, sz));
  ++(ctx->cnt);
  return (ctx->err);
}

int pvn_rop_idf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  float mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const float x = (B[i + offB] = A[i + offA]);
      if (isfinite(x)) {
        mB = fminf(mB,  x);
        MB = fminf(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_id(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const double x = (B[i + offB] = A[i + offA]);
      if (isfinite(x)) {
        mB = fmin(mB,  x);
        MB = fmin(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_idl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  long double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const long double x = (B[i + offB] = A[i + offA]);
      if (isfinite(x)) {
        mB = fminl(mB,  x);
        MB = fminl(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_absf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  float mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const float x = (B[i + offB] = fabsf(A[i + offA]));
      if (isfinite(x)) {
        mB = fminf(mB,  x);
        MB = fminf(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_abs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const double x = (B[i + offB] = fabs(A[i + offA]));
      if (isfinite(x)) {
        mB = fmin(mB,  x);
        MB = fmin(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_absl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  long double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const long double x = (B[i + offB] = fabsl(A[i + offA]));
      if (isfinite(x)) {
        mB = fminl(mB,  x);
        MB = fminl(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_lgabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  float mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const float x = (B[i + offB] = log2f(fabsf(A[i + offA])));
      if (isfinite(x)) {
        mB = fminf(mB,  x);
        MB = fminf(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_lgabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const double x = (B[i + offB] = log2(fabs(A[i + offA])));
      if (isfinite(x)) {
        mB = fmin(mB,  x);
        MB = fmin(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_lgabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  long double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const long double x = (B[i + offB] = log2l(fabsl(A[i + offA])));
      if (isfinite(x)) {
        mB = fminl(mB,  x);
        MB = fminl(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_logabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  float mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const float x = (B[i + offB] = log10f(fabsf(A[i + offA])));
      if (isfinite(x)) {
        mB = fminf(mB,  x);
        MB = fminf(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_logabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const double x = (B[i + offB] = log10(fabs(A[i + offA])));
      if (isfinite(x)) {
        mB = fmin(mB,  x);
        MB = fmin(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_rop_logabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!minB)
    return -7;
  if (!maxB)
    return -8;
  if (m > pvn_umin(ldA, ldB))
    return -9;
  long double mB = INFINITY, MB = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB) reduction(min:mB,MB)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    for (unsigned i = 0u; i < m; ++i) {
      const long double x = (B[i + offB] = log10l(fabsl(A[i + offA])));
      if (isfinite(x)) {
        mB = fminl(mB,  x);
        MB = fminl(MB, -x);
      }
    }
  }
  MB = -MB;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  return 0;
}

int pvn_cop_idf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  float mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      const float x = (B[i + offB] = crealf(z));
      const float y = (C[i + offC] = cimagf(z));
      if (isfinite(x)) {
        mB = fminf(mB,  x);
        MB = fminf(MB, -x);
      }
      if (isfinite(y)) {
        mC = fminf(mC,  y);
        MC = fminf(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  return 0;
}

int pvn_cop_id(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      const double x = (B[i + offB] = creal(z));
      const double y = (C[i + offC] = cimag(z));
      if (isfinite(x)) {
        mB = fmin(mB,  x);
        MB = fmin(MB, -x);
      }
      if (isfinite(y)) {
        mC = fmin(mC,  y);
        MC = fmin(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  return 0;
}

int pvn_cop_idl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  long double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      const long double x = (B[i + offB] = creall(z));
      const long double y = (C[i + offC] = cimagl(z));
      if (isfinite(x)) {
        mB = fminl(mB,  x);
        MB = fminl(MB, -x);
      }
      if (isfinite(y)) {
        mC = fminl(mC,  y);
        MC = fminl(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  return 0;
}

int pvn_cop_absf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  float mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      const float x = (B[i + offB] = cabsf(z));
      const float y = (C[i + offC] = cargf(z));
      if (isfinite(x)) {
        mB = fminf(mB,  x);
        MB = fminf(MB, -x);
      }
      if (isfinite(y)) {
        mC = fminf(mC,  y);
        MC = fminf(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_abs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      const double x = (B[i + offB] = cabs(z));
      const double y = (C[i + offC] = carg(z));
      if (isfinite(x)) {
        mB = fmin(mB,  x);
        MB = fmin(MB, -x);
      }
      if (isfinite(y)) {
        mC = fmin(mC,  y);
        MC = fmin(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_absl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  long double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      const long double x = (B[i + offB] = cabsl(z));
      const long double y = (C[i + offC] = cargl(z));
      if (isfinite(x)) {
        mB = fminl(mB,  x);
        MB = fminl(MB, -x);
      }
      if (isfinite(y)) {
        mC = fminl(mC,  y);
        MC = fminl(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_lgabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  float mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      const float x = (B[i + offB] = log2f(cabsf(z)));
      const float y = (C[i + offC] = cargf(z));
      if (isfinite(x)) {
        mB = fminf(mB,  x);
        MB = fminf(MB, -x);
      }
      if (isfinite(y)) {
        mC = fminf(mC,  y);
        MC = fminf(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_lgabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      const double x = (B[i + offB] = log2(cabs(z)));
      const double y = (C[i + offC] = carg(z));
      if (isfinite(x)) {
        mB = fmin(mB,  x);
        MB = fmin(MB, -x);
      }
      if (isfinite(y)) {
        mC = fmin(mC,  y);
        MC = fmin(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_lgabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  long double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      const long double x = (B[i + offB] = log2l(cabsl(z)));
      const long double y = (C[i + offC] = cargl(z));
      if (isfinite(x)) {
        mB = fminl(mB,  x);
        MB = fminl(MB, -x);
      }
      if (isfinite(y)) {
        mC = fminl(mC,  y);
        MC = fminl(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_logabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  float mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const float complex z = A[i + offA];
      const float x = (B[i + offB] = log10f(cabsf(z)));
      const float y = (C[i + offC] = cargf(z));
      if (isfinite(x)) {
        mB = fminf(mB,  x);
        MB = fminf(MB, -x);
      }
      if (isfinite(y)) {
        mC = fminf(mC,  y);
        MC = fminf(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_logabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const double complex z = A[i + offA];
      const double x = (B[i + offB] = log10(cabs(z)));
      const double y = (C[i + offC] = carg(z));
      if (isfinite(x)) {
        mB = fmin(mB,  x);
        MB = fmin(MB, -x);
      }
      if (isfinite(y)) {
        mC = fmin(mC,  y);
        MC = fmin(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  /* check for overflow of |z| */
  return (MB == INFINITY);
}

int pvn_cop_logabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC)
{
  if (!m)
    return -1;
  if (!n)
    return -2;
  if (!A)
    return -3;
  if (!ldA)
    return -4;
  if (!B)
    return -5;
  if (!ldB)
    return -6;
  if (!C)
    return -7;
  if (!ldC)
    return -8;
  if (!minB)
    return -9;
  if (!maxB)
    return -10;
  if (!minC)
    return -11;
  if (!maxC)
    return -12;
  if (m > pvn_umin(pvn_umin(ldA, ldB), ldC))
    return -13;
  long double mB = INFINITY, MB = INFINITY, mC = INFINITY, MC = INFINITY;
#ifdef _OPENMP
#pragma omp parallel for default(none) shared(m,n,A,ldA,B,ldB,C,ldC) reduction(min:mB,MB,mC,MC)
#endif /* _OPENMP */
  for (unsigned j = 0u; j < n; ++j) {
    const size_t offA = j * ldA;
    const size_t offB = j * ldB;
    const size_t offC = j * ldC;
    for (unsigned i = 0u; i < m; ++i) {
      const long double complex z = A[i + offA];
      const long double x = (B[i + offB] = log10l(cabsl(z)));
      const long double y = (C[i + offC] = cargl(z));
      if (isfinite(x)) {
        mB = fminl(mB,  x);
        MB = fminl(MB, -x);
      }
      if (isfinite(y)) {
        mC = fminl(mC,  y);
        MC = fminl(MC, -y);
      }
    }
  }
  MB = -MB;
  MC = -MC;
  if (isfinite(mB))
    *minB = fminl(*minB, mB);
  if (isfinite(MB))
    *maxB = fmaxl(*maxB, MB);
  if (isfinite(mC))
    *minC = fminl(*minC, mC);
  if (isfinite(MC))
    *maxC = fmaxl(*maxC, MC);
  /* check for overflow of |z| */
  return (MB == INFINITY);
}
#endif /* ?PVN_TEST */

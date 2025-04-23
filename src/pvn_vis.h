#ifndef PVN_VIS_H
#define PVN_VIS_H

#ifndef PVN_H
#error pvn_vis.h not intended for direct inclusion
#endif /* !PVN_H */

typedef int (*pvn_rop_f)(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB);
typedef int (*pvn_rop)(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB);
typedef int (*pvn_rop_l)(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB);

typedef struct {
  float minB, maxB;
  pvn_rop_f op;
  float *B;
  unsigned m, n, cnt;
  int fdB;
} pvn_rvis_ctx_f;

typedef struct {
  double minB, maxB;
  pvn_rop op;
  double *B;
  unsigned m, n, cnt;
  int fdB;
} pvn_rvis_ctx;

typedef struct {
  long double minB, maxB;
  pvn_rop_l op;
  long double *B;
  unsigned m, n, cnt;
  int fdB;
} pvn_rvis_ctx_l;

typedef int (*pvn_cop_f)(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC);
typedef int (*pvn_cop)(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC);
typedef int (*pvn_cop_l)(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC);

typedef struct {
  float minB, maxB, minC, maxC;
  pvn_cop_f op;
  float *B, *C;
  unsigned m, n, cnt;
  int fdB, fdC, err;
} pvn_cvis_ctx_f;

typedef struct {
  double minB, maxB, minC, maxC;
  pvn_cop op;
  double *B, *C;
  unsigned m, n, cnt;
  int fdB, fdC, err;
} pvn_cvis_ctx;

typedef struct {
  long double minB, maxB, minC, maxC;
  pvn_cop_l op;
  long double *B, *C;
  unsigned m, n, cnt;
  int fdB, fdC, err;
} pvn_cvis_ctx_l;

PVN_EXTERN_C int pvn_rvis_start_f(pvn_rvis_ctx_f *const ctx, const unsigned m, const unsigned n, const pvn_rop_f op, const char *const fnB);
PVN_EXTERN_C pvn_rvis_ctx_f* PVN_FABI(pvn_rvis_start_f,PVN_RVIS_START_F)(const unsigned *const m, const unsigned *const n, const unsigned *const fop, const char *const fnB, ...);
PVN_EXTERN_C int pvn_rvis_start(pvn_rvis_ctx *const ctx, const unsigned m, const unsigned n, const pvn_rop op, const char *const fnB);
PVN_EXTERN_C pvn_rvis_ctx* PVN_FABI(pvn_rvis_start,PVN_RVIS_START)(const unsigned *const m, const unsigned *const n, const unsigned *const fop, const char *const fnB, ...);
#if (!defined(_WIN32) || defined(_DLL))
PVN_EXTERN_C int pvn_rvis_start_l(pvn_rvis_ctx_l *const ctx, const unsigned m, const unsigned n, const pvn_rop_l op, const char *const fnB);
PVN_EXTERN_C pvn_rvis_ctx_l* PVN_FABI(pvn_rvis_start_l,PVN_RVIS_START_L)(const unsigned *const m, const unsigned *const n, const unsigned *const fop, const char *const fnB, ...);
#endif /* !_WIN32 || _DLL */

PVN_EXTERN_C int pvn_rvis_frame_f(pvn_rvis_ctx_f *const ctx, const float *const restrict A, const size_t ldA);
PVN_EXTERN_C int PVN_FABI(pvn_rvis_frame_f,PVN_RVIS_FRAME_F)(pvn_rvis_ctx_f *const *const ctx, const float *const restrict A, const size_t *const ldA);
PVN_EXTERN_C int pvn_rvis_frame(pvn_rvis_ctx *const ctx, const double *const restrict A, const size_t ldA);
PVN_EXTERN_C int PVN_FABI(pvn_rvis_frame,PVN_RVIS_FRAME)(pvn_rvis_ctx *const *const ctx, const double *const restrict A, const size_t *const ldA);
#if (!defined(_WIN32) || defined(_DLL))
PVN_EXTERN_C int pvn_rvis_frame_l(pvn_rvis_ctx_l *const ctx, const long double *const restrict A, const size_t ldA);
PVN_EXTERN_C int PVN_FABI(pvn_rvis_frame_l,PVN_RVIS_FRAME_L)(pvn_rvis_ctx_l *const *const ctx, const long double *const restrict A, const size_t *const ldA);
#endif /* !_WIN32 || _DLL */

PVN_EXTERN_C int pvn_rvis_stop_f(pvn_rvis_ctx_f *const ctx, const unsigned sx, const unsigned sy, const unsigned bppB, const char *const bnB);
PVN_EXTERN_C int PVN_FABI(pvn_rvis_stop_f,PVN_RVIS_STOP_F)(pvn_rvis_ctx_f *const *const ctx, const unsigned *const sx, const unsigned *const sy, const unsigned *const bppB, const char *const bnB, ...);
PVN_EXTERN_C int pvn_rvis_stop(pvn_rvis_ctx *const ctx, const unsigned sx, const unsigned sy, const unsigned bppB, const char *const bnB);
PVN_EXTERN_C int PVN_FABI(pvn_rvis_stop,PVN_RVIS_STOP)(pvn_rvis_ctx *const *const ctx, const unsigned *const sx, const unsigned *const sy, const unsigned *const bppB, const char *const bnB, ...);
#if (!defined(_WIN32) || defined(_DLL))
PVN_EXTERN_C int pvn_rvis_stop_l(pvn_rvis_ctx_l *const ctx, const unsigned sx, const unsigned sy, const unsigned bppB, const char *const bnB);
PVN_EXTERN_C int PVN_FABI(pvn_rvis_stop_l,PVN_RVIS_STOP_L)(pvn_rvis_ctx_l *const *const ctx, const unsigned *const sx, const unsigned *const sy, const unsigned *const bppB, const char *const bnB, ...);
#endif /* !_WIN32 || _DLL */

PVN_EXTERN_C int pvn_cvis_start_f(pvn_cvis_ctx_f *const ctx, const unsigned m, const unsigned n, const pvn_cop_f op, const char *const fnB, const char *const fnC);
PVN_EXTERN_C pvn_cvis_ctx_f* PVN_FABI(pvn_cvis_start_f,PVN_CVIS_START_F)(const unsigned *const m, const unsigned *const n, const unsigned *const fop, const char *const fnB, const char *const fnC, ...);
PVN_EXTERN_C int pvn_cvis_start(pvn_cvis_ctx *const ctx, const unsigned m, const unsigned n, const pvn_cop op, const char *const fnB, const char *const fnC);
PVN_EXTERN_C pvn_cvis_ctx* PVN_FABI(pvn_cvis_start,PVN_CVIS_START)(const unsigned *const m, const unsigned *const n, const unsigned *const fop, const char *const fnB, const char *const fnC, ...);
#if (!defined(_WIN32) || defined(_DLL))
PVN_EXTERN_C int pvn_cvis_start_l(pvn_cvis_ctx_l *const ctx, const unsigned m, const unsigned n, const pvn_cop_l op, const char *const fnB, const char *const fnC);
PVN_EXTERN_C pvn_cvis_ctx_l* PVN_FABI(pvn_cvis_start_l,PVN_CVIS_START_L)(const unsigned *const m, const unsigned *const n, const unsigned *const fop, const char *const fnB, const char *const fnC, ...);
#endif /* !_WIN32 || _DLL */

PVN_EXTERN_C int pvn_cvis_frame_f(pvn_cvis_ctx_f *const ctx, const float complex *const restrict A, const size_t ldA);
PVN_EXTERN_C int PVN_FABI(pvn_cvis_frame_f,PVN_CVIS_FRAME_F)(pvn_cvis_ctx_f *const *const ctx, const float complex *const restrict A, const size_t *const ldA);
PVN_EXTERN_C int pvn_cvis_frame(pvn_cvis_ctx *const ctx, const double complex *const restrict A, const size_t ldA);
PVN_EXTERN_C int PVN_FABI(pvn_cvis_frame,PVN_CVIS_FRAME)(pvn_cvis_ctx *const *const ctx, const double complex *const restrict A, const size_t *const ldA);
#if (!defined(_WIN32) || defined(_DLL))
PVN_EXTERN_C int pvn_cvis_frame_l(pvn_cvis_ctx_l *const ctx, const long double complex *const restrict A, const size_t ldA);
PVN_EXTERN_C int PVN_FABI(pvn_cvis_frame_l,PVN_CVIS_FRAME_L)(pvn_cvis_ctx_l *const *const ctx, const long double complex *const restrict A, const size_t *const ldA);
#endif /* !_WIN32 || _DLL */

PVN_EXTERN_C int pvn_cvis_stop_f(pvn_cvis_ctx_f *const ctx, const unsigned sx, const unsigned sy, const unsigned bppB, const char *const bnB, const unsigned bppC, const char *const bnC);
PVN_EXTERN_C int PVN_FABI(pvn_cvis_stop_f,PVN_CVIS_STOP_F)(pvn_cvis_ctx_f *const *const ctx, const unsigned *const sx, const unsigned *const sy, const unsigned *const bppB, const char *const bnB, const unsigned *const bppC, const char *const bnC, ...);
PVN_EXTERN_C int pvn_cvis_stop(pvn_cvis_ctx *const ctx, const unsigned sx, const unsigned sy, const unsigned bppB, const char *const bnB, const unsigned bppC, const char *const bnC);
PVN_EXTERN_C int PVN_FABI(pvn_cvis_stop,PVN_CVIS_STOP)(pvn_cvis_ctx *const *const ctx, const unsigned *const sx, const unsigned *const sy, const unsigned *const bppB, const char *const bnB, const unsigned *const bppC, const char *const bnC, ...);
#if (!defined(_WIN32) || defined(_DLL))
PVN_EXTERN_C int pvn_cvis_stop_l(pvn_cvis_ctx_l *const ctx, const unsigned sx, const unsigned sy, const unsigned bppB, const char *const bnB, const unsigned bppC, const char *const bnC);
PVN_EXTERN_C int PVN_FABI(pvn_cvis_stop_l,PVN_CVIS_STOP_L)(pvn_cvis_ctx_l *const *const ctx, const unsigned *const sx, const unsigned *const sy, const unsigned *const bppB, const char *const bnB, const unsigned *const bppC, const char *const bnC, ...);
#endif /* !_WIN32 || _DLL */

PVN_EXTERN_C int pvn_rop_idf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB);
PVN_EXTERN_C int pvn_rop_id(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB);
PVN_EXTERN_C int pvn_rop_idl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB);

PVN_EXTERN_C int pvn_rop_absf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB);
PVN_EXTERN_C int pvn_rop_abs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB);
PVN_EXTERN_C int pvn_rop_absl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB);

PVN_EXTERN_C int pvn_rop_lgabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB);
PVN_EXTERN_C int pvn_rop_lgabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB);
PVN_EXTERN_C int pvn_rop_lgabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB);

PVN_EXTERN_C int pvn_rop_logabsf(const unsigned m, const unsigned n, const float *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict minB, float *const restrict maxB);
PVN_EXTERN_C int pvn_rop_logabs(const unsigned m, const unsigned n, const double *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict minB, double *const restrict maxB);
PVN_EXTERN_C int pvn_rop_logabsl(const unsigned m, const unsigned n, const long double *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict minB, long double *const restrict maxB);

PVN_EXTERN_C int pvn_cop_idf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC);
PVN_EXTERN_C int pvn_cop_id(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC);
PVN_EXTERN_C int pvn_cop_idl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC);

PVN_EXTERN_C int pvn_cop_absf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC);
PVN_EXTERN_C int pvn_cop_abs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC);
PVN_EXTERN_C int pvn_cop_absl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC);

PVN_EXTERN_C int pvn_cop_lgabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC);
PVN_EXTERN_C int pvn_cop_lgabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC);
PVN_EXTERN_C int pvn_cop_lgabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC);

PVN_EXTERN_C int pvn_cop_logabsf(const unsigned m, const unsigned n, const float complex *const restrict A, const size_t ldA, float *const restrict B, const size_t ldB, float *const restrict C, const size_t ldC, float *const restrict minB, float *const restrict maxB, float *const restrict minC, float *const restrict maxC);
PVN_EXTERN_C int pvn_cop_logabs(const unsigned m, const unsigned n, const double complex *const restrict A, const size_t ldA, double *const restrict B, const size_t ldB, double *const restrict C, const size_t ldC, double *const restrict minB, double *const restrict maxB, double *const restrict minC, double *const restrict maxC);
PVN_EXTERN_C int pvn_cop_logabsl(const unsigned m, const unsigned n, const long double complex *const restrict A, const size_t ldA, long double *const restrict B, const size_t ldB, long double *const restrict C, const size_t ldC, long double *const restrict minB, long double *const restrict maxB, long double *const restrict minC, long double *const restrict maxC);
#endif /* !PVN_VIS_H */

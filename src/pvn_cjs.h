#ifndef PVN_CJS_H
#define PVN_CJS_H

#ifndef PVN_H
#error pvn_cjs.h not intended for direct inclusion
#endif /* !PVN_H */

typedef struct {
  int id, n, p, q;
} pvn_cjs_rolcyc;

typedef struct {
  int id, n, stp, swp;
  const int *tbl; /* start of table */
  const int *nxt; /* iterator */
} pvn_cjs_maneb2;

typedef struct {
  int id, n, stp, swp;
} pvn_cjs_modmod;

PVN_EXTERN_C int pvn_cjs_init(void *const js, const int id, const int n);
PVN_EXTERN_C int PVN_FABI(pvn_cjs_init,PVN_CJS_INIT)(void **const ret, const int *const id, const int *const n);
PVN_EXTERN_C int pvn_cjs_next(void *const js, int *const arr);
PVN_EXTERN_C int PVN_FABI(pvn_cjs_next,PVN_CJS_NEXT)(void *const *const js, int *const arr);
PVN_EXTERN_C int pvn_cjs_free(void *const js);
PVN_EXTERN_C int PVN_FABI(pvn_cjs_free,PVN_CJS_FREE)(void **const js);

#endif /* !PVN_CJS_H */

#ifndef PVN_BIO_H
#define PVN_BIO_H

#ifndef PVN_H
#error pvn_bio.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C int pvn_bopen_ro_(off_t *const sz, const char *const fn, ...);
PVN_EXTERN_C int pvn_bopen_rw_(off_t *const sz, const char *const fn, ...);
PVN_EXTERN_C int pvn_bopen_wo_(off_t *const sz, const char *const fn, ...);
PVN_EXTERN_C int pvn_bclose_(const int *const fd);
PVN_EXTERN_C ssize_t pvn_bwrite_(const int *const fd, const void *const buf, const size_t *const nB, const off_t *const off);
PVN_EXTERN_C ssize_t pvn_bread_(const int *const fd, void *const buf, const size_t *const nB, const off_t *const off);

#ifdef __x86_64__
PVN_EXTERN_C ssize_t pvn_bwrite80_(const int *const fd, const long double *const buf, const size_t *const n, const off_t *const off);
PVN_EXTERN_C ssize_t pvn_bread80_(const int *const fd, long double *const buf, const size_t *const n, const off_t *const off);
#endif /* __x86_64__ */

#endif /* !PVN_BIO_H */

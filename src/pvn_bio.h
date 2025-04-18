#ifndef PVN_BIO_H
#define PVN_BIO_H

#ifndef PVN_H
#error pvn_bio.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C int PVN_FABI(pvn_bopen_ro,PVN_BOPEN_RO)(off_t *const sz, const char *const fn, ...);
PVN_EXTERN_C int PVN_FABI(pvn_bopen_rw,PVN_BOPEN_RW)(off_t *const sz, const char *const fn, ...);
PVN_EXTERN_C int PVN_FABI(pvn_bopen_wo,PVN_BOPEN_WO)(off_t *const sz, const char *const fn, ...);
PVN_EXTERN_C int PVN_FABI(pvn_bclose,PVN_BCLOSE)(const int *const fd);
#ifndef _WIN32
PVN_EXTERN_C ssize_t PVN_FABI(pvn_bwrite,PVN_BWRITE)(const int *const fd, const void *const buf, const size_t *const nB, const off_t *const off);
PVN_EXTERN_C ssize_t PVN_FABI(pvn_bread,PVN_BREAD)(const int *const fd, void *const buf, const size_t *const nB, const off_t *const off);
#ifdef __x86_64__
PVN_EXTERN_C ssize_t PVN_FABI(pvn_bwrite80,PVN_BWRITE80)(const int *const fd, const long double *const buf, const size_t *const n, const off_t *const off);
PVN_EXTERN_C ssize_t PVN_FABI(pvn_bread80,PVN_BREAD80)(const int *const fd, long double *const buf, const size_t *const n, const off_t *const off);
#endif /* __x86_64__ */
#endif /* !_WIN32 */

#endif /* !PVN_BIO_H */

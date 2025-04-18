#ifndef PVN_LOCK_H
#define PVN_LOCK_H

#ifdef _WIN32
#error pvn_lock is not supported on Windows
#endif /* _WIN32 */

#ifndef PVN_H
#error pvn_lock.h not intended for direct inclusion
#endif /* !PVN_H */

typedef pthread_mutex_t pvn_lock_t;

PVN_EXTERN_C size_t pvn_lock_size_(size_t *const);
PVN_EXTERN_C int pvn_lock_init_(pvn_lock_t *const);
PVN_EXTERN_C int pvn_lock_destroy_(pvn_lock_t *const);
PVN_EXTERN_C int pvn_lock_(pvn_lock_t *const);
PVN_EXTERN_C int pvn_unlock_(pvn_lock_t *const);

#endif /* !PVN_LOCK_H */

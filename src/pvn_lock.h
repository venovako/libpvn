#ifndef PVN_LOCK_H
#define PVN_LOCK_H

#ifndef PVN_H
#error pvn_lock.h not intended for direct inclusion
#endif /* !PVN_H */

#ifdef _WIN32
typedef void* pvn_lock_t;
#else /* !_WIN32 */
typedef pthread_mutex_t pvn_lock_t;
#endif /* ?_WIN32 */

PVN_EXTERN_C size_t PVN_FABI(pvn_lock_size,PVN_LOCK_SIZE)(size_t *const);
PVN_EXTERN_C int PVN_FABI(pvn_lock_init,PVN_LOCK_INIT)(pvn_lock_t *const);
PVN_EXTERN_C int PVN_FABI(pvn_lock_destroy,PVN_LOCK_DESTROY)(pvn_lock_t *const);
PVN_EXTERN_C int PVN_FABI(pvn_lock,PVN_LOCK)(pvn_lock_t *const);
PVN_EXTERN_C int PVN_FABI(pvn_unlock,PVN_UNLOCK)(pvn_lock_t *const);

#endif /* !PVN_LOCK_H */

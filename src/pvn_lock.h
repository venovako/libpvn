#ifndef PVN_LOCK_H
#define PVN_LOCK_H

#ifndef PVN_H
#error pvn_lock.h not intended for direct inclusion
#endif /* !PVN_H */

typedef struct {
  pthread_mutex_t mutex;
  int mutex_type;
  volatile sig_atomic_t count;
} pvn_lock_t;

PVN_EXTERN_C void pvn_lock_init(pvn_lock_t *const, const bool);
PVN_EXTERN_C void pvn_lock_destroy(pvn_lock_t *const);
PVN_EXTERN_C sig_atomic_t pvn_lock(pvn_lock_t *const);
PVN_EXTERN_C sig_atomic_t pvn_unlock(pvn_lock_t *const);

#endif /* !PVN_LOCK_H */

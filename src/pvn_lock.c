#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  size_t a = (size_t)0u;
  (void)printf("sizeof(pvn_lock_t)=%zu\n", PVN_FABI(pvn_lock_size,PVN_LOCK_SIZE)(&a));
  (void)printf("alignof(pvn_lock_t)=%zu\n", a);
  pvn_lock_t lock;
  (void)printf("pvn_lock_init=%d\n", PVN_FABI(pvn_lock_init,PVN_LOCK_INIT)(&lock));
  (void)printf("pvn_lock=%d\n", PVN_FABI(pvn_lock,PVN_LOCK)(&lock));
  (void)printf("pvn_unlock=%d\n", PVN_FABI(pvn_unlock,PVN_UNLOCK)(&lock));
  (void)printf("pvn_lock_destroy=%d\n", PVN_FABI(pvn_lock_destroy,PVN_LOCK_DESTROY)(&lock));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
size_t PVN_FABI(pvn_lock_size,PVN_LOCK_SIZE)(size_t *const a)
{
  if (a)
    *a = alignof(pvn_lock_t);
  return sizeof(pvn_lock_t);
}

int PVN_FABI(pvn_lock_init,PVN_LOCK_INIT)(pvn_lock_t *const lock)
{
  PVN_ASSERT(lock);
  pthread_mutexattr_t attr;
  PVN_SYSI_CALL(pthread_mutexattr_init(&attr));
  PVN_SYSI_CALL(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK));
  PVN_SYSI_CALL(pthread_mutex_init(lock, &attr));
  PVN_SYSI_CALL(pthread_mutexattr_destroy(&attr));
  return PTHREAD_MUTEX_ERRORCHECK;
}

int PVN_FABI(pvn_lock_destroy,PVN_LOCK_DESTROY)(pvn_lock_t *const lock)
{
  PVN_ASSERT(lock);
  return pthread_mutex_destroy(lock);
}

int PVN_FABI(pvn_lock,PVN_LOCK)(pvn_lock_t *const lock)
{
  PVN_ASSERT(lock);
  return pthread_mutex_lock(lock);
}

int PVN_FABI(pvn_unlock,PVN_UNLOCK)(pvn_lock_t *const lock)
{
  PVN_ASSERT(lock);
  return pthread_mutex_unlock(lock);
}
#endif /* ?PVN_TEST */

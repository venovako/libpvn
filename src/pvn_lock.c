#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  size_t a = (size_t)0u;
  (void)printf("sizeof(pvn_lock_t)=%zu\n", pvn_lock_size_(&a));
  (void)printf("alignof(pvn_lock_t)=%zu\n", a);
  pvn_lock_t lock;
  (void)printf("pvn_lock_init_=%d\n", pvn_lock_init_(&lock));
  (void)printf("pvn_lock_=%d\n", pvn_lock_(&lock));
  (void)printf("pvn_unlock_=%d\n", pvn_unlock_(&lock));
  (void)printf("pvn_lock_destroy_=%d\n", pvn_lock_destroy_(&lock));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
size_t pvn_lock_size_(size_t *const a)
{
  if (a)
    *a = alignof(pvn_lock_t);
  return sizeof(pvn_lock_t);
}

int pvn_lock_init_(pvn_lock_t *const lock)
{
  assert(lock);
  pthread_mutexattr_t attr;
  PVN_SYSI_CALL(pthread_mutexattr_init(&attr));
  PVN_SYSI_CALL(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK));
  PVN_SYSI_CALL(pthread_mutex_init(lock, &attr));
  PVN_SYSI_CALL(pthread_mutexattr_destroy(&attr));
  return PTHREAD_MUTEX_ERRORCHECK;
}

int pvn_lock_destroy_(pvn_lock_t *const lock)
{
  assert(lock);
  return pthread_mutex_destroy(lock);
}

int pvn_lock_(pvn_lock_t *const lock)
{
  assert(lock);
  return pthread_mutex_lock(lock);
}

int pvn_unlock_(pvn_lock_t *const lock)
{
  assert(lock);
  return pthread_mutex_unlock(lock);
}
#endif /* ?PVN_TEST */

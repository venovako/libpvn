#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (1 != argc) {
    (void)fprintf(stderr, "%s takes no arguments\n", *argv);
    return EXIT_FAILURE;
  }
  (void)printf("%s\n", pvn_get_error());
  PVN_STOP("PVN_STOP")
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
const char *pvn_get_error()
{
  return strerror(errno);
}

int PVN_FABI(pvn_get_errno,PVN_GET_ERRNO)()
{
  return errno;
}

void PVN_FABI(pvn_set_errno,PVN_SET_ERRNO)(const int *const e)
{
  errno = (e ? *e : 0);
}
#endif /* ?PVN_TEST */

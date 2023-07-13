#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  (void)printf("%s\n", pvn_get_error());
  PVN_STOP("PVN_STOP");
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
const char *pvn_get_error()
{
  return strerror(errno);
}
#endif /* ?PVN_TEST */

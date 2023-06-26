#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  PVN_STOP("PVN_STOP");
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#endif /* ?PVN_TEST */

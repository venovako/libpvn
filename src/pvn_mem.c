#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  (void)printf("page size = %zu\n", pvn_pagesize());
}
#else /* !PVN_TEST */
size_t pvn_pagesize()
{
  const long ps = sysconf(_SC_PAGESIZE);
  PVN_SYSI_CALL(ps <= 0L);
  return (size_t)ps;
}

size_t pvn_alignment(const size_t a)
{
  size_t i = sizeof(void*);
  for (; i; i <<= 1u)
    if (i >= a)
      break;
  if (!i)
    i = pvn_zmax(pvn_pagesize(), sizeof(void*));
  return i;
}
#endif /* ?PVN_TEST */

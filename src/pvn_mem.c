#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  (void)printf("PVN_VECLEN = %u\n", PVN_VECLEN);
  (void)printf("PVN_SAFELEN(float) = %zu\n", PVN_SAFELEN(float));
  (void)printf("PVN_SAFELEN(double) = %zu\n", PVN_SAFELEN(double));
  (void)printf("page size = %zu\n", pvn_pagesize_());
  (void)printf("pvn_alignment(10) = %zu\n", pvn_alignment_(10u));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
size_t pvn_pagesize_()
{
  const long ps = sysconf(_SC_PAGESIZE);
  return (size_t)((ps < 0L) ? 0L : ps);
}

size_t pvn_alignment_(const size_t a)
{
  const size_t j = pvn_zmax((size_t)PVN_VECLEN, sizeof(void*));
  size_t i = j;
  for (; i; i <<= 1u)
    if (i >= a)
      break;
  if (!i)
    i = pvn_zmax(pvn_pagesize_(), j);
  return i;
}
#endif /* ?PVN_TEST */

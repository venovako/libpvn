#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  (void)printf("PVN_VECLEN = %u\n", PVN_VECLEN);
  (void)printf("PVN_SAFELEN(float) = %zu\n", PVN_SAFELEN(float));
  (void)printf("PVN_SAFELEN(double) = %zu\n", PVN_SAFELEN(double));
  (void)printf("page size = %zu\n", PVN_FABI(pvn_pagesize,PVN_PAGESIZE)());
  const size_t a = (size_t)10u;
  (void)printf("pvn_alignment(10) = %zu\n", PVN_FABI(pvn_alignment,PVN_ALIGNMENT)(&a));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
unsigned PVN_FABI(pvn_vec_len,PVN_VEC_LEN)()
{
  return (PVN_VECLEN);
}

size_t PVN_FABI(pvn_pagesize,PVN_PAGESIZE)()
{
#ifdef _WIN32
  return (size_t)4096u;
#else /* !_WIN32 */
  const long ps = sysconf(_SC_PAGESIZE);
  return (size_t)((ps < 0l) ? 0l : ps);
#endif /* ?_WIN32 */
}

size_t PVN_FABI(pvn_alignment,PVN_ALIGNMENT)(const size_t *const a)
{
  PVN_ASSERT(a);
  const size_t j = pvn_zmax((size_t)PVN_VECLEN, sizeof(void*));
  size_t i = j;
  for (; i; i <<= 1u)
    if (i >= *a)
      break;
  if (!i)
    i = pvn_zmax(PVN_FABI(pvn_pagesize,PVN_PAGESIZE)(), j);
  return i;
}
#endif /* ?PVN_TEST */

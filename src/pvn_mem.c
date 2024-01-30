#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  (void)printf("PVN_VECLEN = %u\n", PVN_VECLEN);
  (void)printf("PVN_SAFELEN(float) = %zu\n", PVN_SAFELEN(float));
  (void)printf("PVN_SAFELEN(double) = %zu\n", PVN_SAFELEN(double));
  (void)printf("page size = %zu\n",
#ifdef _WIN32
               PVN_PAGESIZE
#else /* !_WIN32 */
               pvn_pagesize_
#endif /* ?_WIN32 */
               ());
  (void)printf("pvn_alignment(10) = %zu\n", pvn_alignment(10u));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
unsigned
#ifdef _WIN32
PVN_VEC_LEN
#else /* !_WIN32 */
pvn_vec_len_
#endif /* ?_WIN32 */
()
{
  return (PVN_VECLEN);
}

size_t pvn_pagesize()
{
#ifdef _WIN32
  const long ps = 4096L;
#else /* !_WIN32 */
  const long ps = sysconf(_SC_PAGESIZE);
#endif /* ?_WIN32 */
  return (size_t)((ps < 0L) ? 0L : ps);
}

size_t
#ifdef _WIN32
PVN_PAGESIZE
#else /* !_WIN32 */
pvn_pagesize_
#endif /* ?_WIN32 */
()
{
  return pvn_pagesize();
}

size_t pvn_alignment(const size_t a)
{
  const size_t j = pvn_zmax((size_t)PVN_VECLEN, sizeof(void*));
  size_t i = j;
  for (; i; i <<= 1u)
    if (i >= a)
      break;
  if (!i)
    i = pvn_zmax(
#ifdef _WIN32
                 PVN_PAGESIZE
#else /* !_WIN32 */
                 pvn_pagesize_
#endif /* ?_WIN32 */
                 (), j);
  return i;
}

size_t
#ifdef _WIN32
PVN_ALIGNMENT
#else /* !_WIN32 */
pvn_alignment_
#endif /* ?_WIN32 */
(const size_t *const a)
{
  assert(a);
  return pvn_alignment(*a);
}
#endif /* ?PVN_TEST */

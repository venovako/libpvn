#include <stdio.h>
#include <string.h>

int
#ifdef _WIN32
CHK_STR
#else /* !_WIN32 */
chk_str_
#endif /* ?_WIN32 */
(const char *const a, const char *const b, const size_t la, const size_t lb)
{
  if (!b)
    return -2;
  if (!a)
    return -1;
  size_t l = strlen(a);
  int ret = 0;
  (void)printf("a: %zu %zu\n", l, la);
  if ((l + 1u) != la)
    ret |= 1;
  l = strlen(b);
  (void)printf("b: %zu %zu\n", l, lb);
  if ((l + 1u) != lb)
    ret |= 2;
  return ret;
}

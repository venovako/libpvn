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

void
#ifdef _WIN32
CHK_RET
#else /* !_WIN32 */
chk_ret_
#endif /* ?_WIN32 */
(char *const s, const size_t ls, const char *const a, const size_t la)
{
  const size_t lm = ((ls <= la) ? ls : la);
  size_t i = 0u;
  (void)printf("%zu %zu\n", ls, la);
  for (; i < lm; ++i)
    s[i] = a[i];
}

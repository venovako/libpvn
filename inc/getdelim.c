#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#ifndef ssize_t
#define ssize_t long long
#endif /* !ssize_t */
#ifndef SSIZE_MAX
#define SSIZE_MAX LLONG_MAX
#endif /* !SSIZE_MAX */
#endif /* _WIN32 */
#include "pvn_ext.h"
#include "getdelim.h"

ssize_t getdelim(char **restrict linep, size_t *restrict linecapp, int delimiter, FILE *restrict stream)
{
  if (!linep || !linecapp || !stream) {
    errno = EINVAL;
    return -1;
  }
  if (!*linep) {
    if (!*linecapp)
      *linecapp = (size_t)128u;
    if (!(*linep = (char*)malloc(*linecapp)))
      return -1;
  }
  ssize_t r = 0;
  int c = 0;
  while ((c = fgetc(stream)) != EOF) {
    if ((size_t)r >= (*linecapp - (size_t)1u)) {
      if (!(*linep = (char*)realloc(*linep, (*linecapp <<= 1u))))
        return -1;
    }
    if (((*linep)[r++] = c) == delimiter)
      break;
    if (r >= SSIZE_MAX)
      return -1;
  }
  (*linep)[r] = '\0';
  return ((c == EOF) ? -1 : r);
}

ssize_t getline(char **restrict linep, size_t *restrict linecapp, FILE *restrict stream)
{
  return getdelim(linep, linecapp, (int)'\n', stream);
}

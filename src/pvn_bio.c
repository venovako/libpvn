#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
static const size_t GiB = ((size_t)1ul << 30u);

int pvn_bopen_ro_(off_t *const sz, const char *const fn, ...)
{
  const int fd = (fn ? open(fn, (O_RDONLY | PVN_LF64)) : -2);
  if (fd >= 0) {
    if (sz) {
      struct stat buf;
      if (fstat(fd, &buf) < 0) {
        *sz = close(fd);
        return -1;
      }
      *sz = buf.st_size;
    }
  }
  return fd;
}

int pvn_bopen_rw_(off_t *const sz, const char *const fn, ...)
{
  const int fd = (fn ? open(fn, (O_RDWR | O_CREAT | PVN_LF64), (S_IRUSR | S_IWUSR)) : -2);
  if (fd >= 0) {
    if (sz) {
      if (*sz >= 0) {
        if ((ftruncate(fd, *sz) < 0) || (fsync(fd) < 0)) {
          *sz = close(fd);
          return -1;
        }
      }
      else {
        struct stat buf;
        if (fstat(fd, &buf) < 0) {
          *sz = close(fd);
          return -1;
        }
        *sz = buf.st_size;
      }
    }
  }
  return fd;
}

int pvn_bopen_wo_(off_t *const sz, const char *const fn, ...)
{
  const int fd = (fn ? open(fn, (O_WRONLY | O_CREAT | PVN_LF64), S_IWUSR) : -2);
  if (fd >= 0) {
    if (sz) {
      if (*sz >= 0) {
        if ((ftruncate(fd, *sz) < 0) || (fsync(fd) < 0)) {
          *sz = close(fd);
          return -1;
        }
      }
      else {
        struct stat buf;
        if (fstat(fd, &buf) < 0) {
          *sz = close(fd);
          return -1;
        }
        *sz = buf.st_size;
      }
    }
  }
  return fd;
}

int pvn_bclose_(const int *const fd)
{
  return (fd ? ((*fd < 0) ? -3 : close(*fd)) : -2);
}

ssize_t pvn_bwrite_(const int *const fd, const void *const buf, const size_t *const nB, const off_t *const off)
{
  if (!fd || (*fd < 0))
    return -1;
  if (!nB)
    return -3;
  if (*nB && !buf)
    return -2;
  if (!off || (*off < 0))
    return -4;
  if (!*nB)
    return 0;

  const void *_buf = buf;
  size_t _nB = *nB;
  off_t _off = *off;
  const size_t qGiB = (_nB >> 30u);
  ssize_t ret = 0;
  for (size_t i = 0u; i < qGiB; ++i) {
    const ssize_t ret1 = pwrite(*fd, _buf, GiB, _off);
    if ((ret1 <= 0) || ((size_t)ret1 != GiB))
      return -5;
    if (fsync(*fd) < 0)
      return -6;
    _buf = (const void*)(((const char*)_buf) + ret1);
    _nB -= ret1;
    _off += ret1;
    ret += ret1;
  }
  const ssize_t ret1 = pwrite(*fd, _buf, _nB, _off);
  if ((ret1 <= 0) || ((size_t)ret1 != _nB))
    return -7;
  if (fsync(*fd) < 0)
    return -8;
  ret += ret1;
  return ret;
}

ssize_t pvn_bread_(const int *const fd, void *const buf, const size_t *const nB, const off_t *const off)
{
  if (!fd || (*fd < 0))
    return -1;
  if (!nB)
    return -3;
  if (*nB && !buf)
    return -2;
  if (!off || (*off < 0))
    return -4;
  if (!*nB)
    return 0;

  void *_buf = buf;
  size_t _nB = *nB;
  off_t _off = *off;
  const size_t qGiB = (_nB >> 30u);
  ssize_t ret = 0;
  for (size_t i = 0u; i < qGiB; ++i) {
    const ssize_t ret1 = pread(*fd, _buf, GiB, _off);
    if ((ret1 <= 0) || ((size_t)ret1 != GiB))
      return -5;
    _buf = (void*)(((char*)_buf) + ret1);
    _nB -= ret1;
    _off += ret1;
    ret += ret1;
  }
  const ssize_t ret1 = pread(*fd, _buf, _nB, _off);
  if ((ret1 <= 0) || ((size_t)ret1 != _nB))
    return -6;
  ret += ret1;
  return ret;
}
#endif /* ?PVN_TEST */

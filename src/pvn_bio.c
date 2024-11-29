#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc != 3) {
    (void)fprintf(stderr, "%s fn sz\n", *argv);
    return EXIT_FAILURE;
  }
  off_t sz = (off_t)atol(argv[2]);
  int fd = pvn_bopen_wo_(&sz, argv[1]);
  (void)printf("pvn_bopen_wo_=%d, sz=%ld\n", fd, (long)sz);
  if (fd >= 0)
    (void)printf("pvn_bclose_=%d\n", (fd = pvn_bclose_(&fd)));
  return ((fd >= 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}
#else /* !PVN_TEST */
static const size_t GiB = ((size_t)1ul << 30u);
static const mode_t perms = (S_IRUSR | S_IWUSR);

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
  const int fd = (fn ? open(fn, (O_RDWR | O_CREAT | PVN_LF64), perms) : -2);
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
  const int fd = (fn ? open(fn, (O_WRONLY | O_CREAT | PVN_LF64), perms) : -2);
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

#ifdef __x86_64__
ssize_t pvn_bwrite80_(const int *const fd, const long double *const buf, const size_t *const n, const off_t *const off)
{
  if (!fd || (*fd < 0))
    return -1;
  if (!n)
    return -3;
  if (*n && !buf)
    return -2;
  if (!off || (*off < 0))
    return -4;
  if (!*n)
    return 0;

  const long double *_buf = buf;
  size_t _n = *n;
  off_t _off = *off;
  ssize_t ret = 0;
  for (size_t i = 0u; i < _n; ++i) {
    const ssize_t ret1 = pwrite(*fd, _buf++, (size_t)10u, _off);
    if (ret1 != (ssize_t)10)
      return -5;
#ifndef NDEBUG
    if (fsync(*fd) < 0)
      return -6;
#endif /* !NDEBUG */
    _off += (off_t)ret1;
    ret += ret1;
  }
  if (fsync(*fd) < 0)
    return -8;
  return ret;
}

ssize_t pvn_bread80_(const int *const fd, long double *const buf, const size_t *const n, const off_t *const off)
{
  if (!fd || (*fd < 0))
    return -1;
  if (!n)
    return -3;
  if (*n && !buf)
    return -2;
  if (!off || (*off < 0))
    return -4;
  if (!*n)
    return 0;

  long double *_buf = buf;
  size_t _n = *n;
  off_t _off = *off;
  ssize_t ret = 0;
  for (size_t i = 0u; i < _n; ++i) {
    const ssize_t ret1 = pread(*fd, _buf++, (size_t)10u, _off);
    if (ret1 != (ssize_t)10)
      return -5;
    _off += (off_t)ret1;
    ret += ret1;
  }
  return ret;
}
#endif /* __x86_64__ */
#endif /* ?PVN_TEST */

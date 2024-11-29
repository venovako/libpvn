#include "pvn.h"

#ifdef PVN_TEST
int main(/* int argc, char *argv[] */)
{
  const int fd = STDOUT_FILENO;
  const unsigned sz = 4u;
  (void)fprintf(stderr, "pvn_tar_add_file_=%d\n", pvn_tar_add_file_(&fd, "foo", &sz, "bar\n"));
  (void)fprintf(stderr, "pvn_tar_add_dir_=%d\n", pvn_tar_add_dir_(&fd, "dir"));
  (void)fprintf(stderr, "pvn_tar_add_file_=%d\n", pvn_tar_add_file_(&fd, "dir/FOO", &sz, "BAR\n"));
  (void)fprintf(stderr, "pvn_tar_terminate_=%d\n", pvn_tar_terminate_(&fd));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */

static_assert(sizeof(header_posix_ustar) == 512u, "sizeof(header_posix_ustar) != 512");

int pvn_tar_add_file_(const int *const fd, const char *const fn, const unsigned *const sz, const void *const buf)
{
  if (!sz)
    return -3;
  if (*sz && !buf)
    return -4;
  if (!fd || (*fd < 0))
    return -1;
  unsigned fnl = (fn ? (unsigned)strlen(fn) : 0u);
  if (!fnl || (fnl > 256u))
    return -2;
  unsigned i = fnl;
  while (i) {
    if (fn[--i] == '/')
      --fnl;
    else
      break;
  }
  if (!fnl)
    return -2;
  if ((fnl == 256u) && (fn[155u] != '/'))
    return -2;
  /* directory? */
  const int de = (!*sz && buf);
  header_posix_ustar hdr;
  (void)memset(&hdr, 0, sizeof(hdr));
  unsigned ni = 0u;
  for (i = fnl; i; ) {
    if (fn[--i] == '/') {
      ni = (i ? 1u : 0u);
      if (((fnl - i) <= (100u + ni)) && (i <= 155u)) {
        ni += i;
        (void)strncpy(hdr.name, fn + ni, fnl - ni);
        (void)strncpy(hdr.prefix, fn, i);
        ni = (fnl - ni);
        break;
      }
      ni = 0u;
    }
    else if (!i && (fnl <= 100u)) {
      (void)strncpy(hdr.name, fn, fnl);
      ni = fnl;
    }
  }
  if (!ni)
    return -2;
  if (de)
    (hdr.name)[ni] = '/';
  /* never allow setuid/setgid or executable files */
  (void)strcpy(hdr.mode, (de ? "0000777" : "0000666"));
  /* uid and gid 0 probably exist */
  (void)strcpy(hdr.gid, strcpy(hdr.uid, "0000000"));
  if (sprintf(hdr.size, "%011o", *sz) != 11)
    return -3;
  if (sprintf(hdr.mtime, "%011o", (unsigned)time((time_t*)NULL)) != 11)
    return 1;
  (hdr.typeflag)[0u] = (de ? '5' : '0');
  (void)strcpy(hdr.magic, "ustar");
  (hdr.version)[1u] = (hdr.version)[0u] = '0';
  /* never disclose the real username */
  (void)strcpy(hdr.gname, strcpy(hdr.uname, "root"));
  for (i = 0u; i < 8u; ++i)
    (hdr.checksum)[i] = ' ';
  const unsigned char *const u = (const unsigned char*)&hdr;
  unsigned c = 0u;
  for (i = 0u; i < (unsigned)sizeof(hdr); ++i)
    c += u[i];
  if (sprintf(hdr.checksum, "%06o", c) != 6)
    return 2;
  if (write(*fd, &hdr, sizeof(hdr)) != (ssize_t)sizeof(hdr))
    return 3;
  for (i = 0u; i < *sz; i += (unsigned)sizeof(hdr))
    if (write(*fd, memcpy(memset(&hdr, 0, sizeof(hdr)), (const char*)buf + i, pvn_umin(*sz - i, (unsigned)sizeof(hdr))), sizeof(hdr)) != (ssize_t)sizeof(hdr))
      return 4;
  return 0;
}

int pvn_tar_add_dir_(const int *const fd, const char *const dn)
{
  const unsigned sz = 0u;
  return pvn_tar_add_file_(fd, dn, &sz, &sz);
}

int pvn_tar_terminate_(const int *const fd)
{
  if (!fd || (*fd < 0))
    return -1;
  header_posix_ustar hdr;
  if (write(*fd, memset(&hdr, 0, sizeof(hdr)), sizeof(hdr)) != (ssize_t)sizeof(hdr))
    return 1;
  if (write(*fd, memset(&hdr, 0, sizeof(hdr)), sizeof(hdr)) != (ssize_t)sizeof(hdr))
    return 2;
  return 0;
}
#endif /* ?PVN_TEST */

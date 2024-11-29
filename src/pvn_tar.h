#ifndef PVN_TAR_H
#define PVN_TAR_H

#ifndef PVN_H
#error pvn_tar.h not intended for direct inclusion
#endif /* !PVN_H */

/* from tar(5) manpage on macOS */
typedef struct {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char typeflag[1];
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char pad[12];
} header_posix_ustar;

PVN_EXTERN_C int pvn_tar_add_file_(const int *const fd, const char *const fn, const unsigned *const sz, const void *const buf);
PVN_EXTERN_C int pvn_tar_add_dir_(const int *const fd, const char *const dn);
PVN_EXTERN_C int pvn_tar_terminate_(const int *const fd);

#endif /* !PVN_TAR_H */

#ifndef PVN_MPI_H
#define PVN_MPI_H

#ifndef PVN_H
#error pvn_mpi.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C int pvn_mpi_initialize(int *const argc, char ***const argv, const int required, int *const provided);
PVN_EXTERN_C int pvn_mpi_info_print(const int fd, MPI_Info info);
PVN_EXTERN_C int pvn_mpi_finalize(const int force);

#endif /* !PVN_MPI_H */

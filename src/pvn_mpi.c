#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc != 2) {
    (void)dprintf(STDERR_FILENO, "%s multithreaded\n", *argv);
    return EXIT_FAILURE;
  }
  int i = 0, j = 0;
  (void)dprintf(STDERR_FILENO, "MPI_Get_version=%d\n", MPI_Get_version(&i, &j));
  (void)dprintf(STDOUT_FILENO, "%d.%d\n", i, j);
#if (MPI_MAX_LIBRARY_VERSION_STRING >= MPI_MAX_PROCESSOR_NAME)
  char s[MPI_MAX_LIBRARY_VERSION_STRING] = { '\0' };
  j = MPI_MAX_LIBRARY_VERSION_STRING;
#else
  char s[MPI_MAX_PROCESSOR_NAME] = { '\0' };
  j = MPI_MAX_PROCESSOR_NAME;
#endif
  (void)dprintf(STDERR_FILENO, "MPI_Get_library_version=%d\n", MPI_Get_library_version(s, &j));
  (void)dprintf(STDOUT_FILENO, "%s\n", s);
  (void)dprintf(STDERR_FILENO, "MPI_Abi_get_version=%d\n", MPI_Abi_get_version(&i, &j));
  (void)dprintf(STDOUT_FILENO, "%d.%d\n", i, j);
  MPI_Info info = MPI_INFO_NULL;
  (void)dprintf(STDERR_FILENO, "MPI_Abi_get_info=%d\n", MPI_Abi_get_info(&info));
  (void)dprintf(STDERR_FILENO, "pvn_mpi_info_print=%d\n", pvn_mpi_info_print(STDOUT_FILENO, info));
  (void)dprintf(STDERR_FILENO, "MPI_Info_free=%d\n", MPI_Info_free(&info));
  j = (atoi(argv[1]) ? MPI_THREAD_MULTIPLE : MPI_THREAD_SINGLE);
  (void)dprintf(STDERR_FILENO, "pvn_mpi_initialize(%d)=%d\n", j, pvn_mpi_initialize(&argc, &argv, j, &i));
  switch (i) {
  case MPI_THREAD_SINGLE:
    (void)dprintf(STDOUT_FILENO, "MPI_THREAD_SINGLE\n");
    break;
  case MPI_THREAD_FUNNELED:
    (void)dprintf(STDOUT_FILENO, "MPI_THREAD_FUNNELED\n");
    break;
  case MPI_THREAD_SERIALIZED:
    (void)dprintf(STDOUT_FILENO, "MPI_THREAD_SERIALIZED\n");
    break;
  case MPI_THREAD_MULTIPLE:
    (void)dprintf(STDOUT_FILENO, "MPI_THREAD_MULTIPLE\n");
    break;
  default:
    (void)dprintf(STDOUT_FILENO, "%d\n", i);
  }
  info = MPI_INFO_ENV;
  (void)dprintf(STDERR_FILENO, "pvn_mpi_info_print=%d\n", pvn_mpi_info_print(STDOUT_FILENO, info));
  info = MPI_INFO_NULL;
  (void)dprintf(STDERR_FILENO, "MPI_Abi_get_fortran_info=%d\n", MPI_Abi_get_fortran_info(&info));
  (void)dprintf(STDERR_FILENO, "pvn_mpi_info_print=%d\n", pvn_mpi_info_print(STDOUT_FILENO, info));
  (void)dprintf(STDERR_FILENO, "MPI_info_free=%d\n", MPI_Info_free(&info));
  int *a = (int*)NULL;
  /* TODO: unsupported with Intel(R) MPI Library 2021.17 for Linux* OS
  (void)dprintf(STDERR_FILENO, "MPI_Abi_get_fortran_booleans=%d\n", MPI_Abi_get_fortran_booleans(4, &i, &j, (int*)&a));
  if (a)
    (void)dprintf(STDOUT_FILENO, "T:%d F:%d\n", i, j);
  */
  (void)dprintf(STDERR_FILENO, "MPI_Get_processor_name=%d\n", MPI_Get_processor_name(s, &j));
  (void)dprintf(STDOUT_FILENO, "%s\n", s);
  (void)dprintf(STDERR_FILENO, "MPI_Get_hw_resource_info=%d\n", MPI_Get_hw_resource_info(&info));
  (void)dprintf(STDERR_FILENO, "pvn_mpi_info_print=%d\n", pvn_mpi_info_print(STDOUT_FILENO, info));
  (void)dprintf(STDERR_FILENO, "MPI_info_free=%d\n", MPI_Info_free(&info));
  a = (int*)NULL;
  j = 0;
  (void)dprintf(STDERR_FILENO, "MPI_Comm_get_attr=%d\n", MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_TAG_UB, &a, &j));
  if (j) {
    (void)dprintf(STDOUT_FILENO, "MPI_TAG_UB=%d\n", (i = *a));
    a = (int*)NULL;
    j = 0;
  }
  (void)dprintf(STDERR_FILENO, "MPI_Comm_get_attr=%d\n", MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_IO, &a, &j));
  if (j) {
    switch (i = *a) {
    case MPI_PROC_NULL:
      (void)dprintf(STDOUT_FILENO, "MPI_IO=MPI_PROC_NULL\n");
      break;
    case MPI_ANY_SOURCE:
      (void)dprintf(STDOUT_FILENO, "MPI_IO=MPI_ANY_SOURCE\n");
      break;
    default:
      (void)dprintf(STDOUT_FILENO, "MPI_IO=%d\n", i);
    }
    a = (int*)NULL;
    j = 0;
  }
  (void)dprintf(STDERR_FILENO, "MPI_Comm_get_attr=%d\n", MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_WTIME_IS_GLOBAL, &a, &j));
  if (j) {
    (void)dprintf(STDOUT_FILENO, "MPI_WTIME_IS_GLOBAL=%d\n", (i = *a));
    a = (int*)NULL;
    j = 0;
  }
  (void)dprintf(STDERR_FILENO, "pvn_mpi_finalize=%d\n", pvn_mpi_finalize(0));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int pvn_mpi_initialize(int *const argc, char ***const argv, const int required, int *const provided)
{
  int flag = 0;
  if (MPI_Finalized(&flag) != MPI_SUCCESS)
    return -1;
  if (flag)
    return 0;
  if (MPI_Initialized(&flag) != MPI_SUCCESS)
    return -2;
  if (!provided)
    return -4;
  if (flag) {
    if (MPI_Query_thread(provided) != MPI_SUCCESS)
      return -3;
    if (MPI_Is_thread_main(&flag) != MPI_SUCCESS)
      return -5;
    return (flag ? 3 : 2);
  }
  return ((MPI_Init_thread(argc, argv, required, provided) != MPI_SUCCESS) ? -6 : 1);
}

int pvn_mpi_info_print(const int fd, MPI_Info info)
{
  char *val = (char*)NULL;
  int nkeys = 0, old = 0, len = 0, flag = 0;
  char key[MPI_MAX_INFO_KEY] = { '\0' };
  if (fd < 0)
    return -1;
  if (info == MPI_INFO_NULL)
    return -2;
  if (MPI_Info_get_nkeys(info, &nkeys) != MPI_SUCCESS)
    return -3;
  for (int i = 0; i < nkeys; ++i) {
    if (MPI_Info_get_nthkey(info, i, key) != MPI_SUCCESS) {
      nkeys = -4;
      break;
    }
    old = len;
    len = 0;
    if (MPI_Info_get_string(info, key, &len, val, &flag) != MPI_SUCCESS) {
      nkeys = -5;
      break;
    }
    if (flag) {
      if (!len) {
        nkeys = -6;
        break;
      }
      if (len > old)
        val = (char*)realloc(val, (size_t)len);
      if (val) {
        if (MPI_Info_get_string(info, key, &len, val, &flag) != MPI_SUCCESS) {
          nkeys = -7;
          break;
        }
        if (dprintf(fd, "%d:(%s,%s)\n", i, key, val) < 8) {
          nkeys = -8;
          break;
        }
      }
      else {
        nkeys = -9;
        break;
      }
    }
    else {
      nkeys = -10;
      break;
    }
  }
  if (val)
    free(val);
  return nkeys;
}

int pvn_mpi_finalize(const int force)
{
  int flag = 0;
  if (MPI_Finalized(&flag) != MPI_SUCCESS)
    return -1;
  if (flag)
    return 0;
  if (MPI_Initialized(&flag) != MPI_SUCCESS)
    return -2;
  /* force: should MPI_Finalize be called even if MPI_Init* was not? */
  if (!(flag || force))
    return 2;
  return ((MPI_Finalize() != MPI_SUCCESS) ? -3 : 1);
}
#endif /* ?PVN_TEST */

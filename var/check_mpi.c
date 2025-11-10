#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

static int info_print(FILE *const f, MPI_Info info)
{
  char *val = (char*)NULL;
  int nkeys = 0, old = 0, len = 0, flag = 0;
  char key[MPI_MAX_INFO_KEY] = { '\0' };
  if (!f)
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
        if (fprintf(f, "%d:(%s,%s)\n", i, key, val) < 8) {
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
  if (fflush(f))
    return -11;
  return nkeys;
}

static int mpi_initialize(int *const argc, char ***const argv, const int required, int *const provided)
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

static int mpi_finalize()
{
  int flag = 0;
  if (MPI_Finalized(&flag) != MPI_SUCCESS)
    return -1;
  if (flag)
    return 0;
  if (MPI_Initialized(&flag) != MPI_SUCCESS)
    return -2;
  /* TODO: should MPI_Finalize be called even if MPI_Init* was not? */
  if (!flag)
    return 2;
  return ((MPI_Finalize() != MPI_SUCCESS) ? -3 : 1);
}

int main(int argc, char* argv[])
{
  int i, j;
  (void)fprintf(stderr, "MPI_Get_version=%d\n", MPI_Get_version(&i, &j));
  (void)fprintf(stdout, "%d.%d\n", i, j);
#if (MPI_MAX_LIBRARY_VERSION_STRING >= MPI_MAX_PROCESSOR_NAME)
  char s[MPI_MAX_LIBRARY_VERSION_STRING];
  j = MPI_MAX_LIBRARY_VERSION_STRING;
#else
  char s[MPI_MAX_PROCESSOR_NAME];
  j = MPI_MAX_PROCESSOR_NAME;
#endif
  (void)fprintf(stderr, "MPI_Get_library_version=%d\n", MPI_Get_library_version(s, &j));
  (void)fprintf(stdout, "%s\n", s);
  (void)fprintf(stderr, "MPI_Abi_get_version=%d\n", MPI_Abi_get_version(&i, &j));
  (void)fprintf(stdout, "%d.%d\n", i, j);
  MPI_Info info = MPI_INFO_NULL;
  (void)fprintf(stderr, "MPI_Abi_get_info=%d\n", MPI_Abi_get_info(&info));
  (void)fprintf(stderr, "info_print=%d\n", info_print(stdout, info));
  (void)fprintf(stderr, "MPI_Info_free=%d\n", MPI_Info_free(&info));
  (void)fprintf(stderr, "MPI_Abi_get_fortran_info=%d\n", MPI_Abi_get_fortran_info(&info));
  (void)fprintf(stderr, "info_print=%d\n", info_print(stdout, info));
  j = MPI_THREAD_MULTIPLE;
  (void)fprintf(stderr, "mpi_initialize(%d)=%d\n", j, mpi_initialize(&argc, &argv, j, &i));
  switch (i) {
  case MPI_THREAD_SINGLE:
    (void)fprintf(stdout, "MPI_THREAD_SINGLE\n");
    break;
  case MPI_THREAD_FUNNELED:
    (void)fprintf(stdout, "MPI_THREAD_FUNNELED\n");
    break;
  case MPI_THREAD_SERIALIZED:
    (void)fprintf(stdout, "MPI_THREAD_SERIALIZED\n");
    break;
  case MPI_THREAD_MULTIPLE:
    (void)fprintf(stdout, "MPI_THREAD_MULTIPLE\n");
    break;
  default:
    (void)fprintf(stdout, "%d\n", i);
  }
  (void)fprintf(stderr, "info_print=%d\n", info_print(stdout, MPI_INFO_ENV));
  (void)fprintf(stderr, "MPI_Get_processor_name=%d\n", MPI_Get_processor_name(s, &j));
  (void)fprintf(stdout, "%s\n", s);
  (void)fprintf(stderr, "MPI_Get_hw_resource_info=%d\n", MPI_Get_hw_resource_info(&info));
  (void)fprintf(stderr, "info_print=%d\n", info_print(stdout, info));
  (void)fprintf(stderr, "MPI_info_free=%d\n", MPI_Info_free(&info));
  int *a = (int*)NULL;
  j = 0;
  (void)fprintf(stderr, "MPI_Comm_get_attr=%d\n", MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_TAG_UB, &a, &j));
  if (j) {
    (void)fprintf(stdout, "MPI_TAG_UB=%d\n", (i = *a));
    a = (int*)NULL;
    j = 0;
  }
  (void)fprintf(stderr, "MPI_Comm_get_attr=%d\n", MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_IO, &a, &j));
  if (j) {
    switch (i = *a) {
    case MPI_PROC_NULL:
      (void)fprintf(stdout, "MPI_IO=MPI_PROC_NULL\n");
      break;
    case MPI_ANY_SOURCE:
      (void)fprintf(stdout, "MPI_IO=MPI_ANY_SOURCE\n");
      break;
    default:
      (void)fprintf(stdout, "MPI_IO=%d\n", i);
    }
    a = (int*)NULL;
    j = 0;
  }
  (void)fprintf(stderr, "MPI_Comm_get_attr=%d\n", MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_WTIME_IS_GLOBAL, &a, &j));
  if (j) {
    (void)fprintf(stdout, "MPI_WTIME_IS_GLOBAL=%d\n", (i = *a));
    a = (int*)NULL;
    j = 0;
  }
  (void)fprintf(stderr, "mpi_finalize=%d\n", mpi_finalize());
  return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

static int info_print(MPI_Info info)
{
  int nkeys = 0, old = 0, len = 0, flag = 0;
  if (MPI_Info_get_nkeys(info, &nkeys) != MPI_SUCCESS)
    return -1;
  char key[MPI_MAX_INFO_KEY];
  char *val = (char*)NULL;
  for (int i = 0; i < nkeys; ++i) {
    if (MPI_Info_get_nthkey(info, i, key) != MPI_SUCCESS)
      return -2;
    old = len;
    len = 0;
    if (MPI_Info_get_string(info, key, &len, val, &flag) != MPI_SUCCESS)
      return -3;
    if (flag && len) {
      if (len > old)
        val = realloc(val, (size_t)len);
      if (val) {
        if (MPI_Info_get_string(info, key, &len, val, &flag) != MPI_SUCCESS)
          return -4;
        (void)printf("%d:(%s,%s)\n", i, key, val);
      }
    }
  }
  if (val)
    free(val);
  return nkeys;
}

int main(int argc, char* argv[])
{
  int i, j;
  (void)fprintf(stderr, "MPI_Get_version=%d\n", MPI_Get_version(&i, &j));
  (void)fprintf(stdout, "%d.%d\n", i, j);
  char s[MPI_MAX_LIBRARY_VERSION_STRING];
  (void)fprintf(stderr, "MPI_Get_library_version=%d\n", MPI_Get_library_version(s, &j));
  (void)fprintf(stdout, "%s\n", s);
  (void)fprintf(stderr, "MPI_Abi_get_version=%d\n", MPI_Abi_get_version(&i, &j));
  (void)fprintf(stdout, "%d.%d\n", i, j);
  MPI_Info info;
  (void)fprintf(stderr, "MPI_Abi_get_info=%d\n", MPI_Abi_get_info(&info));
  (void)fprintf(stderr, "info_print=%d\n", info_print(info));
  return EXIT_SUCCESS;
}

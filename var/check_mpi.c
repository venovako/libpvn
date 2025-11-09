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
  if (MPI_Info_get_nkeys(info, &nkeys) != MPI_SUCCESS)
    return -2;
  for (int i = 0; i < nkeys; ++i) {
    if (MPI_Info_get_nthkey(info, i, key) != MPI_SUCCESS) {
      nkeys = -3;
      break;
    }
    old = len;
    len = 0;
    if (MPI_Info_get_string(info, key, &len, val, &flag) != MPI_SUCCESS) {
      nkeys = -4;
      break;
    }
    if (flag && len) {
      if (len > old)
        val = (char*)realloc(val, (size_t)len);
      if (val) {
        if (MPI_Info_get_string(info, key, &len, val, &flag) != MPI_SUCCESS) {
          nkeys = -5;
          break;
        }
        if (fprintf(f, "%d:(%s,%s)\n", i, key, val) < 8) {
          nkeys = -6;
          break;
        }
      }
    }
  }
  if (val)
    free(val);
  if (fflush(f))
    return -7;
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
  (void)fprintf(stderr, "info_print=%d\n", info_print(stdout, info));
  return EXIT_SUCCESS;
}

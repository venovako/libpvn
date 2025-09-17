#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc == 1) {
#ifdef _WIN32
    (void)printf("pvn.");
#else /* !_WIN32 */
    (void)printf("libpvn.");
#endif /* ?_WIN32 */
#ifdef PVN_DYNAMIC
    (void)printf("%s ", PVN_DYNAMIC);
#else /* !PVN_DYNAMIC */
#ifdef _WIN32
#ifdef _DLL
    (void)printf("dll ");
#else /* !_DLL */
    (void)printf("lib ");
#endif /* ?_DLL */
#else /* !_WIN32 */
    (void)printf("a ");
#endif /* ?_WIN32 */
#endif /* ?PVN_DYNAMIC */
    (void)printf("built on %s with %s for %s on %s ", __DATE__, PVN_COMPILER, PVN_OS, PVN_ARCH);
#ifdef NDEBUG
    (void)printf("with optimizations ");
#else /* !NDEBUG */
    (void)printf("for debugging ");
#endif /* ?NDEBUG */
    (void)printf("and with ");
#ifdef PVN_CILK
    (void)printf("OpenCilk %d\n", PVN_CILK);
#else /* !PVN_CILK */
    (void)printf("OpenMP ");
#ifdef _OPENMP
    (void)printf("%d\n", _OPENMP);
#else /* !_OPENMP */
    (void)printf("disabled\n");
#endif /* ?_OPENMP */
#endif /* ?PVN_CILK */
    (void)printf("endianness  : %s\n", (pvn_le() ? "little" : "big"));
#ifdef PVN_CR_MATH
    (void)printf("PVN_CR_MATH : %s\n", PVN_CR_MATH);
#endif /* PVN_CR_MATH */
#ifdef PVN_QUADMATH
    (void)printf("PVN_QUADMATH: %s\n", PVN_QUADMATH);
#endif /* PVN_QUADMATH */
  }
  else {
    struct option longopts[] = {
      { "dynamic",  no_argument, (int*)NULL, 'd' },
      { "debug",    no_argument, (int*)NULL, 'g' },
      { "parallel", no_argument, (int*)NULL, 'p' },
      { (char*)NULL, 0, (int*)NULL, 0 }
    };
    for (int ch = 0; (ch = getopt_long(argc, argv, "dgp", longopts, (int*)NULL)) != -1; ) {
      switch ((char)ch) {
      case 'g':
#ifdef NDEBUG
        (void)printf("debug=false\n");
#else /* !NDEBUG */
        (void)printf("debug=true\n");
#endif /* ?NDEBUG */
        break;
      case 'd':
#if (defined(PVN_DYNAMIC) || (defined(_WIN32) && defined(_DLL)))
        (void)printf("dynamic=true\n");
#else /* !DYNAMIC */
        (void)printf("dynamic=false\n");
#endif /* ?DYNAMIC */
        break;
      case 'p':
#ifdef _OPENMP
        (void)printf("OpenMP=%d\n", _OPENMP);
#else /* !_OPENMP */
#ifdef PVN_CILK
        (void)printf("OpenCilk=%d\n", PVN_CILK);
#else /* !PVN_CILK */
        (void)printf("parallel=false\n");
#endif /* ?PVN_CILK */
#endif /* ?_OPENMP */
        break;
      default:
      err:
        (void)fprintf(stderr, "Usage: pvn.exe [-d|--dynamic] [-g|--debug] [-p|--parallel]\n");
        return EXIT_FAILURE;
      }
    }
    argv += optind;
    argc -= optind;
    if (argc)
      goto err;
  }
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
int PVN_FABI(pvn_le,PVN_LE)()
{
  return pvn_le();
}

int PVN_FABI(pvn_omp,PVN_OMP)()
{
  return pvn_omp();
}
#endif /* ?PVN_TEST */

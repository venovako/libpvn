#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc == 1) {
    (void)fprintf(stderr, "built on        : %s\n", __DATE__);
    (void)fprintf(stderr, "debug           : ");
#ifdef NDEBUG
    (void)fprintf(stderr, "false\n");
#else /* !NDEBUG */
    (void)fprintf(stderr, "true\n");
#endif /* ?NDEBUG */
#ifdef PVN_CILK
    (void)fprintf(stderr, "OpenCilk        : %d\n", PVN_CILK);
#else /* !PVN_CILK */
#ifdef _OPENMP
    (void)fprintf(stderr, "OpenMP          : %d\n", _OPENMP);
#endif /* _OPENMP */
#endif /* ?PVN_CILK */
    (void)fprintf(stderr, "endianness      : %s\n", (pvn_le() ? "little" : "big"));
    (void)fprintf(stderr, "math_errhandling: %d\n", PVN_FABI(c_math_err,C_MATH_ERR)());
#ifdef PVN_CR_MATH
    (void)fprintf(stderr, "PVN_CR_MATH     : %d\n", PVN_CR_MATH);
#endif /* PVN_CR_MATH */
#ifdef PVN_QUADMATH
    (void)fprintf(stderr, "PVN_QUADMATH    : %d\n", PVN_QUADMATH);
#endif /* PVN_QUADMATH */
    (void)fprintf(stdout, "PVN_CC=%s\n", PVN_CC);
    (void)fprintf(stdout, "PVN_CXX=%s\n", PVN_CXX);
    (void)fprintf(stdout, "PVN_FC=%s\n", PVN_FC);
    (void)fprintf(stdout, "PVN_CPPFLAGS=%s\n", PVN_CPPFLAGS);
    (void)fprintf(stdout, "PVN_CFLAGS=%s\n", PVN_CFLAGS);
    (void)fprintf(stdout, "PVN_CXXFLAGS=%s\n", PVN_CXXFLAGS);
    (void)fprintf(stdout, "PVN_FCFLAGS=%s\n", PVN_FCFLAGS);
    (void)fprintf(stdout, "PVN_LIBS=%s\n", PVN_LIBS);
    (void)fprintf(stdout, "PVN_LDFLAGS=%s\n", PVN_LDFLAGS);
  }
  else {
    struct option longopts[] = {
      { "cc",       no_argument, (int*)NULL, 'C' },
      { "fc",       no_argument, (int*)NULL, 'F' },
      { "ldflags",  no_argument, (int*)NULL, 'L' },
      { "cxx",      no_argument, (int*)NULL, 'X' },
      { "cflags",   no_argument, (int*)NULL, 'c' },
      { "dynamic",  no_argument, (int*)NULL, 'd' },
      { "fcflags",  no_argument, (int*)NULL, 'f' },
      { "cppflags", no_argument, (int*)NULL, 'i' },
      { "libs",     no_argument, (int*)NULL, 'l' },
      { "parallel", no_argument, (int*)NULL, 'p' },
      { "cxxflags", no_argument, (int*)NULL, 'x' },
      { (char*)NULL, 0, (int*)NULL, 0 }
    };
    for (int ch = 0; (ch = getopt_long(argc, argv, "CFLXcdfilpx", longopts, (int*)NULL)) != -1; ) {
      switch ((char)ch) {
      case 'C':
        (void)fprintf(stdout, "%s\n", PVN_CC);
        break;
      case 'F':
        (void)fprintf(stdout, "%s\n", PVN_FC);
        break;
      case 'L':
        (void)fprintf(stdout, "%s\n", PVN_LDFLAGS);
        break;
      case 'X':
        (void)fprintf(stdout, "%s\n", PVN_CXX);
        break;
      case 'c':
        (void)fprintf(stdout, "%s\n", PVN_CFLAGS);
        break;
      case 'd':
#if (defined(PVN_DYNAMIC) || (defined(_WIN32) && defined(_DLL)))
        (void)fprintf(stdout, "true\n");
#else /* !DYNAMIC */
        (void)fprintf(stdout, "false\n");
#endif /* ?DYNAMIC */
        break;
      case 'f':
        (void)fprintf(stdout, "%s\n", PVN_FCFLAGS);
        break;
      case 'i':
        (void)fprintf(stdout, "%s\n", PVN_CPPFLAGS);
        break;
      case 'l':
        (void)fprintf(stdout, "%s\n", PVN_LIBS);
        break;
      case 'p':
#ifdef _OPENMP
        (void)fprintf(stdout, "OpenMP=%d\n", _OPENMP);
#else /* !_OPENMP */
#ifdef PVN_CILK
        (void)fprintf(stdout, "OpenCilk=%d\n", PVN_CILK);
#else /* !PVN_CILK */
        (void)fprintf(stdout, "simd\n");
#endif /* ?PVN_CILK */
#endif /* ?_OPENMP */
        break;
      case 'x':
        (void)fprintf(stdout, "%s\n", PVN_CXXFLAGS);
        break;
      default:
        goto err;
      }
    }
    argv += optind;
    argc -= optind;
    if (argc)
      goto err;
  }
  return EXIT_SUCCESS;
 err:
  (void)fprintf(stderr, "pvn.exe [-C|--cc] [-F|--fc] [-X|--cxx] [-L|--ldflags] [-c|--cflags] [-d|--dynamic] [-f|--fcflags] [-i|--cppflags] [-l|--libs] [-p|--parallel] [-x|--cxxflags]\n");
  return EXIT_FAILURE;
}
#else /* !PVN_TEST */
#ifdef PVN_CILK
unsigned PVN_FABI(pvn_cilk_nworkers,PVN_CILK_NWORKERS)()
{
  return (unsigned)pvn_atoz(getenv("CILK_NWORKERS"));
}
#endif /* PVN_CILK */

int PVN_FABI(pvn_le,PVN_LE)()
{
  return pvn_le();
}

int PVN_FABI(pvn_omp,PVN_OMP)()
{
  return pvn_omp();
}

int PVN_FABI(c_math_err,C_MATH_ERR)()
{
  return math_errhandling;
}
#endif /* ?PVN_TEST */

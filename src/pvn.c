#include "pvn.h"

#ifdef PVN_TEST
int main(int argc, char *argv[])
{
  if (argc == 1) {
    (void)printf("built on    : %s\n", __DATE__);
    (void)printf("debug       : ");
#ifdef NDEBUG
    (void)printf("false\n");
#else /* !NDEBUG */
    (void)printf("true\n");
#endif /* ?NDEBUG */
#ifdef PVN_CILK
    (void)printf("OpenCilk    : %d\n", PVN_CILK);
#else /* !PVN_CILK */
#ifdef _OPENMP
    (void)printf("OpenMP      : %d\n", _OPENMP);
#endif /* ?_OPENMP */
#endif /* ?PVN_CILK */
    (void)printf("endianness  : %s\n", (pvn_le() ? "little" : "big"));
#ifdef PVN_CR_MATH
    (void)printf("PVN_CR_MATH : %d\n", PVN_CR_MATH);
#endif /* PVN_CR_MATH */
#ifdef PVN_QUADMATH
    (void)printf("PVN_QUADMATH: %d\n", PVN_QUADMATH);
#endif /* PVN_QUADMATH */
    (void)printf("PVN_CPPFLAGS: %s\n", PVN_CPPFLAGS);
    (void)printf("PVN_CFLAGS  : %s\n", PVN_CFLAGS);
    (void)printf("PVN_LDFLAGS : %s\n", PVN_LDFLAGS);
    (void)printf("\nmath_errhandling=%d\n", PVN_FABI(c_math_err,C_MATH_ERR)());
  }
  else {
    struct option longopts[] = {
      { "dynamic",  no_argument, (int*)NULL, 'd' },
      { "debug",    no_argument, (int*)NULL, 'g' },
      { "cppflags", no_argument, (int*)NULL, 'i' },
      { "cflags",   no_argument, (int*)NULL, 'c' },
      { "ldflags",  no_argument, (int*)NULL, 'l' },
      { "parallel", no_argument, (int*)NULL, 'p' },
      { (char*)NULL, 0, (int*)NULL, 0 }
    };
    for (int ch = 0; (ch = getopt_long(argc, argv, "dgiclp", longopts, (int*)NULL)) != -1; ) {
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
      case 'i':
        (void)printf("%s\n", PVN_CPPFLAGS);
        break;
      case 'c':
        (void)printf("%s\n", PVN_CFLAGS);
        break;
      case 'l':
        (void)printf("%s\n", PVN_LDFLAGS);
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
        (void)fprintf(stderr, "pvn.exe [-d|--dynamic] [-g|--debug] [-i|--cppflags] [-l|--ldflags] [-p|--parallel]\n");
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

int PVN_FABI(c_math_err,C_MATH_ERR)()
{
  return math_errhandling;
}
#endif /* ?PVN_TEST */

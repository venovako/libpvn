#ifndef PVN_ERR_H
#define PVN_ERR_H

#ifndef PVN_H
#error pvn_err.h not intended for direct inclusion
#endif /* !PVN_H */

#ifdef _WIN32
#ifndef PVN_STOP
#define PVN_STOP(msg) {                                                   \
    if (msg)                                                              \
      (void)fprintf(stderr, "\n%s(%d): %s\n", __FILE__, __LINE__, (msg)); \
    else                                                                  \
      (void)fprintf(stderr, "\n%s(%d)\n", __FILE__, __LINE__);            \
    exit(EXIT_FAILURE);                                                   \
  }
#else /* PVN_STOP */
#error PVN_STOP already defined
#endif /* ?PVN_STOP */
#else /* !_WIN32 */
#ifndef PVN_BTRACE_BUFSIZ
#define PVN_BTRACE_BUFSIZ 128
#endif /* ?PVN_BTRACE_BUFSIZ */

#ifndef PVN_BTRACE
#define PVN_BTRACE {                                        \
    void* buffer[PVN_BTRACE_BUFSIZ];                        \
    const int bsz = backtrace(buffer, (PVN_BTRACE_BUFSIZ)); \
    if (bsz > 0)                                            \
      backtrace_symbols_fd(buffer, bsz, STDERR_FILENO);     \
  }
#else /* PVN_BTRACE */
#error PVN_BTRACE already defined
#endif /* ?PVN_BTRACE */

#ifndef PVN_STOP
#define PVN_STOP(msg) {                                           \
    if (msg)                                                      \
      (void)dprintf(STDERR_FILENO, "\n%s(%d) in thread %p: %s\n", \
                    __FILE__, __LINE__,                           \
                    (const void*)pthread_self(), (msg));          \
    else                                                          \
      (void)dprintf(STDERR_FILENO, "\n%s(%d) in thread %p\n",     \
                    __FILE__, __LINE__,                           \
                    (const void*)pthread_self());                 \
    PVN_BTRACE                                                    \
    exit(EXIT_FAILURE);                                           \
  }
#else /* PVN_STOP */
#error PVN_STOP already defined
#endif /* ?PVN_STOP */
#endif /* ?_WIN32 */

PVN_EXTERN_C const char *pvn_get_error();

#ifndef PVN_SYSI_CALL
#define PVN_SYSI_CALL(call) {                  \
    if (0 != (int)(call)) {                    \
      const char *const msg = pvn_get_error(); \
      PVN_STOP(msg)                            \
    }                                          \
  }
#else /* PVN_SYSI_CALL */
#error PVN_SYSI_CALL already defined
#endif /* ?PVN_SYSI_CALL */

#ifndef PVN_SYSP_CALL
#define PVN_SYSP_CALL(call) {                  \
    if (NULL == (const void*)(call)) {         \
      const char *const msg = pvn_get_error(); \
      PVN_STOP(msg)                            \
    }                                          \
  }
#else /* PVN_SYSP_CALL */
#error PVN_SYSP_CALL already defined
#endif /* ?PVN_SYSP_CALL */

#endif /* !PVN_ERR_H */

#ifndef PVN_FMT_H
#define PVN_FMT_H

#ifndef PVN_H
#error pvn_fmt.h not intended for direct inclusion
#endif /* !PVN_H */

/* see https://fortranwiki.org/fortran/show/ansi_colors */

#ifndef PVN_ANSI_BLACK
#define PVN_ANSI_BLACK "\e[30m"
#else /* PVN_ANSI_BLACK */
#error PVN_ANSI_BLACK already defined
#endif /* ?PVN_ANSI_BLACK */

#ifndef PVN_ANSI_RED
#define PVN_ANSI_RED "\e[31m"
#else /* PVN_ANSI_RED */
#error PVN_ANSI_RED already defined
#endif /* ?PVN_ANSI_RED */

#ifndef PVN_ANSI_GREEN
#define PVN_ANSI_GREEN "\e[32m"
#else /* PVN_ANSI_GREEN */
#error PVN_ANSI_GREEN already defined
#endif /* ?PVN_ANSI_GREEN */

#ifndef PVN_ANSI_YELLOW
#define PVN_ANSI_YELLOW "\e[33m"
#else /* PVN_ANSI_YELLOW */
#error PVN_ANSI_YELLOW already defined
#endif /* ?PVN_ANSI_YELLOW */

#ifndef PVN_ANSI_BLUE
#define PVN_ANSI_BLUE "\e[34m"
#else /* PVN_ANSI_BLUE */
#error PVN_ANSI_BLUE already defined
#endif /* ?PVN_ANSI_BLUE */

#ifndef PVN_ANSI_MAGENTA
#define PVN_ANSI_MAGENTA "\e[35m"
#else /* PVN_ANSI_MAGENTA */
#error PVN_ANSI_MAGENTA already defined
#endif /* ?PVN_ANSI_MAGENTA */

#ifndef PVN_ANSI_CYAN
#define PVN_ANSI_CYAN "\e[36m"
#else /* PVN_ANSI_CYAN */
#error PVN_ANSI_CYAN already defined
#endif /* ?PVN_ANSI_CYAN */

#ifndef PVN_ANSI_WHITE
#define PVN_ANSI_WHITE "\e[37m"
#else /* PVN_ANSI_WHITE */
#error PVN_ANSI_WHITE already defined
#endif /* ?PVN_ANSI_WHITE */

#ifndef PVN_ANSI_CLEAR
#define PVN_ANSI_CLEAR "\e[0m"
#else /* PVN_ANSI_CLEAR */
#error PVN_ANSI_CLEAR already defined
#endif /* ?PVN_ANSI_CLEAR */

PVN_EXTERN_C size_t pvn_atoz(const char *const s);
PVN_EXTERN_C char *pvn_stoa(char *const s, const float x);
PVN_EXTERN_C char *pvn_dtoa(char *const s, const double x);
#if (!defined(_WIN32) || defined(_DLL))
PVN_EXTERN_C long double pvn_atox(const char *const s);
PVN_EXTERN_C char *pvn_xtoa(char *const s, const long double x);
#ifdef PVN_QUADMATH
PVN_EXTERN_C __float128 pvn_atoq(const char *const s);
PVN_EXTERN_C char *pvn_qtoa(char *const s, const __float128 x);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C long double pvn_atoq(const char *const s);
PVN_EXTERN_C char *pvn_qtoa(char *const s, const long double x);
#endif /* ?PVN_QUADMATH */
#endif /* !_WIN32 || _DLL */

#endif /* !PVN_FMT_H */

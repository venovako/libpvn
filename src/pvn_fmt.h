#ifndef PVN_FMT_H
#define PVN_FMT_H

#ifndef PVN_H
#error pvn_cvt.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C size_t pvn_atoz(const char *const s);
PVN_EXTERN_C char *pvn_stoa(char *const s, const float x);
PVN_EXTERN_C char *pvn_dtoa(char *const s, const double x);
#ifndef _WIN32
PVN_EXTERN_C char *pvn_xtoa(char *const s, const long double x);
#endif /* !_WIN32 */

#endif /* !PVN_FMT_H */

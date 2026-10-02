#ifndef PVN_EXT_H
#define PVN_EXT_H

#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C extern "C"
#else /* C */
#define EXTERN_C extern
#endif /* ?__cplusplus */
#else /* EXTERN_C */
#error EXTERN_C already defined
#endif /* ?EXTERN_C */

#ifndef PVN_EXTERN_C
#ifdef _WIN32
#ifdef _DLL
#ifdef PVN_DYNAMIC
#define PVN_EXTERN_C EXTERN_C __declspec(dllexport)
#else /* !PVN_DYNAMIC */
#define PVN_EXTERN_C EXTERN_C __declspec(dllimport)
#endif /* ?PVN_DYNAMIC */
#else /* !_DLL */
#define PVN_EXTERN_C EXTERN_C
#endif /* ?_DLL */
#else /* !_WIN32 */
#define PVN_EXTERN_C EXTERN_C
#endif /* ?_WIN32 */
#endif /* !PVN_EXTERN_C */

#ifndef PVN_QUADMATH
#ifndef __float128
#define __float128 _Float128
#endif /* !__float128 */
#endif /* !PVN_QUADMATH */

#endif /* !PVN_EXT_H */

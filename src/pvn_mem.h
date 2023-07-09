#ifndef PVN_MEM_H
#define PVN_MEM_H

#ifndef PVN_H
#error pvn_mem.h not intended for direct inclusion
#endif /* !PVN_H */

/* imperfect detection of the maximal vector length */
#ifndef PVN_VECLEN
#ifdef __AVX512F__
#define PVN_VECLEN 64u
#else /* !__AVX512F__ */
#ifdef __x86_64__
/* assume AVX */
#define PVN_VECLEN 32u
#else /* !__x86_64__ */
#define PVN_VECLEN 16u
#endif /* ?__x86_64__ */
#endif /* ?__AVX512F__ */
#endif /* !PVN_VECLEN */

#ifndef PVN_SAFELEN
#define PVN_SAFELEN(x) ((PVN_VECLEN) / sizeof(x))
#else /* PVN_SAFELEN */
#error PVN_SAFELEN already defined
#endif /* ?PVN_SAFELEN */

#ifndef PVN_NOT_VECALIGNED
#define PVN_NOT_VECALIGNED(x) ((uintptr_t)(x) & ((PVN_VECLEN) - 1u))
#else /* PVN_NOT_VECALIGNED */
#error PVN_NOT_VECALIGNED already defined
#endif /* ?PVN_NOT_VECALIGNED */

PVN_EXTERN_C size_t pvn_pagesize_();
PVN_EXTERN_C size_t pvn_alignment_(const size_t a);

#endif /* !PVN_MEM_H */

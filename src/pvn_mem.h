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

#ifndef PVN_ASSUME_ALIGNED
#if (defined(__ICC) || defined(__INTEL_COMPILER) || defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER))
#define PVN_ASSUME_ALIGNED(p,a) __assume_aligned((p),(a))
#else /* !Intel */
#define PVN_ASSUME_ALIGNED(p,a) (void)__builtin_assume_aligned((p),(a))
#endif /* ?Intel */
#else /* PVN_ASSUME_ALIGNED */
#error PVN_ASSUME_ALIGNED already defined
#endif /* ?PVN_ASSUME_ALIGNED */

#ifndef PVN_IS_VECALIGNED
#define PVN_IS_VECALIGNED(p) (p && !((uintptr_t)(p) & (uintptr_t)((PVN_VECLEN) - 1u)))
#else /* PVN_IS_VECALIGNED */
#error PVN_IS_VECALIGNED already defined
#endif /* ?PVN_IS_VECALIGNED */

#ifndef PVN_ASSUME_VECALIGNED
#define PVN_ASSUME_VECALIGNED(p) PVN_ASSUME_ALIGNED(p,PVN_VECLEN)
#else /* PVN_ASSUME_VECALIGNED */
#error PVN_ASSUME_VECALIGNED already defined
#endif /* ?PVN_ASSUME_VECALIGNED */

PVN_EXTERN_C unsigned pvn_vec_len_();
PVN_EXTERN_C size_t pvn_pagesize_();
PVN_EXTERN_C size_t pvn_alignment_(const size_t *const a);

#endif /* !PVN_MEM_H */

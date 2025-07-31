#ifndef PVN_MEM_H
#define PVN_MEM_H

#ifndef PVN_H
#error pvn_mem.h not intended for direct inclusion
#endif /* !PVN_H */

#ifndef PVN_ASSUME_ALIGNED
#if (defined(__INTEL_CLANG_COMPILER) || defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER))
#define PVN_ASSUME_ALIGNED(p,a) __assume_aligned((p),(a))
#else /* !Intel */
#define PVN_ASSUME_ALIGNED(p,a) (void)__builtin_assume_aligned((p),(a))
#endif /* ?Intel */
#else /* PVN_ASSUME_ALIGNED */
#error PVN_ASSUME_ALIGNED already defined
#endif /* ?PVN_ASSUME_ALIGNED */

#ifndef PVN_IS_ALIGNED
#define PVN_IS_ALIGNED(p,a) !((uintptr_t)(p) & (uintptr_t)((unsigned)(a) - 1u))
#else /* PVN_IS_ALIGNED */
#error PVN_IS_ALIGNED already defined
#endif /* ?PVN_IS_ALIGNED */

PVN_EXTERN_C size_t PVN_FABI(pvn_pagesize,PVN_PAGESIZE)();
PVN_EXTERN_C size_t PVN_FABI(pvn_alignment,PVN_ALIGNMENT)(const size_t *const a);

#endif /* !PVN_MEM_H */

#ifndef PVN_MEM_H
#define PVN_MEM_H

#ifndef PVN_H
#error pvn_mem.h not intended for direct inclusion
#endif /* !PVN_H */

static inline size_t pvn_zmin(const size_t a, const size_t b)
{
  return ((a <= b) ? a : b);
}

static inline size_t pvn_zmax(const size_t a, const size_t b)
{
  return ((a >= b) ? a : b);
}

PVN_EXTERN_C size_t pvn_pagesize();
PVN_EXTERN_C size_t pvn_alignment(const size_t a);

#endif /* !PVN_MEM_H */

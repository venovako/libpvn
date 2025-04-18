#ifndef PVN_PROF_H
#define PVN_PROF_H

#ifdef _WIN32
#error pvn_prof is not supported on Windows
#endif /* _WIN32 */

#ifndef PVN_H
#error pvn_prof.h not intended for direct inclusion
#endif /* !PVN_H */

typedef struct {
  void *addr;
  long fof;
  char *sym;
} pvn_addr_rec_t;

typedef struct {
  void *this_fn;
  void *call_site;
  long tns;
} pvn_prof_rec_t;

#endif /* !PVN_PROF_H */

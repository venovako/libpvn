#include "pvn.h"

#ifdef PVN_TEST
typedef struct {
  void *this_fn;
  void *call_site;
  long tns;
  size_t level;
} pvn_prof_rec_x;

static FILE *pt = (FILE*)NULL;
static FILE *bt = (FILE*)NULL;
static FILE *st = (FILE*)NULL;
static FILE *tt = (FILE*)NULL;

static pvn_addr_rec_t *addrs = (pvn_addr_rec_t*)NULL;
static pvn_prof_rec_x *profs = (pvn_prof_rec_x*)NULL;

static size_t profc = (size_t)0u;
static size_t addrc = (size_t)0u;

static int addr_cmp(const void *a, const void *b)
{
  if (a == b)
    return 0;
  const pvn_addr_rec_t *const A = (const pvn_addr_rec_t*)a;
  const pvn_addr_rec_t *const B = (const pvn_addr_rec_t*)b;
  if (A->addr < B->addr)
    return -1;
  if (B->addr < A->addr)
    return 1;
  return 0;
}

static ssize_t proct()
{
  PVN_SYSP_CALL(addrs = (pvn_addr_rec_t*)calloc((size_t)1u, sizeof(pvn_addr_rec_t)));
  while (fread((addrs + addrc), (sizeof(pvn_addr_rec_t) - sizeof(char*)), (size_t)1u, bt) == (size_t)1u) {
    PVN_SYSI_CALL(fseek(st, addrs[addrc].fof, SEEK_SET));
    addrs[addrc].fof = 0l;
    addrs[addrc].sym = (char*)NULL;
    PVN_SYSI_CALL((addrs[addrc].fof = (long)getline(&(addrs[addrc].sym), (size_t*)&(addrs[addrc].fof), st)) < (ssize_t)0);
    addrs[addrc].sym[addrs[addrc].fof - 1] = '\t';
    PVN_SYSP_CALL(addrs = (pvn_addr_rec_t*)realloc(addrs, (++addrc + 1u) * sizeof(pvn_addr_rec_t)));
  }
#ifndef NDEBUG
  addrs[addrc].addr = NULL;
  addrs[addrc].fof = 0l;
  addrs[addrc].sym = (char*)NULL;
#endif /* !NDEBUG */
  qsort(addrs, addrc, sizeof(pvn_addr_rec_t), addr_cmp);
  PVN_SYSP_CALL(profs = (pvn_prof_rec_x*)calloc((size_t)1u, sizeof(pvn_prof_rec_x)));
  ssize_t max_level = (ssize_t)0;
  for (size_t level = 0u; fread((profs + profc), sizeof(pvn_prof_rec_t), (size_t)1u, pt) == (size_t)1u; ) {
    if (profs[profc].call_site) {
      pvn_addr_rec_t *addr = (addrs + addrc);
      PVN_SYSP_CALL(addr = bsearch((profs + profc), addrs, addrc, sizeof(pvn_addr_rec_t), addr_cmp));
      profs[profc].call_site = (void*)(addr->sym);
      profs[profc].level = level++;
      PVN_SYSP_CALL(profs = (pvn_prof_rec_x*)realloc(profs, (++profc + 1u) * sizeof(pvn_prof_rec_x)));
    }
    else if (profc) {
      for (size_t i = (profc - 1u); ; --i) {
        if (profs[profc].this_fn == profs[i].this_fn) {
          /* assumes no wrap-around of the timer */
          profs[i].tns = (profs[profc].tns - profs[i].tns);
          break;
        }
        if (!i) {
          max_level = -(ssize_t)(profc + 1u);
          goto cleanup;
        }
      }
      --level;
    }
    else {
      max_level = (ssize_t)-1;
      goto cleanup;
    }
  }
#ifndef NDEBUG
  profs[profc].this_fn = NULL;
  profs[profc].call_site = NULL;
  profs[profc].tns = 0l;
  profs[profc].level = (size_t)0u;
#endif /* !NDEBUG */
  for (size_t i = 0u; i < profc; ++i) {
    if ((ssize_t)(profs[i].level) > max_level)
      max_level = (ssize_t)(profs[i].level);
    for (size_t j = 0u; j < profs[i].level; ++j) {
      PVN_SYSI_CALL(fputc('\t', tt) == EOF);
    }
    PVN_SYSI_CALL(fprintf(tt, "%s%ld ns\n", (const char*)(profs[i].call_site), profs[i].tns) < 7);
  }
 cleanup:
  free(profs);
  profs = (pvn_prof_rec_x*)NULL;
  for (size_t i = 0u; i < addrc; ++i) {
    free(addrs[i].sym);
#ifndef NDEBUG
    addrs[i].sym = (char*)NULL;
#endif /* !NDEBUG */
  }
  free(addrs);
  addrs = (pvn_addr_rec_t*)NULL;
  return max_level;
}

int main(int argc, char *argv[])
{
#ifdef PVN_PROFILE
  (void)printf("PVN_PROFILE=%u\n", PVN_PROFILE);
#endif /* PVN_PROFILE */
  if (argc != 2) {
    (void)fprintf(stderr, "%s prefix\n", *argv);
    return EXIT_FAILURE;
  }
  char fn[20] = { '\0' };
  (void)strcpy(strncpy(fn, argv[1], 16u) + 16, ".?t");
  (void)printf("FILES=%s\n", fn);
  fn[17] = 'p';
  PVN_SYSP_CALL(pt = fopen(fn, "rb"));
  fn[17] = 'b';
  PVN_SYSP_CALL(bt = fopen(fn, "rb"));
  fn[17] = 's';
  PVN_SYSP_CALL(st = fopen(fn, "rb"));
  fn[17] = 't';
  PVN_SYSP_CALL(tt = fopen(fn, "w"));
  (void)printf("DEPTH=%zd\n", proct());
  PVN_SYSI_CALL(fclose(tt));
  PVN_SYSI_CALL(fclose(st));
  PVN_SYSI_CALL(fclose(bt));
  PVN_SYSI_CALL(fclose(pt));
  return EXIT_SUCCESS;
}
#else /* !PVN_TEST */
#ifdef PVN_PROFILE
#if (PVN_PROFILE > 0u)
static pthread_mutex_t prof_lock = PTHREAD_MUTEX_INITIALIZER;
#ifndef PVN_THREAD
#define PVN_THREAD __thread
#else /* PVN_THREAD */
#error PVN_THREAD already defined
#endif /* ?PVN_THREAD */
#else /* PVN_PROFILE == 0u */
#ifndef PVN_THREAD
#define PVN_THREAD
#else /* PVN_THREAD */
#error PVN_THREAD already defined
#endif /* ?PVN_THREAD */
#endif /* ?PVN_PROFILE */
static void (*on_exit_ptr)(void) = (void (*)(void))NULL;
static void *bt_root = NULL;
static FILE *bt_file = (FILE*)NULL;
static FILE *st_file = (FILE*)NULL;
static PVN_THREAD char file_name[20];
static PVN_THREAD FILE *prof_file = (FILE*)NULL;

#ifndef PVN_PROFILE_TIMEREF
#ifdef CLOCK_MONOTONIC_RAW
#define PVN_PROFILE_TIMEREF CLOCK_MONOTONIC_RAW
#else /* !CLOCK_MONOTONIC_RAW */
#define PVN_PROFILE_TIMEREF CLOCK_MONOTONIC
#endif /* ?CLOCK_MONOTONIC_RAW */
#endif /* !PVN_PROFILE_TIMEREF */

static void PVN_NO_PROF pflerror(const char *const f, const int l)
{
  (void)fprintf(stderr, "\n%s(%d): %s\n", f, l, strerror(errno));
  (void)fflush(stderr);
}

static int PVN_NO_PROF bt_comp(const void *a, const void *b)
{
  if (a == b)
    return 0;
  const void *const *const A = (const void *const *)a;
  const void *const *const B = (const void *const *)b;
  if (*A < *B)
    return -1;
  if (*B < *A)
    return 1;
  return 0;
}

static void PVN_NO_PROF bt_action(const void *node, VISIT order, int /*level*/)
{
  pvn_addr_rec_t *const ar = *(pvn_addr_rec_t**)node;
  if ((order == postorder) || (order == leaf)) {
    const size_t sl = (size_t)(ar->fof + 1);
    ar->fof = ftell(st_file);
    if (fwrite(ar->sym, sizeof(char), sl, st_file) != sl) {
      pflerror(__FILE__, __LINE__);
      return;
    }
    if (fwrite(ar, (sizeof(*ar) - sizeof(ar->sym)), 1, bt_file) != 1)
      pflerror(__FILE__, __LINE__);
  }
}

static void PVN_NO_PROF bt_destroy()
{
  while (bt_root) {
    pvn_addr_rec_t *const p = *(pvn_addr_rec_t**)bt_root;
    void *const r = tdelete(p, &bt_root, bt_comp);
    if (p) {
      free(p->sym);
      free(p);
    }
    if (!r) {
      pflerror(__FILE__, __LINE__);
      return;
    }
  }
}

/* It is assumed that atexit() handlers are called when exactly one thread of the process has remained running. */
static void PVN_NO_PROF on_prog_exit()
{
#if (PVN_PROFILE > 0u)
  if (pthread_mutex_lock(&prof_lock)) {
    pflerror(__FILE__, __LINE__);
    return;
  }
#endif /* PVN_PROFILE */
  file_name[17] = 's';
  if (!(st_file = fopen(file_name, "wb"))) {
    pflerror(__FILE__, __LINE__);
    goto err;
  }
  file_name[17] = 'b';
  if (!(bt_file = fopen(file_name, "wb"))) {
    pflerror(__FILE__, __LINE__);
    goto err;
  }
  twalk(bt_root, bt_action);
 err:
  if (bt_file && fclose(bt_file))
    pflerror(__FILE__, __LINE__);
  if (st_file && fclose(st_file))
    pflerror(__FILE__, __LINE__);
  if (fflush((FILE*)NULL))
    pflerror(__FILE__, __LINE__);
  bt_destroy();
#if (PVN_PROFILE > 0u)
  if (pthread_mutex_unlock(&prof_lock))
    pflerror(__FILE__, __LINE__);
  if (pthread_mutex_destroy(&prof_lock))
    pflerror(__FILE__, __LINE__);
#endif /* PVN_PROFILE */
}

static int PVN_NO_PROF bt_insert(void *addr)
{
  if (!addr) {
    pflerror(__FILE__, __LINE__);
    return -1;
  }
#if (PVN_PROFILE > 0u)
  if (pthread_mutex_lock(&prof_lock)) {
    pflerror(__FILE__, __LINE__);
    return -2;
  }
#endif /* PVN_PROFILE */
  int r = 0;
  pvn_addr_rec_t **node = (pvn_addr_rec_t**)tfind((const void*)&addr, &bt_root, bt_comp);
  if (node)
    r = 1;
  else {
    Dl_info info;
    if (!dladdr(addr, (Dl_info*)memset(&info, 0, sizeof(info)))) {
      pflerror(__FILE__, __LINE__);
      r = -3;
    }
    else {
      pvn_addr_rec_t *const ar = (pvn_addr_rec_t*)malloc(sizeof(pvn_addr_rec_t));
      if (!ar) {
        pflerror(__FILE__, __LINE__);
        r = -4;
      }
      else {
        ar->addr = addr = info.dli_saddr;
        ar->fof = (long)(info.dli_sname ? strlen(info.dli_sname) : (size_t)0u);
        if (!(ar->sym = (char*)malloc(ar->fof + 2))) {
          free(ar);
          pflerror(__FILE__, __LINE__);
          r = -5;
        }
        else {
          (void)strcpy(ar->sym, (info.dli_sname ? info.dli_sname : ""));
          (ar->sym)[ar->fof] = '\n';
          (ar->sym)[ar->fof + 1] = '\0';
          if (!(node = (pvn_addr_rec_t**)tsearch((const void*)ar, &bt_root, bt_comp))) {
            free(ar->sym);
            free(ar);
            pflerror(__FILE__, __LINE__);
            r = -6;
          }
        }
      }
    }
  }
#if (PVN_PROFILE > 0u)
  if (pthread_mutex_unlock(&prof_lock)) {
    pflerror(__FILE__, __LINE__);
    r = -7;
  }
#endif /* PVN_PROFILE */
  return r;
}

PVN_EXTERN_C void PVN_NO_PROF __cyg_profile_func_enter(void *const this_fn, void *const call_site)
{
  if (!prof_file) {
    if (snprintf(file_name, sizeof(file_name), "%016tx.pt", (ptrdiff_t)pthread_self()) <= 0) {
      pflerror(__FILE__, __LINE__);
      return;
    }
    if (!(prof_file = fopen(file_name, "wb"))) {
      pflerror(__FILE__, __LINE__);
      return;
    }
#if (PVN_PROFILE > 0u)
    if (pthread_mutex_lock(&prof_lock)) {
      pflerror(__FILE__, __LINE__);
      return;
    }
#endif /* PVN_PROFILE */
    if (!on_exit_ptr) {
      if (atexit(on_prog_exit))
        pflerror(__FILE__, __LINE__);
      else
        on_exit_ptr = on_prog_exit;
    }
#if (PVN_PROFILE > 0u)
    if (pthread_mutex_unlock(&prof_lock))
      pflerror(__FILE__, __LINE__);
#endif /* PVN_PROFILE */
  }

  if (bt_insert(this_fn) < 0)
    return;

  pvn_prof_rec_t pr;
  pr.this_fn = this_fn;
  pr.call_site = call_site;
  struct timespec tv;
  if (clock_gettime(PVN_PROFILE_TIMEREF, (struct timespec*)memset(&tv, 0, sizeof(tv))))
    pflerror(__FILE__, __LINE__);
  pr.tns = pvn_t2ns(&tv);

  if (fwrite(&pr, sizeof(pr), 1, prof_file) != 1)
    pflerror(__FILE__, __LINE__);
}

PVN_EXTERN_C void PVN_NO_PROF __cyg_profile_func_exit(void *const this_fn, void *const /*call_site*/)
{
  pvn_prof_rec_t pr;
  pr.this_fn = this_fn;
  pr.call_site = NULL; /* a marker for func_exit */
  struct timespec tv;
  if (clock_gettime(PVN_PROFILE_TIMEREF, (struct timespec*)memset(&tv, 0, sizeof(tv))))
    pflerror(__FILE__, __LINE__);
  pr.tns = pvn_t2ns(&tv);

  if (fwrite(&pr, sizeof(pr), 1, prof_file) != 1)
    pflerror(__FILE__, __LINE__);
}
#endif /* PVN_PROFILE */
#endif /* ?PVN_TEST */
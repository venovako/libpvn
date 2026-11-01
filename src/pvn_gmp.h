#ifndef PVN_GMP_H
#define PVN_GMP_H

#ifndef PVN_H
#error pvn_gmp.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C int PVN_FABI(pvn_gmp_start,PVN_GMP_START)(mp_bitcnt_t *const prec);
PVN_EXTERN_C int PVN_FABI(pvn_gmp_stop,PVN_GMP_STOP)();

#endif /* !PVN_GMP_H */

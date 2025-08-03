#ifndef PVN_RAN_H
#define PVN_RAN_H

#ifndef PVN_H
#error pvn_ran.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C int PVN_FABI(pvn_ran_open,PVN_RAN_OPEN)();
PVN_EXTERN_C int PVN_FABI(pvn_ran_close,PVN_RAN_CLOSE)(const int *const u);

PVN_EXTERN_C int PVN_FABI(pvn_ran_16,PVN_RAN_16)(const int *const u, uint16_t *const r);
PVN_EXTERN_C int PVN_FABI(pvn_ran_32,PVN_RAN_32)(const int *const u, uint32_t *const r);
PVN_EXTERN_C int PVN_FABI(pvn_ran_64,PVN_RAN_64)(const int *const u, uint64_t *const r);

PVN_EXTERN_C float PVN_FABI(pvn_ran_safe_f,PVN_RAN_SAFE_F)(const int *const u, const int *const p);
PVN_EXTERN_C float PVN_FABI(pvn_ran_f,PVN_RAN_F)(const int *const u);
PVN_EXTERN_C double PVN_FABI(pvn_ran_safe,PVN_RAN_SAFE)(const int *const u, const int *const p);
PVN_EXTERN_C double PVN_FABI(pvn_ran,PVN_RAN)(const int *const u);
PVN_EXTERN_C long double PVN_FABI(pvn_ran_safe_l,PVN_RAN_SAFE_L)(const int *const u, const int *const p);
PVN_EXTERN_C long double PVN_FABI(pvn_ran_l,PVN_RAN_L)(const int *const u);
#ifdef PVN_QUADMATH
PVN_EXTERN_C __float128 PVN_FABI(pvn_ran_safe_q,PVN_RAN_SAFE_Q)(const int *const u, const int *const p);
PVN_EXTERN_C __float128 PVN_FABI(pvn_ran_q,PVN_RAN_Q)(const int *const u);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C long double PVN_FABI(pvn_ran_safe_q,PVN_RAN_SAFE_Q)(const int *const u, const int *const p);
PVN_EXTERN_C long double PVN_FABI(pvn_ran_q,PVN_RAN_Q)(const int *const u);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_RAN_H */

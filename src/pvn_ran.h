#ifndef PVN_RAN_H
#define PVN_RAN_H

#ifndef PVN_H
#error pvn_ran.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C int pvn_ran_open_();
PVN_EXTERN_C int pvn_ran_close_(const int *const u);

PVN_EXTERN_C float pvn_ran_safe_f_(const int *const u);
PVN_EXTERN_C float pvn_ran_f_(const int *const u);
PVN_EXTERN_C double pvn_ran_safe_(const int *const u);
PVN_EXTERN_C double pvn_ran_(const int *const u);
PVN_EXTERN_C long double pvn_ran_safe_l_(const int *const u);
PVN_EXTERN_C long double pvn_ran_l_(const int *const u);
#ifdef PVN_QUADMATH
PVN_EXTERN_C __float128 pvn_ran_safe_q_(const int *const u);
PVN_EXTERN_C __float128 pvn_ran_q_(const int *const u);
#else /* !PVN_QUADMATH */
PVN_EXTERN_C long double pvn_ran_safe_q_(const int *const u);
PVN_EXTERN_C long double pvn_ran_q_(const int *const u);
#endif /* ?PVN_QUADMATH */
#endif /* !PVN_RAN_H */

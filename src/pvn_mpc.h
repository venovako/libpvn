#ifndef PVN_MPC_H
#define PVN_MPC_H

#ifndef PVN_H
#error pvn_mpc.h not intended for direct inclusion
#endif /* !PVN_H */

PVN_EXTERN_C int PVN_FABI(pvn_mpc_start,PVN_MPC_START)(mpfr_rnd_t *const rnd, mpfr_prec_t *const prec, mpfr_exp_t *const emin, mpfr_exp_t *const emax);
PVN_EXTERN_C int PVN_FABI(pvn_mpc_stop,PVN_MPC_STOP)();

#endif /* !PVN_MPC_H */

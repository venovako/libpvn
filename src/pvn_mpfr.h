#ifndef PVN_MPFR_H
#define PVN_MPFR_H

#ifndef PVN_H
#error pvn_mpfr.h not intended for direct inclusion
#endif /* !PVN_H */

/* Fortran: __MPFR_RND_INVALID = -1_MPFR_RND_KIND */
#ifdef __MPFR_RND_INVALID
#error __MPFR_RND_INVALID already defined
#else /* !__MPFR_RND_INVALID */
#define __MPFR_RND_INVALID MPFR_RNDNA
#endif /* ?__MPFR_RND_INVALID */

/* Fortran: __MPFR_PREC_INVALID = 0_MPFR_PREC_KIND */
#ifdef __MPFR_PREC_INVALID
#error __MPFR_PREC_INVALID already defined
#else /* !__MPFR_PREC_INVALID */
#define __MPFR_PREC_INVALID (mpfr_prec_t)0
#endif /* ?__MPFR_PREC_INVALID */

/* Fortran: __MPFR_EXP_INVALID = -HUGE(0_MPFR_EXP_KIND)-1_MPFR_EXP_KIND */
#ifdef __MPFR_EXP_INVALID
#error __MPFR_EXP_INVALID already defined
#else /* !__MPFR_EXP_INVALID */
#define __MPFR_EXP_INVALID (-__MPFR_EXP_MAX-(mpfr_exp_t)1)
#endif /* ?__MPFR_EXP_INVALID */

PVN_EXTERN_C int PVN_FABI(pvn_mpfr_start,PVN_MPFR_START)(mpfr_rnd_t *const rnd, mpfr_prec_t *const prec, mpfr_exp_t *const emin, mpfr_exp_t *const emax);
PVN_EXTERN_C int PVN_FABI(pvn_mpfr_stop,PVN_MPFR_STOP)();

#endif /* !PVN_MPFR_H */

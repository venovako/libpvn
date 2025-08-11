#ifndef PVN_NRM_H
#define PVN_NRM_H

#ifndef PVN_H
#error pvn_nrm.h not intended for direct inclusion
#endif /* !PVN_H */

#ifdef PVN_MPFR
PVN_EXTERN_C float PVN_FABI(pvn_mps_nrmf,PVN_MPS_NRMF)(const size_t *const n, const float *const x);
PVN_EXTERN_C double PVN_FABI(pvn_mpd_nrmf,PVN_MPD_NRMF)(const size_t *const n, const double *const x);
PVN_EXTERN_C long double PVN_FABI(pvn_mpx_nrmf,PVN_MPX_NRMF)(const size_t *const n, const long double *const x);
#endif /* PVN_MPFR */

#ifdef PVN_LAPACK
/* just calls xNRM2 */
PVN_EXTERN_C float PVN_FABI(pvn_las_nrmf,PVN_LAS_NRMF)(const size_t *const n, const float *const x);
PVN_EXTERN_C double PVN_FABI(pvn_lad_nrmf,PVN_LAD_NRMF)(const size_t *const n, const double *const x);
#endif /* PVN_LAPACK */

PVN_EXTERN_C float PVN_FABI(pvn_res_nrmf,PVN_RES_NRMF)(const size_t *const n, const float *const x);
PVN_EXTERN_C double PVN_FABI(pvn_red_nrmf,PVN_RED_NRMF)(const size_t *const n, const double *const x);
PVN_EXTERN_C long double PVN_FABI(pvn_rex_nrmf,PVN_REX_NRMF)(const size_t *const n, const long double *const x);

PVN_EXTERN_C float PVN_FABI(pvn_crs_nrmf,PVN_CRS_NRMF)(const size_t *const n, const float *const x);
PVN_EXTERN_C double PVN_FABI(pvn_crd_nrmf,PVN_CRD_NRMF)(const size_t *const n, const double *const x);
PVN_EXTERN_C long double PVN_FABI(pvn_crx_nrmf,PVN_CRX_NRMF)(const size_t *const n, const long double *const x);

PVN_EXTERN_C float PVN_FABI(pvn_snrm2,PVN_SNRM2)(const size_t *const n, const float *const x);
PVN_EXTERN_C float PVN_FABI(pvn_cnrm2,PVN_CNRM2)(const size_t *const n, const float *const x);
PVN_EXTERN_C double PVN_FABI(pvn_dnrm2,PVN_DNRM2)(const size_t *const n, const double *const x);
PVN_EXTERN_C double PVN_FABI(pvn_znrm2,PVN_ZNRM2)(const size_t *const n, const double *const x);
PVN_EXTERN_C long double PVN_FABI(pvn_xnrm2,PVN_XNRM2)(const size_t *const n, const long double *const x);
PVN_EXTERN_C long double PVN_FABI(pvn_wnrm2,PVN_WNRM2)(const size_t *const n, const long double *const x);

#ifdef PVN_QUADMATH
#ifdef PVN_MPFR
PVN_EXTERN_C __float128 PVN_FABI(pvn_mpq_nrmf,PVN_MPQ_NRMF)(const size_t *const n, const __float128 *const x);
#endif /* PVN_MPFR */
PVN_EXTERN_C __float128 PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(const size_t *const n, const __float128 *const x);
PVN_EXTERN_C __float128 PVN_FABI(pvn_crq_nrmf,PVN_CRQ_NRMF)(const size_t *const n, const __float128 *const x);
PVN_EXTERN_C __float128 PVN_FABI(pvn_qnrm2,PVN_QNRM2)(const size_t *const n, const __float128 *const x);
PVN_EXTERN_C __float128 PVN_FABI(pvn_ynrm2,PVN_YNRM2)(const size_t *const n, const __float128 *const x);
#else /* !PVN_QUADMATH */
#ifdef PVN_MPFR
PVN_EXTERN_C long double PVN_FABI(pvn_mpq_nrmf,PVN_MPQ_NRMF)(const size_t *const n, const long double *const x);
#endif /* PVN_MPFR */
PVN_EXTERN_C long double PVN_FABI(pvn_req_nrmf,PVN_REQ_NRMF)(const size_t *const n, const long double *const x);
PVN_EXTERN_C long double PVN_FABI(pvn_crq_nrmf,PVN_CRQ_NRMF)(const size_t *const n, const long double *const x);
PVN_EXTERN_C long double PVN_FABI(pvn_qnrm2,PVN_QNRM2)(const size_t *const n, const long double *const x);
PVN_EXTERN_C long double PVN_FABI(pvn_ynrm2,PVN_YNRM2)(const size_t *const n, const long double *const x);
#endif /* ?PVN_QUADMATH */

/* with __builtin_hypot[fl] */
PVN_EXTERN_C float PVN_FABI(pvn_rfs_nrmf,PVN_RFS_NRMF)(const size_t *const n, const float *const x);
PVN_EXTERN_C double PVN_FABI(pvn_rfd_nrmf,PVN_RFD_NRMF)(const size_t *const n, const double *const x);
PVN_EXTERN_C long double PVN_FABI(pvn_rfx_nrmf,PVN_RFX_NRMF)(const size_t *const n, const long double *const x);

/* when x is veclen-aligned */
#if (defined(__AVX__) && defined(__FMA__))
PVN_EXTERN_C float PVN_FABI(pvn_rxs_nrmf,PVN_RXS_NRMF)(const size_t *const n, const float *const x);
PVN_EXTERN_C double PVN_FABI(pvn_rxd_nrmf,PVN_RXD_NRMF)(const size_t *const n, const double *const x);
PVN_EXTERN_C float PVN_FABI(pvn_rys_nrmf,PVN_RYS_NRMF)(const size_t *const n, const float *const x);
PVN_EXTERN_C double PVN_FABI(pvn_ryd_nrmf,PVN_RYD_NRMF)(const size_t *const n, const double *const x);
#ifdef __AVX512F__
PVN_EXTERN_C float PVN_FABI(pvn_rzs_nrmf,PVN_RZS_NRMF)(const size_t *const n, const float *const x);
PVN_EXTERN_C double PVN_FABI(pvn_rzd_nrmf,PVN_RZD_NRMF)(const size_t *const n, const double *const x);
#endif /* __AVX512F__ */
#endif /* __AVX__ && __FMA__ */
#endif /* !PVN_NRM_H */

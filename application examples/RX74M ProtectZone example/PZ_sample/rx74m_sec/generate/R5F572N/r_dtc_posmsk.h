#ifndef R_DTC_POSMSK_H
#define R_DTC_POSMSK_H

/* =========================================================================================================================== */
/* ================                                Pos/Mask Peripheral Section                                ================ */
/* =========================================================================================================================== */

/** @addtogroup PosMask_peripherals
 * @{
 */

/* =========================================================================================================================== */
/* ================                                           R_DTC                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  DTCCR  ========================================================= */
#define R_DTC_DTCCR_RRS_Pos          (4UL)          /*!< RRS (Bit 4)                                           */
#define R_DTC_DTCCR_RRS_Msk          (0x10UL)       /*!< RRS (Bitfield-Mask: 0x01)                             */
/* ========================================================  DTCVBR  ========================================================= */
#define R_DTC_DTCVBR_DTCVBR_Pos      (0UL)          /*!< DTCVBR (Bit 0)                                        */
#define R_DTC_DTCVBR_DTCVBR_Msk      (0xffffffffUL) /*!< DTCVBR (Bitfield-Mask: 0xffffffff)                    */
/* =======================================================  DTCADMOD  ======================================================== */
#define R_DTC_DTCADMOD_SHORT_Pos     (0UL)          /*!< SHORT (Bit 0)                                         */
#define R_DTC_DTCADMOD_SHORT_Msk     (0x1UL)        /*!< SHORT (Bitfield-Mask: 0x01)                           */
/* =========================================================  DTCST  ========================================================= */
#define R_DTC_DTCST_DTCST_Pos        (0UL)          /*!< DTCST (Bit 0)                                         */
#define R_DTC_DTCST_DTCST_Msk        (0x1UL)        /*!< DTCST (Bitfield-Mask: 0x01)                           */
/* ========================================================  DTCSTS  ========================================================= */
#define R_DTC_DTCSTS_ACT_Pos         (15UL)         /*!< ACT (Bit 15)                                          */
#define R_DTC_DTCSTS_ACT_Msk         (0x8000UL)     /*!< ACT (Bitfield-Mask: 0x01)                             */
#define R_DTC_DTCSTS_VECN_Pos        (0UL)          /*!< VECN (Bit 0)                                          */
#define R_DTC_DTCSTS_VECN_Msk        (0xffUL)       /*!< VECN (Bitfield-Mask: 0xff)                            */
/* ========================================================  DTCIBR  ========================================================= */
#define R_DTC_DTCIBR_DTCIBR_Pos      (0UL)          /*!< DTCIBR (Bit 0)                                        */
#define R_DTC_DTCIBR_DTCIBR_Msk      (0xffffffffUL) /*!< DTCIBR (Bitfield-Mask: 0xffffffff)                    */
/* =========================================================  DTCOR  ========================================================= */
#define R_DTC_DTCOR_SQTFL_Pos        (0UL)          /*!< SQTFL (Bit 0)                                         */
#define R_DTC_DTCOR_SQTFL_Msk        (0x1UL)        /*!< SQTFL (Bitfield-Mask: 0x01)                           */
/* ========================================================  DTCSQE  ========================================================= */
#define R_DTC_DTCSQE_ESPSEL_Pos      (15UL)         /*!< ESPSEL (Bit 15)                                       */
#define R_DTC_DTCSQE_ESPSEL_Msk      (0x8000UL)     /*!< ESPSEL (Bitfield-Mask: 0x01)                          */
#define R_DTC_DTCSQE_VECN_Pos        (0UL)          /*!< VECN (Bit 0)                                          */
#define R_DTC_DTCSQE_VECN_Msk        (0xffUL)       /*!< VECN (Bitfield-Mask: 0xff)                            */
/* ========================================================  DTCDISP  ======================================================== */
#define R_DTC_DTCDISP_DTCDISP_Pos    (0UL)          /*!< DTCDISP (Bit 0)                                       */
#define R_DTC_DTCDISP_DTCDISP_Msk    (0xffffffffUL) /*!< DTCDISP (Bitfield-Mask: 0xffffffff)                   */

/** @} */ /* End of group PosMask_peripherals */

#endif /* R_DTC_POSMARK_H */

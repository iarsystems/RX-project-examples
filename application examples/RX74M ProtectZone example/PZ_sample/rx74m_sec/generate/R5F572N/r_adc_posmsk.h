#ifndef R_ADC_POSMSK_H
#define R_ADC_POSMSK_H

/* =========================================================================================================================== */
/* ================                                           R_ADC0                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  ADCSR  ========================================================= */
#define R_ADC0_ADCSR_ADST_Pos            (15UL)     /*!< ADST (Bit 15)                                         */
#define R_ADC0_ADCSR_ADST_Msk            (0x8000UL) /*!< ADST (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADCSR_ADCS_Pos            (13UL)     /*!< ADCS (Bit 13)                                         */
#define R_ADC0_ADCSR_ADCS_Msk            (0x6000UL) /*!< ADCS (Bitfield-Mask: 0x03)                            */
#define R_ADC0_ADCSR_ADIE_Pos            (12UL)     /*!< ADIE (Bit 12)                                         */
#define R_ADC0_ADCSR_ADIE_Msk            (0x1000UL) /*!< ADIE (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADCSR_TRGE_Pos            (9UL)      /*!< TRGE (Bit 9)                                          */
#define R_ADC0_ADCSR_TRGE_Msk            (0x200UL)  /*!< TRGE (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADCSR_EXTRG_Pos           (8UL)      /*!< EXTRG (Bit 8)                                         */
#define R_ADC0_ADCSR_EXTRG_Msk           (0x100UL)  /*!< EXTRG (Bitfield-Mask: 0x01)                           */
#define R_ADC0_ADCSR_DBLE_Pos            (7UL)      /*!< DBLE (Bit 7)                                          */
#define R_ADC0_ADCSR_DBLE_Msk            (0x80UL)   /*!< DBLE (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADCSR_GBADIE_Pos          (6UL)      /*!< GBADIE (Bit 6)                                        */
#define R_ADC0_ADCSR_GBADIE_Msk          (0x40UL)   /*!< GBADIE (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADCSR_DBLANS_Pos          (0UL)      /*!< DBLANS (Bit 0)                                        */
#define R_ADC0_ADCSR_DBLANS_Msk          (0x1fUL)   /*!< DBLANS (Bitfield-Mask: 0x1f)                          */
/* ========================================================  ADANSA  ========================================================= */
#define R_ADC0_ADANSA_ANSA015_Pos        (15UL)     /*!< ANSA015 (Bit 15)                                      */
#define R_ADC0_ADANSA_ANSA015_Msk        (0x8000UL) /*!< ANSA015 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA014_Pos        (14UL)     /*!< ANSA014 (Bit 14)                                      */
#define R_ADC0_ADANSA_ANSA014_Msk        (0x4000UL) /*!< ANSA014 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA013_Pos        (13UL)     /*!< ANSA013 (Bit 13)                                      */
#define R_ADC0_ADANSA_ANSA013_Msk        (0x2000UL) /*!< ANSA013 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA012_Pos        (12UL)     /*!< ANSA012 (Bit 12)                                      */
#define R_ADC0_ADANSA_ANSA012_Msk        (0x1000UL) /*!< ANSA012 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA011_Pos        (11UL)     /*!< ANSA011 (Bit 11)                                      */
#define R_ADC0_ADANSA_ANSA011_Msk        (0x800UL)  /*!< ANSA011 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA010_Pos        (10UL)     /*!< ANSA010 (Bit 10)                                      */
#define R_ADC0_ADANSA_ANSA010_Msk        (0x400UL)  /*!< ANSA010 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA009_Pos        (9UL)      /*!< ANSA009 (Bit 9)                                       */
#define R_ADC0_ADANSA_ANSA009_Msk        (0x200UL)  /*!< ANSA009 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA008_Pos        (8UL)      /*!< ANSA008 (Bit 8)                                       */
#define R_ADC0_ADANSA_ANSA008_Msk        (0x100UL)  /*!< ANSA008 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA007_Pos        (7UL)      /*!< ANSA007 (Bit 7)                                       */
#define R_ADC0_ADANSA_ANSA007_Msk        (0x80UL)   /*!< ANSA007 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA006_Pos        (6UL)      /*!< ANSA006 (Bit 6)                                       */
#define R_ADC0_ADANSA_ANSA006_Msk        (0x40UL)   /*!< ANSA006 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA005_Pos        (5UL)      /*!< ANSA005 (Bit 5)                                       */
#define R_ADC0_ADANSA_ANSA005_Msk        (0x20UL)   /*!< ANSA005 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA004_Pos        (4UL)      /*!< ANSA004 (Bit 4)                                       */
#define R_ADC0_ADANSA_ANSA004_Msk        (0x10UL)   /*!< ANSA004 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA003_Pos        (3UL)      /*!< ANSA003 (Bit 3)                                       */
#define R_ADC0_ADANSA_ANSA003_Msk        (0x8UL)    /*!< ANSA003 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA002_Pos        (2UL)      /*!< ANSA002 (Bit 2)                                       */
#define R_ADC0_ADANSA_ANSA002_Msk        (0x4UL)    /*!< ANSA002 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA001_Pos        (1UL)      /*!< ANSA001 (Bit 1)                                       */
#define R_ADC0_ADANSA_ANSA001_Msk        (0x2UL)    /*!< ANSA001 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSA_ANSA000_Pos        (0UL)      /*!< ANSA000 (Bit 0)                                       */
#define R_ADC0_ADANSA_ANSA000_Msk        (0x1UL)    /*!< ANSA000 (Bitfield-Mask: 0x01)                         */
/* =========================================================  ADADS  ========================================================= */
#define R_ADC0_ADADS_ADS015_Pos          (15UL)     /*!< ADS015 (Bit 15)                                       */
#define R_ADC0_ADADS_ADS015_Msk          (0x8000UL) /*!< ADS015 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS014_Pos          (14UL)     /*!< ADS014 (Bit 14)                                       */
#define R_ADC0_ADADS_ADS014_Msk          (0x4000UL) /*!< ADS014 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS013_Pos          (13UL)     /*!< ADS013 (Bit 13)                                       */
#define R_ADC0_ADADS_ADS013_Msk          (0x2000UL) /*!< ADS013 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS012_Pos          (12UL)     /*!< ADS012 (Bit 12)                                       */
#define R_ADC0_ADADS_ADS012_Msk          (0x1000UL) /*!< ADS012 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS011_Pos          (11UL)     /*!< ADS011 (Bit 11)                                       */
#define R_ADC0_ADADS_ADS011_Msk          (0x800UL)  /*!< ADS011 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS010_Pos          (10UL)     /*!< ADS010 (Bit 10)                                       */
#define R_ADC0_ADADS_ADS010_Msk          (0x400UL)  /*!< ADS010 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS009_Pos          (9UL)      /*!< ADS009 (Bit 9)                                        */
#define R_ADC0_ADADS_ADS009_Msk          (0x200UL)  /*!< ADS009 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS008_Pos          (8UL)      /*!< ADS008 (Bit 8)                                        */
#define R_ADC0_ADADS_ADS008_Msk          (0x100UL)  /*!< ADS008 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS007_Pos          (7UL)      /*!< ADS007 (Bit 7)                                        */
#define R_ADC0_ADADS_ADS007_Msk          (0x80UL)   /*!< ADS007 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS006_Pos          (6UL)      /*!< ADS006 (Bit 6)                                        */
#define R_ADC0_ADADS_ADS006_Msk          (0x40UL)   /*!< ADS006 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS005_Pos          (5UL)      /*!< ADS005 (Bit 5)                                        */
#define R_ADC0_ADADS_ADS005_Msk          (0x20UL)   /*!< ADS005 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS004_Pos          (4UL)      /*!< ADS004 (Bit 4)                                        */
#define R_ADC0_ADADS_ADS004_Msk          (0x10UL)   /*!< ADS004 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS003_Pos          (3UL)      /*!< ADS003 (Bit 3)                                        */
#define R_ADC0_ADADS_ADS003_Msk          (0x8UL)    /*!< ADS003 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS002_Pos          (2UL)      /*!< ADS002 (Bit 2)                                        */
#define R_ADC0_ADADS_ADS002_Msk          (0x4UL)    /*!< ADS002 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS001_Pos          (1UL)      /*!< ADS001 (Bit 1)                                        */
#define R_ADC0_ADADS_ADS001_Msk          (0x2UL)    /*!< ADS001 (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADADS_ADS000_Pos          (0UL)      /*!< ADS000 (Bit 0)                                        */
#define R_ADC0_ADADS_ADS000_Msk          (0x1UL)    /*!< ADS000 (Bitfield-Mask: 0x01)                          */
/* =========================================================  ADADC  ========================================================= */
#define R_ADC0_ADADC_AVEE_Pos            (7UL)      /*!< AVEE (Bit 7)                                          */
#define R_ADC0_ADADC_AVEE_Msk            (0x80UL)   /*!< AVEE (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADADC_ADC_Pos             (0UL)      /*!< ADC (Bit 0)                                           */
#define R_ADC0_ADADC_ADC_Msk             (0x7UL)    /*!< ADC (Bitfield-Mask: 0x07)                             */
/* =========================================================  ADCER  ========================================================= */
#define R_ADC0_ADCER_ADRFMT_Pos          (15UL)     /*!< ADRFMT (Bit 15)                                       */
#define R_ADC0_ADCER_ADRFMT_Msk          (0x8000UL) /*!< ADRFMT (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADCER_DIAGM_Pos           (11UL)     /*!< DIAGM (Bit 11)                                        */
#define R_ADC0_ADCER_DIAGM_Msk           (0x800UL)  /*!< DIAGM (Bitfield-Mask: 0x01)                           */
#define R_ADC0_ADCER_DIAGLD_Pos          (10UL)     /*!< DIAGLD (Bit 10)                                       */
#define R_ADC0_ADCER_DIAGLD_Msk          (0x400UL)  /*!< DIAGLD (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADCER_DIAGVAL_Pos         (8UL)      /*!< DIAGVAL (Bit 8)                                       */
#define R_ADC0_ADCER_DIAGVAL_Msk         (0x300UL)  /*!< DIAGVAL (Bitfield-Mask: 0x03)                         */
#define R_ADC0_ADCER_ACE_Pos             (5UL)      /*!< ACE (Bit 5)                                           */
#define R_ADC0_ADCER_ACE_Msk             (0x20UL)   /*!< ACE (Bitfield-Mask: 0x01)                             */
#define R_ADC0_ADCER_ADPRC_Pos           (1UL)      /*!< ADPRC (Bit 1)                                         */
#define R_ADC0_ADCER_ADPRC_Msk           (0x6UL)    /*!< ADPRC (Bitfield-Mask: 0x03)                           */
/* ========================================================  ADSTRGR  ======================================================== */
#define R_ADC0_ADSTRGR_TRSA_Pos          (8UL)      /*!< TRSA (Bit 8)                                          */
#define R_ADC0_ADSTRGR_TRSA_Msk          (0x3f00UL) /*!< TRSA (Bitfield-Mask: 0x3f)                            */
#define R_ADC0_ADSTRGR_TRSB_Pos          (0UL)      /*!< TRSB (Bit 0)                                          */
#define R_ADC0_ADSTRGR_TRSB_Msk          (0x3fUL)   /*!< TRSB (Bitfield-Mask: 0x3f)                            */
/* ========================================================  ADEXICR  ======================================================== */
#define R_ADC0_ADEXICR_EXOEN_Pos         (15UL)     /*!< EXOEN (Bit 15)                                        */
#define R_ADC0_ADEXICR_EXOEN_Msk         (0x8000UL) /*!< EXOEN (Bitfield-Mask: 0x01)                           */
#define R_ADC0_ADEXICR_EXSEL_Pos         (13UL)     /*!< EXSEL (Bit 13)                                        */
#define R_ADC0_ADEXICR_EXSEL_Msk         (0x6000UL) /*!< EXSEL (Bitfield-Mask: 0x03)                           */
#define R_ADC0_ADEXICR_OCSB_Pos          (11UL)     /*!< OCSB (Bit 11)                                         */
#define R_ADC0_ADEXICR_OCSB_Msk          (0x800UL)  /*!< OCSB (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADEXICR_TSSB_Pos          (10UL)     /*!< TSSB (Bit 10)                                         */
#define R_ADC0_ADEXICR_TSSB_Msk          (0x400UL)  /*!< TSSB (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADEXICR_OCSA_Pos          (9UL)      /*!< OCSA (Bit 9)                                          */
#define R_ADC0_ADEXICR_OCSA_Msk          (0x200UL)  /*!< OCSA (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADEXICR_TSSA_Pos          (8UL)      /*!< TSSA (Bit 8)                                          */
#define R_ADC0_ADEXICR_TSSA_Msk          (0x100UL)  /*!< TSSA (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADEXICR_OCSAD_Pos         (1UL)      /*!< OCSAD (Bit 1)                                         */
#define R_ADC0_ADEXICR_OCSAD_Msk         (0x2UL)    /*!< OCSAD (Bitfield-Mask: 0x01)                           */
#define R_ADC0_ADEXICR_TSSAD_Pos         (0UL)      /*!< TSSAD (Bit 0)                                         */
#define R_ADC0_ADEXICR_TSSAD_Msk         (0x1UL)    /*!< TSSAD (Bitfield-Mask: 0x01)                           */
/* ========================================================  ADANSB  ========================================================= */
#define R_ADC0_ADANSB_ANSB015_Pos        (15UL)     /*!< ANSB015 (Bit 15)                                      */
#define R_ADC0_ADANSB_ANSB015_Msk        (0x8000UL) /*!< ANSB015 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB014_Pos        (14UL)     /*!< ANSB014 (Bit 14)                                      */
#define R_ADC0_ADANSB_ANSB014_Msk        (0x4000UL) /*!< ANSB014 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB013_Pos        (13UL)     /*!< ANSB013 (Bit 13)                                      */
#define R_ADC0_ADANSB_ANSB013_Msk        (0x2000UL) /*!< ANSB013 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB012_Pos        (12UL)     /*!< ANSB012 (Bit 12)                                      */
#define R_ADC0_ADANSB_ANSB012_Msk        (0x1000UL) /*!< ANSB012 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB011_Pos        (11UL)     /*!< ANSB011 (Bit 11)                                      */
#define R_ADC0_ADANSB_ANSB011_Msk        (0x800UL)  /*!< ANSB011 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB010_Pos        (10UL)     /*!< ANSB010 (Bit 10)                                      */
#define R_ADC0_ADANSB_ANSB010_Msk        (0x400UL)  /*!< ANSB010 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB009_Pos        (9UL)      /*!< ANSB009 (Bit 9)                                       */
#define R_ADC0_ADANSB_ANSB009_Msk        (0x200UL)  /*!< ANSB009 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB008_Pos        (8UL)      /*!< ANSB008 (Bit 8)                                       */
#define R_ADC0_ADANSB_ANSB008_Msk        (0x100UL)  /*!< ANSB008 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB007_Pos        (7UL)      /*!< ANSB007 (Bit 7)                                       */
#define R_ADC0_ADANSB_ANSB007_Msk        (0x80UL)   /*!< ANSB007 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB006_Pos        (6UL)      /*!< ANSB006 (Bit 6)                                       */
#define R_ADC0_ADANSB_ANSB006_Msk        (0x40UL)   /*!< ANSB006 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB005_Pos        (5UL)      /*!< ANSB005 (Bit 5)                                       */
#define R_ADC0_ADANSB_ANSB005_Msk        (0x20UL)   /*!< ANSB005 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB004_Pos        (4UL)      /*!< ANSB004 (Bit 4)                                       */
#define R_ADC0_ADANSB_ANSB004_Msk        (0x10UL)   /*!< ANSB004 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB003_Pos        (3UL)      /*!< ANSB003 (Bit 3)                                       */
#define R_ADC0_ADANSB_ANSB003_Msk        (0x8UL)    /*!< ANSB003 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB002_Pos        (2UL)      /*!< ANSB002 (Bit 2)                                       */
#define R_ADC0_ADANSB_ANSB002_Msk        (0x4UL)    /*!< ANSB002 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB001_Pos        (1UL)      /*!< ANSB001 (Bit 1)                                       */
#define R_ADC0_ADANSB_ANSB001_Msk        (0x2UL)    /*!< ANSB001 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSB_ANSB000_Pos        (0UL)      /*!< ANSB000 (Bit 0)                                       */
#define R_ADC0_ADANSB_ANSB000_Msk        (0x1UL)    /*!< ANSB000 (Bitfield-Mask: 0x01)                         */
/* ========================================================  ADDBLDR  ======================================================== */
#define R_ADC0_ADDBLDR_ADDBLDR_Pos       (0UL)      /*!< ADDBLDR (Bit 0)                                       */
#define R_ADC0_ADDBLDR_ADDBLDR_Msk       (0xffffUL) /*!< ADDBLDR (Bitfield-Mask: 0xffff)                       */
/* ========================================================  ADTSDR  ========================================================= */
#define R_ADC0_ADTSDR_ADTSDR_Pos         (0UL)      /*!< ADTSDR (Bit 0)                                        */
#define R_ADC0_ADTSDR_ADTSDR_Msk         (0xffffUL) /*!< ADTSDR (Bitfield-Mask: 0xffff)                        */
/* ========================================================  ADOCDR  ========================================================= */
#define R_ADC0_ADOCDR_ADOCDR_Pos         (0UL)      /*!< ADOCDR (Bit 0)                                        */
#define R_ADC0_ADOCDR_ADOCDR_Msk         (0xffffUL) /*!< ADOCDR (Bitfield-Mask: 0xffff)                        */
/* ======================================================  ADRD_RIGHT  ======================================================= */
#define R_ADC0_ADRD_RIGHT_DIAGST_Pos     (14UL)     /*!< DIAGST (Bit 14)                                       */
#define R_ADC0_ADRD_RIGHT_DIAGST_Msk     (0xc000UL) /*!< DIAGST (Bitfield-Mask: 0x03)                          */
#define R_ADC0_ADRD_RIGHT_AD_Pos         (0UL)      /*!< AD (Bit 0)                                            */
#define R_ADC0_ADRD_RIGHT_AD_Msk         (0x3fffUL) /*!< AD (Bitfield-Mask: 0x3fff)                            */
/* =======================================================  ADRD_LEFT  ======================================================= */
#define R_ADC0_ADRD_LEFT_AD_Pos          (2UL)      /*!< AD (Bit 2)                                            */
#define R_ADC0_ADRD_LEFT_AD_Msk          (0xfffcUL) /*!< AD (Bitfield-Mask: 0x3fff)                            */
#define R_ADC0_ADRD_LEFT_DIAGST_Pos      (0UL)      /*!< DIAGST (Bit 0)                                        */
#define R_ADC0_ADRD_LEFT_DIAGST_Msk      (0x3UL)    /*!< DIAGST (Bitfield-Mask: 0x03)                          */
/* =========================================================  ADDR  ========================================================== */
#define R_ADC0_ADDR_ADDR_Pos             (0UL)      /*!< ADDR (Bit 0)                                          */
#define R_ADC0_ADDR_ADDR_Msk             (0xffffUL) /*!< ADDR (Bitfield-Mask: 0xffff)                          */
/* ========================================================  ADSAMPR  ======================================================== */
#define R_ADC0_ADSAMPR_PRO_Pos           (0UL)      /*!< PRO (Bit 0)                                           */
#define R_ADC0_ADSAMPR_PRO_Msk           (0x3UL)    /*!< PRO (Bitfield-Mask: 0x03)                             */
/* ========================================================  ADSHCR  ========================================================= */
#define R_ADC0_ADSHCR_SHANS_Pos          (8UL)      /*!< SHANS (Bit 8)                                         */
#define R_ADC0_ADSHCR_SHANS_Msk          (0x700UL)  /*!< SHANS (Bitfield-Mask: 0x07)                           */
#define R_ADC0_ADSHCR_SSTSH_Pos          (0UL)      /*!< SSTSH (Bit 0)                                         */
#define R_ADC0_ADSHCR_SSTSH_Msk          (0xffUL)   /*!< SSTSH (Bitfield-Mask: 0xff)                           */
/* =========================================================  ADSAM  ========================================================= */
#define R_ADC0_ADSAM_SAM_Pos             (5UL)      /*!< SAM (Bit 5)                                           */
#define R_ADC0_ADSAM_SAM_Msk             (0x20UL)   /*!< SAM (Bitfield-Mask: 0x01)                             */
/* ========================================================  ADDISCR  ======================================================== */
#define R_ADC0_ADDISCR_ADNDIS_Pos        (0UL)      /*!< ADNDIS (Bit 0)                                        */
#define R_ADC0_ADDISCR_ADNDIS_Msk        (0x1fUL)   /*!< ADNDIS (Bitfield-Mask: 0x1f)                          */
/* ========================================================  ADSHMSR  ======================================================== */
#define R_ADC0_ADSHMSR_SHMD_Pos          (0UL)      /*!< SHMD (Bit 0)                                          */
#define R_ADC0_ADSHMSR_SHMD_Msk          (0x1UL)    /*!< SHMD (Bitfield-Mask: 0x01)                            */
/* ========================================================  ADGSPCR  ======================================================== */
#define R_ADC0_ADGSPCR_GBRP_Pos          (15UL)     /*!< GBRP (Bit 15)                                         */
#define R_ADC0_ADGSPCR_GBRP_Msk          (0x8000UL) /*!< GBRP (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADGSPCR_LGRRS_Pos         (14UL)     /*!< LGRRS (Bit 14)                                        */
#define R_ADC0_ADGSPCR_LGRRS_Msk         (0x4000UL) /*!< LGRRS (Bitfield-Mask: 0x01)                           */
#define R_ADC0_ADGSPCR_GBRSCN_Pos        (1UL)      /*!< GBRSCN (Bit 1)                                        */
#define R_ADC0_ADGSPCR_GBRSCN_Msk        (0x2UL)    /*!< GBRSCN (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADGSPCR_PGS_Pos           (0UL)      /*!< PGS (Bit 0)                                           */
#define R_ADC0_ADGSPCR_PGS_Msk           (0x1UL)    /*!< PGS (Bitfield-Mask: 0x01)                             */
/* =======================================================  ADDBLDRA  ======================================================== */
#define R_ADC0_ADDBLDRA_ADDBLDRA_Pos     (0UL)      /*!< ADDBLDRA (Bit 0)                                      */
#define R_ADC0_ADDBLDRA_ADDBLDRA_Msk     (0xffffUL) /*!< ADDBLDRA (Bitfield-Mask: 0xffff)                      */
/* =======================================================  ADDBLDRB  ======================================================== */
#define R_ADC0_ADDBLDRB_ADDBLDRB_Pos     (0UL)      /*!< ADDBLDRB (Bit 0)                                      */
#define R_ADC0_ADDBLDRB_ADDBLDRB_Msk     (0xffffUL) /*!< ADDBLDRB (Bitfield-Mask: 0xffff)                      */
/* =======================================================  ADWINMON  ======================================================== */
#define R_ADC0_ADWINMON_MONCMPB_Pos      (5UL)      /*!< MONCMPB (Bit 5)                                       */
#define R_ADC0_ADWINMON_MONCMPB_Msk      (0x20UL)   /*!< MONCMPB (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADWINMON_MONCMPA_Pos      (4UL)      /*!< MONCMPA (Bit 4)                                       */
#define R_ADC0_ADWINMON_MONCMPA_Msk      (0x10UL)   /*!< MONCMPA (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADWINMON_MONCOMB_Pos      (0UL)      /*!< MONCOMB (Bit 0)                                       */
#define R_ADC0_ADWINMON_MONCOMB_Msk      (0x1UL)    /*!< MONCOMB (Bitfield-Mask: 0x01)                         */
/* ========================================================  ADCMPCR  ======================================================== */
#define R_ADC0_ADCMPCR_CMPAIE_Pos        (15UL)     /*!< CMPAIE (Bit 15)                                       */
#define R_ADC0_ADCMPCR_CMPAIE_Msk        (0x8000UL) /*!< CMPAIE (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADCMPCR_WCMPE_Pos         (14UL)     /*!< WCMPE (Bit 14)                                        */
#define R_ADC0_ADCMPCR_WCMPE_Msk         (0x4000UL) /*!< WCMPE (Bitfield-Mask: 0x01)                           */
#define R_ADC0_ADCMPCR_CMPBIE_Pos        (13UL)     /*!< CMPBIE (Bit 13)                                       */
#define R_ADC0_ADCMPCR_CMPBIE_Msk        (0x2000UL) /*!< CMPBIE (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADCMPCR_CMPAE_Pos         (11UL)     /*!< CMPAE (Bit 11)                                        */
#define R_ADC0_ADCMPCR_CMPAE_Msk         (0x800UL)  /*!< CMPAE (Bitfield-Mask: 0x01)                           */
#define R_ADC0_ADCMPCR_CMPBE_Pos         (9UL)      /*!< CMPBE (Bit 9)                                         */
#define R_ADC0_ADCMPCR_CMPBE_Msk         (0x200UL)  /*!< CMPBE (Bitfield-Mask: 0x01)                           */
#define R_ADC0_ADCMPCR_CMPAB_Pos         (0UL)      /*!< CMPAB (Bit 0)                                         */
#define R_ADC0_ADCMPCR_CMPAB_Msk         (0x3UL)    /*!< CMPAB (Bitfield-Mask: 0x03)                           */
/* ======================================================  ADCMPANSER  ======================================================= */
#define R_ADC0_ADCMPANSER_CMPSOC_Pos     (1UL)      /*!< CMPSOC (Bit 1)                                        */
#define R_ADC0_ADCMPANSER_CMPSOC_Msk     (0x2UL)    /*!< CMPSOC (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADCMPANSER_CMPSTS_Pos     (0UL)      /*!< CMPSTS (Bit 0)                                        */
#define R_ADC0_ADCMPANSER_CMPSTS_Msk     (0x1UL)    /*!< CMPSTS (Bitfield-Mask: 0x01)                          */
/* =======================================================  ADCMPLER  ======================================================== */
#define R_ADC0_ADCMPLER_CMPLOC_Pos       (1UL)      /*!< CMPLOC (Bit 1)                                        */
#define R_ADC0_ADCMPLER_CMPLOC_Msk       (0x2UL)    /*!< CMPLOC (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADCMPLER_CMPLTS_Pos       (0UL)      /*!< CMPLTS (Bit 0)                                        */
#define R_ADC0_ADCMPLER_CMPLTS_Msk       (0x1UL)    /*!< CMPLTS (Bitfield-Mask: 0x01)                          */
/* =======================================================  ADCMPANSR  ======================================================= */
#define R_ADC0_ADCMPANSR_CMPCHA15_Pos    (15UL)     /*!< CMPCHA15 (Bit 15)                                     */
#define R_ADC0_ADCMPANSR_CMPCHA15_Msk    (0x8000UL) /*!< CMPCHA15 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPANSR_CMPCHA14_Pos    (14UL)     /*!< CMPCHA14 (Bit 14)                                     */
#define R_ADC0_ADCMPANSR_CMPCHA14_Msk    (0x4000UL) /*!< CMPCHA14 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPANSR_CMPCHA13_Pos    (13UL)     /*!< CMPCHA13 (Bit 13)                                     */
#define R_ADC0_ADCMPANSR_CMPCHA13_Msk    (0x2000UL) /*!< CMPCHA13 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPANSR_CMPCHA12_Pos    (12UL)     /*!< CMPCHA12 (Bit 12)                                     */
#define R_ADC0_ADCMPANSR_CMPCHA12_Msk    (0x1000UL) /*!< CMPCHA12 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPANSR_CMPCHA11_Pos    (11UL)     /*!< CMPCHA11 (Bit 11)                                     */
#define R_ADC0_ADCMPANSR_CMPCHA11_Msk    (0x800UL)  /*!< CMPCHA11 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPANSR_CMPCHA10_Pos    (10UL)     /*!< CMPCHA10 (Bit 10)                                     */
#define R_ADC0_ADCMPANSR_CMPCHA10_Msk    (0x400UL)  /*!< CMPCHA10 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPANSR_CMPCHA9_Pos     (9UL)      /*!< CMPCHA9 (Bit 9)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA9_Msk     (0x200UL)  /*!< CMPCHA9 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADCMPANSR_CMPCHA8_Pos     (8UL)      /*!< CMPCHA8 (Bit 8)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA8_Msk     (0x100UL)  /*!< CMPCHA8 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADCMPANSR_CMPCHA7_Pos     (7UL)      /*!< CMPCHA7 (Bit 7)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA7_Msk     (0x80UL)   /*!< CMPCHA7 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADCMPANSR_CMPCHA6_Pos     (6UL)      /*!< CMPCHA6 (Bit 6)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA6_Msk     (0x40UL)   /*!< CMPCHA6 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADCMPANSR_CMPCHA5_Pos     (5UL)      /*!< CMPCHA5 (Bit 5)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA5_Msk     (0x20UL)   /*!< CMPCHA5 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADCMPANSR_CMPCHA4_Pos     (4UL)      /*!< CMPCHA4 (Bit 4)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA4_Msk     (0x10UL)   /*!< CMPCHA4 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADCMPANSR_CMPCHA3_Pos     (3UL)      /*!< CMPCHA3 (Bit 3)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA3_Msk     (0x8UL)    /*!< CMPCHA3 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADCMPANSR_CMPCHA2_Pos     (2UL)      /*!< CMPCHA2 (Bit 2)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA2_Msk     (0x4UL)    /*!< CMPCHA2 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADCMPANSR_CMPCHA1_Pos     (1UL)      /*!< CMPCHA1 (Bit 1)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA1_Msk     (0x2UL)    /*!< CMPCHA1 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADCMPANSR_CMPCHA0_Pos     (0UL)      /*!< CMPCHA0 (Bit 0)                                       */
#define R_ADC0_ADCMPANSR_CMPCHA0_Msk     (0x1UL)    /*!< CMPCHA0 (Bitfield-Mask: 0x01)                         */
/* ========================================================  ADCMPLR  ======================================================== */
#define R_ADC0_ADCMPLR_CMPLCHA15_Pos     (15UL)     /*!< CMPLCHA15 (Bit 15)                                    */
#define R_ADC0_ADCMPLR_CMPLCHA15_Msk     (0x8000UL) /*!< CMPLCHA15 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPLR_CMPLCHA14_Pos     (14UL)     /*!< CMPLCHA14 (Bit 14)                                    */
#define R_ADC0_ADCMPLR_CMPLCHA14_Msk     (0x4000UL) /*!< CMPLCHA14 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPLR_CMPLCHA13_Pos     (13UL)     /*!< CMPLCHA13 (Bit 13)                                    */
#define R_ADC0_ADCMPLR_CMPLCHA13_Msk     (0x2000UL) /*!< CMPLCHA13 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPLR_CMPLCHA12_Pos     (12UL)     /*!< CMPLCHA12 (Bit 12)                                    */
#define R_ADC0_ADCMPLR_CMPLCHA12_Msk     (0x1000UL) /*!< CMPLCHA12 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPLR_CMPLCHA11_Pos     (11UL)     /*!< CMPLCHA11 (Bit 11)                                    */
#define R_ADC0_ADCMPLR_CMPLCHA11_Msk     (0x800UL)  /*!< CMPLCHA11 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPLR_CMPLCHA10_Pos     (10UL)     /*!< CMPLCHA10 (Bit 10)                                    */
#define R_ADC0_ADCMPLR_CMPLCHA10_Msk     (0x400UL)  /*!< CMPLCHA10 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPLR_CMPLCHA9_Pos      (9UL)      /*!< CMPLCHA9 (Bit 9)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA9_Msk      (0x200UL)  /*!< CMPLCHA9 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPLR_CMPLCHA8_Pos      (8UL)      /*!< CMPLCHA8 (Bit 8)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA8_Msk      (0x100UL)  /*!< CMPLCHA8 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPLR_CMPLCHA7_Pos      (7UL)      /*!< CMPLCHA7 (Bit 7)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA7_Msk      (0x80UL)   /*!< CMPLCHA7 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPLR_CMPLCHA6_Pos      (6UL)      /*!< CMPLCHA6 (Bit 6)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA6_Msk      (0x40UL)   /*!< CMPLCHA6 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPLR_CMPLCHA5_Pos      (5UL)      /*!< CMPLCHA5 (Bit 5)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA5_Msk      (0x20UL)   /*!< CMPLCHA5 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPLR_CMPLCHA4_Pos      (4UL)      /*!< CMPLCHA4 (Bit 4)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA4_Msk      (0x10UL)   /*!< CMPLCHA4 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPLR_CMPLCHA3_Pos      (3UL)      /*!< CMPLCHA3 (Bit 3)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA3_Msk      (0x8UL)    /*!< CMPLCHA3 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPLR_CMPLCHA2_Pos      (2UL)      /*!< CMPLCHA2 (Bit 2)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA2_Msk      (0x4UL)    /*!< CMPLCHA2 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPLR_CMPLCHA1_Pos      (1UL)      /*!< CMPLCHA1 (Bit 1)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA1_Msk      (0x2UL)    /*!< CMPLCHA1 (Bitfield-Mask: 0x01)                        */
#define R_ADC0_ADCMPLR_CMPLCHA0_Pos      (0UL)      /*!< CMPLCHA0 (Bit 0)                                      */
#define R_ADC0_ADCMPLR_CMPLCHA0_Msk      (0x1UL)    /*!< CMPLCHA0 (Bitfield-Mask: 0x01)                        */
/* =======================================================  ADCMPDR0  ======================================================== */
#define R_ADC0_ADCMPDR0_ADCMPDR0_Pos     (0UL)      /*!< ADCMPDR0 (Bit 0)                                      */
#define R_ADC0_ADCMPDR0_ADCMPDR0_Msk     (0xffffUL) /*!< ADCMPDR0 (Bitfield-Mask: 0xffff)                      */
/* =======================================================  ADCMPDR1  ======================================================== */
#define R_ADC0_ADCMPDR1_ADCMPDR1_Pos     (0UL)      /*!< ADCMPDR1 (Bit 0)                                      */
#define R_ADC0_ADCMPDR1_ADCMPDR1_Msk     (0xffffUL) /*!< ADCMPDR1 (Bitfield-Mask: 0xffff)                      */
/* ========================================================  ADCMPSR  ======================================================== */
#define R_ADC0_ADCMPSR_CMPSTCHA15_Pos    (15UL)     /*!< CMPSTCHA15 (Bit 15)                                   */
#define R_ADC0_ADCMPSR_CMPSTCHA15_Msk    (0x8000UL) /*!< CMPSTCHA15 (Bitfield-Mask: 0x01)                      */
#define R_ADC0_ADCMPSR_CMPSTCHA14_Pos    (14UL)     /*!< CMPSTCHA14 (Bit 14)                                   */
#define R_ADC0_ADCMPSR_CMPSTCHA14_Msk    (0x4000UL) /*!< CMPSTCHA14 (Bitfield-Mask: 0x01)                      */
#define R_ADC0_ADCMPSR_CMPSTCHA13_Pos    (13UL)     /*!< CMPSTCHA13 (Bit 13)                                   */
#define R_ADC0_ADCMPSR_CMPSTCHA13_Msk    (0x2000UL) /*!< CMPSTCHA13 (Bitfield-Mask: 0x01)                      */
#define R_ADC0_ADCMPSR_CMPSTCHA12_Pos    (12UL)     /*!< CMPSTCHA12 (Bit 12)                                   */
#define R_ADC0_ADCMPSR_CMPSTCHA12_Msk    (0x1000UL) /*!< CMPSTCHA12 (Bitfield-Mask: 0x01)                      */
#define R_ADC0_ADCMPSR_CMPSTCHA11_Pos    (11UL)     /*!< CMPSTCHA11 (Bit 11)                                   */
#define R_ADC0_ADCMPSR_CMPSTCHA11_Msk    (0x800UL)  /*!< CMPSTCHA11 (Bitfield-Mask: 0x01)                      */
#define R_ADC0_ADCMPSR_CMPSTCHA10_Pos    (10UL)     /*!< CMPSTCHA10 (Bit 10)                                   */
#define R_ADC0_ADCMPSR_CMPSTCHA10_Msk    (0x400UL)  /*!< CMPSTCHA10 (Bitfield-Mask: 0x01)                      */
#define R_ADC0_ADCMPSR_CMPSTCHA9_Pos     (9UL)      /*!< CMPSTCHA9 (Bit 9)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA9_Msk     (0x200UL)  /*!< CMPSTCHA9 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPSR_CMPSTCHA8_Pos     (8UL)      /*!< CMPSTCHA8 (Bit 8)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA8_Msk     (0x100UL)  /*!< CMPSTCHA8 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPSR_CMPSTCHA7_Pos     (7UL)      /*!< CMPSTCHA7 (Bit 7)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA7_Msk     (0x80UL)   /*!< CMPSTCHA7 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPSR_CMPSTCHA6_Pos     (6UL)      /*!< CMPSTCHA6 (Bit 6)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA6_Msk     (0x40UL)   /*!< CMPSTCHA6 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPSR_CMPSTCHA5_Pos     (5UL)      /*!< CMPSTCHA5 (Bit 5)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA5_Msk     (0x20UL)   /*!< CMPSTCHA5 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPSR_CMPSTCHA4_Pos     (4UL)      /*!< CMPSTCHA4 (Bit 4)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA4_Msk     (0x10UL)   /*!< CMPSTCHA4 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPSR_CMPSTCHA3_Pos     (3UL)      /*!< CMPSTCHA3 (Bit 3)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA3_Msk     (0x8UL)    /*!< CMPSTCHA3 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPSR_CMPSTCHA2_Pos     (2UL)      /*!< CMPSTCHA2 (Bit 2)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA2_Msk     (0x4UL)    /*!< CMPSTCHA2 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPSR_CMPSTCHA1_Pos     (1UL)      /*!< CMPSTCHA1 (Bit 1)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA1_Msk     (0x2UL)    /*!< CMPSTCHA1 (Bitfield-Mask: 0x01)                       */
#define R_ADC0_ADCMPSR_CMPSTCHA0_Pos     (0UL)      /*!< CMPSTCHA0 (Bit 0)                                     */
#define R_ADC0_ADCMPSR_CMPSTCHA0_Msk     (0x1UL)    /*!< CMPSTCHA0 (Bitfield-Mask: 0x01)                       */
/* =======================================================  ADCMPSER  ======================================================== */
#define R_ADC0_ADCMPSER_CMPFOC_Pos       (1UL)      /*!< CMPFOC (Bit 1)                                        */
#define R_ADC0_ADCMPSER_CMPFOC_Msk       (0x2UL)    /*!< CMPFOC (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADCMPSER_CMPFTS_Pos       (0UL)      /*!< CMPFTS (Bit 0)                                        */
#define R_ADC0_ADCMPSER_CMPFTS_Msk       (0x1UL)    /*!< CMPFTS (Bitfield-Mask: 0x01)                          */
/* =======================================================  ADCMPBNSR  ======================================================= */
#define R_ADC0_ADCMPBNSR_CMPLB_Pos       (7UL)      /*!< CMPLB (Bit 7)                                         */
#define R_ADC0_ADCMPBNSR_CMPLB_Msk       (0x80UL)   /*!< CMPLB (Bitfield-Mask: 0x01)                           */
#define R_ADC0_ADCMPBNSR_CMPCHB_Pos      (0UL)      /*!< CMPCHB (Bit 0)                                        */
#define R_ADC0_ADCMPBNSR_CMPCHB_Msk      (0x3fUL)   /*!< CMPCHB (Bitfield-Mask: 0x3f)                          */
/* =======================================================  ADWINLLB  ======================================================== */
#define R_ADC0_ADWINLLB_ADWINLLB_Pos     (0UL)      /*!< ADWINLLB (Bit 0)                                      */
#define R_ADC0_ADWINLLB_ADWINLLB_Msk     (0xffffUL) /*!< ADWINLLB (Bitfield-Mask: 0xffff)                      */
/* =======================================================  ADWINULB  ======================================================== */
#define R_ADC0_ADWINULB_ADWINULB_Pos     (0UL)      /*!< ADWINULB (Bit 0)                                      */
#define R_ADC0_ADWINULB_ADWINULB_Msk     (0xffffUL) /*!< ADWINULB (Bitfield-Mask: 0xffff)                      */
/* =======================================================  ADCMPBSR  ======================================================== */
#define R_ADC0_ADCMPBSR_CMPSTB_Pos       (0UL)      /*!< CMPSTB (Bit 0)                                        */
#define R_ADC0_ADCMPBSR_CMPSTB_Msk       (0x1UL)    /*!< CMPSTB (Bitfield-Mask: 0x01)                          */
/* ========================================================  ADANSC  ========================================================= */
#define R_ADC0_ADANSC_ANSC015_Pos        (15UL)     /*!< ANSC015 (Bit 15)                                      */
#define R_ADC0_ADANSC_ANSC015_Msk        (0x8000UL) /*!< ANSC015 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC014_Pos        (14UL)     /*!< ANSC014 (Bit 14)                                      */
#define R_ADC0_ADANSC_ANSC014_Msk        (0x4000UL) /*!< ANSC014 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC013_Pos        (13UL)     /*!< ANSC013 (Bit 13)                                      */
#define R_ADC0_ADANSC_ANSC013_Msk        (0x2000UL) /*!< ANSC013 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC012_Pos        (12UL)     /*!< ANSC012 (Bit 12)                                      */
#define R_ADC0_ADANSC_ANSC012_Msk        (0x1000UL) /*!< ANSC012 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC011_Pos        (11UL)     /*!< ANSC011 (Bit 11)                                      */
#define R_ADC0_ADANSC_ANSC011_Msk        (0x800UL)  /*!< ANSC011 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC010_Pos        (10UL)     /*!< ANSC010 (Bit 10)                                      */
#define R_ADC0_ADANSC_ANSC010_Msk        (0x400UL)  /*!< ANSC010 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC009_Pos        (9UL)      /*!< ANSC009 (Bit 9)                                       */
#define R_ADC0_ADANSC_ANSC009_Msk        (0x200UL)  /*!< ANSC009 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC008_Pos        (8UL)      /*!< ANSC008 (Bit 8)                                       */
#define R_ADC0_ADANSC_ANSC008_Msk        (0x100UL)  /*!< ANSC008 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC007_Pos        (7UL)      /*!< ANSC007 (Bit 7)                                       */
#define R_ADC0_ADANSC_ANSC007_Msk        (0x80UL)   /*!< ANSC007 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC006_Pos        (6UL)      /*!< ANSC006 (Bit 6)                                       */
#define R_ADC0_ADANSC_ANSC006_Msk        (0x40UL)   /*!< ANSC006 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC005_Pos        (5UL)      /*!< ANSC005 (Bit 5)                                       */
#define R_ADC0_ADANSC_ANSC005_Msk        (0x20UL)   /*!< ANSC005 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC004_Pos        (4UL)      /*!< ANSC004 (Bit 4)                                       */
#define R_ADC0_ADANSC_ANSC004_Msk        (0x10UL)   /*!< ANSC004 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC003_Pos        (3UL)      /*!< ANSC003 (Bit 3)                                       */
#define R_ADC0_ADANSC_ANSC003_Msk        (0x8UL)    /*!< ANSC003 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC002_Pos        (2UL)      /*!< ANSC002 (Bit 2)                                       */
#define R_ADC0_ADANSC_ANSC002_Msk        (0x4UL)    /*!< ANSC002 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC001_Pos        (1UL)      /*!< ANSC001 (Bit 1)                                       */
#define R_ADC0_ADANSC_ANSC001_Msk        (0x2UL)    /*!< ANSC001 (Bitfield-Mask: 0x01)                         */
#define R_ADC0_ADANSC_ANSC000_Pos        (0UL)      /*!< ANSC000 (Bit 0)                                       */
#define R_ADC0_ADANSC_ANSC000_Msk        (0x1UL)    /*!< ANSC000 (Bitfield-Mask: 0x01)                         */
/* =======================================================  ADGCEXCR  ======================================================== */
#define R_ADC0_ADGCEXCR_OCSC_Pos         (1UL)      /*!< OCSC (Bit 1)                                          */
#define R_ADC0_ADGCEXCR_OCSC_Msk         (0x2UL)    /*!< OCSC (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADGCEXCR_TSSC_Pos         (0UL)      /*!< TSSC (Bit 0)                                          */
#define R_ADC0_ADGCEXCR_TSSC_Msk         (0x1UL)    /*!< TSSC (Bitfield-Mask: 0x01)                            */
/* =======================================================  ADGCTRGR  ======================================================== */
#define R_ADC0_ADGCTRGR_GRCE_Pos         (7UL)      /*!< GRCE (Bit 7)                                          */
#define R_ADC0_ADGCTRGR_GRCE_Msk         (0x80UL)   /*!< GRCE (Bitfield-Mask: 0x01)                            */
#define R_ADC0_ADGCTRGR_GCADIE_Pos       (6UL)      /*!< GCADIE (Bit 6)                                        */
#define R_ADC0_ADGCTRGR_GCADIE_Msk       (0x40UL)   /*!< GCADIE (Bitfield-Mask: 0x01)                          */
#define R_ADC0_ADGCTRGR_TRSC_Pos         (0UL)      /*!< TRSC (Bit 0)                                          */
#define R_ADC0_ADGCTRGR_TRSC_Msk         (0x3fUL)   /*!< TRSC (Bitfield-Mask: 0x3f)                            */
/* ========================================================  ADSSTRL  ======================================================== */
#define R_ADC0_ADSSTRL_SST_Pos           (0UL)      /*!< SST (Bit 0)                                           */
#define R_ADC0_ADSSTRL_SST_Msk           (0xffUL)   /*!< SST (Bitfield-Mask: 0xff)                             */
/* ========================================================  ADSSTRT  ======================================================== */
#define R_ADC0_ADSSTRT_SST_Pos           (0UL)      /*!< SST (Bit 0)                                           */
#define R_ADC0_ADSSTRT_SST_Msk           (0xffUL)   /*!< SST (Bitfield-Mask: 0xff)                             */
/* ========================================================  ADSSTRO  ======================================================== */
#define R_ADC0_ADSSTRO_SST_Pos           (0UL)      /*!< SST (Bit 0)                                           */
#define R_ADC0_ADSSTRO_SST_Msk           (0xffUL)   /*!< SST (Bitfield-Mask: 0xff)                             */
/* ========================================================  ADSSTR  ========================================================= */
#define R_ADC0_ADSSTR_SST_Pos            (0UL)      /*!< SST (Bit 0)                                           */
#define R_ADC0_ADSSTR_SST_Msk            (0xffUL)   /*!< SST (Bitfield-Mask: 0xff)                             */

#endif /* R_ADC_POSMSK_H */

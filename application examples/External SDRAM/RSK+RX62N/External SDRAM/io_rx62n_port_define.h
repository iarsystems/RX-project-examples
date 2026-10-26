/**
 * \file  io_rx62n_port_define.h
 * \brief Header file to include the correct cpu io define file
 *
 * This file includes the correct cpu specific io port definations
 *
 * \internal Author, version, and date are provided by SVN.  Do not edit \endinternal
 * $Author$
 *          
 * $Date$
 *
 * (C)2011 INOVA Geophysical Equipment
 * All Rights Reserved
 *
 */

//-------- SYSTEM.SYSCR0 -------- 16bit
#define SYSCR0_ROME_bp 0u
#define SYSCR0_ROME_bm 0x1
#define SYSCR0_EXBE_bp 1u
#define SYSCR0_EXBE_bm 0x1
#define SYSCR0_KEY_bp  8u
#define SYSCR0_KEY_bm  0xFF

#define ROME_ONCHIP_ROM_DISABLED    ((0x0 & SYSCR0_ROME_bm) << SYSCR0_ROME_bp)
#define ROME_ONCHIP_ROM_ENABLED     ((0x1 & SYSCR0_ROME_bm) << SYSCR0_ROME_bp)
#define EXBE_EXTBUS_DISABLED        ((0x0 & SYSCR0_EXBE_bm) << SYSCR0_EXBE_bp)
#define EXBE_EXTBUS_ENABLED         ((0x1 & SYSCR0_EXBE_bm) << SYSCR0_EXBE_bp)
#define SYSCRO_KEY_CODE             ((0x5A & SYSCR0_KEY_bm) << SYSCR0_KEY_bp)


//-------- SYSTEM.SYSCR1 -------- 16bit
#define SYSCR1_RAME_bp 0u
#define SYSCR1_RAME_bm 0x1

#define RAME_ONCHIP_RAM_DISABLED    ((0x0 & SYSCR1_RAME_bm) << SYSCR1_RAME_bp)
#define RAME_ONCHIP_RAM_ENABLED     ((0x1 & SYSCR1_RAME_bm) << SYSCR1_RAME_bp)


//-------- SYSTEM.SCKCR -------- 32bit
#define SCKCR_PCK_bp    8u
#define SCKCR_PCK_bm    0xF
#define SCKCR_BCK_bp    16u
#define SCKCR_BCK_bm    0xF
#define SCKCR_PSTOP0_bp 22u
#define SCKCR_PSTOP0_bm 0x1
#define SCKCR_PSTOP1_bp 23u
#define SCKCR_PSTOP1_bm 0x1
#define SCKCR_ICK_bp    24u
#define SCKCR_ICK_bm    0xF

#define PCK_EXTAL_8     ((0x0 & SCKCR_PCK_bm) << SCKCR_PCK_bp)
#define PCK_EXTAL_4     ((0x1 & SCKCR_PCK_bm) << SCKCR_PCK_bp)
#define PCK_EXTAL_2     ((0x2 & SCKCR_PCK_bm) << SCKCR_PCK_bp)
#define PCK_EXTAL_1     ((0x3 & SCKCR_PCK_bm) << SCKCR_PCK_bp)

#define BCK_EXTAL_8     ((0x0 & SCKCR_BCK_bm) << SCKCR_BCK_bp)
#define BCK_EXTAL_4     ((0x1 & SCKCR_BCK_bm) << SCKCR_BCK_bp)
#define BCK_EXTAL_2     ((0x2 & SCKCR_BCK_bm) << SCKCR_BCK_bp)
#define BCK_EXTAL_1     ((0x3 & SCKCR_BCK_bm) << SCKCR_BCK_bp)

#define PSTOP0_SDCLK_OUTPUT_ENABLE      ((0x0 & SCKCR_PSTOP0_bm) << SCKCR_PSTOP0_bp)
#define PSTOP0_SDCLK_OUTPUT_DISABLE     ((0x1 & SCKCR_PSTOP0_bm) << SCKCR_PSTOP0_bp)

#define PSTOP1_BCLK_OUTPUT_ENABLE     ((0x0 & SCKCR_PSTOP1_bm) << SCKCR_PSTOP1_bp)
#define PSTOP1_BCLK_OUTPUT_DISABLE    ((0x1 & SCKCR_PSTOP1_bm) << SCKCR_PSTOP1_bp)

#define ICK_EXTAL_8     ((0x0 & SCKCR_ICK_bm) << SCKCR_ICK_bp)
#define ICK_EXTAL_4     ((0x1 & SCKCR_ICK_bm) << SCKCR_ICK_bp)
#define ICK_EXTAL_2     ((0x2 & SCKCR_ICK_bm) << SCKCR_ICK_bp)
#define ICK_EXTAL_1     ((0x3 & SCKCR_ICK_bm) << SCKCR_ICK_bp)


//-------- SYSTEM.BCKCR -------- 8bit
#define BCKCR_BCLKDIV_bp 0u
#define BCKCR_BCLKDIV_bm 0x1

#define BCLKDIV_1    ((0x0 & BCKCR_BCLKDIV_bm) << BCKCR_BCLKDIV_bp)
#define BCLKDIV_2    ((0x1 & BCKCR_BCLKDIV_bm) << BCKCR_BCLKDIV_bp)


//-------- SYSTEM.OSTDCR -------- 16bit
#define OSTDCR_OSTDF_bp 6u
#define OSTDCR_OSTDF_bm 0x1
#define OSTDCR_OSTDE_bp 7u
#define OSTDCR_OSTDE_bm 0x1
#define OSTDCR_KEY_bp   8u
#define OSTDCR_KEY_bm   0xFF

#define MAIN_OSC_ENABLE                   ((0x0 & OSTDCR_OSTDF_bm) << OSTDCR_OSTDF_bp)
#define MAIN_OSC_STOPPED                  ((0x1 & OSTDCR_OSTDF_bm) << OSTDCR_OSTDF_bp)
#define MAIN_OSC_DETECT_STOP_DISABLED     ((0x0 & OSTDCR_OSTDE_bm) << OSTDCR_OSTDE_bp)
#define MAIN_OSC_DETECT_STOP_ENABLED      ((0x1 & OSTDCR_OSTDE_bm) << OSTDCR_OSTDE_bp)
#define OSTDCR_KEY_CODE                   ((0xAC & OSTDCR_KEY_bm) << OSTDCR_KEY_bp)


//-------- SYSTEM.SUBOSCCR -------- 8bit
#define SUBOSCCR_SUBSTOP_bp 0u
#define SUBOSCCR_SUBSTOP_bm 0x1

#define SUBSTOP_SUBCLOCK_ENABLED    ((0x0 & SUBOSCCR_SUBSTOP_bm) << SUBOSCCR_SUBSTOP_bp)
#define SUBSTOP_SUBCLOCK_STOPPED    ((0x1 & SUBOSCCR_SUBSTOP_bm) << SUBOSCCR_SUBSTOP_bp)


//-------- BSC.CSnCR -------- 16bit
#define CSnCR_EXENB_bp  0u
#define CSnCR_EXENB_bm  0x1
#define CSnCR_BSIZE_bp  4u
#define CSnCR_BSIZE_bm  0x3
#define CSnCR_EMODE_bp  8u
#define CSnCR_EMODE_bm  0x1

#define EXENB_CS_DISABLED ((0x0 & CSnCR_EXENB_bm) << CSnCR_EXENB_bp)
#define EXENB_CS_ENABLED  ((0x1 & CSnCR_EXENB_bm) << CSnCR_EXENB_bp)
#define BSIZE_EXTERNAL_BUS_WIDTH_16   ((0x0 & CSnCR_BSIZE_bm) << CSnCR_BSIZE_bp)
#define BSIZE_EXTERNAL_BUS_WIDTH_32   ((0x1 & CSnCR_BSIZE_bm) << CSnCR_BSIZE_bp)
#define BSIZE_EXTERNAL_BUS_WIDTH_8    ((0x2 & CSnCR_BSIZE_bm) << CSnCR_BSIZE_bp)
#define EMODE_ENDIAN_SAME             ((0x0 & CSnCR_EMODE_bm) << CSnCR_EMODE_bp)
#define EMODE_ENDIAN_DIFFERENT        ((0x1 & CSnCR_EMODE_bm) << CSnCR_EMODE_bp)


//-------- BSC.CSnREC -------- 16bit
#define CSnREC_RRCV_bp  0u
#define CSnREC_RRCV_bm  0xF
#define CSnREC_WRCV_bp  8u
#define CSnREC_WRCV_bm  0xF

#define RRCV_READ_RECOVERY_0  ((0x0 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_1  ((0x1 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_2  ((0x2 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_3  ((0x3 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_4  ((0x4 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_5  ((0x5 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_6  ((0x6 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_7  ((0x7 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_8  ((0x8 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_9  ((0x9 & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_10 ((0xA & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_11 ((0xB & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_12 ((0xC & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_13 ((0xD & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_14 ((0xE & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)
#define RRCV_READ_RECOVERY_15 ((0xF & CSnREC_RRCV_bm) << CSnREC_RRCV_bp)

#define WRCV_WRITE_RECOVERY_0   ((0x0 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_1   ((0x1 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_2   ((0x2 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_3   ((0x3 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_4   ((0x4 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_5   ((0x5 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_6   ((0x6 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_7   ((0x7 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_8   ((0x8 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_9   ((0x9 & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_10  ((0xA & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_11  ((0xB & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_12  ((0xC & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_13  ((0xD & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_14  ((0xE & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)
#define WRCV_WRITE_RECOVERY_15  ((0xF & CSnREC_WRCV_bm) << CSnREC_WRCV_bp)

//-------- BSC.CSnMOD -------- 16bit
#define CSnMOD_WRMOD_bp 0u
#define CSnMOD_WRMOD_bm 0x1
#define CSnMOD_EWENB_bp 3u
#define CSnMOD_EWENB_bm 0x1
#define CSnMOD_PRENB_bp 8u
#define CSnMOD_PRENB_bm 0x1
#define CSnMOD_PWENB_bp 9u
#define CSnMOD_PWENB_bm 0x1
#define CSnMOD_PRMOD_bp 15u
#define CSnMOD_PRMOD_bm 0x1

#define WRMOD_BYTE_STROBE_MODE            ((0x0 & CSnMOD_WRMOD_bm) << CSnMOD_WRMOD_bp)
#define WRMOD_SINGLE_WRITE_STROBE_MODE    ((0x1 & CSnMOD_WRMOD_bm) << CSnMOD_WRMOD_bp)

#define EWENB_EXTERN_WAIT_DISABLED   ((0x0 & CSnMOD_EWENB_bm) << CSnMOD_EWENB_bp)
#define EWENB_EXTERN_WAIT_ENABLED    ((0x1 & CSnMOD_EWENB_bm) << CSnMOD_EWENB_bp)

#define PRENB_PAGE_READ_DISABLED   ((0x0 & CSnMOD_PRENB_bm) << CSnMOD_PRENB_bp)
#define PRENB_PAGE_READ_ENABLED    ((0x1 & CSnMOD_PRENB_bm) << CSnMOD_PRENB_bp)

#define PWENB_PAGE_WRITE_DISABLED  ((0x0 & CSnMOD_PWENB_bm) << CSnMOD_PWENB_bp)
#define PWENB_PAGE_WRITE_ENABLED   ((0x1 & CSnMOD_PWENB_bm) << CSnMOD_PWENB_bp)

#define PRMOD_READ_ACCESS_NORMAL            ((0x0 & CSnMOD_PRMOD_bm) << CSnMOD_PRMOD_bp)
#define PRMOD_EXTERN_DATA_READ_CONTINUOUS   ((0x1 & CSnMOD_PRMOD_bm) << CSnMOD_PRMOD_bp)

//-------- BSC.CSnWCR1 -------- 32bit (0-7)
#define CSnWCR1_CSPWWAIT_bp   0u
#define CSnWCR1_CSPWWAIT_bm   0x7
#define CSnWCR1_CSPRWAIT_bp   8u
#define CSnWCR1_CSPRWAIT_bm   0x7
#define CSnWCR1_CSWWAIT_bp    16u
#define CSnWCR1_CSWWAIT_bm    0x1F
#define CSnWCR1_CSRWAIT_bp    24u
#define CSnWCR1_CSRWAIT_bm    0x1F

#define CSPWWAIT_PAGE_WRITE_WAIT_0 ((0x0 & CSnWCR1_CSPWWAIT_bm) << CSnWCR1_CSPWWAIT_bp)
#define CSPWWAIT_PAGE_WRITE_WAIT_1 ((0x1 & CSnWCR1_CSPWWAIT_bm) << CSnWCR1_CSPWWAIT_bp)
#define CSPWWAIT_PAGE_WRITE_WAIT_2 ((0x2 & CSnWCR1_CSPWWAIT_bm) << CSnWCR1_CSPWWAIT_bp)
#define CSPWWAIT_PAGE_WRITE_WAIT_3 ((0x3 & CSnWCR1_CSPWWAIT_bm) << CSnWCR1_CSPWWAIT_bp)
#define CSPWWAIT_PAGE_WRITE_WAIT_4 ((0x4 & CSnWCR1_CSPWWAIT_bm) << CSnWCR1_CSPWWAIT_bp)
#define CSPWWAIT_PAGE_WRITE_WAIT_5 ((0x5 & CSnWCR1_CSPWWAIT_bm) << CSnWCR1_CSPWWAIT_bp)
#define CSPWWAIT_PAGE_WRITE_WAIT_6 ((0x6 & CSnWCR1_CSPWWAIT_bm) << CSnWCR1_CSPWWAIT_bp)
#define CSPWWAIT_PAGE_WRITE_WAIT_7 ((0x7 & CSnWCR1_CSPWWAIT_bm) << CSnWCR1_CSPWWAIT_bp)

#define CSPRWAIT_PAGE_READ_WAIT_0 ((0x0 & CSnWCR1_CSPRWAIT_bm) << CSnWCR1_CSPRWAIT_bp)
#define CSPRWAIT_PAGE_READ_WAIT_1 ((0x1 & CSnWCR1_CSPRWAIT_bm) << CSnWCR1_CSPRWAIT_bp)
#define CSPRWAIT_PAGE_READ_WAIT_2 ((0x2 & CSnWCR1_CSPRWAIT_bm) << CSnWCR1_CSPRWAIT_bp)
#define CSPRWAIT_PAGE_READ_WAIT_3 ((0x3 & CSnWCR1_CSPRWAIT_bm) << CSnWCR1_CSPRWAIT_bp)
#define CSPRWAIT_PAGE_READ_WAIT_4 ((0x4 & CSnWCR1_CSPRWAIT_bm) << CSnWCR1_CSPRWAIT_bp)
#define CSPRWAIT_PAGE_READ_WAIT_5 ((0x5 & CSnWCR1_CSPRWAIT_bm) << CSnWCR1_CSPRWAIT_bp)
#define CSPRWAIT_PAGE_READ_WAIT_6 ((0x6 & CSnWCR1_CSPRWAIT_bm) << CSnWCR1_CSPRWAIT_bp)
#define CSPRWAIT_PAGE_READ_WAIT_7 ((0x7 & CSnWCR1_CSPRWAIT_bm) << CSnWCR1_CSPRWAIT_bp)

#define CSWWAIT_WRITE_WAIT_0  ((0x0 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_1  ((0x1 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_2  ((0x2 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_3  ((0x3 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_4  ((0x4 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_5  ((0x5 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_6  ((0x6 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_7  ((0x7 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_8  ((0x8 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_9  ((0x9 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_10  ((0xA & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_11  ((0xB & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_12  ((0xC & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_13  ((0xD & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_14  ((0xE & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_15  ((0xF & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_16  ((0x10 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_17  ((0x11 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_18  ((0x12 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_19  ((0x13 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_20  ((0x14 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_21  ((0x15 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_22  ((0x16 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_23  ((0x17 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_24  ((0x18 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_25  ((0x19 & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_26  ((0x1A & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_27  ((0x1B & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_28  ((0x1C & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_29  ((0x1D & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_30  ((0x1E & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)
#define CSWWAIT_WRITE_WAIT_31  ((0x1F & CSnWCR1_CSWWAIT_bm) << CSnWCR1_CSWWAIT_bp)

#define CSRWAIT_READ_WAIT_0   ((0x0 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_1   ((0x1 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_2   ((0x2 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_3   ((0x3 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_4   ((0x4 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_5   ((0x5 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_6   ((0x6 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_7   ((0x7 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_8   ((0x8 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_9   ((0x9 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_10  ((0xA & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_11  ((0xB & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_12  ((0xC & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_13  ((0xD & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_14  ((0xE & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_15  ((0xF & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_16  ((0x10 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_17  ((0x11 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_18  ((0x12 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_19  ((0x13 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_20  ((0x14 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_21  ((0x15 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_22  ((0x16 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_23  ((0x17 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_24  ((0x18 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_25  ((0x19 & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_26  ((0x1A & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_27  ((0x1B & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_28  ((0x1C & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_29  ((0x1D & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_30  ((0x1E & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)
#define CSRWAIT_READ_WAIT_31  ((0x1F & CSnWCR1_CSRWAIT_bm) << CSnWCR1_CSRWAIT_bp)

//-------- BSC.CSnWCR2 -------- 32bit (0-7)
#define CSnWCR2_CSROFF_bp   0u
#define CSnWCR2_CSROFF_bm   0x7
#define CSnWCR2_CSWOFF_bp   4u
#define CSnWCR2_CSWOFF_bm   0x7
#define CSnWCR2_WDOFF_bp    8u
#define CSnWCR2_WDOFF_bm    0x7
#define CSnWCR2_RDON_bp    16u
#define CSnWCR2_RDON_bm    0x7
#define CSnWCR2_WRON_bp    20u
#define CSnWCR2_WRON_bm    0x7
#define CSnWCR2_WDON_bp    24u
#define CSnWCR2_WDON_bm    0x7
#define CSnWCR2_CSON_bp    28u
#define CSnWCR2_CSON_bm    0x7

#define CSROFF_READ_ACCESS_EXTEND_0 ((0x0 & CSnWCR2_CSROFF_bm) << CSnWCR2_CSROFF_bp)
#define CSROFF_READ_ACCESS_EXTEND_1 ((0x1 & CSnWCR2_CSROFF_bm) << CSnWCR2_CSROFF_bp)
#define CSROFF_READ_ACCESS_EXTEND_2 ((0x2 & CSnWCR2_CSROFF_bm) << CSnWCR2_CSROFF_bp)
#define CSROFF_READ_ACCESS_EXTEND_3 ((0x3 & CSnWCR2_CSROFF_bm) << CSnWCR2_CSROFF_bp)
#define CSROFF_READ_ACCESS_EXTEND_4 ((0x4 & CSnWCR2_CSROFF_bm) << CSnWCR2_CSROFF_bp)
#define CSROFF_READ_ACCESS_EXTEND_5 ((0x5 & CSnWCR2_CSROFF_bm) << CSnWCR2_CSROFF_bp)
#define CSROFF_READ_ACCESS_EXTEND_6 ((0x6 & CSnWCR2_CSROFF_bm) << CSnWCR2_CSROFF_bp)
#define CSROFF_READ_ACCESS_EXTEND_7 ((0x7 & CSnWCR2_CSROFF_bm) << CSnWCR2_CSROFF_bp)

#define CSWOFF_WRITE_ACCESS_EXTEND_0 ((0x0 & CSnWCR2_CSWOFF_bm) << CSnWCR2_CSWOFF_bp)
#define CSWOFF_WRITE_ACCESS_EXTEND_1 ((0x1 & CSnWCR2_CSWOFF_bm) << CSnWCR2_CSWOFF_bp)
#define CSWOFF_WRITE_ACCESS_EXTEND_2 ((0x2 & CSnWCR2_CSWOFF_bm) << CSnWCR2_CSWOFF_bp)
#define CSWOFF_WRITE_ACCESS_EXTEND_3 ((0x3 & CSnWCR2_CSWOFF_bm) << CSnWCR2_CSWOFF_bp)
#define CSWOFF_WRITE_ACCESS_EXTEND_4 ((0x4 & CSnWCR2_CSWOFF_bm) << CSnWCR2_CSWOFF_bp)
#define CSWOFF_WRITE_ACCESS_EXTEND_5 ((0x5 & CSnWCR2_CSWOFF_bm) << CSnWCR2_CSWOFF_bp)
#define CSWOFF_WRITE_ACCESS_EXTEND_6 ((0x6 & CSnWCR2_CSWOFF_bm) << CSnWCR2_CSWOFF_bp)
#define CSWOFF_WRITE_ACCESS_EXTEND_7 ((0x7 & CSnWCR2_CSWOFF_bm) << CSnWCR2_CSWOFF_bp)

#define WDOFF_WRITE_DATA_EXTEND_0 ((0x0 & CSnWCR2_WDOFF_bm) << CSnWCR2_WDOFF_bp)
#define WDOFF_WRITE_DATA_EXTEND_1 ((0x1 & CSnWCR2_WDOFF_bm) << CSnWCR2_WDOFF_bp)
#define WDOFF_WRITE_DATA_EXTEND_2 ((0x2 & CSnWCR2_WDOFF_bm) << CSnWCR2_WDOFF_bp)
#define WDOFF_WRITE_DATA_EXTEND_3 ((0x3 & CSnWCR2_WDOFF_bm) << CSnWCR2_WDOFF_bp)
#define WDOFF_WRITE_DATA_EXTEND_4 ((0x4 & CSnWCR2_WDOFF_bm) << CSnWCR2_WDOFF_bp)
#define WDOFF_WRITE_DATA_EXTEND_5 ((0x5 & CSnWCR2_WDOFF_bm) << CSnWCR2_WDOFF_bp)
#define WDOFF_WRITE_DATA_EXTEND_6 ((0x6 & CSnWCR2_WDOFF_bm) << CSnWCR2_WDOFF_bp)
#define WDOFF_WRITE_DATA_EXTEND_7 ((0x7 & CSnWCR2_WDOFF_bm) << CSnWCR2_WDOFF_bp)

#define RDON_RD_ASSERT_WAIT_0 ((0x0 & CSnWCR2_RDON_bm) << CSnWCR2_RDON_bp)
#define RDON_RD_ASSERT_WAIT_1 ((0x1 & CSnWCR2_RDON_bm) << CSnWCR2_RDON_bp)
#define RDON_RD_ASSERT_WAIT_2 ((0x2 & CSnWCR2_RDON_bm) << CSnWCR2_RDON_bp)
#define RDON_RD_ASSERT_WAIT_3 ((0x3 & CSnWCR2_RDON_bm) << CSnWCR2_RDON_bp)
#define RDON_RD_ASSERT_WAIT_4 ((0x4 & CSnWCR2_RDON_bm) << CSnWCR2_RDON_bp)
#define RDON_RD_ASSERT_WAIT_5 ((0x5 & CSnWCR2_RDON_bm) << CSnWCR2_RDON_bp)
#define RDON_RD_ASSERT_WAIT_6 ((0x6 & CSnWCR2_RDON_bm) << CSnWCR2_RDON_bp)
#define RDON_RD_ASSERT_WAIT_7 ((0x7 & CSnWCR2_RDON_bm) << CSnWCR2_RDON_bp)

#define WRON_WR_ASSERT_WAIT_0 ((0x0 & CSnWCR2_WRON_bm) << CSnWCR2_WRON_bp)
#define WRON_WR_ASSERT_WAIT_1 ((0x1 & CSnWCR2_WRON_bm) << CSnWCR2_WRON_bp)
#define WRON_WR_ASSERT_WAIT_2 ((0x2 & CSnWCR2_WRON_bm) << CSnWCR2_WRON_bp)
#define WRON_WR_ASSERT_WAIT_3 ((0x3 & CSnWCR2_WRON_bm) << CSnWCR2_WRON_bp)
#define WRON_WR_ASSERT_WAIT_4 ((0x4 & CSnWCR2_WRON_bm) << CSnWCR2_WRON_bp)
#define WRON_WR_ASSERT_WAIT_5 ((0x5 & CSnWCR2_WRON_bm) << CSnWCR2_WRON_bp)
#define WRON_WR_ASSERT_WAIT_6 ((0x6 & CSnWCR2_WRON_bm) << CSnWCR2_WRON_bp)
#define WRON_WR_ASSERT_WAIT_7 ((0x7 & CSnWCR2_WRON_bm) << CSnWCR2_WRON_bp)

#define WDON_WRITE_DATA_WAIT_0 ((0x0 & CSnWCR2_WDON_bm) << CSnWCR2_WDON_bp)
#define WDON_WRITE_DATA_WAIT_1 ((0x1 & CSnWCR2_WDON_bm) << CSnWCR2_WDON_bp)
#define WDON_WRITE_DATA_WAIT_2 ((0x2 & CSnWCR2_WDON_bm) << CSnWCR2_WDON_bp)
#define WDON_WRITE_DATA_WAIT_3 ((0x3 & CSnWCR2_WDON_bm) << CSnWCR2_WDON_bp)
#define WDON_WRITE_DATA_WAIT_4 ((0x4 & CSnWCR2_WDON_bm) << CSnWCR2_WDON_bp)
#define WDON_WRITE_DATA_WAIT_5 ((0x5 & CSnWCR2_WDON_bm) << CSnWCR2_WDON_bp)
#define WDON_WRITE_DATA_WAIT_6 ((0x6 & CSnWCR2_WDON_bm) << CSnWCR2_WDON_bp)
#define WDON_WRITE_DATA_WAIT_7 ((0x7 & CSnWCR2_WDON_bm) << CSnWCR2_WDON_bp)

#define CSON_CS_ASSERT_WAIT_0 ((0x0 & CSnWCR2_CSON_bm) << CSnWCR2_CSON_bp)
#define CSON_CS_ASSERT_WAIT_1 ((0x1 & CSnWCR2_CSON_bm) << CSnWCR2_CSON_bp)
#define CSON_CS_ASSERT_WAIT_2 ((0x2 & CSnWCR2_CSON_bm) << CSnWCR2_CSON_bp)
#define CSON_CS_ASSERT_WAIT_3 ((0x3 & CSnWCR2_CSON_bm) << CSnWCR2_CSON_bp)
#define CSON_CS_ASSERT_WAIT_4 ((0x4 & CSnWCR2_CSON_bm) << CSnWCR2_CSON_bp)
#define CSON_CS_ASSERT_WAIT_5 ((0x5 & CSnWCR2_CSON_bm) << CSnWCR2_CSON_bp)
#define CSON_CS_ASSERT_WAIT_6 ((0x6 & CSnWCR2_CSON_bm) << CSnWCR2_CSON_bp)
#define CSON_CS_ASSERT_WAIT_7 ((0x7 & CSnWCR2_CSON_bm) << CSnWCR2_CSON_bp)


//-------- BSC.SDCCR -------- 8 bit
#define SDCCR_EXENB_bp   0u
#define SDCCR_EXENB_bm   0x1
#define SDCCR_BSIZE_bp   4u
#define SDCCR_BSIZE_bm   0x3

#define EXENB_SDRAM_ADDRESS_DISABLE ((0x0 & SDCCR_EXENB_bm) << SDCCR_EXENB_bp)
#define EXENB_SDRAM_ADDRESS_ENABLE  ((0x1 & SDCCR_EXENB_bm) << SDCCR_EXENB_bp)

#define BSIZE_SDRAM_BUS_WIDTH_16  ((0x0 & SDCCR_EXENB_bm) << SDCCR_EXENB_bp)
#define BSIZE_SDRAM_BUS_WIDTH_32  ((0x1 & SDCCR_EXENB_bm) << SDCCR_EXENB_bp)
#define BSIZE_SDRAM_BUS_WIDTH_8   ((0x2 & SDCCR_EXENB_bm) << SDCCR_EXENB_bp)

//-------- PORTn.DDR -------- 8 bit
#define PIN0_bp 0u
#define PIN0_bm 0x1
#define PIN1_bp 1u
#define PIN1_bm 0x1
#define PIN2_bp 2u
#define PIN2_bm 0x1
#define PIN3_bp 3u
#define PIN3_bm 0x1
#define PIN4_bp 4u
#define PIN4_bm 0x1
#define PIN5_bp 5u
#define PIN5_bm 0x1
#define PIN6_bp 6u
#define PIN6_bm 0x1
#define PIN7_bp 7u
#define PIN7_bm 0x1

#define PIN0_INPUT    ((0x0 & PIN0_bm) << PIN0_bp)
#define PIN0_OUTPUT   ((0x1 & PIN0_bm) << PIN0_bp)
#define PIN1_INPUT    ((0x0 & PIN1_bm) << PIN1_bp)
#define PIN1_OUTPUT   ((0x1 & PIN1_bm) << PIN1_bp)
#define PIN2_INPUT    ((0x0 & PIN2_bm) << PIN2_bp)
#define PIN2_OUTPUT   ((0x1 & PIN2_bm) << PIN2_bp)
#define PIN3_INPUT    ((0x0 & PIN3_bm) << PIN3_bp)
#define PIN3_OUTPUT   ((0x1 & PIN3_bm) << PIN3_bp)
#define PIN4_INPUT    ((0x0 & PIN4_bm) << PIN4_bp)
#define PIN4_OUTPUT   ((0x1 & PIN4_bm) << PIN4_bp)
#define PIN5_INPUT    ((0x0 & PIN5_bm) << PIN5_bp)
#define PIN5_OUTPUT   ((0x1 & PIN5_bm) << PIN5_bp)
#define PIN6_INPUT    ((0x0 & PIN6_bm) << PIN6_bp)
#define PIN6_OUTPUT   ((0x1 & PIN6_bm) << PIN6_bp)
#define PIN7_INPUT    ((0x0 & PIN7_bm) << PIN7_bp)
#define PIN7_OUTPUT   ((0x1 & PIN7_bm) << PIN7_bp)

//-------- PORTn.DR -------- 8 bit
#define PIN0_OUTPUT_LOW    ((0x0 & PIN0_bm) << PIN0_bp)
#define PIN0_OUTPUT_HIGH   ((0x1 & PIN0_bm) << PIN0_bp)
#define PIN1_OUTPUT_LOW    ((0x0 & PIN1_bm) << PIN1_bp)
#define PIN1_OUTPUT_HIGH   ((0x1 & PIN1_bm) << PIN1_bp)
#define PIN2_OUTPUT_LOW    ((0x0 & PIN2_bm) << PIN2_bp)
#define PIN2_OUTPUT_HIGH   ((0x1 & PIN2_bm) << PIN2_bp)
#define PIN3_OUTPUT_LOW    ((0x0 & PIN3_bm) << PIN3_bp)
#define PIN3_OUTPUT_HIGH   ((0x1 & PIN3_bm) << PIN3_bp)
#define PIN4_OUTPUT_LOW    ((0x0 & PIN4_bm) << PIN4_bp)
#define PIN4_OUTPUT_HIGH   ((0x1 & PIN4_bm) << PIN4_bp)
#define PIN5_OUTPUT_LOW    ((0x0 & PIN5_bm) << PIN5_bp)
#define PIN5_OUTPUT_HIGH   ((0x1 & PIN5_bm) << PIN5_bp)
#define PIN6_OUTPUT_LOW    ((0x0 & PIN6_bm) << PIN6_bp)
#define PIN6_OUTPUT_HIGH   ((0x1 & PIN6_bm) << PIN6_bp)
#define PIN7_OUTPUT_LOW    ((0x0 & PIN7_bm) << PIN7_bp)
#define PIN7_OUTPUT_HIGH   ((0x1 & PIN7_bm) << PIN7_bp)

//-------- PORTn.ICR -------- 8 bit
#define PIN0_INPUT_BUFFER_DISABLED  ((0x0 & PIN0_bm) << PIN0_bp)
#define PIN0_INPUT_BUFFER_ENABLED   ((0x1 & PIN0_bm) << PIN0_bp)
#define PIN1_INPUT_BUFFER_DISABLED  ((0x0 & PIN1_bm) << PIN1_bp)
#define PIN1_INPUT_BUFFER_ENABLED   ((0x1 & PIN1_bm) << PIN1_bp)
#define PIN2_INPUT_BUFFER_DISABLED  ((0x0 & PIN2_bm) << PIN2_bp)
#define PIN2_INPUT_BUFFER_ENABLED   ((0x1 & PIN2_bm) << PIN2_bp)
#define PIN3_INPUT_BUFFER_DISABLED  ((0x0 & PIN3_bm) << PIN3_bp)
#define PIN3_INPUT_BUFFER_ENABLED   ((0x1 & PIN3_bm) << PIN3_bp)
#define PIN4_INPUT_BUFFER_DISABLED  ((0x0 & PIN4_bm) << PIN4_bp)
#define PIN4_INPUT_BUFFER_ENABLED   ((0x1 & PIN4_bm) << PIN4_bp)
#define PIN5_INPUT_BUFFER_DISABLED  ((0x0 & PIN5_bm) << PIN5_bp)
#define PIN5_INPUT_BUFFER_ENABLED   ((0x1 & PIN5_bm) << PIN5_bp)
#define PIN6_INPUT_BUFFER_DISABLED  ((0x0 & PIN6_bm) << PIN6_bp)
#define PIN6_INPUT_BUFFER_ENABLED   ((0x1 & PIN6_bm) << PIN6_bp)
#define PIN7_INPUT_BUFFER_DISABLED  ((0x0 & PIN7_bm) << PIN7_bp)
#define PIN7_INPUT_BUFFER_ENABLED   ((0x1 & PIN7_bm) << PIN7_bp)

//-------- PORTn.ODR -------- 8 bit
#define PIN0_OUTPUT_CMOS  ((0x0 & PIN0_bm) << PIN0_bp)
#define PIN0_OUTPUT_NMOS  ((0x1 & PIN0_bm) << PIN0_bp)
#define PIN1_OUTPUT_CMOS  ((0x0 & PIN1_bm) << PIN1_bp)
#define PIN1_OUTPUT_NMOS  ((0x1 & PIN1_bm) << PIN1_bp)
#define PIN2_OUTPUT_CMOS  ((0x0 & PIN2_bm) << PIN2_bp)
#define PIN2_OUTPUT_NMOS  ((0x1 & PIN2_bm) << PIN2_bp)
#define PIN3_OUTPUT_CMOS  ((0x0 & PIN3_bm) << PIN3_bp)
#define PIN3_OUTPUT_NMOS  ((0x1 & PIN3_bm) << PIN3_bp)
#define PIN4_OUTPUT_CMOS  ((0x0 & PIN4_bm) << PIN4_bp)
#define PIN4_OUTPUT_NMOS  ((0x1 & PIN4_bm) << PIN4_bp)
#define PIN5_OUTPUT_CMOS  ((0x0 & PIN5_bm) << PIN5_bp)
#define PIN5_OUTPUT_NMOS  ((0x1 & PIN5_bm) << PIN5_bp)
#define PIN6_OUTPUT_CMOS  ((0x0 & PIN6_bm) << PIN6_bp)
#define PIN6_OUTPUT_NMOS  ((0x1 & PIN6_bm) << PIN6_bp)
#define PIN7_OUTPUT_CMOS  ((0x0 & PIN7_bm) << PIN7_bp)
#define PIN7_OUTPUT_NMOS  ((0x1 & PIN7_bm) << PIN7_bp)

//-------- PORTn.PCR -------- 8 bit
#define PIN0_INPUT_PULLUP_DISABLED ((0x0 & PIN0_bm) << PIN0_bp)
#define PIN0_INPUT_PULLUP_ENABLED  ((0x1 & PIN0_bm) << PIN0_bp)
#define PIN1_INPUT_PULLUP_DISABLED ((0x0 & PIN1_bm) << PIN1_bp)
#define PIN1_INPUT_PULLUP_ENABLED  ((0x1 & PIN1_bm) << PIN1_bp)
#define PIN2_INPUT_PULLUP_DISABLED ((0x0 & PIN2_bm) << PIN2_bp)
#define PIN2_INPUT_PULLUP_ENABLED  ((0x1 & PIN2_bm) << PIN2_bp)
#define PIN3_INPUT_PULLUP_DISABLED ((0x0 & PIN3_bm) << PIN3_bp)
#define PIN3_INPUT_PULLUP_ENABLED  ((0x1 & PIN3_bm) << PIN3_bp)
#define PIN4_INPUT_PULLUP_DISABLED ((0x0 & PIN4_bm) << PIN4_bp)
#define PIN4_INPUT_PULLUP_ENABLED  ((0x1 & PIN4_bm) << PIN4_bp)
#define PIN5_INPUT_PULLUP_DISABLED ((0x0 & PIN5_bm) << PIN5_bp)
#define PIN5_INPUT_PULLUP_ENABLED  ((0x1 & PIN5_bm) << PIN5_bp)
#define PIN6_INPUT_PULLUP_DISABLED ((0x0 & PIN6_bm) << PIN6_bp)
#define PIN6_INPUT_PULLUP_ENABLED  ((0x1 & PIN6_bm) << PIN6_bp)
#define PIN7_INPUT_PULLUP_DISABLED ((0x0 & PIN7_bm) << PIN7_bp)
#define PIN7_INPUT_PULLUP_ENABLED  ((0x1 & PIN7_bm) << PIN7_bp)

//-------- IOPORT.PF0CSE -------- 8 bit
//Each bit enables or disables the corresponding CSn# output.
#define PF0CSE_CS0E_bp 0u
#define PF0CSE_CS0E_bm 0x1
#define PF0CSE_CS1E_bp 1u
#define PF0CSE_CS1E_bm 0x1
#define PF0CSE_CS2E_bp 2u
#define PF0CSE_CS2E_bm 0x1
#define PF0CSE_CS3E_bp 3u
#define PF0CSE_CS3E_bm 0x1
#define PF0CSE_CS4E_bp 4u
#define PF0CSE_CS4E_bm 0x1
#define PF0CSE_CS5E_bp 5u
#define PF0CSE_CS5E_bm 0x1
#define PF0CSE_CS6E_bp 6u
#define PF0CSE_CS6E_bm 0x1
#define PF0CSE_CS7E_bp 7u
#define PF0CSE_CS7E_bm 0x1

#define CS0E_CS0_IO_CONTROL ((0x0 & PF0CSE_CS0E_bm) << PF0CSE_CS0E_bp)
#define CS0E_CS0_ENABLED    ((0x1 & PF0CSE_CS0E_bm) << PF0CSE_CS0E_bp)

#define CS1E_CS1_IO_CONTROL ((0x0 & PF0CSE_CS1E_bm) << PF0CSE_CS1E_bp)
#define CS1E_CS1_ENABLED    ((0x1 & PF0CSE_CS1E_bm) << PF0CSE_CS1E_bp)

#define CS2E_CS2_IO_CONTROL ((0x0 & PF0CSE_CS2E_bm) << PF0CSE_CS2E_bp)
#define CS2E_CS2_ENABLED    ((0x1 & PF0CSE_CS2E_bm) << PF0CSE_CS2E_bp)

#define CS3E_CS3_IO_CONTROL ((0x0 & PF0CSE_CS3E_bm) << PF0CSE_CS3E_bp)
#define CS3E_CS3_ENABLED    ((0x1 & PF0CSE_CS3E_bm) << PF0CSE_CS3E_bp)

#define CS4E_CS4_IO_CONTROL ((0x0 & PF0CSE_CS4E_bm) << PF0CSE_CS4E_bp)
#define CS4E_CS4_ENABLED    ((0x1 & PF0CSE_CS4E_bm) << PF0CSE_CS4E_bp)

#define CS5E_CS5_IO_CONTROL ((0x0 & PF0CSE_CS5E_bm) << PF0CSE_CS5E_bp)
#define CS5E_CS5_ENABLED    ((0x1 & PF0CSE_CS5E_bm) << PF0CSE_CS5E_bp)

#define CS6E_CS6_IO_CONTROL ((0x0 & PF0CSE_CS6E_bm) << PF0CSE_CS6E_bp)
#define CS6E_CS6_ENABLED    ((0x1 & PF0CSE_CS6E_bm) << PF0CSE_CS6E_bp)

#define CS7E_CS7_IO_CONTROL ((0x0 & PF0CSE_CS7E_bm) << PF0CSE_CS7E_bp)
#define CS7E_CS7_ENABLED    ((0x1 & PF0CSE_CS7E_bm) << PF0CSE_CS7E_bp)

//-------- IOPORT.PF1CSS -------- 8 bit
//These bits select a pin for each CSn# output when a CSn# output is enabled
#define PF1CSS_CS4S_bp 0u
#define PF1CSS_CS4S_bm 0x3
#define PF1CSS_CS5S_bp 2u
#define PF1CSS_CS5S_bm 0x3
#define PF1CSS_CS6S_bp 4u
#define PF1CSS_CS6S_bm 0x3
#define PF1CSS_CS7S_bp 6u
#define PF1CSS_CS7S_bm 0x3

#define CS4S_P64_CS4_A ((0x0 & PF1CSS_CS4S_bm) << PF1CSS_CS4S_bp)
#define CS4S_P74_CS4_B ((0x1 & PF1CSS_CS4S_bm) << PF1CSS_CS4S_bp)
#define CS4S_P24_CS4_C ((0x2 & PF1CSS_CS4S_bm) << PF1CSS_CS4S_bp)

#define CS5S_P65_CS5_A ((0x0 & PF1CSS_CS5S_bm) << PF1CSS_CS5S_bp)
#define CS5S_P75_CS5_B ((0x1 & PF1CSS_CS5S_bm) << PF1CSS_CS5S_bp)
#define CS5S_P25_CS5_C ((0x2 & PF1CSS_CS5S_bm) << PF1CSS_CS5S_bp)

#define CS6S_P66_CS6_A ((0x0 & PF1CSS_CS6S_bm) << PF1CSS_CS6S_bp)
#define CS6S_P76_CS6_B ((0x1 & PF1CSS_CS6S_bm) << PF1CSS_CS6S_bp)
#define CS6S_P26_CS6_C ((0x2 & PF1CSS_CS6S_bm) << PF1CSS_CS6S_bp)

#define CS7S_P67_CS7_A ((0x0 & PF1CSS_CS7S_bm) << PF1CSS_CS7S_bp)
#define CS7S_P77_CS7_B ((0x1 & PF1CSS_CS7S_bm) << PF1CSS_CS7S_bp)
#define CS7S_P27_CS7_C ((0x2 & PF1CSS_CS7S_bm) << PF1CSS_CS7S_bp)

//-------- IOPORT.PF2CSS -------- 8 bit
//These bits select a pin for each CSn# output when a CSn# output is enabled
#define PF2CSS_CS0S_bp 0u
#define PF2CSS_CS0S_bm 0x1
#define PF2CSS_CS1S_bp 2u
#define PF2CSS_CS1S_bm 0x3
#define PF2CSS_CS2S_bp 4u
#define PF2CSS_CS2S_bm 0x3
#define PF2CSS_CS3S_bp 6u
#define PF2CSS_CS3S_bm 0x3

#define CS0S_P60_CS0_A ((0x0 & PF2CSS_CS0S_bm) << PF2CSS_CS0S_bp)
#define CS0S_PC7_CS0_B ((0x1 & PF2CSS_CS0S_bm) << PF2CSS_CS0S_bp)

#define CS1S_P61_CS1_A ((0x0 & PF2CSS_CS1S_bm) << PF2CSS_CS1S_bp)
#define CS1S_P71_CS1_B ((0x1 & PF2CSS_CS1S_bm) << PF2CSS_CS1S_bp)
#define CS1S_PC6_CS1_C ((0x2 & PF2CSS_CS1S_bm) << PF2CSS_CS1S_bp)

#define CS2S_P62_CS2_A ((0x0 & PF2CSS_CS2S_bm) << PF2CSS_CS2S_bp)
#define CS2S_P72_CS2_B ((0x1 & PF2CSS_CS2S_bm) << PF2CSS_CS2S_bp)
#define CS2S_PC5_CS2_C ((0x2 & PF2CSS_CS2S_bm) << PF2CSS_CS2S_bp)

#define CS3S_P63_CS3_A ((0x0 & PF2CSS_CS3S_bm) << PF2CSS_CS3S_bp)
#define CS3S_P73_CS3_B ((0x1 & PF2CSS_CS3S_bm) << PF2CSS_CS3S_bp)
#define CS3S_PC4_CS3_C ((0x2 & PF2CSS_CS3S_bm) << PF2CSS_CS3S_bp)

//-------- IOPORT.PF3BUS -------- 8 bit
// Enables or disables an address output
#define PF3BUS_A16E_bp 0u
#define PF3BUS_A16E_bm 0x1
#define PF3BUS_A17E_bp 1u
#define PF3BUS_A17E_bm 0x1
#define PF3BUS_A18E_bp 2u
#define PF3BUS_A18E_bm 0x1
#define PF3BUS_A19E_bp 3u
#define PF3BUS_A19E_bm 0x1
#define PF3BUS_A20E_bp 4u
#define PF3BUS_A20E_bm 0x1
#define PF3BUS_A21E_bp 5u
#define PF3BUS_A21E_bm 0x1
#define PF3BUS_A22E_bp 6u
#define PF3BUS_A22E_bm 0x1
#define PF3BUS_A23E_bp 7u
#define PF3BUS_A23E_bm 0x1

#define A16E_ADDRESS_A16_DISABLED ((0x0 & PF3BUS_A16E_bm) << PF3BUS_A16E_bp)
#define A16E_ADDRESS_A16_ENABLED  ((0x1 & PF3BUS_A16E_bm) << PF3BUS_A16E_bp)
#define A17E_ADDRESS_A17_DISABLED ((0x0 & PF3BUS_A17E_bm) << PF3BUS_A17E_bp)
#define A17E_ADDRESS_A17_ENABLED  ((0x1 & PF3BUS_A17E_bm) << PF3BUS_A17E_bp)
#define A18E_ADDRESS_A18_DISABLED ((0x0 & PF3BUS_A18E_bm) << PF3BUS_A18E_bp)
#define A18E_ADDRESS_A18_ENABLED  ((0x1 & PF3BUS_A18E_bm) << PF3BUS_A18E_bp)
#define A19E_ADDRESS_A19_DISABLED ((0x0 & PF3BUS_A19E_bm) << PF3BUS_A19E_bp)
#define A19E_ADDRESS_A19_ENABLED  ((0x1 & PF3BUS_A19E_bm) << PF3BUS_A19E_bp)
#define A20E_ADDRESS_A20_DISABLED ((0x0 & PF3BUS_A20E_bm) << PF3BUS_A20E_bp)
#define A20E_ADDRESS_A20_ENABLED  ((0x1 & PF3BUS_A20E_bm) << PF3BUS_A20E_bp)
#define A21E_ADDRESS_A21_DISABLED ((0x0 & PF3BUS_A21E_bm) << PF3BUS_A21E_bp)
#define A21E_ADDRESS_A21_ENABLED  ((0x1 & PF3BUS_A21E_bm) << PF3BUS_A21E_bp)
#define A22E_ADDRESS_A22_DISABLED ((0x0 & PF3BUS_A22E_bm) << PF3BUS_A22E_bp)
#define A22E_ADDRESS_A22_ENABLED  ((0x1 & PF3BUS_A22E_bm) << PF3BUS_A22E_bp)
#define A23E_ADDRESS_A23_DISABLED ((0x0 & PF3BUS_A23E_bm) << PF3BUS_A23E_bp)
#define A23E_ADDRESS_A23_ENABLED  ((0x1 & PF3BUS_A23E_bm) << PF3BUS_A23E_bp)

//-------- IOPORT.PF4BUS -------- 8 bit
// Enables or disables an address output
#define PF4BUS_ADRLE_bp 0u
#define PF4BUS_ADRLE_bm 0x3
#define PF4BUS_A10E_bp 2u
#define PF4BUS_A10E_bm 0x1
#define PF4BUS_A11E_bp 3u
#define PF4BUS_A11E_bm 0x1
#define PF4BUS_A12E_bp 4u
#define PF4BUS_A12E_bm 0x1
#define PF4BUS_A13E_bp 5u
#define PF4BUS_A13E_bm 0x1
#define PF4BUS_A14E_bp 6u
#define PF4BUS_A14E_bm 0x1
#define PF4BUS_A15E_bp 7u
#define PF4BUS_A15E_bm 0x1

#define ADRLE_ADDRESS_A9_TO_A0_DISABLED ((0x0 & PF4BUS_ADRLE_bm) << PF4BUS_ADRLE_bp)
#define ADRLE_ADDRESS_A9_TO_A4_DISABLED ((0x1 & PF4BUS_ADRLE_bm) << PF4BUS_ADRLE_bp)
#define ADRLE_ADDRESS_A9_TO_A8_DISABLED ((0x2 & PF4BUS_ADRLE_bm) << PF4BUS_ADRLE_bp)
#define ADRLE_ADDRESS_A9_TO_A0_ENABLED  ((0x3 & PF4BUS_ADRLE_bm) << PF4BUS_ADRLE_bp)
#define A10E_ADDRESS_A10_DISABLED ((0x0 & PF4BUS_A10E_bm) << PF4BUS_A10E_bp)
#define A10E_ADDRESS_A10_ENABLED  ((0x1 & PF4BUS_A10E_bm) << PF4BUS_A10E_bp)
#define A11E_ADDRESS_A11_DISABLED ((0x0 & PF4BUS_A11E_bm) << PF4BUS_A11E_bp)
#define A11E_ADDRESS_A11_ENABLED  ((0x1 & PF4BUS_A11E_bm) << PF4BUS_A11E_bp)
#define A12E_ADDRESS_A12_DISABLED ((0x0 & PF4BUS_A12E_bm) << PF4BUS_A12E_bp)
#define A12E_ADDRESS_A12_ENABLED  ((0x1 & PF4BUS_A12E_bm) << PF4BUS_A12E_bp)
#define A13E_ADDRESS_A13_DISABLED ((0x0 & PF4BUS_A13E_bm) << PF4BUS_A13E_bp)
#define A13E_ADDRESS_A13_ENABLED  ((0x1 & PF4BUS_A13E_bm) << PF4BUS_A13E_bp)
#define A14E_ADDRESS_A14_DISABLED ((0x0 & PF4BUS_A14E_bm) << PF4BUS_A14E_bp)
#define A14E_ADDRESS_A14_ENABLED  ((0x1 & PF4BUS_A14E_bm) << PF4BUS_A14E_bp)
#define A15E_ADDRESS_A15_DISABLED ((0x0 & PF4BUS_A15E_bm) << PF4BUS_A15E_bp)
#define A15E_ADDRESS_A15_ENABLED  ((0x1 & PF4BUS_A15E_bm) << PF4BUS_A15E_bp)

//-------- IOPORT.PF5BUS -------- 8 bit
#define PF5BUS_ADRHMS_bp  1u
#define PF5BUS_ADRHMS_bm  0x1
#define PF5BUS_DHE_bp     4u
#define PF5BUS_DHE_bm     0x1
#define PF5BUS_WR1BC1E_bp 6u
#define PF5BUS_WR1BC1E_bm 0x1

#define ADRHMS_A19_TO_A16_A   ((0x0 & PF5BUS_ADRHMS_bm) << PF5BUS_ADRHMS_bp)
#define ADRHMS_A19_TO_A16_B   ((0x1 & PF5BUS_ADRHMS_bm) << PF5BUS_ADRHMS_bp)
#define DHE_D15_TO_D8_DISABLE ((0x0 & PF5BUS_DHE_bm) << PF5BUS_DHE_bp)
#define DHE_D15_TO_D8_ENABLE  ((0x1 & PF5BUS_DHE_bm) << PF5BUS_DHE_bp)
#define WR1BC1E_P51_IO        ((0x0 & PF5BUS_WR1BC1E_bm) << PF5BUS_WR1BC1E_bp)
#define WR1BC1E_P51_WR1_BC1   ((0x1 & PF5BUS_WR1BC1E_bm) << PF5BUS_WR1BC1E_bp)


//-------- IOPORT.PF6BUS -------- 8 bit
#define PF6BUS_WAITS_bp  0u
#define PF6BUS_WAITS_bm  0x3
#define PF6BUS_MDSDE_bp  4u
#define PF6BUS_MDSDE_bm  0x1
#define PF6BUS_DQM1E_bp  6u
#define PF6BUS_DQM1E_bm  0x1
#define PF6BUS_SDCLKE_bp  7u
#define PF6BUS_SDCLKE_bm  0x1

#define WAITS_P55_WAIT_B  ((0x0 & PF6BUS_WAITS_bm) << PF6BUS_WAITS_bp)
#define WAITS_P55_WAIT_C  ((0x2 & PF6BUS_WAITS_bm) << PF6BUS_WAITS_bp)
#define WAITS_P55_WAIT_D  ((0x3 & PF6BUS_WAITS_bm) << PF6BUS_WAITS_bp)
#define MDSDE_SDRAM_DISABLE ((0x0 & PF6BUS_MDSDE_bm) << PF6BUS_MDSDE_bp)
#define MDSDE_SDRAM_ENABLE  ((0x1 & PF6BUS_MDSDE_bm) << PF6BUS_MDSDE_bp)
#define DQM1E_SDRAM_DISABLE_P67  ((0x0 & PF6BUS_DQM1E_bm) << PF6BUS_DQM1E_bp)
#define DQM1E_SDRAM_ENABLE_ALL   ((0x1 & PF6BUS_DQM1E_bm) << PF6BUS_DQM1E_bp)
#define SDCLKE_SDCLK_OUT_DISABLED  ((0x0 & PF6BUS_SDCLKE_bm) << PF6BUS_SDCLKE_bp)
#define SDCLKE_SDCLK_OUT_ENABLED   ((0x1 & PF6BUS_SDCLKE_bm) << PF6BUS_SDCLKE_bp)

//-------- IOPORT.PF7DMA -------- 8 bit
#define PF7DMA_EDMA0S_bp  4u
#define PF7DMA_EDMA0S_bm  0x3
#define PF7DMA_EDMA1S_bp  6u
#define PF7DMA_EDMA1S_bm  0x3

#define EDMA0S_EDREQ0_EDACK0_A  ((0x0 & PF7DMA_EDMA0S_bm) << PF7DMA_EDMA0S_bp)
#define EDMA0S_EDREQ0_EDACK0_B  ((0x1 & PF7DMA_EDMA0S_bm) << PF7DMA_EDMA0S_bp)
#define EDMA0S_EDREQ0_EDACK0_C  ((0x2 & PF7DMA_EDMA0S_bm) << PF7DMA_EDMA0S_bp)
#define EDMA1S_EDREQ1_EDACK1_A  ((0x0 & PF7DMA_EDMA1S_bm) << PF7DMA_EDMA1S_bp)
#define EDMA1S_EDREQ1_EDACK1_B  ((0x1 & PF7DMA_EDMA1S_bm) << PF7DMA_EDMA1S_bp)
#define EDMA1S_EDREQ1_EDACK1_C  ((0x2 & PF7DMA_EDMA1S_bm) << PF7DMA_EDMA1S_bp)


//-------- IOPORT.PF8IRQ -------- 8 bit
#define PF8IRQ_ITS8_bp 0u
#define PF8IRQ_ITS8_bm 0x1
#define PF8IRQ_ITS9_bp 1u
#define PF8IRQ_ITS9_bm 0x1
#define PF8IRQ_ITS10_bp 2u
#define PF8IRQ_ITS10_bm 0x1
#define PF8IRQ_ITS11_bp 3u
#define PF8IRQ_ITS11_bm 0x1
#define PF8IRQ_ITS13_bp 5u
#define PF8IRQ_ITS13_bm 0x1
#define PF8IRQ_ITS15_bp 7u
#define PF8IRQ_ITS15_bm 0x1

#define ITS8_IRQ8_A  ((0x0 & PF8IRQ_ITS8_bm) << PF8IRQ_ITS8_bp)
#define ITS8_IRQ8_B  ((0x1 & PF8IRQ_ITS8_bm) << PF8IRQ_ITS8_bp)
#define ITS9_IRQ9_A  ((0x0 & PF8IRQ_ITS9_bm) << PF8IRQ_ITS9_bp)
#define ITS9_IRQ9_B  ((0x1 & PF8IRQ_ITS9_bm) << PF8IRQ_ITS9_bp)
#define ITS10_IRQ10_A  ((0x0 & PF8IRQ_ITS10_bm) << PF8IRQ_ITS10_bp)
#define ITS10_IRQ10_B  ((0x1 & PF8IRQ_ITS10_bm) << PF8IRQ_ITS10_bp)
#define ITS11_IRQ11_A  ((0x0 & PF8IRQ_ITS11_bm) << PF8IRQ_ITS11_bp)
#define ITS11_IRQ11_B  ((0x1 & PF8IRQ_ITS11_bm) << PF8IRQ_ITS11_bp)
#define ITS13_IRQ13_A  ((0x0 & PF8IRQ_ITS13_bm) << PF8IRQ_ITS13_bp)
#define ITS13_IRQ13_B  ((0x1 & PF8IRQ_ITS13_bm) << PF8IRQ_ITS13_bp)
#define ITS15_IRQ15_A  ((0x0 & PF8IRQ_ITS15_bm) << PF8IRQ_ITS15_bp)
#define ITS15_IRQ15_B  ((0x1 & PF8IRQ_ITS15_bm) << PF8IRQ_ITS15_bp)

//-------- IOPORT.PF9IRQ -------- 8 bit
#define PF9IRQ_ITS2_bp 2u
#define PF9IRQ_ITS2_bm 0x1
#define PF9IRQ_ITS3_bp 3u
#define PF9IRQ_ITS3_bm 0x1
#define PF9IRQ_ITS4_bp 4u
#define PF9IRQ_ITS4_bm 0x1
#define PF9IRQ_ITS5_bp 5u
#define PF9IRQ_ITS5_bm 0x1
#define PF9IRQ_ITS6_bp 6u
#define PF9IRQ_ITS6_bm 0x1
#define PF9IRQ_ITS7_bp 7u
#define PF9IRQ_ITS7_bm 0x1

#define ITS2_IRQ2_A  ((0x0 & PF9IRQ_ITS2_bm) << PF9IRQ_ITS2_bp)
#define ITS2_IRQ2_B  ((0x1 & PF9IRQ_ITS2_bm) << PF9IRQ_ITS2_bp)
#define ITS3_IRQ3_A  ((0x0 & PF9IRQ_ITS3_bm) << PF9IRQ_ITS3_bp)
#define ITS3_IRQ3_B  ((0x1 & PF9IRQ_ITS3_bm) << PF9IRQ_ITS3_bp)
#define ITS4_IRQ4_A  ((0x0 & PF9IRQ_ITS4_bm) << PF9IRQ_ITS4_bp)
#define ITS4_IRQ4_B  ((0x1 & PF9IRQ_ITS4_bm) << PF9IRQ_ITS4_bp)
#define ITS5_IRQ5_A  ((0x0 & PF9IRQ_ITS5_bm) << PF9IRQ_ITS5_bp)
#define ITS5_IRQ5_B  ((0x1 & PF9IRQ_ITS5_bm) << PF9IRQ_ITS5_bp)
#define ITS6_IRQ6_A  ((0x0 & PF9IRQ_ITS6_bm) << PF9IRQ_ITS6_bp)
#define ITS6_IRQ6_B  ((0x1 & PF9IRQ_ITS6_bm) << PF9IRQ_ITS6_bp)
#define ITS7_IRQ7_A  ((0x0 & PF9IRQ_ITS7_bm) << PF9IRQ_ITS7_bp)
#define ITS7_IRQ7_B  ((0x1 & PF9IRQ_ITS7_bm) << PF9IRQ_ITS7_bp)

//-------- IOPORT.PFAADC -------- 8 bit
#define PFAADC_ADTRG0S_bp 0u
#define PFAADC_ADTRG0S_bm 0x1

#define ADTRG0S_A  ((0x0 & PFAADC_ADTRG0S_bm) << PFAADC_ADTRG0S_bp)
#define ADTRG0S_B  ((0x1 & PFAADC_ADTRG0S_bm) << PFAADC_ADTRG0S_bp)


//-------- IOPORT.PFBTMR -------- 8 bit
//Each bit selects a pin for a TMRn input.
#define PFBTMR_TMR0S_bp 0u
#define PFBTMR_TMR0S_bm 0x1
#define PFBTMR_TMR1S_bp 1u
#define PFBTMR_TMR1S_bm 0x1
#define PFBTMR_TMR2S_bp 2u
#define PFBTMR_TMR2S_bm 0x1

#define TMR0S_TMCI0_TMRIO_A ((0x0 & PFBTMR_TMR0S_bm) << PFBTMR_TMR0S_bp)
#define TMR0S_TMCI0_TMRIO_B ((0x1 & PFBTMR_TMR0S_bm) << PFBTMR_TMR0S_bp)
#define TMR1S_TMCI1_TMRI1_A ((0x0 & PFBTMR_TMR1S_bm) << PFBTMR_TMR1S_bp)
#define TMR1S_TMCI1_TMRI1_B ((0x1 & PFBTMR_TMR1S_bm) << PFBTMR_TMR1S_bp)
#define TMR2S_TMCI2_TMRI2_A ((0x0 & PFBTMR_TMR2S_bm) << PFBTMR_TMR2S_bp)
#define TMR2S_TMCI2_TMRI2_B ((0x1 & PFBTMR_TMR2S_bm) << PFBTMR_TMR2S_bp)


//-------- IOPORT.PFCMTU -------- 8 bit
#define PFCMTU_MTUS2_bp 2u
#define PFCMTU_MTUS2_bm 0x1
#define PFCMTU_MTUS3_bp 3u
#define PFCMTU_MTUS3_bm 0x1
#define PFCMTU_MTUS4_bp 4u
#define PFCMTU_MTUS4_bm 0x1
#define PFCMTU_MTUS5_bp 5u
#define PFCMTU_MTUS5_bm 0x1
#define PFCMTU_TCLKS_bp 7u
#define PFCMTU_TCLKS_bm 0x1

#define MTUS2_MTIOC3C_A ((0x0 & PFCMTU_MTUS2_bm) << PFCMTU_MTUS2_bp)
#define MTUS2_MTIOC3C_B ((0x1 & PFCMTU_MTUS2_bm) << PFCMTU_MTUS2_bp)

#define MTUS3_MTIOC3B_MTIOC3D_A ((0x0 & PFCMTU_MTUS3_bm) << PFCMTU_MTUS3_bp)
#define MTUS3_MTIOC3B_MTIOC3D_B ((0x1 & PFCMTU_MTUS3_bm) << PFCMTU_MTUS3_bp)

#define MTUS4_MTIOC4A_MTIOC4C_A ((0x0 & PFCMTU_MTUS4_bm) << PFCMTU_MTUS4_bp)
#define MTUS4_MTIOC4A_MTIOC4C_B ((0x1 & PFCMTU_MTUS4_bm) << PFCMTU_MTUS4_bp)

#define MTUS5_MTIOC4B_MTIOC4D_A ((0x0 & PFCMTU_MTUS5_bm) << PFCMTU_MTUS5_bp)
#define MTUS5_MTIOC4B_MTIOC4D_B ((0x1 & PFCMTU_MTUS5_bm) << PFCMTU_MTUS5_bp)

#define TCLKS_MTCLKA_MTCLKB_MTCLKC_MTCLKD_A ((0x0 & PFCMTU_TCLKS_bm) << PFCMTU_TCLKS_bp)
#define TCLKS_MTCLKA_MTCLKB_MTCLKC_MTCLKD_B ((0x1 & PFCMTU_TCLKS_bm) << PFCMTU_TCLKS_bp)

//-------- IOPORT.PFDMTU -------- 8 bit
#define PFDMTU_MTUS6_bp 6u
#define PFDMTU_MTUS6_bm 0x1
#define PFDMTU_TCLKS_bp 7u
#define PFDMTU_TCLKS_bm 0x1

#define MTUS6_MTIC11U_MTIC11V_MTIC11W_A ((0x0 & PFDMTU_MTUS6_bm) << PFDMTU_MTUS6_bp)
#define MTUS6_MTIC11U_MTIC11V_MTIC11W_B ((0x1 & PFDMTU_MTUS6_bm) << PFDMTU_MTUS6_bp)
#define TCLKS_MTCLKE_MTCLKF_MTCLKG_MTCLKH_A ((0x0 & PFDMTU_TCLKS_bm) << PFDMTU_TCLKS_bp)
#define TCLKS_MTCLKE_MTCLKF_MTCLKG_MTCLKH_B ((0x1 & PFDMTU_TCLKS_bm) << PFDMTU_TCLKS_bp)

//-------- IOPORT.PFENET -------- 8 bit
#define PFENET_ENETE0_bp 0u
#define PFENET_ENETE0_bm 0x1
#define PFENET_ENETE1_bp 1u
#define PFENET_ENETE1_bm 0x1
#define PFENET_ENETE2_bp 2u
#define PFENET_ENETE2_bm 0x1
#define PFENET_ENETE3_bp 3u
#define PFENET_ENETE3_bm 0x1
#define PFENET_PHYMODE_bp 4u
#define PFENET_PHYMODE_bm 0x1
#define PFENET_EE_bp 7u
#define PFENET_EE_bm 0x1

#define ENETE0_ET_WOL_DISABLED      ((0x0 & PFENET_ENETE0_bm) << PFENET_ENETE0_bp)
#define ENETE0_ET_WOL_ENABLED       ((0x1 & PFENET_ENETE0_bm) << PFENET_ENETE0_bp)
#define ENETE1_ET_LINKSTA_DISABLED  ((0x0 & PFENET_ENETE1_bm) << PFENET_ENETE1_bp)
#define ENETE1_ET_LINKSTA_ENABLED   ((0x1 & PFENET_ENETE1_bm) << PFENET_ENETE1_bp)
#define ENETE2_ET_EXOUT_DISABLED    ((0x0 & PFENET_ENETE2_bm) << PFENET_ENETE2_bp)
#define ENETE2_ET_EXOUT_ENABLED     ((0x1 & PFENET_ENETE2_bm) << PFENET_ENETE2_bp)
#define ENETE3_ET_TX_ER_DISABLED    ((0x0 & PFENET_ENETE3_bm) << PFENET_ENETE3_bp)
#define ENETE3_ET_TX_ER_ENABLED     ((0x1 & PFENET_ENETE3_bm) << PFENET_ENETE3_bp)
#define PHYMODE_RMII_MODE           ((0x0 & PFENET_PHYMODE_bm) << PFENET_PHYMODE_bp)
#define PHYMODE_MII_MODE            ((0x1 & PFENET_PHYMODE_bm) << PFENET_PHYMODE_bp)
#define EE_ETHERNET_DISABLED        ((0x0 & PFENET_EE_bm) << PFENET_EE_bp)
#define EE_ETHERNET_ENABLED         ((0x1 & PFENET_EE_bm) << PFENET_EE_bp)

//-------- IOPORT.PFFSCI -------- 8 bit
//Each bit selects a pin for an SCI channel-n input/output.
#define PFFSCI_SCI2S_bp 2u
#define PFFSCI_SCI2S_bm 0x1
#define PFFSCI_SCI3S_bp 3u
#define PFFSCI_SCI3S_bm 0x1
#define PFFSCI_SCI6S_bp 6u
#define PFFSCI_SCI6S_bm 0x1

#define SCI2S_SCK2_RxD2_TxD2_A ((0x0 & PFFSCI_SCI2S_bm) << PFFSCI_SCI2S_bp)
#define SCI2S_SCK2_RxD2_TxD2_B ((0x1 & PFFSCI_SCI2S_bm) << PFFSCI_SCI2S_bp)
#define SCI3S_SCK2_RxD3_TxD3_A ((0x0 & PFFSCI_SCI3S_bm) << PFFSCI_SCI3S_bp)
#define SCI3S_SCK2_RxD3_TxD3_B ((0x1 & PFFSCI_SCI3S_bm) << PFFSCI_SCI3S_bp)
#define SCI6S_SCK2_RxD6_TxD6_A ((0x0 & PFFSCI_SCI6S_bm) << PFFSCI_SCI6S_bp)
#define SCI6S_SCK2_RxD6_TxD6_B ((0x1 & PFFSCI_SCI6S_bm) << PFFSCI_SCI6S_bp)

//-------- IOPORT.PFGSPI -------- 8 bit
//Each bit selects a pin for an SCI channel-n input/output. (Channel 0)
#define PFGSPI_RSPI_bp 0u
#define PFGSPI_RSPI_bm 0x1
#define PFGSPI_RSCPKE_bp 1u
#define PFGSPI_RSCPKE_bm 0x1
#define PFGSPI_MOSIE_bp 2u
#define PFGSPI_MOSIE_bm 0x1
#define PFGSPI_MISOE_bp 3u
#define PFGSPI_MISOE_bm 0x1
#define PFGSPI_SSL0E_bp 4u
#define PFGSPI_SSL0E_bm 0x1
#define PFGSPI_SSL1E_bp 5u
#define PFGSPI_SSL1E_bm 0x1
#define PFGSPI_SSL2E_bp 6u
#define PFGSPI_SSL2E_bm 0x1
#define PFGSPI_SSL3E_bp 7u
#define PFGSPI_SSL3E_bm 0x1

#define RSPIS_RSPIA_PIN_SELECT_A ((0x0 & PFGSPI_RSPI_bm) << PFGSPI_RSPI_bp)
#define RSPIS_RSPIA_PIN_SELECT_B ((0x1 & PFGSPI_RSPI_bm) << PFGSPI_RSPI_bp)
#define RSPCKE_RSPCKA_DISABLED ((0x0 & PFGSPI_RSPI_bm) << PFGSPI_RSPI_bp)
#define RSPCKE_RSPCKA_ENABLED  ((0x1 & PFGSPI_RSPI_bm) << PFGSPI_RSPI_bp)
#define MOSIE_MOSIA_DISABLED   ((0x0 & PFGSPI_MOSIE_bm) << PFGSPI_MOSIE_bp)
#define MOSIE_MOSIA_ENABLED    ((0x1 & PFGSPI_MOSIE_bm) << PFGSPI_MOSIE_bp)
#define MISOE_MISOA_DISABLED   ((0x0 & PFGSPI_MISOE_bm) << PFGSPI_MISOE_bp)
#define MISOE_MISOA_ENABLED    ((0x1 & PFGSPI_MISOE_bm) << PFGSPI_MISOE_bp)
#define SSL0E_SSLA0_DISABLED   ((0x0 & PFGSPI_SSL0E_bm) << PFGSPI_SSL0E_bp)
#define SSL0E_SSLA0_ENABLED    ((0x1 & PFGSPI_SSL0E_bm) << PFGSPI_SSL0E_bp)
#define SSL1E_SSLA1_DISABLED   ((0x0 & PFGSPI_SSL1E_bm) << PFGSPI_SSL1E_bp)
#define SSL1E_SSLA1_ENABLED    ((0x1 & PFGSPI_SSL1E_bm) << PFGSPI_SSL1E_bp)
#define SSL2E_SSLA2_DISABLED   ((0x0 & PFGSPI_SSL2E_bm) << PFGSPI_SSL2E_bp)
#define SSL2E_SSLA2_ENABLED    ((0x1 & PFGSPI_SSL2E_bm) << PFGSPI_SSL2E_bp)
#define SSL3E_SSLA3_DISABLED   ((0x0 & PFGSPI_SSL3E_bm) << PFGSPI_SSL3E_bp)
#define SSL3E_SSLA3_ENABLED    ((0x1 & PFGSPI_SSL3E_bm) << PFGSPI_SSL3E_bp)

//-------- IOPORT.PFHSPI -------- 8 bit
//Each bit selects a pin for an SCI channel-n input/output. (Channel 1)
#define PFHSPI_RSPI_bp 0u
#define PFHSPI_RSPI_bm 0x1
#define PFHSPI_RSCPKE_bp 1u
#define PFHSPI_RSCPKE_bm 0x1
#define PFHSPI_MOSIE_bp 2u
#define PFHSPI_MOSIE_bm 0x1
#define PFHSPI_MISOE_bp 3u
#define PFHSPI_MISOE_bm 0x1
#define PFHSPI_SSL0E_bp 4u
#define PFHSPI_SSL0E_bm 0x1
#define PFHSPI_SSL1E_bp 5u
#define PFHSPI_SSL1E_bm 0x1
#define PFHSPI_SSL2E_bp 6u
#define PFHSPI_SSL2E_bm 0x1
#define PFHSPI_SSL3E_bp 7u
#define PFHSPI_SSL3E_bm 0x1

#define RSPIS_RSPIB_PIN_SELECT_A ((0x0 & PFHSPI_RSPI_bm) << PFHSPI_RSPI_bp)
#define RSPIS_RSPIB_PIN_SELECT_B ((0x1 & PFHSPI_RSPI_bm) << PFHSPI_RSPI_bp)
#define RSPCKE_RSPCKB_DISABLED ((0x0 & PFHSPI_RSPI_bm) << PFHSPI_RSPI_bp)
#define RSPCKE_RSPCKB_ENABLED  ((0x1 & PFHSPI_RSPI_bm) << PFHSPI_RSPI_bp)
#define MOSIE_MOSIB_DISABLED   ((0x0 & PFHSPI_MOSIE_bm) << PFHSPI_MOSIE_bp)
#define MOSIE_MOSIB_ENABLED    ((0x1 & PFHSPI_MOSIE_bm) << PFHSPI_MOSIE_bp)
#define MISOE_MISOB_DISABLED   ((0x0 & PFHSPI_MISOE_bm) << PFHSPI_MISOE_bp)
#define MISOE_MISOB_ENABLED    ((0x1 & PFHSPI_MISOE_bm) << PFHSPI_MISOE_bp)
#define SSL0E_SSLB0_DISABLED   ((0x0 & PFHSPI_SSL0E_bm) << PFHSPI_SSL0E_bp)
#define SSL0E_SSLB0_ENABLED    ((0x1 & PFHSPI_SSL0E_bm) << PFHSPI_SSL0E_bp)
#define SSL1E_SSLB1_DISABLED   ((0x0 & PFHSPI_SSL1E_bm) << PFHSPI_SSL1E_bp)
#define SSL1E_SSLB1_ENABLED    ((0x1 & PFHSPI_SSL1E_bm) << PFHSPI_SSL1E_bp)
#define SSL2E_SSLB2_DISABLED   ((0x0 & PFHSPI_SSL2E_bm) << PFHSPI_SSL2E_bp)
#define SSL2E_SSLB2_ENABLED    ((0x1 & PFHSPI_SSL2E_bm) << PFHSPI_SSL2E_bp)
#define SSL3E_SSLB3_DISABLED   ((0x0 & PFHSPI_SSL3E_bm) << PFHSPI_SSL3E_bp)
#define SSL3E_SSLB3_ENABLED    ((0x1 & PFHSPI_SSL3E_bm) << PFHSPI_SSL3E_bp)

//-------- IOPORT.PFJCAN -------- 8 bit
//PFJCAN is used to select I/O pins for the CAN.
#define PFJCAN_CAN0E_bp 0u
#define PFJCAN_CAN0E_bm 0x1

#define CAN0E_CAN_DISABLED ((0x0 & PFJCAN_CAN0E_bm) << PFJCAN_CAN0E_bp)
#define CAN0E_CAN_ENABLED  ((0x1 & PFJCAN_CAN0E_bm) << PFJCAN_CAN0E_bp)

//-------- IOPORT.PFKUSB -------- 8 bit
//PFKUSB is used to set I/O pins for the USB0.
#define PFKUSB_USBMD_bp   0u
#define PFKUSB_USBMD_bm   0x3
#define PFKUSB_PUPHZS_bp  2u
#define PFKUSB_PUPHZS_bm  0x1
#define PFKUSB_PDHZS_bp   3u
#define PFKUSB_PDHZS_bm   0x1
#define PFKUSB_USBE_bp    4u
#define PFKUSB_USBE_bm    0x1

#define USBMD_MODE_FUNCTION       ((0x0 & PFKUSB_USBMD_bm) << PFKUSB_USBMD_bp)
#define USBMD_MODE_HOST           ((0x1 & PFKUSB_USBMD_bm) << PFKUSB_USBMD_bp)
#define USBMD_MODE_HF_OPT         ((0x2 & PFKUSB_USBMD_bm) << PFKUSB_USBMD_bp)
#define USBMD_MODE_OTG            ((0x3 & PFKUSB_USBMD_bm) << PFKUSB_USBMD_bp)
#define PUPHZS_DPUPE_EXT_PULL     ((0x0 & PFKUSB_PUPHZS_bm) << PFKUSB_PUPHZS_bp)
#define PUPHZS_DPUPE_HIGH_Z       ((0x1 & PFKUSB_PUPHZS_bm) << PFKUSB_PUPHZS_bp)
#define PDHZS_DPRPD_DRPD_EXT_PULL ((0x0 & PFKUSB_PDHZS_bm) << PFKUSB_PDHZS_bp)
#define PDHZS_DPRPD_DRPD_HIGH_Z   ((0x1 & PFKUSB_PDHZS_bm) << PFKUSB_PDHZS_bp)
#define USBE_USB_DISABLED         ((0x0 & PFKUSB_USBE_bm) << PFKUSB_USBE_bp)
#define USBE_USB_ENABLED          ((0x1 & PFKUSB_USBE_bm) << PFKUSB_USBE_bp)

//-------- IOPORT.PFMPOE -------- 8 bit
//PFMPOE enables or disables POE input pins.
#define PFMPOE_POE0E_bp 0u
#define PFMPOE_POE0E_bm 0x1
#define PFMPOE_POE1E_bp 1u
#define PFMPOE_POE1E_bm 0x1
#define PFMPOE_POE2E_bp 2u
#define PFMPOE_POE2E_bm 0x1
#define PFMPOE_POE3E_bp 3u
#define PFMPOE_POE3E_bm 0x1
#define PFMPOE_POE4E_bp 4u
#define PFMPOE_POE4E_bm 0x1
#define PFMPOE_POE5E_bp 5u
#define PFMPOE_POE5E_bm 0x1
#define PFMPOE_POE6E_bp 6u
#define PFMPOE_POE6E_bm 0x1
#define PFMPOE_POE7E_bp 7u
#define PFMPOE_POE7E_bm 0x1

#define POE0E_POE0_DISABLED ((0x0 & PFMPOE_POE0E_bm) << PFMPOE_POE0E_bp)
#define POE0E_POE0_ENABLED  ((0x1 & PFMPOE_POE0E_bm) << PFMPOE_POE0E_bp)
#define POE1E_POE1_DISABLED ((0x0 & PFMPOE_POE1E_bm) << PFMPOE_POE1E_bp)
#define POE1E_POE1_ENABLED  ((0x1 & PFMPOE_POE1E_bm) << PFMPOE_POE1E_bp)
#define POE2E_POE2_DISABLED ((0x0 & PFMPOE_POE2E_bm) << PFMPOE_POE2E_bp)
#define POE2E_POE2_ENABLED  ((0x1 & PFMPOE_POE2E_bm) << PFMPOE_POE2E_bp)
#define POE3E_POE3_DISABLED ((0x0 & PFMPOE_POE3E_bm) << PFMPOE_POE3E_bp)
#define POE3E_POE3_ENABLED  ((0x1 & PFMPOE_POE3E_bm) << PFMPOE_POE3E_bp)
#define POE4E_POE4_DISABLED ((0x0 & PFMPOE_POE4E_bm) << PFMPOE_POE4E_bp)
#define POE4E_POE4_ENABLED  ((0x1 & PFMPOE_POE4E_bm) << PFMPOE_POE4E_bp)
#define POE5E_POE5_DISABLED ((0x0 & PFMPOE_POE5E_bm) << PFMPOE_POE5E_bp)
#define POE5E_POE5_ENABLED  ((0x1 & PFMPOE_POE5E_bm) << PFMPOE_POE5E_bp)
#define POE6E_POE6_DISABLED ((0x0 & PFMPOE_POE6E_bm) << PFMPOE_POE6E_bp)
#define POE6E_POE6_ENABLED  ((0x1 & PFMPOE_POE6E_bm) << PFMPOE_POE6E_bp)
#define POE7E_POE7_DISABLED ((0x0 & PFMPOE_POE7E_bm) << PFMPOE_POE7E_bp)
#define POE7E_POE7_ENABLED  ((0x1 & PFMPOE_POE7E_bm) << PFMPOE_POE7E_bp)

//-------- IOPORT.PFNPOE -------- 8 bit
//PFMPOE enables or disables POE input pins.
#define PFNPOE_POE8E_bp 0u
#define PFNPOE_POE8E_bm 0x1
#define PFNPOE_POE9E_bp 1u
#define PFNPOE_POE9E_bm 0x1

#define POE8E_POE8_DISABLED ((0x0 & PFMPOE_POE8E_bm) << PFMPOE_POE8E_bp)
#define POE8E_POE8_ENABLED  ((0x1 & PFMPOE_POE8E_bm) << PFMPOE_POE8E_bp)
#define POE9E_POE9_DISABLED ((0x0 & PFMPOE_POE9E_bm) << PFMPOE_POE9E_bp)
#define POE9E_POE9_ENABLED  ((0x1 & PFMPOE_POE9E_bm) << PFMPOE_POE9E_bp)


//-------- ICU.IRQCRn -------- 8 bit
#define IRQCRn_IRQMD_bp 2u
#define IRQCRn_IRQMD_bm 0x3

#define IRQMD_DETECT_LOW_LEVEL                ((0x0 & IRQCRn_IRQMD_bm) << IRQCRn_IRQMD_bp)
#define IRQMD_DETECT_FALLING_EDGE             ((0x1 & IRQCRn_IRQMD_bm) << IRQCRn_IRQMD_bp)
#define IRQMD_DETECT_RISING_EDGE              ((0x2 & IRQCRn_IRQMD_bm) << IRQCRn_IRQMD_bp)
#define IRQMD_DETECT_RISING_AND_FALLING_EDGE  ((0x3 & IRQCRn_IRQMD_bm) << IRQCRn_IRQMD_bp)

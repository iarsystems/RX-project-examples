/***********************************************************************************************************************
 * Copyright [2020-2022] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability.  No license, express or implied, to any
 * intellectual property right is granted by Renesas.  This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/** @addtogroup Renesas Electronics Corporation
 * @{
 */

/** @addtogroup R5K574M
 * @{
 */

#ifndef R5K574M_H
 #define R5K574M_H

 #ifdef __cplusplus
extern "C" {
 #endif

// #include "system.h"                   /*!< R5K574M System                                                          */

#include <stdint.h>

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */


 #ifndef __IM                          /*!< Fallback for older CMSIS versions                                         */
  #define __IM     __I
 #endif
 #ifndef __OM                          /*!< Fallback for older CMSIS versions                                         */
  #define __OM     __O
 #endif
 #ifndef __IOM                         /*!< Fallback for older CMSIS versions                                         */
  #define __IOM    __IO
 #endif

/** @addtogroup Device_Peripheral_clusters
 * @{
 */
 #include "R5K574M/r_icu_reg.h"

// #include "R5K574M/r_mram_reg.h"
 #include "R5K574M/r_port_reg.h"

// #include "R5K574M/r_rtc_reg.h"
// #include "R5K574M/r_sram_reg.h"
 #include "R5K574M/r_system_reg.h"
 #include "R5K574M/r_cmt_reg.h"        // Copy from R5F572N to R5K574M
 #include "R5K574M/r_cpscu_reg.h"      // forRX74M
 #include "R5K574M/r_pscu_reg.h"       // forRX74M

/** @} */ /* End of group Device_Peripheral_clusters */

/** @addtogroup Device_Peripheral_peripheralAddr
 * @{
 */
#if defined(_RX_PZ_NONSECURE)
 #define BASE_S_OFFSET         0U
#else
 #define BASE_S_OFFSET         (BSP_FEATURE_PZ_S_OFFSET)
#endif



 #define R_ICU_BASE       (0x07006000UL + BASE_S_OFFSET)

// #define R_MRMS_BASE       0x8709C000UL
 #define R_MSTP_BASE      (0x07803000UL + BASE_S_OFFSET)

// #define R_RTC_BASE        0x87802000UL
 #define R_PORT0_BASE     (0x07C00000UL + BASE_S_OFFSET)
 #define R_PORT1_BASE     (0x07C00020UL + BASE_S_OFFSET)
 #define R_PORT2_BASE     (0x07C00040UL + BASE_S_OFFSET)
 #define R_PORT3_BASE     (0x07C00060UL + BASE_S_OFFSET)
 #define R_PORT4_BASE     (0x07C00080UL + BASE_S_OFFSET)
 #define R_PORT5_BASE     (0x07C000A0UL + BASE_S_OFFSET)
 #define R_PORT6_BASE     (0x07C000C0UL + BASE_S_OFFSET)
 #define R_PORT7_BASE     (0x07C000E0UL + BASE_S_OFFSET)
 #define R_PORT8_BASE     (0x07C00100UL + BASE_S_OFFSET)
 #define R_PORT9_BASE     (0x07C00120UL + BASE_S_OFFSET)
 #define R_PORT10_BASE    (0x07C00140UL + BASE_S_OFFSET)
 #define R_PORT11_BASE    (0x07C00160UL + BASE_S_OFFSET)
 #define R_PORT12_BASE    (0x07C00180UL + BASE_S_OFFSET)
 #define R_PORT13_BASE    (0x07C001A0UL + BASE_S_OFFSET)
 #define R_PORT14_BASE    (0x07C001C0UL + BASE_S_OFFSET)
 #define R_PORT15_BASE    (0x07C001E0UL + BASE_S_OFFSET)
 #define R_PORT16_BASE    (0x07C00200UL + BASE_S_OFFSET)
 #define R_PFS_BASE       (0x07C00800UL + BASE_S_OFFSET)
 #define R_PMISC_BASE     (0x07C00D00UL + BASE_S_OFFSET)

// #define R_SRAM_BASE       0x8701C000UL
// #define R_SRAM_NS_BASE    0x0701C000UL
 #define R_SYSTEM_BASE    (0x07019000UL + BASE_S_OFFSET)
 #define R_CMT_BASE       (0x07826000UL + BASE_S_OFFSET)
 #define R_CPSCU_BASE     (0x07018000UL + BASE_S_OFFSET) // forRX74M FPGA
 #define R_PSCU_BASE      (0x07804000UL + BASE_S_OFFSET) // forRX74M FPGA

/** @} */ /* End of group Device_Peripheral_peripheralAddr */

/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_declaration
 * @{
 */

 #define R_ICU       ((R_ICU_Type *) R_ICU_BASE)

// #define R_MRMS       ((R_MRMS_Type *) R_MRMS_BASE)
 #define R_MSTP      ((R_MSTP_Type *) R_MSTP_BASE)
 #define R_PORT0     ((R_PORT0_Type *) R_PORT0_BASE)
 #define R_PORT1     ((R_PORT0_Type *) R_PORT1_BASE)
 #define R_PORT2     ((R_PORT0_Type *) R_PORT2_BASE)
 #define R_PORT3     ((R_PORT0_Type *) R_PORT3_BASE)
 #define R_PORT4     ((R_PORT0_Type *) R_PORT4_BASE)
 #define R_PORT5     ((R_PORT0_Type *) R_PORT5_BASE)
 #define R_PORT6     ((R_PORT0_Type *) R_PORT6_BASE)
 #define R_PORT7     ((R_PORT0_Type *) R_PORT7_BASE)
 #define R_PORT8     ((R_PORT0_Type *) R_PORT8_BASE)
 #define R_PORT9     ((R_PORT0_Type *) R_PORT9_BASE)
 #define R_PORT10    ((R_PORT0_Type *) R_PORT10_BASE)
 #define R_PORT11    ((R_PORT0_Type *) R_PORT11_BASE)
 #define R_PORT12    ((R_PORT0_Type *) R_PORT12_BASE)
 #define R_PORT13    ((R_PORT0_Type *) R_PORT13_BASE)
 #define R_PORT14    ((R_PORT0_Type *) R_PORT14_BASE)
 #define R_PORT15    ((R_PORT0_Type *) R_PORT15_BASE)
 #define R_PORT16    ((R_PORT0_Type *) R_PORT16_BASE)
 #define R_PFS       ((R_PFS_Type *) R_PFS_BASE)
 #define R_PMISC     ((R_PMISC_Type *) R_PMISC_BASE)

// #define R_RTC        ((R_RTC_Type *) R_RTC_BASE)
// #define R_SRAM       ((R_SRAM_Type *) R_SRAM_BASE)
// #define R_SRAM_NS    ((R_SRAM_NS_Type *) R_SRAM_NS_BASE)
 #define R_SYSTEM    ((R_SYSTEM_Type *) R_SYSTEM_BASE)
 #define R_CMT       ((R_CMT_Type *) R_CMT_BASE)
 #define R_CPSCU     ((R_CPSCU_Type *) R_CPSCU_BASE) // forRX74M FPGA
 #define R_PSCU      ((R_PSCU_Type *) R_PSCU_BASE)   // forRX74M FPGA

/** @} */ /* End of group Device_Peripheral_declaration */

/* =========================================================================================================================== */
/* ================                                 Pos/Mask Cluster Section                                  ================ */
/* =========================================================================================================================== */

/** @addtogroup PosMask_clusters
 * @{
 */
 #include "R5K574M/r_icu_posmsk.h"

// #include "R5K574M/r_mram_posmsk.h"
 #include "R5K574M/r_port_posmsk.h"

// #include "R5K574M/r_rtc_posmsk.h"
// #include "R5K574M/r_sram_posmsk.h"
 #include "R5K574M/r_system_posmsk.h"
 #include "R5F572N/r_cmt_posmsk.h"     // Copy from R5F572N to R5K574M

/** @} */ /* End of group PosMask_peripherals */

 #ifdef __cplusplus
}
 #endif

#endif                                 /* R5K574M_H */

/** @} */ /* End of group R5K574M */

/** @} */ /* End of group Renesas Electronics Corporation */

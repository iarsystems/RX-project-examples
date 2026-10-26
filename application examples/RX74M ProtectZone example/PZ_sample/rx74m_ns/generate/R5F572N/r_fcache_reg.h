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

#ifndef R_FCACHE_REG_H
#define R_FCACHE_REG_H

/* =========================================================================================================================== */
/* ================                                         R_FCACHE                                          ================ */
/* =========================================================================================================================== */

/**
 * @brief Flash Memory Cache (R_FCACHE)
 */

typedef struct                         /*!< (@ 0x00081000) R_FCACHE Structure                                         */
{
    union
    {
        __IOM uint16_t ROMCE;          /*!< (@ 0x00000000) ROM Cache Enable Register                                  */

        struct
        {
            __IOM uint16_t ROMCEN : 1; /*!< [0..0] ROM Cache Enable                                                   */
            uint16_t              : 15;
        } ROMCE_b;
    };
    __IM uint16_t RESERVED;

    union
    {
        __IOM uint16_t ROMCIV;         /*!< (@ 0x00000004) ROM Cache Enable Register                                  */

        struct
        {
            __IOM uint16_t ROMCIV : 1; /*!< [0..0] ROM Cache Invalidate                                               */
            uint16_t              : 15;
        } ROMCIV_b;
    };
} R_FCACHE_Type;                       /*!< Size = 6 (0x6)                                                            */

#endif /* FCACHE_REG_H */

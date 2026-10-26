/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : dtc.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines DTC transfer functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines RSK R5F563TE specific defines */
#include "rskrx63tdef.h"

/* Defines  R5F563TE port registers */
#include <iorx63t.h>

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* Declares prototypes defined in this file */
#include "dtc.h"


/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Define the data source for the DTC transfer as the ADC result register */
#define DTC_SOURCE (&S12AD.ADDR0)

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Reserve an area for the DTC vector table */
#pragma location = 0x00000400

/* Declaration of the DTC vector table */
__root uint32_t dtc_vector_table[256];

/* Reserve 16 bytes for the ADC trigger transfer data area */
uint32_t dtc_s12ad_transfer_data[4];

/* Declare the DTC transfer destination array */
uint16_t g_dtc_destination[16];

/* Declare number of transfers container variable */
const uint16_t num_transfers = sizeof(g_dtc_destination) / 2u;

/* Declare a pointer to the DTC vector table */
uint32_t * vector_table_pointer = dtc_vector_table;

/* DTC transfer count temporary storage variable */
uint16_t dtc_transfer_count = 0x0000;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Switch read function prototype declaration */
static void cb_switch_dtc(void);


/*******************************************************************************
* Function Name: Init_DTC
* Description  : This function initialises the DTC unit to perform a transfer
*                from the pointer specified as 'DTC_SOURCE', to each element of
*                the array 'g_dtc_destination' when triggered by an ADC
*                conversion. The DTC unit will transfer to an incremented
*                address until the array is full, then resets its contents to 0
*                and begin again.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_DTC (void)
{
    /* Clear the contents of the 'g_dtc_destination' array */
    if (NULL == memset(g_dtc_destination, 0u, sizeof(g_dtc_destination)))
    {
        /* Halt in while loop when errors detected */
        while (1)
        {
            /* Wait */
        }
    }

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;

    /* Cancel the DTC module clock stop mode */
    MSTP_DTC = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Ensure DTC is disabled */
    DTC.DTCST.BIT.DTCST = 0x0;

    /* Reset the transfer skip flag */
    DTC.DTCCR.BIT.RRS = 0;

    /* Use short address mode */
    DTC.DTCADMOD.BIT.SHORT = 0x1;

    /* Set the base address for calculating the DTC vector table address */
    DTC.DTCVBR = dtc_vector_table;

    /* Write to the MRA and SAR registers */
    dtc_s12ad_transfer_data[0] =
                    ((uint32_t)0x10ul << 24u) | ((uint32_t)DTC_SOURCE);

    /* Write to the MRB and DAR registers */
    dtc_s12ad_transfer_data[1] =
                    ((uint32_t)0x08ul << 24u) | ((uint32_t)g_dtc_destination);

    /* Write to the CRA and CRB registers */
    dtc_s12ad_transfer_data[2] =
                    (uint32_t)(num_transfers << 16u) & 0xffff0000u;

    /* Point to the S12AD vector in the DTC vector table */
    vector_table_pointer += VECT_S12AD_S12ADI;

    /* Store the data start address */
    *vector_table_pointer = (uint32_t)dtc_s12ad_transfer_data;

    /* Enable DTC trigger on ADC conversion end */
    ICU.DTCER[VECT_S12AD_S12ADI].BIT.DTCE = 1;

    /* Set the transfer skip flag */
    DTC.DTCCR.BIT.RRS = 1;

    /* Enable S12AD interrupt requests */
    IEN(S12AD,S12ADI) = 0x1;

    /* Configure the 12-bit S12AD unit for single scan mode */
    S12AD.ADCSR.BIT.ADCS = 0x0;

    /* Enable automatic clearing of ADDR0 register after it's been read */
    S12AD.ADCER.BIT.ACE = 0x1;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001;

    /* Enable scan end interrupts */
    S12AD.ADCSR.BIT.ADIE = 1;

    /* Configure switch callback function */
    SetSwitchReleaseCallback(cb_switch_dtc);

    /* Enable the DTC transfer */
    DTC.DTCST.BIT.DTCST = 0x1;
}
/*******************************************************************************
* End of function Init_DTC
*******************************************************************************/

/*******************************************************************************
* Function Name: cb_switch_dtc
* Description  : Switch callback function. This function checks if switch SW1
*                was pressed, and fetches the status of the DTC transfer. If
*                there are no more remaining transfer, the function clears the
*                destination array and re-enables the DTC transfers from the
*                start of the destination array. The function then triggers an
*                ADC conversion. If there are remaining transfers, it simply
*                triggers an ADC conversion then exits.
* Argument     : none
* Return value : none
*******************************************************************************/
static void cb_switch_dtc (void)
{
    /* Check if switch pressed was SW1 */
    if (g_switch_flag & SWITCHPRESS_1)
    {
        /* Fetch the remaining number of DTC transfers left */
        dtc_transfer_count = (uint16_t)(*(dtc_s12ad_transfer_data + 2) >> 16);

        /* If required number of DTC transfers are complete, clear the
           buffer & re-start DTC */
        if (0u == dtc_transfer_count)
        {
            /* Clear the contents of the g_dtc_destination array */
            if (NULL == memset(g_dtc_destination, 0u,
                                (size_t)sizeof(g_dtc_destination)))
            {
                /* Halt in while loop when errors detected */
                while (1)
                {
                    /* Halt */
                }
            }

            /* Clear the ADC interrupt flag */
            IR(S12AD,S12ADI) = 0x0;

            /* Write to the MRB and DAR registers */
            dtc_s12ad_transfer_data[1] =
                     ((uint32_t)0x08ul << 24) | ((uint32_t)g_dtc_destination);

            /* Write to the CRA and CRB registers */
            dtc_s12ad_transfer_data[2] =
                     (uint32_t)(num_transfers << 16) & 0xffff0000;

            /* Enable DTC trigger on ADC conversion end */
            ICU.DTCER[VECT_S12AD_S12ADI].BIT.DTCE = 1;

            /* Restart the DTC */
            DTC.DTCST.BIT.DTCST = 1;
        }

        /* Toggle LED0 once the DTC completes transferring 16 bytes */
        LED0 = (uint8_t)~LED0;

        /* Start AD conversion */
        S12AD.ADCSR.BIT.ADST = 0x1;
    }

    /* Clear switch press flags */
    g_switch_flag &= ~SWITCHPRESS_ALL;
}
/*******************************************************************************
* End of function cb_switch_dtc
*******************************************************************************/


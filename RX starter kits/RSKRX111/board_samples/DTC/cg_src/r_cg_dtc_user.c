/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software.  By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_dtc_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for DTC module.
* Creation Date: 2013-08-16
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_dtc.h"
/* Start user code for include. Do not edit comment generated here */
#include "stdint.h"
#include <string.h>
#include "r_cg_icu.h"
#include "switch.h"
#include "lcd.h"
#include "r_cg_port.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
void displayDestinationArray(void);

/* 16 bit integer to character string function prototype declaration */
static void uint16_to_string(uint8_t * const output_string, uint8_t pos,
                                const uint16_t input_number);

/* Declaration of the DTC vector table */
#pragma location=0x00001000
__root __no_init uint32_t dtc_vector_table[256];

/* Reserve 16 bytes for the ADC trigger transfer data area */
uint32_t dtc_s12ad_transfer_data[4];

/* Declare the DTC transfer destination array */
#pragma location=0x00001400
__root __no_init uint16_t g_dtc_destination[16];

/* Declare number of transfers container variable */
const uint16_t num_transfers = sizeof(g_dtc_destination) / 2u;

/* Declare a pointer to the DTC vector table */
uint32_t * vector_table_pointer = dtc_vector_table;

/* DTC transfer count temporary storage variable */
uint16_t dtc_transfer_count = 0x0000;

static void cb_switch_dtc(void);
/* End user code. Do not edit comment generated here */


/* Start user code for adding. Do not edit comment generated here */
/*******************************************************************************
* Function Name: Init_DTC
* Description  : This function clears the destination buffer and sets the
*              : switch callback function to the cb_switch_dtc function
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

    /* Ensure that the DTC is set to the ADC Channel 0 result as the source */
    dtc_transferdata0.mra_sar = ((uint32_t)(_00_DTC_SRC_ADDRESS_FIXED |
                                            _10_DTC_TRANSFER_SIZE_16BIT |
                                            _00_DTC_TRANSFER_MODE_NORMAL) << 24U) |
                                            ((uint32_t)(&(S12AD.ADDR0)) & 0x00FFFFFF);

    /* Configure switch callback function */
    SetSwitchReleaseCallback(cb_switch_dtc);

    R_DTC0_Start();
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
        dtc_transfer_count = (uint16_t)((dtc_transferdata0.cra_crb) >> 16);

        /* If required number of DTC transfers are complete, clear the
           buffer & re-start DTC */
        if (0u == dtc_transfer_count)
        {
            /* Clear the contents of the g_dtc_destination array */
            if (memset(g_dtc_destination, 0u,(size_t)sizeof(g_dtc_destination)) == NULL)
            {
                /* Halt in while loop when errors detected */
                while (1)
                {
                    /* Halt */
                }
            }

            /* Clear the ADC interrupt flag */
            //IR(S12AD,S12ADIO) = 0x0;

            /* Write to the MRB and DAR registers */
            dtc_transferdata0.mrb_dar =
                     ((uint32_t)0x08ul << 24) | ((uint32_t)g_dtc_destination);

            /* Write to the CRA and CRB registers */
            dtc_transferdata0.cra_crb =
                     (uint32_t)(num_transfers << 16) & 0xffff0000;

            /* Enable DTC trigger on ADC conversion end */
            ICU.DTCER[102].BIT.DTCE = 1;

            /* Restart the DTC */
            DTC.DTCST.BIT.DTCST = 1;
        }

        /* Toggle LED0 once the DTC completes transferring 16 bytes */
        LED0_Toggle();

        /* Start AD conversion */
        S12AD.ADCSR.BIT.ADST = 0x1;
    }

    /* Clear switch press flags */
    g_switch_flag &= ~SWITCHPRESS_ALL;
}
/*******************************************************************************
* End of function cb_switch_dtc
*******************************************************************************/

/*******************************************************************************
* Function Name: displayDestinationArray
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
void displayDestinationArray (void)
{
    uint8_t dispArray[] = "                    ";
    uint8_t ctr = 0;

    /*display each line of the array elements in turn on the LCD */
    for (ctr = 0; ctr < 4; ctr++)
    {
        /* add relevant element from g_dtc_destination 0-3 */
        uint16_to_string(dispArray, 0, g_dtc_destination[ctr]);

        /* add relevant element from g_dtc_destination 4-7 */
        uint16_to_string(dispArray, 5, g_dtc_destination[ctr+4]);

        /* add relevant element from g_dtc_destination 8-11 */
        uint16_to_string(dispArray, 10, g_dtc_destination[ctr+8]);

        /* add relevant element from g_dtc_destination 12-15 */
        uint16_to_string(dispArray, 15, g_dtc_destination[ctr+12]);

        /* update display with current row */
        Display_LCD((uint8_t)(ctr+4), dispArray);
    }

}
/*******************************************************************************
* End of function displayDestinationArray
*******************************************************************************/

/*******************************************************************************
* Function Name: uint16_to_string
* Description  : Function converts a 16 bit integer into a character string,
*                inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to char array that will hold
*                                  character string.
*                            pos : uint8_t number, element number to begin
*                                  inserting the character string from (offset).
*                   input_number : 16 bit integer to convert into a string.
* Return value : none
* Note         : No input validation is used, so output data can overflow the
*                array passed.
*******************************************************************************/
static void uint16_to_string (uint8_t * const output_string, uint8_t pos,
                                const uint16_t input_number)
{
    /* Declare temporary character storage variable, and bit_shift variable */
    uint8_t  a         = 0x00;
    uint8_t  bit_shift = 12u;

    /* Declare 16bit mask variable */
    uint16_t mask      = 0xF000;

    /* Loop through until each hex digit is converted to an ASCII character */
    while (bit_shift < 30u)
    {

        /* Mask and shift the hex digit, and store in temporary variable, a */
        a = (uint8_t)((input_number & mask) >> bit_shift);

        /* Convert the hex digit into an ASCII character, and store in output
           string */
        output_string[pos] = (uint8_t)((a < 0x0A) ? (a + 0x30) : (a + 0x37));

        /* Shift the bit mask 4 bits to the right, to convert the next digit */
        mask = (uint16_t) (mask >> 4u);

        /* Decrement the bit_shift counter by 4 (bits in a each digit) */
        bit_shift -= 4u;

        /* Increment the output string location */
        pos++;
    }
}
/*******************************************************************************
* End of function uint16_to_string
*******************************************************************************/
/* End user code. Do not edit comment generated here */

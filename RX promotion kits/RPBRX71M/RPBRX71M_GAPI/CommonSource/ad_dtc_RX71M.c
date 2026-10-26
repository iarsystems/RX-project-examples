/* Adapted to IAR Embedded Workbench */
/******************************************************************************
 * DISCLAIMER

 * This software is supplied by Renesas Electronics Corp. and is only
 * intended for use with Renesas products. No other uses are authorized.

 * This software is owned by Renesas Electronics Corp. and is protected under
 * all applicable laws, including copyright laws.

 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
 * REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY
 * DISCLAIMED.

 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
 * FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS
 * AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

 * Renesas reserves the right, without notice, to make changes to this
 * software and to discontinue the availability of this software.
 * By using this software, you agree to the additional terms and
 * conditions found by accessing the following link:
 * http://www.renesas.com/disclaimer
 ******************************************************************************/
/************************************************************************
 Revision History
 DD.MM.YYYY OSO-UID Description
 02.28.2007 RTA-PMM First Release
 ************************************************************************/
#include "dtc_vect_RX.h"

#include "stdint.h"
#include "stdio.h"
#include "Global.h"
/* allocate DTC vector and table */
#include "machine.h"

#include "ad_dtc_RX71M.h"

#pragma location=".DTC_VECT"
__root dtc_vect_table_t *dtc_vectors[256]; /* start of DTC vector table */

#pragma location=".DTC_TABLE"
__root dtc_vect_table_t ad_dtc_table;

/* return to normal section definition */

/* allocate buffer for 8 samples on each A/D channel */
volatile uint16_t AD_Buffer[8][8];

#define CAPTURE_VAL 0xfff

/*******************************************************************************
 * Function Name: ad_init
 * Description  : Initialises ADC and DTC to interface with the touch-panel
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
void ad_init (void)
{
    DTC.DTCCR.BIT.RRS = 1;

    /* configure A/D Converter */
    /* ADC Clock ADCLK = PCLKB = 60MHZ */
    S12AD.ADCSR.WORD = 0;
    S12AD.ADCSR.BIT.ADIE = 1; /* enable A/D interrupts - feeds ICU */
    S12AD.ADCSR.BIT.ADCS = 3; /* Continuous scan mode*/

    S12AD.ADANSA0.WORD = 0x0F; /* Enable sampling of channels 0..7 */

    S12AD.ADADS0.WORD = 0x0F; /* Configure channels 0..7 for addition/average mode */
    S12AD.ADADC.BYTE = 3; /* Configure for 4x addition */

    S12AD.ADCSR.BIT.ADST = 1; /* Start conversions */

    ad_dtc_table.SAR = (void *) &S12AD.ADDR0;
    ad_dtc_table.DAR = (void *) AD_Buffer;
    ad_dtc_table.MR.LONG = 0;
    ad_dtc_table.MR.BIT.MRA_MD = 2;  // block transfer
    ad_dtc_table.MR.BIT.MRA_SZ = 1;  // word transfer size (less bus cycles)
    ad_dtc_table.MR.BIT.MRA_SM = 2;  // increment source address
    ad_dtc_table.MR.BIT.MRB_DM = 2;  // increment destination address
    ad_dtc_table.MR.BIT.MRB_DTS = 1; // source side is block source
    ad_dtc_table.CR.CRA = 0x0808;            // 8 words (16 bytes) per transfer
    ad_dtc_table.CR.CRB = size_ra(AD_Buffer);  // number of block transfers

    dtc_vectors[VECT_S12ADC0_S12ADI0] = &ad_dtc_table;

    DTCE(S12ADC0,S12ADI0)= 1;
    IEN(S12ADC0,S12ADI0)= 1;
}
/*******************************************************************************
 End of function ad_init
 *******************************************************************************/

/*******************************************************************************
 * Function Name: ad_acquire
 * Description  : Initialises the ADC module for continuous conversions. The
 *                results are transferred to storage buffers by the DTC module.
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
void ad_acquire (void)
{
    ad_dtc_table.DAR = (void *) AD_Buffer;
    ad_dtc_table.CR.CRB = size_ra(AD_Buffer); /* number of block transfers */
    DTCE(S12ADC0,S12ADI0)= 1;
    nop();
    IR(S12ADC0,S12ADI0)= 0;

    /* RTOS_USAGE */
    vTaskDelay(1L);

    /* Free up to another task while the the DTC transfers the ADC values in */
}
/*******************************************************************************
 End of function ad_acquire
 *******************************************************************************/

/*******************************************************************************
 * Function Name: ad_value
 * Description  : Initialises the ADC module for continuous conversions. The
 *                results are transferred to storage buffers by the DTC module.
 * Arguments    : int
 *                  channel: ADC channel.
 * Return Value : int
 *                  value: ADC channel's average conversion result.
 *******************************************************************************/
uint16_t ad_value (int channel)
{
    int16_t index;
    uint32_t value;

    if (channel >= size_ra(AD_Buffer[1]))
    {
        return (0);
    }

    /* sum values */
    for (index = 0, value = 0; index < size_ra(AD_Buffer); index++)
    {
        value += AD_Buffer[index][channel];
    }

    /* shift data */
    value /= 64;

    return ((uint16_t) value);
}
/*******************************************************************************
 End of function ad_value
 *******************************************************************************/


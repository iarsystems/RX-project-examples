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
* File Name    : r_cg_main.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements main function.
* Creation Date: 2013-08-26
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
#include "r_cg_cgc.h"
#include "r_cg_icu.h"
#include "r_cg_port.h"
#include "r_cg_cmt.h"
#include "r_cg_sci.h"
#include "r_cg_s12adb.h"
/* Start user code for include. Do not edit comment generated here */
/* Following header file provides string type definitions. */
#include <string.h>
#include "lcd.h"
#include "utility.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

/* Master header transmission function prototype */
static void lin_receive_transmit_header(void);

/* Send ADC value back to master function prototype declaration */
static void lin_slave_reply_adc(const uint16_t adc);

/* Flag to indicate when ADC data has been received from the slave */
volatile uint8_t g_ad_received = 0;

/* Variable that stores the bit period, in us, found by measurement */
static uint16_t measured_bit_period_us;

/* End user code. Do not edit comment generated here */


/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint32_t OFS0_location = 0xFFFFFFFFUL;
#pragma location = "OPTBYTE"
__root const uint32_t OFS1_location = 0xFFFFFF7FUL;


void R_MAIN_UserInit(void);
/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */

    /* Display sample name banner */
    Display_LCD(0, (uint8_t *)"LIN Slave");

    /* Initialise SCI12 extended mode for LIN Slave break detection and bit period measurement */
    R_SCI12_LIN_Slave_Mode();

    /* Start the SCI12 UART module */
    R_SCI12_Start();

    while (1U)
    {
        uint8_t id;
        char    lcd_buffer[9] = "=H\'WXYZ\0";

        /* Receive the LIN transmit Header from the LIN Master */
        lin_receive_transmit_header();

        /* Call the SCI12 API to receive the id byte from the master */
        R_SCI12_Serial_Receive(&id, 1);

        while (0 == g_id_received)
        {
            /* Wait - the SCI12 interrupt handler will receive an
               id byte and the r_sci12_callback_receive end will set
               g_id_received */
        }

        /* reset the flag */
        g_id_received = 0;

        if (0x10 == id)
        {
            uint16_t adc_result;

            /* Start the ADC module */
            R_S12ADb_Start();

            /* Wait for completion then read the result */
            while (1 == S12AD.ADCSR.BIT.ADST)
            {
                /* Wait */
            }

            R_S12ADb_Get_ValueResult(ADCHANNEL0, &adc_result);

            /* Send it to the master */
            lin_slave_reply_adc(adc_result);

            /* Convert ADC result into a character string, and store in the
               local string lcd_buffer */
            uint16_to_string(lcd_buffer, 3, adc_result);

             /* Display the contents of the local string lcd_buffer */
            Display_LCD(1, (uint8_t *)lcd_buffer);
        }
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    Init_LCD();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/*******************************************************************************
* Function Name:  lin_receive_transmit_header
* Description  :  Receives the SYNC break, counts edges on the SYNC byte to
*                 determine master transmission baud rate, sets the reception
*                 baud rate, then enables SCI12 receive interrupts.
* Parameters   :  none
* Return value :  none
*******************************************************************************/
static void lin_receive_transmit_header (void)
{
    uint16_t m;
    uint8_t  i;
    uint16_t pclks_per_bit;

    /* The following set of register writes configure the SCI12 timer to detect
       the LIN break - Maximum LIN bit rate is 20,000 bps, which makes the
       minimum length of a break signal = 650 us. So we set our timer at 500 us
       and let it underflow during break detection */
    /* Set up for LIN Break Field - low width determination mode */
    SCI12.TMR.BIT.TOMS = 0x1;

    /* Set up the clock source for PCLK/1 at 24MHz*/
    SCI12.TMR.BIT.TCSS = 0x0;

    /* Set up the for Timer 500 us.  500us / 41.66 ns gives 12000 clock periods
       or 120 * 100 */
    SCI12.TPRE = 120u - 1u;
    SCI12.TCNT = 100u - 1u;

    /* Enable break field detection */
    SCI12.CR1.BIT.BFE = 0x1;
    SCI12.ICR.BIT.BFDIE = 0x1;

    /* Disable reception - we need to do this to prevent the UART from receiving
       SYNC break and SYNC which will causes framing errors in the UART */
    SCI12.SCR.BIT.RE = 0x0;

    /* First clear the STR register */
    SCI12.STCR.BYTE = 0x3F;

    /* Start the SCI12 counter */
    SCI12.TCR.BIT.TCST = 0x1;

    /* Wait for the Break Field Low Width Detection Flag to be set - which
       indicates the timer has underflowed because a break of > 500 us has
       been detected */
    while (0x0 == SCI12.STR.BIT.BFDF)
    {
        /* Wait */
    }

    /* Clear the BFDF in the STR register */
    SCI12.STCR.BIT.BFDCL = 0x1;

    /* Stop the timer counting */
    SCI12.TCR.BIT.TCST = 0;

    /* Next we expect the SYNC byte so we start the baud rate detection */
    /* Use 0xFFFF as the counter reload values */
    SCI12.TPRE = 0xFF;
    SCI12.TCNT = 0xFF;

    /* Clear the AEDF bit */
    SCI12.STCR.BIT.AEDCL = 0x1;

    /* Enable bit rate measurement in SCI12 CR0 */
    SCI12.CR0.BIT.BRME = 0x01;

    /* Need to change timer mode from break field low measurement back
       to timer mode */
    SCI12.TMR.BIT.TOMS = 0x0;

    /* Start the timer */
    SCI12.TCR.BIT.TCST = 0x1;

    /* Wait for the first edge - this will be the rising edge
       corresponding to the end of the SYNC break */
    while (0x0 == SCI12.STR.BIT.AEDF)
    {
        /* Wait */
    }

    /* Clear the AEDF bit */
    SCI12.STCR.BIT.AEDCL = 0x1;

    /* Get the values from TCT and TPRE */
    /* Cast required to stop complier warning: Precision lost */
    m = (uint16_t)((SCI12.TCNT << 8) + SCI12.TPRE);

    /* Measurement starts when the break signal ends and the RXDX12 pin
       goes high. Wait for first edge - this will be the falling edge
       corresponding to the SYNC byte start bit */
    while (0x0 == SCI12.STR.BIT.AEDF)
    {
        /* Wait */
    }

    /* Clear the AEDF bit */
    SCI12.STCR.BIT.AEDCL = 0x1;

    /* Get the values from TCNT and TPRE */
    /* Cast required to stop complier warning: Precision lost */
    m = (uint16_t)((SCI12.TCNT << 8) + SCI12.TPRE);

    /* Next we expect the 9 equal bit periods coressponding to the SYNC
       byte plus start bit.  We set our measurement to 0 then wait for the 9
       edges, accumulating the measured time as we go */
    m = 0;

    for (i = 0; i < 9; i++)
    {
        /* Wait for the next edge - this will be 1 bit period */
        while (0x0 == SCI12.STR.BIT.AEDF)
        {
            /* Wait */
        }

        /* Clear the AEDF bit */
        SCI12.STCR.BIT.AEDCL = 0x1;

        /* Get the values from TCNT and TPRE - measures the length of the start
            bit */
        /* Cast required to stop complier warning: Precision lost */
        m += (uint16_t) ~((SCI12.TCNT << 8) + SCI12.TPRE);
    }

    /* Stop the timer */
    SCI12.TCR.BIT.TCST = 0x0;

    /* Disable bit rate measurement in SCI12 CR0 */
    SCI12.CR0.BIT.BRME = 0x0;

    /* In the next few lines we calculate and set the baud rate */
    /* First work out the number of pclk ticks per bit period */
    pclks_per_bit = (uint16_t)(m / 9u);

    /* Calculate and store the bit period in uS for later use */
    measured_bit_period_us = (uint16_t)(pclks_per_bit / 24u);

    /* PCLK = 24MHz, Set baud rate to BAUD_RATE bps, x is value we put in BRR
       register, x = 24MHz / (64*0.5*BAUD_RATE)-1 */
    /* But we have already worked out our baud rate in terms of pclks_per_bit
        i.e. BAUD_RATE = 1/(pclks_per_bit * (1/PCLK)) = PCLK/pclks_per_bit
        Subst. this into above equation gives: x = (pclks_per_bit/32) - 1 */
    SCI12.BRR = (uint8_t)((pclks_per_bit / 32u) - 1);
    SCI12.BRR = 0x26;

    /* Allow a small delay */
    for (i = 0; i < 20; i++)
    {
        /* delay */
    }

    /* Clear bits ORER, PER and FER */
    SCI12.SSR.BYTE &= 0xC7u;

}

/*******************************************************************************
* End of function lin_receive_transmit_header
*******************************************************************************/

/*******************************************************************************
* Function Name : lin_slave_reply_adc
* Description   : Sends the ADC value and checksum to the LIN master
* Parameters    : adc - uint16_t contains the ADC value to send to the master
* Return value  : none
*******************************************************************************/
static void lin_slave_reply_adc (const uint16_t adc)
{
    uint16_t cs;
    uint8_t  tx_byte;

    /* Disable reception - we need to do this to prevent the UART from receiving
       on the bidirectional LIN line when we transmit */
    SCI12.SCR.BIT.RE = 0x0;

    /* Wait for 1 bit period delay */
    R_Microsec_Delay(measured_bit_period_us);

   /* Send the MSB of the adc result */
    tx_byte = (uint8_t)((adc & 0xFF00) >> 8);;
    R_SCI12_Serial_Send(&tx_byte, 1);

    /* Wait for transmission complete */
    while (0x0 == g_tx12_done)
    {
        /* Wait */
    }

    /* Sent it, reset the flag */
    g_tx12_done = 0;

    /* Wait for 1 bit period delay */
    R_Microsec_Delay(measured_bit_period_us);

    /* Send the LSB of the adc result */
    tx_byte = (uint8_t)(adc & 0x00FF);
    R_SCI12_Serial_Send(&tx_byte, 1);

    /* Wait for transmission complete */
    while (0x0 == g_tx12_done)
    {
        /* Wait */
    }

    /* Sent it, reset the flag */
    g_tx12_done = 0;

    /* Wait for 1 bit period delay */
    R_Microsec_Delay(measured_bit_period_us);

    /* Calculate the checksum and send it */
    cs = (uint16_t)(((adc & 0xFF00) >> 8) + (adc & 0x00FF));
    tx_byte = (uint8_t) ~cs;

    R_SCI12_Serial_Send(&tx_byte, 1);

    /* Wait for transmission complete */
    while (0x0 == g_tx12_done)
    {
        /* Wait */
    }

    /* Sent it, reset the flag */
    g_tx12_done = 0;

    /* Status Flag Clear */
    SCI12.STCR.BYTE = 0x3F;

}
/*******************************************************************************
* End of function lin_slave_reply_adc
*******************************************************************************/

/* End user code. Do not edit comment generated here */

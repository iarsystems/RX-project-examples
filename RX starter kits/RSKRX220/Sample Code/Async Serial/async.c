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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : async.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines SCI functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/

/* Defines standard boolean variable types used in this file */
#include <stdbool.h>

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Declares functions defined in this file */
#include "async.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Declaration of the SCI status flag global variable */
volatile bool gSCI_Flag = true;

/* Declare transmit and receive buffer variables used in this file */
char g_sci_tx_buffer[2] = {0x2Fu, 0x0};

/* Declare a receive buffer variable */
volatile char gSCI_RXBuffer;

/* Declaration of the command string to clear the terminal screen */
const char cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0};

/* Declaration of the command string to bring the cursor home on the terminal */
const char cmd_cur_home[] = {27,'[','H',0};

/* Declare a variable used to select a oneshot or priodic timer */
uint8_t g_timer_mode_flag = 0x0;

/* Declare a variable used to count the number of elapsed timer periods */
volatile uint32_t g_periodic_delay;

/* Declare a variable to store the global count value */
volatile uint32_t g_delay_counter = 0x0;

/* Periodic Delay end callback function */
void (*g_periodic_delay_callback)(void) = NULL;


/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* init_timer function prototype */
static void init_timer (void);

/* transmit_async function prototype */
static void transmit_async (void);

/* Timer_Delay function prototype */
static void timer_delay (const uint16_t user_delay, const uint8_t timer_mode,
                                                  void( *const callback)(void));

/* text_write function prptotype */
static void text_write (const char * const msg_string);

/*******************************************************************************
* Function Name: init_async
* Description  : This function initialises the SCI channel connected to the
*                RS232 connector on the RSK. The channel is configured for
*                transmission and reception, and instructions are sent to the
*                terminal.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_async (void)
{
    /* Initialise CMT2 */
    init_timer();

    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the SCI1 module clock stop mode */
    MSTP_SCI1 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Configure SCI1's TXD1 and SCK1 pins for peripheral functions */
    PORT2.PMR.BIT.B6 = 0x1u;
    PORT2.PMR.BIT.B7 = 0x1u;

    /* Configure SCI1's RXD1 pin for peripheral functions */
    PORT3.PMR.BIT.B0 = 0x1u;

    /* PWPR.PFSWE write protect off */
    MPC.PWPR.BYTE = 0x00;

    /* PFS register write protect off */
    MPC.PWPR.BYTE = PFSPROTECTOFF;

    /* Configure SCI1's communication pins */
    /* Set Port2's pin 6 function to TXD1 */
    MPC.P26PFS.BYTE = 0x0Au;

    /* Set Port3's pin 0 function to RXD1 */
    MPC.P30PFS.BYTE = 0x0Au;

    /* Set Port2's pin 7 function to SCK1 */
    MPC.P27PFS.BYTE = 0x0Au;

    /* Turn on PFS register write protect */
    MPC.PWPR.BYTE = PFSPROTECTON;

    /* Disable All bits and CKE0,1 = 00 */
    SCI1.SCR.BYTE = 0x00;

    /* Set output port TXD0(P26) */
    PORT2.PDR.BIT.B6 = 0x1u;

    /* Set Receive port RXD0(P30)*/
    PORT3.PDR.BIT.B0 = 0x1u;

    /* disable All bits and CKE0,1 = 00 once more */
    SCI1.SCR.BYTE = 0x00;

    /* CKS[0-1] = 00: PCLK 0 div
       1Sop 8bits non-parity     */
    SCI1.SMR.BYTE = 0x00;

    /* SMIF = 0: Serial  */
    SCI1.SCMR.BYTE = 0xf2u;

    /* PCLK = 20MHz, Set baudrate to 19200 bps
       32 = 20MHz / (64*0.5*19200)-1 */
    SCI1.BRR = 32u;

    /* Declare a loop count variable */
    volatile uint16_t i = 0;

    /* Allow a small delay */
    for (i = 0; i < 20; i++)
    {
        /* Do nothing until the delay has finished */
    }

    /* Clear bits ORER, PER and FER */
    SCI1.SSR.BYTE &= 0xC7u;

    /* Set SCI1 RXI1 interrupt priority level to 7 */
    IPR(SCI1,RXI1) = 0x7u;

    /* Enable SCI1 RXI1 interrupts */
    IEN(SCI1,RXI1) = 0x1u;

    /* Clear SCI1 RXI1 interrupt flag */
    IR(SCI1,RXI1) = 0x0;

    /* Set bits TIE, RIE, RE and TE. Enable receive interrupts */
    SCI1.SCR.BYTE = 0xF0u;

    /* Clear the text on terminal window */
    text_write(cmd_clr_scr);
    text_write(cmd_cur_home);

    /* Display splash screen on terminal window */
    text_write("Renesas RX220 Async Serial \n\r");

    /* Inform user on how to stop transmission */
    text_write("Press 'z' to stop and any key to resume\r\n\n");

    /* Initialise the transmit buffer array to end in a NULL character */
    g_sci_tx_buffer[1] = 0x00;

    /* Start periodic CMT2 count */
    timer_delay(200u, PERIODIC_MODE, transmit_async);
}
/*******************************************************************************
* End of function init_async
*******************************************************************************/

/*******************************************************************************
* Function Name: init_timer
* Description  : This function initialises the CMT channel 2.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void init_timer (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Set the CMT2 interrupt level to 4 */
    IPR(CMT2,CMI2) = 0x4u;

    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1u;

    /* clear CMT2 interrupt request flag */
    IR(CMT2,CMI2) = 0x0;
}
/*******************************************************************************
* End of function init_timer
*******************************************************************************/

/*******************************************************************************
* Function Name: transmit_async
* Description  : Sends incrementing numbers across the UART link to the
*                terminal window.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void transmit_async (void)
{
    /* Check SCI status flag */
    if (gSCI_Flag)
    {
        /* Increment number in transmit buffer */
        g_sci_tx_buffer[0]++;

        /* Check if hex value of transmit buffer is greater than 0x39 (9) */
        if (g_sci_tx_buffer[0] > 0x39)
        {
            /* Insert a new line and return the cursor left */
            text_write("\r\n");

            /* Reset the ASCII character in the transmit buffer back to 0 */
            g_sci_tx_buffer[0] = 0x30u;
        }

        /* Send the transmit buffer to the terminal */
        text_write(g_sci_tx_buffer);

        /* Turn on LED0 */
        LED0 = LED_ON;

        /* Turn off LED1 */
        LED1 = LED_OFF;
    }
}
/******************************************************************************
* End of function transmit_async
******************************************************************************/

/******************************************************************************
* Function Name: timer_delay
* Description  : Function used to create delays in milliseconds for controlling
*                the debug LCD function's calls.
* Arguments    : (const uint32_t)user_delay - Timer length in milliseconds.
*                (uint8_t)timer_mode        - Sets timer to run continuously
*                                             and start a callback on timeout
*                                             (PERIODIC_MODE macro) or run once
*                                             and return after delay (0).
*                (void(*))callback          - Callback function for periodic
*                                             timer.
* Return Value : none
******************************************************************************/
static void timer_delay (const uint16_t user_delay, const uint8_t timer_mode,
                                                  void( *const callback)(void))
{
    /* Clear the timer's count */
    g_delay_counter = 0x0;

    /* Select the PCLK clock division as PCLK/32 = 625KHz */
    CMT2.CMCR.BIT.CKS = 0x1u;

    /* Store a copy of the user delay value to g_periodic_delay */
    g_periodic_delay = user_delay;

    /* Set the callback */
    if (callback)
    {
        g_periodic_delay_callback = callback;
    }

    /* Specify the timer period (1ms) */
    CMT2.CMCOR = 625u;

    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 0x1u;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 0x1u;

    /* Skip the following instructions if a periodic timer is required */
    if (PERIODIC_MODE != timer_mode)
    {
        /* Wait for the timer to timeout */
        while ((g_delay_counter != g_periodic_delay))
        {
            /* Wait */
        }

        /* Stop CMT2 count */
        CMT.CMSTR1.BIT.STR2 = 0x0;
    }
}
/******************************************************************************
* End of function timer_delay
******************************************************************************/

/******************************************************************************
* Function Name: Excep_CMTU1_CMT2_isr
* Description  : CMT2 interrupt handler.
* Arguments    : none
* Return Value : none
******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMTU1_CMT2_isr(void)
{
    /* Increment counter */
    g_delay_counter++;

    /* If period has been reached and periodic mode is set,
       run the callback and reset the counter */
    if ((g_periodic_delay < g_delay_counter) && g_periodic_delay_callback)
    {
        g_delay_counter = 0x0;

        /* This was checked to be not NULL above */
        g_periodic_delay_callback();
    }

}
/******************************************************************************
* End of function Excep_CMTU1_CMT2_isr
******************************************************************************/

/*******************************************************************************
* Function Name: text_write
* Description  : Transmits null-terminated string.
* Arguments    : (char *)msg_string - null terminated string
* Return Value : None
*******************************************************************************/
static void text_write (const char * const msg_string)
{
    /* Declare a count variable */
    uint8_t  i = 0x0;

    /* Declare a count variable */
    uint16_t j = 0x0;

    /* This loop reads the text string and puts it into the UART's
       transmit buffer */
    for (i = 0; msg_string[i]; i++)
    {
        /* Check that the last transmission was completed */
        while (0x0 == SCI1.SSR.BIT.TEND)
        {
        }

        /* Transmit the next character */
        SCI1.TDR = (unsigned char)msg_string[i];

        /* Count up to 1000 */
        for (j = 0; j < 1000u; j++)
        {
            /* delay */
        }
    }
}
/*******************************************************************************
* End of function text_write
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_SCI1_RXI1_isr
* Description  : SCI1 receive interrupt handler, called when a character is
*                received from the terminal program. Sets rx_flag if character
*                is 'z' entered.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_SCI1_RXI1
__interrupt  void Excep_SCI1_RXI1_isr(void)
{
    /* Store the received character */
    gSCI_RXBuffer = (char)SCI1.RDR;

    /* Check if last received character was 'z' */
    if ('z' == gSCI_RXBuffer)
    {
        /* Set SCI status flag to 0 */
        gSCI_Flag = false;

        /* Turn off LED0 */
        LED0 = LED_OFF;

        /* Turn on LED1 */
        LED1 = LED_ON;
    }

    /* Check if the SCI status flag is 0, and the last received character
       was not 'z' */
    else if (!gSCI_Flag)
    {
        /* Set SCI status flag to 1 */
        gSCI_Flag = true;
    }
    else
    {
        /* Do nothing */
    }
}
/*******************************************************************************
* End of function Excep_SCI1_RXI1_isr
*******************************************************************************/


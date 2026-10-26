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
* File Name     : async.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines ADC functions used for 12bit S12AD unit.
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
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
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Defines macros relating to the R5F563TE user LEDs and switches */
#include "rskrx63tdef.h"

/* Declares functions defined in this file */
#include "async.h"

/*******************************************************************************
User Defines
*******************************************************************************/
/* Define a macro to indicate selection of a periodic timer mode */
#define PERIODIC_MODE 0x1

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declaration of the SCI status flag global variable */
volatile bool g_sci_flag = true;

/* Declare transmit and receive buffer variables used in this file */
char g_sci_tx_buffer[2] = {0x2F, 0x0};

/* Declare a receive buffer variable */
volatile char g_sci_rx_buffer;

/* Declaration of the command string to clear the terminal screen */
const char cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0};

/* Declare a variable used to select a oneshot or priodic timer */
uint8_t g_timer_mode_flag = 0;

/* Declare a variable used to count the number of elapsed timer periods */
volatile uint32_t g_periodic_delay;

/* Declare a variable to store the global count value */
volatile uint32_t g_delay_counter = 0;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* init_timer function prototype */
static void init_timer(void);

/* transmit_async function prototype */
static void transmit_async(void);

/* timer_delay function prototype */
static void timer_delay(const uint32_t user_delay, const uint8_t unit,
                            const uint8_t timer_mode);

/* text_write function prptotype */
static void text_write(const char * const msg_string);

/*******************************************************************************
* Function Name: Init_Async
* Description  : This function initialises the SCI channel connected to the
*                RS232 connector on the RSK. The channel is configured for
*                transmission and reception, and instructions are sent to the
*                terminal.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_Async (void)
{
    /* Initialise CMT2 */
    init_timer();

    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the SCI1 module clock stop mode */
    MSTP_SCI1 = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Configure SCI1's TXD0, RXD0 and SCK1 pins for peripheral functions */
    PORTD.PMR.BIT.B3 = 1;
    PORTD.PMR.BIT.B4 = 1;
    PORTD.PMR.BIT.B5 = 1;

    /* PWPR.PFSWE write protect off */
    MPC.PWPR.BYTE = 0x00;

    /* PFS register write protect off */
    MPC.PWPR.BYTE = 0x40;

    /* Configure SCI0's communication pins */
    /* Set PortD's pin 4 function to SCK1 */
    MPC.PD4PFS.BYTE = 0x0A;

    /* Set PortD's pin 5 function to RXD1 */
    MPC.PD5PFS.BYTE = 0x0A;

    /* Set PortD's pin 3 function to TXD1 */
    MPC.PD3PFS.BYTE = 0x0A;

    /* Turn on PFS register write protect */
    MPC.PWPR.BYTE = 0x80;

    /* Disable All bits and CKE0,1 = 00 */
    SCI1.SCR.BYTE = 0x00;

    /* Set output port TXD1(PD3) - port direction output */
    PORTD.PDR.BIT.B3 = 1;

    /* Set Receive port RXD1(PD5) - port direction input*/
    PORTD.PDR.BIT.B5 = 0;

    /* disable All bits and CKE0,1 = 00 once more */
    SCI1.SCR.BYTE = 0x00;

    /* CKS[0-1] = 00: PCLK 0 div
       1Sop 8bits non-parity     */
    SCI1.SMR.BYTE = 0x00;

    /* SMIF = 0: Serial  */
    SCI1.SCMR.BYTE = 0xf2;

    /* PCLK = 48MHz, Set baudrate to 19200 bps
       77 = 48MHz / (64*0.5*19200)-1 */
    SCI1.BRR = 77;

    /* Allow a small delay */
    for (uint8_t i = 0; i < 20; i++)
    {
        /* delay */
    }

    /* Clear bits ORER, PER and FER */
    SCI1.SSR.BYTE &= 0xC7u;

    /* Set SCI0 RXI0 interrupt priority level to 7 */
    IPR(SCI1,RXI1) = 0x7;

    /* Enable SCI0 RXI0 interrupts */
    IEN(SCI1,RXI1) = 0x1;

    /* Clear SCI0 RXI0 interrupt flag */
    IR(SCI1,RXI1) = 0x0;

    /* Set bits TIE, RIE, RE and TE. Enable receive interrupts */
    SCI1.SCR.BYTE = 0xF0;

    /* Clear the text on terminal window */
    text_write(cmd_clr_scr);

    /* Display splash screen on terminal window */
    text_write("Renesas RX63T Async Serial \n\r");

    /* Inform user on how to stop transmission */
    text_write("Press 'z' to stop and any key to resume\r\n\n");

    /* Initialise the transmit buffer array to end in a NULL character */
    g_sci_tx_buffer[1] = 0x00;

    /* Start periodic CMT2 count */
    timer_delay(300, MILLISECS, PERIODIC_MODE);
}
/*******************************************************************************
* End of function Init_Async
*******************************************************************************/

/*******************************************************************************
* Function Name: init_timer
* Description  : This function initialises the CMT channel 2.
* Argument     : none
* Return value : none
*******************************************************************************/
static void init_timer (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Set the CMT2 interrupt level to 4 */
    IPR(CMT2,CMI2) = 0x4;

    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2) = 0x1;

    /* clear CMT2 interrupt request flag */
    IR(CMT2,CMI2) = 0x0;
}
/*******************************************************************************
* End of function init_timer
*******************************************************************************/

/*******************************************************************************
* Function Name : transmit_async
* Description   : Sends incrementing numbers across the UART link to the
*                 terminal window.
* Argument      : none
* Return value  : none
*******************************************************************************/
static void transmit_async (void)
{
    /* Check SCI status flag */
    if (g_sci_flag)
    {
        /* Increment number in transmit buffer */
        g_sci_tx_buffer[0]++;

        /* Check if hex value of transmit buffer is greater than 0x39 (9) */
        if (g_sci_tx_buffer[0] > 0x39)
        {
            /* Insert a new line and return the cursor left */
            text_write("\r\n");

            /* Reset the ASCII character in the transmit buffer back to 0 */
            g_sci_tx_buffer[0] = 0x30;
        }

        /* Send the transmit buffer to the terminal */
        text_write(g_sci_tx_buffer);

        /* Turn on LED0 */
        LED0 = LED_ON;

        /* Turn off LED1 */
        LED1 = LED_OFF;
    }
}
/*******************************************************************************
* End of function transmit_async
*******************************************************************************/

/*******************************************************************************
* Function Name : timer_delay
* Description   : Function used to create delays in milliseconds or
*                 microseconds depending on the user selection for controlling
*                 the debug LCD function's calls.
* Argument      : uint32_t  -   user_delay (Maximum 697 ms, 10922 uS)
*                 int8      -   unit
*                 int8      -   timer_mode
* Return value  : none
*******************************************************************************/
static void timer_delay (const uint32_t user_delay, const uint8_t unit,
                            const uint8_t timer_mode)
{
    /* Clear the timer's count */
    g_delay_counter = 0;

    /* Check if microseconds delay is required */
    if (unit == 'u')
    {
        /* Select the PCLK clock division as PCLK/8 = 6MHz */
        CMT2.CMCR.BIT.CKS = 0x0;

        /* Store a copy of the user delay value to g_periodic_delay */
        g_periodic_delay = user_delay * 6;

        /* Specify the timer period */
        /* Cast required to avoid warning:
        Implicit conversion: unsigned long to unsigned short.  */
        CMT2.CMCOR = (uint16_t)g_periodic_delay;
    }

    /* Check if milliseconds delay is required */
    if (unit == 'm')
    {
        /* Select the PCLK clock division as PCLK/512 = 93.75KHz */
        CMT2.CMCR.BIT.CKS = 0x3;

        /* Store a copy of the user delay value to g_periodic_delay */
        g_periodic_delay = user_delay * 94;

        /* Specify the timer period */
        /* Implicit conversion: unsigned long to unsigned short.  */
        CMT2.CMCOR = (uint16_t)g_periodic_delay;
    }

    /* Enable the compare match interrupt */
    CMT2.CMCR.BIT.CMIE = 1;

    /* Start CMT2 count */
    CMT.CMSTR1.BIT.STR2 = 1;

    /* Skip the following instructions if a periodic timer is required */
    if (PERIODIC_MODE != timer_mode)
    {
        /* Wait for the timer to timeout */
        while ((g_delay_counter != g_periodic_delay))
        {
            /* Wait */
        }

        /* Stop CMT2 count */
        CMT.CMSTR1.BIT.STR2 = 0;
     }
}
/*******************************************************************************
* End of function timer_delay
*******************************************************************************/

/*******************************************************************************
* Function Name : text_write
* Description   : Transmits null-terminated string.
* Argument      : (char*) msg_string - null terminated string
* Return value  : None
*******************************************************************************/
static void text_write (const char * const msg_string)
{
    /* Declare a count variable */
    uint8_t     i = 0;

    /* Declare a count variable */
    uint16_t    j = 0;

    /* This loop reads the text string and puts it into the UART's
        transmit buffer */
    for (i = 0; msg_string[i]; i++)
    {
        /* Check that the last transmission was completed */
        while (0x0 == SCI1.SSR.BIT.TEND)
        {
            /* wait for tx complete */
        }

        /* Transmit the next character */
        SCI1.TDR = (uint8_t)msg_string[i];

        /* Count up to 1000 */
        for (j = 0; j < 1000; j++)
        {
            /* delay */
        }
    }
}
/*******************************************************************************
* End of function text_write
*******************************************************************************/

/*******************************************************************************
* Function Name : Excep_SCI1_RXI1
* Description   : SCI0 receive interrupt handler, called when a character is
*                 received from the terminal program. Sets rx_flag if character
*                 is 'z' entered.
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_SCI1_RXI1
__interrupt  void Excep_SCI1_RXI1(void)
{
    /* Store the received character */
    /* cast required as register is defined as uint8_t */
    g_sci_rx_buffer = (char)SCI1.RDR;

    /* Check if last received character was 'z' */
    if (g_sci_rx_buffer == 'z')
    {
        /* Set SCI status flag to 0 */
        g_sci_flag = false;

        /* Turn off LED0 */
        LED0 = LED_OFF;

        /* Turn on LED1 */
        LED1 = LED_ON;
    }

    /* Check if the SCI status flag is 0, and the last received character
    was not 'z' */
    else if (!g_sci_flag)
    {
        /* Set SCI status flag to 1 */
        g_sci_flag = true;
    }
    else
    {
        /* do nothing */
    }
}
/*******************************************************************************
* End of function Excep_SCI1_RXI1
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_CMT2_CMI2
* Description   : CMT2 interrupt handler. This function calls the Transmit_Async
*                 function and increments the g_delay_counter variable before
*                 exiting.
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt  void Excep_CMT2_CMI2(void)
{
    /* Transmit a byte */
    transmit_async();

    /* Increment counter */
    g_delay_counter++;
}
/*******************************************************************************
* End of function Excep_CMT2_CMI2
*******************************************************************************/


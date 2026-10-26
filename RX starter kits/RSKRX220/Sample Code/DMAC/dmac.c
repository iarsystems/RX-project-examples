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
* File Name     : dmac.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines DMAC intialisation and interrupt handler functions.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/


/*******************************************************************************
System Includes
*******************************************************************************/

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Provides declarations of functions defined in this file */
#include "dmac.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* DMAC end of transfer callback function prototype */
void CB_DMACTransferEnd_DMAC (void);

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* DMAC transfer destination array */
uint8_t g_dma_data_buff[DMA_BUFFER_SIZE];

/* DMAC transfer source variable */
uint8_t g_dma_data_source = 'X';

/*******************************************************************************
* Function Name: init_dmac
* Description  : This function configures DMAC channel 0 for a single block
*                software-triggered transfer. The transfer source address is
*                set to fixed and configured to be that of the variable
*                'g_dma_data_source'. The transfer destination address is set to
*                be incremented after each transfer and configured to be the
*                address of buffer 'g_dma_data_buff'. A transfer interrupt
*                request to the CPU is set to occur after all data have
*                transferred.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_dmac (void)
{
    /* Disable DMA transfers */
    DMAC0.DMCNT.BIT.DTE = 0x0;

    /* Set the following:
       transfer destination address is incremented,
       transfer source address is fixed. */
    DMAC0.DMAMD.WORD = 0x0080u;

    /* Configure DMA transfer modes as follows:
       Block transfer,
       The source is specified as the block area
       8-bit transfer data size
       Software request source */
    DMAC0.DMTMD.WORD = 0x9000u;

    /* Specify the source address */
    DMAC0.DMSAR = &g_dma_data_source;

    /* Specify the destination address */
    DMAC0.DMDAR = g_dma_data_buff;

    /* Set the number of transfer operations to 1024 */
    DMAC0.DMCRA = sizeof(g_dma_data_buff);

    /* Set the number of block transfers operations to 1*/
    DMAC0.DMCRB = 0x1u;

    /* Set the activation source's interrupt
       flag to issue an interrupt to rhe CPU */
    DMAC0.DMCSL.BIT.DISEL = 0x1u;

    /* Enable DMA transfer end interrupts */
    DMAC0.DMINT.BIT.DTIE = 0x1u;

    /* Set DMAC0 interrupt priority level to 5 */
    IPR(DMAC,DMAC0I) = 0x5u;

    /* Enable DMAC0 interrupts */
    IEN(DMAC,DMAC0I) = 0x1u;

    /* Clear DMAC0 interrupt flag */
    IR(DMAC,DMAC0I) = 0x0;

    /* Enable DMA transfers */
    DMAC0.DMCNT.BIT.DTE = 0x1u;

    /* Enable DMAC operation */
    DMAC.DMAST.BIT.DMST = 0x1u;

    /* Enable and trigger the DMAC transfer. */
    DMAC0.DMREQ.BIT.SWREQ = 0x1u;

}
/*******************************************************************************
* End of function init_dmac
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_DMAC_DMAC0I_isr
* Description  : Interrupt function handler called on the completion of the
*                block transfer. The status of the DMAC is read and if the
*                transfer was completed successfully, LED1 is turned on and
*                "  DONE  " is displayed on the second line of the debug LCD.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_DMAC_DMAC0I
__interrupt  void Excep_DMAC_DMAC0I_isr(void)
{
    /* Read the status for channel 0 */
    if (DMAC0.DMSTS.BIT.DTIF)
    {
        /* Turn on LED1 to indicate end of transfer */
        LED1 = LED_ON;

        /* Inform the user that the transfer has ended */
        Display_LCD(LCD_LINE2, "  DONE  ");

        /* Clear the transfer end interrupt flag */
        DMAC0.DMSTS.BIT.DTIF = 0x0;
    }
}
/*******************************************************************************
* End of function Excep_DMAC_DMAC0I_isr
*******************************************************************************/


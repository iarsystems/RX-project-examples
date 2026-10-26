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
* File Name     : main_dmac.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : This sample demonstrates the DMAC unit by performing a DMA
*                 transfer from one variable, and duplicating it into a 1024
*                 element array.
* Operation     : 1. Compile and download the sample code to the target.
*
*                 2. Right-click the buffer 'g_dma_data_buff' and select 'Add
*                    Watch'. Within the Watch window right-click on
*                    'g_dma_data_buff' and select 'Enable Auto Update'.
*
*                 3. The contents of 'g_dma_data_buff can now be observed in the
*                    variable watch window.
*
*                 4. Click 'Reset Go' to start the software.
*
*                 5. The contents of 'g_dma_data_buff' will be cleared to 0
*                    before being filled with the content of the variable
*                    'g_dma_data_source', being character 'X'. If the transfer
*                    is successful, LED1 will turn on.
*
*         Note  : In order to watch the contents of the 'g_dma_data_buff' array
*                 during execution, the sample must be built in 'debug' mode.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
#include <intrinsics.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Defines DMAC functions used in this file */
#include "dmac.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* DMAC transfer destination array */
extern uint8_t g_dma_data_buff[DMA_BUFFER_SIZE];

/*******************************************************************************
* Function Name: main
* Description  : Main program function. This function first initialises the
*                debug LCD and displays the name of the application on the
*                screen. Main then calls the DMAC initialisation function, which
*                configures and starts the DMAC transfer from 'g_dma_data_source'
*                to every element of 'g_dma_data_buff' buffer.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void main (void)
{
    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD display */
    Init_LCD();

    /* Display instructions on the debug LCD */
    Display_LCD(LCD_LINE1, "  DMAC  ");
    Display_LCD(LCD_LINE2, "        ");

    /* Initialise the DMAC unit to transfer the contents
       of 'g_dma_data_source' to the array 'g_dma_data_buff' */
    init_dmac();

    /* Infinite while loop */
    while (1)
    {
        /* Sample operation is performed by interrupt routine */
    }
}
/*******************************************************************************
* End of function main
*******************************************************************************/


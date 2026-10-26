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
* Device        : R5F563T6
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T
* Description   : Defines ADC functions used for 12bit S12AD unit.
*******************************************************************************/
/*******************************************************************************
* History       : 05.02.2013  Ver. 1.00 First Release
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
/* Defines port registers */
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "sci.h"
#include "async.h"

/*******************************************************************************
User Defines
*******************************************************************************/

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Declaration of the command string to clear the terminal screen */
const char cmd_clr_scr[] = {27, 91, 50, 74, 0, 27, 91, 72, 0};

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/* text_write function prototype */
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
void Init_Async(void)
{
    startSCI1();    //start SCI1
    initSCI1Receive();

    /* Clear the text on terminal window */
    text_write(cmd_clr_scr);

    /* Display splash screen on terminal window */
    text_write("Renesas RX111 Async Serial \r\n");

    /* Inform user on how to stop transmission */
    text_write("Press 'z' to stop and any key to resume\r\n\n");
}
/*******************************************************************************
* End of function Init_Async
*******************************************************************************/

/*******************************************************************************
* Function Name : text_write
* Description   : Transmits null-terminated string.
* Argument      : (char*) msg_string - null terminated string
* Return value  : None
*******************************************************************************/
static void text_write (const char * const msg_string)
{
    while (sci1_tx_is_busy())
    {
    }
    sendSCI1((uint8_t *)msg_string, (uint16_t)strlen(msg_string));
}
/*******************************************************************************
* End of function text_write
*******************************************************************************/




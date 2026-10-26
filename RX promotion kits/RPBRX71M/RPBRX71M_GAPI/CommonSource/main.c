/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
 * SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : main.c
 * H/W Platform : RPBRX71M
 * Description  : This is the RPB RX71M GAPI Demonstration.
 *
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 07.03.2016 1.00     First Release
 ***********************************************************************************************************************/
#define MEM_DECL

/* Renesas Direct LCD Driver include files */
#include "Global.h"
#include "LCD_demo.h"
#include "r_usb_pmsc_apl.h"

/**********************************************************************************
 Function Name:   main
 Description:  Initialize the USB Subsystem, Start the event manager task and
 start the OS scheduler.
 Arguments:    None.
 Return value: None
 ***********************************************************************************/
void main (void)
{
    usb_main(); /* Start the USB Function Mass Storage Class Driver */
    EventManagerInit(); /* start the event manager tasks */
    vTaskStartScheduler(); /* RTOS_USAGE */
}
/*******************************************************************************
 End of function main
 *******************************************************************************/


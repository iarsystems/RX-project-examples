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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : flash_loader_user_app_main.c
* Device(s)	   : RX610, RX62N Group, RX63N Group
* Tool-Chain   : HEW, Renesas RX Toolchain v1.02
* Description  : This is the User Application that would start up the Flash Loader state machine.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 16.08.2012 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Used for sprintf() */
#include <stdio.h>
/* Fixed width integer support. */
#include <stdint.h>
/* bool support */
#include <stdbool.h>
/* Functions to start the Flash Loader state machine. */
#include "r_fl_downloader.h"
/* Board support. Using LCD functions below. */
#include "platform.h"
/* Used for calling Flash Loader state machine on a periodic basis. */
#include "r_cmt_rx_if.h"
/* Used to get current running application header info. Would not need to include this in your application. */
#include "r_fl_globals.h"
/* Used for writing to LCD. */
#include "lcd.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* What frequency to use to call the Flash Loader state machine. */
#define USER_APP_CMT_FREQUENCY        (20)      //Call the FL state machine at 20Hz

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static void fl_trigger_sm(void * pdata);

/* String to hold application version info. */
static uint8_t g_version_string[9];
/* Flag that tells the state machine to process. */
static bool     g_sm_process;

/***********************************************************************************************************************
* Function Name: main
* Description  : The main loop
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void main(void)
{
    /* Which CMT channel is used for state machine. */
    uint32_t cmt_channel;
    
    /* Init LCD. */
    lcd_initialize();

    /* Make ID string. */
    sprintf((char *)&g_version_string[0], "I=0x%04X",g_fl_cur_app_header.image_ID);

    /* Display message on LCD. */
    lcd_display(LCD_LINE1, &g_version_string[0]);

    /* Make version string. */
    sprintf((char *)&g_version_string[0], "V=0x%04X",g_fl_cur_app_header.version_num);

    lcd_display(LCD_LINE2, &g_version_string[0]);

    /* Initialize the Flash Loader code. */
    R_FL_DownloaderInit();

    /* Initialize state machine process flag. */
    g_sm_process = false;

    /* Create periodic timer to drive Flash Loader state machine. */
    R_CMT_CreatePeriodic(USER_APP_CMT_FREQUENCY, fl_trigger_sm, &cmt_channel);
    
    while(1) 
    {
        /* Call state machine after flag has been set. */
        if (true == g_sm_process)
        {
            /* Trigger state machine. */
            R_FL_StateMachine();

            g_sm_process = false;
        }
    }       
} /* End of function main() */

/***********************************************************************************************************************
* Function Name: fl_trigger_sm
* Description  : Sets flag to tell state machine to process again.
* Arguments    : channel -
*                    Which CMT channel this callback is for.
* Return value : none
***********************************************************************************************************************/
static void fl_trigger_sm (void * pdata)
{  
    /* Create periodic timer to call Flash Loader state machine. */
    g_sm_process = true;
}


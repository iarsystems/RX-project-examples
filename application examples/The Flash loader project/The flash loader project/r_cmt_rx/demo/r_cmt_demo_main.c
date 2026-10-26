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
* File Name    : r_cmt_demo_main.c
* H/W Platform : RDKRX63N
* Description  : This tests out the API functions of the r_cmt_rx module.
* Operation	   : 1. Compile and download the sample code. Click 'Reset Go' to start the software.
*                2. Use the board's switches to try out different API functions.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 08.11.2012 1.00     First Release
*         : 30.01.2013 1.10     Added support for RSKRX111.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Platform information. */
#include "platform.h"
/* r_cmt_rx support. */
#include "r_cmt_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* Signals that last one-shot has finished. */
bool g_last_op_finished;
/* Number of CMT channels for this MCU. */
uint32_t g_num_cmt_channels;

static void cmt_callback(void * pdata);

/***********************************************************************************************************************
* Function Name: main
* Description  : The main loop
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void main(void)
{
    /* Holds CMT channel that is being used. */
    uint32_t cmt_channel;
    
    g_num_cmt_channels = R_CMT_Control(CMT_NO_CHANNEL, CMT_CMD_GET_NUM_CHANNELS, &g_num_cmt_channels);

    g_last_op_finished = true;

    while (1)
    {        
        if (SW_ACTIVE == SW1)
        {
            while (SW_ACTIVE == SW1);

            /* RX111 will create the first 2 since it only has 2 CMT channels. */
            R_CMT_CreatePeriodic(2,  cmt_callback, &cmt_channel);
            R_CMT_CreatePeriodic(4,  cmt_callback, &cmt_channel);
            R_CMT_CreatePeriodic(8,  cmt_callback, &cmt_channel);
            R_CMT_CreatePeriodic(16, cmt_callback, &cmt_channel);
        }
        else if (SW_ACTIVE == SW2)
        {
            while (SW_ACTIVE == SW2);
            
            if (true == g_last_op_finished)
            {            
#if defined(BSP_BOARD_RDKRX63N) || defined(BSP_BOARD_RDKRX62N)
                LED5 ^= 1;
                LED6 ^= 1;
                LED7 ^= 1;
                LED8 ^= 1;
#else
                LED0 ^= 1;
                LED1 ^= 1;
                LED2 ^= 1;
                LED3 ^= 1;
#endif

                /* RX111 will create the first 2 since it only has 2 CMT channels. */
                R_CMT_CreateOneShot(100000,  cmt_callback, &cmt_channel);
                R_CMT_CreateOneShot(10000,   cmt_callback, &cmt_channel);
                R_CMT_CreateOneShot(1000,    cmt_callback, &cmt_channel);
                R_CMT_CreateOneShot(100,     cmt_callback, &cmt_channel);
                
                g_last_op_finished = false;
            }
        }
        else if (SW_ACTIVE == SW3)
        {
            while (SW_ACTIVE == SW3);

#if defined(BSP_BOARD_RDKRX63N) || defined(BSP_BOARD_RDKRX62N)
            LED5 = LED_OFF;
            LED6 = LED_OFF;
            LED7 = LED_OFF;
            LED8 = LED_OFF;
#else
            LED0 = LED_OFF;
            LED1 = LED_OFF;
            LED2 = LED_OFF;
            LED3 = LED_OFF;
#endif
            
            R_CMT_Stop(0);
            R_CMT_Stop(1);
            R_CMT_Stop(2);
            R_CMT_Stop(3);
        }        
    }
} 

/***********************************************************************************************************************
* Function Name: cmt_callback
* Description  : Function that will be called when compare match occurs.
* Arguments    : pdata -
*                    Which channel is causing this compare match.
* Return Value : none
***********************************************************************************************************************/
static void cmt_callback(void * pdata)
{
    uint32_t channel = (uint32_t)pdata;
    
    switch (channel)
    {        
        case 0:
#if defined(BSP_BOARD_RDKRX63N) || defined(BSP_BOARD_RDKRX62N)
            LED5 ^= 1;
#else
            LED0 ^= 1;
#endif
        break;
        case 1:
#if defined(BSP_BOARD_RDKRX63N) || defined(BSP_BOARD_RDKRX62N)
            LED6 ^= 1;
#else
            LED1 ^= 1;
#endif
        break;
        case 2:
#if defined(BSP_BOARD_RDKRX63N) || defined(BSP_BOARD_RDKRX62N)
            LED7 ^= 1;
#else
            LED2 ^= 1;
#endif
        break;
        case 3:
#if defined(BSP_BOARD_RDKRX63N) || defined(BSP_BOARD_RDKRX62N)
            LED8 ^= 1;
#else
            LED3 ^= 1;
#endif
        break;    
        default:
            while(1);
        break;
    }

    if (g_num_cmt_channels == (channel+1))
    {
        g_last_op_finished = true;
    }
}

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
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_delay_demo_main.c
* Description  : Tests out the r_delay module.
* Operation	   : 1. Compile and download the sample code. Click 'Reset Go' to start the software.
*                2. Watch LED on scope to see delay time.
*                3. Press SW1 to lengthen delay, SW2 to shorten, SW3 to reset.
*                4. custom_delay() will only be called if DELAY_CFG_USER_DELAY_FUNCTION is defined in r_delay_config.h.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 14.11.2012 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Platform information. */
#include "platform.h"
/* r_cmt_rx support. */
#include "r_delay_if.h"

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

/***********************************************************************************************************************
* Function Name: main
* Description  : The main loop
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void main(void)
{
    uint32_t delay_time = 1000;
    
    while (1)
    {        
        if (SW_ACTIVE == SW1)
        {
            while (SW_ACTIVE == SW1);
                        
            delay_time += 100;
        }
        else if (SW_ACTIVE == SW2)
        {
            while (SW_ACTIVE == SW2);
            
            delay_time -= 100;
        }
        else if (SW_ACTIVE == SW3)
        {
            while (SW_ACTIVE == SW3);

            delay_time = 1000;            
        }        
    
#if defined(PLATFORM_BOARD_RDKRX62N) || defined(PLATFORM_BOARD_RDKRX63N)   
        LED4 ^= 1;  
#else
        LED0 ^= 1;
#endif

        R_DELAY_Wait(DELAY_UNIT_US, delay_time);
    }
} 

/***********************************************************************************************************************
* Function Name: my_delay
* Description  : If the user configures the r_delay module to use their own delay implementation then this function
*                will be called. The user enables the use of their own delay implementation by defining the 
*                DELAY_CFG_USER_DELAY_FUNCTION macro in r_delay_config.h to the function they want to be called.
* Arguments    : wait_units - 
*                    The unit of the time sent in (e.g. milliseconds, microseconds, etc). See r_delay_if.h for available
*                    options.
*                wait_time -
*                    The amount of time to wait (in units of delay_units). This is an integer value, to use a value like
*                    1.5ms step down to the next smallest unit (e.g. 1.5ms = 1500us).
* Return Value : true - 
*                    Delay time has passed.
*                false -
*                    Invalid parameters input.
***********************************************************************************************************************/
bool my_delay (delay_unit_t wait_units, uint32_t wait_time)
{
    return true;
}

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
* File Name     : low_power.c
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Defines functions to demonstrate the low power modes of the
*                 CPU.
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* Low power consumption RPDL function definitions */
#include "r_pdl_lpc.h"
/* DMAC RPDL function definitions */
#include "r_pdl_dmac.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Contains the prototypes of the functions defined in this file */
#include "low_power.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Switch callback function prototype declaration */
void CB_Switch_LowPower(void);
/* Prototype for the idle function */
static void IdleFunction_LowPower(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare a variable to hold current CPU state */
uint16_t gCPUMode = CPU_HIGH_ACTIVE;
/* Flag indicating whether to change the mode   */
bool gChangeMode=0;

/*******************************************************************************
* Outline       : Init_LowPower
* Description   : Setup the low power configuration. 
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_LowPower(void)
{   
    /* Declare error flag */
    bool err = true;

    /* Configure switch callback function */
    SetSwitchReleaseCallback(CB_Switch_LowPower);       
    
    /* Configure the low power mode configuration register */
    err &=  R_LPC_Create
            (
                PDL_LPC_HIGH_SPEED_MODE | PDL_LPC_SOFTCUT_HOCO_LVD,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA         
            );  
                            
    /* Use PLL. Disable all other clocks */
    err &=  R_CGC_Control
            (
                PDL_CGC_CLK_PLL,
                PDL_CGC_MAIN_ENABLE|PDL_CGC_LOCO_DISABLE|PDL_CGC_HOCO_POWER_OFF,
                PDL_CGC_PLL_ENABLE|PDL_CGC_IWDTLOCO_DISABLE|PDL_CGC_SUB_CLOCK_DISABLE
            );

    /* Disable the DMAC & DTC modules   */
    err &=  R_DMAC_Destroy
            (
                0
            );

    /* Halt in while loop when RPDL errors detected */
    while(!err);    
    
    /* Idle function. Flashes LED0 */
    IdleFunction_LowPower();
}
/*******************************************************************************
* End of function Init_LowPower
*******************************************************************************/

/*******************************************************************************
* Outline       : IdleFunction_LowPower
* Description   : Idle function. This functions configures the required CPU
*                 operating mode depending upon the switch press. The function
*                 also toggles LED0 after every 25 ms.
* Argument      : none
* Return value  : none
*******************************************************************************/
static void IdleFunction_LowPower(void)
{   
    /* Declare error flag */
    bool err = true;

    /* The application does not exit this loop  */
    while(1)
    {
        /* Key press detected to change the mode ?  */
        if(gChangeMode != 0)
        {
            /* Reset the change mode flag   */
            gChangeMode = 0;

            /* Configure the CPU registers to switch to the required mode   */
            switch (gCPUMode)
            {
                case CPU_LOW_ACTIVE:
                {
                    /* Enable low speed clock on-chip oscillator */ 
                    err &=  R_CGC_Set
                            (
                                PDL_CGC_CLK_SUB_CLOCK,
                                PDL_CGC_BCLK_DISABLE | 
                                PDL_CGC_SUB_CLOCK_CL_STANDARD,
                                32768,
                                32768,
                                PDL_NO_DATA,
                                512,
                                512,
                                PDL_NO_DATA
                            );

                    /* Use subclock oscillator */
                    err &=  R_CGC_Control
                            (
                                PDL_CGC_CLK_SUB_CLOCK,
                                PDL_NO_DATA,
                                PDL_CGC_SUB_CLOCK_ENABLE | PDL_CGC_PLL_DISABLE
                            );

                    /* Switch to Low-speed operating mode 2 */
                    err &=  R_LPC_Create
                            (
                                PDL_LPC_LOW_SPEED_MODE_2 | PDL_LPC_SOFTCUT_HOCO_LVD,
                                PDL_NO_DATA,
                                PDL_NO_DATA,
                                PDL_NO_DATA,
                                PDL_NO_DATA,
                                PDL_NO_DATA,
                                PDL_NO_DATA         
                            );  
                            
                    /* Stop the main clock */
                    err &=  R_CGC_Control
                            (
                                PDL_NO_DATA,
                                PDL_CGC_MAIN_DISABLE,
                                PDL_NO_DATA
                            );
                                    
                    break;  
                }   
            
                case CPU_SLEEP:
                {
                    /* Turn off the LED0    */
                    err &=  R_IO_PORT_Write
                            (
                                PDL_IO_PORT_1_4,
                                1
                            );                      
                    
                    /* Place the device in standby mode */
                    err &=  R_LPC_Control
                            (
                                PDL_LPC_MODE_SOFTWARE_STANDBY | PDL_LPC_IO_RELEASE          
                            );      
                    break;
                }
                        
                case CPU_HIGH_ACTIVE:   
                {
                    /* Switch to high-speed operating mode */
                    err &=  R_LPC_Create
                            (
                                PDL_LPC_HIGH_SPEED_MODE | PDL_LPC_SOFTCUT_HOCO_LVD,
                                PDL_NO_DATA,
                                PDL_NO_DATA,
                                PDL_NO_DATA,
                                PDL_NO_DATA,
                                PDL_NO_DATA,
                                PDL_NO_DATA         
                            );  

                    /* Enable PLL clock */ 
                    err &=  R_CGC_Set   
                            (
                                PDL_CGC_CLK_PLL,
                                PDL_CGC_BCLK_DIV_2,
                                100E6,
                                50E6,
                                1.5625E6,
                                1.5625E6,
                                1.5625E6,
                                1.5625E6
                            );

                    /* Allow 100us for the main clock to stabilise */
                    err &=  R_CMT_CreateOneShot
                            (
                                0,
                                PDL_NO_DATA,
                                100E-6,
                                PDL_NO_FUNC,
                                0
                            );

                    /* Use PLL. Disable all other clocks */
                    err &=  R_CGC_Control
                            (
                                PDL_CGC_CLK_PLL,
                                PDL_CGC_MAIN_ENABLE,
                                PDL_CGC_PLL_ENABLE | PDL_CGC_SUB_CLOCK_DISABLE
                            );
                    break;
                }
                
                default:
                        break;
            }
        }
        else
        {
            /* Toggle LED0  */
            err &=  R_IO_PORT_Modify
                    (
                        PDL_IO_PORT_1_4,
                        PDL_IO_PORT_XOR, 
                        1
                    );
        
            /* Wait for 25 ms, before toggling the LED again */
            err &=  R_CMT_CreateOneShot
                    (
                        0,
                        PDL_NO_DATA,
                        25E-3,
                        PDL_NO_FUNC,
                        0
                    );  
        }
        
        /* Halt in while loop when RPDL errors detected */
        while(!err);        
    }
}
/*******************************************************************************
* End of function IdleFunction_LowPower
*******************************************************************************/

/*******************************************************************************
* Outline       : CB_Switch_LowPower 
* Description   : Callback function for switch.
* Argument      : none
* Return value  : none
*******************************************************************************/
void CB_Switch_LowPower (void)
{
    /* Check if SW1 was pressed */
    if(gSwitchFlag & SWITCHPRESS_1)
    {
        /* Enter low power (CPU active) mode */
        gCPUMode = CPU_LOW_ACTIVE;
        gChangeMode = 1;
    }
    /* Check if SW2 was pressed */  
    else if(gSwitchFlag & SWITCHPRESS_2)
    {
        /* Enter sleep (CPU inactive) mode  */
        gCPUMode = CPU_SLEEP;
        gChangeMode =1;
    }
    /* Check if SW3 was pressed */  
    else if(gSwitchFlag & SWITCHPRESS_3)
    {
        /* Enter sleep (CPU inactive) mode  */
        gCPUMode = CPU_HIGH_ACTIVE;
        gChangeMode =1;
    }
    else
    {
        /*   Do nothing */
    }
    
    /* Clear the switch status flag */
    gSwitchFlag = 0;    
}
/*******************************************************************************
* End of function CB_Switch_LowPower 
*******************************************************************************/

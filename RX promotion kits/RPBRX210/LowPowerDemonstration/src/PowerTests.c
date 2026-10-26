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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name		: PowerTests.c
* Version 		: 1.00
* Device 		: R5F52108DFP
* Tool-Chain 	: RX Family C Compiler
* H/W Platform	: RPBRX210
* Description   : Low power test routines
*******************************************************************************/
/*******************************************************************************
* History 		: 13.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Include machine.h to use the wait() and nop() functions */
#include    <machine.h>
/* Standard integer types with specified widths definitions */
#include    <stdint.h>                 
/* Register address definitions */
#include <iodefine.h>
//#include    "iodefine.h" old
/* Hardware platform definitions */
#include    "rsk_defines.h"
/* RAM based CPU exercise function prototypes */
#include    "PowerTests.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/  
/* For use in demonstrating CPU load and operation of DMA transfer */
volatile uint32_t  g_Product;
/* For use in demonstrating CPU load and operation of DMA transfer */
const uint16_t g_Val = 0x1234;

/* This function is linked in RAM to minimise power used on prefetch */
//#pragma section POWERTESTS

/*******************************************************************************
* Outline       : PerformPowerTests
* Description   : Function to allow user to measure power usage
*                 based on clock_mode and cpu_calc.
*                 Runs in high power mode until SW1 pressed then enters 
*                 selected low power mode.
*                 Press RES1 to exit and restart.
* Argument      : uint8_t clock_mode - Selected Clock Mode
*               : uint8_t cpu_calc - Selected CPU Load
* Return value  : Never returns
*******************************************************************************/
void PerformPowerTests( uint8_t clock_mode, uint8_t cpu_calc)
{
    /* For switch debounce delay loop */
    volatile uint32_t debounce;

    /* Align while loop to 16 byte boundary to minimise prefetch */
    nop();
    nop();

    /* Handle differently if set to a sleep, or software standby mode */
    if( (SW_SLEEP == clock_mode) ||
        (SW_STBY == clock_mode) || 
        (SW_DP_STBY == clock_mode) )
    {
        /* Loop forever (until reset pressed) */
        while(1)
        {
            /* Check for SW1 being pressed */
            if (PRESSED == SW1)
            {    
                /* SW1 pressed, now debounce the switch press */
                for(debounce=0; debounce<5000; debounce++)
                {
                    nop();
                }
                
                /* Disable Interrupts */
                IEN(MTU0,TGIA0) = 0;
                MTU0.TIER.BIT.TGIEA = 0;
                       
                /* Enter low power mode */                    
                wait();
            }
         
        }
    }
    /* Or if not in SW_SLEEP, SW_STBY or SW_DP_STBY mode */
    else
    {
        /* Use cpu_calc to determine whether to perform MUL and DMA
           operations - only do this if set to CPU_MAX */
        switch ( cpu_calc )
        {
        /* Treat CPU_MIN and CPU_TYP the same in this code but note there
           is still a difference in which peripherals are enabled */
        case CPU_MIN:
        case CPU_TYP:

            /* Align while loop to 16 byte boundary to minimise prefetch */
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            
            /* Loop forever (until reset pressed) */
            while(1)
            {
                
                /* Execute nops in this loop until SW1 is pressed.
                Use many nops to minimise the frequency of looping as this
                results in a prefetch every time there is a branch.
                Do not use a function call here. */
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();

                /* Check for SW1 being pressed */
                if (PRESSED == SW1)
                {
                    /* Enter sleep mode */
                    wait();
                }
            }
        case CPU_MAX:
            /* Align while loop to 16 byte boundary to minimise prefetch */
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            nop();
            
            /* Loop forever (until reset pressed) */
            while(1)                    
            {
                /* Execute nops in this loop until SW1 is pressed.
                Use many nops to minimise the frequency of looping as this
                results in a prefetch every time there is a branch.
                Do not use a function call here. */                           
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();
                nop();

                /* Do a MUL operation */
                g_Product = (uint32_t)( g_Val * MTU3.TCNT * CMT0.CMCNT );
                                                        
                /* Check for SW1 being pressed */
                if (PRESSED == SW1)
                {
                    /* Disable the interrupt that is triggering the DMAC, 
                       otherwise the CPU will be woken from sleep mode
                       on every interrupt. */ 
                    IEN(MTU0,TGIA0) = 0;     
                    MTU0.TIER.BIT.TGIEA = 0; 
                    /* Enter sleep mode */
                    wait();
                }
            }
        /* Default case should never be used */   
        default:
            break;
        }
    }
}
/******************************************************************************
* End of function PerformPowerTests
******************************************************************************/

/* End of section POWERTESTS */
//#pragma section
/* End of file */
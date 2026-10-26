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
* File Name		: setupclocks.c
* Version 		: 1.00
* Device 		: R5F52108DFP
* Tool-Chain 	: RX Family C Compiler
* H/W Platform	: RPBRX210
* Description   : Routines for setting up the clock mode
*******************************************************************************/
/*******************************************************************************
* History 		: 13.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Include machine.h to use the nop() function */
#include    <machine.h>
/* Standard integer types with specified widths definitions */
#include    <stdint.h>
/* Register address definitions */
#include <iodefine.h>
//#include    "iodefine.h" old 
/* Clock Mode setup function prototypes */
#include    "setupclocks.h" 
/* Hardware platform definitions */
#include    "rsk_defines.h"
 
/*******************************************************************************
* Outline       : Setclock_mode
* Description   : Configures clocks for selected operation modes.
* Argument  	: uint8_t clock_mode indicates Clock Mode setting
*               : uint8_t cpu_calc indicates CPU load setting
* Return value  : none
*******************************************************************************/
void Setclock_mode(uint8_t clock_mode, uint8_t cpu_calc )
{
    volatile uint32_t i;

    /* Enable or Disable the High Speed On-Chip Oscillator
        according to Clock Mode */
    if(    SUB_32k == clock_mode ||
        LOCO_125k == clock_mode )
    {
        /* Turn off the High Speed On-Chip Oscillator power supply */    
        SYSTEM.HOCOPCR.BYTE = 0x01;    
    }
    else if ( (HOCO_1M == clock_mode) ||
              (HOCO_50M == clock_mode) || 
              (SW_DP_STBY == clock_mode) ||
              (SW_STBY == clock_mode) || 
              (SW_SLEEP == clock_mode) )
    {
        /* Turn on the High Speed On-Chip Oscillator power supply */    
        SYSTEM.HOCOPCR.BYTE = 0x00;
        
        /* Select HOCO clock according to Clock Mode */
        if( (HOCO_1M == clock_mode) )
        {
            /* Select - 32MHz */
            SYSTEM.HOCOCR2.BYTE = 0x00;    
        }
        else if ((HOCO_50M == clock_mode) || 
                 (SW_DP_STBY == clock_mode) ||
                 (SW_STBY == clock_mode) || 
                 (SW_SLEEP == clock_mode) )
        {
            /* Select - 50MHz */
            SYSTEM.HOCOCR2.BYTE = 0x03;    
        }
        
        /* wait for High Speed On-Chip Oscillator to stabilise */
        for(i=0; i<100; i++)                        
        {
            nop();
        }
    }

    /* Check if selected mode uses sub clock*/
    if( (SUB_32k == clock_mode) ||
        (SW_DP_STBY == clock_mode) ||
        (SW_STBY == clock_mode) || 
        (SW_SLEEP == clock_mode) )
    {        
         /* Specify drive ability for low CL for the Sub Clock circuit */
        RTC.RCR3.BIT.RTCDV = 1;
    }

    /* Specify the clock dividers for ICLK, PCLKB, PCLKD, BCLK & FCLK based upon
       the measurement type we wish to do.
       The types are
       CPU_MIN
       CPU_TYP
       CPU_MAX
       See rsk_defines.h for more info.
    */
       
    /* The BUS CLOCK output is always disabled */
    SYSTEM.SCKCR.BIT.PSTOP1 = 1;            

    /* Set System Clock Control Register for SUB_32k or LOCO_125k  */           
    if(( SUB_32k == clock_mode ) || ( LOCO_125k == clock_mode ))
    {
        /* Set values according to cpu_calc */
        if( CPU_MIN == cpu_calc )
        {
            /* Set Peripheral Module Clock D to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKD     = 6;            
            /* Set Peripheral Module Clock B to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKB     = 6;            
            /* Set External Bus Clock to x 1/64 */
            SYSTEM.SCKCR.BIT.BCK     = 6;            
            /* Set System Clock to x 1 */
            SYSTEM.SCKCR.BIT.ICK     = 0;
            /* Set FlashIF Clock to x 1/64 */
            SYSTEM.SCKCR.BIT.FCK     = 6;             
        }
        else if( CPU_TYP == cpu_calc )
        {
            /* Set Peripheral Module Clock D to x 1 */
            SYSTEM.SCKCR.BIT.PCKD     = 0;            
            /* Set Peripheral Module Clock B to x 1 */
            SYSTEM.SCKCR.BIT.PCKB     = 0;            
            /* Set External Bus Clock to x 1 */
            SYSTEM.SCKCR.BIT.BCK     = 0;            
            /* Set System Clock to x 1 */
            SYSTEM.SCKCR.BIT.ICK     = 0;
            /* Set FlashIF Clock to x 1 */
            SYSTEM.SCKCR.BIT.FCK     = 0;              
        }
        else if( CPU_MAX == cpu_calc )
        {
            /* Set Peripheral Module Clock D to x 1 */
            SYSTEM.SCKCR.BIT.PCKD     = 0;            
            /* Set Peripheral Module Clock B to x 1 */
            SYSTEM.SCKCR.BIT.PCKB     = 0;            
            /* Set External Bus Clock to x 1 */
            SYSTEM.SCKCR.BIT.BCK     = 0;            
            /* Set System Clock to x 1 */
            SYSTEM.SCKCR.BIT.ICK     = 0;
            /* Set FlashIF Clock to x 1 */
            SYSTEM.SCKCR.BIT.FCK     = 0;
        }
    }
    /* Set System Clock Control Register for HOCO_1M  */           
    else if( HOCO_1M == clock_mode )
    {
        if( CPU_MIN == cpu_calc )
        {
            
            /* Set Peripheral Module Clock D to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKD     = 6;
            /* Set Peripheral Module Clock B to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKB     = 6;
            /* Set External Bus Clock to x 1/64 */
            SYSTEM.SCKCR.BIT.BCK     = 6;
            /* Set System Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.ICK     = 5;
            /* Set FlashIF Clock to x 1/64 */
            SYSTEM.SCKCR.BIT.FCK     = 6;
        }
        else if( CPU_TYP == cpu_calc )
        {
            /* Set Peripheral Module Clock D to x 1/32 */
            SYSTEM.SCKCR.BIT.PCKD     = 5;
            /* Set Peripheral Module Clock B to x 1/32 */
            SYSTEM.SCKCR.BIT.PCKB     = 5;
            /* Set External Bus Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.BCK     = 5;
            /* Set System Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.ICK     = 5;
            /* Set FlashIF Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.FCK     = 5;
        }
        else if( CPU_MAX == cpu_calc )
        {
            /* Set Peripheral Module Clock D to x 1/32 */
            SYSTEM.SCKCR.BIT.PCKD     = 5;
            /* Set Peripheral Module Clock B to x 1/32 */
            SYSTEM.SCKCR.BIT.PCKB     = 5;
            /* Set External Bus Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.BCK     = 5;
            /* Set System Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.ICK     = 5;
            /* Set FlashIF Clock to x 1/32 */
            SYSTEM.SCKCR.BIT.FCK     = 5;          
        }
    }
    /* Set System Clock Control Register for SW_DP_STBY, SW_STBY & SW_SLEEP */           
    else if(( SW_DP_STBY == clock_mode ) ||
            ( SW_STBY == clock_mode ) || 
            ( SW_SLEEP == clock_mode ))
    {
            /* Set Peripheral Module Clock D to x 1/2 */
            SYSTEM.SCKCR.BIT.PCKD     = 1;            
            /* Set Peripheral Module Clock B to x 1/2 */
            SYSTEM.SCKCR.BIT.PCKB     = 1;            
            /* Set External Bus Clock to x 1/2 */
            SYSTEM.SCKCR.BIT.BCK     = 1;            
            /* Set System Clock to x 1 */
            SYSTEM.SCKCR.BIT.ICK     = 0;            
            /* Set FlashIF Clock to x 1/2 */
            SYSTEM.SCKCR.BIT.FCK     = 1; 
    } 
    /* Set System Clock Control Register for HOCO_50M & any other modes */
    else
    {
        if( CPU_MIN == cpu_calc )
        {
            /* Set Peripheral Module Clock D to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKD     = 6;            
            /* Set Peripheral Module Clock B to x 1/64 */
            SYSTEM.SCKCR.BIT.PCKB     = 6;            
            /* Set External Bus Clock to x 1/64 */
            SYSTEM.SCKCR.BIT.BCK     = 6;            
            /* Set System Clock to x 1 */
            SYSTEM.SCKCR.BIT.ICK     = 0;
            /* Set FlashIF Clock to x 1/64 */
            SYSTEM.SCKCR.BIT.FCK     = 6;            
        }
        else if( CPU_TYP == cpu_calc )
        {
            /* Set Peripheral Module Clock D to x 1/2 */
            SYSTEM.SCKCR.BIT.PCKD     = 1;            
            /* Set Peripheral Module Clock B to x 1/2 */
            SYSTEM.SCKCR.BIT.PCKB     = 1;            
            /* Set External Bus Clock to x 1/2 */
            SYSTEM.SCKCR.BIT.BCK     = 1;            
            /* Set System Clock to x 1 */
            SYSTEM.SCKCR.BIT.ICK     = 0;            
            /* Set FlashIF Clock to x 1/2 */
            SYSTEM.SCKCR.BIT.FCK     = 1;            
        }
        else if( CPU_MAX == cpu_calc )
        {
            /* Set Peripheral Module Clock D to x 1/2 */
            SYSTEM.SCKCR.BIT.PCKD     = 1;            
            /* Set Peripheral Module Clock B to x 1/2 */
            SYSTEM.SCKCR.BIT.PCKB     = 1;            
            /* Set External Bus Clock to x 1/2 */
            SYSTEM.SCKCR.BIT.BCK     = 1;            
            /* Set System Clock to x 1 */
            SYSTEM.SCKCR.BIT.ICK     = 0;            
            /* Set FlashIF Clock to x 1/2 */
            SYSTEM.SCKCR.BIT.FCK     = 1;         
        }
    }

    /* Wait for the Operating Power Control Mode Transition Status Flag
       in the Operating Power Control Register before continuing */
    while( SYSTEM.OPCCR.BIT.OPCMTSF == 1 );
    
    /* Match the Operating Power Control Mode to the
       chosen frequency and voltage
       b2 b1 b0
       1  1  1 : Low-speed operating mode 2
       1  1  0 : Low-speed operating mode 1
       0  1  1 : Middle-speed operating mode 2
       0  1  0 : Middle-speed operating mode 1
       0  0  0 : High-speed operating mode     */

    /* Set Operating Power Control Mode in the
       Operating Power Control Register
       for SUB_32k Clock Mode */
    if( SUB_32k == clock_mode )
    {
        /* Set Low Speed Operating Mode 2 */
        SYSTEM.OPCCR.BIT.OPCM = 0x07;    
    }
    /* Set Operating Power Control Mode in the
       Operating Power Control Register
       for HOCO_1M Clock Mode */
    else if(( HOCO_1M == clock_mode ) ||
            ( LOCO_125k == clock_mode))
    {
        /* Set Low Speed Operating Mode 1 */
        SYSTEM.OPCCR.BIT.OPCM = 0x06;        
    }
    /* Set Operating Power Control Mode in the
       Operating Power Control Register
       for HOCO_50M, SW_DP_STBY, SW_STBY & SW_SLEEP modes */
    else if(( HOCO_50M == clock_mode ) || 
            ( SW_DP_STBY == clock_mode ) ||
            ( SW_STBY == clock_mode ) || 
            ( SW_SLEEP == clock_mode ))
    {
        /* Set High Speed Operating Mode */
        SYSTEM.OPCCR.BIT.OPCM = 0x00;        
    }
    /* Catch all - should not be used */
    else
    {
    }


    /* Wait for the Operating Power Control Mode Transition Status Flag
       in the Operating Power Control Register before continuing */
    while( SYSTEM.OPCCR.BIT.OPCMTSF == 1 );

    /* Switch from Low Speed On-Chip Oscillator to the desired clock */
    if( SUB_32k == clock_mode )
    {
        /* Use Subclock */
        SYSTEM.SCKCR3.WORD = 0x0300;
    }
    else if( ( HOCO_50M == clock_mode )      || 
             ( SW_DP_STBY == clock_mode ) ||
             ( SW_STBY == clock_mode )      || 
             ( SW_SLEEP == clock_mode )     ||
             ( HOCO_1M == clock_mode ) )
    {
        /* Use High Speed On Chip Oscillator */
        SYSTEM.SCKCR3.WORD = 0x0100;
    }
    else if( LOCO_125k == clock_mode )
    {
        /* Use Low Speed On Chip Oscillator */
        SYSTEM.SCKCR3.WORD = 0x0000;
    }
}
/******************************************************************************
* End of function Setclock_mode
******************************************************************************/
/* End of file */
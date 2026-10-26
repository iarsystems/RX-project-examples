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
* File Name		: s12ad.c
* Version 		: 1.00
* Device 		: R5F52108DFP
* Tool-Chain 	: RX Family C Compiler
* H/W Platform	: RPBRX210
* Description   : Analogue to digital and user interface functions
*******************************************************************************/
/*******************************************************************************
* History 		: 13.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Standard integer types with specified widths definitions */
#include    <stdint.h>
/* Register address definitions */
#include <iodefine.h>
//#include    "iodefine.h" old
/* Hardware platform definitions */
#include    "rsk_defines.h"
/* Analogue to digital and user interface function prototypes */
#include    "s12ad.h"

/*******************************************************************************
* Outline       : InitS12AD
* Description   : Function to initialise the ADC
* Argument      : none
* Return value  : none
*******************************************************************************/
void InitS12AD(void)
{
    /* Enable ADC */
    MSTP(S12AD) = 0x00;
    
    /* Set ADC to single-cycle scan mode */
    S12AD.ADCSR.BIT.ADCS = 0x00;

    /* Set to be software triggered */                                
    S12AD.ADCSR.BIT.TRGE = 0x00;
    
    /* Pad A/D data at the LSB end */
    S12AD.ADCER.BIT.ADRFMT = 0x00;
    
    /* Select AN000 */
    S12AD.ADANSA.BIT.ANSA0 = 1;      
    
    /* Disable intterupt generation */                           
    S12AD.ADCSR.BIT.ADIE = 0x00;
    
    /* "Analog Function Select": Set port 4.0 as an analogue pin */
    MPC.P40PFS.BIT.ASEL = 1;
    
    /* Set Port4.0 as NOT a general I/O pin */
    PORT4.PMR.BIT.B0 = 1;
}

/*******************************************************************************
* Outline       : GetUserInput
* Description   : Function to act as a menu to get user input via ADC / switch.
*                 User adjusts potentiometer RV1
*                 Corresponding value will be displayed on LEDs in binary.
*                 LED0 is the lsb but is on the left side of the RPB
*                 User then presses SW1 to select indicated value.
*                 Note that this function will only return once a selection
*                 has been made (SW1 pressed).
* Argument      : uint8_t Min   Minimum value in menu
*               : uint8_t Max   Maximum value in menu
* Return value  : uint8_t to indicate selected value
*******************************************************************************/
uint8_t GetUserInput(uint8_t min, uint8_t max)
{
    /* Variable for interpreted (scaled) value of potentiometer */
    uint8_t pot_val = 0x00;
    /* Debounce loop counter */
    uint32_t debounce;
    /* Key pressed flag */
    _Bool key_was_pressed = 0;
    
    /* Execute at least once to set the LEDs */
    do
    {
        /* check for button press and await release */
        while (PRESSED == SW1)
        {
            /* Set key_was_pressed flag */
            key_was_pressed = 1; 
            
            /* Await button release */
            while (NOT_PRESSED == SW1);
            
            /* Debounce the button release */
            for(debounce=2000; debounce; debounce--);
        }
        
        /* Initiate getting ADC value from RV1 */
        S12AD.ADCSR.BIT.ADST = 1;
    
        /* Wait for ADC completion */
        while(S12AD.ADCSR.BIT.ADST == 1);         
    
        /* Calculate corresponding value - quantised evenly between min and max */
        pot_val = (uint8_t)((S12AD.ADDR0 * (max - min + 1) / 4096) + min);
            
        /* Output value to LEDs */
        OutputBinaryLEDValue(pot_val);
        
    } while( (!key_was_pressed) && /* Loop until key pressed */
             (pot_val >= min) &&  /* and until pot_val is within */
             (pot_val <= max) );  /* the valid range */
             
    /* Return the pot_val, the value selected by the user */
    return ( pot_val );
}
/******************************************************************************
* End of function GetUserInput
******************************************************************************/

/******************************************************************************
* Outline 		: OutputBinaryLEDValue
* Description 	: Outputs argument on LEDs using binary.
* Argument  	: uint8_t Value Bitmask to indicate on LEDs
*                         b0: LED0 (0x01)   1 = On, 0 = Off
*                         b1: LED0 (0x02)   1 = On, 0 = Off
*                         b2: LED0 (0x04)   1 = On, 0 = Off
* Return value  : none
******************************************************************************/
void OutputBinaryLEDValue( uint8_t value )
{
    /* Output value on LEDs */
    LED0 = (uint8_t) !(value & LED0Mask);
    LED1 = (uint8_t) !(value & LED1Mask);
    LED2 = (uint8_t) !(value & LED2Mask);
}
/******************************************************************************
* End of function OutputBinaryLEDValue
******************************************************************************/
/* End of file */
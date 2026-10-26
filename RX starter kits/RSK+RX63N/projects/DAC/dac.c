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
* File Name     : dac.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines DAC functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes 
*******************************************************************************/
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>
/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines DAC functions used in this file */
#include "dac.h"
/* Defines MCU registers used in this file */
#include "iorx63n.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Timer initialisation function prototype */
void InitTimer_DAC(void);
/* ADC initialisation function prototype */
void InitADC_DAC(void);

/*******************************************************************************
* Outline         : Init_DAC
* Description   : Initialises the DAC unit, and also calls the ADC and timer
*                 initialisation functions.
* Argument          : none
* Return value  : none
*******************************************************************************/
void Init_DAC(void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    
    /* PWPR.PFSWE write protect off */
    MPC.PWPR.BYTE = 0x00u;    
    
    /* PFS register write protect off */
    MPC.PWPR.BYTE = 0x40u;    
    
    /* Cancel the DAC module clock stop mode */
    MSTP_DA = 0;
    
    /* Set DAC output pin, P01, to output */
    PORT0.PDR.BIT.B3 = 0u;
    
    /* Set DAC output pin, P01, from general I/O to peripheral controlled. */
    PORT0.PMR.BIT.B3 = 0u;
    
    /* Set DAC output pin, P01, to an analog pin */
    MPC.P03PFS.BIT.ASEL = 1u;
        
    /* Enable ADC & DAC synchronization */
    DA.DAADSCR.BYTE = 0x01u;
    
    /* Configure DAC data register to align right (lsb = bit0) */
    DA.DADPR.BYTE = 0x00u;
        
    /* Enable DAC channel DA0 */
    DA.DACR.BIT.DAOE0 = 1u; 
    
    /* Set DAC data register to 0x0000 for first conversion */
    DA.DADR0 = 0x0000;
     
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;
    
    /* Turn on PFS register write protect */
    MPC.PWPR.BYTE = 0x80u;    
    
    /* Initialise timer for DAC update interrupt */
    InitTimer_DAC();
    InitADC_DAC();
}
/*******************************************************************************
* End of function Init_DAC
*******************************************************************************/

/*******************************************************************************
* Outline       : InitTimer_DAC
* Description   : Initialises a timer channel to provide interval interrupts.
* Argument      : none
* Return value  : none
*******************************************************************************/
void InitTimer_DAC(void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    
    /* Cancel the CMT2 module clock stop mode */
    MSTP_CMT2 = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;    
    
    /* Set CMT2 interrupt priority level to 5 */    
    IPR(CMT2,CMI2) = 0x5;
    
    /* Enable CMT2 interrupts */
    IEN(CMT2,CMI2)      = 0x1;
    CMT2.CMCR.BIT.CMIE  = 0x1;
    
    /* Clear CMT2 interrupt flag */
    IR(CMT2,CMI2) = 0x0;
    
    /* Select the PCLK clock division as PCLK/128 = 375KHz */ 
    CMT2.CMCR.BIT.CKS = 0x2;
        
    /* Set compare match value to generate 100us interval
       (37.5 cycles @ 375KHz) */
    CMT2.CMCOR = 38u;   
 
    /* Start timer running */
    CMT.CMSTR1.BIT.STR2 = 0x1;
}
/*******************************************************************************
* End of function InitTimer_DAC
*******************************************************************************/

/*******************************************************************************
* Outline       : InitADC_DAC
* Description   : Initialises the ADC unit to perform repeat conversions.
* Argument      : none
* Return value  : none
*******************************************************************************/
void InitADC_DAC(void)
{    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    
    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;
    
    /* Use the AN000 (Potentiometer) pin as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01;
    
    /* ADC clock = PCLK/8, continuous scan mode */
    S12AD.ADCSR.BYTE |= 0x40;
    
    /* Selects AN000 */
    S12AD.ADANS0.WORD = 0x0001;    
    
    /* Set the sampling cycle to 255 states (approximately 5 us) */
    S12AD.ADSSTR23.WORD = 0x14FF;
        
    /* Start ADC */
    S12AD.ADCSR.BIT.ADST = 1;
}
/*******************************************************************************
* End of function InitADC_DAC
*******************************************************************************/

/*******************************************************************************
* Outline       : Excep_CMT2_CMI2
* Description   : CMT timer interrupt. This function fetches the latest ADC
*                 result, truncates the last 2 bits of precision and writes the
*                 value to the DAC unit.
* Argument      : none
* Return value  : none
*******************************************************************************/
#pragma vector=VECT_CMT2_CMI2
__interrupt void Excep_CMT2_CMI2(void)
{    
    /* Fetch ADC value, truncate the last 2bits and write to DAC register */
    DA.DADR0 = (uint16_t)(S12AD.ADDR0 >> 2);
}
/*******************************************************************************
* End of function Excep_CMT2_CMI2
*******************************************************************************/

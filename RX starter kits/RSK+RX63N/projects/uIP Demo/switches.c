/*******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. 

* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : switches.c
* Version       : 1.0
* Device(s)     : R5F563NB
* Tool-Chain    : Renesas RX Standard Toolchain 1.1.0.0
* OS            : None
* H/W Platform  : RSK+RX63N
* Description   : Sets up push button switches and provides ISR code. 
*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 01.10.2011     1.00        First release
*******************************************************************************/

/******************************************************************************
Includes <System Includes> , “Project Includes”
******************************************************************************/
/* Defines standard type structures */
#include <stdint.h>
/* Defines boolean type structures */
#include <stdbool.h>
/* Defines Special Flag Registers (SFR)s for RX63N */
#include "iorx63n.h"
#include "switches.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* Pin configuration defines */
#define SW1_PIDR        (PORT3.PIDR.BIT.B2)
#define SW2_PIDR        (PORT0.PIDR.BIT.B0)
#define SW3_PIDR        (PORT0.PIDR.BIT.B7)

#define SW1_PMR         (PORT3.PMR.BIT.B2)
#define SW2_PMR         (PORT0.PMR.BIT.B0)
#define SW3_PMR         (PORT0.PMR.BIT.B7)

#define SW1_PDR         (PORT3.PDR.BIT.B2)
#define SW2_PDR         (PORT0.PDR.BIT.B0)
#define SW3_PDR         (PORT0.PDR.BIT.B7)

#define SWITCH_IRQ_PRIORITY     (1)

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
uint32_t g_sw1;
uint32_t g_sw2;

/******************************************************************************
Private global variables and functions
******************************************************************************/
static void switches_setup_sw1(void);
static void switches_setup_sw2(void);
static void switches_setup_sw3(void);


/******************************************************************************
* Function Name: switches_initialize
* Description  : Sets up globals and initializes push button switches. 
* Arguments    : None
* Return Value : None
******************************************************************************/
void switches_initialize(void)
{
    /* Globals are set to not pressed */
    g_sw1 = SWITCH_NOT_PRESSED;
    g_sw2 = SWITCH_NOT_PRESSED;

    /* Call each switch setup function */
    switches_setup_sw1();
    switches_setup_sw2();
    switches_setup_sw3();
}

/******************************************************************************
* Function Name: switches_read
* Description  : Reads the current status of a switch
* Arguments    : sw - switch to read 
* Return Value : rc - state of the switch 
******************************************************************************/
uint32_t switches_read(uint32_t sw)
{
    uint32_t rc;

    switch (sw)
    {
        case SW1:
            rc = SW1_PIDR;
            break;

        case SW2:
            rc= SW2_PIDR;
            break;

        case SW3:
            rc = SW3_PIDR;
            break;

        default:
            rc = 1;
            break;  
    }

    if (rc == 0)
    {
        rc = SWITCH_PRESSED;
    }
    else
    {
        rc = SWITCH_NOT_PRESSED;
    }

    return rc;
}


/******************************************************************************
* Function Name: switches_setup_sw1
* Description  : Sets up the port pin for SW1 and interrupt for switch press 
* Arguments    : None
* Return Value : None
******************************************************************************/
void switches_setup_sw1(void)
{
    /* Enable writes to Pin Function Select (PFS) register */
    MPC.PWPR.BIT.B0WI = 0;                  // Enable write to PFSWE
    MPC.PWPR.BIT.PFSWE = 1;                 // Enable write to PFS

    /* Switch 1 is on P32 and used as IRQ2 */
    MPC.P32PFS.BIT.ISEL = 1;    
    
    /* Set as I/O mode. Do not need to set as a peripheral for IRQ2. */
    SW1_PMR = 0;
    SW1_PDR = 0;

    /* Set IRQ2 sense and digital filter */
    ICU.IRQCR[2].BIT.IRQMD = 0;             // Low level
    ICU.IRQFLTC0.BIT.FCLKSEL2 = 3;          // Set digital filter sample to PCLK/64
    ICU.IRQFLTE0.BIT.FLTEN2 = 1;            // Enable digital filter

    /* Clear interrupt before enable it */
    IR(ICU, IRQ2) = 0;

    /* Enable interrupt */        
    IPR(ICU, IRQ2) = SWITCH_IRQ_PRIORITY;   // Set interrupt level
    IEN(ICU, IRQ2) = 1;                     // Enable interrupt

    /* Disable writes to PFS */
    MPC.PWPR.BIT.PFSWE = 0;                 // Disable write to PFS
    MPC.PWPR.BIT.B0WI = 1;                  // Disable write to PFSWE
}


/******************************************************************************
* Function Name: switches_setup_sw2
* Description  : Sets up the port pin for SW2 and interrupt for switch press 
* Arguments    : None
* Return Value : None
******************************************************************************/
void switches_setup_sw2(void)
{
    /* Enable writes to Pin Function Select (PFS) register */
    MPC.PWPR.BIT.B0WI = 0;                  // Enable write to PFSWE
    MPC.PWPR.BIT.PFSWE = 1;                 // Enable write to PFS

    /* Switch 1 is on P32 and used as IRQ8 */
    MPC.P00PFS.BIT.ISEL = 1;    
    
    /* Set as I/O mode. Do not need to set as a peripheral for IRQ8. */
    SW2_PMR = 0;
    SW2_PDR = 0;

    /* Set IRQ2 sense and digital filter */
    ICU.IRQCR[8].BIT.IRQMD = 0;             // Low level
    ICU.IRQFLTC1.BIT.FCLKSEL8 = 3;          // Set digital filter sample to PCLK/64
    ICU.IRQFLTE1.BIT.FLTEN8 = 1;            // Enable digital filter

    /* Clear interrupt before enable it */
    IR(ICU, IRQ8) = 0;

    /* Enable interrupt */        
    IPR(ICU, IRQ8) = SWITCH_IRQ_PRIORITY;   // Set interrupt level
    IEN(ICU, IRQ8) = 1;                     // Enable interrupt

    /* Disable writes to PFS */
    MPC.PWPR.BIT.PFSWE = 0;                 // Disable write to PFS
    MPC.PWPR.BIT.B0WI = 1;                  // Disable write to PFSWE
}


/******************************************************************************
* Function Name: switches_setup_sw3
* Description  : Sets up the port pin for SW3 
* Arguments    : None
* Return Value : None
******************************************************************************/
void switches_setup_sw3(void)
{
    /* Switch 3 is an input pin */
    SW3_PMR = 0;
    SW3_PDR = 0;
}


/******************************************************************************
* Function Name: switches_sw1_isr
* Description  : Switch 1 interrupt service routine
* Arguments    : None
* Return Value : None
******************************************************************************/
#pragma vector = VECT_ICU_IRQ2 //TODO save interrupt
__interrupt void switches_sw1_isr(void)
{
    /* Set global flag */
    g_sw1 = SWITCH_PRESSED;

    /* Clear interrupt */
    IR(ICU, IRQ2) = 0;      
}

/******************************************************************************
* Function Name: switches_sw2_isr
* Description  : Switch 2 interrupt service routine
* Arguments    : None
* Return Value : None
******************************************************************************/
#pragma vector = VECT_ICU_IRQ8  //TODO save interrupt
__interrupt void switches_sw2_isr(void)
{
    /* Set global flag */
    g_sw2 = SWITCH_PRESSED;

    /* Clear flag */
    IR(ICU, IRQ8) = 0;      
}

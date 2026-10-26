/* Adapted for use with IAR Embedded Workbench */
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
* File Name     : hwsetup.c
* Version      : 1.0 
* Device(s)    : RX
* H/W Platform : RSKRX610
* Description  : Defines the initialisation routines used each time the MCU is restarted.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 22.11.2011 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* I/O Register and board definitions */
#include "platform.h"
/* Contains delcarations for the functions defined in this file */
#include "hwsetup.h"

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* Configure the Clock Generation Circuit (CGC) */
static void operating_frequency_set(void);

/* MCU I/O port configuration function delcaration */
static void output_ports_configure(void);

/* Interrupt configuration function delcaration */
static void interrupts_configure(void);

/* MCU peripheral module configuration function declaration */
static void peripheral_modules_enable(void);


/***********************************************************************************************************************
* Function name: hardware_setup
* Description  : Contains setup functions called at device restart
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void hardware_setup(void)
{
  operating_frequency_set();
  output_ports_configure();
  interrupts_configure();
  peripheral_modules_enable();
}

/***********************************************************************************************************************
* Function name: operating_frequency_set
* Description  : Configures the clock settings for each of the device clocks
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void operating_frequency_set(void)
{
  /* 
  Clock Description              Frequency
  ----------------------------------------
  Input Clock Frequency..............12MHz
  Internal Clock Frequency...........96MHz
  Peripheral Clock Frequency.........48MHz
  External Bus Clock Frequency.......12MHz */
  
  /* SCKCR - System Clock Control Register 
     b27:b24 ICK[3:0] = 0, system clock = EXTAL * 8 = 96 MHz
     b23     PSTOP1   = 0, BCLK pin output disabled
     b19:b16 BCK[3:0] = 3, BCLK set to lowest multiplier (x1)to reduce power
     b11:b8  PCK[3:0] = 1, PCK = EXTAL * 4 = 48 MHz
  */
  SYSTEM.SCKCR.LONG = (unsigned long)0x00030100;
}

/***********************************************************************************************************************
* Function name: output_ports_configure
* Description  : Configures the port and pin direction settings, and sets the pin outputs to a safe level.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void output_ports_configure(void)
{
  /* Enable LEDs. */
  /* Start with LEDs off. */
  LED0 = LED_OFF;
  LED1 = LED_OFF;
  LED2 = LED_OFF;
  LED3 = LED_OFF;
  
  /* Set LED pins as outputs. */
  LED0_DDR = 1;
  LED1_DDR = 1;
  LED2_DDR = 1;
  LED3_DDR = 1;
  
  /* Enable switches. */
  /* Set pins as inputs. */
  SW1_DDR = 0;
  SW2_DDR = 0;
  SW3_DDR = 0;
  
  /* Set Input Buffer Control registers for switches. */
  SW1_ICR = 1;
  SW2_ICR = 1;
  SW3_ICR = 1;
}

/***********************************************************************************************************************
* Function name: interrupts_configure
* Description  : Configures interrupts used
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void interrupts_configure(void)
{
  /* Enable the bus error interrupt to catch accesses to illegal/reserved areas
     of memory. */

  /* Clear any pending interrupts */
  IR(BSC,BUSERR) = 0;
  /* Make this the highest priority interrupt (adjust as necessary for your
     application) */
  IPR(BSC,BUSERR) = 0x07; 
  /* Enable the interrupt in the ICU */
  IEN(BSC,BUSERR) = 1; 
  /* Enable illegal address interrupt in the BSC */
  BSC.BEREN.BIT.IGAEN = 1;
}

/***********************************************************************************************************************
* Function name: peripheral_modules_enable
* Description  : Enables and configures peripheral devices on the MCU
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void peripheral_modules_enable(void)
{
  /* Add code here to enable peripherals used by the application */
}

/******************************************************************************
* Function name: buserr_isr
* Description  : Sample ISR for bus error (must do hardware setup first!)
*                By default, this demo code enables the Bus Error Interrupt.
*                This interrupt will fire if the user tries to access code
*                or data from one of the reserved areas in the memory map, 
*                including the areas covered by disabled chip selects.
*                A nop statement is included here as a convenient place
*                to set a breakpoint during debugging and development, and
*                further handling should be added by the user for their 
*                application.
* Arguments    : none
* Return value : none
******************************************************************************/
#pragma vector = VECT_BSC_BUSERR
#ifdef ROM_BGO
CODE_IN_RAM
#endif
__interrupt void buserr_isr(void)
{
  /* To find the address that was accessed when the bus error occured, read
     the register BSC.BERSR2.WORD.  The upper 13 bits of this register
     contain the upper 13-bits of the offending address (in 512K byte units).
  */
  
  /* Add your own code here to handle this interrupt */
  __no_operation();
}

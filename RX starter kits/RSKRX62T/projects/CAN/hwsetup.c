/* Adapted for use with IAR Embedded Workbench */
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
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : hwsetup.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Enables CAN related hardware. 
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Removed RPDL)
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
/* C99 standard library header */
#include "stdint.h"
/* Defines RX62T MCU registers */
#include <iorx62t.h>
/* Provides switches related definitions and function prototypes */
#include "rskrx62tdef.h"
/* Provides definitions and functions used in file r_can_api.c */
#include "r_can_api.h"
/* Includes defines used in LCD routines */
#include "lcd.h"
/* Defines the initialisation routines used each time the MCU is restarted.  */
#include "hwsetup.h"

  
/*******************************************************************************
Function Prototypes
*******************************************************************************/
/* CAN module stop state enable function prototype */
static void PeripheralStandbyControl(void);

/*******************************************************************************
* Outline     : HardwareSetup
* Description   : Contains all the setup functions called at device restart
* Argument      : none
* Return value  : none
*******************************************************************************/
void HardwareSetup(void)
{
  /* ICLK = EXTAL*8 = 12.5M*8 = 100 MHz. */
  SYSTEM.SCKCR.BIT.ICK  =  0; 
  /* PCLK = EXTAL*4 = 12.5M*4 = 50 MHz. */
  SYSTEM.SCKCR.BIT.PCK  =  1;
  
  /*** SPECIFIC port settings. ***/
  /* Switches are inputs. 
  SW1 is pE_5, SW2 is pE_4, SW3 is pB_4. */
  PORTE.DDR.BIT.B5 = 0;
  PORTE.DDR.BIT.B4 = 0;
  PORTB.DDR.BIT.B4 = 0;    

  /* Switches; set ICR */
  PORTE.ICR.BIT.B4 = 1;
  PORTE.ICR.BIT.B5 = 1;
  PORTB.ICR.BIT.B4 = 1;
  
  /* set LCD control signal */
  RS_PIN = 0;    
    EN_PIN = 0;
    DATA_PORT = 0x00;
    PORTE.DDR.BIT.B1 = 1;
    PORTE.DDR.BIT.B0 = 1;
    PORT9.DDR.BYTE |= 0x0f;
  
  /* Set LED ports as outputs. */
  ENABLE_LEDS
         
  /* Turn the user LEDs on */
    ALL_LEDS_ON
  
  /* For CAN see API. Set default port for CAN so an output high on a 
    CAN pin or low doesn't affect the bus. */
  R_CAN_PortSet(0, ENABLE);
      
    /* Enable CAN peripheral clock signal */      
  PeripheralStandbyControl();
}
/*******************************************************************************
* End of function CAN_Enable
*******************************************************************************/  

/******************************************************************************
* Outline      : PeripheralStandbyControl
* Description  : Control whether peripherals are in standby or is on.
* Arguments    : none 
* Return value : none
******************************************************************************/
static void PeripheralStandbyControl(void)
{
  /* CAN: Module Stop Control Register B (MSTPCRB) bit0 is for CAN Module.
  0: The module stop state is canceled.
  1: Transition to the module stop state is made. */
  SYSTEM.MSTPCRB.BIT.MSTPB0  =  0;
  
}
/*******************************************************************************
* End of function PeripheralStandbyControl
*******************************************************************************/  


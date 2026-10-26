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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name      : power_off.c
* Version       : 1.00
* Device       : R5F5630E
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description   : Defines power off functions.
*******************************************************************************/
/*******************************************************************************
* History     : 23 Jan. 2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Defines machine level functions used in this file */
#include <machine.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Declares functions defined in this file */
#include "power_off.h"
/* Defines switch functions used in this file */
#include "switch.h"
/* Defines device registers used in this file */
#include "iorx630.h"
/* Defines device LEDs and switches */
#include "rskrx630def.h"
/* Defines LCD functions used in this file */
#include "lcd.h"

/*******************************************************************************
* Global Variables 
*******************************************************************************/
/* Switches ready to enter standby ready flag variable (imported) */
extern volatile bool gSwitchStandbyReady;
/* Switch status flag variable (imported) */
extern volatile uint8_t gSwitchFlag;
/* Prepare to enter standby flag variable */
volatile bool gEnterStandby = false;

/*******************************************************************************
* Outline      : Excep_BRK
* Description  : Iterrupt routine to execute a WAIT instruction if 
*                 there was a BREAK
* Argument     : none
* Return value : none
*******************************************************************************/

#pragma vector=0
__interrupt void Excep_BRK(void)
{
  
  __wait_for_interrupt();
  
}

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Switch callback function prototype declaration */
static void SwitchCB_PowerOff(void);

/******************************************************************************
* Outline     : Init_PowerOff
* Description   : Function sets the switch callback function.
* Argument      : none
* Return value  : none
******************************************************************************/
void Init_PowerOff(void)
{
  /* Set the switch release callback function to SwitchCB_PowerOff */
  SetSwitchReleaseCallback(SwitchCB_PowerOff);
}
/******************************************************************************
* End of function Init_PowerOff
******************************************************************************/  

/******************************************************************************
* Outline     : SwitchCB_PowerOff
* Description   : Switch release callback function, which is executed by the
*          switch IRQ ISR. Function checks if switch 1 was pressed, then
*          sets the gEnterStandby flag true. Otherwise if just clears the
*          switch press flag and exits.
* Argument      : none
* Return value  : none
******************************************************************************/
static void SwitchCB_PowerOff(void)
{
  /* Check if the switch pressed was switch 1 */
  if(SWITCHPRESS_1 == gSwitchFlag)
  {
    /* Set the enter standby flag true */
    gEnterStandby = true;
  }
  
  /* Clear the switch press flag */
  gSwitchFlag &= 0x0F;
}
/******************************************************************************
* End of function SwitchCB_PowerOff
******************************************************************************/  

/******************************************************************************
* Outline     : EnterStandby_PowerOff
* Description   : This function configures the MCU to run from the internal 
*          HOCO, and prepares the MCU to enter standy mode. When the
*          wakes from standby, it continues through the function.
* Argument      : none
* Return value  : none
******************************************************************************/
void EnterStandby_PowerOff(void)
{
  /* Turn user LEDs 0 to 2 */
  LED0 = LED_OFF;
  LED1 = LED_OFF;
  LED2 = LED_OFF;
  
  /* Turn on LED3 to indicate program is waiting for user to release all
     switches */
  LED3 = LED_ON;
  
  /* Wait for switches to be ready to enter standby */
  while(!gSwitchStandbyReady);
  
  /* Turn off LED3 */
  LED3 = LED_OFF;
  
  /* Display MCU mode as 'Standby' on the debug LCD */
  Display_LCD(LCD_LINE2, " Standby");
  
  /* Disable register protection */
  SYSTEM.PRCR.WORD = 0xA50B;  
      
  /* Start oscillating the HOCO */
  SYSTEM.HOCOCR.BIT.HCSTP = 0;
  
  /* Wait for the HOCO to stabilise */
  uint16_t delay = 0xFFF;
  while(delay--);
  
  /* Switch system clock to HOCO */
  SYSTEM.SCKCR3.BIT.CKSEL = 0x1;
    
  /* Wait for the HOCO to stabilise */
  delay = 0xFFF;
  while(delay--);
  
  /* Set SSBY bit to enable sleep after executing wait() */
  SYSTEM.SBYCR.BIT.SSBY = 1;
  
  /* Wait until SSBY bit has been written successfully */
  while(!SYSTEM.SBYCR.BIT.SSBY);
  
  /* Re-enable register protection */
  SYSTEM.PRCR.BIT.PRKEY = 0xA5u;
  SYSTEM.PRCR.WORD &= 0xFF00;  

  /* Execute break instruction (which in turn excutes the wait instruction)
     This enters the MCU into sleep mode. */
  brk();
  
  /* The MCU will return here when it wakes from sleep mode */
  
  /* Disable register protection */
  SYSTEM.PRCR.WORD = 0xA50B;  
  
  /* Switch system clock to PLL */
  SYSTEM.SCKCR3.BIT.CKSEL = 0x4;
  
  /* Wait for clock to stabilise */
  delay = 0xFFF;
  while(delay--);
  
  /* Turn on all the user LEDs */
  LED0 = LED_ON;
  LED1 = LED_ON;
  LED2 = LED_ON;
  LED3 = LED_ON;
  
  /* Re-initialise the debug LCD */
  Init_LCD();
  
  /* Display current MCU mode on the debug LCD */
  Display_LCD(LCD_LINE1, "PowerOff");
  Display_LCD(LCD_LINE2, " Active ");
}
/******************************************************************************
* End of function EnterStandby_PowerOff
******************************************************************************/  

/******************************************************************************
* Outline     : FlashLEDs_PowerOff
* Description   : Function toggles the LEDs and waits in a delay while loop. 
*          When the function is called repeatedly in a while(1) loop, it
*          visibly flashes the user LEDs.
* Argument      : none
* Return value  : none
******************************************************************************/
void FlashLEDs_PowerOff(void)
{
  /* Toggle the user LEDs */
  if(LED_ON == LED0)
  {
    /* Set all user LEDs to off */
    LED0 = LED1 = LED2 = LED3 = LED_OFF;
  }
  else
  {
    /* Set all user LEDs to ON */
    LED0 = LED1 = LED2 = LED3 = LED_ON;
  }
  
  /* Create a time delay using a while loop */
  uint32_t delay = 0xFFFFF;
  while(delay--);
}
/******************************************************************************
* End of function FlashLEDs_PowerOff
******************************************************************************/  

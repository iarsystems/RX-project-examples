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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name      : main_fla_bgo.c
* Version     : 1.00
* Device     : R5F5630E
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description    : This sample demonstrates the FCU unit, by writing ADC results
*          to RAM using the DTC, then copying the data from RAM to 
*          incrementing data flash memory locations.
*
* Operation       :
*         1. Click the 'Connect' icon to connect to the RSK.
*
*         2. Build and download the sample code to the RSK.
*
*         3. Open the memory viewing, right click in the window and click
*          'address'. Enter the Display and Scroll addresses as 0x00100000. 
*          Enter the Scroll End address as 0x00100200 then click
*          OK. 
*
*         4. Ensure the 'Auto Refresh' option in the memory viewer is
*          disabled before proceeding. (It is not possible to use this
*          feature when observing flash memory). Open the 'Watch' window 
*          from the menu bar View > Symbol > Watch (Watch1) to view the 
*          gStore buffer array's contents. Right-click on gStore and 
*          select 'Enable Auto Update'.
*
*         5. Click 'Reset Go' to start the sample. Instructions will be
*           displayed on the debug LCD.
*
*         6. LED2 will turn on to indicate the flash erase operation. 
*          ADC conversions will then begin and triggers the DTC to transfer 
*          the ADC values to the gStore buffer, seen in the Watch1 window. 
*          After 256 conversions, the DTC resets its destination pointer 
*          to the begin of the gStore buffer and repeats the process.
*          
*         7. Adjust the potentiometer to see the values change. 
*
*         8. Press switch SW2 to stop ADC conversions; the message "ADC STOP" 
*          will appear briefly on the second line of the LCD. Press SW1 to 
*          erase the data flash block (LED2 will flash) before writing the
*                   gStore buffer contents to the data flash memory. " MATCH  " will
*                   appear on the second line of the LCD after a memory comparison, 
*          followed by "WR. PASS" and LED1 will also turn on then off.
*
*         9. Click the 'STOP' icon to halt program execution. Verify 
*          the flash memory area matches that of gStore.
*
*        10. Click 'GO' to resume program execution. Press SW2 to 
*          restart ADC conversions. "ADC RUN " will briefly appear on 
*          the second line of the LCD. Pressing SW3 will erase the 
*          data flash memory whilst conversions are occuring. 
*          "ER. PASS" will briefly appear on the second line of the 
*          LCD
*
*        11. Press SW1 to write the DTC transferred values to the flash 
*          data memory locations. No memory comparison will take place 
*          whilst ADC conversion are occuring.
*
* Note         : Memory locations which have not initialised do not have a
*          defined state, and will continuously change with random 
*          values.
*          Should the program halt as a result of a FLASH operation, 
*          when debugging using an E1 click 'Disconnect' and go to 
*          Step 1, otherwise disconnect then recoonect the power supply.
*******************************************************************************/  
/*******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines RX630 port registers */
#include "iorx630.h"
/* Defines macros relating to the RX630 user LEDs and switches */
#include "rskrx630def.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines flash functions used in this file */
#include "flash_bgo.h"

#include "intrinsics.h"

#include "hwsetup.h"


/******************************************************************************
* Global Variables
******************************************************************************/
/* External declaration of variable used for clearing the debug LCD */
extern bool gClear_LCD;
/* External declare of a flag to indicate flash write operation statuses */
extern bool gWrite_Status;
/* External declare of a flag to indicate ADC status */
extern bool gADC_Status;

/*******************************************************************************
* Outline      : main
* Description  : Main program function. This function first initialises the
*         debug LCD and displays instructions on the screen. Main then
*         calls the flash intialisation function which configures the FCU
*         unit, and sets up callback functions for the switch presses.
*         The function then waits in an infinite while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void main(void)
{
  
  HardwareSetup();
  
  __enable_interrupt();
  
  /* Initialise the debug LCD */
  Init_LCD();
  
  /* Display instructions on the debug LCD */  
  Display_LCD(LCD_LINE1, "Data BGO");
  Display_LCD(LCD_LINE2, "Push SW ");
  
  /* Initialise the flash controller unit */
  Init_FlashBGO();
      
  /* Start ADC conversions */
  S12AD.ADCSR.BIT.ADST = 1;
  
  /* Infinite while loop */
  while(1)
  {
    /* Display instructions on the debug LCD */  
    Display_LCD(LCD_LINE2, "Push SW ");
  
    /* write to flash if switch 1 is pressed */
    if(gSwitchFlag & SWITCHPRESS_1)
    {
      /* Erase flash memory before writing to it */
      Erase_FlashData();
      
      /* Turn off LED 2 */
      LED2 = (uint8_t)LED_OFF;
      
      /* write gStore data to flash memory */
      Write_FlashData();
      
      /* Check flash operation status returned by the FLASH interrupt */
      if(gWrite_Status == false)
      {
        /* Display instructions on the debug LCD */  
        Display_LCD(LCD_LINE2, "WR. PASS");
      }
      else
      {
        /* Display instructions on the debug LCD */  
        Display_LCD(LCD_LINE2, "WR. FAIL");
      }
      
      /* Create a short delay display delay */
      DisplayDelay((uint32_t)0x1FFFF);  
      
      /* Turn off LED 1 */
      LED1 = (uint8_t)LED_OFF;
      
      /* Clear switch flag */
      gSwitchFlag = 0x0;
    }
    
    /* Write to flash if switch 3 is pressed */
    if(gSwitchFlag & SWITCHPRESS_3)
    {  
      /* Erase flash memory */
      Erase_FlashData();
      
      /* Display instructions on the debug LCD */  
      Display_LCD(LCD_LINE2, "ER. PASS");
        
      /* Create a short delay display delay */
      DisplayDelay(0x1FFFF);
      
      /* Turn off LED 2 */
      LED2 = (uint8_t)LED_OFF;
      /* Turn off LED 3 */
      LED3 = (uint8_t)LED_OFF;
      
      /* Clear switch flag */
      gSwitchFlag = 0x0;
    }
    
    /* Stop A/D conversions if switch SW2 is pressed */
    if(gSwitchFlag & SWITCHPRESS_2)
    {  
      /* Toggle the ADC conversion start bit to either 
         stop or restart conversions based on last state. */
      S12AD.ADCSR.BIT.ADST = (uint8_t)~(S12AD.ADCSR.BIT.ADST);
      
      /* Check if the ADC start bit is set */
      if(S12AD.ADCSR.BIT.ADST == 0x0)
      {
        /* Display instructions on the debug LCD */  
        Display_LCD(LCD_LINE2, "ADC STOP");
        
        /* Set the flag to false if ADC is stopped */
        gADC_Status = false;
      }
      else
      {
        /* Display instructions on the debug LCD */  
        Display_LCD(LCD_LINE2, "ADC RUN ");
        
        /* Set the flag to true if ADC is running */
        gADC_Status = true;
      }
      /* Create a short delay display delay */
      DisplayDelay((uint32_t)0x1FFFF);
      
      /* Clear switch flag */
      gSwitchFlag = 0x0;
    }
    
    /* Check if the LCD's second line needs clearing */
    if(gClear_LCD == true)
    {
      /* Clear line 2 of the debug LCD */  
      Display_LCD(LCD_LINE2, "        ");  
      
      /* Set flag to false */
      gClear_LCD = false;
    }
  }
}
/******************************************************************************
* End of function main
******************************************************************************/

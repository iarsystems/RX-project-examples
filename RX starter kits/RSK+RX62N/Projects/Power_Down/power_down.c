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
*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : power_down.c 
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : Defines low power consumption functions.
*******************************************************************************/
/*******************************************************************************
* History    : 03.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Low power consumption RPDL function definitions */
#include "r_pdl_lpc.h"
/* IO port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions used in this file */
#include "switch.h"
/* Defines debug LCD functions used in this file */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "power_down.h"

/*******************************************************************************
* Imported Global Variables
*******************************************************************************/
/* Imported global variable, switch press flag */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Enter standby function prototype delcaration */
void Standby_PowerDown(void);

/*******************************************************************************
* Outline      : Init_PowerDown
* Description  : This function configures the MCU standby registers. The MCU is
*         configured to set the external bus to the high impedance state,
*         and resume the MCU after 65536 cycles of the peripheral clock.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_PowerDown(void)
{
  /* Declare error flag */
  bool err = true;

  /* Configure the MCU standby power registers */
  err &=  R_LPC_Create
      (
        PDL_LPC_EXT_BUS_HI_Z,
        PDL_LPC_STANDBY_65536
      );
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Init_PowerDown
*******************************************************************************/

/*******************************************************************************
* Outline      : Flash_LEDs
* Description  : This function toggles the user LEDs every 100ms. The function
*         polls for switch presses, and enters the MCU into standby mode
*         if switch SW1 has been pressed. Any switch interrupt resumes
*         the MCU, and exits the function.
* Argument     : none
* Return value : none
*******************************************************************************/
void Flash_LEDs(void)
{
  /* Declare error flag */
  bool err = true;

  /* Check switch flag each iteration, and exit if switch SW1 is pressed */
  while(!(gSwitchFlag & SWITCHPRESS_1))
  {
    /* Create a CMT oneshot timer for LED flash delay */
    err &=  R_CMT_CreateOneShot
        (
          2,
          PDL_NO_DATA,
          100E-3,
          PDL_NO_FUNC,
          0
        );
      
    /* Toggle LEDs */
    err &=  R_IO_PORT_Modify
        (
          PDL_IO_PORT_0_2|PDL_IO_PORT_0_3|PDL_IO_PORT_0_5,
          PDL_IO_PORT_XOR,
          1
        );
      
    /* Toggle LEDs */
    err &=  R_IO_PORT_Modify
        (
          PDL_IO_PORT_3_4,
          PDL_IO_PORT_XOR,
          1
        );
  }
  
  /* Put the MCU into standby mode */
  Standby_PowerDown();
  
  /* After MCU wakes up, program resumes here */
  
  /* Display current power mode as 'Active' on the debug LCD */
  DisplayLCD(LCD_LINE2, "Active  ");
  
  /* Turn the user LEDs on */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_0_2|PDL_IO_PORT_0_3|PDL_IO_PORT_0_5,
        0
      );

  /* Turn the user LEDs on */      
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_3_4,
        0
      );
  
  /* Halt in while loop when RPDL errors detected */
  while(!err);  
  
}
/*******************************************************************************
* End of function Flash_LEDs
*******************************************************************************/

/*******************************************************************************
* Outline      : Standby_PowerDown
* Description  : This function prepares the MCU for standby mode by turning off
*         the LEDs, and waiting until all switches have been released. If 
*         switch is held down whilst the program is preparing for 
*         standby, LED3 will remain lit until the switches are released.
* Argument     : none
* Return value : none
*******************************************************************************/
void Standby_PowerDown(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Display the current power mode as 'Standby' on the debug LCD */
  DisplayLCD(LCD_LINE2, "Standby ");
  
  /* Turn the user LEDs off */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_0_2|PDL_IO_PORT_0_3|PDL_IO_PORT_0_5,
        1
      );
      
  /* Turn LED3 on to indicate MCU is preparing to enter standby */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_3_4,
        0
      );
      
  /* Wait until switches are ready for standby */
  while(!gSwitchStandbyReady);
  
  /* Turn LED3 off to indicate MCU has entered standby */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_3_4,
        1
      );
    
  /* Put the MCU into standby mode */
  err &=   R_LPC_Control
      (
        PDL_LPC_MODE_SOFTWARE_STANDBY|PDL_LPC_IO_RELEASE
      );
  
  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function Standby_PowerDown
*******************************************************************************/

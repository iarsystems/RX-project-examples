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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name      : main_iic_master.c
* Version        : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description    : Demonstrates usage of IIC unit in master mode, by reading and
*          writing to an on-board R1EX24016 (EEPROM device) IC. 
* Operation       : 1. Compile and download the sample code to the target. click 
*           'Reset Go' icon on the 'Debug Run' toolbar to start the 
*           software. 
*
*          2. The name of the application will by displayed on the debug 
*           LCD.
*          
*          3. Press switch SW2 to perform a write operation. If the write
*             is successful, the data identifier will be displayed on the
*           screen. Re-press SW2 to write next data.
*         
*          4. Press switch SW3 to perform a read operation. If the read 
*             is successful, the identifier of the data read will appear
*           on the debug LCD. Re-press SW3 to repeat the read 
*           instruction.
*          
*          5. If a write error occurs, "Error W." will be displayed on 
*             the debug LCD & if read error occurs, "Error R." will be 
*           displayed on debug LCD.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/  
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Defines the EEPROM master sequence function used in this file */
#include "iic_eeprom_master_app.h"

/*******************************************************************************
* Outline      : main
* Description  : The main program function. This function first initialises the
*         debug LCD, and displays the application name. It then calls  
*         the MasterSequence_EEPROM function, from which it does not exit
*         from. The master sequence performs read and write operations to
*         the EEPROM device when user switches are pressed. Refer to  
*         iic_eeprom_master_app.c for further details.
* Argument     : none
* Return value : none
*******************************************************************************/
#include "intrinsics.h"
#include "hwsetup.h"

void main(void)
{
  /* Hardware setup */
  HardwareSetup();

  /* Enable Interrupt */
  __enable_interrupt();

  /* Initialise the debug LCD */
  Init_LCD();
  
  /* Display IIC mode on the debug LCD */
  Display_LCD(LCD_LINE1, "IIC Mast");
    
  /* Call the Master application program */
  MasterSequence_EEPROM();
  
  /* Infinite while loop */  
  while(1);
}
/*******************************************************************************
* End of function main
*******************************************************************************/

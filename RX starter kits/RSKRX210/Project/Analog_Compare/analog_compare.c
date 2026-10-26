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
* File Name    : analog_compare.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Defines the analog comparator configuration functions.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CPA RPDL function definitions */
#include "r_pdl_cpa.h"
/* INTC RPDL function definitions */
#include "r_pdl_intc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "analog_compare.h"

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Callback function for comparator A1 interrupt  */
void CB_Comparator_AnalogCompare(void);

/*******************************************************************************
* Outline     : Init_AnalogCompare
* Description   : This function configures the analog comparator A using the 
*          RPDL functions. 
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_AnalogCompare(void)
{  
  /* Declare error flag */
  bool err = true;

  /*   Configure the comparator A1 as follows -
    Enable CMPA1 interrupt
    Generate interrupt when CMPA1 input voltage crosses voltage at CVREFA 
    (below or above) 
    Specify the callback function for comparator A   */
  err &=  R_CPA_Create
      (
        0,
        PDL_CPA_INTERRUPT_RESET_ENABLE|PDL_CPA_MODE_SELECT_INTERRUPT|\
        PDL_CPA_IRQ_CROSS_CVREFA|PDL_CPA_MASKABLE_INTERRUPT,
        CB_Comparator_AnalogCompare,
        10
      );

  /* Halt in while loop when RPDL errors detected */
  while(!err);  
}
/******************************************************************************
* End of function Init_AnalogCompare
******************************************************************************/

/******************************************************************************
* Outline     : CB_Comparator_AnalogCompare
* Description   : Callback function for comparator A1 interrupt
* Argument    : none
* Return value  : none
******************************************************************************/
void CB_Comparator_AnalogCompare(void)
{
  /* Declare error flag */
  bool err = true;

  /* Stores the status of voltage detection */
  uint8_t cmpa_status=0;  

  /* Read current status of Comparator A   */
  err &=   R_CPA_GetStatus
      (
        &cmpa_status
      );
  while(!err);

  /* Display comparator reference on the LCD  */
  Display_LCD(LCD_LINE1, "COMPA1  ");    

  /* Check if there is any change detected */
  if((cmpa_status & 0x01) != 0)
  {
    /* Check if the CMPA1 voltage is equal to or more than reference
       voltage */
    if((cmpa_status & 0x02)!=0)
    {
      /* Turn off LED2 & LED3  */
      err &=  R_IO_PORT_Write
          (
            PDL_IO_PORT_1_6|PDL_IO_PORT_1_7,
            1
          );

      /* Turn on LED0 & LED1  */  
      err &=  R_IO_PORT_Write
          (
            PDL_IO_PORT_1_4|PDL_IO_PORT_1_5,
            0
          );
      
      /* Display the comparison result on LCD  */          
      Display_LCD(LCD_LINE2, "VI>=VREF");

      /* Wait for 500ms to avoid flickering on the display in case the
         input supply is not stable. */
      err &=  R_CMT_CreateOneShot
          (
            0,
            PDL_NO_DATA,
            500E-3,
            PDL_NO_FUNC,
            0
          );        
    }
    /* The CMPA1 voltage is less than the reference voltage */
    else
    {
      /* Turn off LED0 & LED1  */
      err &=  R_IO_PORT_Write
          (
            PDL_IO_PORT_1_4|PDL_IO_PORT_1_5,
            1
          );

      /* Turn on LED2 & LED3  */
      err &=  R_IO_PORT_Write
          (
            PDL_IO_PORT_1_6|PDL_IO_PORT_1_7,
            0
          );
      
      /* Display the comparison result on LCD  */
      Display_LCD(LCD_LINE2, "VI<VREF ");

      /* Wait for 500ms to avoid flickering on the display in case the
         input supply is not stable. */
      err &=  R_CMT_CreateOneShot
          (
            0,
            PDL_NO_DATA,
            500E-3,
            PDL_NO_FUNC,
            0
          );      
    }  
  }

  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/******************************************************************************
* End of function CB_Comparator_AnalogCompare
******************************************************************************/

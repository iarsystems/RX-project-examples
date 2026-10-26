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
* File Name    : vdet.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Defines control functions for voltage detect circuitry.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CMT RPDL function definitions */
#include "r_pdl_cmt.h"
/* LVD RPDL function definitions */
#include "r_pdl_lvd.h"
/* INTC RPDL function definitions */
#include "r_pdl_intc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "vdet.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Timer period interrupt callback function prototype declaration */
void CB_Timer_VDET(void);
/* Low voltage detect interrupt callback function prototype declaration */
void CB_LVD2_VDET(void);
/* NMI interrupt callback function prototype declaration */
void CB_NMI_VDET(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Global variable used to signal the need for LEDs flashing synchronisation */
uint8_t gLEDsync_flg = 0x0;

/*******************************************************************************
* Outline        : Init_VDET
* Description    : This function initialises the voltage detect circuitry to 
*          use the threshold set by VDET1 & VDET2. It also creates a  
*          periodic timer called every 500ms to toggle the LEDs.
* Argument    : none
* Return value  : none
*******************************************************************************/
void Init_VDET(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Configure the NMI interrupt to be triggered instead of VDET1 */
  err &=  R_INTC_CreateExtInterrupt
      (
        PDL_INTC_NMI,
        PDL_INTC_FILTER_DISABLE|PDL_INTC_FALLING|PDL_INTC_LVD1_ENABLE,
        CB_NMI_VDET,
        PDL_NO_DATA
      );      
              
  /* Generate a maskable VDET2 interrupt when Vdet2 is reached;
     generate NMI interrupt if VDET1 is reached. Set the voltage levels
     as follows - 
     VDET1 = 1.90V (will trigger the NMI interrupt)
     VDET2 = 4.0V (will trigger 'CB_LVD2_VDET' callback function ) */
  err &=  R_LVD_Create
      (
        PDL_LVD_ENABLE|PDL_LVD_INTERRUPT_NMI_DETECT_FALL,
        PDL_LVD_ENABLE|PDL_LVD_INTERRUPT_MI_DETECT_FALL|\
        PDL_LVD_VDET2_PIN_VCC,
        PDL_LVD_VOLTAGE_LEVEL_190,
        PDL_LVD_VOLTAGE_LEVEL_400,
        PDL_NO_FUNC,
        PDL_NO_DATA,
        CB_LVD2_VDET,
        8
      );

  /* Use the timer to toggle LEDs every 500ms */
  err &=  R_CMT_Create
      (
         2, 
         PDL_CMT_PERIOD, 
         500E-3, 
         CB_Timer_VDET, 
         6
      );
            
  /* Halt in while loop when RPDL errors detected  */
  while(!err);
}
/*******************************************************************************
* End of function Init_VDET
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Timer_VDET
* Description  : Timer period interrupt callback function. This function 
*         toggles the LEDs.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Timer_VDET(void)
{
  /* Declare error flag */
  bool err = true;

  /* The LED flashing will be controlled depending upon whether the VDET
     interrupt has occurred or not.   */
  if(gLEDsync_flg == 0x1)
  {
    /* Toggle user LEDs LED0-LED2 */
    err &=  R_IO_PORT_Modify
        (
          PDL_IO_PORT_1_4|PDL_IO_PORT_1_5|PDL_IO_PORT_1_6,
          PDL_IO_PORT_XOR,
          1
        );
    
    /* Reset the synchronisation flag to indicate that the user LEDs
       are synchronised */
    gLEDsync_flg = 0x0;
  }

  /* Toggle user LEDs LED0-LED3 */
  err &=  R_IO_PORT_Modify
      (
        PDL_IO_PORT_1_4|PDL_IO_PORT_1_5|PDL_IO_PORT_1_6|\
        PDL_IO_PORT_1_7,
        PDL_IO_PORT_XOR,
        1
      );

  /* Halt in while loop when RPDL errors detected */  
  while(!err);    
}
/*******************************************************************************
* End of function CB_Timer_VDET
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_LVD2_VDET
* Description  : Interrupt handler function for VDET2. Whenever the VCC voltage 
*         is below VDET2 level & above VDET1 level, LED0-LED2 will be
*         turned off & LED3 will be turned ON. Once the VCC voltage level 
*         increases above VDET2, the LEDs LED0-LED2 will start flashing 
*         alternate to LED3 to indicate that the VDET2 interrupt was
*         occurred.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_LVD2_VDET(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Stores the status of voltage detection */
  uint8_t vdet_status=0;
  
  /* Stop the timer */
  err &=  R_CMT_Control
      (
        2,
        PDL_CMT_STOP,
        500E-3
      );

  /* Turn off all LED0-LED2 */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_1_4|PDL_IO_PORT_1_5|PDL_IO_PORT_1_6,
        1
      );

  /* Turn on LED3 */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_1_7,
        0
      );

  /* Wait until the power supply increases above LVD2's detection level */
  while((vdet_status & 0x20) == 0)
  {
    /* Read current status of LVD  */
    err &= R_LVD_GetStatus(&vdet_status);
    while(!err);
  
    /* Set the synchronisation flag to indicate 
         that the user LEDs are no longer synchronised */
    gLEDsync_flg = 0x1;
  }
  
  /* Turn off LED3 */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_1_7,
        1
      );
  
  /* Re-start the timer */
  err &=  R_CMT_Control
      (
        2,
        PDL_CMT_START,
        500E-3
      );

  /* Clear the VDET2 interrupt flag */
  err &=  R_INTC_Write 
      (
        PDL_INTC_REG_IR_LVD_LVD2,
        0
      );  
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);    
}
/*******************************************************************************
* End of function CB_LVD2_VDET
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_NMI_VDET
* Description  : Non-maskable interrupt callback function. This interrupt 
*         function will be triggered when the VCC voltage level falls
*         below VDET1. This function turns off all the LEDs.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_NMI_VDET(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Stores the status of voltage detection */
  uint8_t vdet_status=0;
  
  /* Turn off all LEDs */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_1_4|PDL_IO_PORT_1_5|PDL_IO_PORT_1_6|\
        PDL_IO_PORT_1_7,
        1
      );

  /* Wait until the power supply increases above LVD1's detection level */
  while((vdet_status & 0x02) == 0)
  {
    /* Read current status of LVD  */
    err &= R_LVD_GetStatus(&vdet_status);
    while(!err);
  }

  /* Turn on LED3 */
  err &=  R_IO_PORT_Write
      (
        PDL_IO_PORT_1_7,
        0
      );

  /* Clear the VDET1 interrupt flag set in NMISR register */
  err &=  R_INTC_ControlExtInterrupt
      (
        PDL_INTC_NMI,
        PDL_INTC_CLEAR_LVD1_FLAG
      );  
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);    
}
/*******************************************************************************
* End of function CB_NMI_VDET
*******************************************************************************/

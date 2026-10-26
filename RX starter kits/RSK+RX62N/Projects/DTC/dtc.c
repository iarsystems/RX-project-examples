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
* File Name    : dtc.c 
* Version      : 2.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : Defines DTC transfer functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History      : 23.08.2010 Ver. 1.00 First Release
*         : 18.04.2011 Ver. 2.00 Second Release (Updated to RPDL CS4)
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Defines standard string functions used in this file */
#include <string.h>

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* 10 bit ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* DTC RPDL function definitions */
#include "r_pdl_dtc.h"
/* Interrupt configuration function definitions */
#include "r_pdl_intc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Provides declarations of functions defined in this file */
#include "dtc.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Define the data source for the DTC transfer as the ADC result register */ 
#define DTC_SOURCE (uint16_t *)&AD0.ADDRA

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Reserve an area for the DTC vector table */
#pragma location=0x00015000
/* Declaration of the DTC vector table */
__no_init uint32_t dtc_vector_table[256];

/* Reserve 16bytes for the ADC trigger transfer data area */
uint32_t dtc_adi0_transfer_data[4];
/* Declare the DTC transfer destination array */
uint16_t gDTC_Destination[16];

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* ADC read function prototype declaration */
void CB_Switch(void);

/*******************************************************************************
* Outline      : Init_DTC
* Description  : This function initialises the DTC unit to perform a transfer
*         from the pointer specified as DTC_SOURCE, to each element of
*         the array gDTC_Destination when triggered by an ADC conversion.
*         The DTC unit will transfer to an incremented address until the
*         array is full, then resets its contents to 0 and begin again.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_DTC(void)
{
  /* Declare error flag */
  bool err = true;

  /* Declare number of transfers container variable */
  uint16_t num_transfers = sizeof(gDTC_Destination) / 2u;

  /* Clear the contents of the gDTC_Destination array */
  memset(gDTC_Destination, 0u, sizeof(gDTC_Destination));
    
  /* Configure the DTC unit */
  err &=  R_DTC_Set
      (
        PDL_DTC_READ_SKIP_DISABLE|PDL_DTC_ADDRESS_FULL,
        dtc_vector_table
      );
  
  /* Configure DTC transfer settings */
  err &=  R_DTC_Create
      (
        PDL_DTC_NORMAL|PDL_DTC_SOURCE_ADDRESS_FIXED
          |PDL_DTC_DESTINATION_ADDRESS_PLUS|PDL_DTC_SIZE_16
          |PDL_DTC_TRIGGER_ADI0,
        dtc_adi0_transfer_data,
        DTC_SOURCE,
        gDTC_Destination,
        num_transfers,
        PDL_NO_DATA
      );
        
  /* Configure the ADC unit */
  err &=  R_ADC_10_Create
      (
        0,
        PDL_ADC_10_MODE_SINGLE|PDL_ADC_10_CHANNELS_OPTION_1
          |PDL_ADC_10_TRIGGER_SOFTWARE|PDL_ADC_10_DTC_TRIGGER_ENABLE,
        48E6,
        4E-6,
        PDL_NO_FUNC,
        0
      );
  
  /* Enable trigger reception for the DTC unit */
  err &=  R_DTC_Control
      (
        PDL_DTC_START,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_DATA,
        PDL_NO_DATA
      );
  
  /* Configure switch callback function */
  SetSwitchReleaseCallback(CB_Switch);
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Init_DTC
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Switch
* Description  : Switch callback function. This function checks if switch SW3
*         was pressed, and fetches the status of the DTC transfer. If 
*         there are no more remaining transfer, the function clears the
*         destination array and re-enables the DTC transfers from the 
*         start of the destination array. The function then triggers an
*         ADC conversion. If there are remaining transfers, it simply
*         triggers an ADC conversion then exits.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_Switch(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* DTC transfer count temporary storage variable */
  uint16_t dtc_transfer_count = 0x0000;
  
  /* Check if switch pressed was SW3 */
  if(gSwitchFlag & SWITCHPRESS_3)
  {
    /* Fetch the remaining number of DTC transfers left */
    err &=  R_DTC_GetStatus
        (
          dtc_adi0_transfer_data,
          PDL_NO_PTR,
          PDL_NO_PTR,
          PDL_NO_PTR,
          &dtc_transfer_count,
          PDL_NO_PTR
        );
      
    if(!dtc_transfer_count)
    {
      /* Declare number of transfers temporary container variable */
      uint16_t num_transfers = sizeof(gDTC_Destination) / 2u;
      
      /* Clear the contents of the gDTC_Destination array */
      memset(gDTC_Destination, 0u, sizeof(gDTC_Destination));
      
      /* Re-enable DTC transfers */
      err &=  R_DTC_Control
          (
            PDL_DTC_UPDATE_DESTINATION|PDL_DTC_UPDATE_COUNT
              |PDL_DTC_TRIGGER_ADI0,
            dtc_adi0_transfer_data,
            PDL_NO_DATA,
            gDTC_Destination,
            num_transfers,
            PDL_NO_DATA
          );
          
      /* Clear the interrupt request */
      err &=  R_INTC_Write
           (
              PDL_INTC_REG_IR_AD0_ADI,
              0
           );  
    }
      
    /* Trigger AD conversion */
    err &=  R_ADC_10_Control
        (
          PDL_ADC_10_0_ON
        );

  }  
    
  /* Clear switch press flags */
  gSwitchFlag &= 0x0F;
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function CB_Switch
*******************************************************************************/

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
* File Name    : dmac.c 
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : Defines DMAC functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History     : 23.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* DMAC RPDL function definitions */
#include "r_pdl_dmac.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "dmac.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Data switching function prototype declaration */
void CB_ChangeDataInBuffer(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
uint8_t gAlternateData[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                              'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                              'U', 'V', 'W', 'X', 'Y', 'Z'};
uint8_t gAlternateDataIndex = 0;

/*******************************************************************************
* Imported Global Variables
*******************************************************************************/
/* Imported global variable, DMA transfer destination array */
extern uint8_t gDMA_DataBuff[1024];
/* Imported global variable, DMA transfer source variable */
extern uint8_t gDMA_DataSource;
/* Switch flag which indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Outline      : Init_DMAC
* Description  : This function configures channel 1 for a single block 
*         software-triggered transfer.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_DMAC(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Configure the DMAC to perform a single block transfer of 1024 bytes, from
     a single byte repeat address area to an incremented destination. Transfer
     is software-triggered */ 
  err &=   R_DMAC_Create
      (
        1,
        PDL_DMAC_BLOCK|PDL_DMAC_SOURCE|PDL_DMAC_SOURCE_ADDRESS_FIXED 
          |PDL_DMAC_DESTINATION_ADDRESS_PLUS|PDL_DMAC_SIZE_8,
        PDL_DMAC_TRIGGER_SW,
        &gDMA_DataSource,
        gDMA_DataBuff,
        1,
        0,        
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_FUNC,
        0
      );
    
  /* Enable and trigger the DMAC transfer */    
  err &=  R_DMAC_Control
      (
        1,
        PDL_DMAC_ENABLE|PDL_DMAC_START,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_DATA, 
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA
      );
      
  /* Configure switch callback function */
  SetSwitchReleaseCallback(CB_ChangeDataInBuffer);

  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Init_DMAC
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_ChangeDataInBuffer
* Description  : Switch callback function. This function checks if switch SW3
*                was pressed, and changes the contents of the variable
*                'gDMA_DataSource' by walking throgh an array of data elements.
*                A DMAC transfer is initiated to update 'gDMA_DataBuff'.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_ChangeDataInBuffer(void)
{
  /* Check if switch pressed was SW3 */
  if(gSwitchFlag & SWITCHPRESS_3)
  {
    if (gAlternateDataIndex > 25)
      gAlternateDataIndex = 0;
    
    gDMA_DataSource = gAlternateData[gAlternateDataIndex];
    gAlternateDataIndex++;
   
    /* Trigger new DMAC transfer */    
    R_DMAC_Control
        (
          1,
          PDL_DMAC_ENABLE|PDL_DMAC_START|PDL_DMAC_UPDATE_SOURCE
            |PDL_DMAC_UPDATE_DESTINATION|PDL_DMAC_UPDATE_COUNT
            |PDL_DMAC_UPDATE_SIZE,
          &gDMA_DataSource,
          gDMA_DataBuff,
          1, 
          0,
          PDL_NO_DATA,
          PDL_NO_DATA,
          PDL_NO_DATA
        );
  }

  /* Clear switch press flags */
  gSwitchFlag &= 0x0F;
}
/*******************************************************************************
* End of function CB_SwitchDataInBuffer
*******************************************************************************/

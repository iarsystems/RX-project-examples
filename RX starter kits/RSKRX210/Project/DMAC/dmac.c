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
* File Name    : dmac.c
* Version     : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description   : Defines ADC repeat functions used for 12bit ADC unit.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_dmac.h"
/* INTC RPDL function definitions */
#include "r_pdl_intc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Provides declarations of functions defined in this file */
#include "dmac.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* DMAC end of transfer callback function prototype */
void CB_DMACTransferEnd_DMAC(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* DMAC transfer destination array */
uint8_t gDMA_DataBuff[DMA_BUFFER_SIZE];
/* DMAC transfer source variable */
uint8_t gDMA_DataSource = 'X';

/*******************************************************************************
* Outline      : Init_DMAC
* Description  : This function configures DMAC channel 0 for a single block 
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
        0,
        PDL_DMAC_BLOCK | PDL_DMAC_SOURCE | PDL_DMAC_SIZE_8 |\
        PDL_DMAC_IRQ_END| PDL_DMAC_SOURCE_ADDRESS_FIXED |\
        PDL_DMAC_DESTINATION_ADDRESS_PLUS, 
        PDL_DMAC_TRIGGER_SW,
        &gDMA_DataSource,
        gDMA_DataBuff,
        1,
        sizeof(gDMA_DataBuff),
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        CB_DMACTransferEnd_DMAC,
        6
      );

  /* Enable the DMAC interrupt  */
  err &=  R_INTC_Modify
      (
        PDL_INTC_REG_IER18,
        PDL_INTC_OR,
        0x40
      );
    
  /* Enable and trigger the DMAC transfer */    
  err &=  R_DMAC_Control
      (
        0,
        PDL_DMAC_ENABLE | PDL_DMAC_START,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_DATA, 
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA
      );
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Init_DMAC
*******************************************************************************/

/*******************************************************************************
* Function Name: CB_DMACTransferEnd_DMAC
* Description  : Callback function called on the completion of the block 
*         transfer. The status of the DMAC is read and if the transfer 
*         was completed successfully, LED1 is turned on. 
* Arguments    : none
* Return Value : none
*******************************************************************************/
void CB_DMACTransferEnd_DMAC(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Declare a variable to store the status of DMAC */
  uint8_t dmac_status=0;

  /* Read the status for channel 0 */
  err &=   R_DMAC_GetStatus
      (
        0,
        &dmac_status,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_PTR
      );

  /* Check the transfer status */
  if(dmac_status & 0x04) 
  {
    /* Turn on LED1 to indicate end of transfer */
    err &=  R_IO_PORT_Write
        (
          PDL_IO_PORT_1_5,
          0
        );
  }
  
  /* Clear the DMAC transfer end flag in DMSTS register. */
  err &=   R_DMAC_Control 
      (
        0,
        PDL_DMAC_CLEAR_DTIF,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA
      );

  /* Halt in while loop when RPDL errors detected */
  while(!err);
}
/*******************************************************************************
* End of function CB_DMACTransferEnd_DMAC
*******************************************************************************/

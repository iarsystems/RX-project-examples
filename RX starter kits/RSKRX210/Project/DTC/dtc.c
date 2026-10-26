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
* File Name      : dtc.c 
* Version        : 1.00
* Device     : R5F52108DFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX210
* Description    : Defines DTC transfer functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History     : 05.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines standard string functions used in this file */
#include <string.h>
/* ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* DTC RPDL function definitions */
#include "r_pdl_dtc.h"
/* Interrupt configuration function definitions */
#include "r_pdl_intc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Declares prototypes defined in this file */
#include "dtc.h"

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/* Switch read function prototype declaration */
void CB_Switch_DTC(void);

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Define the data source for the DTC transfer as the ADC result register */ 
#define DTC_SOURCE ((uint16_t *)&S12AD.ADDR0)

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Reserve an area for the DTC vector table */
#pragma location=0x00002000
/* Declaration of the DTC vector table */
uint32_t dtc_vector_table[256];
/* Reserve 16 bytes for the ADC trigger transfer data area */
static uint32_t dtc_adi0_transfer_data[4];
/* Declare the DTC transfer destination array */
extern uint16_t gDTC_Destination[16];
/* Declare number of transfers container variable */
const uint16_t num_transfers = sizeof(gDTC_Destination) / 2u;

/*******************************************************************************
* Outline      : Init_DTC
* Description  : This function initialises the DTC unit to perform a transfer
*         from the pointer specified as 'DTC_SOURCE', to each element of
*         the array 'gDTC_Destination' when triggered by an ADC 
*         conversion. The DTC unit will transfer to an incremented 
*         address until the array is full, then resets its contents to 0
*         and begin again.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_DTC(void)
{
  /* Declare error flag */
  bool err = true;

  /* Clear the contents of the 'gDTC_Destination' array */
  if(memset(gDTC_Destination, 0u, sizeof(gDTC_Destination)) == NULL)
  {
    /* Halt in while loop when errors detected */
    while(1);
  }

  /* Configure the ADC pin to be used */
  err &=  R_ADC_12_Set
      (
        PDL_ADC_12_PIN_AN000_P40
      );
  
  /* Configure the 12-bit ADC unit */
  err &=  R_ADC_12_CreateUnit
      (
        0,
        PDL_ADC_12_SCAN_SINGLE,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_DATA,
        PDL_NO_FUNC,
        PDL_NO_DATA,
        PDL_NO_FUNC,
        PDL_NO_DATA
      );    

  /* Configure the ADC channel AN000
     Use the sampling time as 5 uS  */
  err &=  R_ADC_12_CreateChannel
      (
        0,
        0,
        PDL_ADC_12_CH_GROUP_A,
        5E-6
      );
      
  /* Configure the DTC unit */
  err &=  R_DTC_Set
      (
        PDL_DTC_ADDRESS_SHORT,
        dtc_vector_table
      );
  
  /*   Configure DTC transfer settings as follows -
    Fixed source address
    Incrementing destination address
    DTC trigger - ADC conversion end
    DTC source - ADC result register
    DTC destination - memory buffer 'gDTC_Destination'
    Transfer 16 ADC readings     */
  err &=  R_DTC_Create
      (
        PDL_DTC_NORMAL | PDL_DTC_SOURCE_ADDRESS_FIXED | \
        PDL_DTC_DESTINATION_ADDRESS_PLUS |PDL_DTC_SIZE_16 | \
        PDL_DTC_TRIGGER_ADC12,         
        dtc_adi0_transfer_data,
        DTC_SOURCE,
        gDTC_Destination,
        num_transfers,
        PDL_NO_DATA
      );  

  /* Enable the DTC transfer */
  err &=  R_DTC_Control
      (
        PDL_DTC_START,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_PTR,
        PDL_NO_DATA,
        PDL_NO_DATA
      );

  /* Enable DTC trigger on ADC conversion end */
  err &=  R_INTC_Write
      (
        PDL_INTC_REG_DTCER_S12AD0_S12ADI,
        1
      );

  /* Enable the DTC interrupt  */
  err &=  R_INTC_Modify
      (
        PDL_INTC_REG_IER0C,
        PDL_INTC_OR,
        0x40
      );
  
  /* Configure switch callback function */
  SetSwitchReleaseCallback(CB_Switch_DTC);
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function Init_DTC
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_Switch_DTC
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
void CB_Switch_DTC(void)
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
      
    /* If required number of DTC transfers are complete, clear the
       buffer & re-start DTC */
    if(!dtc_transfer_count)
    {
      /* Toggle LED0 once the DTC completes transferring 16 bytes */
      err &=   R_IO_PORT_Modify
          (
            PDL_IO_PORT_1_4,
            PDL_IO_PORT_XOR,
            1
          );      
      
      /* Stop DTC transfers */
      err &=  R_DTC_Control
          (
            PDL_DTC_STOP,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_DATA,
            PDL_NO_DATA
          );

      /* Clear the contents of the gDTC_Destination array */
      if(memset(gDTC_Destination, 0u,(size_t) 
          sizeof(gDTC_Destination)) == NULL)
      {
        /* Halt in while loop when errors detected */
        while(1);
      }  
        
      /* Clear the ADC interrupt flag */
      err &=  R_INTC_Write 
          (
            PDL_INTC_REG_IR_S12AD0_S12ADI,
            0
          );

      /* Re-configure the DTC  */
      err &=  R_DTC_Control
          (
            PDL_DTC_STOP | PDL_DTC_TRIGGER_ADC12|\
            PDL_DTC_UPDATE_DESTINATION | PDL_DTC_UPDATE_COUNT,
            dtc_adi0_transfer_data,
            PDL_NO_PTR,
            gDTC_Destination,
            num_transfers,
            PDL_NO_DATA
          );

      /* Enable the DTC transfer */
      err &=  R_DTC_Control
          (
            PDL_DTC_START,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_PTR,
            PDL_NO_DATA,
            PDL_NO_DATA
          );
    }
      
    /* Start AD conversion */
    err &=  R_ADC_12_Control
        (
          PDL_ADC_12_0_ON
        );
  }
    
  /* Clear switch press flags */
  gSwitchFlag &= 0x0F;
      
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function CB_Switch_DTC
*******************************************************************************/

/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WAWRRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : dtc.c
* Version       : 1.1
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : Defines DTC transfer functions used in this sample.
* Limitations   : None
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 08.Oct.2010     1.00        First release
*         : 02.Dec.2010     1.10        Second YRDK release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard string functions used in this file */
#include <string.h>
/* 10 bit ADC RPDL function definitions */
#include "r_pdl_adc_12.h"
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
#define DTC_SOURCE (uint16_t *)&S12AD.ADDR4

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Reserve an area for the DTC vector table */
#pragma location = 0x00015000
/* Declaration of the DTC vector table */
uint32_t dtc_vector_table[256];
/* Reserve 16bytes for the ADC trigger transfer data area */
uint32_t dtc_ad12_transfer_data[4];
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
*                from the pointer specified as DTC_SOURCE, to each element of
*                the array gDTC_Destination when triggered by an ADC conversion.
*                The DTC unit will transfer to an incremented address until the
*                array is full, then resets its contents to 0 and begin again.
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
  err &=    R_DTC_Set
    (
    PDL_DTC_READ_SKIP_DISABLE | PDL_DTC_ADDRESS_FULL, // configuration options
    dtc_vector_table                                  // vector table base address
    );

  /* Configure DTC transfer settings */
  err &=    R_DTC_Create
    (
    PDL_DTC_NORMAL | PDL_DTC_SOURCE_ADDRESS_FIXED // configuration selection
    | PDL_DTC_DESTINATION_ADDRESS_PLUS | PDL_DTC_SIZE_16
    | PDL_DTC_TRIGGER_ADC12,
    dtc_ad12_transfer_data, // transfer data start address
    DTC_SOURCE,             // source        start address
    gDTC_Destination,       // destination   start address
    num_transfers,          // transfer count
    PDL_NO_DATA             // block size
    );

  /* Configure the ADC unit */
  err &=    R_ADC_12_Create
    (
    0,                           // unit selection
    PDL_ADC_12_SCAN_SINGLE | PDL_ADC_12_CHANNEL_4 | PDL_ADC_12_DATA_ALIGNMENT_RIGHT | // conversion options
    PDL_ADC_12_DIV_8 | PDL_ADC_12_DTC_TRIGGER_ENABLE,
    PDL_ADC_12_TRIGGER_SOFTWARE, // trigger selection
    PDL_NO_DATA,                 // value addition mode options
    PDL_NO_FUNC,                 // callback function
    0                            // interrupt priority level
    );

  /* Enable trigger reception for the DTC unit */
  err &=    R_DTC_Control
    (
    PDL_DTC_START, // control options
    PDL_NO_PTR,    // transfer data start address
    PDL_NO_PTR,    // source        start address
    PDL_NO_PTR,    // destination   start address
    PDL_NO_DATA,   // transfer count
    PDL_NO_DATA    // block size
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
*                was pressed, and fetches the status of the DTC transfer. If
*                there are no more remaining transfer, the function clears the
*                destination array and re-enables the DTC transfers from the
*                start of the destination array. The function then triggers an
*                ADC conversion. If there are remaining transfers, it simply
*                triggers an ADC conversion then exits.
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
    err &=    R_DTC_GetStatus
      (
      dtc_ad12_transfer_data, // transfer data start address pointer
      PDL_NO_PTR,             // status  flags               pointer
      PDL_NO_PTR,             // current source      address pointer
      PDL_NO_PTR,             // current destination address pointer
      &dtc_transfer_count,    // current transfer    count   pointer
      PDL_NO_PTR              // current block size  count   pointer
      );

    if(!dtc_transfer_count)
    {
      /* Declare number of transfers temporary container variable */
      uint16_t num_transfers = sizeof(gDTC_Destination) / 2u;

      /* Clear the contents of the gDTC_Destination array */
      memset(gDTC_Destination, 0u, sizeof(gDTC_Destination));

      /* Re-enable DTC transfers */
      err &=    R_DTC_Control
        (
        PDL_DTC_UPDATE_DESTINATION | PDL_DTC_UPDATE_COUNT
        | PDL_DTC_TRIGGER_ADC12,
        dtc_ad12_transfer_data,
        PDL_NO_DATA,
        gDTC_Destination,
        num_transfers,
        PDL_NO_DATA
        );

      /* Clear the interrupt request */
      err &=    R_INTC_Write
        (
        PDL_INTC_REG_IR_S12AD_ADI,
        0
        );
    }

    /* Trigger AD conversion */
    err &=    R_ADC_12_Control
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
* End of function CB_Switch
*******************************************************************************/

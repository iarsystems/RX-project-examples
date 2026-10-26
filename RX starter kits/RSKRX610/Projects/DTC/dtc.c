/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER:
* The software supplied by Renesas Technology Europe Ltd is
* intended and supplied for use on Renesas Technology products.
* This software is owned by Renesas Technology Europe, Ltd. Or
* Renesas Technology Corporation and is protected under applicable
* copyright laws. All rights are reserved.
*
* THIS SOFTWARE IS PROVIDED "AS IS". NO WARRANTIES, WHETHER EXPRESS,
* IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* APPLY TO THIS SOFTWARE. RENESAS TECHNOLOGY AMERICA, INC. AND
* AND RENESAS TECHNOLOGY CORPORATION RESERVE THE RIGHT, WITHOUT
* NOTICE, TO MAKE CHANGES TO THIS SOFTWARE. NEITHER RENESAS
* TECHNOLOGY AMERICA, INC. NOR RENESAS TECHNOLOGY CORPORATION SHALL,
* IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
* CONSEQUENTIAL DAMAGES FOR ANY REASON WHATSOEVER ARISING OUT OF THE
* USE OR APPLICATION OF THIS SOFTWARE.
******************************************************************************/
/* Copyright (C) 2009. Renesas Technology Europe, All Rights Reserved */
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : dtc.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Provides DTC functions    
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/


/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* ADC RPDL function definitions */
#include "r_pdl_adc_10.h"
/* SCI RPDL function definitions */
#include "r_pdl_sci.h"
/* Interrupt RPDL function definitions */
#include "r_pdl_intc.h"
/* Timer RPDL function definitions */
#include "r_pdl_tmr.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* DTC RPDL funcion definitions */
#include "r_pdl_dtc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Declares prototypes defined in this file */
#include "dtc.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Declare destination array for DTC transfer */
extern short gDTC_DataBuff[256];
/* Reserve area for the DTC vector table */
#pragma data_alignment=4096
uint32_t dtc_vector_table[256];
/* Declare data transfer area for adc0 trigger */
uint32_t dtc_adc0_transfer_data[4];


/***********************************************************************************
Function Name : Init_DTC
Description    : Function initialises the ADC and the DTC for trigger operation.  
Parameters    : none
Return value  : none
***********************************************************************************/
void Init_DTC(void)
{  
  /* Set global DTC options */
  R_DTC_Set(0, dtc_vector_table);
  
  /* Intialise DTC transfer settings */
  R_DTC_Create(PDL_DTC_BLOCK|PDL_DTC_SOURCE_ADDRESS_FIXED|
            PDL_DTC_SOURCE|PDL_DTC_DESTINATION_ADDRESS_PLUS|
         PDL_DTC_SIZE_16|PDL_DTC_TRIGGER_ADI0, dtc_adc0_transfer_data,
         (uint8_t *)&AD0.ADDRA, gDTC_DataBuff, 0, 1);

  /* Initialise ADC peripheral */
  R_ADC_10_Create(0, PDL_ADC_10_DTC_TRIGGER_ENABLE, 50E6, 6E-7, PDL_NO_FUNC, 0);
    
  /* Enable the DTC */
  R_DTC_Control(PDL_DTC_START);
}
/***********************************************************************************
End Init_DTC function
***********************************************************************************/


/***********************************************************************************
Function Name : ADC_Run
Description    : Function is called on switch 1 keypress. Function re-enables DTC
        trigger reception, and initiates an AD converesion.
Parameters    : none
Return value  : none
***********************************************************************************/
void ADC_Run(void)
{
  /* Initiate AD conversion */
  R_ADC_10_Control(PDL_ADC_10_0_ON);
}
/***********************************************************************************
End ADC_Run function
***********************************************************************************/

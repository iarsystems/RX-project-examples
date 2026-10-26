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
* File Name    : dmac.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Provides DMAC functions    
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
/* DMAC RPDL function definitions */
#include "r_pdl_dmac.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Declares prototypes defined in this file */
#include "dmac.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
/* Declares global variable to write DMAC data to */
extern char gDMA_DataBuff[1024];
/* Defines the data source to be written via DMAC, the word 'BEEF' */
char DMA_DataSource[] = {0xFF, 0xEE};


/*””FUNC COMMENT””*************************************************************
* Outline     : Init_DMAC
* Description   : Initializes the DMAC peripheral
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void Init_DMAC(void)
{  
  /* Intialise DMAC transfer settings */
  R_DMAC_Create(0, PDL_DMAC_DESTINATION_ADDRESS_PLUS|PDL_DMAC_SIZE_16|
          PDL_DMAC_CONSECUTIVE, PDL_DMAC_REQUEST_SW, DMA_DataSource, 
          gDMA_DataBuff, 1024, 0, 0, 0, DMAC_callback, 3);
  
  /* Begin DMA transfer */
  R_DMAC_Control(PDL_DMAC_0, PDL_DMAC_ENABLE|PDL_DMAC_START, 0, 0, 0, 0);
}
/******************************************************************************
End of function Init_DMAC
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : DMAC_callback
* Description   : Callback function that executes after a succesful DMAC
*          transfer. Function lights LED to indicate transfer completion.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void DMAC_callback(void)
{
  /* Light LED to indicate transfer complete */
  R_IO_PORT_Modify(PDL_IO_PORT_8, PDL_IO_PORT_AND, 0xEF);
}
/******************************************************************************
End of function DMAC_callback 
******************************************************************************/

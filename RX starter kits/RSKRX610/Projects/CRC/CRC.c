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
* File Name    : crc.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Provides CRC generation functionality
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
/* CRC RPDL function definitions */
#include "r_pdl_crc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Declares functions defined in this file */
#include "CRC.h"


/*””FUNC COMMENT””*************************************************************
* Outline     : CRC_Init
* Description   : Initialises the CRC unit
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void CRC_Init(void)
{
  /* Initialise the CRC calculator module */
  R_CRC_Create(PDL_CRC_POLY_CRC_8|PDL_CRC_LSB_FIRST);
}
/******************************************************************************
End of function main
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : CRC_Generate
* Description   : Generates a CRC checksum from a char input
* Argument    : char (Value to create checksum for)
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
short CRC_Generate(uint8_t input)
{
  /* Declare function output variable */
  uint16_t CRC_Result;
  
  /* Write input variable to the CRC module */
  R_CRC_Write(input);  
  
  /* Read checksum from CRC module */
  R_CRC_Read(&CRC_Result, PDL_CRC_CLEAR_RESULT);
  
  /* Return checksum value */
  return CRC_Result;
}
/******************************************************************************
End of function CRC_Generate
******************************************************************************/

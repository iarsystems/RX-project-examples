/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized.
 * This software is owned by Renesas Electronics Corporation and is  protected
 * under all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
 * REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
 * INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR  A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE  EXPRESSLY
 * DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
 * FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
 * AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this
 * software and to discontinue the availability of this software.
 * By using this software, you agree to the additional terms and
 * conditions found by accessing the following link:
 * http://www.renesas.com/disclaimer
 ******************************************************************************/
 /* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.*/
 
/*””FILE COMMENT””******************************* Technical reference data ****
* File Name    : async.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62N
* Description   : File provides async serial output        
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
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Declares functions defined in this file */
#include "async.h"


/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
#define UART_CHANNEL     2
#define UART_BAUD       115200
#define UART_INT_PRIORITY  5

/*””FUNC COMMENT””*************************************************************
* Outline     : UART_Init
* Description   : Initialises UART 2
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void UART_Init(void)
{  
  /* Initialise UART device */
  
  /* Configure the pin selection of SCI */
   R_SCI_Set(PDL_SCI_PIN_SCI2_A);

  /* Configure the RS232 port */
  R_SCI_Create(UART_CHANNEL, PDL_SCI_8N1, UART_BAUD, UART_INT_PRIORITY);
}
/******************************************************************************
End of function UART_Init
******************************************************************************/

/*””FUNC COMMENT””*************************************************************
* Outline     : charput
* Description   : Write character out.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void __write(unsigned char ch)
{
  R_SCI_Send(UART_CHANNEL, 0, &ch, 0, PDL_NO_FUNC);
}

/*””FUNC COMMENT””*************************************************************
* Outline     : charget
* Description   : Not implemented but required by stdio
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
unsigned char charget(void)
{
  return '!';
}

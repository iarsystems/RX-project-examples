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
* File Name    : sync_serial.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : Defines functions used for synchronus serial function
*         
******************************************************************************/

/******************************************************************************
* History     : 30.10.2008 Ver. 1.00 First Release
*””FILE COMMENT END””*********************************************************/

/******************************************************************************
User Includes
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
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Declares functions defined in this file */
#include "sync_serial.h"


/******************************************************************************
Global Variables
******************************************************************************/
/* Transmit and receive data buffers */
extern uint8_t gSCI_TransmitBuffer[8], gSCI_ReceiveBuffer[8];


/*””FUNC COMMENT””*************************************************************
* Outline     : SCI_Init
* Description   : Serial communications interface intialisation routine. The 
*          function sets channel 1 and 3 for synchronus serial 
*          communications, at 19200 bit/s.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SCI_Init(void)
{
  /* Initialise Transmitting UART device, Ch1 */
  R_SCI_Create(1, PDL_SCI_SYNC, 19200, 5);
        
  /* Initialise Receiving UART device, Ch3 */
  R_SCI_Create(3, PDL_SCI_SYNC|PDL_SCI_CLK_EXT, 19200, 5);
  
  /* Initialise UART receive interrupt, with SCI_RX_callback set as the
     receive callback function */
  R_SCI_Receive(3, PDL_NO_DATA, gSCI_ReceiveBuffer, 8, SCI_RX_callback, PDL_NO_FUNC);
}
/******************************************************************************
End of function SCI_Init
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : SCI_Send
* Description   : Function transmits the contents of the transmit data buffer, 
          gSCU_TransmitBuffer from SCI channel 1 to channel 3. Lights
          LED 1 to indicate transmission in progress.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SCI_Send(void)
{
  /* Light LED to signal Tx */
  R_IO_PORT_Modify(PDL_IO_PORT_8_4, PDL_IO_PORT_AND, 0);
  R_IO_PORT_Modify(PDL_IO_PORT_8_3, PDL_IO_PORT_OR, 1);

  /* Send message in loop back over SCI */
  R_SCI_Send(1, 0, gSCI_TransmitBuffer, 0, PDL_NO_FUNC);  
}
/******************************************************************************
End of function SCI_Send
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : SCI_RX_callback
* Description   : Callback function that is executed when the required number of
*          bytes has been received by the SCI module. Function lights 
*          LED 0 to indicate transmission complete, and displays the
*          received data on the LCD display.
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void SCI_RX_callback(void)
{
  /* Light LED to signal Rx led no. */
  R_IO_PORT_Modify(PDL_IO_PORT_8_3, PDL_IO_PORT_AND, 0);
  R_IO_PORT_Modify(PDL_IO_PORT_8_4, PDL_IO_PORT_OR, 1);
  
  /* Display recieved message onto LCD */
  DisplayLCD(LCD_LINE2, gSCI_ReceiveBuffer);
}
/******************************************************************************
End of function SCI_RX_callback
******************************************************************************/

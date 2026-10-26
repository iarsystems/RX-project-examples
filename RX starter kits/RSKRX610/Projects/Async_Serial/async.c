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
* File Name    : async.c
* Version     : 1.00
* Device     : R5F56107VNFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX610
* Description   : File provides async serial functionality         
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
/* rx_flag variable indicates when 'z' has been pressed on the terminal */
unsigned short rx_flag, count = 0x0, test;
/* Declares transmit and receive buffer variables used in this file */
uint8_t tx_buf[2], rx_buf;
/* Defines UART commands used to clear the screen, and return the cursor home */
const uint8_t cmd_clr_scr[] = {27,'[','2','J',0};
const uint8_t cmd_cur_home[] = {27,'[','H',0};


/*””FUNC COMMENT””*************************************************************
* Outline     : UART_Init
* Description   : Initialises the UART device and settings
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void UART_Init(void)
{
  /* Set the Tx Pin Data Direction */
  R_IO_PORT_Set(PDL_IO_PORT_2_6, PDL_IO_PORT_OUTPUT, 0, 0, 0);
  
  /* Set the Rx Pin Data Direction */
  R_IO_PORT_Set(PDL_IO_PORT_2_5, PDL_IO_PORT_INPUT, 0, 0, 0);
  
  /* Initialise UART device */
  R_SCI_Create(1, PDL_SCI_8N1, 19200, 6);
  
  /* Initialise UART recieve interrupt */
  R_SCI_Receive(1, PDL_NO_DATA, &rx_buf, 1, rx_callback, PDL_NO_FUNC);
    
  /* Clear the terminal screen, and return the cursor to the start position */
  R_SCI_Send(1, 0, cmd_clr_scr, 0, PDL_NO_FUNC);
  R_SCI_Send(1, 0, cmd_cur_home, 0, PDL_NO_FUNC);
  
  /* Display splash screen on terminal window */
  R_SCI_Send(1, 0, "Renesas RX610 UART Test \n\r", 0, PDL_NO_FUNC);
  R_SCI_Send(1, 0, "Press 'z' to stop and any key to resume\r\n\n", 0,
        PDL_NO_FUNC);
  
  /* Initialise the transmit buffer array to end in a NULL character */
  tx_buf[1] = (char) 0x0;

}
/******************************************************************************
End of function UART_Init
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : Text_Write
* Description   : Sends an incrementing numbers across the UART link to the 
          terminal window
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void Text_Write(void)
{  
  /* Initialises the delay variable */
  unsigned long delay = 0xFFFFF;
  
  /* Load the Transmit buffer */
  tx_buf[0] = (char) 48 + count;
  
  /* Send the Transmit buffer */
  R_SCI_Send(1, 0, tx_buf, 0, PDL_NO_FUNC);
    
  /* Increment the counting variable, count */
  count++;

  /* Loop the counting variable back to 0 after 9, and start a new line*/
  if(count > 0x9)
  {
    /* Reset counting variable */
    count = 0;
    
    /* Start a new line and return the cursor on the terminal window */
    R_SCI_Send(1, 0, "\n\r", 0, PDL_NO_FUNC);
  }
      
  /* Turn on LED0 to indicate UART TX */
  R_IO_PORT_Write(PDL_IO_PORT_8_3, 0);
  R_IO_PORT_Write(PDL_IO_PORT_8_4, 1);
    
  /* Delay slows the speed at which characters are written to the terminal */  
  while(delay--);
}
/******************************************************************************
End of function Text_Write
******************************************************************************/


/*””FUNC COMMENT””*************************************************************
* Outline     : rx_callback
* Description   : Callback function, called when a character is received from
          the terminal program. Sets rx_flag if character is 'z'
* Argument    : none
* Return value  : none
*””FUNC COMMENT END””*********************************************************/
void rx_callback(void)
{  
  /* If recieved character is z, set z key recieve flag to 1 */
  if(rx_buf == 'z')
  {
    /* Set receive flag */
    rx_flag = 1;
  }
  /* If z has already been pressed, accept any key to resume */
  else if(rx_flag)
  {
    /* Clear receive flag */
    rx_flag = 0;
  }
  
  /* Reinitialise UART recieve interrupt */
  R_SCI_Receive(1, PDL_NO_DATA, &rx_buf, 1, rx_callback, PDL_NO_FUNC);
}
/******************************************************************************
End of function rx_callback
******************************************************************************/

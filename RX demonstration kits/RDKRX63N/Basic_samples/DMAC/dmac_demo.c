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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : dmac_demo.c
* Version       : 1.0
* Device(s)     : R5F562N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX62N
* Description   : This contains the sample DMAC demo application. The application
*                 initializes an ADC channel, a MTU timer to trigger it, and 
*                 a DMAC channel to transfer the ADC results. The 
*                 ADC conversion triggers the DMAC unit to perform a single transfer
*                 from the ADC result register to a destination buffer. The DMAC
*                 increments the destination address with each transfer until the
*                 requested number of transfers has been made, which also fills 
*                 the buffer. When the DMAC completion interrupt is received, 
*                 the demo uses a callback to switch the DMAC destination buffer
*                 to an alternate buffer and resets the DMAC transfer request count.       
*                 The process then continually repeats, with the 2nd buffer being 
*                 filled, and then back to the first buffer, and so on.
*
*                 This demonstrates how the DMAC can be used with a double buffering
*                 scheme to provide a continuous stream of data where an 
*                 application can access the data in the idle buffer while the DMAC
*                 is actively filling the alternate buffer. 
*                 Note:      
*                 
* Limitations:    A suitable blocking scheme should be implemented to avoid 
*                 simultaneous access to the same buffer by the DMAC and the CPU. 
*******************************************************************************
* History : DD.MM.YYYY   Version    Description
*         : 22.09.2011   1.00       First release
*           20.01.2012              CS revisions. Ported to FIT environment.
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <iorx63n.h>

#include "dmac.h"
#include "adc_mtu_timer.h"


/*******************************************************************************
Macro definitions
*******************************************************************************/
#define DMAC_SOURCE   (uint16_t *)&S12AD.ADDR2 /* DTC transfer source is ADC result register */
#define DEST_BUF_SIZE 256    /* 256 16-bit = 512 bytes. */
#define TIMER_COUNT  1000    /* Should give ~20.83 uS with 48MHz pclk = 48kHz sample rate. */


/*******************************************************************************
Imported global variables and functions (from other files)
*******************************************************************************/
extern void S12ADC_init (void);
extern void S12ADC_start(void);


/*******************************************************************************
Private global variables and functions
*******************************************************************************/
uint16_t g_dmac_dest_buf_1[DEST_BUF_SIZE]; // DMAC transfer destination array 1.
uint16_t g_dmac_dest_buf_2[DEST_BUF_SIZE]; // DMAC transfer destination array 2.

uint16_t * g_current_dmac_buf;   // This will track the buffer currently in use. 

static void dmac_interrupt_callback(void);


/*****************************************************************************
* Function Name : dmac_demo_start()
* Description   : This is the entry point for the DMAC demonstration application.
* Arguments     : None
* Return Value  : None
*****************************************************************************/
void dmac_demo_start(void)
{
  uint32_t i;
  
  for (i = 0; i < DEST_BUF_SIZE; i++)
  {
    g_dmac_dest_buf_1[i] = 0x00;  // Clear the transfer destination arrays.
    g_dmac_dest_buf_2[i] = 0x00;  
  }
  
  g_current_dmac_buf = g_dmac_dest_buf_1;  // Start out with buffer 1.
  
  dmac_callback_set(&dmac_interrupt_callback); // Set up for double buffer handling.
  
  adc_mtu_init(TIMER_COUNT); // Initialize the MTU ADC trigger timer, set sample rate.
  
  S12ADC_init();             // Initialize the 12-bit ADC
  S12ADC_start();
  
  dmac_init(VECT_S12AD_S12ADI0, 
            DMAC_SOURCE, 
            g_dmac_dest_buf_1, /* Start out with buffer 1. */
            (uint16_t)DEST_BUF_SIZE);
  
  /* Starting the timer begins the transfer process by triggering the ADC which triggers the DMAC. */
  adc_mtu_start(); 
} /* End of function dmac_demo_start() */


/*****************************************************************************
* Function Name : dmac_interrupt_callback()
* Description   : Switch between alternate DMAC destination buffers.
* Arguments     : None
* Return Value  : None
*
* Limitations:    A suitable blocking scheme should be implemented to avoid 
*                 simultaneous access to the same buffer by the DMAC and the CPU. 
*****************************************************************************/
static void dmac_interrupt_callback(void)
{
  /* Switch between alternate DMAC destination buffers. */
  if (g_current_dmac_buf == g_dmac_dest_buf_1)
  {
    g_current_dmac_buf =  g_dmac_dest_buf_2;
  }
  else if (g_current_dmac_buf == g_dmac_dest_buf_2)
  {
    g_current_dmac_buf =  g_dmac_dest_buf_1;
  }
  else
  {
    while(1) /* bad pointer error. */
    {
    }
  }
  
  DMAC0.DMDAR         = g_current_dmac_buf; /* Change DMAC destination to the 2nd buffer. */
  DMAC0.DMCRA         = DEST_BUF_SIZE; /* Reset the DMAC transfer counter */
  DMAC0.DMCNT.BIT.DTE = 1;             /* Re-enable DMAC transfers. */
} /* End of function dmac_interrupt_callback() */

/*******************************************************************************
* End of file dmac_demo.c
*******************************************************************************/

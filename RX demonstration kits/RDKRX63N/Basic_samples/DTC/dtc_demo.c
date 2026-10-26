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
* File Name     : dtc_demo.c
* Version       : 1.0
* Device(s)     : RX63N
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* H/W Platform  : YRDKRX63N
* Description   : This contains the sample application.
* Limitations   : None
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY   Version    Description
*         : 20.09.2011   1.00       First release
*           23.01.2012              CS revisions
*******************************************************************************/

/*******************************************************************************
Includes <System Includes>, "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <iorx63n.h>
#include "dtc_demo.h"
#include "dtc.h"
#include "cmt_oneshot.h"
#include "S12ADC.h"
#include "lcd.h"


/*******************************************************************************
* Macro definitions
*******************************************************************************/
/* DTC transfer source is ADC result register */
#define DTC_SOURCE (uint16_t *)&S12AD.ADDR2 

/* CMT timer 0 compare match interrupt. */
#define DTC_VECTOR (uint8_t) VECT_CMT0_CMI0 

#define DTC_NUM_TRANSFERS 16

/* Should give 0.1 second with 48MHz pclk / 512 */
#define TIMER_COUNT 9375  


/*******************************************************************************
* Imported global variables and functions (from other files)
*******************************************************************************/
extern void sw1_interrupt_enable(void);


/******************************************************************************
Local function declarations and local global variables
******************************************************************************/
/* DTC transfer destination array. */
uint16_t g_dtc_destination[DTC_NUM_TRANSFERS]; 

static void dtc_demo_end_callback(void);


/*****************************************************************************
* Function Name : DtcDemoStart()
* Description   : This is the entry point for the DTC demonstration application.
* Arguments     : None
* Return Value  : None
*****************************************************************************/
void DtcDemoStart(void)
{
  uint32_t i;
  
  for (i = 0; i < DTC_NUM_TRANSFERS; i++)
  {
    g_dtc_destination[i] = 0x00;  /* Clear the transfer destination array. */
  }
  
  CmtCallbackSet(&dtc_demo_end_callback); /* Have the CMT invoke cleanup when done. */
  CmtInit();             /* Initialize the compare match timer. */
  
  S12ADC_init();         /* Initialize the 12-bit ADC */
  S12ADC_start();        /* Start A to D conversion. */
  
  /* Prepare the DTC peripheral for the data transfer operation. */
  /* In this demo, DTC_SOURCE is the address of the 12-bit A to D converter's 
     16-bit result register. The pointer is passed as a 32-bit value so that 
     it can be written into the DTC's source address register (SAR). 
     g_dtc_destination is the buffer into which data will be transferred. 
     Its pointer is passed as a 32-bit value so that it can be written into
     the DTC's destination address register (DAR). DTC_NUM_TRANSFERS is the 
     count of the number of transfers the DTC is to perform.
  */
  DtcInit(DTC_VECTOR, 
          (uint32_t)DTC_SOURCE, 
          (uint32_t)g_dtc_destination, 
          (uint16_t)DTC_NUM_TRANSFERS);
  DtcStart(DTC_VECTOR);     
  
  lcd_display(LCD_LINE8, "..running...");  
  
  /* This begins the transfer sequence. TIMER_COUNT is the delay constant 
     for the CMT counter to used pace the triggering of the DTC. The DTC is
     performing one transfer for each interrupt from the CMT. To see the 
     DTC run at a different rate, adjust TIMER_COUNT. */
  CmtStart(TIMER_COUNT); 
} /* End of function DtcStop() */


/*****************************************************************************
* Function Name : dtc_demo_end_callback()
* Description   : This is the cleanup for the DTC demonstration application.
* Arguments     : None
* Return Value  : None
*****************************************************************************/
static void dtc_demo_end_callback(void)
{
  lcd_display(LCD_LINE8, "....idle....");  
  sw1_interrupt_enable();
} /* End of function DtcStop() */


/*******************************************************************************
End of file: dtc_demo.c
*******************************************************************************/

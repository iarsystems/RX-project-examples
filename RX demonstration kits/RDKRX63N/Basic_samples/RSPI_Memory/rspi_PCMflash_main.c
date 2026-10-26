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
/******************************************************************************
* File Name     : rspi_PCMflash_main.c
* Version       : 1.0
* Device(s)     : R5F563N8
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.1
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Demonstrates usage of RSPI peripheral to write and read from 
*               : the RSPI attached flash device.
*                 This program writes 2048 bytes of data to the flash device 
*                 located on RSPI-0, 64 bytes at a time.  It then proceeds to
*                 read the data back 512 bytes at a time and compares it to the 
*                 original data.  Upon successful completion, LED 4 is turned on.
*                 However, if the programming fails, LED 14 is turned on.
*
* Operation     : 1) Build and download the sample code to the YRDKRX63N.
*               : 2) Click 'Go'.
*               : 3) Observe messages on LCD.
*               : 4) Observe LEDs for result status.
* 
*******************************************************************************
* History : DD.MMM.YYYY     Version     Description
*         : 22.Sep.2011     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <intrinsics.h>
#include "yrdkrx62n_rspi_api.h"
#include "lcd.h"
#include "flash_demo_functions.h"
#include "yrdkrx63n.h"

/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/

/******************************************************************************
* Function name: main
* Description  : The main program function. 
*                This function initializes the hardware and then performs 
*                an erase/write/read/verify to the M5Q flash device on the SPI.
*                2000 bytes of data gets programmed into the defined sector  
*                of the device. 
*                Results of the sequence get displayed on the LCD and LED4  
*                for success or LED14 for failure. 
*
* Arguments    : none
* Return value : none
*******************************************************************************/
void main(void)
{
  bool     err = true; 
  uint32_t led_counter;   /* Used to pace the LED toggle rate. */
  
  __enable_interrupt();
  
  /* Initialize LCD */
  YRDKRX62N_RSPI_Init();
  lcd_initialize();
  
  /* Clear LCD */
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "  RENESAS   ");
  lcd_display(LCD_LINE2, " YRDKRX63N  ");
  lcd_display(LCD_LINE4, " SPI Flash  ");
  
  /* 
  Open the terminal I/O window to see the output.
  */
  printf("This is the debug console\r\n");
  
  /* Check the SPI flash manufacturer and device ID. */
  err &= PQ5_flash_verify();
  
  /* Display results on the LCD and light an appropriate LED */
  if (err == true)
  {     
    lcd_display(LCD_LINE7, "  verified  ");
  }
  else
  {
    lcd_display(LCD_LINE7, "not verified");
  }
  
  /* Now perform an erase/write/read/verify to the flash device */
  err &= PQ5_flash_write_read_demo();
  
  /* Display results on the LCD and light an appropriate LED */
  if (err == true)
  {     
    lcd_display(LCD_LINE8, "- success! -");
  }
  else
  {
    lcd_display(LCD_LINE8, " - failed - ");
  }
  
  /* Done with demo. Now just toggle status LED periodically */
  while (1)
  {
    for (led_counter = 0; led_counter < 4000000; led_counter++)
    {
    }
    
    if (err == true)
    {
      LED4 = ~LED4; /* Flash LED4 to signal success status. */
    }
    else
    {
      LED5 = ~LED5; /* Flash LED5 to signal failure status. */
    }
  }
} /* End of function main() */

/*******************************************************************************
* End of file: rspi_PCMflash_main.c
*******************************************************************************/

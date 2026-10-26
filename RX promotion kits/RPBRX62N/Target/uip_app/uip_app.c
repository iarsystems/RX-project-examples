/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Technology Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Technology Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
*******************************************************************************  
* File Name    : user-app.c
* Version      : 1.00
* Description  : User application that controls the LEDs from a Web page
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.10.2009 1.00    First Release
*         : 06.04.2010 1.01    RX62N changes
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>

#include "iodefine.h"
#include "rskrx62ndef.h"
#include "uip_app.h"
#include "uip.h"
#include "main.h"
#include "mt.h"
#include "vnc-server.h"
extern void run_dry (char *msg, int n);

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
extern char g_szDhrystone_msg[128];   /* For the dhrystone result web page */

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
char          LEDbuf[128];
char          HIDbuf[128];
unsigned char LEDflag;
unsigned char g_ucHIDflag;
unsigned char g_ucDryFlag     = 0;
unsigned char g_ucRunHid      = 0;
unsigned char g_ucDemoSel     = 0;
unsigned char g_ucVncLastDemo = 0;

/******************************************************************************
Private global variables and functions
******************************************************************************/


/**
 * Device specific configutation parameters in flash are defined here.               
 * These parameters can be serial number, model name, etc.                           
 *                                                                                   
 * The size of this section is 64 bytes and all members must be                      
 * defined as constants.                                                             
 *                                                                                   
 * Name              Size (bytes)    Info                                            
 * -----            --------------  ------                                           
 * my_mac              6             Must be unique per device                       
 * my_ip               4             Default IP address if DHCP server not available 
 * my_netmask          4             Default net mask                                
 * my_default_router   4             Default router address                          
 */
//#pragma section _FLASH_CONFIG_PARAMS

// Default IP address configuration
// Please make sure this IP address in the same subnet with the DHCP server
const u8_t my_ip[4]             = {192, 168, 1, 10};
const u8_t my_netmask[4]        = {255, 255, 255, 0};
const u8_t my_default_router[4] = {192, 168, 1, 1};

// Reserved 
const u8_t reserved[46] = {0};

//#pragma section

// Who owns/uses LED0? If g_use_by_LED_control is 0,
// then CMT0 uses it to show that timer 0 is ticking.
// If 1, then the LED control page took ownership.

int g_use_by_LED_control = 0;

/******************************************************************************
* Function Name: user_app
* Description  : This is a simple user application that receives a         
*                message from LED Control Web page and turns on or off a   
*                group of LEDs on the Renesas RSK board.  On the Web page  
*                LEDs are named as A, B, and C to allow probability between
*                different boards. This code will require some understanding
*                of HTML and HTTP.                                           
* Arguments    : none
* Return Value : none
******************************************************************************/
int g_iLeds = 0;
void user_app(void)
{
  if (LEDflag)
  {
    u8_t ledval, i;
    char *ptr1, *ptr2;

    LEDflag = 0;                   
    ledval  = 0;
    ptr1    = LEDbuf;                  // pointer to command string

  g_use_by_LED_control = 1;

    for (i = 0; i < 3; i++)
    {
      // Search for key word "LED"
      ptr2 = strstr(ptr1, "LED");   
      if (ptr2 != NULL)
      {
        // Determine the LED settings, on or off
        ledval = strncmp(ptr2+5, "On", 2) ? LED_OFF : LED_ON;
        ptr1 = ptr2+5;
      }

      // NOTE: LED0 is used by timer tick to indicate activity
      switch(i)
      {
      case 0:  
          LED0 = ledval;    // either ON or OFF
        break;
      case 1:  
          LED1 = ledval;    // either ON or OFF
        break;
      case 2:  
          LED2 = ledval;    // either ON or OFF
        break;
      default:
        break;
      }
    }
  
    g_iLeds = 0;
  if (!LED0) g_iLeds |= 1;
  if (!LED1) g_iLeds |= 2;
  if (!LED2) g_iLeds |= 4;
  }
  
  if (g_ucHIDflag)
  {
    u8_t hidval;
    char *ptr1, *ptr2;

    g_ucHIDflag = 0;                   
    hidval  = 0;
    ptr1    = HIDbuf;                  // pointer to command string

    // Search for key word "HIDMOUSE"
    ptr2 = strstr(ptr1, "HIDMOUSE");   
    if (ptr2 != NULL)
    {
      // Determine the LED settings, on or off
      hidval = strncmp(ptr2+9, "On", 2) ? HID_OFF : HID_ON;
      ptr1 = ptr2+9;
    }

  g_ucRunHid = hidval;
  }

  if (g_ucDryFlag)
  {
  g_ucDryFlag = 0;
  
  /* Release claim on LED usage */
  g_use_by_LED_control = 0;
  
  /* We don't want task switching to interfere */
  /* with the dhrystone measurement!           */
  disable_tasktimertick();
  
    // Run Dhrystone
    run_dry(g_szDhrystone_msg, sizeof(g_szDhrystone_msg));

  /* Back to task switching */
  enable_tasktimertick();
  }
  
  if (g_ucDemoSel)
  {
    switch (g_ucDemoSel)
    {
      case DEMO_BALLS:
      SuspendAllDemos();
      g_ucVncLastDemo = DEMO_BALLS;
        vnc_server_init();
    ResumeBalls();
      break;
    case DEMO_GAME:
      SuspendAllDemos();
      g_ucVncLastDemo = DEMO_GAME;
        vnc_server_init();
    ResumePong();
      break;
    default:
      SuspendAllDemos();
      g_ucVncLastDemo = DEMO_MANDELBROT;
        vnc_server_init();
    ResumeMandelbrot();
      break;
    }
  g_ucDemoSel = 0;
  }
}

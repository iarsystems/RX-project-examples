/* Adapted to IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THE THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :DemoInit.c                                            */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :Framebuffer and Icon callback initialization          */        
/*                                                                     */
/*  CPU TYPE    :RX64M/RX71M                                           */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
12.09.2015 REE - Ported from RX63N GAPI Demo
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"

static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

/* The Classic Home Screen ICON table will be built from the _SCR_HM_CLASSIC section collected from all the screen files
     - By doing this, screens can be added (or removed) simply by linking them into the project */
/* To replace this mechanism (table built from _SCR_HM_CLASSIC sections), simply create a local ScreenObj_t array and 
   point the ScreenHomeData.objects value to this table (refer to any other screen file for example) */

#pragma location=".SCR_HM_CLASSIC"
__root const ScreenObj_t HomeClassicBack= { &pBMP_HomeIcon,      T_HomeScheme,     ButtonBack,       278,     207 };

/* MUST INCLUDE NULL TERMINATED RECORD */
/* The _SCR_HMCT section MUST be located immediately after the _SCR_HM_CLASSIC to properly terminate the home list */

#pragma location=".SCR_HMCT"
__root const ScreenObj_t HomeClassicIconNull= { NULL,         NULL,           NULL,                   0,   0 };

#pragma section=".SCR_HM_CLASSIC"
const Screen_t ScreenHomeClassicData=
{
  Constructor, Destructor, (ScreenObj_t *)__section_begin(".SCR_HM_CLASSIC")
};

/* Shared event message */
static EVENT_MSG msg;

/*******************************************************************************
* Function Name: Constructor
* Description  : Opens a screen object.
* Arguments    : pS -
*                    screen object
* Return Value : None
*******************************************************************************/
static void Constructor(Screen_t const *pS)
{
  /* locate and point to common resources */
  if(NULL == pBMP_ButtonS)
  {
    pBMP_ButtonS = FileFind(pResources, "ButtonS");
  }
    
  if(NULL == pBMP_ButtonR)
  {
    pBMP_ButtonR = FileFind(pResources, "ButtonR");
  }
    
  if(NULL == pBMP_Slider)
  {
    pBMP_Slider = FileFind(pResources, "Slider");
  }
    
  if(NULL == pBMP_SliderButton)
  {
    pBMP_SliderButton = FileFind(pResources, "SliderButton");
  }
  
  {
    static const BMP_ColorTable_t local_ct[]={CT_RED, CT_YELLOW};

    /* Clear the background */
    (void)R_GAPI_FillGradient(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT, &local_ct[0], 90);
    (void)R_GAPI_FillGradient(backFrameBuf, DD_FRAME_WIDTH/4, DD_FRAME_HEIGHT/4, DD_FRAME_WIDTH/2, DD_FRAME_HEIGHT/2, &local_ct[0], 0);
  }

  /* Run default behavior */
  BasicConstructor(pS);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

/*******************************************************************************
* Function Name: Destructor
* Description  : Closes a screen object.
* Arguments    : pS -
*                    screen object
* Return Value : None
*******************************************************************************/
static void Destructor(Screen_t const *pS)
{
  /* Run default behavior */
  BasicDestructor(pS);
}
/*******************************************************************************
End of function Destructor
*******************************************************************************/

/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
- By doing this, screens can be added (or removed) simply by linking them into the project */
static void HomeClassic(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static BMP_t const *pBMP_ButtonClassic;

#pragma location=".SCR_HM"
__root const ScreenObj_t HomeIconClassic= { &pBMP_ButtonClassic,   T_SchemeBlue,    HomeClassic,        244,     108 };

/**********************************************************************************
Function Name:   HomeIconClassic
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void HomeClassic(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(NULL == pBMP_ButtonS)
  {
    pBMP_ButtonClassic = FileFind(pResources, "ButtonS");
  }
  
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Classic", state)))
  {
    if (ME_RELEASE == pMsg->event)
    {
      /* send a screen change command to the handler */
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenHomeClassicData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
  }
}
/*******************************************************************************
End of function HomeClassic
*******************************************************************************/


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
/*  FILE        :ScreenAnimate.c                                       */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :Demonstrate animation capabilities                    */
/*                                                                     */
/*  CPU TYPE    :RX64M/RX71M                                           */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
12.09.2015 REE - Ported from RX63N GAPI Demo
************************************************************************/
#include <math.h>
#include "Global.h"
#include "LCD_demo.h"

#define SCHEME T_SchemeBlue
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

static void ScrollStart(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void AnimateStart(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#define SCROLL_ICON 1
#define ANIMATE_ICON 2
static BMP_t const *pBMP_Renesas;
static BMP_t const *pBMP_Animate;

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
  { &pBMP_HomeIcon,T_HomeScheme,   ButtonBack,     278, 207 },
  { &pBMP_Renesas, T_Scheme50_100, ScrollStart,    201, 6   },
  { &pBMP_ButtonS, T_SchemeBlue, AnimateStart,     120, 122 },
  { NULL,         NULL,           NULL,           0,   0 },
};

static const Screen_t ScreenAnimateData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;
static uint8_t AnimateState;
static uint8_t ScrollRunning;

static void Constructor(Screen_t const *pS)
{
    if(NULL == pBMP_Animate)
    {
        pBMP_Animate = FileFind(pResources, "MrSun");
    }

    if(NULL == pBMP_Renesas)
    {
        pBMP_Renesas = FileFind(pResources, "Renesas");
    }

    /* Clear the background */
    (void)R_GAPI_FillGradient(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT,  SCHEME[0].ct , 90);
    (void)R_GAPI_ColorCopy(FileFind(pResources, "TestImageRLE"), backFrameBuf, 0, 0, T_Scheme50_100);

    /* Run default behavior */
    BasicConstructor(pS);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

static void Destructor(Screen_t const *pS)
{
  /* Run default behavior */
  BasicDestructor(pS);
}
/*******************************************************************************
End of function Destructor
*******************************************************************************/

/**********************************************************************************
Function Name:   ScreenAnimate
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
     - By doing this, screens can be added (or removed) simply by linking them into the project */
static void ScreenAnimate(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#pragma location=".SCR_HM_CLASSIC"
__root const ScreenObj_t HomeIconAnimate= { &pBMP_ButtonR, T_SchemeGreen,  ScreenAnimate,  110, 143};

static void ScreenAnimate(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;
    if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Animate", state)))
    {
        if (ME_RELEASE == pMsg->event)
        {
            /* send a screen change command to the handler */
            msg.source = MS_DISPLAY;
            msg.event = ME_SCREEN_PUSH;
            msg.param.pS = &ScreenAnimateData;
            (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
        }
    }
}
/*******************************************************************************
End of function ScreenAnimate
*******************************************************************************/

/**********************************************************************************
Function Name:   ScrollDemo
Description:  Screen task to show scrolling behaviour
Arguments:   None
Return value:   TickType: Amount of time to sleep until next wakeup.
***********************************************************************************/
static portTickType ScrollDemo(void)
{
  static int16_t scroll_split = 0;
  uint16_t scroll_type;
  ScreenObj_t const *pI = &ScreenAnimateData.objects[SCROLL_ICON];
  portTickType xDelay = port_mS_to_TICK(60);  /* RTOS_USAGE */

  /* Scrolling the text after the timeout */
  scroll_type = (uint16_t)(((++scroll_split) < 250) ? 0: 1);

  if (scroll_split > 1000)
  {
        scroll_split = 0;
  }

  /* Let system know we're accessing Display Memory */
  R_DDLCD_DispMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
  {
    R_GAPI_Scroll(*pI->ppBmp, displayFrameBuf, pI->PosX, pI->PosY, scroll_split*2, scroll_type);
  }
  R_DDLCD_DispMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */

  return (xDelay);
}
/*******************************************************************************
End of function ScrollDemo
*******************************************************************************/

/**********************************************************************************
Function Name:   ScrollStart
Description:  Button to start scrolling demo
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void ScrollStart(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;

    if(ME_DRAW == pMsg->event)
    {
        ScrollRunning = 0;
    }

    if(0 != (state =ButtonHandler(pS, pMsg, (uint8_t *)"Scroll", state)))
    {
        if (ME_MOVE == pMsg->event)
        {
            if(0 == ScrollRunning)
            {
                /* start timer Scrolling Demo */
                (void)ScreenTaskStart(ScrollDemo);
                ScrollRunning=1;
            }
            else
            {
                /* start timer Scrolling Demo */
                (void)ScreenTaskStop(ScrollDemo);
                ScrollRunning=0;
            }
        }
    }
}
/*******************************************************************************
End of function ScrollStart
*******************************************************************************/

static const GAPI_Scheme_t AnimateScheme[]=
{
  /* Text Only */
  {
    {CT_NONE,CT_NONE},
    { &pfontSqBT_B42, {CT_NONE,CT_NONE,}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_LEFT,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_MULT},
  },
};

/* interpolate along path between positions index=2.14 format */
static void PathCalc(uint16_t path_index, int16_t *pX, int16_t *pY)
{
  *pX = (int16_t)lroundf(sin(2.*3.14159*(float)path_index/16384) * 89);
  *pY = (int16_t)lroundf(cos(2.*3.14159*(float)path_index/16384) * 89);

  /* Note: In the above calculations the final multiplier (*90) controls
   * the orbit. The lower the number the tighter the orbit.   */
}
/*******************************************************************************
End of function PathCalc
*******************************************************************************/

static const char* const  AnimateLabel[]                ={"Go", "0", "1", "2", "3", "4", "5", "6", "7", "8"};

static portTickType AnimateDemo(void)
{
  static uint16_t path_index = 0x1000;
  portTickType xDelay = port_mS_to_TICK(50);  /* RTOS_USAGE */
  ScreenObj_t const *pI = &ScreenAnimateData.objects[ANIMATE_ICON];
  int16_t PosXold;
  int16_t PosX;
  int16_t PosYold;
  int16_t PosY;

  /* old positions */
  PathCalc(path_index, &PosXold, &PosYold);
  PosXold += (pI->PosX -10);
  PosYold += (pI->PosY -10);

  /* update path */
  path_index +=  (4 << AnimateState);
  path_index %= 0x4000;

  /* new positions */
  PathCalc(path_index, &PosX, &PosY);
  PosX += (pI->PosX - 10);
  PosY += (pI->PosY - 10);

  /* Let system know we're accessing Display Memory */
  R_DDLCD_DispMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
  {
    /* move the image */
    BMPMove(pBMP_Animate, PosXold, PosYold, PosX, PosY, AnimateScheme);
  }
  R_DDLCD_DispMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */

  return (xDelay);
}
/*******************************************************************************
End of function AnimateDemo
*******************************************************************************/

/**********************************************************************************
Function Name:   AnimateStart
Description:  Button to start scrolling demo
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void AnimateStart(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;

  if(ME_DRAW == pMsg->event)
  {
    AnimateState = 0;
  }

  if(0 != (state =ButtonHandler(pS, pMsg, (uint8_t *)AnimateLabel[AnimateState], state)))
  {
    if (ME_PRESS == pMsg->event)
    {
        AnimateState++;
        if(AnimateState >= size_ra(AnimateLabel))
        {
            AnimateState = 0;
        }

        if(0 == AnimateState)
        {
            /* stop the task */
            (void)ScreenTaskStop(AnimateDemo);
        }
        if(1 == AnimateState)
        {
            /* start timer Scrolling Demo */
            (void)ScreenTaskStart(AnimateDemo);
        }
    }
  }
}
/*******************************************************************************
End of function AnimateStart
*******************************************************************************/





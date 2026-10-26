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
/*  FILE        :ScreenWrites.c                                        */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :Screen Handler for showing BMP write speed            */        
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

#define SCHEME T_SchemeBlue
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

static void Button1bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void Button4bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void Button8bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void ButtonRLE(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void Button16bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void Button32bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
  { &pBMP_HomeIcon,T_HomeScheme,   ButtonBack,     278, 207 },
  { &pBMP_ButtonS, T_SchemeBlue,   Button1bpp,      3,  16 },
  { &pBMP_ButtonS, T_SchemeBlue,   Button4bpp,      56,  16 },
  { &pBMP_ButtonS, T_SchemeBlue,   Button8bpp,     109,  16 },
  { &pBMP_ButtonS, T_SchemeBlue,   ButtonRLE,      162,  16 },
  { &pBMP_ButtonS, T_SchemeBlue,   Button16bpp,    215,  16 },
  { &pBMP_ButtonS, T_SchemeBlue,   Button32bpp,    268,  16 },
  {  NULL,         NULL,           NULL,           0,   0 },
};

static const Screen_t ScreenWritesData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;
static char TextBuffer[32];

/**********************************************************************************
Function Name: Constructor
Description  : Constructor
Arguments:   ps
Return value:   None
***********************************************************************************/
static void Constructor(Screen_t const *pS)
{
  /* Clear the background */
  (void)R_GAPI_FillGradient(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT,  SCHEME[1].ct , 90);

  /* Run default behavior */
  BasicConstructor(pS);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

/**********************************************************************************
Function Name:   ScreenWrites
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void Destructor(Screen_t const *pS)
{
  /* Run default behavior */
  BasicDestructor(pS);
}
/*******************************************************************************
End of function Destructor
*******************************************************************************/

/**********************************************************************************
Function Name:   ScreenWrites
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
     - By doing this, screens can be added (or removed) simply by linking them into the project */
static void ScreenWrites(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#pragma location=".SCR_HM_CLASSIC"
__root const ScreenObj_t HomeIconWrites= { &pBMP_ButtonR, T_SchemeBlue,   ScreenWrites,  110, 85 };

static void ScreenWrites(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Writes", state)))
  {
    if (ME_RELEASE == pMsg->event)
    {
      /* send a screen change command to the handler */
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenWritesData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
  }
}
/*******************************************************************************
End of function ScreenWrites
*******************************************************************************/

/**********************************************************************************
Function Name:   WriteTest
Description:  Button to test 1bpp performance
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void WriteTest(BMP_t const *const pS, uint32_t transparency)
{
  if(NULL != pS)
  {
    int16_t Width=R_BMP_Width(pS);
    int16_t Height=R_BMP_Height(pS);
    int16_t PosX;
    int16_t PosY;
    int16_t dTime;
    uint16_t WriteStart = (uint16_t)xTaskGetTickCount();  /* RTOS_USAGE */

    for (PosY=0; PosY < DD_FRAME_HEIGHT; PosY+=Height)
    {
        for (PosX=0; PosX < DD_FRAME_WIDTH; PosX+=Width)
        {
          (void)R_GAPI_CopySub(pS,backFrameBuf,PosX,PosY,0,0,Width,Height,R_BMP_Ct(pS),transparency);
        }
    }

    dTime = (uint16_t)xTaskGetTickCount()-WriteStart; /* RTOS_USAGE */
    dTime = (uint16_t)port_TICK_to_mS(dTime);          /* RTOS_USAGE */
  
    BasicConstructor(&ScreenWritesData);

    (void)sprintf(TextBuffer, "%uKb,%umS to Back,", (uint16_t)(R_BMP_FileSize(pS)/1024), dTime);
    PosX=R_GAPI_PutS((uint8_t const *)TextBuffer, &T1, displayFrameBuf, SX(0.000), SY(0.000)); 
#if 0
    dTime = (uint16_t)xTaskGetTickCount()-WriteStart; /* RTOS_USAGE */
    dTime = (uint16_t)port_TICK_to_mS(dTime);          /* RTOS_USAGE */
    (void)sprintf(TextBuffer, "%umS Overall", dTime);
    PosX=R_GAPI_PutS((uint8_t const *)TextBuffer, &T1, displayFrameBuf, PosX, SY(0.000)); 
#endif  
    (void)sprintf(TextBuffer, "%lu Px/Sec",(((uint32_t)DD_FRAME_HEIGHT * (uint32_t)DD_FRAME_WIDTH) * 1000L) / (uint32_t)dTime);
    PosX=R_GAPI_PutS((uint8_t const *)TextBuffer, &T1, displayFrameBuf, PosX, SY(0.000)); 
  }
}
/*******************************************************************************
End of function WriteTest
*******************************************************************************/

/**********************************************************************************
Function Name:   Button1bpp
Description:  Button to test 1bpp performance
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void Button1bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;

    if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"1bpp", state)))
    {
        if (ME_RELEASE == pMsg->event)
        {
            WriteTest(FileFind(pResources, "TestImage1bpp"),
                                        (((uint32_t) 1 << 16) + 0));
        }
    }
}
/*******************************************************************************
End of function Button1bpp
*******************************************************************************/

/**********************************************************************************
Function Name:   Button4bpp
Description:  Button to test 4bpp performance
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void Button4bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;
    if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"4bpp", state)))
    {
        if (ME_RELEASE == pMsg->event)
        {
            WriteTest(FileFind(pResources, "TestImage4bpp"),
                                        (((uint32_t) 1 << 16) + 0));
        }
    }
}
/*******************************************************************************
End of function Button4bpp
*******************************************************************************/

/**********************************************************************************
Function Name:   Button8bpp
Description:  Button to test 8bpp performance
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void Button8bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;

    if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"8bpp", state)))
    {
        if (ME_RELEASE == pMsg->event)
        {
            WriteTest(FileFind(pResources, "TestImage8bpp"),
                                        (((uint32_t) 1 << 16) + 0));
        }
    }
}
/*******************************************************************************
End of function Button8bpp
*******************************************************************************/

/**********************************************************************************
Function Name:   ButtonRLE
Description:  Button to test RLE performance
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void ButtonRLE(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;

    if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"RLE", state)))
    {
        if (ME_RELEASE == pMsg->event)
        {
            WriteTest(FileFind(pResources, "TestImageRLE"),
                                        (((uint32_t) 1 << 16) + 0));
        }
    }
}
/*******************************************************************************
End of function ButtonRLE
*******************************************************************************/

/**********************************************************************************
Function Name:   Button16bpp
Description:  Button to test 16bpp performance
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void Button16bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;

    if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"16bpp", state)))
    {
        if (ME_RELEASE == pMsg->event)
        {
            WriteTest(FileFind(pResources, "TestImage16bpp"),
                                        (((uint32_t) 1 << 16) + 0));
        }
    }
}
/*******************************************************************************
End of function Button16bpp
*******************************************************************************/

/**********************************************************************************
Function Name:   Button32bpp
Description:  Button to test 24bpp performance
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void Button32bpp(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;

    if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"32bpp", state)))
    {
        if (ME_RELEASE == pMsg->event)
        {
            WriteTest(FileFind(pResources, "MrSun"),
                                        (((uint32_t) 6 << 16) + 0));
        }
    }
}
/*******************************************************************************
End of function Button32bpp
*******************************************************************************/


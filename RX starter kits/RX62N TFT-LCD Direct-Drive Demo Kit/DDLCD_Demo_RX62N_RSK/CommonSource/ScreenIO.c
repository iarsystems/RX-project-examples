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
/*  FILE        :ScreenIO.c                                            */
/*  DATE        :Wen, Mar 12, 2008                                     */
/*  DESCRIPTION :Screen Handler for adjusting frame rate               */        
/*                                                                     */
/*  CPU TYPE    :H8S/H8SX                                              */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
07.16.2008 RTA-JMB First Release
*
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"

extern const GAPI_Scheme_t T_SchemeSliderGreen[];
#define SCHEME T_SchemeGreen
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);
static void SliderFrameRate(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
  { &pBMP_HomeIcon,T_HomeScheme,         ButtonBack,     SX(0.872), SY(0.870) },
  { &pBMP_Slider,  T_SchemeSliderGreen,  SliderFrameRate,  SX(0.100), SY(0.200) },
  { NULL,         NULL,                  NULL,             0,   0 },
};

static const Screen_t ScreenIOData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;
static char TextBuffer[32];
static SLIDERSTATE_type Slider;

static void Constructor(Screen_t const *pS)
{
  // Clear the background
  (void)R_GAPI_FillGradient(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT,  SCHEME[1].ct , 90);

  /* grab frame rate outside basic constructor */
  Slider.Value = R_DDLCD_GetFrameRate() * 256;

  // Run default behavior
  BasicConstructor(pS);
}

static void Destructor(Screen_t const *pS)
{
  // Run default behavior
  BasicDestructor(pS);
}

/**********************************************************************************
Function Name:   ScreenIO
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
     - By doing this, screens can be added (or removed) simply by linking them into the project */
static void ScreenIO(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
#pragma location=".C_SCR_HM_CLASSIC"
__root const ScreenObj_t HomeIconIO= { &pBMP_ButtonR, T_SchemeGreen,  ScreenIO,             SX(0.688), SY(0.500) };

static void ScreenIO(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"I/O", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // send a screen change command to the handler
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenIOData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
}

/**********************************************************************************
Function Name:   SliderFrameRate
Description:  Slider to control frame rate
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void SliderFrameRate(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  Slider.Touched = SliderHandler(pS, pMsg, pBMP_SliderButton, &Slider );
  if ((0 != Slider.Touched ) || (ME_DRAW_DISPLAY == pMsg->event))
  {
    /* Attempt to change frame rate */
    int16_t new_rate = Slider.Value/256;
    int16_t vblank_pct = R_DDLCD_SetFrameRate(new_rate);
    if (vblank_pct > 0)
    {
      /* Display Values */
      (void)sprintf(TextBuffer, "Frame Rate=%3d, Vblank=%3d%%", new_rate, vblank_pct);
      (void)R_GAPI_PutS((uint8_t *)TextBuffer, &T1, displayFrameBuf, pS->PosX, pS->PosY-R_GAPI_FontHeight(&T1)); 
    }
  }
}


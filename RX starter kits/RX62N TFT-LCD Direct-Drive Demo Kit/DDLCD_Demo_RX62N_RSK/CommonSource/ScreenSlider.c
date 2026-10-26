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
/*  FILE        :ScreenSlider.c                                        */
/*  DATE        :Wen, Mar 12, 2008                                     */
/*  DESCRIPTION :Screen Handler for showing slider operation           */        
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

#define CT_BLUE_xC0 {0xC0, 0x00, 0x00, 0x00}
#define CT_GREEN_xC0 {0x00, 0xC0, 0x00, 0x00}
#define CT_RED_xC0 {0x00, 0x00, 0xC0, 0x00}

const GAPI_Scheme_t T_SchemeSliderBlue[]= 
{
  /* Slider */
  {
    {CT_BLACK,CT_BLUE_xC0}, 
    {&pfontTerminal, {CT_NONE,CT_GRAY_50}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  },
  /* Button */
  {
    {CT_BLACK,CT_BLUE}, 
    {&pfontTerminal, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  }
};

const GAPI_Scheme_t T_SchemeSliderGreen[]= 
{
  /* Slider */
  {
    {CT_BLACK,CT_GREEN_xC0}, 
    {&pfontTerminal, {CT_NONE,CT_GRAY_50}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  },
  /* Button */
  {
    {CT_BLACK,CT_GREEN}, 
    {&pfontTerminal, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  }
};

const GAPI_Scheme_t T_SchemeSliderRed[]= 
{
  /* Slider */
  {
    {CT_BLACK,CT_RED_xC0}, 
    {&pfontTerminal, {CT_NONE,CT_GRAY_50}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  },
  /* Button */
  {
    {CT_BLACK,CT_RED}, 
    {&pfontTerminal, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  }
};

#define SCHEME T_SchemeBlue
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

static void SliderRed(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void SliderGreen(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void SliderBlue(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

enum{SLIDER_RED, SLIDER_GREEN, SLIDER_BLUE};
static SLIDERSTATE_type State[3];

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
  { &pBMP_Slider,  T_SchemeSliderRed,    SliderRed,      SX(0.050), SY(0.550) },
  { &pBMP_Slider,  T_SchemeSliderGreen,  SliderGreen,    SX(0.050), SY(0.350) },
  { &pBMP_Slider,  T_SchemeSliderBlue,   SliderBlue,     SX(0.050), SY(0.150) },
  { &pBMP_HomeIcon,T_HomeScheme,   ButtonBack,     SX(0.872), SY(0.870) },
  {  NULL,         NULL,           NULL,           0,   0 },
};

static const Screen_t ScreenSliderData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;

static void Constructor(Screen_t const *pS)
{
  BMP_t *pBmp = (BMP_t *)FileFind(pResources, "TestImage16bpp");
  static const BMP_ColorTable_t bg_color=CT_GRAY_50;

  // Clear the background
  (void)R_GAPI_Fill(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT,  &bg_color);
  (void)R_GAPI_CopySub(pBmp, backFrameBuf, SX(0.2) ,SY(0.7), 0,0 ,SX(0.2), SY(0.25),  R_BMP_Ct(pBmp) , TRANSPARENCY(GAPI_T_NONE,0));

  // Run default behavior
  BasicConstructor(pS);
}

static void Destructor(Screen_t const *pS)
{
  // Run default behavior
  BasicDestructor(pS);
}

/**********************************************************************************
Function Name:   ScreenSlider
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
     - By doing this, screens can be added (or removed) simply by linking them into the project */
static void ScreenSlider(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
#pragma location=".C_SCR_HM_CLASSIC"
__root const ScreenObj_t HomeIconSlider= { &pBMP_ButtonR, T_SchemeRed,    ScreenSlider,         SX(0.063), SY(0.500)};

static void ScreenSlider(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Slider",state)))
    if (ME_RELEASE == pMsg->event)
    {
      // send a screen change command to the handler
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenSliderData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
}

/**********************************************************************************
Function Name:   Sliders
Description:  Button to control Red color channel
Arguments:   Standard Button Inputs
              Instance (to distinguish among sliders)
Return value:   None
***********************************************************************************/
static void Sliders(ScreenObj_t const *pS, EVENT_MSG const *pMsg, int16_t instance)
{
  char TextBuffer[8];
  static int16_t Width[3];
  State[instance].Touched = SliderHandler(pS, pMsg, pBMP_SliderButton, &State[instance] );

  if ((0 != State[instance].Touched) || (ME_DRAW_DISPLAY == pMsg->event))
  {
    /* Display Values */
    int16_t PosX = pS->PosX+R_BMP_Width(*pS->ppBmp)+1;
    /* restore previous area */
    (void)BMPCopyBackDisplay(PosX,pS->PosY, Width[instance], R_GAPI_FontHeight(&T1));

    (void)sprintf(TextBuffer, "%3d", State[instance].Value/128);
    Width[instance] = R_GAPI_FontWidthString((uint8_t *)TextBuffer, &T1);
    (void)R_GAPI_PutS((uint8_t *)TextBuffer, &T1, displayFrameBuf, PosX, pS->PosY); 

    {
      /* Fill in color table */
      BMP_ColorTable_t ct;
      ct.channel[BMP_CT_RED] = (uint8_t)((uint16_t)State[0].Value/128);
      ct.channel[BMP_CT_GREEN] = (uint8_t)((uint16_t)State[1].Value/128);
      ct.channel[BMP_CT_BLUE] = (uint8_t)((uint16_t)State[2].Value/128);

      R_GAPI_Fill(displayFrameBuf,SX(0.4),SY(0.7),SX(0.2),SY(0.25),&ct);
      R_GAPI_CopySub(backFrameBuf, displayFrameBuf, SX(0.22) ,SY(0.72), SX(0.22) ,SY(0.72),SX(0.16), SY(0.21), R_BMP_Ct(displayFrameBuf), TRANSPARENCY(GAPI_T_MULK,R_GAPI_24_Disp(&ct)));
    }
  }
}

/**********************************************************************************
Function Name:   SliderRed
Description:  Button to control Red color channel
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void SliderRed(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  Sliders(pS, pMsg, SLIDER_RED );
}

/**********************************************************************************
Function Name:   SliderGreen
Description:  Button to control Red color channel
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void SliderGreen(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  Sliders(pS, pMsg, SLIDER_GREEN );
}

/**********************************************************************************
Function Name:   SliderBlue
Description:  Button to control Red color channel
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void SliderBlue(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  Sliders(pS, pMsg, SLIDER_BLUE );
}



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
/*  FILE        :ScreenUTF.c                                           */
/*  DATE        :Aug 08, 2016                                          */
/*  DESCRIPTION :Screen Handler for displaying various languages       */
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
#include "UTF_strings.h"

static GAPI_Font_t const *pfontUTF;
const GAPI_Scheme_t T_SchemeUTF[]=
{
  /* Button off */
  {
    {CT_BLACK,CT_GREEN_xA0},
    {&pfontUTF, {CT_NONE,CT_GRAY_50}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_LINEAR_CHANGE,CT_TEXT_CENTER,GAPI_T_MAGIC}
  },

  /* Button on */
  {
    {CT_BLACK,CT_GREEN},
    {&pfontUTF, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_LINEAR_CHANGE,CT_TEXT_CENTER,GAPI_T_MAGIC}
  }
};

#define SCHEME T_SchemeGreen
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);
static void LanguageSelect(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
  { &pBMP_HomeIcon,T_HomeScheme,         ButtonBack,     278, 207 },
  { &pBMP_ButtonR, T_SchemeUTF,          LanguageSelect, 112,  96 },
  { NULL,         NULL,                  NULL,             0,   0 },
};

static const Screen_t ScreenUTFData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;

/*******************************************************************************
* Function Name: Constructor
* Description  : Constructor.
* Arguments    : Ps
* Return Value : None
*******************************************************************************/
static void Constructor(Screen_t const *pS)
{
  /* Clear the background */
  (void)R_GAPI_FillGradient(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT,  SCHEME[1].ct , 90);

  /* locate and point to resources */
  if(NULL == pfontUTF)
  {
    pfontUTF = FileFind(pResources, "unifont_Medium_16");
  }

  /* Run default behavior */
  BasicConstructor(pS);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

/*******************************************************************************
* Function Name: Destructor
* Description  : Destructor.
* Arguments    : Ps
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

/**********************************************************************************
Function Name:   ScreenUTF8
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
     - By doing this, screens can be added (or removed) simply by linking them into the project */
static void ScreenUTF8(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#pragma location = ".SCR_HM_CLASSIC"
__root const ScreenObj_t HomeIconUTF= { &pBMP_ButtonR, T_SchemeGreen,  ScreenUTF8,  110, 26 };

static void ScreenUTF8(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"UTF-8", state)))
  {
    if (ME_RELEASE == pMsg->event)
    {
      /* send a screen change command to the handler */
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenUTFData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
  }
}
/*******************************************************************************
End of function ScreenUTF8
*******************************************************************************/

/**********************************************************************************
Function Name:   LanguageSelect
Description:  Button to control text language
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
const GAPI_PUT_t tUTF={&pfontUTF, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_LEFT,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}};
#define UTF_TEXT_X 0
#define UTF_TEXT_Y 0

static void LanguageSelect(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)UTF_Languages[UTF_Language], state)))
  {
    if (ME_PRESS == pMsg->event)
    {
      /* change the language selection */
      UTF_Language++;
      if (UTF_Language >= UTF_LANGAUGES)
      {
          UTF_Language=UTF_ENGLISH;
      }

      /* Update the button text */
      ButtonHandler(pS, pMsg, (uint8_t *)UTF_Languages[UTF_Language], state);

      /* cleanly put text on screen by rendering in work buffer after copying background */
      static int16_t width;
      int16_t height = R_GAPI_FontHeight(&tUTF);
      int16_t PosX_end;
      width = DD_FRAME_WIDTH;
      (void)BMPCopyBackWork( UTF_TEXT_X, UTF_TEXT_Y, width, height );
      PosX_end = R_GAPI_PutS((uint8_t const *)UTF_Hello_World[UTF_Language], &tUTF, workFrameBuf, UTF_TEXT_X, UTF_TEXT_Y);
      (void)BMPCopyWorkDisplay( UTF_TEXT_X, UTF_TEXT_Y, width, height );
      width = (int16_t)(PosX_end - UTF_TEXT_X);
    }
  }
}
/*******************************************************************************
End of function LanguageSelect
*******************************************************************************/


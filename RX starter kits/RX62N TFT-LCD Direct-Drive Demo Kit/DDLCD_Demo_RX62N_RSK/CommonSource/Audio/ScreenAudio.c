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
/*  DATE        :Wen, Mar 12, 2008                                     */
/*  DESCRIPTION :Framebuffer and Icon callback initialization          */        
/*                                                                     */
/*  CPU TYPE    :H8S/2378R                                             */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
*
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"

#define SCREEN_type Screen_t
#define ICON_type ScreenObj_t

static void Constructor(SCREEN_type const *pS);
static void Destructor(SCREEN_type const *pS);

void initAudio(void);
void playAudio(char *name);


static void PlayBroil(ICON_type const *pS, EVENT_MSG const *pMsg);
static void PlayBake(ICON_type const *pS, EVENT_MSG const *pMsg);
static void PlayWarm(ICON_type const *pS, EVENT_MSG const *pMsg);
static void PlayRaise(ICON_type const *pS, EVENT_MSG const *pMsg);
static void PlayLower(ICON_type const *pS, EVENT_MSG const *pMsg);
static void PlayStart(ICON_type const *pS, EVENT_MSG const *pMsg);
static void PlayStop(ICON_type const *pS, EVENT_MSG const *pMsg);
static void ScreenAudio(ICON_type const *pS, EVENT_MSG const *pMsg);

/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
     - By doing this, screens can be added (or removed) simply by linking them into the project */
#pragma location=".C_SCR_HM"
__root const ICON_type HomeIconAudio= { &pBMP_ButtonR, T_SchemeRed,    ScreenAudio,          SX(0.375), SY(0.250) };

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ICON_type Icons[]=
{
  { &pBMP_ButtonS, T_SchemeBlue,   ButtonBack,     SX(0.850), SY(0.750) },
  { &pBMP_ButtonR, T_SchemeGreen,  PlayBroil,      SX(0.063), SY(0.500) },
  { &pBMP_ButtonR, T_SchemeGreen,  PlayBake,       SX(0.375), SY(0.500) },
  { &pBMP_ButtonR, T_SchemeGreen,  PlayWarm,       SX(0.688), SY(0.500) },
  { &pBMP_ButtonR, T_SchemeRed,    PlayRaise,      SX(0.063), SY(0.250) },
  { &pBMP_ButtonR, T_SchemeRed,    PlayLower,      SX(0.375), SY(0.250) },
  { &pBMP_ButtonR, T_SchemeBlue,   PlayStart,      SX(0.063), SY(0.000) },
  { &pBMP_ButtonR, T_SchemeBlue,   PlayStop,       SX(0.375), SY(0.000) },
  { NULL,         NULL,           NULL,                   0,   0 },
};

SCREEN_type ScreenAudioData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;

static void Constructor(SCREEN_type const *pS)
{
  const BMP_ColorTable_t local_ct[]={{0x00, 0x00, 0xFF, 0x00},{0x00, 0xFF, 0xFF, 0x00}};

  // Clear the background
  (void)R_GAPI_FillGradient(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT, &local_ct[0], 90);

  playAudio("Intro");

  // Run default behavior
  BasicConstructor(pS);
}

static void Destructor(SCREEN_type const *pS)
{
  // Run default behavior
  BasicDestructor(pS);
}

/*****************************************************************************/
//
// FUNCTION    :ScreenAudio()
// DESCRIPTION :Activate Audio Screen (called by higher level screen)
//
/*****************************************************************************/
static void ScreenAudio(ICON_type const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Audio", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // send a screen change command to the handler
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenAudioData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
}

/*****************************************************************************/
//
// FUNCTION    :ButtonAudio()
// DESCRIPTION :play requested audio
//
/*****************************************************************************/
static void PlayBroil(ICON_type const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Broil", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // play audio
      playAudio("Broil");
    }
}

static void  PlayBake(ICON_type const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Bake", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // play audio
      playAudio("Bake");
    }
}

static void  PlayWarm(ICON_type const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Warm", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // play audio
      playAudio("Warm");
    }
}

static void PlayRaise(ICON_type const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Raise", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // play audio
      playAudio("Raise");
    }
}

static void PlayLower(ICON_type const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Lower", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // play audio
      playAudio("Lower");
    }
}

static void PlayStart(ICON_type const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Start", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // play audio
      playAudio("Start");
    }
}

static void  PlayStop(ICON_type const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Stop", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // play audio
      playAudio("Stop");
    }
}

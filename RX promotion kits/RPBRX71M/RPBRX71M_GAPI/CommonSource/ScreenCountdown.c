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
/*  FILE        :ScreenCountdown.c                                     */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :Screen Handler for Coutdown Timer                     */        
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

#define SCHEME T_SchemeGreen
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

static void CountDownStart(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void CountDownEntry(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void CountDownClear(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void CountDownTimerDraw(int16_t value);

/* Create Font information for countdown timer (used fixed spacing) */
static GAPI_Font_t const *pfontTimer;
static const GAPI_PUT_t FontCountdown = { &pfontTimer, {{0x00,0x00,0x00,0x00},{0x00,0xFF,0xFF,0x00},}, -1, 0, {0,0,0,0,1,0}};

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
  { &pBMP_HomeIcon,T_HomeScheme,   ButtonBack,       278,    207},
  { &pBMP_ButtonS, T_SchemeGreen,  CountDownStart,   252,     6 },
  { &pBMP_ButtonS, T_SchemeBlue,   CountDownEntry,   156,     6 },
  { &pBMP_ButtonS, T_SchemeRed,    CountDownClear,   156,     6 },
  { NULL,          NULL,           NULL,               0,     0 },
};

static const Screen_t ScreenCountdownData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;
static int16_t TimerValue = 0;

/*******************************************************************************
* Function Name: Constructor
* Description  : Constructor.
* Arguments    : pS
* Return Value : None
*******************************************************************************/
static void Constructor(Screen_t const *pS)
{
  if(NULL == pfontTimer)
  {
      pfontTimer = FileFind(pResources, "FontTimerSmall");
      //pfontTimer = FileFind(pResources, "Square721 BT_Roman_B60");
  }
    
  /* Clear the background */
  (void)R_GAPI_FillGradient(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT,  SCHEME[1].ct , 90);

  /* Run default behavior */
  BasicConstructor(pS);

  CountDownTimerDraw(TimerValue);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

/*******************************************************************************
* Function Name: Destructor
* Description  : Closes a demo.
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
Function Name:   ScreenCountdown
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM_CLASSIC section will be used as the Icon list for the home screen
     - By doing this, screens can be added (or removed) simply by linking them into the project */
static void ScreenCountdown(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#pragma location=".SCR_HM_CLASSIC"
__root const ScreenObj_t HomeIconCountdown= { &pBMP_ButtonR, T_SchemeBlue,   ScreenCountdown, 5, 142};

static void ScreenCountdown(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;

  if(0 != (state =ButtonHandler(pS, pMsg, (uint8_t *)"Countdown", state)))
  {
    if (ME_RELEASE == pMsg->event)
    {
      /* send a screen change command to the handler */
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenCountdownData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
  }
}
/*******************************************************************************
End of function ScreenCountdown
*******************************************************************************/

/**********************************************************************************
Function Name:   CountDownTimerDraw
Description:  Helper function to place timer on the screen
Arguments:   Value: Display Value
Return value:   None
***********************************************************************************/
static void CountDownTimerDraw(int16_t value)
{
  char text_buffer[8];
  int16_t Width;
  int16_t PosX;
  int16_t PosY;
  int16_t Height = R_GAPI_FontHeight(&FontCountdown);
  
  /* creat value text string */
  sprintf(text_buffer, "%3.0d", value);
  
  /* clear previous count value from work buffer (font is spec'd as fixed width, so width should be same always) */
  Width=R_GAPI_FontWidthString((uint8_t *)text_buffer, &FontCountdown);
  PosX = SX(0.025);
  PosY = (DD_FRAME_HEIGHT-Height)/2;
  
  (void)BMPCopyBackWork(PosX, PosY, Width, Height);

  /* paint new text in work buffer */
  (void)R_GAPI_PutS((uint8_t *)text_buffer, &FontCountdown, workFrameBuf, PosX, PosY);

  /* paint new image into display buffer */
  (void)BMPCopyWorkDisplay(PosX, PosY, Width, Height);
}
/*******************************************************************************
End of function CountDownTimerDraw
*******************************************************************************/

/**********************************************************************************
Function Name:   CountDownTimer
Description:  Screen task to show countdown timer...once launched decrements counter once/second
Arguments:   None
Return value:   TickType: Amount of time to sleep until next wakeup.
***********************************************************************************/
static portTickType CountDownTimer(void)
{
    portTickType xDelay = port_mS_to_TICK(1000);  /* RTOS_USAGE */

    /* Let system know we're accessing Display Memory */
    R_DDLCD_DispMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
    {
        CountDownTimerDraw(TimerValue);
    }
    R_DDLCD_DispMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */

    if(TimerValue > 0)
    {
        TimerValue--;
        return (xDelay);
    }
    else
    {
        return (0);
    }
}
/*******************************************************************************
End of function CountDownTimer
*******************************************************************************/

/**********************************************************************************
Function Name:   CountDownStart
Description:  Button to start countdown timer
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void CountDownStart(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t  state=0;
    static int16_t do_once = 0;
    static uint16_t state_change = 0;

    if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Ready",state)))
    {
        if ((ME_RELEASE == pMsg->event) && (0 == state_change))
        {
            if (0 == do_once)
            {
                do_once = 1;
                ButtonHandler(pS, pMsg, (uint8_t *)"Start",state);
            }
            else
            {
                ButtonHandler(pS, pMsg, (uint8_t *)"Pause",state);
            }

            /* start timer count-down */
            (void)ScreenTaskStart(CountDownTimer);

            state_change = 1;
        }
        else if ((ME_RELEASE == pMsg->event) && (1 == state_change))
        {
            ButtonHandler(pS, pMsg, (uint8_t *)"Start",state);

            /* Pause timer count-down */
            (void)ScreenTaskStop(CountDownTimer);

            state_change = 0;
        }
        else
        {
            /* Do nothing */
        }
    }
}
/*******************************************************************************
End of function CountDownStart
*******************************************************************************/

/**********************************************************************************
Function Name:   CountDownClear
Description:  Button to start countdown timer
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void CountDownClear(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;

    if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Clear",state)))
    {
        if (ME_RELEASE == pMsg->event)
        {
            /* stop timer count-down and clear */
            (void)ScreenTaskStop(CountDownTimer);
            TimerValue = 0;
            CountDownTimerDraw(TimerValue);
        }
    }
}
/*******************************************************************************
End of function CountDownClear
*******************************************************************************/

/**********************************************************************************
Function Name:   CountDownEntry
Description:  Buttons to set countdown timer
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void CountDownEntry(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    uint8_t index;
    int16_t Width = R_BMP_Width(*pS->ppBmp);
    int16_t Height = R_BMP_Height(*pS->ppBmp);
    ScreenObj_t local_it = *pS;
    static int16_t state[10];

    for (index = 0; index < 10; index++)
    {
        static const uint8_t table[10][2]={{1,0},{0,1},{1,1},{2,1},{0,2},{1,2},{2,2},{0,3},{1,3},{2,3}};
        uint8_t Text[]="0";

        Text[0] = (uint8_t)(index+'0');
        local_it.PosX = pS->PosX + ((int16_t)table[index][0] * Width);
        local_it.PosY = pS->PosY + ((int16_t)table[index][1] * Height);

        if(0 != (state[index]=ButtonHandler(&local_it, pMsg, Text, state[index])))
        {
            if (ME_PRESS == pMsg->event)
            {
            TimerValue = ((TimerValue * 10) + (int16_t)index) % 1000;
            CountDownTimerDraw(TimerValue);
        }
    }
  }
}
/*******************************************************************************
End of function CountDownEntry
*******************************************************************************/



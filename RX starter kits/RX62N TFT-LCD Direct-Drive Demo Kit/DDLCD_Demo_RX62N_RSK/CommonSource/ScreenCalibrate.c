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
/*  FILE        :ScreenCalibrate.c                                     */
/*  DATE        :Wen, Mar 12, 2008                                     */
/*  DESCRIPTION :Screen Handler for touchscreen calibration            */        
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
#include "touchscreen.h"

void ScreenSD(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#define SCHEME T_SchemeRed
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

static void ButtonTouch(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static portTickType CalibrateTouch(void);

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
  { &pBMP_HomeIcon,T_HomeScheme,   ButtonBack,     SX(0.872), SY(0.870) },
  { &pBMP_ButtonS, T_SchemeBlue,   ButtonTouch,    SX(0.000), SY(0.000) },
  {  NULL,         NULL,           NULL,           0,   0 },
};

const Screen_t ScreenCalibrateData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;
static char TextBuffer[32];
static _Bool calibrated;

static void Constructor(Screen_t const *pS)
{
  // Clear the background
  (void)R_GAPI_FillGradient(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT,  SCHEME[1].ct , 90);

  // Run default behavior
  BasicConstructor(pS);

  // Calibrate Screen (if not already)
  if (0 == calibrated)
    (void)ScreenTaskStart(CalibrateTouch);
}

static void Destructor(Screen_t const *pS)
{
  // Run default behavior
  BasicDestructor(pS);
}

/**********************************************************************************
Function Name:   ScreenCalibrate
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
     - By doing this, screens can be added (or removed) simply by linking them into the project */
static void ScreenCalibrate(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
#pragma location=".C_SCR_HM_CLASSIC"
__root const ScreenObj_t HomeIconCalibrate= { &pBMP_ButtonR, T_SchemeRed,    ScreenCalibrate,           SX(0.688), SY(0.000) };

static void ScreenCalibrate(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Calibrate", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // send a screen change command to the handler
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenCalibrateData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
}

/**********************************************************************************
Function Name:   CalibrateTouch
Description:  Calibrate the touch screen
Arguments:   none
Return value:   TickType: Amount of time to sleep until next wakeup.
***********************************************************************************/
extern xTaskHandle  TouchScreenHnd; /* RTOS_USAGE */
static const POINT_TYPE cal_loc[]={{0,0}, {DD_FRAME_WIDTH, 0}, {0,DD_FRAME_HEIGHT},{DD_FRAME_WIDTH,DD_FRAME_HEIGHT}}; 

#define CALIBRATE_TIMEOUT 1000  /* amount of time (mS) touch point must remain within CALIBRATE_LIMIT */
#define CALIBRATE_LIMIT 100     /* A/D count value limit for stability */

static portTickType CalibrateTouch(void)
{
  static const char CalMsg[]="Touch Corner and Hold";
  int16_t index = 0;
  POINT_TYPE msg_loc;
  int16_t Width, Height;

  portTickType xDelay = port_mS_to_TICK(1000);  /* RTOS_USAGE */
  
  // only run this once
  if (0 != calibrated)
    return (xDelay);
  
  // run through calibration corners
  for(index=0; index < size_ra(TouchCalibration); index++)
  {
    // Suspend touch screen during calibration process to prevent resource conflict
    vTaskSuspend( TouchScreenHnd ); /* RTOS_USAGE */

    /* Let system know we're accessing External Memory */
    R_DDLCD_ExMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
    {
      // display user message in the corner
      Width = R_GAPI_FontWidthString((uint8_t *)CalMsg, &T1);
      Height = R_GAPI_FontHeight(&T1);
      msg_loc = cal_loc[index];
      
      if (msg_loc.x + Width >= DD_FRAME_WIDTH)
        msg_loc.x -= Width;
      if (msg_loc.y + Height >= DD_FRAME_HEIGHT)
        msg_loc.y -= Height;

      (void)R_GAPI_PutS((uint8_t const *)CalMsg, &T1, displayFrameBuf, msg_loc.x, msg_loc.y); 
    }
    R_DDLCD_ExMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
  
    {
      // Gather raw data and make sure stable (time and position) for sufficient period prior to acceptance
      portTickType touched_time = (uint16_t)xTaskGetTickCount();  /* RTOS_USAGE */
      portTickType dTime; /* RTOS_USAGE */
      do
      {
        POINT_TYPE sample;
        if (false == rawTouch(&sample))
        {
          /* keep timer reset if no touch */
          touched_time = (uint16_t)xTaskGetTickCount(); /* RTOS_USAGE */
        }
        else
        {
          POINT_TYPE diff;
          diff.x = sample.x - TouchCalibration[index].x;
          diff.x = (diff.x < 0)? -diff.x : diff.x;
          diff.y = sample.y - TouchCalibration[index].y;
          diff.y = (diff.y < 0)? -diff.y : diff.y;
          /* if touchpint varies from previous value by more than CALIBRATE_LIMIT, record new touch point and reset timeout */
          if ((diff.x > CALIBRATE_LIMIT) || (diff.y > CALIBRATE_LIMIT))
          {
            TouchCalibration[index] = sample;
            touched_time = (uint16_t)xTaskGetTickCount(); /* RTOS_USAGE */
          }
        }
        dTime = (uint16_t)xTaskGetTickCount() - touched_time; /* RTOS_USAGE */
      }
      while(dTime < port_mS_to_TICK(CALIBRATE_TIMEOUT));  /* RTOS_USAGE */
    }

    /* Let system know we're accessing External Memory */
    R_DDLCD_ExMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
    {
      // clear out calibration message
      (void)BMPCopyBackDisplay(msg_loc.x,msg_loc.y, Width, Height);

      // display calibration results...paint in back buffer and copy to front
      sprintf(TextBuffer, "%04X,%04X", TouchCalibration[index].x, TouchCalibration[index].y);
      Width = R_GAPI_FontWidthString((uint8_t *)TextBuffer, &T1);
      (void)R_GAPI_PutS((uint8_t const *)TextBuffer, &T1, backFrameBuf, SX(0.400), SY(0.800)-(index*Height)); 
      (void)BMPCopyBackDisplay(SX(0.400), SY(0.800)-(index*Height), Width, Height);
    }
    R_DDLCD_ExMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */

    if(index < size_ra(TouchCalibration)-1)
    {
      // require user to release touch prior to proceeding (if not last calibration point)
      portTickType touched_time = (uint16_t)xTaskGetTickCount();  /* RTOS_USAGE */
      portTickType dTime; /* RTOS_USAGE */
      do
      {
        POINT_TYPE sample;
        if (true == rawTouch(&sample))
        {
          touched_time = (uint16_t)xTaskGetTickCount(); /* RTOS_USAGE */
        }
        dTime = (uint16_t)xTaskGetTickCount() - touched_time; /* RTOS_USAGE */
      }
      while(dTime < port_mS_to_TICK(1000));  /* RTOS_USAGE */
    }
  }

  calibrated = true;
  // restore normal touchscreen operation
  vTaskResume( TouchScreenHnd );  /* RTOS_USAGE */
  
  return (xDelay);
}    

/**********************************************************************************
Function Name:   ButtonTouch
Description:  Display touch location data
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void ButtonTouch(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t Width=0;
  static int16_t PosX=0, PosY=0;
  int16_t Height = R_GAPI_FontHeight(&T1);
  int16_t PosXold = PosX;
  int16_t PosYold = PosY;
  int16_t WidthOld = Width;

  switch (pMsg->event)
  {
    case ME_MOVE:
      TextBuffer[0] = 'M';
      break;
    case ME_PRESS:
      TextBuffer[0] = 'P';
      break;
    case ME_RELEASE:
      TextBuffer[0] = 'R';
      break;
    default:
      return;
  }
  /* clear previous text from work buffer */
  (void)BMPCopyBackWork(PosXold,PosYold, WidthOld, Height);

  PosX = pMsg->param.coord[0];
  PosY = pMsg->param.coord[1];
  
  sprintf(&TextBuffer[1], "%d,%d", PosX, PosY);
  Width = R_GAPI_FontWidthString((uint8_t *)TextBuffer, &T1);
      
  if (PosX + Width >= DD_FRAME_WIDTH)
    PosX-=Width;
  if (PosY + Height >= DD_FRAME_HEIGHT)
    PosY-=Height;

  /* create in working buffer then copy to foreground buffer */
  (void)R_GAPI_PutS((uint8_t const *)TextBuffer, &T1, workFrameBuf, PosX, PosY); 
  /* move restore area and new text from work to display */
  (void)BMPCopyWorkDisplay(PosXold,PosYold, WidthOld, Height);
  (void)BMPCopyWorkDisplay(PosX,PosY, Width, Height);
}





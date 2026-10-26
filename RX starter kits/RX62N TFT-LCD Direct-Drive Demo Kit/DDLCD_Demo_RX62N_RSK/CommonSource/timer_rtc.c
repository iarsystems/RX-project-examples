/* Adapted for use with IAR Embedded Workbench */
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
/***********************************************************************************
FILE NAME:     timer_rtc.c
DESCRIPTION:   Provide Realtime Clock Services.


Copyright : 2010 Renesas Electronics America
Copyright : 2010 Renesas Electronics Corporation.
All Rights Reserved
***********************************************************************************/

/**********************************************************************************
System Includes
***********************************************************************************/
#include <machine.h>

/**********************************************************************************
User Includes
***********************************************************************************/
#include "Global.h"     
#include "LCD_demo.h"

/**********************************************************************************
Global variables
***********************************************************************************/

static uint32_t  time_counter32bit;
static EVENT_MSG msg;
static int16_t retVal;
static int16_t  ignore;
static char time_str[35];

enum {T_AM, T_PM};
enum {JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC};
enum {SUN, MON, TUE, WED, THU, FRI, SAT};

static const char day_of_week[7][4]= {"SUN","MON","TUE","WED","THU","FRI","SAT"};
static const char month[12][4]={"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
static const char am_pm[2][3]={"AM","PM"};

typedef struct {
  int tm_sec;       /*  seconds after the minute    */
  int tm_min;       /*  minutes after the hour      */
  int tm_hour;      /*  hours since midnight        */
  int tm_mday;      /*  day of the month            */
  int tm_mon;       /*  months since January        */
  int tm_year;      /*  years since 1900            */
  int tm_wday;      /*  days since Sunday           */
  int tm_yday;      /*  days since January 1        */
  int tm_isdst;     /*  daylight savings time flag  */
  int tm_am_pm;    //  am or pm
}tm;

tm time;        // Declare an instance of the time structure defined in timer32k.h


/**********************************************************************************
User Program Code
***********************************************************************************/

/**********************************************************************************
Function Name:  Timer32k_init
Description:  Initialize timer32K. The calendar is also initialized here.
Arguments:    none
Return value:  none
***********************************************************************************/
static void Timer1S(void *pvParameters);        // The task of Event Manager
xTaskHandle      Timer1SHnd;     /* RTOS_USAGE */

void TimerRTC_init(void)       
{
  (void)xTaskCreate(Timer1S, "t:Time", configMINIMAL_STACK_SIZE, NULL, mainDEMOTASKS_PRIORITY+1, &Timer1SHnd);        /* RTOS_USAGE */
  
  time.tm_sec=00;
  time.tm_min=59;
  time.tm_hour=11;
  time.tm_am_pm=T_AM;
  time.tm_wday=FRI;
  time.tm_mday=20;
  time.tm_mon=SEP;
  time.tm_year=2010;
  
}         

/**********************************************************************************
Function Name:  TimeUpdate
Description:  Update time structure
Arguments:    none
Return value:  none
***********************************************************************************/
void TimeUpdate(uint32_t timer)
{
  static uint32_t prev_timer;
  time.tm_sec+= timer-prev_timer;

  prev_timer=timer;

  if(time.tm_sec>=60)
  {
    time.tm_min++;
    time.tm_sec=0;
  }
  if(time.tm_min>=60)
  {
    time.tm_hour++;
    time.tm_min=0;
  
    if(time.tm_hour==12)
    {
      time.tm_am_pm++;
      
      if(time.tm_am_pm>T_PM)
      {
        time.tm_am_pm=T_AM;
        time.tm_mday++;
        time.tm_wday++;
      }
      if(time.tm_wday>SAT)
      {
        time.tm_wday=SUN;
      }
      if( ((time.tm_mday==29) & (time.tm_mon==FEB)) \
         |((time.tm_mday==31) & ((time.tm_mon==SEP)|(time.tm_mon==APR)|(time.tm_mon==JUN)|(time.tm_mon==NOV))) \
         |(time.tm_mday==32))
      {
        time.tm_mon++;
        time.tm_mday=1;
      }
      if(time.tm_mon>DEC)
      {
        time.tm_mon=JAN;
        time.tm_year++;
      }    
    }
  }
  if(time.tm_hour==13)
  {
    time.tm_hour=1;
  }
}
/**********************************************************************************
End of function TimeUpdate
***********************************************************************************/  

static void Timer1S(void *pvParameters)
{
  EVENT_MSG msg;
  msg.source = MS_DISPLAY;
  msg.event = ME_TIME_UPDATE;

  portTickType xLastWakeTime, xFrequency = port_mS_to_TICK(1000); /* RTOS_USAGE */

  xLastWakeTime = xTaskGetTickCount();  /* RTOS_USAGE */
  
  for ( ;; )
  {    
    // Wait for the next cycle.
    vTaskDelayUntil( &xLastWakeTime, xFrequency );  /* RTOS_USAGE */
    time_counter32bit++;      // a 32 bit counter that is updated every tick ( 1 second)
    TimeUpdate(time_counter32bit);
    msg.param.value = time_counter32bit;
    (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
  }
}

/**********************************************************************************
Function Name:   TimerRTC_Display
Description:  Return to previous screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void TimerRTC_Display(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t width;
  static int16_t height;
  
  if ((ME_TIME_UPDATE == pMsg->event) || (ME_DRAW_WORK == pMsg->event) )
  {
    (void)BMPCopyBackWork( pS->PosX, pS->PosY, width, height );

    sprintf(time_str,"%s  %02d:%02d:%02d %s   %02d %s %04d",day_of_week[time.tm_wday],time.tm_hour,time.tm_min,time.tm_sec,am_pm[time.tm_am_pm],time.tm_mday,month[time.tm_mon],time.tm_year);
    (void)R_GAPI_PutS((uint8_t *)time_str, &pS->pScheme[0].font, workFrameBuf,pS->PosX, pS->PosY);

    if(ME_DRAW_WORK != pMsg->event)
      (void)BMPCopyWorkDisplay( pS->PosX, pS->PosY, width, height );
  
    width = R_GAPI_FontWidthString((uint8_t *)time_str, &pS->pScheme[0].font); // this is done here so the info is available next time
    height= R_GAPI_FontHeight(&pS->pScheme[0].font);
  }
}

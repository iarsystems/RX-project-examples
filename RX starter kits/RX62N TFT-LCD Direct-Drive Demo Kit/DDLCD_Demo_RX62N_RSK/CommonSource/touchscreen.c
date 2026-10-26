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
/*  FILE        :TouchScreen.c                                         */
/*  DATE        :Mon, Jul 23, 2007                                     */
/*  DESCRIPTION :Drives the touch screen                               */
/*  CPU TYPE    :H8S/2378R                                             */
/*                                                                     */
/*                                                                     */
/*                                                                     */
/***********************************************************************/

/**********************************************************************
*  External interface:
*  initTouch() - Call before using touch panel
*  calibrateTouch() - Calibrates the touch panel to the screen
*  scanTouch() - Scan the touch panel for touches
**********************************************************************/
#include "Global.h"
#include "LCD_demo.h"
#include "touchscreen.h"

/***********************************************************************************
Macro Defines
***********************************************************************************/
#define TOUCH_PANEL_WIDTH         DD_H_DOT_DISPLAY
#define TOUCH_PANEL_HEIGHT        DD_V_LINES_DISPLAY

void getTouchCalibration(int16_t *pTC);

void ad_init(void);
void ad_acquire(void);
uint16_t ad_value(int channel);

enum{TC_LL, TC_LR, TC_UL, TC_UR};

POINT_TYPE TouchCalibration[4]=  /* calibration points 0=LL, 1=LR, 2=UL, 3=UR */
{
  {TC_LL_X, TC_LL_Y},
  {TC_LR_X, TC_LR_Y},
  {TC_UL_X, TC_UL_Y},
  {TC_UR_X, TC_UR_Y}
};

/*****************************************************************************/
//
// FUNCTION    :initTouch()
// PARAMETERS  :none
// RETURNS     :none
//
// DESCRIPTION :Initialize the touch panel.  Set up the I/O's and ADC
//
/*****************************************************************************/
void initTouch()
{
  PINo(OUT_TS_XR) = 0;
  PINo(OUT_TS_XL) = 0;
  PINo(OUT_TS_YU) = 0;
  PINo(OUT_TS_YL) = 0;
  DDRo(OUT_TS_XR);
  DDRo(OUT_TS_XL);
  DDRo(OUT_TS_YU);
  DDRo(OUT_TS_YL);
  
  /* configure DTC to acquire multiple A/D samples */
  ad_init();
}

/*****************************************************************************/
//
// FUNCTION    :checkTouch()
// PARAMETERS  :none
// RETURNS     :compare test
//
// DESCRIPTION :helper function to test if x/y inputs are at same potential.
/*****************************************************************************/
#define COMPARE_LIMIT (64)
static _Bool checkTouch(void)
{
  int16_t compare;
  ad_acquire();
  vTaskDelay(1L); /* RTOS_USAGE */

  compare = (int16_t)ad_value(ADC_TS_XL) + (int16_t)ad_value(ADC_TS_XR);
  compare -= (int16_t)ad_value(ADC_TS_YU) + (int16_t)ad_value(ADC_TS_YL);
  compare = (compare >= 0)? compare:-compare;
  if (compare > COMPARE_LIMIT)
    return(false);
  else
    return(true);
}

/*****************************************************************************/
//
// FUNCTION    :rawTouch()
// PARAMETERS  :pointer to a "point"
// RETURNS     :0=no touch, 1=touched
//
// DESCRIPTION :Read raw status of the touch screen.
//
/*****************************************************************************/
_Bool rawTouch(POINT_TYPE *pRaw)
{
  _Bool touched = true;

  // Check if inputs all at same potential
  if (false == checkTouch())
    touched = false;
    
  // read axis values...even if not touched to keep timing consistant
  // Assert Y axis drivers
  PINo(OUT_TS_YU) = 1;
  PINo(OUT_TS_YL) = 1;
 
  // Wait for inputs to settle
  vTaskDelay(1L); /* RTOS_USAGE */

  // Read Y axis inputs
  ad_acquire();
  vTaskDelay(1L); /* RTOS_USAGE */
  pRaw->y = (int16_t)ad_value(ADC_TS_XL) + (int16_t)ad_value(ADC_TS_XR);
  
  // de-assert Y axis drivers
  PINo(OUT_TS_YU) = 0;
  PINo(OUT_TS_YL) = 0;

  vTaskDelay(1L); /* RTOS_USAGE */

  // Check if inputs all at same potential
  if (false == checkTouch())
    touched = false;
    
  // Assert X axis drivers
  PINo(OUT_TS_XL) = 1;
  PINo(OUT_TS_XR) = 1;

  // Wait for inputs to settle
  vTaskDelay(1L); /* RTOS_USAGE */

  // Read X axis input
  ad_acquire();
  vTaskDelay(1L); /* RTOS_USAGE */
  pRaw->x = (int16_t)ad_value(ADC_TS_YU) + (int16_t)ad_value(ADC_TS_YL);

  // de-assert X axis drivers
  PINo(OUT_TS_XL) = 0;
  PINo(OUT_TS_XR) = 0;

  // Wait for inputs to settle
  vTaskDelay(1L); /* RTOS_USAGE */
  
  // Check if inputs all at same potential
  if (false == checkTouch())
    touched = false;

  return(touched); 
}

/*****************************************************************************/
//
// FUNCTION    :scanTouch()
// PARAMETERS  :pointer to a "point"
// RETURNS     :TOUCH, NO_TOUCH or MOVE_TO
//
// DESCRIPTION :Scan the touch pad to see if it's been touched.
//
/*****************************************************************************/
#define DEBOUNCE_TIME (75)
MessageEvent_t scanTouch(POINT_TYPE *pt)
{
  static MessageEvent_t pressed = ME_RELEASE;
  _Bool touched = true;
  static _Bool touched_old = true;
  static uint16_t touched_time;

  POINT_TYPE AD_new;
  static POINT_TYPE touchPoint;

  touched = rawTouch(&AD_new);
 
  // Ensure rational readings
  {
    int index;
    POINT_TYPE test_min;
    POINT_TYPE test_max;
    /* test min x */
    for (index=1, test_max=test_min=TouchCalibration[0]; index < size_ra(TouchCalibration); index++)
    {
      test_min.x = (test_min.x < TouchCalibration[index].x) ? test_min.x : TouchCalibration[index].x;
      test_min.y = (test_min.y < TouchCalibration[index].y) ? test_min.y : TouchCalibration[index].y;
      test_max.x = (test_max.x > TouchCalibration[index].x) ? test_max.x : TouchCalibration[index].x;
      test_max.y = (test_max.y > TouchCalibration[index].y) ? test_max.y : TouchCalibration[index].y;
    }
    if ((AD_new.x < test_min.x) || (AD_new.y < test_min.y) || (AD_new.x > test_max.x) || (AD_new.y > test_max.y))
      touched = false;
  }
 
  /* reset timeout if there's a transition */
  if(touched != touched_old)
  {
    touched_time = (uint16_t)xTaskGetTickCount(); /* RTOS_USAGE */
    touched_old = touched;
  }

  {
    /* check debounce timeout for reporting */
    uint16_t dTime = (uint16_t)xTaskGetTickCount() - touched_time; /* RTOS_USAGE */
    dTime =  (uint16_t)port_TICK_to_mS(dTime);    /* RTOS_USAGE */
    if (dTime < DEBOUNCE_TIME)
      return(ME_INVALID);
    else
      touched_time = (uint16_t)xTaskGetTickCount(); /* RTOS_USAGE */
  }

  // Convert raw samples to Panel coordinates
  {
    /* get proportional location on screen...top/bottom/left/right */
    int32_t x_U, x_L, y_L, y_R;
    x_U = ((AD_new.x - TouchCalibration[TC_UL].x) * 0x10000L)/(TouchCalibration[TC_UR].x-TouchCalibration[TC_UL].x);
    x_L = ((AD_new.x - TouchCalibration[TC_LL].x) * 0x10000L)/(TouchCalibration[TC_LR].x-TouchCalibration[TC_LL].x);
    y_L = ((AD_new.y - TouchCalibration[TC_LL].y) * 0x10000L)/(TouchCalibration[TC_UL].y-TouchCalibration[TC_LL].y);
    y_R = ((AD_new.y - TouchCalibration[TC_LR].y) * 0x10000L)/(TouchCalibration[TC_UR].y-TouchCalibration[TC_LR].y);
    
    /* factor in location on screen to utilize 4 pt calibration data */
    touchPoint.x = (int16_t)(((((x_U - x_L) * ((y_L+y_R)/2) >> 16) + x_L) * TOUCH_PANEL_WIDTH) >> 16);
    touchPoint.y = (int16_t)(((((y_R - y_L) * ((x_L+x_U)/2) >> 16) + y_L) * TOUCH_PANEL_HEIGHT) >> 16);

    touchPoint.x = (touchPoint.x < 0) ? 0 : touchPoint.x;    
    touchPoint.x = (touchPoint.x >= TOUCH_PANEL_WIDTH) ? TOUCH_PANEL_WIDTH-1 : touchPoint.x;    
    touchPoint.y = (touchPoint.y < 0) ? 0 : touchPoint.y;    
    touchPoint.y = (touchPoint.y >= TOUCH_PANEL_HEIGHT) ? TOUCH_PANEL_HEIGHT-1 : touchPoint.y;    
  }

  if (touched == true)
  {
    if (pressed == ME_RELEASE)
      pressed = ME_PRESS;
    else
      pressed = ME_MOVE;
    *pt= touchPoint;
  }
  else
  {
    if (pressed != ME_RELEASE)
      pressed = ME_RELEASE;
    else
      return(ME_INVALID);
  }
  return(pressed);
}


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
/*  FILE        :DemoInit.c                                            */
/*  DATE        :Sep 12, 20015                                         */
/*  DESCRIPTION :Framebuffer and Icon callback initialization          */        
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
#include "r_touch_simulate.h"

static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);
static portTickType democheck(void);

/* The number of ticks before demo mode is entered */
#define TICKS_FOR_DEMO (30000)


/* The Home Screen ICON table will be built from the _SCR_HM section collected from all the screen files
     - By doing this, screens can be added (or removed) simply by linking them into the project */
/* To replace this mechanism (table built from _SCR_HM sections), simply create a local ScreenObj_t array and 
   point the ScreenHomeData.objects value to this table (refer to any other screen file for example) */

/* MUST INCLUDE NULL TERMINATED RECORD */
/* The _SCR_HMT section MUST be located immediately after the _SCR_HM to properly terminate the home list */
#pragma location=".SCR_HMT"
__root const ScreenObj_t HomeIconNull= { NULL,         NULL,           NULL,                   0,   0 };

#pragma section=".SCR_HM"

const Screen_t ScreenHomeData=
{
  Constructor, Destructor, (ScreenObj_t *)__section_begin(".SCR_HM")
};

_Bool b_first_touch = 0;  /* Used to check if demo mode should be entered */
portTickType homescr_start_time;
/*******************************************************************************
* Function Name: Constructor
* Description  : Opens a screen object.
* Arguments    : pS -
*                    screen object
* Return Value : None
*******************************************************************************/
static void Constructor(Screen_t const *pS)
{
  /* locate and point to common resources */
  if(NULL == pfontTerminal )
    {
		pfontTerminal = FileFind(pResources, "Square721 BT_Roman_16");
	}
    
  if(NULL == pBMP_HomeIcon )
    {
		pBMP_HomeIcon = FileFind(pResources, "Home");
	}
    
  if(NULL == pfontSqBT_10 )
    {
		pfontSqBT_10 = FileFind(pResources, "Square721 BT_Roman_10");
	}  
  
  if(NULL == pfontSqBT_B12 )
    {
		pfontSqBT_B12 = FileFind(pResources, "Square721 BT_Roman_B12");
	}

  if(NULL == pfontSqBT_13 )
    {
		pfontSqBT_13 = FileFind(pResources, "Square721 BT_Roman_13");
	}
  
  if(NULL == pfontSqBT_14 )
    {
		pfontSqBT_14 = FileFind(pResources, "Square721 BT_Roman_14");
	}
  
  if(NULL == pfontSqBT_16 )
    {
		pfontSqBT_16 = FileFind(pResources, "Square721 BT_Roman_16");
	}
  
  if(NULL == pfontSqBT_18 )
    {
		pfontSqBT_18 = FileFind(pResources, "Square721 BT_Roman_18");
	}

  if(NULL == pfontSqBT_B24 )
    {
		pfontSqBT_B24 = FileFind(pResources, "Square721 BT_Roman_B24");
	}
  
  if(NULL == pfontSqBT_40 )
    {
		pfontSqBT_40 = FileFind(pResources, "Square721 BT_Roman_40");
	}
  
  if(NULL == pfontSqBT_B42 )
    {
		pfontSqBT_B42 = FileFind(pResources, "Square721 BT_Roman_B42");
	}
  
  if(NULL == pfontSqBT_B60 )
    {
		pfontSqBT_B60 = FileFind(pResources, "Square721 BT_Roman_B60");
	}
	
  if(NULL == pfontComicSansMS_Regular_B18 )
    {
		pfontComicSansMS_Regular_B18 = FileFind(pResources,
				"ComicSansMS_Regular_B18");
	}
  
  if(NULL == pfontComicSansMS_Regular_B22 )
    {
		pfontComicSansMS_Regular_B22 = FileFind(pResources,
				"ComicSansMS_Regular_B22");
	}

  {
    static const BMP_ColorTable_t local_ct={CT_BLACK};

    /* Clear the background */
    (void)R_GAPI_Fill(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT, &local_ct);
  }

  /* Run default behavior */
  BasicConstructor(pS);

  homescr_start_time = xTaskGetTickCount(); /* RTOS_USAGE */

  /* start screen tasks to simulate data and generate events */
  (void)ScreenTaskStart(democheck);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

/*******************************************************************************
* Function Name: Destructor
* Description  : Closes a screen object.
* Arguments    : pS -
*                    screen object
* Return Value : None
*******************************************************************************/
static void Destructor(Screen_t const *pS)
{

  /* First home screen touch, disable demo mode checking */
  b_first_touch = 1;

  /* Run default behaviour */
  BasicDestructor(pS);
}
/*******************************************************************************
End of function Destructor
*******************************************************************************/

/*******************************************************************************
* Function Name: democheck
* Description  : Checks conditions for entry in to demo mode
* Arguments    : None
* Return Value : None
*******************************************************************************/
static portTickType democheck(void)
{
    if( 0 == b_first_touch )
    {
        if((xTaskGetTickCount() - homescr_start_time) > port_mS_to_TICK(TICKS_FOR_DEMO))
        {
            /* There has not been a first touch within the defined period - enable demo mode */
            b_first_touch = 1;
            R_TOUCH_EnableSimulation();
        }
    }

  return (port_mS_to_TICK(100));
}
/*******************************************************************************
End of function democheck
*******************************************************************************/

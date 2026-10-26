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

static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

/* The Home Screen ICON table will be built from the _SCR_HM section collected from all the screen files
     - By doing this, screens can be added (or removed) simply by linking them into the project */
/* To replace this mechanism (table built from _SCR_HM sections), simply create a local ScreenObj_t array and 
   point the ScreenHomeData.objects value to this table (refer to any other screen file for example) */

/* MUST INCLUDE NULL TERMINATED RECORD */
/* The _SCR_HMT section MUST be located immediately after the _SCR_HM to properly terminate the home list */
#pragma location=".C_SCR_HMT"
__root const ScreenObj_t HomeIconNull= { NULL,         NULL,           NULL,                   0,   0 };

// The C_SCR_HM_LOCATION must be defined as the start of the 
// C_SCR_HM_region in the lnkr5f562n8_DirectLCD.icf linker
// configuration file.
#define C_SCR_HM_LOCATION 0xfff8e1dc

const Screen_t ScreenHomeData=
{
  Constructor, Destructor , (ScreenObj_t *)C_SCR_HM_LOCATION
};

static void Constructor(Screen_t const *pS)
{
  /* locate and point to common resources */
  if(pfontTerminal == NULL)
    pfontTerminal = FileFind(pResources, "Square721 BT_Roman_16");
    
  if(pBMP_HomeIcon == NULL)
    pBMP_HomeIcon = FileFind(pResources, "Home");
    
  if(pfontSqBT_10 == NULL)
    pfontSqBT_10= FileFind(pResources, "Square721 BT_Roman_10");  
  
  if(pfontSqBT_B12 == NULL)
    pfontSqBT_B12= FileFind(pResources, "Square721 BT_Roman_B12");

  if(pfontSqBT_13 == NULL)
    pfontSqBT_13= FileFind(pResources, "Square721 BT_Roman_13");
  
  if(pfontSqBT_14 == NULL)
    pfontSqBT_14= FileFind(pResources, "Square721 BT_Roman_14");
  
  if(pfontSqBT_16 == NULL)
    pfontSqBT_16= FileFind(pResources, "Square721 BT_Roman_16");
  
  if(pfontSqBT_18 == NULL)
    pfontSqBT_18= FileFind(pResources, "Square721 BT_Roman_18");

  if(pfontSqBT_B24 == NULL)
    pfontSqBT_B24= FileFind(pResources, "Square721 BT_Roman_B24");
  
  if(pfontSqBT_40 == NULL)
    pfontSqBT_40= FileFind(pResources, "Square721 BT_Roman_40");
  
  if(pfontSqBT_B42 == NULL)
    pfontSqBT_B42= FileFind(pResources, "Square721 BT_Roman_B42");
  
  if(pfontSqBT_B60 == NULL)
    pfontSqBT_B60= FileFind(pResources, "Square721 BT_Roman_B60");
 
  {
    static const BMP_ColorTable_t local_ct={CT_BLACK};
    // Clear the background
    (void)R_GAPI_Fill(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT, &local_ct);
  }

  // Run default behavior
  BasicConstructor(pS);
}

static void Destructor(Screen_t const *pS)
{
  // Run default behavior
  BasicDestructor(pS);
}


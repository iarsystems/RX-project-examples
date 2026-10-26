/* Adapted to IAR Embedded Workbench */
/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : ScreenRefrig.c
* Version      : 1.0 <- Optional as long as history is shown below
* Description  : Screen handler showing refrigerator demo
*******************************************************************************/
/*Screen Refidgerator */
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
12.09.2015 REE - Ported from RX63N GAPI Demo
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"
#include "Jpeg/r_expand_jpegd.h"

/*Prototypes */
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

static void WaterPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void CubesPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void CrushPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

static void LockPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void LightsPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void FilterStatus(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

static void RefrigDataBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void RefrigPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void FreezerDataBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void FreezerPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

/*Declarations */
static BMP_t const *pBMP_TempBox;

static BMP_t const *pBMP_FrzPlus;
static BMP_t const *pBMP_FrzMinus;
static BMP_t const *pBMP_FrgPlus;
static BMP_t const *pBMP_FrgMinus;

static BMP_t const *pBMP_Water;
static BMP_t const *pBMP_Cubes;
static BMP_t const *pBMP_Crush;

static BMP_t const *pBMP_Lock;
static BMP_t const *pBMP_Lights;
static BMP_t const *pBMP_RSlider;
static BMP_t const *pBMP_Status;

static const GAPI_Scheme_t T_RefrigHomeScheme[]= 
{
  /* Button off */
  {
    {CT_NONE,CT_WHITE}, 
    {&pfontSqBT_B12, {CT_NONE,CT_NONE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_MULC}
  },

  /* Button on */
  {
    {CT_NONE,CT_BLACK}, 
    {&pfontSqBT_B12, {CT_NONE,CT_NONE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_MULC}
  }
};

static const GAPI_Scheme_t T_RefrigScheme[]= 
{
  /* Button off */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_10, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.05),GAPI_T_MULT}
  },

  /* Button on */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_10, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.05),GAPI_T_NONE}
  }
};

static const GAPI_Scheme_t T_RefrigSliderScheme[]= 
{
  /* Button off */
  {
    {CT_NONE,CT_WHITE}, 
    {&pfontSqBT_10, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.05),GAPI_T_MULC}
  },

  /* Button on */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_10, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.05),GAPI_T_MULT}
  }
};

static const GAPI_Scheme_t T_RefrigStatusScheme[]= 
{
  /* Button off */
  {
    {CT_NONE,CT_WHITE}, 
    {&pfontSqBT_B12, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  },

  /* Button on */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_B12, {CT_NONE,CT_RED}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_NONE}
  }
};

static const GAPI_Scheme_t T_RefrigPlusMinusScheme[]= 
{
  /* Button off */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_B42, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MUL}
  },

  /* Button on */
  {
    {CT_NONE,{0x79, 0x6E, 0x3E, 0x00}}, 
    {&pfontSqBT_B42, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_NONE}
  }
};

static const GAPI_Scheme_t T_LabelScheme= 
{
  {CT_NONE,CT_GRAY_25}, 
  {&pfontSqBT_B12, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
  {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.95),GAPI_T_MULC}
};

static const GAPI_Scheme_t T_BoxTopScheme[]= 
{
  /* Label */
  {
    {CT_NONE,CT_BLACK}, 
    {&pfontSqBT_18, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.80),GAPI_T_MULC}
  },

  /* Data */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_B24, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_RIGHT,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.90,0.35),GAPI_T_NONE},
  },
};

static const GAPI_Scheme_t T_RefrigCalendarScheme[]= 
{
  /* Calendar Text Rendering */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_14, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_LEFT,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_NONE},
  },
};

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
/*  Bit map Address     Color Scheme                 Function call     X Pos       Y Pos  */
  { &pBMP_ButtonS,      T_RefrigCalendarScheme,      TimerRTC_Display,    6,        220 },
  { &pBMP_HomeIcon,     T_RefrigHomeScheme,          ButtonBack,        278,        207 },
  { &pBMP_TempBox,      T_BoxTopScheme,              FreezerDataBox,      4,        127 },
  { &pBMP_FrzPlus,      T_RefrigPlusMinusScheme,     FreezerPress,        4,         69 },
  { &pBMP_FrzMinus,     T_RefrigPlusMinusScheme,     FreezerPress,       54,         69 },
  { &pBMP_TempBox,      T_BoxTopScheme,              RefrigDataBox,     216,        127 },
  { &pBMP_FrgPlus,      T_RefrigPlusMinusScheme,     RefrigPress,       217,         69 },
  { &pBMP_FrgMinus,     T_RefrigPlusMinusScheme,     RefrigPress,       266,         69 },
  { &pBMP_Water,        T_RefrigScheme,              WaterPress,        129,        156 },
  { &pBMP_Cubes,        T_RefrigScheme,              CubesPress,        129,        109 },
  { &pBMP_Crush,        T_RefrigScheme,              CrushPress,        129,         64 },
  { &pBMP_RSlider,      T_RefrigSliderScheme,        LockPress,          9,           10 },
  { &pBMP_RSlider,      T_RefrigSliderScheme,        LightsPress,       228,          10 },
  { &pBMP_Status,       T_RefrigStatusScheme,        FilterStatus,      111,          6 },
  
  { NULL,               NULL,                        NULL,                0,          0 },
};

Screen_t ScreenRefrigData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;

static void Constructor(Screen_t const *pS)
{
  /* Clear the background */
  {
    uint8_t const *pJPG = FileFind(pResources, "Refrig_BG");
    int32_t JPGsize = FileSizeFind(pResources, "Refrig_BG");
    int32_t width = R_BMP_Width(backFrameBuf);
    int32_t height = R_BMP_Height(backFrameBuf);
    {
      uint16_t JPGwidth;
      uint16_t JPGheight;

  	  R_init_jpeg();

      /* ensure we have valid jpeg file */
      if(R_get_info_jpeg((uint8_t *)pJPG,JPGsize,&JPGwidth,&JPGheight) == 0)
      {
        /* ensure file will fit in frame buffer */
        if((JPGwidth <= width) && (JPGheight <= height))
        {
          /* expand the file (-width to flip orientation to match GAPI convention) */
		      R_expand_jpeg((uint8_t *)pJPG, JPGsize, 
            (uint16_t *)(((uint8_t *)backFrameBuf) + R_BMP_Offset(backFrameBuf))+(width*(height-1)), -width);
        }
      }
    }
  }      
  
  /* Run default behavior */
  BasicConstructor(pS);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

/**********************************************************************************
Function Name: Destructor
Description:   Runs the default behaviour
Parameters:    *pS
               *pMsg
Return value:  None
***********************************************************************************/
static void Destructor(Screen_t const *pS)
{
  /* Run default behavior */
  BasicDestructor(pS);
}
/*******************************************************************************
End of function Destructor
*******************************************************************************/

/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
- By doing this, screens can be added (or removed) simply by linking them into the project */
static BMP_t const *pBMP_ButtonRefrig;
static void Refrig(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#pragma location=".SCR_HM"
__root const ScreenObj_t HomeIconFridge= { &pBMP_ButtonRefrig, T_HomeScheme,    Refrig,     172, 80 };

/**********************************************************************************
Function Name:   Refrig
Description:  Demo Refrig on Demo Home screen
Parameters:   *pS
              *pMsg
Return value:   None
***********************************************************************************/
static void Refrig(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  
  if(NULL == pBMP_ButtonRefrig)
  {
    pBMP_ButtonRefrig = FileFind(pResources, "Home_Refrig");
  }

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"FRIDGE DEMO", state)))
  {
    if (ME_PRESS == pMsg->event)
    {
      /* send a screen change command to the handler */
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenRefrigData;
      (void)(int16_t)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
  }
}
/*******************************************************************************
End of function Refrig
*******************************************************************************/

/**********************************************************************************
Function Name:   WaterPress
Description:  Water Button Press
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void WaterPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{    
  static int16_t state=0;
  
  /* locate the BMP resources */
  if(NULL == pBMP_Water)
  {
    pBMP_Water = FileFind(pResources, "Refrig_water");
  }
  
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"WATER", state)))
  {
	  /* Do nothing */
  }
}
/*******************************************************************************
End of function WaterPress
*******************************************************************************/
  
/**********************************************************************************
Function Name:   CubesPress
Description:  Cubes Button Press
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void CubesPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{    
  static int16_t state=0;
  
  /* locate the BMP resources */
  if(NULL == pBMP_Cubes)
  {
    pBMP_Cubes = FileFind(pResources, "Refrig_cube");
  }

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"CUBES", state)))
  {
	  /* Do nothing */
  }
}
/*******************************************************************************
End of function CubesPress
*******************************************************************************/

/**********************************************************************************
Function Name:   CrushPress
Description:  Crush Button Press
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void CrushPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{    
  static int16_t state=0;
  
  /* locate the BMP resources */
  if(NULL == pBMP_Crush)
  {
    pBMP_Crush = FileFind(pResources, "Refrig_crush");
  }

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"CRUSH", state)))
  {
	  /* Do nothing */
  }
}
/*******************************************************************************
End of function CrushPress
*******************************************************************************/

/**********************************************************************************
Function Name:   LockPress
Description:  Lock Button Press
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void LockPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{ 
  static SLIDERSTATE_type State;
  uint8_t const *const label[]={"LOCK", "UNLOCK"};
  
  /* locate the BMP resources */
  if(NULL == pBMP_Lock)
  {
    pBMP_Lock  = FileFind(pResources, "Refrig_lock");
  }

  if(NULL == pBMP_RSlider)
  {
    pBMP_RSlider= FileFind(pResources, "Refrig_slider");
  }
    
  if(0 != (State.Touched = SliderSwitchHandler(pS, pMsg, pBMP_Lock, label, &State)))
  {
	  /* Do nothing */
  }
}
/*******************************************************************************
End of function LockPress
*******************************************************************************/

/**********************************************************************************
Function Name:   LightsPress
Description:  Lights Button Press
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void LightsPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{    
  static SLIDERSTATE_type State;
  uint8_t const *const label[]={"OFF", "ON"};
  
  /* locate the BMP resources */
  if(NULL == pBMP_Lights)
  {
    pBMP_Lights= FileFind(pResources, "Refrig_bulb");
  }

  if(NULL == pBMP_RSlider)
  {
    pBMP_RSlider= FileFind(pResources, "Refrig_text");
  }
    
  if(0 != (State.Touched = SliderSwitchHandler(pS, pMsg, pBMP_Lights, label, &State)))
  {
      LED0 = ((uint8_t)(State.Value));
  }
}
/*******************************************************************************
End of function LightsPress
*******************************************************************************/

/**********************************************************************************
Function Name:   FilterStatus
Description:  Status Message Service
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
uint8_t FilterCondition;
void FilterStatus(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{    
  static int16_t state=0;
  uint8_t const *const label[]={"GOOD", "PURCHASE", "REPLACE"};

  /* locate the BMP resources */
  if(NULL == pBMP_Status)
  {
    pBMP_Status= FileFind(pResources, "Refrig_text");
  }

  if(pMsg->event == ME_DRAW)
  {
    /* draw shaded dispenser area in background */
    R_GAPI_Button(FileFind(pResources, "Refrig_T"), backFrameBuf, 5, 0,&T_LabelScheme,"DISPENSER");

    /* draw filter text in background */
    (void)R_GAPI_PutS("FILTER", &pS->pScheme[0].font, backFrameBuf, 159, 47);
  }
  
  if(0 != (state = ButtonHandler(pS, pMsg, label[FilterCondition], state)))
  {
    if(pMsg->event == ME_PRESS)
    {
      FilterCondition = (uint8_t)((FilterCondition+1) % ((sizeof(label)) / (sizeof(label[0]))));
    }
  }
}
/*******************************************************************************
End of function FilterStatus
*******************************************************************************/

/**********************************************************************************
Function Name:   RefrigDataBox
Description:  Refrigerator temperature display box
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
int16_t Refrig_Temperature=1.6;
static void RefrigDataBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  char buffer[8];
  
  /* locate resource */
  if(NULL == pBMP_TempBox)
  {
    pBMP_TempBox = FileFind(pResources, "refrig_temp");
  }

  /* make copy of our message */
  msg = *pMsg;

  /* check if the value display needs updating */
  if ((msg.event == ME_USER_UPDATE) && (0 == msg.param.value))
  {
    msg.event = ME_DRAW_DISPLAY;
  }
  
  /* create temperature string */
  (void)sprintf(buffer,"%d\xC2\xB0""C",Refrig_Temperature);
  (void)DataBoxHandler(pS, &msg, "FRIDGE", (uint8_t *)buffer);
}
/*******************************************************************************
End of function RefrigDataBox
*******************************************************************************/

/**********************************************************************************
Function Name:   RefrigPress
Description:  + or - temp Button Pressed
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void RefrigPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{    
  static int16_t state[2];
  uint8_t display_update = 0;
  uint8_t up;
  uint8_t const *const label[]={"-", "+"};

  /* locate the BMP resources */
  if(NULL == pBMP_FrgPlus) 
  {
    pBMP_FrgPlus  = FileFind(pResources, "Refrig_PlusMinus");
  }

  if(NULL == pBMP_FrgMinus)
  {
    pBMP_FrgMinus = FileFind(pResources, "Refrig_PlusMinus");
  }

  /* determine which button is calling us */  
  up = ((&pBMP_FrgPlus) == pS->ppBmp) ? 1: 0;

  /* process the touch */
  state[up] = ButtonHandler(pS, pMsg, label[up], state[up]);
  
  switch(pMsg->event)
  {
    case ME_DRAW_WORK:
      display_update = 1;
      break;
      
    case ME_PRESS:
    //case ME_MOVE:
      if(0 != state[up])
      {
		if ((1 == up) && (Refrig_Temperature < 5))
        {
          Refrig_Temperature++;
          display_update = 1;
        }
		if ((0 == up) && (Refrig_Temperature > 0))
        {
          Refrig_Temperature--;
          display_update = 1;
        }
      }
      break;

    case ME_MOVE:
        display_update = 1;
        break;
      
    case ME_RELEASE:      
      state[up] = 0;
      break;
    default:
      break;
  }
  
  if (0 != display_update)
  {
    /* send event to update display box */
    msg.source = MS_DISPLAY;
    msg.event = ME_USER_UPDATE;
    msg.param.value = 0;
    (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
  }
}
/*******************************************************************************
End of function RefrigPress
*******************************************************************************/

/**********************************************************************************
Function Name:   FreezerDataBox
Description:  Freezer temperature display box
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
int16_t Freezer_Temperature=-18;
static void FreezerDataBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  char buffer[8];

  /* locate resource */
  if(NULL == pBMP_TempBox)
  {
    pBMP_TempBox = FileFind(pResources, "refrig_temp");
  }

  /* make copy of our message */
  msg = *pMsg;

  /* check if the value display needs updating */
  if ((msg.event == ME_USER_UPDATE) && (0 == msg.param.value))
  {
    msg.event = ME_DRAW_DISPLAY;
  }
  
  /* create temperature string */
  (void)sprintf(buffer,"%d\xC2\xB0""C",Freezer_Temperature);
  (void)DataBoxHandler(pS, &msg, "FREEZER", (uint8_t *)buffer);
}
/*******************************************************************************
End of function FreezerDataBox
*******************************************************************************/

/**********************************************************************************
Function Name:   FreezerPress
Description:  + or - temp Button Pressed
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void FreezerPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{    
  static int16_t state[2];
  uint8_t display_update = 0;
  uint8_t up;
  uint8_t const *const label[]={"-", "+"};
  
  /* locate the BMP resources */
  if(NULL == pBMP_FrzPlus) 
  {
    pBMP_FrzPlus  = FileFind(pResources, "Refrig_PlusMinus");
  }
    
  if(NULL == pBMP_FrzMinus) 
  {
    pBMP_FrzMinus = FileFind(pResources, "Refrig_PlusMinus");
  }

  /* determine which button is calling us */  
  up = ((&pBMP_FrzPlus) == pS->ppBmp) ? 1: 0;

  /* process the touch */
  state[up] = ButtonHandler(pS, pMsg, label[up], state[up]);
   
  switch(pMsg->event)
  {
      case ME_DRAW_WORK:
            display_update = 1;
            break;

    case ME_PRESS:
    //case ME_MOVE:
      if(0 != state[up])
      {
		if ((1 == up) && (Freezer_Temperature < 0))
        {
          Freezer_Temperature++;
          display_update = 1;
        }
		if ((0 == up) && (Freezer_Temperature > (-20)))
        {
          Freezer_Temperature--;
          display_update = 1;
        }
      }
      break;

    case ME_MOVE:
        display_update = 1;
        break;
      
    case ME_RELEASE:
      state[up] = 0;
      break;
    default:
    	break;
  }

  if (0 != display_update)
  {
    /* send event to update display box */
    msg.source = MS_DISPLAY;
    msg.event = ME_USER_UPDATE;
    msg.param.value = 0;
    (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
  }
}
/*******************************************************************************
End of function FreezerPress
*******************************************************************************/


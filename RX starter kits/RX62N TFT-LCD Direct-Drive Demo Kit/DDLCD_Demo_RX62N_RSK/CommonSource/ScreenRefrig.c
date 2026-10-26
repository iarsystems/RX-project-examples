//Screen Refidgerator
/***********************************************************************/
/*                                                                     */
/*  FILE        :ScreenRefrig.c                                         */
/*  DATE        :Wen, Mar 11, 2010                                    */
/*  DESCRIPTION :Screen handler showing refrigerator demo              */        
/*                                                                     */
/*  CPU TYPE    :H8S/1668R                                             */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
*
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"

//Prototypes
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

static void DispPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
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

//Declarations
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
    {&pfontSqBT_B12, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.05),GAPI_T_MULT}
  },
  /* Button on */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_B12, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.05),GAPI_T_NONE}
  }
};

static const GAPI_Scheme_t T_RefrigSliderScheme[]= 
{
  /* Button off */
  {
    {CT_NONE,CT_WHITE}, 
    {&pfontSqBT_B12, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.05),GAPI_T_MULC}
  },
  /* Button on */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_B12, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}}, 
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
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.5, 0.85),GAPI_T_MULC}
  },
  /* Data */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_B42, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_RIGHT,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.90,0.35),GAPI_T_NONE},
  },
};

static const GAPI_Scheme_t T_RefrigCalendarScheme[]= 
{
  /* Calendar Text Rendering */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_18, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_LEFT,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_NONE},
  },
};

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
//  Bit map Address     Color Scheme                Function call    X Pos        Y Pos  
  { &pBMP_ButtonS,      T_RefrigCalendarScheme,      TimerRTC_Display,   10,     240 },
  { &pBMP_HomeIcon,     T_RefrigHomeScheme,          ButtonBack,     SX(0.872),   SY(0.870) },
  { &pBMP_TempBox,      T_BoxTopScheme,              FreezerDataBox,  24,        145 },
  { &pBMP_FrzPlus,      T_RefrigPlusMinusScheme,     FreezerPress,    24,         74 },
  { &pBMP_FrzMinus,     T_RefrigPlusMinusScheme,     FreezerPress,   100,         74 },
  { &pBMP_TempBox,      T_BoxTopScheme,              RefrigDataBox,  310,        145 },
  { &pBMP_FrgPlus,      T_RefrigPlusMinusScheme,     RefrigPress,    310,         74 },
  { &pBMP_FrgMinus,     T_RefrigPlusMinusScheme,     RefrigPress,    386,         74 },
  { &pBMP_Water,        T_RefrigScheme,              WaterPress,     210,         158 },
  { &pBMP_Cubes,        T_RefrigScheme,              CubesPress,     210,         108 },
  { &pBMP_Crush,        T_RefrigScheme,              CrushPress,     210,         58  },
  { &pBMP_RSlider,      T_RefrigSliderScheme,        LockPress,       24,         8 },
  { &pBMP_RSlider,      T_RefrigSliderScheme,        LightsPress,    352,         8 },
  { &pBMP_Status,       T_RefrigStatusScheme,        FilterStatus,   190,         8 },
  
  { NULL,                 NULL,             NULL,         0,           0         },
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
  (void)R_GAPI_Copy(FileFind(pResources, "Refrig_BG"), backFrameBuf, 0, 0);
  
  /* Run default behavior */
  BasicConstructor(pS);
}

static void Destructor(Screen_t const *pS)
{
  // Run default behavior
  BasicDestructor(pS);
}

/*****************************************************************************/
//
// FUNCTION    :Refrig()
// DESCRIPTION :Demo Refrig on Demo Home screen
//
/*****************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
- By doing this, screens can be added (or removed) simply by linking them into the project */
static BMP_t const *pBMP_ButtonRefrig;
static void Refrig(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#pragma location=".C_SCR_HM"
__root const ScreenObj_t HomeIconFridge= { &pBMP_ButtonRefrig, T_HomeScheme,    Refrig,     270, 80 };

static void Refrig(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  
  if(NULL == pBMP_ButtonRefrig)
    pBMP_ButtonRefrig = FileFind(pResources, "Home_Refrig");

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"FRIDGE DEMO", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // send a screen change command to the handler
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenRefrigData;
      (void)(int16_t)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
}
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
    pBMP_Water = FileFind(pResources, "Refrig_water");
  
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"WATER", state)))
  {
  }
}
  
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
    pBMP_Cubes = FileFind(pResources, "Refrig_cube");

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"CUBES", state)))
  {
  }
}

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
    pBMP_Crush = FileFind(pResources, "Refrig_crush");

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"CRUSH", state)))
  {
  }
}

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
    pBMP_Lock  = FileFind(pResources, "Refrig_lock");
  if(NULL == pBMP_RSlider)
    pBMP_RSlider= FileFind(pResources, "Refrig_text");  
    
  if(0 != (State.Touched = SliderSwitchHandler(pS, pMsg, pBMP_Lock, label, &State)))
  {
  }
}

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
    pBMP_Lights= FileFind(pResources, "Refrig_bulb");  
  if(NULL == pBMP_RSlider)
    pBMP_RSlider= FileFind(pResources, "Refrig_text");  
    
  if(0 != (State.Touched = SliderSwitchHandler(pS, pMsg, pBMP_Lights, label, &State)))
  {
    DDRo(LED_0);
    PINo(LED_0) = (State.Value == 0);
  }
}

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
    pBMP_Status= FileFind(pResources, "Refrig_text");  

  if(pMsg->event == ME_DRAW)
  {
    /* draw shaded dispenser area in background */
    R_GAPI_Button(FileFind(pResources, "Refrig_T"), backFrameBuf, 10, 0,&T_LabelScheme,"DISPENSER");
    /* draw filter text in background */
    (void)R_GAPI_PutS("FILTER", &pS->pScheme[0].font, backFrameBuf, 240, 45);
  }
  
  if(0 != (state = ButtonHandler(pS, pMsg, label[FilterCondition], state)))
  {
    if(pMsg->event == ME_PRESS)
      FilterCondition = (FilterCondition+1)%(sizeof(label)/sizeof(label[0]));
  }
}

/**********************************************************************************
Function Name:   RefrigDataBox
Description:  Refrigerator temperature display box
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
int16_t Refrig_Temperature=37;
static void RefrigDataBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  char buffer[8];
  
  /* locate resource */
  if(NULL == pBMP_TempBox)
    pBMP_TempBox = FileFind(pResources, "refrig_temp");

  /* make copy of our message */
  msg = *pMsg;
  /* check if the value display needs updating */
  if((msg.event == ME_USER_UPDATE) && (msg.param.value == 1))
  {
    msg.event = ME_DRAW_DISPLAY;
  }
  
  /* create temperature string */
  (void)sprintf(buffer,"%d\xC2\xB0""F",Refrig_Temperature);
  (void)DataBoxHandler(pS, &msg, "FRIDGE", (uint8_t *)buffer);
}

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
    pBMP_FrgPlus  = FileFind(pResources, "Refrig_PlusMinus");
  if(NULL == pBMP_FrgMinus) 
    pBMP_FrgMinus = FileFind(pResources, "Refrig_PlusMinus");

  /* determine which button is calling us */  
  up = (pS->ppBmp == &pBMP_FrgPlus) ? 1 : 0;
  /* process the touch */
  state[up] = ButtonHandler(pS, pMsg, label[up], state[up]);
  
  switch(pMsg->event)
  {
    case ME_DRAW_WORK:
      display_update = 1;
      break;
      
    case ME_PRESS:
    case ME_MOVE:
      if(0 != state[up])
      {
        if((up == 1) && (Refrig_Temperature < 50))
        {
          Refrig_Temperature++;
          display_update = 1;
        }
        if((up == 0) && (Refrig_Temperature > 20))
        {
          Refrig_Temperature--;
          display_update = 1;
        }
      }
      break;
      
    case ME_RELEASE:      
      state[up] = 0;
      break;
  }
  
  if (0 != display_update)
  {
    /* send event to update display box */
    msg.source = MS_DISPLAY;
    msg.event = ME_USER_UPDATE;
    msg.param.value = 1;
    (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
  }
}

/**********************************************************************************
Function Name:   FreezerDataBox
Description:  Freezer temperature display box
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
int16_t Freezer_Temperature=0;
static void FreezerDataBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  char buffer[8];
  /* locate resource */
  if(NULL == pBMP_TempBox)
    pBMP_TempBox = FileFind(pResources, "refrig_temp");

  /* make copy of our message */
  msg = *pMsg;
  /* check if the value display needs updating */
  if((msg.event == ME_USER_UPDATE) && (msg.param.value == 0))
  {
    msg.event = ME_DRAW_DISPLAY;
  }
  
  /* create temperature string */
  (void)sprintf(buffer,"%d\xC2\xB0""F",Freezer_Temperature);
  (void)DataBoxHandler(pS, &msg, "FREEZER", (uint8_t *)buffer);
}

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
    pBMP_FrzPlus  = FileFind(pResources, "Refrig_PlusMinus");
    
  if(NULL == pBMP_FrzMinus) 
    pBMP_FrzMinus = FileFind(pResources, "Refrig_PlusMinus");

  /* determine which button is calling us */  
  up = (pS->ppBmp == &pBMP_FrzPlus) ? 1 : 0;
  /* process the touch */
  state[up] = ButtonHandler(pS, pMsg, label[up], state[up]);
   
  switch(pMsg->event)
  {
    case ME_PRESS:
    case ME_MOVE:
      if(0 != state[up])
      {
        if((up == 1) && (Freezer_Temperature < 20))
        {
          Freezer_Temperature++;
          display_update = 1;
        }
        if((up == 0) && (Freezer_Temperature > -20))
        {
          Freezer_Temperature--;
          display_update = 1;
        }
      }
      break;
      
    case ME_RELEASE:      
      state[up] = 0;
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


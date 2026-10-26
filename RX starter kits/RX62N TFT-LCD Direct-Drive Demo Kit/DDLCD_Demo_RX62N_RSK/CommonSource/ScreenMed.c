//Screen Medical
/***********************************************************************/
/*                                                                     */
/*  FILE        :ScreenMed.c                                         */
/*  DATE        :Wen, Mar 15, 2010                                    */
/*  DESCRIPTION :Screen handler showing medical demo              */        
/*                                                                     */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
*
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"

static const GAPI_Scheme_t T_HomeButtonScheme[]=
{
  /* Button on */
  {
    {CT_NONE,CT_BLACK}, 
    {&pfontSqBT_16, {CT_BLACK,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_SOLID,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_MULC},
  },
  /* Button off */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_16, {CT_BLACK,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_SOLID,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_NONE},
  },
};
const GAPI_Scheme_t T_CalendarScheme[]= 
{
  /* Calendar Text Rendering */
  {
    {CT_NONE,CT_BLACK}, 
    {&pfontSqBT_18, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_LEFT,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_NONE},
  },
};

static const GAPI_Scheme_t T_SliderScheme[]=
{
  /* Button on */
  {
    {CT_NONE,CT_BLACK}, 
    {&pfontSqBT_40, {CT_NONE,CT_RED}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC},
  },
  /* Button off */
  {
    {CT_NONE,CT_BLACK}, 
    {&pfontSqBT_16, {CT_NONE,CT_NONE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_SOLID,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULT},
  },
};

static const GAPI_Scheme_t T_StatusScheme[]= 
{
  /* Label */
  {
    {CT_NONE,CT_GRAY_50}, 
    {&pfontSqBT_14, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.50,0.15),GAPI_T_MULT},
  },
  /* Data */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_B24, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_LEFT,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.10,0.40),GAPI_T_NONE},
  },
};
static const GAPI_Scheme_t T_BoxBottomScheme[]= 
{
  /* Label */
  {
    {CT_NONE,CT_BLACK}, 
    {&pfontSqBT_14, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.50,0.15),GAPI_T_MULC},
  },
  /* Data */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_B42, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_RIGHT,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.90,0.60),GAPI_T_NONE},
  },
};
static const GAPI_Scheme_t T_BoxTopScheme[]= 
{
  /* Label */
  {
    {CT_NONE,CT_BLACK}, 
    {&pfontSqBT_14, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.50,0.80),GAPI_T_MULC},
  },
  /* Data */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_B42, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_RIGHT,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.90,0.35),GAPI_T_NONE},
  },
};
static const GAPI_Scheme_t T_ButtonScheme[]= 
{
  /* Button on */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_18, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MUL},
  },
  /* Button off */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_18, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_NONE},
  },
  {
    {{0xA0,0xA0,0xA0},CT_NONE}, 
    {&pfontSqBT_B24, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_NONE},
  },
};
static const GAPI_Scheme_t T_PowerScheme[]= 
{
  /* Button on */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_40, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MUL},
  },
  /* Button off */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_40, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_NONE},
  },
};


//Prototypes
static void MedModePress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedStatus(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedStartStopPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedONOFFPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

static void MedSlider(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedUpdate(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedDataBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

static enum  {MODE_ALL, MODE_HEART, MODE_BPM, MODE_TEMP_SP02, MODE_COUNT} MedMode;
static int MedState;

//Declarations
static BMP_t const *pBMP_Mode;
static BMP_t const *pBMP_Status;
static BMP_t const *pBMP_StartStop;
static BMP_t const *pBMP_ONOFF;
static BMP_t const *pBMP_MedSlider;
static BMP_t const *pBMP_MedSliderButton;

typedef struct
{
  BMP_t const *Box;
  int16_t Value;
  int16_t ValuePrev;
  int16_t ValueMin;
  int16_t ValueMax;
  int16_t ValueUpdate;
  uint8_t const *Label;
} MedData_type;
enum {BoxHR, BoxTarget, BoxTemp, BoxSP02, BoxSYS, BoxDIA};
MedData_type MedData[]=
{
  /* Box, Value, ValuePrev, ValueMin, ValueMax, ValueUpdate,  Label,                     */
  {NULL,     85,         0,       75,       85,           0,  "HR [bpm] \xE2\x99\xA5"  }, /* heart rate */
  {NULL,     75,         0,       60,      100,           0,  "Set Target \xE2\x99\xA5"}, /* target rate */
  {NULL,    101,         0,       98,      104,           0,  "TEMP [\xC2\xB0""F]"     }, /* temp value */
  {NULL,     98,         0,       90,      100,           0,  "SpO\xE2\x82\x82 [%]"    }, /* sp02 value */
  {NULL,    148,         0,       70,       90,           0,  "SYS [mmHg]"             }, /* sys value */
  {NULL,     81,         0,       90,      100,           0,  "DIA [mmHg]"             }, /* dia value */
};

typedef struct
{
  int16_t PosY;
  int16_t Height;
} T_GraphState;

T_GraphState *pGraphState;

static SLIDERSTATE_type MedSliderState;

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
//  Bit map Address           Color Scheme            Function call        X Pos        Y Pos  
  { &pBMP_HomeIcon,           T_HomeButtonScheme,      ButtonBack,           418,     236 },
  { &pBMP_ButtonS,            T_CalendarScheme,        TimerRTC_Display,      10,     240 },
  { &MedData[BoxHR].Box,      T_BoxBottomScheme,       MedDataBox,            10,     162 },
  { &MedData[BoxTarget].Box,  T_BoxTopScheme,          MedDataBox,            10,     84 },
  { &MedData[BoxTemp].Box,    T_BoxBottomScheme,       MedDataBox,            10,     10 },
  { &MedData[BoxSP02].Box,    T_BoxBottomScheme,       MedDataBox,           128,      10 },
  { &MedData[BoxSYS].Box,     T_BoxBottomScheme,       MedDataBox,           246,      10 },
  { &MedData[BoxDIA].Box,     T_BoxBottomScheme,       MedDataBox,           364,      10 },
  { &pBMP_MedSlider,          T_SliderScheme,          MedSlider,            132,     80 },
  { &pBMP_Status,             T_StatusScheme,          MedStatus,            282,     184 },
  { &pBMP_Mode,               T_ButtonScheme,          MedModePress,         278,     136 },
  { &pBMP_StartStop,          T_ButtonScheme,          MedStartStopPress,    344,     136 },
  { &pBMP_ONOFF,              T_PowerScheme,           MedONOFFPress,        410,     136 },
  { NULL,                     NULL,                    NULL,                   0,        0         },
};

static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

Screen_t ScreenMedicalData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;
static void Constructor(Screen_t const *pS)
{
  pBMP_ONOFF = pBMP_StartStop = pBMP_Mode = FileFind(pResources, "MedButton");
  pBMP_Status = FileFind(pResources, "MedStatusBox");
  pBMP_MedSlider = FileFind(pResources, "MedSlider");
  pBMP_MedSliderButton = FileFind(pResources, "MedButtonSlider");
  MedData[BoxHR].Box = MedData[BoxTemp].Box = MedData[BoxDIA].Box = MedData[BoxSP02].Box = MedData[BoxSYS].Box =  FileFind(pResources, "Med_Box_Lo");
  MedData[BoxTarget].Box = FileFind(pResources, "Med_Box_Hi");
  
  pGraphState = (T_GraphState *)R_GAPI_PoolMemoryGet(1024);
  if(NULL != pGraphState)
  {
    int16_t index;
    for(index = 0; index < 256; index++)
    {
      pGraphState[index].PosY = 0;
      pGraphState[index].Height = 0;
    }
  }
  
  /* Clear the background */
  (void)R_GAPI_Copy(FileFind(pResources, "Med_BG"), backFrameBuf, 0, 0);
  
  // Run default behavior
  BasicConstructor(pS);
}

static void Destructor(Screen_t const *pS)
{
  R_GAPI_PoolMemoryFree((void *)pGraphState);
  // Run default behavior
  BasicDestructor(pS);
}


/**********************************************************************************/
/**********************************************************************************
Function Name:   Medical
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
- By doing this, screens can be added (or removed) simply by linking them into the project */
static void Medical(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static BMP_t const *pBMP_ButtonVital;

#pragma location=".C_SCR_HM"
__root const ScreenObj_t HomeIconMed= { &pBMP_ButtonVital,    T_HomeScheme,    Medical,        150,     80  };

static void Medical(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(NULL == pBMP_ButtonVital)
    pBMP_ButtonVital = FileFind(pResources, "Home_Med1");

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Demo Medical", state)))
    if (ME_RELEASE == pMsg->event)
    {
      // send a screen change command to the handler
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenMedicalData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
}

/**********************************************************************************
Function Name:   drawGraph
Description:  Draw the GraphData values
Arguments:   None
Return value:   None
***********************************************************************************/
static int8_t GraphData[256];
static int16_t GraphHead;
static int16_t GraphState=1;

static void drawGraph(void)
{
  static const BMP_ColorTable_t ct[]={{0x00,0x00,0x00,0x00},{0x80,0x80,0x00,0x00}};
  int index;
  int16_t PosX = 128;
  int16_t PosY = 130;
  int16_t offsetY, Height;

  PosY += 30;
  
  for(index = 1; index < 144; index++)
  {
    int8_t prevData = GraphData[(GraphHead + index - 144) % size_ra(GraphData)];
    int8_t thisData = GraphData[(GraphHead + index - 143) % size_ra(GraphData)];
    if(prevData < thisData)
    {
      offsetY = prevData;
      Height = thisData - prevData;
    }
    else
    {
      offsetY = thisData;
      Height = prevData - thisData;
    }
    Height++;
    (void)BMPCopyBackDisplay(PosX+index, pGraphState[index].PosY, 1, pGraphState[index].Height);
    (void)R_GAPI_Fill(displayFrameBuf, PosX+index, PosY+offsetY, 1, Height, &ct[1]);
    pGraphState[index].PosY = PosY+offsetY;
    pGraphState[index].Height = Height;
  }
}

/**********************************************************************************
Function Name:   genGraph
Description:  Generate graph data on a periodic basis from a screen task
Arguments:   None
Return value:   None
***********************************************************************************/
static const int8_t heartbeat[]=
{
  0,0,0,2,4,6,9,10,10,10,9,6,4,2,0,0,0,0,-2,-4,-4,-2,0,4,
  8,16,24,32,40,48,50,48,40,32,24,16,8,0,-8,-16,-20,-22,-20,
  -16,-8,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,5,5,4,2,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

static portTickType genGraph(void)
{
  if(0 != GraphState)
  {
    /* Let system know we're accessing External Memory */
    R_DDLCD_ExMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
    {
      static uint8_t hb_index;
      GraphHead = (GraphHead+1) % size_ra(GraphData);
      hb_index = (hb_index+1) % size_ra(heartbeat);
      if(0 == MedSliderState.Touched)
        GraphData[GraphHead] = heartbeat[hb_index];
      else
        GraphData[GraphHead] = (int8_t)(MedSliderState.Value/400)-28;
      {    
        static uint8_t drawTime;
        if(++drawTime >=2 )
        {
          drawTime = 0;
          drawGraph();
        }
      }
    }
    R_DDLCD_ExMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
  }  
  return (port_mS_to_TICK(25));
}    

/**********************************************************************************
Function Name:   MedDataBox
Description:  Keep data boxes updated
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void MedDataBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  int index;
  char Text_Buff[4];
  
  for (index = 0; index < size_ra(MedData); index++)
  {
    if (&MedData[index].Box == pS->ppBmp)
    {
      if(pMsg->event == ME_TIME_UPDATE)
      {
        MedData[index].Value += MedData[index].ValueUpdate;
        if(MedData[index].Value > MedData[index].ValueMax)
          MedData[index].Value = MedData[index].ValueMin;
      }
      
      /* make copy of our message */
      msg = *pMsg;
      /* check if the value display needs updating */
      if((pMsg->event == ME_TIME_UPDATE) || (pMsg->event == ME_USER_UPDATE))
      {
        if(MedData[index].ValuePrev != MedData[index].Value)
        {
          msg.event = ME_DRAW_DISPLAY;
        }
        MedData[index].ValuePrev = MedData[index].Value;
      }
      
      (void)sprintf(Text_Buff,"%3d",MedData[index].Value);
      (void)DataBoxHandler(pS, &msg, MedData[index].Label, (uint8_t *)Text_Buff);
    }
  }
}

/**********************************************************************************
Function Name:   MedModePress
Description:  Mode Button Pressed
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void MedModePress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  uint8_t ModeDraw = 0;
  
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"MODE", state)))
  {
    if (ME_PRESS == pMsg->event)
    {
      if(++MedMode >= MODE_COUNT)
        MedMode = MODE_ALL;
        
      /* send event to update display box */
      msg.source = MS_DISPLAY;
      msg.event = ME_USER_UPDATE;
      msg.param.value = MedMode;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
  }
  
  switch (MedMode)
  {
    case MODE_ALL:
      MedData[BoxHR].ValueUpdate = 1;
      MedData[BoxTemp].ValueUpdate = 1;
      MedData[BoxSP02].ValueUpdate = 1;
      MedData[BoxSYS].ValueUpdate = 5;
      MedData[BoxDIA].ValueUpdate = 5;
      break;
      
    case MODE_HEART:
      MedData[BoxHR].ValueUpdate = 1;
      MedData[BoxTemp].ValueUpdate = 0;
      MedData[BoxSP02].ValueUpdate = 0;
      MedData[BoxSYS].ValueUpdate = 0;
      MedData[BoxDIA].ValueUpdate = 0;
      break;
      
    case MODE_BPM:
      MedData[BoxHR].ValueUpdate = 0;
      MedData[BoxTemp].ValueUpdate = 0;
      MedData[BoxSP02].ValueUpdate = 0;
      MedData[BoxSYS].ValueUpdate = 5;
      MedData[BoxDIA].ValueUpdate = 5;
      break;
      
    case MODE_TEMP_SP02:
      MedData[BoxHR].ValueUpdate = 0;
      MedData[BoxTemp].ValueUpdate = 1;
      MedData[BoxSP02].ValueUpdate = 1;
      MedData[BoxSYS].ValueUpdate = 0;
      MedData[BoxDIA].ValueUpdate = 0;
      break;
  }
  if(0 == MedState)
  {
    MedData[BoxHR].ValueUpdate = 0;
    MedData[BoxTemp].ValueUpdate = 0;
    MedData[BoxSP02].ValueUpdate = 0;
    MedData[BoxSYS].ValueUpdate = 0;
    MedData[BoxDIA].ValueUpdate = 0;
  }
}

/**********************************************************************************
Function Name:   MedStatus
Description:  Display status of mode
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static char const * const ModeText[]={"ALL", "HEART", "BPM", "TEMP/SpO\xE2\x82\x82"};

void MedStatus(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{  
  /* make copy of our message */
  msg = *pMsg;
  if((msg.event == ME_USER_UPDATE) && (msg.param.value == MedMode))
  {
    /* modify message to update display */
    msg.event = ME_DRAW_DISPLAY;
  }
  
  (void)DataBoxHandler(pS, &msg, "", (uint8_t *)ModeText[MedMode]);
}

/**********************************************************************************
Function Name:   MedStartStopPress
Description:  StartStop Button Pressed
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static char const * const StateText[]={"RUN", "STOP"};

void MedStartStopPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{  
  static int16_t state=0;
  
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)StateText[MedState], state)))
    if (ME_PRESS == pMsg->event)
    {
      if(0 == MedState)
        MedState = 1;
      else
        MedState = 0;
    }
}

/**********************************************************************************
Function Name:   MedONOFFPress
Description:  OnOff Button Pressed
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void MedONOFFPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{  
  static int16_t state=0;
  
  if (ME_DRAW == pMsg->event)
  {
    (void)R_GAPI_CopyMulC( FileFind(pResources, "MedHeartbeat"), backFrameBuf, 128, 130, &pS->pScheme[0].ct[1] );
  }
  if (ME_DRAW_DISPLAY == pMsg->event)
  {
    (void)ScreenTaskStart(genGraph);
  }

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"\xE2\x9B\xA2", state)))
    if (ME_PRESS == pMsg->event)
    {
      if(0 == GraphState)
        GraphState = 1;
      else
        GraphState = 0;
    }

}

/**********************************************************************************
Function Name:   MedSlider
Description:  Slider to control target heart rate
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void MedSlider(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  int32_t Value;
  
  /* convert current BoxTarget value to Slider Units */
  Value = (int32_t)(MedData[BoxTarget].Value-MedData[BoxTarget].ValueMin) * 32767L;
  MedSliderState.Value = (int16_t) (Value / (int32_t)(MedData[BoxTarget].ValueMax - MedData[BoxTarget].ValueMin));
  
  if (0 != (MedSliderState.Touched = SliderHandler(pS, pMsg, pBMP_MedSliderButton, &MedSliderState)))
  {
    /* send event to update display box */
    msg.source = MS_DISPLAY;
    msg.event = ME_USER_UPDATE;
    msg.param.value = BoxTarget;
    (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
  }
  
  /* convert Slider Units to BoxTarget value (+16384 for proper rounding) */
  Value = (((int32_t)MedSliderState.Value * (int32_t)(MedData[BoxTarget].ValueMax - MedData[BoxTarget].ValueMin)+16384 )/ 32767L);
  MedData[BoxTarget].Value = (int16_t)Value + MedData[BoxTarget].ValueMin;
}





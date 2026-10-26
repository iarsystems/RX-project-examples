/* Adapted to IAR Embedded Workbench */
//Screen Medical
/***********************************************************************/
/*                                                                     */
/*  FILE        :ScreenMed.c                                           */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :Screen handler showing medical demo                   */        
/*                                                                     */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
12.09.2015 REE - Ported from RX63N GAPI Demo
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"
#include <math.h>
#include "Jpeg/r_expand_jpegd.h"

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
    {&pfontSqBT_14, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_LEFT,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
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
    {&pfontSqBT_18, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_LEFT,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.10,0.40),GAPI_T_NONE},
  },
};

static const GAPI_Scheme_t T_BoxBottomScheme[]= 
{
  /* Label */
  {
    {CT_NONE,CT_BLACK}, 
    {&pfontSqBT_10, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
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
    {&pfontSqBT_10, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.50,0.75),GAPI_T_MULC},
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
    {&pfontSqBT_14, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MUL},
  },

  /* Button off */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_14, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_NONE},
  },
  {
    {{0xA0,0xA0,0xA0},CT_NONE}, 
    {&pfontSqBT_B24, {CT_NONE,CT_BLACK}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_NONE},
  },
};

/*Prototypes */
static void MedModePress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedStatus(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedStartStopPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedONOFFPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedSlider(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedDataBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void MedGraph(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static portTickType genData(void);
static portTickType genEvents(void);

static enum  {MODE_ALL, MODE_HEART, MODE_BPM, MODE_TEMP_SP02, MODE_COUNT} MedMode;
static int MedState;

/*Declarations */
static BMP_t const *pBMP_Mode;
static BMP_t const *pBMP_Status;
static BMP_t const *pBMP_StartStop;
static BMP_t const *pBMP_ONOFF;
static BMP_t const *pBMP_MedSlider;
static BMP_t const *pBMP_MedSliderButton;
static BMP_t const *pBMP_Graph;

typedef struct
{
  BMP_t const *Box;
  float Value;
  float ValuePrev;
  float ValueMin;
  float ValueMax;
  int16_t ValueUpdate;
  uint8_t const *Label;
} MedData_type;
enum {BoxTarget, BoxHR, BoxTemp, BoxSP02, BoxSYS, BoxDIA};
MedData_type MedData[]=
{
  /* Box, Value, ValuePrev, ValueMin, ValueMax, ValueUpdate,  Label,                     */
  {NULL,    75.,        0.,      60.,      99.,           0,  "Set Target \xE2\x99\xA5"}, /* target rate */
  {NULL,    85.,        0.,      75.,      85.,           0,  "HR [bpm] \xE2\x99\xA5"  }, /* heart rate */
  {NULL,    37.,        0.,      34.,      38.,           0,  "TEMP [\xC2\xB0""C]"     }, /* temp value */
  {NULL,    98.,        0.,      85.,      99.,           0,  "SpO\xE2\x82\x82 [%]"    }, /* sp02 value */
  {NULL,    90.,        0.,      90.,      99.,           0,  "SYS [mmHg]"             }, /* sys value */
  {NULL,    60.,        0.,      60.,      80.,           0,  "DIA [mmHg]"             }, /* dia value */
};

static SLIDERSTATE_type MedSliderState;

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
/*  Bit map Address           Color Scheme            Function call        X Pos        Y Pos  */
  { &pBMP_HomeIcon,           T_HomeButtonScheme,      ButtonBack,           278,    207 },
  { &pBMP_ButtonS,            T_CalendarScheme,        TimerRTC_Display,      6,     220 },
  { &MedData[BoxTemp].Box,    T_BoxBottomScheme,       MedDataBox,            6,       6 },
  { &MedData[BoxTarget].Box,  T_BoxTopScheme,          MedDataBox,            6,      76 },
  { &MedData[BoxHR].Box,      T_BoxBottomScheme,       MedDataBox,            6,     146 },
  { &MedData[BoxSP02].Box,    T_BoxBottomScheme,       MedDataBox,            85,      6 },
  { &MedData[BoxSYS].Box,     T_BoxBottomScheme,       MedDataBox,           164,      6 },
  { &MedData[BoxDIA].Box,     T_BoxBottomScheme,       MedDataBox,           243,      6 },
  { &pBMP_Graph,              T_BoxBottomScheme,       MedGraph,             82,     118 },
  { &pBMP_MedSlider,          T_SliderScheme,          MedSlider,            82,      73 },
  { &pBMP_Mode,               T_ButtonScheme,          MedModePress,         181,     121 },
  { &pBMP_StartStop,          T_ButtonScheme,          MedStartStopPress,    225,     121 },
  { &pBMP_ONOFF,              T_ButtonScheme,          MedONOFFPress,        269,     121 },
  { &pBMP_Status,             T_StatusScheme,          MedStatus,            181,     163 },
  { NULL,                     NULL,                    NULL,                   0,       0 },
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
  /* Clear the background */
  {
    uint8_t const *pJPG = FileFind(pResources, "Med_BG");
    int32_t JPGsize = FileSizeFind(pResources, "Med_BG");
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

  /* start screen tasks to simulate data and generate events */
  (void)ScreenTaskStart(genData);
  (void)ScreenTaskStart(genEvents);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

static void Destructor(Screen_t const *pS)
{
  /* Run default behavior */
  BasicDestructor(pS);
}
/*******************************************************************************
End of function Destructor
*******************************************************************************/

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

#pragma location=".SCR_HM"
__root const ScreenObj_t HomeIconMed= { &pBMP_ButtonVital,    T_HomeScheme,    Medical,        100,     80  };

static void Medical(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;

  /* locate the BMP resources */
  if(NULL == pBMP_ButtonVital)
  {
    pBMP_ButtonVital = FileFind(pResources, "Home_Med1");
  }

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Demo Medical", state)))
  {
    if (ME_RELEASE == pMsg->event)
    {
      /* send a screen change command to the handler */
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenMedicalData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
  }
}
/*******************************************************************************
End of function Medical
*******************************************************************************/

/**********************************************************************************
Function Name:   MedGraph
Description:  Display graph data
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static int8_t GraphData[256];
static int16_t GraphHead;
static int16_t GraphState=1;

static void MedGraph(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{  
    /* locate the BMP resources */
    if(NULL == pBMP_Graph)
    {
        pBMP_Graph = FileFind(pResources, "MedHeartbeat");
    }

    if (ME_DRAW == pMsg->event)
    {
        /* place box in background */
        (void)R_GAPI_CopyMulC(pBMP_Graph, backFrameBuf, pS->PosX, pS->PosY, &pS->pScheme[0].ct[1] );
    }

    /* check if we have a request to update the graph */
    if (((0 != GraphState) && (ME_USER_UPDATE == pMsg->event))
            && ((uint32_t) &pBMP_Graph == pMsg->param.value))
    {
        static const BMP_ColorTable_t ct[]={{0x00,0x00,0x00,0x00},{0x80,0x80,0x00,0x00}};
        int index;
        int16_t offsetY;
        int16_t Height;

        /* clear the work area */
        (void)BMPCopyBackWork(pS->PosX, pS->PosY, R_BMP_Width(pBMP_Graph), R_BMP_Height(pBMP_Graph));
        for(index = 2; index < (R_BMP_Width(pBMP_Graph)-2); index++)
        {
            int8_t prevData = GraphData[((GraphHead + index) - 144) % size_ra(GraphData)];
            int8_t thisData = GraphData[((GraphHead + index) - 143) % size_ra(GraphData)];
            offsetY = (prevData < thisData)?prevData:thisData;
            Height = abs(thisData - prevData) + 1;
            (void)R_GAPI_Fill(workFrameBuf, pS->PosX+index, (pS->PosY + offsetY) + 30, 1, Height, &ct[1]);
        }

        /* copy graph to display frame */
        (void)BMPCopyWorkDisplay(pS->PosX, pS->PosY, R_BMP_Width(pBMP_Graph), R_BMP_Height(pBMP_Graph));
    }
}
/*******************************************************************************
End of function MedGraph
*******************************************************************************/

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

  /* locate the BMP resources */
  if(NULL == MedData[BoxHR].Box)
  {
    MedData[BoxHR].Box = MedData[BoxTemp].Box = MedData[BoxDIA].Box = MedData[BoxSP02].Box = MedData[BoxSYS].Box =  FileFind(pResources, "Med_Box_Lo");
    MedData[BoxTarget].Box = FileFind(pResources, "Med_Box_Hi");
  }
  
  /* determine instance we are dealing with */
  for (index = 0; index < size_ra(MedData); index++)
  {
    if ((&MedData[index].Box) == pS->ppBmp)
    {
    	/* This break is intentional */
    	break;
    }
  }
      
  /* make copy of our message */
  msg = *pMsg;

  /* check if the value display needs updating */
  if((pMsg->event == ME_USER_UPDATE) && 
     ((uint32_t) pS->ppBmp == pMsg->param.value))
  {
    if((int)MedData[index].ValuePrev != (int)MedData[index].Value)
    {
      msg.event = ME_DRAW_DISPLAY;
    }
    MedData[index].ValuePrev = MedData[index].Value;
  }
  
  (void)sprintf(Text_Buff,"%3d",(int)MedData[index].Value);
  (void)DataBoxHandler(pS, &msg, MedData[index].Label, (uint8_t *)Text_Buff);
}
/*******************************************************************************
End of function MedDataBox
*******************************************************************************/

/**********************************************************************************
Function Name:   MedModePress
Description:  Mode Button Pressed
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void MedModePress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state = 0;

  /* locate the BMP resources */
  if(NULL == pBMP_Mode)
  {
    pBMP_Mode = FileFind(pResources, "MedButton");
  }

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"MODE", state)))
  {
    if (ME_PRESS == pMsg->event)
    {
      if((++MedMode) >= MODE_COUNT)
      {
        MedMode = MODE_ALL;
      }
        
      /* send event to update display box */
      msg.source = MS_DISPLAY;
      msg.event = ME_USER_UPDATE;
      msg.param.value = (uint32_t)&pBMP_Status;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
  }
  
  switch (MedMode)
  {
    case MODE_ALL:
      MedData[BoxHR].ValueUpdate = 1;
      MedData[BoxTarget].ValueUpdate = 1;
      MedData[BoxTemp].ValueUpdate = 1;
      MedData[BoxSP02].ValueUpdate = 1;
      MedData[BoxSYS].ValueUpdate = 1;
      MedData[BoxDIA].ValueUpdate = 1;
      break;
      
    case MODE_HEART:
      MedData[BoxHR].ValueUpdate = 1;
      MedData[BoxTarget].ValueUpdate = 1;
      MedData[BoxTemp].ValueUpdate = 0;
      MedData[BoxSP02].ValueUpdate = 0;
      MedData[BoxSYS].ValueUpdate = 0;
      MedData[BoxDIA].ValueUpdate = 0;
      break;
      
    case MODE_BPM:
      MedData[BoxHR].ValueUpdate = 0;
      MedData[BoxTarget].ValueUpdate = 0;
      MedData[BoxTemp].ValueUpdate = 0;
      MedData[BoxSP02].ValueUpdate = 0;
      MedData[BoxSYS].ValueUpdate = 1;
      MedData[BoxDIA].ValueUpdate = 1;
      break;
      
    case MODE_TEMP_SP02:
      MedData[BoxHR].ValueUpdate = 0;
      MedData[BoxTarget].ValueUpdate = 0;
      MedData[BoxTemp].ValueUpdate = 1;
      MedData[BoxSP02].ValueUpdate = 1;
      MedData[BoxSYS].ValueUpdate = 0;
      MedData[BoxDIA].ValueUpdate = 0;
      break;
    default:
      break;
  }
  if(0 == MedState)
  {
    MedData[BoxHR].ValueUpdate = 0;
    MedData[BoxTarget].ValueUpdate = 0;
    MedData[BoxTemp].ValueUpdate = 0;
    MedData[BoxSP02].ValueUpdate = 0;
    MedData[BoxSYS].ValueUpdate = 0;
    MedData[BoxDIA].ValueUpdate = 0;
  }
}
/*******************************************************************************
End of function MedModePress
*******************************************************************************/

/**********************************************************************************
Function Name:   MedStatus
Description:  Display status of mode
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static char const * const ModeText[]={"ALL", "HEART", "BPM", "TEMP/SpO\xE2\x82\x82"};

static void MedStatus(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{  
  /* locate the BMP resources */
  if (NULL == pBMP_Status)
  {
    pBMP_Status = FileFind(pResources, "MedStatusBox");
  }

  /* make copy of our message */
  msg = *pMsg;
  if ((msg.event == ME_USER_UPDATE)
			&& ((uint32_t) pS->ppBmp == msg.param.value))
  {
    /* modify message to update display */
    msg.event = ME_DRAW_DISPLAY;
  }
  
  (void)DataBoxHandler(pS, &msg, "", (uint8_t *)ModeText[MedMode]);
}
/*******************************************************************************
End of function MedStatus
*******************************************************************************/

/**********************************************************************************
Function Name:   MedStartStopPress
Description:  StartStop Button Pressed
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static char const * const StateText[]={"RUN", "STOP"};

static void MedStartStopPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{  
  static int16_t state=0;

  /* locate the BMP resources */
  if (NULL == pBMP_StartStop)
  {
    pBMP_StartStop= FileFind(pResources, "MedButton");
  }

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)StateText[MedState], state)))
  {
    if (ME_PRESS == pMsg->event)
    {
      MedState = (0 == MedState)? 1: 0;
    }
  }
}
/*******************************************************************************
End of function MedStartStopPress
*******************************************************************************/

/**********************************************************************************
Function Name:   MedONOFFPress
Description:  OnOff Button Pressed
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void MedONOFFPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{  
  static int16_t state=0;
  
  /* locate the BMP resources */
  if (NULL == pBMP_ONOFF)
  {
    pBMP_ONOFF = FileFind(pResources, "MedButtonPower");
  }

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"", state)))
  {
    if (ME_PRESS == pMsg->event)
    {
      GraphState = (0 == GraphState)? 1: 0;
    }
  }
}
/*******************************************************************************
End of function MedONOFFPress
*******************************************************************************/

/**********************************************************************************
Function Name:   MedSlider
Description:  Slider to control target heart rate
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void MedSlider(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  /* locate the BMP resources */
  if(NULL == pBMP_MedSlider)
  {
    pBMP_MedSlider = FileFind(pResources, "MedSlider");
    pBMP_MedSliderButton = FileFind(pResources, "MedButtonSlider");
  }

  /* convert current BoxTarget value to Slider Units */
  if((pMsg->event == ME_USER_UPDATE) &&
		        ((uint32_t) &MedData[BoxTarget].Box == pMsg->param.value))
  {
    MedSliderState.Value = ((MedData[BoxTarget].Value - MedData[BoxTarget].ValueMin) /
    		          (MedData[BoxTarget].ValueMax - MedData[BoxTarget].ValueMin))* 32767.;
  }
  
  if (0 != (MedSliderState.Touched = SliderHandler(pS, pMsg, pBMP_MedSliderButton, &MedSliderState)))
  {
    /* convert Slider Units to BoxTarget value (+16384 for proper rounding) */
    MedData[BoxTarget].Value = (((float)MedSliderState.Value/ 32767.) *
    		 (MedData[BoxTarget].ValueMax - MedData[BoxTarget].ValueMin)) +
			                                     MedData[BoxTarget].ValueMin;
  }
}
/*******************************************************************************
End of function MedSlider
*******************************************************************************/

/**********************************************************************************
Function Name:   genEvents
Description:  Generate events to update objects on periodic basis from a screen task
Arguments:   None
Return value:   None
***********************************************************************************/
static const uint32_t EventValues[]=
{
  /* graph is updated every 20mS, each box is checked every 120mS */
  (uint32_t)&pBMP_Graph,            /*  0mS */
  (uint32_t)&MedData[BoxHR].Box,    /* 10mS */
  (uint32_t)&pBMP_Graph,            /* 20mS */
  (uint32_t)&MedData[BoxTarget].Box,/* 30mS */
  (uint32_t)&pBMP_Graph,            /* 40mS */
  (uint32_t)&MedData[BoxTemp].Box,  /* 50mS */
  (uint32_t)&pBMP_Graph,            /* 60mS */
  (uint32_t)&MedData[BoxSP02].Box,  /* 70mS */
  (uint32_t)&pBMP_Graph,            /* 80mS */
  (uint32_t)&MedData[BoxSYS].Box,   /* 90mS */
  (uint32_t)&pBMP_Graph,            /*100mS */
  (uint32_t)&MedData[BoxDIA].Box,   /*110mS */
};

static portTickType genEvents(void)
{
  static uint8_t object;

  /* send event to update object */
  EVENT_MSG msg_user;

  msg_user.source = MS_DISPLAY;
  msg_user.event = ME_USER_UPDATE;
  msg_user.param.value = EventValues[object];
  (void)xQueueSend(EventQueue, (void *)&msg_user, 0UL); /* RTOS_USAGE */
  if((++object) > size_ra(EventValues))
  {
    object = 0;
  }
  
  return (port_mS_to_TICK(10));
}
/*******************************************************************************
End of function genEvents
*******************************************************************************/

/**********************************************************************************
Function Name:   genData
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

static portTickType genData(void)
{
  static uint8_t hb_index;
  GraphHead = (GraphHead+1) % size_ra(GraphData);
  hb_index = (hb_index+1) % size_ra(heartbeat);

  if(0 == MedSliderState.Touched)
  {
    GraphData[GraphHead] = heartbeat[hb_index];
  }
  else
  {
    GraphData[GraphHead] = (int8_t)(MedSliderState.Value/400)-28;
  }

  {
    int index;
    static float MedInc[size_ra(MedData)] = { 0.20, 0.04, 0.04, 0.06, 0.03, 0.05 };
    
    for (index=0; index < size_ra(MedData); index++)
    {
	  if (0 != MedData[index].ValueUpdate)
      {
        MedData[index].Value += MedInc[index];
        if (MedData[index].Value > MedData[index].ValueMax)
        {
          MedInc[index] = -fabsf(MedInc[index]);
          MedData[index].Value = MedData[index].ValueMax;
        }
        if (MedData[index].Value < MedData[index].ValueMin)
        {
          MedInc[index] = fabsf(MedInc[index]);
          MedData[index].Value = MedData[index].ValueMin;
        }
      }
    }
  }
  return (port_mS_to_TICK(15/*25*/));
}
/*******************************************************************************
End of function genData
*******************************************************************************/

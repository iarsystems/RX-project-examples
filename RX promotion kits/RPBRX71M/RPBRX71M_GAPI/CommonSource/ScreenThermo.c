/* Adapted to IAR Embedded Workbench */
/***********************************************************************/
/*                                                                     */
/*  FILE        :ScreenThermo.c                                        */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :Screen handler showing thermostat demo                */        
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
#include "Jpeg/r_expand_jpegd.h"

#define SCHEME T_SchemeBlue
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);
char Thermo_Buff[20];

typedef struct
{
  int8_t high_temp;
  int8_t low_temp;
  BMP_t const *icon;
  uint8_t day[4];
} Forecast_type;
Forecast_type forecast[7]={{12,32,NULL,"SUN"},{17,28,NULL,"MON"},{16,29,NULL,"TUE"},{12,25,NULL,"WED"},{13,25,NULL,"THU"},{16,28,NULL,"FRI"},{15,30,NULL,"SAT"}};

enum{FG_SUNNY, FG_CLOUDY, FG_RAINY};
BMP_t const *weather_fg[3][3];

struct{
  int8_t indoor_temp;
  int8_t outdoor_temp;
  int8_t humidity;
  int8_t icon;
  uint8_t const *pBgnd;
  int32_t Bgnd_size;
  char forecast[20];
}weather;
  
/*Prototypes */
static void TempPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void ThermoBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void DayPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void WeatherUpdate(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

/*Declarations */
static BMP_t const *pBMP_Sun;
static BMP_t const *pBMP_Mon;
static BMP_t const *pBMP_Tue;
static BMP_t const *pBMP_Wed;
static BMP_t const *pBMP_Thu;
static BMP_t const *pBMP_Fri;
static BMP_t const *pBMP_Sat;
static BMP_t const *pBMP_Plus;
static BMP_t const *pBMP_Minus;
static BMP_t const *pBMP_TempBox;

const GAPI_PUT_t fontSqBT14   = { &pfontSqBT_14, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}};

static const GAPI_Scheme_t T_HomeButtonScheme[]=
{
  /* Button on */
  {
    {CT_NONE,CT_WHITE}, 
    {&pfontSqBT_16, {CT_BLACK,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_MULC},
  },

  /* Button off */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_16, {CT_BLACK,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_NONE},
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
    {&pfontSqBT_B24, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_RIGHT,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT(0.90,0.35),GAPI_T_NONE},
  },
};

static const GAPI_Scheme_t T_ThermoBigScheme[]= 
{
  /* Text Only */
  {
    {CT_NONE,CT_NONE}, 
    { &pfontSqBT_40, {CT_NONE,CT_WHITE,}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_LEFT,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_NONE},
  },
};

static const GAPI_Scheme_t T_ThermoSmallScheme[]= 
{
  /* Button off */
  {
    {CT_NONE,CT_NONE},
    { &pfontSqBT_10, {CT_NONE,CT_WHITE,}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,1,GAPI_T_MUL}
  },

  /* Button on */
  {
    {CT_NONE,CT_NONE},
    { &pfontSqBT_10, {CT_NONE,CT_BLACK,}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,1,GAPI_T_NONE}
  }
};

static const GAPI_Scheme_t T_ThermoMidScheme[]=
{
  /* Button off */
  {
    {CT_NONE,CT_NONE},
    { &pfontSqBT_13, {CT_NONE,CT_WHITE,}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,1,GAPI_T_MUL}
  },

  /* Button on */
  {
    {CT_NONE,CT_NONE}, 
    { &pfontSqBT_13, {CT_NONE,CT_BLACK,}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_BOTTOM,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,1,GAPI_T_NONE}
  }
};



static const GAPI_Scheme_t T_ThermoPlusMinusScheme[]= 
{
  /* Button off */
  {
    {CT_NONE,CT_BLACK},
    { &pfontSqBT_B42, {CT_NONE,CT_WHITE,}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  },

  /* Button on */
  {
    {CT_NONE,CT_NONE}, 
    { &pfontSqBT_B42, {CT_NONE,CT_BLACK,}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_NONE}
  }
};

static const GAPI_Scheme_t T_ThermoCalendarScheme[]= 
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
/*  Bit map Address      Color Scheme             Function call    X Pos    Y Pos     */
  { &pBMP_ButtonS,       T_ThermoCalendarScheme,  WeatherUpdate,      9,     240 },   /* this callback must be first as it paints background */
  { &pBMP_HomeIcon,      T_HomeButtonScheme,      ButtonBack,        278,     207 },
  { &pBMP_ButtonS,       T_ThermoCalendarScheme,  TimerRTC_Display,   6,     220 },
  { &pBMP_Sun,           T_ThermoSmallScheme,     DayPress,           4,       8 },
  { &pBMP_Mon,           T_ThermoSmallScheme,     DayPress,           49,       8 },
  { &pBMP_Tue,           T_ThermoSmallScheme,     DayPress,           94,       8 },
  { &pBMP_Wed,           T_ThermoSmallScheme,     DayPress,          139,       8 },
  { &pBMP_Thu,           T_ThermoSmallScheme,     DayPress,          184,       8 },
  { &pBMP_Fri,           T_ThermoSmallScheme,     DayPress,          229,       8 },
  { &pBMP_Sat,           T_ThermoSmallScheme,     DayPress,          274,       8 },
  { &pBMP_TempBox,       T_BoxTopScheme,          ThermoBox,         221,     130 },
  { &pBMP_Plus,          T_ThermoPlusMinusScheme, TempPress,         272,      73 },
  { &pBMP_Minus,         T_ThermoPlusMinusScheme, TempPress,         224,      73 },

  { NULL,                NULL,                    NULL,                0,       0 },
};

Screen_t ScreenThermoData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;

/*******************************************************************************
* Function Name: Constructor
* Description  : Constructor.
* Arguments    : ps
* Return Value : None
*******************************************************************************/
static void Constructor(Screen_t const *pS)
{
  if(NULL == pBMP_Sun)
  {
    /* initialize button pointers */
    pBMP_Sun = pBMP_Mon = pBMP_Tue = pBMP_Wed = pBMP_Thu = pBMP_Fri = pBMP_Sat = FileFind(pResources, "ThermoButton");
    pBMP_Minus = pBMP_Plus = FileFind(pResources, "ThermoButton");
    pBMP_TempBox = FileFind(pResources, "ThermoBox");
    
    
    /*initailize text and variables */
    weather.indoor_temp= 19;    //initial thermostat temp
    weather.outdoor_temp= 28;
    weather.humidity= 50;
    weather.pBgnd=FileFind(pResources, "ThermoCloudy_bg");
    weather.Bgnd_size=FileSizeFind(pResources, "ThermoCloudy_bg");
    weather.icon=FG_CLOUDY;
    (void)sprintf(weather.forecast,"Cloudy");
    
    weather_fg[FG_SUNNY][0] = FileFind(pResources, "Thermo_Sunny1");
    weather_fg[FG_SUNNY][1] = FileFind(pResources, "Thermo_Sunny2");
    weather_fg[FG_SUNNY][2] = FileFind(pResources, "Thermo_Sunny3");
    weather_fg[FG_CLOUDY][0] = FileFind(pResources, "Thermo_Cloudy1");
    weather_fg[FG_CLOUDY][1] = FileFind(pResources, "Thermo_Cloudy2");
    weather_fg[FG_CLOUDY][2] = FileFind(pResources, "Thermo_Cloudy3");
    weather_fg[FG_RAINY][0] = FileFind(pResources, "Thermo_Rainy1");
    weather_fg[FG_RAINY][1] = FileFind(pResources, "Thermo_Rainy2");
    weather_fg[FG_RAINY][2] = FileFind(pResources, "Thermo_Rainy3");
    forecast[0].icon =FileFind(pResources, "Thermo_miniSun");
    forecast[1].icon =FileFind(pResources, "Thermo_miniCloudy");
    forecast[2].icon =FileFind(pResources, "Thermo_miniCloudy");
    forecast[3].icon =FileFind(pResources, "Thermo_miniRainy");
    forecast[4].icon =FileFind(pResources, "Thermo_miniRainy");
    forecast[5].icon =FileFind(pResources, "Thermo_miniCloudy");
    forecast[6].icon =FileFind(pResources, "Thermo_miniSun");

  }
  
  /* Run default behavior */
  BasicConstructor(pS);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

/*******************************************************************************
* Function Name: Destructor
* Description  : Does an example task. Making this longer just to see how it
*                wraps.
* Arguments    : index -
*                    Where to start looking
*                p_output -
*                    Pointer of where to put the output data
* Return Value : count -
*                    How many entries were found
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
Function Name:   Thermo
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
- By doing this, screens can be added (or removed) simply by linking them into the project */
static void Thermo(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static BMP_t const *pBMP_ButtonThermo;

#pragma location=".SCR_HM"
__root const ScreenObj_t HomeIconThermo= { &pBMP_ButtonThermo,    T_HomeScheme,    Thermo,        28,     80 };

static void Thermo(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(NULL == pBMP_ButtonThermo)
  {
    pBMP_ButtonThermo = FileFind(pResources, "Home_Thermo1");
  }
  
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"Demo Thermo", state)))
  {
    if (ME_RELEASE == pMsg->event)
    {
      /* send a screen change command to the handler */
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenThermoData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
  }
}
/*******************************************************************************
End of function Thermo
*******************************************************************************/

/**********************************************************************************
Function Name:   WeatherAnimate
Description:  Screen task to index the weather icons
Arguments:   None
Return value:   TickType: Amount of time to sleep until next wakeup.
***********************************************************************************/
static const BMP_ColorTable_t local_ctTest = {0xE0, 0xE0, 0xFF, 0x00};
static uint8_t weather_index = 0;
#define WEATHER_ICON_X 101
#define WEATHER_ICON_Y 104

static portTickType WeatherAnimate(void)
{
  /* Let system know we're accessing Display Memory */
  R_DDLCD_DispMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
  {
    if ((++weather_index) >= size_ra(weather_fg[weather.icon]))
    {
      weather_index = 0;
    }

    {
      int16_t Height = R_BMP_Height(weather_fg[weather.icon][weather_index]);
      int16_t Width = R_BMP_Width(weather_fg[weather.icon][weather_index]);
      BMPCopyBackWork(WEATHER_ICON_X,WEATHER_ICON_Y,Width,Height);
      (void)R_GAPI_CopyMulC(weather_fg[weather.icon][weather_index], workFrameBuf,WEATHER_ICON_X, WEATHER_ICON_Y, &local_ctTest); 
      BMPCopyWorkDisplay(WEATHER_ICON_X,WEATHER_ICON_Y,Width,Height);
    }    
  }
  R_DDLCD_DispMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
  
  return (port_mS_to_TICK(500));
}
/*******************************************************************************
End of function WeatherAnimate
*******************************************************************************/

/**********************************************************************************
Function Name:  WeatherUpdate
Description  :  Keep the weather up to date on the screen
Arguments    :  *pS -
                *pMsg
Return value :  None.
***********************************************************************************/
static void WeatherUpdate(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  if (ME_DRAW == pMsg->event)
  {
    /* Clear the background */
    {
      uint8_t const *pJPG = weather.pBgnd;
      int32_t JPGsize = weather.Bgnd_size;
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
  }

  if (ME_DRAW_WORK == pMsg->event)
  {
   (void)R_GAPI_CopyMulC(weather_fg[weather.icon][weather_index], workFrameBuf,WEATHER_ICON_X, WEATHER_ICON_Y, &local_ctTest); 

   /*place weather text */
   (void)sprintf(Thermo_Buff,"%d\xC2\xB0""C",weather.outdoor_temp);                    //Outside temp to screen
   (void)R_GAPI_PutS((uint8_t *)Thermo_Buff, &T_ThermoBigScheme->font, workFrameBuf, 4, 142);  //Outside temp to screen

   (void)R_GAPI_PutS((uint8_t *)weather.forecast, &T_ThermoMidScheme->font, workFrameBuf, 49, 124);  //Forecast to screen

   (void)sprintf(Thermo_Buff,"%d%% Humidity",weather.humidity);                //Outside humidity to screen
   (void)R_GAPI_PutS((uint8_t *)Thermo_Buff, &T_ThermoMidScheme->font, workFrameBuf, 49, 110);  //Outside humidity to screen
  } 
  
  if(ME_DRAW_DISPLAY == pMsg->event)
  {
    (void)ScreenTaskStart(WeatherAnimate);
  }
}
/*******************************************************************************
End of function WeatherUpdate
*******************************************************************************/

/**********************************************************************************
Function Name:   DayButtonDraw
Description:  Draw button with day and forecast shown.
Arguments:   s: pointer to source button BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              pScheme: pointer to information about how to present the image
              Forecast: Data structure with day/forcast information
Return value:   0:OK, Not 0: NG
***********************************************************************************/
GAPI_Resp_t DayButtonDraw(BMP_t const *const s, BMP_t *d, int16_t PosX, int16_t PosY, GAPI_Scheme_t const *pScheme, Forecast_type const * info)
{
  static const BMP_ColorTable_t local_ct1 = {0xAD, 0xAD, 0xAE, 0x00};
  GAPI_Resp_t status;
  int16_t Width = R_BMP_Width(s);
  int16_t Height = R_BMP_Height(s);
  int16_t iWidth = R_BMP_Width(info->icon);
  int16_t iHeight = R_BMP_Height(info->icon);
  int16_t BMP_x_center=(Width/2)+PosX;
  int16_t BMP_y_center=(Height/2)+PosY;
  
  status=R_GAPI_ColorCopy(s, d, PosX, PosY, pScheme);
  
  if(0 != status)
  {
    return(status);
  }
    
  (void)R_GAPI_PutS(info->day, &pScheme->font, d,BMP_x_center, ((PosY + Height) - R_GAPI_FontHeight(&pScheme->font))- 4);  //Print Days to each Icon
    
  (void)sprintf(Thermo_Buff,"%d\xC2\xB0/%d\xC2\xB0",info->high_temp, info->low_temp);
  
  (void)R_GAPI_PutS((uint8_t *)Thermo_Buff, &pScheme->font, d,BMP_x_center, PosY);  //Print Hi/Lo to each icon
    
  status = R_GAPI_CopyMulC( info->icon, d, BMP_x_center-(iWidth/2), BMP_y_center-(iHeight/2), &local_ct1 );

  return(status);
}
/*******************************************************************************
End of function DayButtonDraw
*******************************************************************************/

/**********************************************************************************
Function Name:   DayButtonHandler
Description:  Handles day button behavior...checks if in bounds and displays based on message
Arguments:   pS: pointer to icon information
              pMsg: pointer to current message information
              state: State of image 0: off, 1: on
              Day: Day of week (0=sun, 6=sat)
Return value:   0:Not in bounds, Not 0: in bounds of button
***********************************************************************************/
int16_t DayButtonHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, int16_t State, uint8_t Day )
{
  int16_t Width = R_BMP_Width(*pS->ppBmp);
  int16_t Height = R_BMP_Height(*pS->ppBmp);
  int16_t Touched = 0;

  switch(pMsg->event)
  {
    case ME_DRAW_WORK:
    {
      (void)DayButtonDraw(*pS->ppBmp, workFrameBuf, pS->PosX, pS->PosY, &pS->pScheme[0], &forecast[Day]);
      break;
    }
    
    case ME_MOVE:
    case ME_PRESS:
    case ME_RELEASE:
    {
      int16_t DrawState = -1;  // Default is don't draw

      /* Check if touch is in button bounds*/
      if (((pS->PosX <= pMsg->param.coord[0]) && (pMsg->param.coord[0] < (pS->PosX + Width))) &&
          ((pS->PosY <= pMsg->param.coord[1]) && (pMsg->param.coord[1] < (pS->PosY + Height))))
      {
        Touched = 1;
      }

      /* Make sure we're displaying proper behavior */
      if ((0 == Touched) && (1 == State))
      {
        DrawState = 0;
      }
      else if ((1 == Touched) && (0 == State))
      {
        DrawState = 1;
      }
      else
      {
    	  /* Do nothing */
      }

      if(Touched)
      {
        if (ME_RELEASE == pMsg->event)
        {
          DrawState = 0;
        }

        if (ME_PRESS == pMsg->event)
        {
          DrawState = 1;
        }
      }

      if((-1) != DrawState)
      {  
        (void)BMPCopyBackWork(pS->PosX, pS->PosY, Width, Height);
  
        if(0 == DrawState)
        {
          (void)DayButtonDraw(*pS->ppBmp, workFrameBuf, pS->PosX, pS->PosY, &pS->pScheme[0], &forecast[Day]);
        }
        else if(1 == DrawState)
        {
          (void)DayButtonDraw(*pS->ppBmp, workFrameBuf, pS->PosX, pS->PosY, &pS->pScheme[1], &forecast[Day]);
        }
        else
        {
        	/* Do nothing */
        }
    
        (void)BMPCopyWorkDisplay(pS->PosX, pS->PosY, Width, Height);
      }
      break;
    }
    default:
      return(State);  /* if this isn't a message id we process...don't change state */
  } /* switch */
  return(Touched);
}
/*******************************************************************************
End of function DayButtonHandler
*******************************************************************************/

/**********************************************************************************
Function Name:   DayPress
Description:  Day Button Pressed
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void DayPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{  
  static int16_t state[7];
  uint8_t Day = 5;

  if ((&pBMP_Sun) == pS->ppBmp) {
		Day = 0;
	}
  else if ((&pBMP_Mon) == pS->ppBmp) {
		Day = 1;
	}
  else if ((&pBMP_Tue) == pS->ppBmp)
		{
		Day = 2;
	}
  else if ((&pBMP_Wed) == pS->ppBmp) {
		Day = 3;
	}
  else if ((&pBMP_Thu) == pS->ppBmp) {
		Day = 4;
	}
  else if ((&pBMP_Fri) == pS->ppBmp) {
		Day = 5;
	}
  else if ((&pBMP_Sat) == pS->ppBmp) {
		Day = 6;
	}
  else {
	  /* Do nothing */
	}

  if(0 != (state[Day] = DayButtonHandler(pS, pMsg, state[Day], Day)))
  {
    if (ME_PRESS == pMsg->event)
    {
      switch (Day)
      {
        case 0:
        {
          weather.pBgnd=FileFind(pResources, "ThermoSunny_bg");
          weather.Bgnd_size=FileSizeFind(pResources, "ThermoSunny_bg");
          weather.icon=FG_SUNNY;
          weather.outdoor_temp= 30;
          weather.humidity= 82;
          (void)sprintf(weather.forecast,"Sunny");
          break;
        }  
        case 1:
        {
          weather.pBgnd=FileFind(pResources, "ThermoCloudy_bg");
          weather.Bgnd_size=FileSizeFind(pResources, "ThermoCloudy_bg");
          weather.icon=FG_CLOUDY;
          weather.outdoor_temp= 24;
          weather.humidity= 30;
          (void)sprintf(weather.forecast,"Partly Cloudy");
          break;
        }  
        case 2:
        {
          weather.pBgnd=FileFind(pResources, "ThermoCloudy_bg");
          weather.Bgnd_size=FileSizeFind(pResources, "ThermoCloudy_bg");
          weather.icon=FG_CLOUDY;
          weather.outdoor_temp= 25;
          weather.humidity= 30;
          (void)sprintf(weather.forecast,"Partly Cloudy");
          break;
        }  
        case 3:
        {
          weather.pBgnd=FileFind(pResources, "ThermoRain_bg");
          weather.Bgnd_size=FileSizeFind(pResources, "ThermoRain_bg");
          weather.icon=FG_RAINY;
          weather.outdoor_temp= 22;
          weather.humidity= 30;
          (void)sprintf(weather.forecast,"Rain Showers");
          break;
        }  
        case 4:
        {
          weather.pBgnd=FileFind(pResources, "ThermoRain_bg");
          weather.Bgnd_size=FileSizeFind(pResources, "ThermoRain_bg");
          weather.icon=FG_RAINY;
          weather.outdoor_temp= 23;
          weather.humidity= 30;
          (void)sprintf(weather.forecast,"Rain Showers");
          break;
        }  
        case 5:
        {
          weather.pBgnd=FileFind(pResources, "ThermoCloudy_bg");
          weather.Bgnd_size=FileSizeFind(pResources, "ThermoCloudy_bg");
          weather.icon=FG_CLOUDY;
          weather.outdoor_temp= 25;
          weather.humidity= 30;
          (void)sprintf(weather.forecast,"Partly Cloudy");
          break;
        }  
        case 6:
        {
          weather.pBgnd=FileFind(pResources, "ThermoSunny_bg");
          weather.Bgnd_size=FileSizeFind(pResources, "ThermoSunny_bg");
          weather.icon=FG_SUNNY;
          weather.outdoor_temp= 28;
          weather.humidity= 82;
          (void)sprintf(weather.forecast,"Sunny");
          break;
        }  
        default:
        {
        	break;
        }
      }

      {
        msg.source = MS_DISPLAY;
        msg.event = ME_SCREEN_CHANGE;
        msg.param.pS = &ScreenThermoData;
        (void)xQueueSend(EventQueue, (void *)&msg, 0UL);
      }    
    }
  }
}
/*******************************************************************************
End of function DayPress
*******************************************************************************/

/**********************************************************************************
Function Name:   ThermoBox
Description:  Thermostat display box
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void ThermoBox(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  /* make copy of our message */
  msg = *pMsg;

  /* check if the value display needs updating */
  if(pMsg->event == ME_USER_UPDATE)
  {
    msg.event = ME_DRAW_DISPLAY;
  }
  
  /* create temperature string */
  (void)sprintf(Thermo_Buff,"%d\xC2\xB0""C",weather.indoor_temp);
  (void)DataBoxHandler(pS, &msg, "THERMOSTAT", (uint8_t *)Thermo_Buff);
}
/*******************************************************************************
End of function ThermoBox
*******************************************************************************/

/**********************************************************************************
Function Name:   TempPress
Description:  + or - temp Button Pressed
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void TempPress(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{    
  static int16_t state[2];
  uint8_t up;
  char *pLabel;
  uint8_t display_update = 0;
  
  if ((&pBMP_Plus) == pS->ppBmp)
  {
    pLabel = "+";
    up = 1;
  }
  else
  {
    pLabel = "-";
    up = 0;
  }
  
  state[up] = ButtonHandler(pS, pMsg, (uint8_t *)pLabel, state[up]);
  
  switch(pMsg->event)
  {
    case ME_DRAW:
    {
	  if (1 == up)
      {
        /* only do this drawing for one button instance (up==1)...locations are relative to "up" button */
        static const BMP_ColorTable_t trans_pct = {0xF0, 0xF0, 0xF0, 0x00};    /* make box 94% transparent */
        
        (void)R_GAPI_FillMulK(backFrameBuf, pS->PosX-10, pS->PosY-8, 160, 148, &trans_pct);
      }
      break;
    }

    case ME_DRAW_WORK:
      display_update = 1;
      break;
      
    case ME_PRESS:
    //case ME_MOVE:
      if(0 != state[up])
      {
		if ((1 == up) && (weather.indoor_temp <= 30))
        {
          weather.indoor_temp++;
        }

		if ((0 == up) && (weather.indoor_temp >= 3))
        {
          weather.indoor_temp--;
        }

        display_update = 1;
      }
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
    msg.param.value = weather.indoor_temp;
    (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
  }
}
/*******************************************************************************
End of function TempPress
*******************************************************************************/


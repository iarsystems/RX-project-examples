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
/*  FILE        :ScreenSlider.c                                        */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :Screen Handler for showing slider operation           */        
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

#define CT_BLUE_xC0 {0xC0, 0x00, 0x00, 0x00}
#define CT_GREEN_xC0 {0x00, 0xC0, 0x00, 0x00}
#define CT_RED_xC0 {0x00, 0x00, 0xC0, 0x00}

const GAPI_Scheme_t T_SchemeSliderBlue[]= 
{
  /* Slider */
  {
    {CT_BLACK,CT_BLUE_xC0}, 
    {&pfontSqBT_10, {CT_NONE,CT_GRAY_50}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  },

  /* Button */
  {
    {CT_BLACK,CT_BLUE}, 
    {&pfontSqBT_10, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  }
};

const GAPI_Scheme_t T_SchemeSliderGreen[]= 
{
  /* Slider */
  {
    {CT_BLACK,CT_GREEN_xC0}, 
    {&pfontSqBT_10, {CT_NONE,CT_GRAY_50}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  },

  /* Button */
  {
    {CT_BLACK,CT_GREEN}, 
    {&pfontSqBT_10, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  }
};

const GAPI_Scheme_t T_SchemeSliderRed[]= 
{
  /* Slider */
  {
    {CT_BLACK,CT_RED_xC0}, 
    {&pfontSqBT_10, {CT_NONE,CT_GRAY_50}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  },

  /* Button */
  {
    {CT_BLACK,CT_RED}, 
    {&pfontSqBT_10, {CT_NONE,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}
  }
};

#define SCHEME T_SchemeBlue

/* local task to control rendering of test images */
static void render_task(void *pvParameters);        // The task of Event Manager
xTaskHandle render_taskHnd;                         /* RTOS_USAGE */

static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

static void SliderRed(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void SliderGreen(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void SliderBlue(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void ImgSelect(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

static BMP_t const *pTestImage[4];
static uint8_t slider_touched = 0;
#define ICON_HEIGHT 29
#define ICON_WIDTH  33
#define POSY        134

enum{SLIDER_RED, SLIDER_GREEN, SLIDER_BLUE};
static SLIDERSTATE_type State[3];

/* MUST INCLUDE NULL TERMINATED RECORD */
static const ScreenObj_t Icons[]=
{
  { &pTestImage[0],  T_Scheme50_100,    ImgSelect,     0,  POSY + 2*ICON_HEIGHT + 16 },
  { &pTestImage[1],  T_Scheme50_100,    ImgSelect,     1,  POSY + 2*ICON_HEIGHT + 16 },
  { &pTestImage[2],  T_Scheme50_100,    ImgSelect,     2,  POSY + 2*ICON_HEIGHT + 16 },
  { &pTestImage[3],  T_Scheme50_100,    ImgSelect,     3,  POSY + 2*ICON_HEIGHT + 16 },
  { &pBMP_Slider,  T_SchemeSliderRed,    SliderRed,      24,  85 },
  { &pBMP_Slider,  T_SchemeSliderGreen,  SliderGreen,    24,  45 },
  { &pBMP_Slider,  T_SchemeSliderBlue,   SliderBlue,     24,   5 },
  { &pBMP_HomeIcon,T_HomeScheme,   ButtonBack,     278, 207 },
  {  NULL,         NULL,           NULL,           0,   0 },
};

static const Screen_t ScreenSliderData=
{
  Constructor, Destructor, Icons
};

/* Shared event message */
static EVENT_MSG msg;

static void Constructor(Screen_t const *pS)
{
  static const BMP_ColorTable_t bg_color=CT_GRAY_50;

  /* Clear the background */
  (void)R_GAPI_Copy(FileFind(pResources, "TestImageRLE"), backFrameBuf, 0, 0);
  (void)R_GAPI_Fill(backFrameBuf, 0, 0, DD_FRAME_WIDTH, 120,  &bg_color);

  pTestImage[0] = (BMP_t *)FileFind(pResources, "Gradient0");
  pTestImage[1] = (BMP_t *)FileFind(pResources, "Home");
  pTestImage[2] = (BMP_t *)FileFind(pResources, "Gradient2");
  pTestImage[3] = (BMP_t *)FileFind(pResources, "Gradient3");

  /* Run default behavior */
  BasicConstructor(pS);

  /* Create task for rendering of test images */
  (void)xTaskCreate(render_task, "r:render", configMINIMAL_STACK_SIZE+256, NULL, mainDEMOTASKS_PRIORITY+1, &render_taskHnd);        /* RTOS_USAGE */
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

static void Destructor(Screen_t const *pS)
{
    /* stop render task */
    vTaskDelete(render_taskHnd);

    /* Run default behavior */
    BasicDestructor(pS);
}
/*******************************************************************************
End of function Destructor
*******************************************************************************/

/**********************************************************************************
Function Name:   ScreenSlider
Description:  Process activation button from calling screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
/* Include this in the Home Screen...the _SCR_HM section will be used as the Icon list for the home screen
     - By doing this, screens can be added (or removed) simply by linking them into the project */
static void ScreenSlider(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#pragma location=".SCR_HM_CLASSIC"
__root const ScreenObj_t HomeIconSlider= { &pBMP_ButtonR, T_SchemeRed,    ScreenSlider, 5, 84};

static void ScreenSlider(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"GAPI_T Use",state)))
    if (ME_RELEASE == pMsg->event)
    {
      /* send a screen change command to the handler */
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenSliderData;
      (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
    }
}
/*******************************************************************************
End of function ScreenSlider
*******************************************************************************/

/**********************************************************************************
Function Name:   Sliders
Description:  Button to control Red color channel
Arguments:   Standard Button Inputs
              Instance (to distinguish among sliders)
Return value:   None
***********************************************************************************/
static int ImageSelect=0;
static const struct
{
  uint16_t mode;
  char const *text;
}ModeData[]=
{
  {GAPI_T_NONE, "NONE"},
  {GAPI_T_FILL, "FILL"},
  {GAPI_T_MUL,  "MUL"},
  {GAPI_T_MULK, "MULK"},
  {GAPI_T_MULC, "MULC"},
  {GAPI_T_MULT, "MULT"},
  {GAPI_T_MULS, "MULS"},
  
  {GAPI_TI_NONE, "I_NONE"},
  {GAPI_TI_FILL, "I_FILL"},
  {GAPI_TI_MUL,  "I_MUL"},
  {GAPI_TI_MULK, "I_MULK"},
  {GAPI_TI_MULC, "I_MULC"},
  {GAPI_TI_MULT, "I_MULT"},
  {GAPI_TI_MULS, "I_MULS"}
};

/*******************************************************************************
* Function Name: RenderMode
* Description  : Renders.
* Arguments    : None
* Return Value : None
*******************************************************************************/
static void RenderMode(void)
{
  int index;
  uint16_t Color;
  int16_t PosX;
  int16_t PosY = POSY;

  /* Fill in color table */
  BMP_t const *pBmp = pTestImage[ImageSelect];
  BMP_ColorTable_t ct;
  ct.channel[BMP_CT_RED] = (uint8_t)((uint16_t)State[0].Value/128);
  ct.channel[BMP_CT_GREEN] = (uint8_t)((uint16_t)State[1].Value/128);
  ct.channel[BMP_CT_BLUE] = (uint8_t)((uint16_t)State[2].Value/128);
  Color = R_GAPI_24_Disp(&ct);

  /* display the examples */
  for (index=0, PosX = 4; index<size_ra(ModeData); index++, PosX += (ICON_WIDTH + 11))
  {
    if(((size_ra(ModeData)/2)) == index)
    {
      /* move to 2nd row */
      PosX = 4;
      PosY += (ICON_HEIGHT + R_GAPI_FontHeight(&T1));
    }

    (void)R_GAPI_PutS((uint8_t *)ModeData[index].text, &T1, displayFrameBuf, PosX, PosY - R_GAPI_FontHeight(&T1)); 

    (void)R_GAPI_CopySub(backFrameBuf, workFrameBuf,     PosX,PosY, PosX,PosY, ICON_WIDTH,ICON_HEIGHT, R_BMP_Ct(backFrameBuf), TRANSPARENCY(GAPI_T_NONE,0));
    if(0 == ImageSelect)
    {
      (void)R_GAPI_CopySub(backFrameBuf, workFrameBuf,     PosX,PosY, PosX,PosY, ICON_WIDTH,ICON_HEIGHT, R_BMP_Ct(backFrameBuf), TRANSPARENCY(ModeData[index].mode,Color));
    }
    else
    {
      (void)R_GAPI_CopySub(pBmp, workFrameBuf,             PosX,PosY,    0,   0, ICON_WIDTH,ICON_HEIGHT,         R_BMP_Ct(pBmp), TRANSPARENCY(ModeData[index].mode,Color));
    }
    (void)R_GAPI_CopySub(workFrameBuf, displayFrameBuf,  PosX,PosY, PosX,PosY, ICON_WIDTH,ICON_HEIGHT, R_BMP_Ct(workFrameBuf), TRANSPARENCY(GAPI_T_NONE,0));
    }
}
/*******************************************************************************
End of function RenderMode
*******************************************************************************/

/* task to control rendering of test images */
static void render_task(void *pvParameters)
{
    portTickType xLastWakeTime, xFrequency = port_mS_to_TICK(1000); /* RTOS_USAGE */

    xLastWakeTime = xTaskGetTickCount();  /* RTOS_USAGE */

    for ( ;; )
    {
        /* Wait for the next cycle. */
        vTaskDelayUntil( &xLastWakeTime, xFrequency );  /* RTOS_USAGE */
        if (!slider_touched)
        {
            RenderMode();
        }
    }
}

/**********************************************************************************
Function Name:   Sliders
Description:  Button to control Red color channel
Arguments:   Standard Button Inputs
              Instance (to distinguish among sliders)
Return value:   None
***********************************************************************************/
static void Sliders(ScreenObj_t const *pS, EVENT_MSG const *pMsg, int16_t instance)
{
  char TextBuffer[8];
  static int16_t Width[3];
  State[instance].Touched = SliderHandler(pS, pMsg, pBMP_SliderButton, &State[instance] );

  if ((0 != State[instance].Touched) || (ME_DRAW_DISPLAY == pMsg->event))
  {
    slider_touched = 1u;

    /* Display Values */
    int16_t PosX = (pS->PosX + R_BMP_Width(*pS->ppBmp)) + 1;

    /* restore previous area */
    (void)BMPCopyBackDisplay(PosX,pS->PosY, Width[instance], R_GAPI_FontHeight(&T1));

    (void)sprintf(TextBuffer, "%3d", State[instance].Value/128);
    Width[instance] = R_GAPI_FontWidthString((uint8_t *)TextBuffer, &T1);
    (void)R_GAPI_PutS((uint8_t *)TextBuffer, &T1, displayFrameBuf, PosX, pS->PosY);
  }
  else
  {
      slider_touched = 0u;
  }
}
/*******************************************************************************
End of function Sliders
*******************************************************************************/

/**********************************************************************************
Function Name:   SliderRed
Description:  Button to control Red color channel
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void SliderRed(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  Sliders(pS, pMsg, SLIDER_RED );
}
/*******************************************************************************
End of function SliderRed
*******************************************************************************/

/**********************************************************************************
Function Name:   SliderGreen
Description:  Button to control Red color channel
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void SliderGreen(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  Sliders(pS, pMsg, SLIDER_GREEN );
}
/*******************************************************************************
End of function SliderGreen
*******************************************************************************/

/**********************************************************************************
Function Name:   SliderBlue
Description:  Button to control Red color channel
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void SliderBlue(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  Sliders(pS, pMsg, SLIDER_BLUE );
}
/*******************************************************************************
End of function SliderBlue
*******************************************************************************/

/**********************************************************************************
Function Name:   ImgSelect
Description:  Button to select image to render
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void ImgSelect(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;
  ScreenObj_t tempObj = *pS;

  tempObj.PosX = 4 + (pS->PosX * (ICON_WIDTH+18));

  if(0 != (state = ButtonHandler(&tempObj, pMsg, (uint8_t *)"", state)))
  {
    if (ME_RELEASE == pMsg->event)
    {
      ImageSelect = pS->PosX;
      RenderMode();
    }
  }
}
/*******************************************************************************
End of function ImgSelect
*******************************************************************************/



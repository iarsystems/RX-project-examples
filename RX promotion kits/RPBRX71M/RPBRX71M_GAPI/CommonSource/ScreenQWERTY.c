/* Adapted to IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Technology Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Technology Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* TECHNOLOGY CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
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
/*  FILE        :ScreenQWERTY.c                                        */
/*  DATE        :Sep 12, 2015                                          */
/*  DESCRIPTION :Screen Handler for QWERTY keypad                      */        
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
#include <string.h>
#include <stdio.h>
int errno_addr;

#define SCHEME T_SchemeGreen
static void Constructor(Screen_t const *pS);
static void Destructor(Screen_t const *pS);

static void ScreenQWERTY(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void Caps(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void AlphaNum(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void Spacebar(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
static void BackSpace(ScreenObj_t const *pS, EVENT_MSG const *pMsg);
void TextBufferChange(char Key);

static void Keyboard(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

typedef struct
{
  char Key[3];
  int16_t PosX;
  int16_t PosY;
} KEY_t;

#define KEY_SIZE    27
#define KEY_SPACE   3
#define KEY_BASE_X  0
#define KEY_BASE_Y  55
#define K_XY(X, Y)  (int16_t)((KEY_SIZE+KEY_SPACE)*X+KEY_BASE_X), (int16_t)((KEY_SIZE+KEY_SPACE)*Y+KEY_BASE_Y)

enum {MODE_UPPER, MODE_LOWER, MODE_SYMBOL} KeyMode;
struct
{
  int lower: 1;
  int symbol: 1;
} KeyState;

const KEY_t Keyboard_Caps[26]=
{
  {{'Q','q','1',},K_XY(0.5,2)},
  {{'W','w','2',},K_XY(1.5,2)},
  {{'E','e','3',},K_XY(2.5,2)},
  {{'R','r','4',},K_XY(3.5,2)},
  {{'T','t','5',},K_XY(4.5,2)},
  {{'Y','y','6',},K_XY(5.5,2)},
  {{'U','u','7',},K_XY(6.5,2)},
  {{'I','i','8',},K_XY(7.5,2)},
  {{'O','o','9',},K_XY(8.5,2)},
  {{'P','p','0',},K_XY(9.5,2)},
  
  {{'A','a','~',},K_XY(1.0,1)},
  {{'S','s','!',},K_XY(2.0,1)},
  {{'D','d','@',},K_XY(3.0,1)},
  {{'F','f','#',},K_XY(4.0,1)},
  {{'G','g','$',},K_XY(5.0,1)},
  {{'H','h','%',},K_XY(6.0,1)},
  {{'J','j','^',},K_XY(7.0,1)},
  {{'K','k','&',},K_XY(8.0,1)},
  {{'L','l','*',},K_XY(9.0,1)},
  
  {{'Z','z','(',},K_XY(2.0,0)},
  {{'X','x',')',},K_XY(3.0,0)},
  {{'C','c','-',},K_XY(4.0,0)},
  {{'V','v','+',},K_XY(5.0,0)},
  {{'B','b','=',},K_XY(6.0,0)},
  {{'N','n',';',},K_XY(7.0,0)},
  {{'M','m',':',},K_XY(8.0,0)}
};
  
#define CURSOR_START_POS    13
#define SPACE_BW_LETTERS    0

/* Number of displayed character is limited by number and available screen area */
#define MAX_DISPLAY_CHAR    (23)
#define TEXT_BOX_WIDTH		(292)		/* Width of the white text box on the screen background */

BMP_t const *pBMP_ButtonQWERTY;
BMP_t const *pBMP_ButtonBkSp;
BMP_t const *pBMP_ButtonSpbar;
BMP_t const *pBMP_ButtonHouse;
BMP_t const *pBMP_ButtonLeft;
BMP_t const *pBMP_ButtonRight;
BMP_t const *pBMP_ButtonCaps;
BMP_t const *pBMP_Button123;
BMP_t const *pBMP_ButtonBkSpIcon;

static  GAPI_PUT_t QWERTY_Font = { &pfontComicSansMS_Regular_B18, {CT_WHITE,CT_BLACK}, 0, 0, {0,0,0,0,1,0}};

#define CT_KEY_BLUE {192, 143, 36, 0x00}

static  GAPI_Scheme_t T_QWERTYScheme[]= 
{
  /* Button off/on */

  /* image black to dark blue, w/gray fore text, colorize with transparency and text */
  {{CT_BLACK,CT_KEY_BLUE    }, { &pfontComicSansMS_Regular_B22, {CT_NONE,CT_BLACK}, 0, 0, {1,1,0,0,1,0}}, {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}},

  /* image black to blue, w/white fore text, colorize with transparency and text */
  {{CT_BLACK,CT_BLUE}, { &pfontComicSansMS_Regular_B22, {CT_NONE,CT_WHITE}, 0, 0, {1,1,0,0,1,0}}, {CT_COLOR_NO_CHANGE,CT_TEXT_CENTER,GAPI_T_MULC}},
};

static  GAPI_Scheme_t T_HomeButtonScheme[]=
{
  /* Button on */
  {
    {CT_NONE,CT_WHITE}, 
    {&pfontSqBT_16, {CT_BLACK,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_MULT},
  },

  /* Button off */
  {
    {CT_NONE,CT_NONE}, 
    {&pfontSqBT_16, {CT_BLACK,CT_WHITE}, GAPI_F_SPACE_PROPORTIONAL, GAPI_F_SPACE_PROPORTIONAL, {GAPI_F_ALIGN_H_CENTER,GAPI_F_ALIGN_V_CENTER,0,0,GAPI_F_BACK_TRANSPARENT,GAPI_F_FORE_SOLID}},
    {CT_COLOR_NO_CHANGE,CT_TEXT_NONE,GAPI_T_NONE},
  },
};
static  ScreenObj_t Icons[]=
{
  { &pBMP_ButtonQWERTY, T_QWERTYScheme,      Keyboard,       0,       0 },
  { &pBMP_ButtonHouse,  T_HomeButtonScheme,  ButtonBack,    12,     211 },
  { &pBMP_ButtonCaps,   T_QWERTYScheme,      Caps,          13,      16 },
  { &pBMP_Button123,    T_QWERTYScheme,      AlphaNum,      45,      16 },
  { &pBMP_ButtonSpbar,  T_QWERTYScheme,      Spacebar,     77,       16 },
  { &pBMP_ButtonBkSp,   T_QWERTYScheme,      BackSpace,    230,      16 },
  { NULL,               NULL,                NULL,           0,       0 },
};
 BMP_ColorTable_t black_ct[] = {{0x00, 0x00, 0x00, 0x00},{0x00, 0x00,0x00, 0x00}};
 BMP_ColorTable_t white_ct[] = {{0xFF, 0xFF, 0xFF, 0xFF},{0xFF, 0xFF,0xFF, 0xFF}};
static  Screen_t ScreenQWERTYData=
{
  Constructor, Destructor, Icons
};


/* Shared event message */
static EVENT_MSG msg;

/*******************************************************************************
* Function Name: Constructor
* Description  : Closes the screen.
* Arguments    : pS -
*                  locates a screen within the resources
* Return Value : None
*******************************************************************************/
static void Constructor(Screen_t const *pS)
{
  
  if(NULL == pBMP_ButtonHouse)
  {
		pBMP_ButtonHouse = FileFind(pResources, "p_button_home");
	} 
  
  if(NULL == pBMP_ButtonQWERTY)
  {
		pBMP_ButtonQWERTY = FileFind(pResources, "p_button_qwerty");
	}
  
  if(NULL == pBMP_ButtonSpbar)
  {
		pBMP_ButtonSpbar = FileFind(pResources, "p_spacebar");
	}
  
  if(NULL == pBMP_ButtonBkSp)
  {
		pBMP_ButtonBkSp = FileFind(pResources, "p_backspace");
	}
  
  if(NULL == pBMP_ButtonLeft)
  {
		pBMP_ButtonLeft = FileFind(pResources, "p_button_arrow_left");
	}
  
  if(NULL == pBMP_ButtonRight)
  {
		pBMP_ButtonRight = FileFind(pResources, "p_button_arrow_right");
	}
  
  if(NULL == pBMP_ButtonCaps)
  {
		pBMP_ButtonCaps = FileFind(pResources, "p_button_shift");
	}

  if(NULL == pBMP_Button123)
  {
		pBMP_Button123 = FileFind(pResources, "p_123_qwerty");
	}
   
  /* Clear the background and set it to black for now. */
  (void)R_GAPI_FillGradient(backFrameBuf, 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT, &black_ct[0], 90);

  /* Copy Background image to background frame */
  (void)R_GAPI_Copy(FileFind(pResources, "p_qwerty_bg16"), backFrameBuf, 0, 0);

  /* Run default behaviour */
  BasicConstructor(pS);

  TextBufferChange(0x00);
}
/*******************************************************************************
End of function Constructor
*******************************************************************************/

/*******************************************************************************
* Function Name: Destructor
* Description  : Closes the screen.
* Arguments    : pS
*                p_output -
*                    Pointer of where to put the output data
* Return Value : None
*******************************************************************************/
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

#pragma location=".SCR_HM_CLASSIC"
__root const ScreenObj_t HomeIconQWERTY= { &pBMP_ButtonR, T_SchemeBlue,   ScreenQWERTY,  216, 26};

/*******************************************************************************
* Function Name: ScreenQWERTY
* Description  : Activates the QWERTY screen.
* Arguments    : pS -
*                      screen object
*                pMsg -
*                      Pointer of where to put the output data
* Return Value : None
*******************************************************************************/
static void ScreenQWERTY(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
    static int16_t state=0;

    if(0 != (state =ButtonHandler(pS, pMsg, (uint8_t *)"QWERTY", state)))
    {
        if (ME_RELEASE == pMsg->event)
        {
            /* send a screen change command to the handler */
            msg.source = MS_DISPLAY;
            msg.event = ME_SCREEN_PUSH;
            msg.param.pS = &ScreenQWERTYData;
            (void)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
        }
    }

    /* Wait for inputs to settle */
    /* RTOS_USAGE */
    vTaskDelay(1L);
}
/*******************************************************************************
End of function ScreenQWERTY
*******************************************************************************/

static char TextBuffer[MAX_DISPLAY_CHAR + 1];
static int TextBufferSize;

/**********************************************************************************
Function Name:   TextBufferChange
Description:  Display Edit String
Parameters:   None
Return value:   None
***********************************************************************************/
void TextBufferChange(char Key)
{
	int16_t width;

    if(Key < 0x20)
    {
        switch(Key)
        {
            case 0x08:
            {
                /* BS */
                if(TextBufferSize > 0)
                {
                    TextBufferSize--;
                    TextBuffer[TextBufferSize] = 0;
                }

                break;
            }
            default:
            {
                break;
            }
        }
    }
    else
    {
        if(TextBufferSize < MAX_DISPLAY_CHAR)
        {
            TextBuffer[TextBufferSize] = Key;
            TextBufferSize++;
            TextBuffer[TextBufferSize] = 0;
        }
    }

    /* Determine if the string can be displayed within the bounds of the background text area */
    width = R_GAPI_FontWidthString((uint8_t *)TextBuffer, &QWERTY_Font);

    /* If the new string can fit on the screen */
    if (width < TEXT_BOX_WIDTH)
    {
    	/* Display it */
        (void)BMPCopyBackWork( CURSOR_START_POS, 175, 440, 20 );
        (void)R_GAPI_PutS((uint8_t const *)TextBuffer, &QWERTY_Font, workFrameBuf, CURSOR_START_POS, 175);
        (void)BMPCopyWorkDisplay( CURSOR_START_POS, 175, 440, 20 );
    }
    else
    {
    	/* The last character would exceed screen size limit, remove it from the displayed string */
        TextBufferSize--;
    }

}
/*******************************************************************************
End of function TextBufferChange
*******************************************************************************/

/**********************************************************************************
Function Name:   Keyboard
Description:  Display Keyboard
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void Keyboard(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  int index, redraw = 0;
  static int16_t state[size_ra(Keyboard_Caps)];
  ScreenObj_t local_obj = *pS;
  uint8_t Text[]="0";
  
  if(NULL == pMsg)
  {
    /* receiving a NULL message is our signal to force redraw */
      msg.event = ME_MOVE;
      msg.param.coord[0] = -1;  /* set coordinates outside touch area */
      msg.param.coord[1] = -1;
      pMsg = &msg;
      redraw = 1;
  }
  
  for (index = 0; index < size_ra(Keyboard_Caps); index++)
  {
    Text[0] = (uint8_t)(Keyboard_Caps[index].Key[KeyMode]);
    local_obj.PosX = Keyboard_Caps[index].PosX;
    local_obj.PosY = Keyboard_Caps[index].PosY;

    if(1 == redraw)
    {
	    state[index] = 1;
	}

    if(0 != (state[index]=ButtonHandler(&local_obj, pMsg, Text, state[index])))
    {
      if (ME_PRESS == pMsg->event)
      {

        TextBufferChange((char)Text[0]);

        /* Wait for inputs to settle */
        /* RTOS_USAGE */
        vTaskDelay(3L);
      }

      /* Wait for inputs to settle */
      /* RTOS_USAGE */
      vTaskDelay(3L);
    }
  }
}
/*******************************************************************************
End of function Keyboard
*******************************************************************************/

/**********************************************************************************
Function Name:   Caps
Description:  Toggle Caps
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void Caps(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)" ",state)))
  {
    if (ME_PRESS == pMsg->event)
    {
      if(1 == KeyState.symbol)
      {
        KeyState.symbol = 0;
      }
      else
      {
          KeyState.lower++;
      }

      if(1 == KeyState.lower)
      {
		KeyMode = MODE_LOWER;
	  }
      else
      {
		KeyMode = MODE_UPPER;
	  }
      
      /* signal the callback to redraw */
      Keyboard(&Icons[0],NULL);

      /* Wait for inputs to settle */
      /* RTOS_USAGE */
      vTaskDelay(3L);
    }

    /* Wait for inputs to settle */
    /* RTOS_USAGE */
    vTaskDelay(3L);
  }
}
/*******************************************************************************
End of function Caps
*******************************************************************************/

/**********************************************************************************
Function Name:   AlphaNum
Description:  Toggle AlphaNum
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void AlphaNum(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"",state)))
  {
    if (ME_PRESS  == pMsg->event)
    {
        KeyState.symbol++;

        if(1 == KeyState.symbol)
        {
            KeyMode = MODE_SYMBOL;
        }
        else if(1 == KeyState.lower)
        {
            KeyMode = MODE_LOWER;
        }
        else
        {
            KeyMode = MODE_UPPER;
        }
        
        /* signal the callback to redraw */
        Keyboard(&Icons[0],NULL);

        /* Wait for inputs to settle */
        /* RTOS_USAGE */
        vTaskDelay(4L);
    }

    /* Wait for inputs to settle */
    /* RTOS_USAGE */
    vTaskDelay(4L);
  }  
}
/*******************************************************************************
End of function AlphaNum
*******************************************************************************/

/**********************************************************************************
Function Name:   Spacebar
Description:  Insert Space
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void Spacebar(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)" ",state)))
  {
    if (ME_PRESS == pMsg->event)
    {

        TextBufferChange(' ');

        /* Wait for inputs to settle */
        /* RTOS_USAGE */
        vTaskDelay(3L);
    }

    /* Wait for inputs to settle */
    /* RTOS_USAGE */
    vTaskDelay(3L);
  }
}
/*******************************************************************************
End of function Spacebar
*******************************************************************************/

/**********************************************************************************
Function Name:   BackSpace
Description:  
Parameters:   Standard Button Inputs
Return value:   None
***********************************************************************************/
static void BackSpace(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t state=0;

  if(0 != (state = ButtonHandler(pS, pMsg, (uint8_t *)"",state)))
  {
    if (ME_PRESS == pMsg->event)
    {
        /* Wait for inputs to settle */
        /* RTOS_USAGE */
        vTaskDelay(3L);

        TextBufferChange(0x08);
    }

    /* Wait for inputs to settle */
    /* RTOS_USAGE */
    vTaskDelay(3L);
  }
}
/*******************************************************************************
End of function BackSpace
*******************************************************************************/

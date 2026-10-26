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
/*  FILE        :ScreenObjs.c                                          */
/*  DESCRIPTION :Support for common screen objects                     */        
/*                                                                     */
/***********************************************************************/
#include "Global.h"
#include "LCD_demo.h"

/**********************************************************************************
  Generic Handlers
***********************************************************************************/
/* Shared event message */
static EVENT_MSG msg;

/**********************************************************************************
Function Name:   ButtonHandler
Description:  Handles button behavior...checks if in bounds and displays based on message
Arguments:   pS: pointer to icon information
              pMsg: pointer to current message information
              pT: Pointer to text string to apply to label.
              state: State of image 0: off, 1: on
Return value:   0:Not in bounds, Not 0: in bounds of button
***********************************************************************************/
int16_t ButtonHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, uint8_t const *pT, int16_t State )
{
  int16_t Width = R_BMP_Width(*pS->ppBmp);
  int16_t Height = R_BMP_Height(*pS->ppBmp);
  int16_t Touched = 0;
  int16_t DrawState = -1;  // Default is don't draw

  /* check if we're forcing a redraw */
  if((-1) == State)
  {
		DrawState = 0;
  }
    
  switch(pMsg->event)
  {
    case ME_DRAW_WORK:
    {
      (void) R_GAPI_Button(*pS->ppBmp, workFrameBuf, pS->PosX, pS->PosY, &pS->pScheme[0], pT);
      break;
    }
    
    case ME_MOVE:
    case ME_PRESS:
    case ME_RELEASE:
    {

      /* Check if touch is in button bounds*/
      if (((pS->PosX <= pMsg->param.coord[0]) && (pMsg->param.coord[0] < (pS->PosX + Width))) &&
          ((pS->PosY <= pMsg->param.coord[1]) && (pMsg->param.coord[1] < (pS->PosY + Height))))
      {
			Touched = 1;
	  }

      /* Make sure we're displaying proper behavior */
      if ((0==Touched) && (1 == State))
      {
          DrawState = 0;
	  }
      else if ((1==Touched) && (0 == State))
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

      break;
    }
    default:
    {
      Touched = State;  /* if this isn't a message id we process...don't change state */
      break;
    }
  } /* switch */
  
  if((-1) != DrawState)
  {  
    (void)BMPCopyBackWork(pS->PosX, pS->PosY, Width, Height);

    if(0 == DrawState)
    {
			(void) R_GAPI_Button(*pS->ppBmp, workFrameBuf, pS->PosX, pS->PosY,
					&pS->pScheme[0], pT);
	}
    else if(1 == DrawState)
    {
			(void) R_GAPI_Button(*pS->ppBmp, workFrameBuf, pS->PosX, pS->PosY,
					&pS->pScheme[1], pT);
	}
    else
    {
    	/* Do nothing */
    }

    (void)BMPCopyWorkDisplay(pS->PosX, pS->PosY, Width, Height);
  }
  
  return(Touched);
}
/*******************************************************************************
End of function ButtonHandler
*******************************************************************************/

/**********************************************************************************
Function Name:   IconHandler
Description:  Handles icon behavior...checks if in bounds and displays based on message
Arguments:   pS: pointer to icon information
              pMsg: pointer to current message information
              index: indexed image within icon bmp
              state: State of image 0: off, 1: on
Return value:   0:Not in bounds, Not 0: in bounds of button
***********************************************************************************/
int16_t IconHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, int16_t index, int16_t State )
{
  int16_t Width = R_BMP_Height(*pS->ppBmp);
  int16_t Height = R_BMP_Height(*pS->ppBmp);
  int16_t Touched = 0;

  switch(pMsg->event)
  {
    case ME_DRAW_WORK:
    {
      (void) R_GAPI_Index(*pS->ppBmp,workFrameBuf,pS->PosX,pS->PosY, index);
      break;
    }
    
    case ME_MOVE:
    case ME_PRESS:
    case ME_RELEASE:
    {
      /* Check if touch is in button bounds*/
      if (((pS->PosX <= pMsg->param.coord[0]) && (pMsg->param.coord[0] < (pS->PosX + Width))) &&
          ((pS->PosY <= pMsg->param.coord[1]) && (pMsg->param.coord[1] < (pS->PosY + Height))))
        {
			Touched = 1;
		}
      break;
    }

    default:
    {
      return(State);  /* if this isn't a message id we process...don't change state */
    }
  } /* switch */  
  return(Touched);
}
/*******************************************************************************
End of function IconHandler
*******************************************************************************/

/**********************************************************************************
Function Name:   SliderHandler
Description:  Handles button behavior...checks if in bounds and displays based on message
Arguments:   pS: pointer to icon information
              pMsg: pointer to current message information
              pB: pointer to button BMP
              pState: pointer to previous/next state
Return value:   0:Not in bounds, Not 0: in bounds of button
***********************************************************************************/
int16_t SliderHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, BMP_t const *const pB, SLIDERSTATE_type *pState )
{
  int16_t SWidth = R_BMP_Width(*pS->ppBmp);
  int16_t SHeight = R_BMP_Height(*pS->ppBmp);
  int16_t BWidth = R_BMP_Width(pB);
  int16_t BHeight = R_BMP_Height(pB);
  int16_t ButtonDraw = 0;
  int16_t Touched = 0;

  if (pState->ValueOld != pState->Value)
  {
    /* state has externally changed...update image */
      ButtonDraw = 1;
  }
    
  switch(pMsg->event)
  {
    case ME_DRAW:
    {
      (void)R_GAPI_ColorCopy(*pS->ppBmp, backFrameBuf, pS->PosX, pS->PosY, &pS->pScheme[0]);
      break;
    }

    case ME_DRAW_WORK:
    {
      ButtonDraw = 1;
      break;
    }
    
    case ME_PRESS:
    case ME_MOVE:
    {
        /* only process move if already touched */
        if((ME_MOVE == pMsg->event) && (0 == pState->Touched))
        {
			return (0);
		}

        /* Check if touch is within slider */
        if (((pS->PosX <= pMsg->param.coord[0]) && (pMsg->param.coord[0] < (pS->PosX + SWidth))) &&
          ((pS->PosY <= pMsg->param.coord[1]) && (pMsg->param.coord[1] < (pS->PosY + SHeight))))
        {
            int16_t Value;
            int16_t Min;
            int16_t Max;

            if (SWidth > SHeight)
            {
                /* Horizontal Orientation*/
                /* Determine new value */
                Min = (int16_t)(pS->PosX + (BWidth/2));
                Max = (int16_t)((pS->PosX + SWidth) - (BWidth/2));
                Value = pMsg->param.coord[0];
            }
            else
            {
                /* Vertical Orientation */
                /* Determine new value */
                Min = (int16_t)(pS->PosY + (BHeight/2));
                Max = (int16_t)((pS->PosY + SHeight) - (BHeight/2));
                Value = pMsg->param.coord[1];
            }
            {
                int16_t Range = (int16_t)(Max-Min);
                Value = (int16_t)((Value < Min) ? Min : Value);
                Value = (int16_t)((Value > Max) ? Max : Value);
                Value = (int16_t)(Value - Min);
                Value = (int16_t)((((int32_t)Value * 32767L) + ((int32_t)Range/2)) / (int32_t)Range);
            }

            pState->Value = Value;
            ButtonDraw = 1;
            Touched = 1;
        }
        break;
    }
    
    case ME_RELEASE:
      Touched = 0;
      break;

    default:
      Touched = pState->Touched;
      break;
  } /* switch */  

  if(0 != ButtonDraw)
  {
    int16_t PosXnew;
    int16_t PosYnew;
    int16_t PosXold;
    int16_t PosYold;
    int16_t Min;
    int16_t Range;
    
    if (SWidth > SHeight)
    {
      /* horizontal orientation */
      PosYold = PosYnew = (pS->PosY + (SHeight/2)) - (BHeight/2);
      Min = pS->PosX + (BWidth/2);
      Range = ((pS->PosX + SWidth) - (BWidth/2)) - Min;
      PosXold = ((int16_t)(((int32_t)Range * (int32_t)pState->ValueOld) / 32768L) + Min)  - (BWidth/2);
      PosXnew = ((int16_t)(((int32_t)Range * (int32_t)pState->Value) / 32768L) + Min) - (BWidth/2);
    }
    else
    {
      /* vertical orientation */
      PosXold = PosXnew = (pS->PosX + (SWidth/2)) - (BWidth/2);
      Min = pS->PosY + (BHeight/2);
      Range = ((pS->PosY + SHeight) - (BHeight/2)) - Min;
      PosYold = ((int16_t)(((int32_t)Range * (int32_t)pState->ValueOld) / 32768L) + Min) - (BHeight/2);
      PosYnew = ((int16_t)(((int32_t)Range * (int32_t)pState->Value) / 32768L) + Min) - (BHeight/2);
    }

    /* Move it */
    (void)BMPMove(pB,PosXold,PosYold, PosXnew,PosYnew,&pS->pScheme[1]);
    pState->ValueOld = pState->Value;
  }  
  return(Touched);
}
/*******************************************************************************
End of function SliderHandler
*******************************************************************************/

/**********************************************************************************
Function Name:   SliderSwitchHandler
Description:  Handles slider switch behaviour
Arguments:   pS: pointer to icon information
              pMsg: pointer to current message information
              pB: pointer to button BMP
              pT: pointer to label text string
              pState: pointer to previous/next state
Return value:   0:Not in bounds, Not 0: in bounds of button
***********************************************************************************/
int16_t SliderSwitchHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, BMP_t const *const pB, uint8_t const *const pT[], SLIDERSTATE_type *pState )
{
  int16_t SWidth = R_BMP_Width(*pS->ppBmp);
  int16_t SHeight = R_BMP_Height(*pS->ppBmp);
  int16_t BWidth = R_BMP_Width(pB);
  int16_t BHeight = R_BMP_Height(pB);
  int16_t ButtonDraw = 0;
  int16_t Touched = 0;

  switch(pMsg->event)
  {
    case ME_DRAW:
    {
      (void)R_GAPI_ColorCopy(*pS->ppBmp, backFrameBuf, pS->PosX, pS->PosY, &pS->pScheme[0]);
      break;
    }

    case ME_DRAW_WORK:
    {
      ButtonDraw = 1;
      break;
    }
    
    case ME_PRESS:
    case ME_MOVE:
    {
      /* only process move if already touched */
      if((ME_MOVE == pMsg->event)  && (0 == pState->Touched))
      {
        return(0);
      }

      /* Check if touch is within slider */
      if (((pS->PosX <= pMsg->param.coord[0]) && (pMsg->param.coord[0] < (pS->PosX + SWidth))) &&
          ((pS->PosY <= pMsg->param.coord[1]) && (pMsg->param.coord[1] < (pS->PosY + SHeight))))
      {
        int16_t Value;
        int16_t Min;
        int16_t Max;

        if (SWidth > SHeight)
        {
          /* Horizontal Orientation*/
          /* Determine new value */
          Min = (int16_t)(pS->PosX + (BWidth/2));
          Max = (int16_t)((pS->PosX + SWidth) - (BWidth/2));
          Value = pMsg->param.coord[0];
        }
        else
        {
          /* Vertical Orientation */
          /* Determine new value */
          Min = (int16_t)(pS->PosY + (BHeight/2));
          Max = (int16_t)((pS->PosY + SHeight) - (BHeight/2));
          Value = pMsg->param.coord[1];
        }
        {
          int16_t Range = (int16_t)(Max - Min);
          Value = (int16_t)((Value < Min) ? Min : Value);
          Value = (int16_t)((Value > Max) ? Max : Value);
          Value = (int16_t)(Value - Min);
          Value = (int16_t)((((int32_t)Value * 32767L) + ((int32_t)Range/2)) / (int32_t)Range);
        }
        Value = (int16_t)((Value < 16384) ? 0:1);

        /* only draw if state changed */
        ButtonDraw = (int16_t)((Value != pState->Value) ? 1 : 0);
        pState->Value = Value;
        Touched = 1;
      }
      break;
    }
    
    case ME_RELEASE:
      return(0);

    default:
      return(pState->Touched);
  } /* switch */  

  if(0 != ButtonDraw)
  {
    int16_t PosXnew;
    int16_t PosYnew;
    int16_t PosXold;
	int16_t PosYold;

    if (SWidth > SHeight)
    {
      /* horizontal orientation */
      PosYold = PosYnew = (int16_t)((pS->PosY + (SHeight/2)) - (BHeight/2));

	  if (0 == pState->Value)
      {
        PosXnew = pS->PosX;
        PosXold = (int16_t)((pS->PosX + SWidth) - BWidth);
      }
      else
      {
        PosXnew = (int16_t)((pS->PosX + SWidth) - BWidth);
        PosXold = pS->PosX;
      }
    }
    else
    {
      /* vertical orientation */
      PosXold = PosXnew = (int16_t)((pS->PosX + (SWidth/2)) - (BWidth/2));

	  if (0 == pState->Value)
      {
        PosYnew = pS->PosY;
        PosYold = (int16_t)((pS->PosY + SHeight) - BHeight);
      }
      else
      {
        PosYnew = (int16_t)((pS->PosY + SHeight) - BHeight);
        PosYold = pS->PosY;
      }
    }

    /* Move it */
    (void)BMPMove(pB,PosXold,PosYold, PosXnew,PosYnew,&pS->pScheme[1]);
    (void)R_GAPI_Label(pB,    workFrameBuf, PosXnew, PosYnew, &pS->pScheme[1], pT[pState->Value]);
    (void)R_GAPI_Label(pB, displayFrameBuf, PosXnew, PosYnew, &pS->pScheme[1], pT[pState->Value]);

    pState->ValueOld = pState->Value;
  }
  return(Touched);
}
/*******************************************************************************
End of function SliderSwitchHandler
*******************************************************************************/

/**********************************************************************************
Function Name:   DataBoxHandler
Description:  Handles Data Displaying behavior...checks if in bounds 
Arguments:   pS: pointer to icon information
              pMsg: pointer to current message information
              pL: Pointer to text string to apply to label.
              pD: Ponter to data string to display
Return value:   0:Not in bounds, Not 0: in bounds of button
***********************************************************************************/
int16_t DataBoxHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, uint8_t const *pL, uint8_t const *pD )
{
  int16_t Width = R_BMP_Width(*pS->ppBmp);
  int16_t Height = R_BMP_Height(*pS->ppBmp);
  int16_t Touched = 0;

  switch(pMsg->event)
  {
    case ME_DRAW:
    {
      (void) R_GAPI_Button(*pS->ppBmp, backFrameBuf, pS->PosX, pS->PosY, &pS->pScheme[0], pL);
      break;
    }
    
    case ME_MOVE:
    case ME_PRESS:
    case ME_RELEASE:
    {
      /* Check if touch is in button bounds*/
      if (((pS->PosX <= pMsg->param.coord[0]) && (pMsg->param.coord[0] < (pS->PosX + Width))) &&
          ((pS->PosY <= pMsg->param.coord[1]) && (pMsg->param.coord[1] < (pS->PosY + Height))))
        {
			Touched = 1;
		}
      break;
    }

    case ME_DRAW_DISPLAY:
    {
      (void)BMPCopyBackWork(pS->PosX, pS->PosY, Width, Height);
      (void)R_GAPI_Label(*pS->ppBmp, workFrameBuf, pS->PosX, pS->PosY, &pS->pScheme[1], pD);
      (void)BMPCopyWorkDisplay(pS->PosX, pS->PosY, Width, Height);
      break;
    }

    default:
    {
    	break;
    }

  } /* switch */
  return(Touched);
}
/*******************************************************************************
End of function DataBoxHandler
*******************************************************************************/

/**********************************************************************************
Function Name:   ButtonBack
Description:  Return to previous screen
Arguments:   Standard Button Inputs
Return value:   None
***********************************************************************************/
void ButtonBack(ScreenObj_t const *pS, EVENT_MSG const *pMsg)
{
  static int16_t BackState=0;

  if(0!=(BackState=ButtonHandler(pS, pMsg, (uint8_t *)"BACK",BackState)))
  {
    if (ME_RELEASE == pMsg->event)
    {
      /* send a screen change command to the handler */
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_POP;
      msg.param.pS = NULL;
      (void)(int16_t)xQueueSend(EventQueue, (void *)&msg, 0UL);   /* RTOS_USAGE */
    }
  }
}
/*******************************************************************************
End of function ButtonBack
*******************************************************************************/



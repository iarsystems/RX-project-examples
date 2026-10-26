/* Adapted for use with IAR Embedded Workbench */
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
******************************************************************************
* Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
*******************************************************************************  
* File Name    : pong_scores.c
* Version      : 1.00
* Description  : Storage area for Pong Score functions
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.10.2010 1.00    First Release
******************************************************************************/
#include "pong.h"
#include "screen.h"
#include "vnc-server.h"

/* IO Port RPDL function defintions */
/* for uint8_t */
#include "r_pdl_io_port.h"


/******************************************************************************
* Function Name: initScores
* Description  : Sets up the score boards
* Arguments    : none
* Return Value : none
******************************************************************************/
void initScores(void)
{
  uint8_t i, j;

  /* Initialise both score boards to '00' */
  for (i = 0; i < SCORE_DIGIT_HGT; i++)
  {
    for(j = 0; j < SCORE_DIGIT_WDT; j++)
    {
      if (number_0_bmp[i][j] != 0x00)
      {
        g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
        g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
        g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
        g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
      }
    }
  }
  updateVNC(P1_SCORE_DIGIT1_X, SCORE_DIGIT_Y, SCORE_WDT, SCORE_DIGIT_HGT, VNC_SCORE_1);
  updateVNC(P2_SCORE_DIGIT1_X, SCORE_DIGIT_Y, SCORE_WDT, SCORE_DIGIT_HGT, VNC_SCORE_2);
}

/******************************************************************************
* Function Name: updateScores
* Description  : Replaces the score boards with new values
* Arguments    : Player number, Score
* Return Value : none
******************************************************************************/
void updateScores(int nPlayer, int nScore)
{
  uint8_t i, j;
  int firstDigit, secondDigit;
  
  /* Cap the score at 99 */
  nScore = nScore % 100;
  
  firstDigit = nScore / 10;
  secondDigit = nScore % 10;
  switch(firstDigit)
  {
    case 0:    
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_0_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_0_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 1:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_1_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_1_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 2:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_2_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_2_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 3:    
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_3_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_3_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 4:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_4_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_4_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 5:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_5_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_5_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 6:    
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_6_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_6_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 7:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_7_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_7_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 8:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_8_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_8_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 9:    
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_9_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_9_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    default:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_0_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_0_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
  }
  switch(secondDigit)
  {
    case 0:    
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_0_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_0_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 1:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_1_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_1_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 2:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_2_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_2_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 3:    
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_3_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_3_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 4:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_4_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_4_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 5:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_5_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_5_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 6:    
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_6_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_6_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 7:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_7_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_7_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 8:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_8_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_8_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    case 9:    
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_9_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_9_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT2_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
    default:
      for (i = 0; i < SCORE_DIGIT_HGT; i++)
      {
        for(j = 0; j < SCORE_DIGIT_WDT; j++)
        {
          if (nPlayer == 1)
          {
            if (number_0_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P1_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
          else if (nPlayer == 2)
          {
            if (number_0_bmp[i][j] == 0x00)
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = PONG_BG;
            }
            else
            {
              g_ucFrameBuf[SCORE_DIGIT_Y + i][P2_SCORE_DIGIT1_X + j] = SCORE_COL;
            }
          }
        }
      }
      break;
  }
  
  if (nPlayer == 1)
  {
    updateVNC
    (
      P1_SCORE_DIGIT1_X,
      SCORE_DIGIT_Y,
      SCORE_WDT,
      SCORE_DIGIT_HGT,
      VNC_SCORE_1
    );
  }
  else if (nPlayer == 2)
  {
    updateVNC
    (
      P2_SCORE_DIGIT1_X,
      SCORE_DIGIT_Y,
      SCORE_WDT,
      SCORE_DIGIT_HGT,
      VNC_SCORE_2
    );
  }
}

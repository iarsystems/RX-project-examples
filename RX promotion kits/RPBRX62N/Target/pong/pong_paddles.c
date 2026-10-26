/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is
* only intended for use with Renesas products.  

* No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is 
* protected under the applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH
* WARRANTIES ARE EXPRESSLY DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER 
* RENESAS ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR AND DIRECT, INDIRECT, SPECIAL, INCIDENTAL
* OR COSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE,
* EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE
* POSSIBILITIES OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue availability of this software.
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010. Renesas Electronics Corp., All Rights Reserved         */
/******************************************************************************
* File Name     : pong_paddles.c
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RX-Stick
* Description   : Storage for paddle functions used with Pong demo
* Operation     :
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 06.10.2010  1.00    First release
*******************************************************************************/

/******************************************************************************
 Include files
******************************************************************************/
#include "pong.h"
#include "screen.h"
#include "vnc-server.h"

/******************************************************************************
 Macro definitions
******************************************************************************/

/******************************************************************************
 Type defintions
******************************************************************************/

/******************************************************************************
 External (imported) data and functions (from other files) [list vars then fns]
******************************************************************************/

/******************************************************************************
 Exported global variables & functions (to be used by other modules)
******************************************************************************/
/* 1 struct for each players paddle in each frame buffer */
PADDLE Paddle_P1;
PADDLE Paddle_P2;

/******************************************************************************
 Private (static) variables and functions
******************************************************************************/

/******************************************************************************
* Function Name: initPaddles
* Description  : Sets up the two paddles for display.
* Arguments    : none
* Return Value : none
******************************************************************************/
void initPaddles(void)
{
  uint8_t i, j;

  /* Set current direction to STILL to begin with */
  Paddle_P1.dir = STILL;
  Paddle_P2.dir = STILL;

  /* Set the X and Y positions for P1 */
  Paddle_P1.x  = PADDLE_P1_X;
  Paddle_P1.y  = PADDLE_Y_START;
  
  /* Set the X and Y positions for P2 */
  Paddle_P2.x  = PADDLE_P2_X;
  Paddle_P2.y  = PADDLE_Y_START;

  /* Draw the paddles */
  for (i = 0; i < PADDLE_HGT; i++)
  {
    for(j = 0; j < PADDLE_WDT; j++)
    {
      g_ucFrameBuf[Paddle_P1.y + i][Paddle_P1.x + j] = PADDLE_COL;
      g_ucFrameBuf[Paddle_P2.y + i][Paddle_P2.x + j] = PADDLE_COL;
    }
  }
  /* Update the VNC server */
  updateVNC
  (
    Paddle_P1.x,
    Paddle_P1.y,
    PADDLE_WDT,
    PADDLE_HGT,
    VNC_PAD_DRAW_1
  );
  updateVNC
  (
    Paddle_P2.x,
    Paddle_P2.y,
    PADDLE_WDT,
    PADDLE_HGT,
    VNC_PAD_DRAW_2
  );
}


/******************************************************************************
* Function Name: refreshPaddle
* Description  : Updates the positions of paddle
* Arguments    : pointer to the Paddle
* Return Value : none
******************************************************************************/
void refreshPaddle(PADDLE *thePaddle)
{
  uint8_t i, j;

  /* Respond to the joystick & update the paddle position. */
  if (thePaddle->dir == UP)
  {
    /* Check if paddle is near the top of the screen */
    if (thePaddle->y <= VERT_DISTANCE)
    {/* If it's not 0, then make it so */
      if (thePaddle->y != 0)
      {
        thePaddle->y = 0;
        /* Remove the bottom of the paddle */
        for (i = 0; i < VERT_DISTANCE; i++)
        {  
          for(j = 0; j < PADDLE_WDT; j++)
          {
            g_ucFrameBuf[PADDLE_HGT + i][thePaddle->x + j] = PONG_BG;
          }
        }
      }
    }
    /* If it's not near the top of the screen */
    else
    {
      /* Move it up */
      thePaddle->y -= VERT_DISTANCE;
    
      /* Replace bottom of paddle with background and draw top */
      for (i = 0; i < VERT_DISTANCE; i++)
      {  
        /* Check if the background is the middle line
         * if so replace with white instead of usual BG */
        if (BOARD_CENTRE == (thePaddle->y + PADDLE_HGT + i))
        {
          for(j = 0; j < PADDLE_WDT; j++)
          {
            g_ucFrameBuf[thePaddle->y + PADDLE_HGT + i][thePaddle->x + j] = CENTER_COL;
            /* Draw top of paddle */
            g_ucFrameBuf[thePaddle->y + i][thePaddle->x + j] = PADDLE_COL;
          }
        }
        else
        {
          for(j = 0; j < PADDLE_WDT; j++)
          {
            g_ucFrameBuf[thePaddle->y + PADDLE_HGT + i][thePaddle->x + j] = PONG_BG;
            /* Draw top of paddle */
            g_ucFrameBuf[thePaddle->y + i][thePaddle->x + j] = PADDLE_COL;
          }
        }
      }
    }
    /* Update the VNC with the relevant paddle */
    if (thePaddle == &Paddle_P1)
    {
      updateVNC
      (
        thePaddle->x,
        thePaddle->y,
        PADDLE_WDT,
        PADDLE_HGT + VERT_DISTANCE,
        VNC_PAD_DRAW_1
      );
    }
    else
    {
      updateVNC
      (
        thePaddle->x,
        thePaddle->y,
        PADDLE_WDT,
        PADDLE_HGT + VERT_DISTANCE,
        VNC_PAD_DRAW_2
      );
    }
  }
  else if (thePaddle->dir == DOWN)
  {
    /* Check if paddle is near the bottom of the screen */
    if (thePaddle->y + PADDLE_HGT >= Y_SIZE - VERT_DISTANCE)
    {/* If it's not at the bottom, then make it so */
      if (thePaddle->y != Y_SIZE - PADDLE_HGT)
      {
        thePaddle->y = Y_SIZE - PADDLE_HGT;
        /* Remove the top of the paddle */
        for (i = 0; i < VERT_DISTANCE; i++)
        {  
          for(j = 0; j < PADDLE_WDT; j++)
          {
            g_ucFrameBuf[thePaddle->y - VERT_DISTANCE + i][thePaddle->x + j] = PONG_BG;
          }
        }
      }
    }
    /* If it's not near the bottom of the screen */
    else
    {
      /* Move it down */
      thePaddle->y += VERT_DISTANCE;
      
      /* Replace top of paddle with background and draw bottom */
      for (i = 0; i < VERT_DISTANCE; i++)
      {  
        /* Check if the background is the middle line
         * if so replace with white instead of usual BG */
        if (BOARD_CENTRE == (thePaddle->y - VERT_DISTANCE + i))
        {
          for(j = 0; j < PADDLE_WDT; j++)
          {
            g_ucFrameBuf[thePaddle->y - VERT_DISTANCE + i][thePaddle->x + j] = CENTER_COL;
            /* Draw bottom of paddle */
            g_ucFrameBuf[thePaddle->y + PADDLE_HGT - VERT_DISTANCE + i][thePaddle->x + j] = PADDLE_COL;
          }
        }
        else
        {
          for(j = 0; j < PADDLE_WDT; j++)
          {
            g_ucFrameBuf[thePaddle->y - VERT_DISTANCE + i][thePaddle->x + j] = PONG_BG;
            /* Draw bottom of paddle */
            g_ucFrameBuf[thePaddle->y + PADDLE_HGT - VERT_DISTANCE + i][thePaddle->x + j] = PADDLE_COL;
          }
        }
      }
    }
    /* Update the VNC with the relevant paddle */
    if (thePaddle == &Paddle_P1)
    {
      updateVNC
      (
        thePaddle->x,
        thePaddle->y - VERT_DISTANCE,
        PADDLE_WDT,
        PADDLE_HGT + VERT_DISTANCE,
        VNC_PAD_DRAW_1
      );
    }
    else
    {
      updateVNC
      (
        thePaddle->x,
        thePaddle->y - VERT_DISTANCE,
        PADDLE_WDT,
        PADDLE_HGT + VERT_DISTANCE,
        VNC_PAD_DRAW_2
      );
    }
  }
}

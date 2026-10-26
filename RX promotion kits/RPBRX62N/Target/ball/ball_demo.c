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
*/
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : ball_demo.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK+RX62N
* Description   : Demonstration of the FPU vs normal SW floating point
*          calculations with the use of bouncing balls.
*          
******************************************************************************/
/******************************************************************************
* History     : 20.09.2010 Ver. 1.00 First Release
******************************************************************************/
#include <stdlib.h>
#include "ball.h"
#include "screen.h"
#include "mt.h"

#include "uip.h"
#include "vnc-server.h"

/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
******************************************************************************/
extern int g_iYLine;

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/


/******************************************************************************
 Private (static) variables and constants
******************************************************************************/

/******************************************************************************
* Function Name: drawBallBackground
* Description  : Draws white background with centred blue Renesas logo.
* Arguments    : none
* Return Value : none
******************************************************************************/
void drawBallInitialScreen(void)
{
  int x, y;
  
  // Make the full screen white
  for (y=0; y<Y_SIZE; y++)
  {
    for (x=0; x<X_SIZE; x++)
    {
      g_ucFrameBuf[y][x] = VWHITE;
    }
  }
  // Add Renesas logo
  for (y=0; y<LOGO_HGT; y++)
  {
    for (x=0; x<LOGO_WDT; x++)
    {
      g_ucFrameBuf[y+UPPER_BOTTOM_EDGE][x] = renesaslogo_bmp[y][x];
    }
  }
}

/******************************************************************************
* Function Name: refresh_theBall
* Description  : 
* Arguments    : none
* Return Value : none
******************************************************************************/
void refresh_Ball(BALL *pBall)
{
  /* Remove old ball */
  if (pBall == &theBall)
  {
    clear_Ball(pBall, VNC_SWBALL_CLEAR);
  }
  else if (pBall == &fpuBall)
  {
    clear_Ball(pBall, VNC_FPUBALL_CLEAR);
  }
  
  pBall->px = pBall->x;
  pBall->py = pBall->y;
  
  /* Draw the ball with current X and Y values */
  if (pBall == &theBall)
  {
    draw_Ball(pBall, VNC_SWBALL_DRAW);
  }
  else if (pBall == &fpuBall)
  {
    draw_Ball(pBall, VNC_FPUBALL_DRAW);
  }
}

/******************************************************************************
* Function Name: clear_Ball
* Description  : Clears ball on the screen, replacing the background and updates VNC struct
* Arguments    : Pointer of which ball to draw, vncBall struct array index
* Return Value : none
******************************************************************************/
void clear_Ball(BALL *pBall, uint8_t vncBlockIndex)
{
  uint8_t i, j;
  
  /* Only update the ball position when the VNC client has been updated with the whole screen */
  if (g_iYLine>=Y_SIZE)
  {
    for (i = 0; i < BALL_HGT; i++)
    {
      /* If it's above or below the logo, just colour white */
      if ((uint8_t)pBall->py + i < UPPER_BOTTOM_EDGE || (uint8_t)pBall->py + i >= LOWER_TOP_EDGE)
      {
        for(j = 0; j < BALL_WDT; j++)
        {
          g_ucFrameBuf[(uint8_t)pBall->py + i][(uint8_t)pBall->px + j] = VWHITE;
        }
      }
      /* If it's in the logo, replace it */
      else
      {
        for(j = 0; j < BALL_WDT; j++)
        {
          g_ucFrameBuf[(uint8_t)pBall->py + i][(uint8_t)pBall->px + j] = 
          renesaslogo_bmp[(uint8_t)pBall->py - UPPER_BOTTOM_EDGE + i][(uint8_t)pBall->px + j];
        }
      }
    }
    vncBall[vncBlockIndex].x      =  (int)pBall->px;
    vncBall[vncBlockIndex].y      =  (int)pBall->py;
    vncBall[vncBlockIndex].dx     =  BALL_WDT;
    vncBall[vncBlockIndex].dy     =  BALL_HGT;
    vncBall[vncBlockIndex].valid  =  1;
  }
}

/******************************************************************************
* Function Name: draw_Ball
* Description  : Draws ball on the screen and updates VNC struct
* Arguments    : Pointer of which ball to draw, vncBall struct array index
* Return Value : none
******************************************************************************/
void draw_Ball(BALL *pBall, uint8_t vncBlockIndex)
{
  uint8_t i, j;
  
  /* Only update the ball position when the VNC client has been updated with the whole screen */
  if (g_iYLine>=Y_SIZE)
  {
  /* The ball array uses 0x00 to show where the edges are so don't bother drawing them. */
    if (pBall == &theBall)
    {
      /* Draw the swBall (Red to represent the slowness) */
      for (i = 0; i < BALL_HGT; i++)
      {
        for(j = 0; j < BALL_WDT; j++)
        {
          if (ball_template[i][j])
          {
            g_ucFrameBuf[(uint8_t)pBall->y + i][(uint8_t)pBall->x + j] = VRED;
          }
        }
      }
    }
    else if (pBall == &fpuBall)
    {
      /* Draw the fpuBall (Green to represent the speed) */
      for (i = 0; i < BALL_HGT; i++)
      {
        for(j = 0; j < BALL_WDT; j++)
        {
          if (ball_template[i][j])
          {
            g_ucFrameBuf[(uint8_t)pBall->y + i][(uint8_t)pBall->x + j] = VGREEN;
          }
        }
      }
    }
    
    /* Update VNC */
    vncBall[vncBlockIndex].x     =  (int)pBall->x;
    vncBall[vncBlockIndex].y     =  (int)pBall->y;
    vncBall[vncBlockIndex].dx    =  BALL_WDT;
    vncBall[vncBlockIndex].dy    =  BALL_HGT;
    vncBall[vncBlockIndex].valid =  1;
  }
}

/******************************************************************************
* Function Name: bounce_fpuBall
* Description  : FPU Ball Thread - dedicated to boucing the FPU ball
* Arguments    : none
* Return Value : none
******************************************************************************/

void bounce_fpuBall(void *data)
{
  int i;
  for(i = 1;i < 1000000;i++) asm("nop");
  while (1)
  {
    fpu_ballUpdatePosition(&fpuBall);
    fpuOps++;
  }
}

/******************************************************************************
* Function Name: bounce_theBall
* Description  : Software Ball Thread - dedicated to boucing the software ball
* Arguments    : none
* Return Value : none
******************************************************************************/

void bounce_theBall(void *data)
{
  int i;
  for(i = 1;i < 1500000;i++) asm("nop");
  while (1)
  {
    ballUpdatePosition(&theBall);
    swOps++;
  }
}


/******************************************************************************
* Function Name: randomVelocity
* Description  : To send back a random velocity for the balls
* Arguments    : none
* Return Value : float
******************************************************************************/
float randomVelocity(void)
{
  return (rand()%50 + 20) / 100000.0;
}

/******************************************************************************
* Function Name: randomPositions
* Description  : To send back a random position for the balls to start at
* Arguments    : none
* Return Value : float
******************************************************************************/
float randomPositions(void)
{
  return rand()%210;
}

/******************************************************************************
* Function Name: drawBalls
* Description  : Main "Ball Bouncing" Demo Thread
* Arguments    : none
* Return Value : none
******************************************************************************/
void drawBalls(void *data)
{
  /* Set intial location & velocity of two balls for demo */
  fpu_ballInit
  (
    &fpuBall,
    randomPositions(),
    randomPositions(),
    randomVelocity(),
    randomVelocity()
  );
  ballInit
  (
    &theBall,
    randomPositions(),
    randomPositions(),
    fpuBall.xv,
    fpuBall.yv
  );
  
  while (1)
  {
    /* Disable Multi Tasking while we update values */
    disable_tasktimertick();
    
    refresh_Ball(&theBall);
    refresh_Ball(&fpuBall);
    
    speedup = (float) (fpuOps) / (float) (swOps);
    
    /* Keep running totals for long-term averaging */
    totalSwOps = totalSwOps + swOps;
    totalFpuOps = totalFpuOps + fpuOps;
    
    swOps = 0;  /* Clear # of SW moves */
    fpuOps = 0;  /* Clear # of HW FPU moves */
    
    /* Long term averaging to mitigate the effects of debugger */
    iterations++ ;
    if (iterations == 100)
    {
      totalSpeedup = (float) (totalFpuOps) / (float) (totalSwOps);
      iterations = 0;
      totalFpuOps = totalSwOps = 0;
    }

    /* Reenable Multi Tasking */
    enable_tasktimertick();
    /* Give up the rest of our time slice */
    mt_yield();
  }
}

/******************************************************************************
* Function Name: adjust_speed
* Description  : adjusts speed of the bouncing balls
* Arguments    : 
*   adjust: +1 to increment speed
*           -1 to decrement speed
* Return Value : none
******************************************************************************/

void adjust_speed(int adjust)
{
  disable_tasktimertick();
  if (adjust > 0)
  {
     /* FPU x */
     if (fpuBall.xv>0)
     {
       fpuBall.xv += 0.00002;
     }
     else
     if (fpuBall.xv<0)
     {
       fpuBall.xv -= 0.00002;
     }
     /* FPU y */
     if (fpuBall.yv>0)
     {
       fpuBall.yv += 0.00002;
     }
     else
     if (fpuBall.yv<0)
     {
       fpuBall.yv -= 0.00002;
     }
     /* INT x */
     if (theBall.xv>0)
     {
       theBall.xv += 0.00002;
     }
     else
     if (theBall.xv<0)
     {
       theBall.xv -= 0.00002;
     }
     /* INT y */
     if (theBall.yv>0)
     {
       theBall.yv += 0.00002;
     }
     else
     if (theBall.yv<0)
     {
       theBall.yv -= 0.00002;
     }
  }
  else
  if (adjust < 0)
  {
     /* FPU x */
     if (fpuBall.xv>0.00002)
     {
       fpuBall.xv -= 0.00002;
     }
     else
     if (fpuBall.xv<-0.00002)
     {
       fpuBall.xv += 0.00002;
     }
     /* FPU y */
     if (fpuBall.yv>0.00002)
     {
       fpuBall.yv -= 0.00002;
     }
     else
     if (fpuBall.yv<-0.00002)
     {
       fpuBall.yv += 0.00002;
     }
     /* INT x */
     if (theBall.xv>0.00002)
     {
       theBall.xv -= 0.00002;
     }
     else
     if (theBall.xv<-0.00002)
     {
       theBall.xv += 0.00002;
     }
     /* INT y */
     if (theBall.yv>0.00002)
     {
       theBall.yv -= 0.00002;
     }
     else
     if (theBall.yv<-0.00002)
     {
       theBall.yv += 0.00002;
     }
  }
  else
  {
     fpuBall.xv = 0.00020;
     fpuBall.yv = 0.00020;
     theBall.xv = 0.00020;
     theBall.yv = 0.00020;
  }
  enable_tasktimertick();
}

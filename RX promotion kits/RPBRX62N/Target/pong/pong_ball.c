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
* File Name     : Ball_Pong.c
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RX-Stick
* Description   : Storage for ball functions used with Pong demo
* Operation     :
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 06.10.2010  1.00    First release
*******************************************************************************/

/******************************************************************************
 Include files
******************************************************************************/
#include <stdlib.h>
#include "ball.h"
#include "screen.h"
#include "pong.h"
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
/* FPU Ball for each frame buffer */
PONGBALL pongBall;

/******************************************************************************
 Private (static) variables and functions
******************************************************************************/
/* Used to set the speed of the Ball*/
int hitCount=0;
/* Random speed */
float ranSpeed=0;
/* FPU ball */
//PONGBALL pongBall;


/******************************************************************************
* Function name : pong_ballInit
* Description   : Set the initial X, Y, X velocity and Y velocity of a ball
* Arguments     : BALL *ball - pointer to ball to be initialized
*                 int16_t x - initial X position
*                 int16_t y - initial Y position
*                 int16_t xv - initial X velocity
*                 int16_t yv - initial Y velocity
*          uint8_t dir - direction ball is travelling
* Return Value  : none - the ball is initialized
******************************************************************************/
void initPongBall(PONGBALL *ball, int16_t x, int16_t y, int16_t xv, int16_t yv, uint8_t dir)
{
  ball->x = x;
  ball->y = y;
  
  if (ball->px == 0 && ball->px == 0)
  {
    ball->px = x;
    ball->py = y;
  }
  
  ball->dir = dir;
  
    
  /* Add an element of randomness to the initial Y velocity */
  yv += randomSpeed();
  
  /* Make sure the correct values correspond to the direction */
  if (dir == DOWN_RIGHT)
  {
    if (xv < 0)
    {
      ball->xv = -xv;
    }
    else
    {
      ball->xv = xv;
    }
    if (yv < 0)
    {
      ball->yv = -yv;
    }
    else
    {
      ball->yv = yv;
    }
  }
  if (dir == UP_RIGHT)
  {
    if (xv < 0)
    {
      ball->xv = -xv;
    }
    else
    {
      ball->xv = xv;
    }
    if (yv > 0)
    {
      ball->yv = -yv;
    }
    else
    {
      ball->yv = yv;
    }
  }
  if (dir == DOWN_LEFT)
  {
    if (xv > 0)
    {
      ball->xv = -xv;
    }
    else
    {
      ball->xv = xv;
    }
    if (yv < 0)
    {
      ball->yv = -yv;
    }
    else
    {
      ball->yv = yv;
    }
  }
  if (dir == UP_LEFT)
  {
    if (xv > 0)
    {
      ball->xv = -xv;
    }
    else
    {
      ball->xv = xv;
    }
    if (yv > 0)
    {
      ball->yv = -yv;
    }
    else
    {
      ball->yv = yv;
    }
  }
}

/******************************************************************************
* Function name : clearPongBall
* Description   : Removes the ball from its previous position
* Arguments     : BALL *ball - pointer to ball to move
* Return Value  : none
******************************************************************************/
void clearPongBall(PONGBALL *ball)
{
  uint8_t i, j;
  /* Remove the ball */
  for (i = 0; i < BALL_HGT; i++)
  {
    if (BOARD_CENTRE == (ball->py + i))
    {
      for(j = 0; j < BALL_WDT; j++)
      {
        g_ucFrameBuf[ball->py + i][ball->px + j] = CENTER_COL;
      }
    }
    else
    {
      for(j = 0; j < BALL_WDT; j++)
      {
        if (g_ucFrameBuf[ball->py + i][ball->px + j] == BALL_COL)
        {
          g_ucFrameBuf[ball->py + i][ball->px + j] = PONG_BG;
        }
      }
    }
  }
  /* Send update update to VNC server */
  updateVNC
  (
    ball->px,
    ball->py,
    BALL_WDT,
    BALL_HGT,
    VNC_BALL_CLEAR
  );

  ball->px = ball->x;
  ball->py = ball->y;
}

/******************************************************************************
* Function name : drawPongBall
* Description   : Updates the balls position
* Arguments     : BALL *ball - pointer to ball to move
* Return Value  : none - the ball's position is updated
******************************************************************************/
void drawPongBall(PONGBALL *ball)
{
  uint8_t i, j;
  uint16_t vnc_x = 0;
  uint16_t vnc_y = 0;  
  uint16_t vnc_w = 0;
  uint16_t vnc_h = 0;
  
  /* Safety checks*/
  if (ball->px < 0)
  {
    ball->px = 0;
  }
  if (ball->py < 0)
  {
    ball->py = 0;
  }
  if (ball->px >= X_SIZE - BALL_WDT)
  {
    ball->px = X_SIZE - BALL_WDT - 1;
  }
  if (ball->py >= Y_SIZE - BALL_HGT)
  {
    ball->py = Y_SIZE - BALL_HGT - 1;
  }
  
  if (ball->x < 0)
  {
    ball->x = 0;
  }
  if (ball->y < 0)
  {
    ball->y = 0;
  }
  if (ball->x >= X_SIZE - BALL_WDT)
  {
    ball->x = X_SIZE - BALL_WDT - 1;
  }
  if (ball->y >= Y_SIZE - BALL_HGT)
  {
    ball->y = Y_SIZE - BALL_HGT - 1;
  }
  
  /* Find which X value is the least */
  if (ball->px == ball->x)
  {
    vnc_x = ball->x;
    vnc_w = BALL_WDT;
  }
  else if (ball->px < ball->x)
  {
    vnc_x = ball->px;
    vnc_w = ball->x - vnc_x + BALL_WDT;
  }
  else
  {
    vnc_x = ball->x;
    vnc_w = ball->px - vnc_x + BALL_WDT;
  }
  
  /* Find which Y value is the least */
  if (ball->py == ball->y)
  {
    vnc_y = ball->y;
    vnc_h = BALL_HGT;
  }
  else if (ball->py < ball->y)
  {
    vnc_y = ball->py;
    vnc_h = ball->y - vnc_y + BALL_HGT;
  }
  else
  {
    vnc_y = ball->y;
    vnc_h = ball->py - vnc_y + BALL_HGT;
  }
  
  /* Remove the ball */
  for (i = 0; i < BALL_HGT; i++)
  {
    for(j = 0; j < BALL_WDT; j++)
    {
      if (g_ucFrameBuf[ball->py + i][ball->px + j] == BALL_COL)
      {
        g_ucFrameBuf[ball->py + i][ball->px + j] = PONG_BG;
      }
    }
  }

  /* Draw the ball */
  for (i = 0; i < BALL_HGT; i++)
  {
    for(j = 0; j < BALL_WDT; j++)
    {
      if (ball_template[i][j] &&
        g_ucFrameBuf[ball->y + i][ball->x + j] == PONG_BG &&
         g_ucFrameBuf[ball->y + i][ball->x + j] != CENTER_COL &&
         g_ucFrameBuf[ball->y + i][ball->x + j] != PADDLE_COL)
      {
        g_ucFrameBuf[ball->y + i][ball->x + j] = BALL_COL;
      }
    }
  }
  
  /* Update the VNC server by drawing the pixels that have changed */
  updateVNC
  (
    vnc_x,
    vnc_y,
    vnc_w,
    vnc_h,
    VNC_BALL_DRAW
  );
  
  /* Save current values of X and Y as 'previous' */
  ball->px = ball->x;
  ball->py = ball->y;
}

/******************************************************************************
* Function Name: updatePongBallPosition
* Description  : Updates the balls position
* Arguments    : BALL *ball - pointer to ball to move
* Return Value : none
******************************************************************************/
void updatePongBallPosition(PONGBALL *ball)
{
  /* Set the speed according to the amout of hits */
  if(hitCount%4 == 0 && hitCount != 0)
  {
    hitCount++;
    
    if (ball->xv < BALL_VEL_LIMIT && ball->xv > -BALL_VEL_LIMIT)
    {
      /* Discover if X velocity is +/- i.e. right/left */
      /* + = right */
      if (ball->xv > 0)
      {/* Cap velocity at 14 */
        if (ball->xv <= BALL_VEL_LIMIT)
        {
          ball->xv += randomSpeed();
          if (ball->xv >= BALL_VEL_LIMIT)
          {
            ball->xv = BALL_VEL_LIMIT;
          }
        }
      }
      else
      {/* Cap velocity at -14 */
        if (ball->xv >= -BALL_VEL_LIMIT)
        {
          ball->xv -= randomSpeed();
          if (ball->xv <= -BALL_VEL_LIMIT)
          {
            ball->xv = -BALL_VEL_LIMIT;
          }
        }
      }
    }
  }
  
  /* MOVE THAT BALL */
  ball->y += ball->yv ; 
  ball->x += ball->xv ;
  
  /* If the ball has gone past the paddle */
  if (ball->x <= LEFT_LIMIT)
  {
    /* Increase Player 2 score */
    p2_score++;
    
    /* Start a new match with the ball on the P1 paddle */
    initPongBall
    (
      &pongBall,
      PADDLE_P1_EDGE,
      BALL_P1_Y_INIT,
      BALL_VEL_INIT_X,
      BALL_VEL_INIT_Y,
      randomDirection_P1()
    );
    clearPongBall(&pongBall);
    drawPongBall(&pongBall);
    /* Update the scores */
    updateScores(2, p2_score);
    g_Scored = 1;
    /* Reset the hit count */
    hitCount = 0;
    return;
  }
  /* If the ball has gone past the paddle */
  if (ball->x >= RIGHT_LIMIT)
  {
    /* Increase Player 1 score */
    p1_score++;
    /* Start a new match with the match ball on the P2 paddle */
    initPongBall
    (
      &pongBall,
      PADDLE_P2_EDGE,
      BALL_P2_Y_INIT,
      BALL_VEL_INIT_X,
      BALL_VEL_INIT_Y,
      randomDirection_P2()
    );
    clearPongBall(&pongBall);
    drawPongBall(&pongBall);
    /* Update the scores */
    updateScores(1, p1_score);
    g_Scored = 2;
    /* Reset the hit count */
    hitCount = 0;
    return;
  }
  
  
  /* Check for ceiling & floor bounces */
  if (ball->y <= TOP_LIMIT)
  {
    /* Reverse the Y direction of the ball */
    ball->yv = -ball->yv;
    ball->y = TOP_LIMIT;
    /* Update the direction variable */
    if (ball->dir == UP_RIGHT)
    {
      ball->dir = DOWN_RIGHT;
    }
    else if (ball->dir == UP_LEFT)
    {
      ball->dir = DOWN_LEFT;
    }
    return;
  }
  else if (ball->y >= BOTTOM_LIMIT)
  {
    /* Reverse the Y direction of the ball */
    ball->yv = -ball->yv;
    ball->y = BOTTOM_LIMIT;
    /* Update the direction variable */
    if (ball->dir == DOWN_RIGHT)
    {
      ball->dir = UP_RIGHT;
    }
    else if (ball->dir == DOWN_LEFT)
    {
      ball->dir = UP_LEFT;
    }
    return;
  }

  /****
   * Paddle 1
   ****/
   
  /* If the ball is on the right edge of the paddle */
  if (ball->x <= PADDLE_P1_X + PADDLE_WDT)
  {
    /* If the ball is between the top & bottom i.e. hitting the face */
    if (ball->y + BALL_HGT > Paddle_P1.y &&
      ball->y < Paddle_P1.y + PADDLE_HGT &&
      ball->x + BALL_WDT > Paddle_P1.x + PADDLE_WDT &&
      ball->x > Paddle_P1.x)
    {
      ball->x = PADDLE_P1_X + PADDLE_WDT;
      /*Respond to the direction the ball is traviling*/
      switch(ball->dir)
      {
        case DOWN_LEFT:
          /* Change the variable 'direction' of the ball */
          ball->dir = DOWN_RIGHT;
          
          /* Determine how much Y velocity to increase/decrease */
          switch(Paddle_P1.dir)
          {
            case UP:
              /* Only decrease velocity if its above the min */
              if (ball->yv > BALL_VEL_ADD)
              {
                ball->yv -= BALL_VEL_ADD;
                if (ball->yv < BALL_VEL_ADD)
                {/* If it's under the min, set it to the min */
                  ball->yv = BALL_VEL_ADD;
                }
              }
              else
              {
                ball->yv = -BALL_VEL_ADD;
              }
              break;
            case DOWN:
              /* Increase the velocity */
              ball->yv += BALL_VEL_ADD;
              break;
            default:
              break;
          }
          break;
        case UP_LEFT:
          /* Change the variable 'direction' of the ball */
          ball->dir = UP_RIGHT;
          
          /* Determine how much Y velocity to increase/decrease */
          /* Bear in mind that if the ball is travelling up,
           *  yv will be negative */
          switch(Paddle_P1.dir)
          {
            case UP:
              /* Increase the velocity */
              ball->yv -= BALL_VEL_ADD;
              break;
            case DOWN:
              if (ball->yv < -BALL_VEL_ADD)
              {
                ball->yv += BALL_VEL_ADD;
                if (ball->yv > -BALL_VEL_ADD)
                {
                  ball->yv = -BALL_VEL_ADD;
                }
              }
              else
              {
                ball->yv = -BALL_VEL_ADD;
              }
              break;
            default:
              break;
          }
          break;
      }
      /* Increase the number in the hit count */
      hitCount++;
      /* Reverse X direction of the ball */
      ball->xv = -ball->xv;
    
    }
    /* If the ball is hitting the top of the paddle */
    if (ball->dir == DOWN_LEFT &&
      ball->y + BALL_HGT >= Paddle_P1.y &&
      ball->y < Paddle_P1.y &&
      ball->x + BALL_WDT >= Paddle_P1.x)
    {
      ball->y = Paddle_P1.y - BALL_HGT;
      ball->dir = UP_LEFT;
      if (ball->xv > 0)
      {
        ball->xv = -ball->xv;
      }
      if (ball->yv > 0)
      {
        ball->yv = -ball->yv;
      }
    }
    /* If the ball is hitting the bottom of the paddle */
    if (ball->dir == UP_LEFT &&
      ball->y <= Paddle_P1.y + PADDLE_HGT &&
      ball->y + BALL_HGT > Paddle_P1.y + PADDLE_HGT &&
      ball->x + BALL_WDT >= Paddle_P1.x)
    {
      ball->y = Paddle_P1.y + PADDLE_HGT;
      ball->dir = DOWN_LEFT;
      if (ball->xv > 0)
      {
        ball->xv = -ball->xv;
      }
      if (ball->yv < 0)
      {
        ball->yv = -ball->yv;
      }
    }
  }
  
  /****
   * Paddle 2
   ****/

  /* If the ball is the same X as the paddle */
  if (ball->x + BALL_WDT >= PADDLE_P2_X)
  {  
    /* If the ball is between the top & bottom i.e. hitting the face */
    if (ball->y + BALL_HGT > Paddle_P2.y &&
      ball->y < Paddle_P2.y + PADDLE_HGT &&
      ball->x < Paddle_P2.x + PADDLE_WDT &&
      ball->x + BALL_WDT < Paddle_P2.x + PADDLE_WDT)
    {
      ball->x = PADDLE_P2_X - BALL_WDT;
      /*Respond to the direction the ball is traviling*/
      switch(ball->dir)
      {
        case DOWN_RIGHT:
          /* Change the variable 'direction' of the ball */
          ball->dir = DOWN_LEFT;
        
          switch(Paddle_P2.dir)
          {
            case UP:
              /* Only decrease velocity if its above the min */
              if (ball->yv > BALL_VEL_ADD)
              {
                ball->yv -= BALL_VEL_ADD;
                if (ball->yv < BALL_VEL_ADD)
                {/* If it's under the min, set it to the min */
                  ball->yv = BALL_VEL_ADD;
                }
              }
              else
              {
                ball->yv = -BALL_VEL_ADD;
              }
              break;
            case DOWN:
              /* Increase the velocity */
              ball->yv += BALL_VEL_ADD;
              break;
            default:
              break;
          }
          break;
        case UP_RIGHT:
          /* Change the variable 'direction' of the ball */
          ball->dir = UP_LEFT;
        
          /* Determine how much Y velocity to increase/decrease */
          /* Bear in mind that if the ball is travelling up,
           *  yv will be negative */
          switch(Paddle_P2.dir)
          {
            case UP:
              /* Increase the velocity */
              ball->yv -= BALL_VEL_ADD;
              break;
            case DOWN:
              /* Only decrease velocity if its above the min */
              if (ball->yv < -BALL_VEL_ADD)
              {
                ball->yv += BALL_VEL_ADD;
                if (ball->yv > -BALL_VEL_ADD)
                {/* If it's under the min, set it to the min */
                  ball->yv = BALL_VEL_ADD;
                }
              }
              else
              {
                ball->yv = -BALL_VEL_ADD;
              }
              break;
            default:
              break;
          }
          break;
      }
      /* Increase the number in the hit count */
      hitCount++;
      /* Reverse X direction of the ball */
      ball->xv = -ball->xv;
    }
    /* If the ball is hitting the top of the paddle */
    if (ball->dir == DOWN_RIGHT &&
      ball->y + BALL_HGT >= Paddle_P2.y &&
      ball->y < Paddle_P2.y &&
      ball->x <= Paddle_P2.x + PADDLE_WDT)
    {
      ball->y = Paddle_P2.y - BALL_HGT;
      ball->dir = UP_RIGHT;
      if (ball->xv < 0)
      {
        ball->xv = -ball->xv;
      }
      if (ball->yv > 0)
      {
        ball->yv = -ball->yv;
      }
    }
    /* If the ball is hitting the bottom of the paddle */
    if (ball->dir == UP_RIGHT &&
      ball->y <= Paddle_P2.y + PADDLE_HGT &&
      ball->y + BALL_HGT > Paddle_P2.y + PADDLE_HGT &&
      ball->x <= Paddle_P2.x + PADDLE_WDT)
    {
      ball->y = Paddle_P2.y + PADDLE_HGT;
      ball->dir = DOWN_RIGHT;
      if (ball->xv < 0)
      {
        ball->xv = -ball->xv;
      }
      if (ball->yv < 0)
      {
        ball->yv = -ball->yv;
      }
    }
  }
}

/******************************************************************************
* Function Name: randomSeed
* Description  : Seed random number generation with the timer tick
* Arguments    : none
* Return Value : none
******************************************************************************/
void randomSeed(void)
{
  srand(timer_clock_ticks);
}

/******************************************************************************
* Function Name: randomDirection_P1
* Description  : To send back a random direction for the ball for player 1
* Arguments    : none
* Return Value : uint8_t
******************************************************************************/
uint8_t randomDirection_P1(void)
{
  return rand()%2;
}

/*****************************************************************************
* Function Name: randomDirection_P2
* Description  : To send back a random direction for the ball for player 2
* Arguments    : none
* Return Value : uint8_t
******************************************************************************/
uint8_t randomDirection_P2(void)
{
  return 2 + rand()%2;
}

/******************************************************************************
* Function Name: randomSpeed
* Description  : Send back a random direction for the ball
* Arguments    : none
* Return Value : uint8_t
******************************************************************************/
uint8_t randomSpeed(void)
{
     return rand()%3;
}

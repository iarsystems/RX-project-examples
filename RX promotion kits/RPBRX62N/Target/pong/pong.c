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
* File Name    : pong.c
* Version      : 1.00
* Description  : Demonstration of the RX62N Joystick and FPU
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.10.2010 1.00    First Release
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/ 
#include "screen.h"
#include "pong.h"
#include "mt.h"

/* IO Port RPDL function defintions */
#include "r_pdl_io_port.h"


/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* Joystick information */
extern uint8_t jCenter, jRight, jDown, jLeft, jUp;
extern bool gJoyActive;

/* VNC block sent variable */
extern int g_iBlockSent;
/******************************************************************************
 Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
 Private variables and constants
******************************************************************************/
/* Game variables */
uint8_t p1_score;  /* Player 1 score */
uint8_t p2_score;  /* Player 2 score */
uint8_t g_Scored;  /* Flag to indicate update of the board */

/******************************************************************************
* Function Name: pongInit
* Description  : Initialises the pong functionality
*         
* Arguments    : none
* Return Value : none
******************************************************************************/
void initPong(void)
{
  /* Reset the board so player 1 has the ball */
  g_Scored = 1;
  p1_score = 0;  /* Player 1 score */
  p2_score = 0;  /* Player 2 score */
  /* Initialise the background */
  initPongBackground();
  /* Initialise the paddles for display */
  initPaddles();
  /* Initialise the score board */
  initScores();
  /* Initialise the ball in each frame*/
  initPongBall
  (
    &pongBall,
    PADDLE_P1_EDGE,
    BALL_P1_Y_INIT,
    BALL_VEL_INIT_X,
    BALL_VEL_INIT_Y,
    randomDirection_P1()
  );
  drawPongBall(&pongBall);
}
/******************************************************************************
* Function Name: pong
* Description  : 
*         
* Arguments    : none
* Return Value : none
******************************************************************************/
void pong(void *data)
{
  while (1)
  {
    /* Disable Multi Tasking while we update values */
    disable_tasktimertick();
    
    /* Only begin updating the ball position if the user has pressed the joystick */
    if (!g_Scored)
    {
      updatePongBallPosition(&pongBall);
      drawPongBall(&pongBall);
    }
    
    if (gJoyActive)
    {
      
      /* Variables are 0 when pressed and 1 when free */
      if (!jUp)
      {
        Paddle_P1.dir = UP;
        Paddle_P2.dir = DOWN;
      }
      if (!jDown)
      {
        Paddle_P1.dir = DOWN;
        Paddle_P2.dir = UP;
      }
      /* Refresh the relevant paddle */
      refreshPaddle(&Paddle_P1);
      refreshPaddle(&Paddle_P2);
      
      /* If the joystick hasn't been pressed and the ball hasn't been launched
        then keep the ball in the centre of the paddle that has it */
      if (jCenter && g_Scored)
      {
        if (g_Scored == 1)
        {
          pongBall.py = pongBall.y;
          pongBall.y = BALL_P1_Y_INIT;
        }
        else if (g_Scored == 2)
        {
          pongBall.py = pongBall.y;
          pongBall.y = BALL_P2_Y_INIT;
        }
        
        drawPongBall(&pongBall);
      }
      /* When we press the center of the joystick then launch the ball */
      if (!jCenter)
      {
        g_Scored = 0;
      }
    }
    
    /* Reset global variable.
     * It will be reactived next time the joystick is used */
    if (jUp && jDown && jCenter)
    {
      gJoyActive = false;
      Paddle_P1.dir = STILL;
      Paddle_P2.dir = STILL;
    }
    
    /* Reenable Multi Tasking */
    enable_tasktimertick();
    
    /* Wait here until VNC has finished sending */
    g_iBlockSent = 0;
    while (!g_iBlockSent)
    {
      /* Give up the rest of our time slice */
      mt_yield();
    }
  }
}

/******************************************************************************
* Function Name: initPongBackground
* Description  : Draws both frame buffers with Pong board and edges.
* Arguments    : none
* Return Value : none
******************************************************************************/
void initPongBackground(void)
{
  uint8_t i, j;
  
  /* Draw the screen, with white through the centre of the board */
  for (i = 0; i < Y_SIZE; i++)
  {
    for(j = 0; j < X_SIZE; j++)
    {
      g_ucFrameBuf[i][j] = PONG_BG;
    }
  }
  for (i = 0; i < Y_SIZE; i++)
  {
    g_ucFrameBuf[i][BOARD_CENTRE] = CENTER_COL;
  }
  for (i = 0; i < X_SIZE; i++)
  {
    g_ucFrameBuf[BOARD_CENTRE][i] = CENTER_COL;
  }
}

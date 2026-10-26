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
* File Name     : pong.h
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RX62N
* Description   : Header file for pong.c
* Operation     :
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 06.10.2010  1.00    First release
*******************************************************************************/
#ifndef _PONG_H_
#define _PONG_H_

/* For the types uint8_t and uint16_t */
#include "stdint.h"
/* For the screen size */
#include "screen.h"

/* Ball Directions */
#define DOWN_RIGHT   0
#define UP_RIGHT     1
#define DOWN_LEFT    2
#define UP_LEFT      3

/* Ball */
#define BALL_HGT   10
#define BALL_WDT   10

/* Increment of velocity added or subtracted from the ball when it hits paddle */
#define BALL_VEL_ADD     1
#define BALL_VEL_LIMIT     14

/* Initial velocity of the ball */
#define BALL_VEL_INIT_X   4
#define BALL_VEL_INIT_Y   1

/* Init ball half way down the face of a paddle */
#define BALL_P1_Y_INIT     (Paddle_P1.y + (PADDLE_HGT - BALL_HGT) / 2)
#define BALL_P2_Y_INIT     (Paddle_P2.y + (PADDLE_HGT - BALL_HGT) / 2)

/* Paddle Directions */
#define UP     0
#define DOWN   1
#define STILL   2

/* Paddle */
#define PADDLE_HGT        Y_SIZE / 8
#define PADDLE_WDT         4
#define PADDLE_P1_EDGE       PADDLE_P1_X + PADDLE_WDT
#define PADDLE_P2_EDGE       PADDLE_P2_X - BALL_WDT
#define PADDLE_P1_X       20
#define PADDLE_P2_X       X_SIZE - PADDLE_WDT - PADDLE_P1_X
#define PADDLE_Y_START      (uint8_t)((Y_SIZE - PADDLE_HGT) / 2 )

/* Colours */
#if RGB233VNC

#define PONG_BG   0x0A
#define PADDLE_COL  0x0B
#define BALL_COL   WHITE
#define CENTER_COL  0xFE
#define SCORE_COL   0xFE

#else

#define PONG_BG   VDARKGREEN
#define PADDLE_COL   VRED
#define BALL_COL   VALMOSTWHITE
#define CENTER_COL  VWHITE
#define SCORE_COL   VWHITE

#endif

/* Score board positions */
#define P1_SCORE_DIGIT1_X   ((X_SIZE / 2) - (X_SIZE / 4) - SCORE_DIGIT_WDT - 1)
#define P1_SCORE_DIGIT2_X   (P1_SCORE_DIGIT1_X + SCORE_DIGIT_WDT + 2)
#define P2_SCORE_DIGIT1_X   ((X_SIZE / 2) + (X_SIZE / 4) - SCORE_DIGIT_WDT - 1)
#define P2_SCORE_DIGIT2_X   (P2_SCORE_DIGIT1_X + SCORE_DIGIT_WDT + 2)
#define SCORE_DIGIT_Y     10
#define SCORE_DIGIT_HGT   7
#define SCORE_DIGIT_WDT   5
#define SCORE_WDT       (2 * SCORE_DIGIT_WDT) + 2

/* Increment of vertical distance of paddles */
#define VERT_DISTANCE   6

/* Draw_Object array definitions */
#define VNC_PAD_DRAW_1  0
#define VNC_PAD_DRAW_2  1
#define VNC_SCORE_1    2
#define VNC_SCORE_2    3
#define VNC_BALL_DRAW  4
#define VNC_BALL_CLEAR  5

/* Type definitions */
typedef struct {
  uint8_t x;    /* Paddle X location */
  uint8_t y;    /* Paddle Y location */
  uint8_t dir;    /* Direction of motion in Y axis */
} PADDLE;

typedef struct {
  int16_t x;        /* Ball X location */
  int16_t y;        /* Ball Y location */
  int16_t px;       /* Previous Ball X location */
  int16_t py;       /* Previous Ball Y location */
  int16_t xv;       /* Ball X velocity */
  int16_t yv;    /* Ball Y velocity */
  uint8_t dir;    /* Ball Direction */
} PONGBALL ;

/* Public functions */
void pong(void *data);
void pongBallThread(void *data);

void initPong(void);

void initPongBackground(void);
void initScores(void);
void updateScores(int nPlayer, int nScore);

void initPaddles(void);
void clearPaddles(void);
void refreshPaddle(PADDLE *paddle);

void initPongBall(PONGBALL *ball, int16_t x, int16_t y, int16_t xv, int16_t yv, uint8_t dir);
void clearPongBall(PONGBALL *ball);
void drawPongBall(PONGBALL *ball);
void updatePongBallPosition(PONGBALL *ball);

void randomSeed(void);
uint8_t randomSpeed(void);
uint8_t randomDirection_P1(void);
uint8_t randomDirection_P2(void);


/* Number bitmaps */
extern const unsigned char number_0_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];
extern const unsigned char number_1_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];
extern const unsigned char number_2_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];
extern const unsigned char number_3_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];
extern const unsigned char number_4_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];
extern const unsigned char number_5_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];
extern const unsigned char number_6_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];
extern const unsigned char number_7_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];
extern const unsigned char number_8_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];
extern const unsigned char number_9_bmp[SCORE_DIGIT_HGT][SCORE_DIGIT_WDT];


/* 1 struct for each players paddle in each frame buffer */
extern PADDLE Paddle_P1;
extern PADDLE Paddle_P2;

/* FPU Ball */
extern PONGBALL pongBall;

/* Scores */
extern uint8_t p1_score;
extern uint8_t p2_score;
extern uint8_t g_Scored;

/* Random seed */
extern clock_time_t timer_clock_ticks;


#endif

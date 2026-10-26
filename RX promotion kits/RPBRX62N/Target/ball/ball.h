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
* File Name     : ball.h
* Version       : 1.0
* Device(s)     : R5F56108VNFP
* Tool-Chain    : RX Family C Compiler
* OS            : None
* H/W Platform  : RX62N
* Description   : Header file for ball.c and FPU_ball.c
* Operation     :
* Limitations   : None
*******************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 14.09.2010  1.00    First release
*******************************************************************************/
#ifndef _BALL_H_
#define _BALL_H_

/* Ball size */
#define BALL_HGT       10
#define BALL_WDT       10

/* For the types uint8_t and uint16_t */
#include "stdint.h"
/* For the screen sizes */
#include "screen.h"

/* Screen boundaries for SW (bottom) ball */
#define SW_LEFT_LIMIT     0
#define SW_RIGHT_LIMIT    ((float)(X_SIZE-BALL_WDT))
#define SW_TOP_LIMIT     0
#define SW_BOTTOM_LIMIT    ((float)(Y_SIZE-BALL_HGT))

/* Screen boundaries for FPU (top) ball */
#define LEFT_LIMIT        0
#define RIGHT_LIMIT      ((float)(X_SIZE-BALL_WDT))
#define TOP_LIMIT       0
#define BOTTOM_LIMIT    ((float)(Y_SIZE-BALL_HGT))

/* Draw_Object array definitions */
#define VNC_SWBALL_DRAW    0
#define VNC_SWBALL_CLEAR  1
#define VNC_FPUBALL_DRAW  2
#define VNC_FPUBALL_CLEAR  3

/* Type definitions */
typedef struct {
  float x;      /* Ball X location */
  float y;      /* Ball Y location */
  float px;     /* Previous Ball X location */
  float py;     /* Previous Ball Y location */
  float xv;     /* Ball X velocity */
  float yv;     /* Ball Y velocity */
} BALL ;

/* Public functions */
void ballUpdatePosition (BALL *ball);
void ballInit (BALL *ball, float x, float y, float xv, float yv);
void fpu_ballUpdatePosition (BALL *ball);
void fpu_ballInit (BALL *ball, float x, float y, float xv, float yv);
/* ball_demo.c */
void drawBallBackground(void);
void refresh_Ball(BALL *pBall);
void clear_Ball(BALL *pBall, uint8_t vncIndex);
void draw_Ball(BALL *pBall, uint8_t vncIndex);
void adjust_speed(int adjust);
float randomVelocity(void);
float randomPositions(void);
/* Threads */
void drawBalls(void *data);
void bounce_theBall(void *data);
void bounce_fpuBall(void *data);
void drawBallInitialScreen(void);

/* Ball arrays */
extern unsigned char ball_template[BALL_HGT][BALL_WDT];
extern BALL theBall;
extern BALL fpuBall;

/* Number of Operations variables */
extern unsigned long int fpuOps;        /* Number of HW floating point ops/interval */
extern unsigned long int swOps;         /* Number of SW floating point ops/interval */
extern unsigned long int totalFpuOps;   /* Total number of HW floating point ops over many loops */
extern unsigned long int totalSwOps;    /* Total number of SW floating point ops over many loops */

/* Speed variables */
extern int iterations;
extern float speedup;
extern float totalSpeedup;

#endif

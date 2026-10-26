/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved. */
/**********************************************************************
* File Name    : mandel.c
* Version     : 1.00
* Device     : Renesas Generic MCU.
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSK Generic
* Description   : Mandelbrot demo.
**********************************************************************/

/**********************************************************************
* History     : 29.09.2010 Ver. 1.00 First Release
**********************************************************************/

#include <stdio.h>
#include <string.h>

#include "screen.h"
#include "mandel.h"
#include "mt.h"
#include "vnc-server.h"
#include "switch.h"

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
// Global variable that is used to communicate with vnc_out: it is cleared
// by the 'sender', mandelbrot, and set by vnc_out when it has processed the
// updated blocks of pixels.
extern int g_iBlockSent;

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/

/******************************************************************************
Private global variables and functions
******************************************************************************/

// Private lobal variable to indicate whether to redraw mandelbrot. We need
// to redraw when the user zooms in or zooms out, and at initialisation.
static int g_iMandelRedraw = 1;

// Private local variable to indicate whether to redraw the cross hairs cursor.
// We need to redraw when the cursor moves left, right, up, or down,
// and after the mandelbrot image has been redrawn.
static int g_iCursorRedraw = 1;

// Private local variable that indicates whether the cross hairs cursor has
// a previous position and that the original pixels need to be put back.
static int g_iPrvCursor    = 0;

// Private local variables that are used to keep the joystick 
// position onthe mandelbrot image as a cross hairs cursor.
static int g_iCurX = X_SIZE/2;  // Cursor X position
static int g_iCurY = Y_SIZE/2;  // Cursor Y position

// Private local variables that are part of mandelbrot.
// They are used to zoom in and out of the mandelbrot image.
static double g_dMandelCx    = 0.0;
static double g_dMandelCy    = 0.0;
static double g_dMandelScale = 0.02;

/******************************************************************************
* Function Name: updateMandelJoystickPosition
* Description  : Called by the joystick interrupt function. It updates 
*                variables relevant to mandelbrot: the cross hairs position
*                and whether to zoom in or out (and redraw the image)                                           
* Arguments    : 
*    uint8_t bCenter - whether joystick center is pressed; 0 means yes
*    uint8_t bRight  - whether joystick right is pressed; 0 means yes
*    uint8_t bDown   - whether joystick down is pressed; 0 means yes
*    uint8_t bLeft   - whether joystick left is pressed; 0 means yes
*    uint8_t bUp     - whether joystick up is pressed; 0 means yes
*    uint8_t bSW1    - whether joystick SW1 is pressed; 0 means yes
* Return Value : none
******************************************************************************/
void updateMandelJoystickPosition(uint8_t _bCenter, 
                  uint8_t _bRight, 
                  uint8_t _bDown, 
                  uint8_t _bLeft, 
                  uint8_t _bUp, 
                  uint8_t _bSW1)
{
  if (!_bRight )
  {
    if (g_iCurX < X_SIZE-1) g_iCurX++;
    g_iCursorRedraw = 1;
  }
  if (!_bLeft  )
  {
    if (g_iCurX >        0) g_iCurX--;
    g_iCursorRedraw = 1;
  }
  if (!_bDown  )
  {
    if (g_iCurY < Y_SIZE-1) g_iCurY++;
    g_iCursorRedraw = 1;
  }
  if (!_bUp    )
  {
    if (g_iCurY >        0) g_iCurY--;
    g_iCursorRedraw = 1;
  }
  if (!_bCenter) 
  {
    g_dMandelCx     = g_dMandelCx + g_dMandelScale*(g_iCurX-X_SIZE/2);
    g_dMandelCy     = g_dMandelCy + g_dMandelScale*(g_iCurY-Y_SIZE/2);
    g_dMandelScale  = g_dMandelScale/2;
  g_iMandelRedraw = 1;
    g_iCursorRedraw = 1;
  }
  if (!_bSW1   )
  {
    g_dMandelCx     = g_dMandelCx + g_dMandelScale*(g_iCurX-X_SIZE/2);
    g_dMandelCy     = g_dMandelCy + g_dMandelScale*(g_iCurY-Y_SIZE/2);
    g_dMandelScale  = g_dMandelScale*2;
  g_iMandelRedraw = 1;
    g_iCursorRedraw = 1;
  }
}

/******************************************************************************
* Function Name: calcMandelbrot
* Description  : Calculates one Mandelbrot image. 
* Arguments    : 
*    int _iUpdateVnc - if 1, send updated blocks to VNC client, if 0
*                      don't send updated blocks to VNC client (this is
*                      for initialisation of the image/screen).
* Return Value : none
******************************************************************************/
static void calcMandelbrot(int _iUpdateVnc)
{
  int    x;
  int    y;
  int    lp;
  int    limit;
  double ax;
  double ay;
  double cx;
  double cy;
  double scale;
  double a1;
  double b1;
  double a2;
  double b2;

  unsigned nLines = 0;
  unsigned k = 0;
  
  /* RealVNC seems to only accept at most 168 raw pixels in one VNC message. */
  /* We pack these in an 8 packet VNC message. We need to calculate how many */
  /* lines we need to get the maximum of out 8x168 pixels:                   */
  /*     n * X_SIZE = 8 * 168                                                */
  /* so  n          = 8 * 168 / X_SIZE                                       */
  nLines = (8 * VNC_MAXPIXELS) / X_SIZE;
  
    // Set up initial values for drawing. Try compiling the program
    // with different values here if you like!}
    cx    = g_dMandelCx;
  cy    = g_dMandelCy;
  scale = g_dMandelScale;
    limit = 4;

    // Loop through all pixels on screen. For reasons that will become
    // clear, I am counting not from (0,0) but from (-160,-100).}
    for (y = -Y_SIZE/2; y < Y_SIZE/2; y++)
  {
      for (x = -X_SIZE/2; x< X_SIZE/2; x++)
    {
        // What is the *mathematical* value of this point?
        ax = cx + x*scale;
    ay = cy + y*scale;

        // And now for the magic formula!
        a1 = ax;
    b1 = ay;
    lp = 0;
        do
    {
          // Do one iteration.
          lp++;
          a2 =   a1*a1 - b1*b1 + ax;
          b2 = 2*a1*b1         + ay;
          // This is indeed the square of a+bi, done component-wise.
          a1 = a2; 
      b1 = b2;
    }
        while ((lp<VWHITE) && ((a1*a1)+(b1*b1)<=limit));
        // The first condition is satisfied if we have convergence.
        // The second is satisfied if we have divergence.

        // Define colour and draw pixel.}
        if (lp >= VWHITE) // white
    {
      lp = VBLACK;     // black
    }
        // If the point converges, it is part of the brot and we
        // draw it with colour 0, or black.
        // pixel(x+160,y+100,lp);
        g_ucFrameBuf[y+Y_SIZE/2][x+X_SIZE/2] = lp&0x3F;
    }

    if (_iUpdateVnc)
    {  
      k++;
      if ((k >= nLines)||((y+Y_SIZE/2) >= (Y_SIZE-1)))
      {
        // Update VNC
        disable_tasktimertick();
        for (unsigned l=0; l<8; l++)
        {
          vncBall[l].x      = (l*X_SIZE)/8;
            vncBall[l].y      = y+Y_SIZE/2-k+1;
            vncBall[l].dx     = X_SIZE/8;
            vncBall[l].dy     = k;
          vncBall[l].valid  = 1;
        }
        enable_tasktimertick();
        // Wait for vnc_out to send the block
         g_iBlockSent=0;
        while (!g_iBlockSent)
        {
        mt_yield();
        }
        k = 0;
      }
    }
  }
}

/******************************************************************************
* Function Name: writeCursor
* Description  : Writes cursor in Mandelbrot image 
* Arguments    : 
* Return Value : none
******************************************************************************/
static void writeCursor(void)
{
  static int prvCurX;
  static int prvCurY;
  static unsigned char cachedY[10];
  static unsigned char cachedX[10];
  
  if (g_iCursorRedraw)
  {
    unsigned x;
    unsigned y;
  
    g_iCursorRedraw = 0;
    
    // Remove previous
    if (g_iPrvCursor)
    {
      x = prvCurX;
      for (y=prvCurY-5; y<prvCurY+5; y++)
      {
           g_ucFrameBuf[y][x] = cachedY[y-prvCurY+5];
      }
      y = prvCurY;
      for (x=prvCurX-5; x<prvCurX+5; x++)
      {
           g_ucFrameBuf[y][x] = cachedX[x-prvCurX+5];
      }
    
      disable_tasktimertick();
      vncBall[1].x      = prvCurX-5;
        vncBall[1].y      = prvCurY-5;
        vncBall[1].dx     = 10;
        vncBall[1].dy     = 10;
      vncBall[1].valid  = 1;
      enable_tasktimertick();
    }

    // Cache pixels we're about to overwrite    
    x = g_iCurX;
    for (y=g_iCurY-5; y<g_iCurY+5; y++)
    {
         cachedY[y-g_iCurY+5] = g_ucFrameBuf[y][x];
    }
    y = g_iCurY;
    for (x=g_iCurX-5; x<g_iCurX+5; x++)
    {
         cachedX[x-g_iCurX+5] = g_ucFrameBuf[y][x];
    }
    
    // Create new
    x = g_iCurX;
    for (y=g_iCurY-5; y<g_iCurY+5; y++)
    {
         g_ucFrameBuf[y][x]  = VWHITE;
    }
    y = g_iCurY;
    for (x=g_iCurX-5; x<g_iCurX+5; x++)
    {
         g_ucFrameBuf[y][x]  = VWHITE;
    }
    
    prvCurX      = g_iCurX;
    prvCurY      = g_iCurY;
    g_iPrvCursor = 1;
  }
  
  // Update VNC
  disable_tasktimertick();
  vncBall[0].x      = g_iCurX-5;
    vncBall[0].y      = g_iCurY-5;
    vncBall[0].dx     = 10;
    vncBall[0].dy     = 10;
  vncBall[0].valid  = 1;
  enable_tasktimertick();
  // Wait for vnc_out to send the block
   g_iBlockSent=0;
  while (!g_iBlockSent)
  {
    mt_yield();
  }
}

/******************************************************************************
* Function Name: mandelbrot
* Description  : Function that is the body of the Mandelbrot thread
* Arguments    : 
*     void *data - default parameter for a thread (not used here)
* Return Value : none
******************************************************************************/
void mandelbrot(void *data)
{
   while (1)
   {
   // g_iMandelRedraw is set to 1 if the joystick center is pressed.
   // Redraw the mandelbrot image with half the current scale. Reset
   // the cursor (cross hairs) to the middle of the image. Redraw 
   // the cursor.
   if (g_iMandelRedraw)
   {
    g_iMandelRedraw = 0;
    g_iPrvCursor    = 0;
    g_iCurX         = X_SIZE/2;
    g_iCurY         = Y_SIZE/2;
    calcMandelbrot(1 /*UpdateVnc*/);
   }
   else
   {
    writeCursor();
   }
   }
}

/******************************************************************************
* Function Name: mandelbrot_init
* Description  : Initialisation for the Mandelbrot thread
* Arguments    : 
* Return Value : none
******************************************************************************/
void mandelbrot_init(void)
{
  g_iMandelRedraw = 1;

  g_iCursorRedraw = 1;

  g_iPrvCursor    = 0;

  g_iCurX         = X_SIZE/2;
  g_iCurY         = Y_SIZE/2;

  g_dMandelCx     = 0.0;
  g_dMandelCy     = 0.0;
  g_dMandelScale  = 0.02;
  
  calcMandelbrot(0 /*Initialise framebuffer only*/);
}


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
/**********************************************************************************
FILE NAME    : r_DDLCD_Frames.c

DESCRIPTION  : DDLCD Direct Drive Raster Frame management module..

Copyright   : 2010 Renesas Electronics America
Copyright   : 2010 Renesas Electronics Corporation.
          All Rights Reserved
**********************************************************************************/
#include "r_ddlcd_public.h" 
#include "r_DDLCD_private.h"


//************************************************
// INTERNAL Memory Allocation

typedef struct 
{
  Raster_t raster[DD_FRAME_HEIGHT * DD_FRAME_WIDTH];
} frame_type;

/* Section definition for SRAM used as DDLCD Frame(s) */
#pragma location=".BDD_RASTERS"
__root  frame_type DDLCD_Frames[DD_RASTER_FRAME_COUNT];  // SRAM allocated for DDLCD frames

static uint16_t Frame;        // index to frame buffer

/*****************************************************************************
Name: R_DDLCD_SetActiveRaster()    
Arguments: frame_request - selection of which frame to have ExDMA display.                     
Returns:  Address of raster that is being used for frame.     
Description: setActiveFrame defines which frame buffer to display
*****************************************************************************/
void * R_DDLCD_SetActiveRaster(uint16_t frame_request)
{
  if (frame_request < DD_RASTER_FRAME_COUNT)
  {
    Frame = frame_request;
    R_DDLCD_SetLineSource(0, DD_FRAME_HEIGHT, DDLCD_Frames[Frame].raster, DD_FRAME_WIDTH);
  }

  return(DDLCD_Frames[Frame].raster);
}

/*****************************************************************************
Name: R_DDLCD_GetActiveFrame()    
Arguments: none.                     
Returns:  Index of raster that is being used for frame.     
Description: Returns information about the which frame is currently active
*****************************************************************************/
uint16_t R_DDLCD_GetActiveFrame(void)
{
  return(Frame);
}

/*****************************************************************************
Name: R_DDLCD_SetRasterOffset()    
Arguments: x,y: pixels to offset raster by, 0,0 = no offset.                     
Returns:  0 = ok, -1 = NG.     
Description: allows setting the display window within frame. Bounds offset as
 necessary.
*****************************************************************************/
int16_t R_DDLCD_SetRasterOffset(int16_t x, int16_t y)
{
  /* ensure requested window is within the resource memory */
  if ((x < 0) || (y < 0))
    return(-1);
    
  if ((x > (DD_FRAME_WIDTH - H_DOT_DISPLAY)) || (y > (DD_FRAME_HEIGHT - V_LINES_DISPLAY)))
    return(-1);

  return (R_DDLCD_SetLineSource(0, V_LINES_DISPLAY, &DDLCD_Frames[Frame].raster[(int32_t)y*DD_FRAME_WIDTH + (int32_t)x], DD_FRAME_WIDTH));
}


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
/*  FILE        :Frames.c                                              */
/*  DATE        :Wen, Mar 12, 2008                                     */
/*  DESCRIPTION :Framebuffer initialization and handling               */        
/*                                                                     */
/*  CPU TYPE    :H8S/H8SX                                              */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
*
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"

#if (DD_RASTER_FRAME_COUNT < 3)
#error GAPI Demonstration Requires at least 3 Frame Buffers
#endif

// allocate BMP wrappers for frames
static BMP_t Frames[DD_RASTER_FRAME_COUNT];
BMP_t *displayFrameBuf;
BMP_t *backFrameBuf;
BMP_t *workFrameBuf;

/**********************************************************************************
Function Name:   FramesInit
Description:  Initializse BMPs for the raster frames.
Arguments:   none
Return value:   None
***********************************************************************************/
void FramesInit(void)
{
  // Before running system initialization, let us setup framebuffers first 
  {
    uint16_t i;
    static const BMP_ColorTable_t local_ct=CT_BLACK;
    for (i= 0; i<DD_RASTER_FRAME_COUNT; i++)
    {
      (void)R_BMP_Init(&Frames[i], (uint32_t)R_DDLCD_SetActiveRaster(i), BMP_DISP_BPP, DD_FRAME_WIDTH, DD_FRAME_HEIGHT);
      /* blank the display buffer */
      (void)R_GAPI_Fill(&Frames[i], 0, 0, DD_FRAME_WIDTH, DD_FRAME_HEIGHT, &local_ct);
    }
  }

  // Set Frame #0 as the active framebuffer and #1 as the work framebuffer and 2 as the background framebuffer
  (void)R_DDLCD_SetActiveRaster(0);
  displayFrameBuf = &Frames[0];
  workFrameBuf = &Frames[1];
  backFrameBuf = &Frames[2];
}

/**********************************************************************************
Function Name:   R_GAPI_WorkDisplay
Description:  Manages viewing of displayFrameBuf & workFrameBuf
              This function will make workFrameBuf the active raster and flip
              work and display pointers.
Arguments:   None
Return value:   Index of active frame buffer.
***********************************************************************************/
uint16_t R_GAPI_WorkDisplay(void)
{
  uint16_t return_value;

  // Set Frame #0 as the active framebuffer and #1 as the background framebuffer
  portENTER_CRITICAL(); /* RTOS_USAGE */

  if(0 == R_DDLCD_GetActiveFrame())
  {
    (void)R_DDLCD_SetActiveRaster(1);
    displayFrameBuf = &Frames[1];
    workFrameBuf = &Frames[0];
    return_value = 1;
  }
  else
  {
    (void)R_DDLCD_SetActiveRaster(0);
    displayFrameBuf = &Frames[0];
    workFrameBuf = &Frames[1];
    return_value = 0;
  }

  portEXIT_CRITICAL();  /* RTOS_USAGE */
  return (return_value);
}



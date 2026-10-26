/* Adapted to IAR Embedded Workbench */
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
 FILE NAME    : r_ddlcd_frames.c

 DESCRIPTION  : DDLCD Direct Drive Raster Frame management module..

 Copyright   : 2010 Renesas Electronics America
 Copyright   : 2010 Renesas Electronics Corporation.
 All Rights Reserved
 /**********************************************************************************/
#include "r_ddlcd_public.h" 
#include "r_ddlcd_private.h"
#include "config_r_gapi.h"

#ifndef DD_PANEL_ROTATE
static const int rotated = 0;
#else
static const int rotated=1;
#endif

#ifndef DD_INVERT_V_LINES
    #define FLIP_ROWS 0
#else
    #define FLIP_ROWS 1
#endif

#ifndef DD_INVERT_H_DOT
    #define FLIP_COLUMNS 0
#else
    #define FLIP_COLUMNS 1
#endif

static const DDLCD_Descriptor_t DDLCD_Descriptor =
{
{ DD_V_LINES_DISPLAY, DD_H_DOT_DISPLAY,
{ 0, FLIP_ROWS, FLIP_COLUMNS } },
{ DD_V_LINES_DISPLAY, DD_H_DOT_DISPLAY, 0, 0 },
{
{
{ TC_LL_X, TC_LL_Y },
{ TC_LR_X, TC_LR_Y },
{ TC_UL_X, TC_UL_Y },
{ TC_UR_X, TC_UR_Y } } } };

static const DDLCD_Descriptor_t DDLCD_Descriptor_Rotated =
{
{ DD_H_DOT_DISPLAY, DD_V_LINES_DISPLAY,
{ 0, FLIP_ROWS, FLIP_COLUMNS } },
{ DD_H_DOT_DISPLAY, DD_V_LINES_DISPLAY, 0, 0 },
{
{
{ TC_UL_Y, TC_UL_X },
{ TC_LL_Y, TC_LL_X },
{ TC_UR_Y, TC_UR_X },
{ TC_LR_Y, TC_LR_X } } } };

DDLCD_Descriptor_t const * pDDLCD_Descriptor;

/**************************************************/
/* INTERNAL Memory Allocation */

typedef struct
{
    Raster_t raster[DD_FRAME_SIZE];
} frame_type;

/* Section definition for SRAM used as DDLCD Frame(s) */
#pragma location=".DD_RAS_INT"
frame_type DDLCD_Frames[DD_RASTER_FRAME_COUNT];  // Internal SRAM allocated for DDLCD frames

static uint16_t Frame;

/*****************************************************************************
 Name: R_DDLCD_SetActiveRaster()
 Arguments: frame_request - selection of which frame to have DMA display.
 Returns:  Address of raster that is being used for frame.
 Description: setActiveFrame defines which frame buffer to display
 *****************************************************************************/
void * R_DDLCD_SetActiveRaster (uint16_t frame_request)
{
    if (NULL == pDDLCD_Descriptor)
    {
        /* initialize the descriptor pointer if nobody else has */
        if (0 == rotated)
        {
            pDDLCD_Descriptor = &DDLCD_Descriptor;
        }
        else
        {
            pDDLCD_Descriptor = &DDLCD_Descriptor_Rotated;
        }
    }

    if (frame_request < DD_RASTER_FRAME_COUNT)
    {
        Frame = frame_request;
        R_DDLCD_SetLineSource(0, /* only region */
        pDDLCD_Descriptor->Panel.Rows, /* lines in this region */
        DDLCD_Frames[Frame].raster, /* start location of region */
        pDDLCD_Descriptor->Frame.Columns); /* step size to next line */
    }

    return (DDLCD_Frames[Frame].raster);
}

/*****************************************************************************
 Name: R_DDLCD_GetActiveFrame()
 Arguments: none.
 Returns:  Index of raster that is being used for frame.
 Description: Returns information about the which frame is currently active
 *****************************************************************************/
uint16_t R_DDLCD_GetActiveFrame (void)
{
    return (Frame);
}

/*******************************************************************************
 * Function Name: R_DDLCD_SetRasterOffset
 * Description  : allows setting the display window within frame. Bounds offset as
 *                necessary.
 * Arguments    : x,y: pixels to offset raster by, 0,0 = no offset.
 * Return Value : 0 = ok, -1 = NG.
 *******************************************************************************/
int16_t R_DDLCD_SetRasterOffset (int16_t x, int16_t y)
{
    /* ensure requested window is within the resource memory */
    if ((x < 0) || (y < 0))
        return (-1);

    if ((x > (pDDLCD_Descriptor->Frame.Columns - pDDLCD_Descriptor->Panel.Columns))
            || (y > (pDDLCD_Descriptor->Frame.Rows - pDDLCD_Descriptor->Panel.Rows)))
        return (-1);

    return (R_DDLCD_SetLineSource(0, /* only region */
    pDDLCD_Descriptor->Panel.Rows, /* lines in this region */
    &DDLCD_Frames[Frame].raster[(int32_t) y * pDDLCD_Descriptor->Frame.Columns + (int32_t) x], /* start location of region */
    pDDLCD_Descriptor->Frame.Columns)); /* step size to next line */
}


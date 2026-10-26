/******************************************************************************
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
 *******************************************************************************
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name    : r_ddlcd_public.h
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : RXC
 * OS           : None
 * H/W Platform : RPB
 * Description  : Direct Drive Application Programming Interfac
 *
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 19.04.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/
#ifndef R_DDLCD_PUBLIC_H
    #define R_DDLCD_PUBLIC_H

/* Typedef fundemental data types that may change between compilers/families */
    #include <inttypes.h>
    #include <stdbool.h>
    #include <stddef.h>

extern volatile uint16_t DDLCD_Vcount;          // frame counter
extern volatile uint16_t DDLCD_BusActive;       // flag indicating if DMA is active

typedef int16_t DDLCD_Resp_t;

typedef struct
{
    int16_t                                                                                                                                                                                                                                                                                                                                   Rows; /* number of rows on LCD Panel...this should reflect Rows AFTER rotation */
    int16_t                                                                                                                                                                                                                                                                                                                                   Columns; /* number of columns on LCD Panel...this should reflect Columns AFTER rotation */
    struct
    {
        uint16_t Rotate :1; /* Rotate the frame while transferring to the LCD Panel 0:no, 1:yes */
        uint16_t Flip_Rows :1; /* invert the row order while transferring to the LCD Panel 0:no, 1:yes */
        uint16_t Flip_Columns :1; /* invert the column while transferring to the LCD Panel 0:no, 1:yes */
    } flags;
} DDLCD_Panel_t;

/* Note that the size of the Framebuffer is determined by the DD_FRAME_SIZE defineition in config_r_ddlcd.h.
 The configuration of belows Rows and Columns MUST be fit into this frame buffer */
typedef struct
{
    int16_t Rows; /* Number of rows in frame buffer */
    int16_t Columns; /* Number of columns in frame buffer */
    int16_t Offset_X; /* offset within frame to Panel display...set to 0 when same size */
    int16_t Offset_Y; /* offset within frame to Panel display...set to 0 when same size */
} DDLCD_Frame_t;

typedef struct
{
    int16_t x;
    int16_t y;
} DDLCD_Point_t;

typedef struct
{
    DDLCD_Point_t InitialCalibration[4]; /* default calibration data to load */
} DDLCD_Touch_t;

typedef struct
{
    DDLCD_Panel_t Panel;
    DDLCD_Frame_t Frame;
    DDLCD_Touch_t Touch;
} DDLCD_Descriptor_t;

extern DDLCD_Descriptor_t const * pDDLCD_Descriptor;

    #define DD_FRAME_HEIGHT pDDLCD_Descriptor->Frame.Rows
    #define DD_FRAME_WIDTH pDDLCD_Descriptor->Frame.Columns

/* Function Prototypes */
DDLCD_Resp_t R_DDLCD_Init (void);
int16_t R_DDLCD_SetLineSource (int16_t Region, int16_t LineCount, void *pSource, int16_t LineStep);
void * R_DDLCD_SetActiveRaster (uint16_t frame);
uint16_t R_DDLCD_GetActiveFrame (void);
int16_t R_DDLCD_SetFrameRate (int16_t rate);
int16_t R_DDLCD_GetFrameRate (void);
int16_t R_DDLCD_SetRasterOffset (int16_t x, int16_t y);
void R_DDLCD_Backlight (int state);
void R_DDLCD_Off (void);

#endif

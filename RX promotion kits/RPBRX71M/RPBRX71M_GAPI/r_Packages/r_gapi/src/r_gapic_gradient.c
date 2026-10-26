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
/***********************************************************************************
FILE NAME    : r_GAPIc_gradient.c

DESCRIPTION  : BMP fill Support for Renesas Graphics API.

Copyright   : 2010 Renesas Electronics America
Copyright   : 2010 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/
#include "r_gapi_private.h"

static const BMP_ColorTable_t CT_zero={0,0,0,0};

/**********************************************************************************
Function Name:   calc_gradient_pixel
Description:  Helper function to calculate intermediate gradient pixel colors
Arguments:   Gradient: position within the gradient scale
              Gradient_end: total size of gradient to ratio over
              ct: pointer to color table for gradient coloring [0] index = start, [1] index = end
              mode: specify order of operations on the color table.
Return value:  ct_out: color corresponding to intermediate gradient position.
***********************************************************************************/
static uint16_t calc_gradient_pixel(int32_t Gradient, int32_t Gradient_end, BMP_ColorTable_t const *const ct, uint8_t mode)
{
  int32_t rounding = Gradient_end/2;
  BMP_ColorTable_t local_ct[2];
  if (mode == 0)
  {
    local_ct[0] = ct[0];
    local_ct[1] = ct[1];
  }
  else
  {
    local_ct[0] = ct[1];
    local_ct[1] = ct[0];
  }

  local_ct[0].channel[BMP_CT_RED]   = (uint8_t)((int32_t)local_ct[0].channel[BMP_CT_RED]   + (((int32_t)local_ct[1].channel[BMP_CT_RED]   - (int32_t)local_ct[0].channel[BMP_CT_RED])   * (int32_t)Gradient + rounding) / Gradient_end);
  local_ct[0].channel[BMP_CT_GREEN] = (uint8_t)((int32_t)local_ct[0].channel[BMP_CT_GREEN] + (((int32_t)local_ct[1].channel[BMP_CT_GREEN] - (int32_t)local_ct[0].channel[BMP_CT_GREEN]) * (int32_t)Gradient + rounding) / Gradient_end);
  local_ct[0].channel[BMP_CT_BLUE]  = (uint8_t)((int32_t)local_ct[0].channel[BMP_CT_BLUE]  + (((int32_t)local_ct[1].channel[BMP_CT_BLUE]  - (int32_t)local_ct[0].channel[BMP_CT_BLUE])  * (int32_t)Gradient + rounding) / Gradient_end);
  return(R_GAPI_24_Disp(&local_ct[0]));
}

/* helper function to create gradient increment */
static inline int32_t calc_gradient_increment(int32_t RangeIn, int32_t RangeOut)
{
  return((RangeIn << 16)/RangeOut);
}

/* helper function to copy gradient value */
static inline void copy_gradient(uint16_t pixel, Raster_t *pd, int16_t count, int16_t step)
{
  for (; count > 0; count--, pd += step)
    *pd = pixel;
}

/**********************************************************************************
Function Name:   R_GAPI_CalcGradientCTN
Description:  function to build a gradient color table...usefull for "colorizing" grayscale BMPs
Arguments:   ct: pointer to color table for gradient coloring , [1] index = end
              mode: gradient generation mode
               0= ct[0] start-> ct[1] end
               1= ct[1] start-> ct[0] end
               2= 0:0:0 start-> ct[0] end
              N: Number of elements in color table
Return value:  ct_out: color corresponding to intermediate gradient position.
***********************************************************************************/
GAPI_Resp_t R_GAPI_CalcGradientCTN(BMP_ColorTable_t const *const ct, uint8_t mode, BMP_ColorTable_t * ct_out, int16_t N)
{
  BMP_ColorTable_t local_ct[2];
  int32_t inc;
  int32_t sum;
  int16_t index,ch;
  switch (mode)
  {
    case 0:
    {
      local_ct[0] = ct[0];
      local_ct[1] = ct[1];
      break;
    }
    case 1:  
    {
      local_ct[0] = ct[1];
      local_ct[1] = ct[0];
      break;
    }
    default:
    {
      local_ct[0] = CT_zero;
      local_ct[1] = ct[0];
      break;
    }
  }
  
  /* initialize the channel value "sum" and determine the increment */
  for(ch=0; ch < 3; ch++)
  {
    BMP_ColorTable_t * ct_write = ct_out;
    sum = (uint32_t)local_ct[0].channel[ch] << 16;
    inc = calc_gradient_increment(((int32_t)local_ct[1].channel[ch] - (int32_t)local_ct[0].channel[ch]), N-1);
    /* calculate value for each color in the table */
    for (index = 0; index < N; index++, ct_write++)
    {
      ct_write->channel[ch] = (uint8_t)(sum >> 16);
      sum += inc;
    }
  }

  return (0);
}

/**********************************************************************************
Function Name:   R_GAPI_CalcShadeCT
Description:  function to build a shaded color table...usefull for "dimming" color BMPs
Arguments:     pS: Source color table
                ct: pointer to color table for shade percentage [0] index= shade percentage, [1] index = na
Return value:  ct_out: color corresponding to shaded output
***********************************************************************************/
GAPI_Resp_t R_GAPI_CalcShadeCT(BMP_ColorTable_t const * pS, BMP_ColorTable_t const *const ct, BMP_ColorTable_t * ct_out)
{
  int16_t index;
  for (index = 0; index < 256; index++,ct_out++,pS++)
  {
    ct_out->channel[BMP_CT_RED] = (uint8_t)(((uint16_t)pS->channel[BMP_CT_RED] * (uint16_t)ct[0].channel[BMP_CT_RED])>>8);
    ct_out->channel[BMP_CT_GREEN] = (uint8_t)(((uint16_t)pS->channel[BMP_CT_GREEN] * (uint16_t)ct[0].channel[BMP_CT_GREEN])>>8);
    ct_out->channel[BMP_CT_BLUE] = (uint8_t)(((uint16_t)pS->channel[BMP_CT_BLUE] * (uint16_t)ct[0].channel[BMP_CT_BLUE])>>8);
  }
  return (0);
}

/**********************************************************************************
Function Name:   R_GAPI_FillGradient
Description:  Fill area of BMP with a gradient color
Arguments:   d: pointer to destination BMP
              dPosX: starting X coordinate within destination BMP (leftmost = 0)
              dPosY: starting Y coordinate within destination BMP (bottommost = 0)
              Width: Width of area to fill
              Height: Height of area to fill
              ct: pointer to fill colors
              angle: angle (degrees) of gradient (presently only supporting 0,90
Return value:   0:OK, Not 0: NG
***********************************************************************************/

GAPI_Resp_t R_GAPI_FillGradient(BMP_t *const d, const int16_t dPosX, const int16_t dPosY, int16_t Width, int16_t Height, BMP_ColorTable_t const *const ct, int16_t angle)
{
  const int16_t dbiHeight = R_BMP_Height(d);
  int16_t dbiWidth = R_BMP_Width(d);
  pRaster_t pdRaster;
  int16_t indexH,indexW;
  uint16_t pixel16;

  if(d==NULL)
    return(-1);
    
  /* only handling display native destination */
  if ((BMP_BPP_DISP | BMP_BPC_DISP) != R_BMP_Format(d))
    return(-1);

  /* bounds check destination and reject or trim if necessary */
  if (dPosX > dbiWidth)
    return(-1);
  if (dPosY > dbiHeight)
    return(-1);

  if ((Width + dPosX) > dbiWidth)
    Width = dbiWidth - dPosX;
  if ((Height + dPosY) > dbiHeight)
    Height = dbiHeight - dPosY;

  /* create a "true" quad line width in bytes */
  dbiWidth = (int16_t)((((int32_t)dbiWidth * (int32_t)d->biBitCount[0] - 1L)/8L) + 1L);
  dbiWidth = (((dbiWidth - 1)/4) + 1) * 4;

  /* point to destination raster data */
  pdRaster.p08 = ((uint8_t *)d) + R_BMP_Offset(d);
  /* point to first destination line */
  pdRaster.p08 += (int32_t)dbiWidth * (int32_t)dPosY;
  /* point to destination pixel */
  pdRaster.pDisp += dPosX;

  switch (angle)
  {
    case 0:
      /* Gradient increases horizontally left (ct[0]) to right (ct[1])*/

    case 180:
    {
      /* Gradient increases horizontally left (ct[1]) to right (ct[0])*/
      for (indexW = 0; indexW < Width; indexW++, pdRaster.pDisp++)
      {
        /* Calculate Pixel for this column */
        pixel16=calc_gradient_pixel((int32_t)indexW,(int32_t)Width,ct,(0 == angle)?0:1);

        /* Copy this pixel to the pixels on this column */
        copy_gradient(pixel16, pdRaster.pDisp, Height, dbiWidth/sizeof(Raster_t));
      }
      return(0);
    }

    case 90:

      /* Gradient increases vertically bottom (ct[0]) to top (ct[1]) */
    case 270:
    {
      /* Gradient increases vertically bottom (ct[1]) to top (ct[0]) */
      for (indexH = 0; indexH < Height; indexH++, pdRaster.p08 += dbiWidth)
      {
        /* Calculate Pixel for this row */
        pixel16=calc_gradient_pixel((int32_t)indexH,(int32_t)Height,ct,(90 == angle)?0:1);

        /* Copy this pixel to the pixels on this row */
        copy_gradient(pixel16, pdRaster.pDisp, Width, 1);
      }
      return(0);
    }

    default:
      return(-1);
  }
}


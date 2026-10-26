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
FILE NAME    : r_GAPIc_xlate.c

DESCRIPTION  : BMP mirror/rotate Support for Renesas Graphics API.

Copyright   : 2010 Renesas Electronics America
Copyright   : 2010 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/
#include "r_gapi_private.h"

/**********************************************************************************
Function Name:   R_GAPI_CopyXlate
Description:  16bpp to 16bpp copy with rotate/mirror.
Arguments:   s: pointer to source BMP
              d: pointer to destination BMP
              dPosX: starting X coordinate within destination BMP (leftmost = 0)
              dPosY: starting Y coordinate within destination BMP (bottommost = 0)
              sPosX: starting X coordinate within source BMP (leftmost = 0)
              sPosY: starting Y coordinate within source BMP (bottommost = 0)
              Width: "width" of source area to copy
              Height: "height" of source area to copy
              Mode:  Opertions to perform duing copy
Return value:   0:OK, Not 0: NG
***********************************************************************************/
GAPI_Resp_t R_GAPI_CopyXlate( BMP_t const *const s, BMP_t *const d, int16_t dPosX, int16_t dPosY, int16_t sPosX, int16_t sPosY, int16_t Width, int16_t Height, uint16_t Mode )
{
  const int16_t sbiHeight = R_BMP_Height(s);
  const int16_t sbiWidth = R_BMP_Width(s);

  const int16_t dbiHeight = R_BMP_Height(d);
  const int16_t dbiWidth = R_BMP_Width(d);

  uint8_t rotate = 0;

  pRaster_t psRaster, pdRaster;

  if((s==NULL) || (d==NULL))
    return(-1);
    
  /* only handling display native destination */
  if ((BMP_BPP_DISP | BMP_BPC_DISP) != R_BMP_Format(d))
    return(-1);

  if((((Mode & GAPI_CX_ROT_MASK) & GAPI_CX_ROT_90) == GAPI_CX_ROT_90) ||
    (((Mode & GAPI_CX_ROT_MASK) & GAPI_CX_ROT_270) == GAPI_CX_ROT_270)) 
  {
    rotate = 1;
  }

  if (rotate == 0)
  {
    /* Region is NOT rotated in destination */
    /* bounds check source and reject or trim if necessary */
    if (sPosX > sbiWidth)
      return(-1);
    if (sPosY > sbiHeight)
      return(-1);
  
    if (sPosX < 0)
    {
      dPosX -= sPosX;
      Width += sPosX;
      sPosX = 0;
    }
    if (sPosY < 0)
    {
      dPosY -= sPosY;
      Height += sPosY;
      sPosY = 0;
    }
  
    if ((Width + sPosX) > sbiWidth)
      Width = sbiWidth - sPosX;
    if ((Height + sPosY) > sbiHeight)
      Height = sbiHeight - sPosY;
  
    /* bounds check destination and reject or trim if necessary */
    if (dPosX > dbiWidth)
      return(-1);
    if (dPosY > dbiHeight)
      return(-1);
  
    if (dPosX < 0)
    {
      sPosX -= dPosX;
      Width += dPosX;
      dPosX = 0;
    }
    if (dPosY < 0)
    {
      sPosY -= dPosY;
      Height += dPosY;
      dPosY = 0;
    }
    if ((Width + dPosX) > dbiWidth)
      Width = dbiWidth - dPosX;
    if ((Height + dPosY) > dbiHeight)
      Height = dbiHeight - dPosY;
  }
  else
  {
    /* Region IS rotated in destination */
    /* bounds check source and reject or trim if necessary */
    if (sPosX > sbiWidth)
      return(-1);
    if (sPosY > sbiHeight)
      return(-1);
  
    if (sPosX < 0)
    {
      dPosY -= sPosX;
      Width += sPosX;
      sPosX = 0;
    }
    if (sPosY < 0)
    {
      dPosX -= sPosY;
      Height += sPosY;
      sPosY = 0;
    }
  
    if ((Width + sPosX) > sbiWidth)
      Width = sbiWidth - sPosX;
    if ((Height + sPosY) > sbiHeight)
      Height = sbiHeight - sPosY;
  
    /* bounds check destination and reject or trim if necessary */
    if (dPosX > dbiWidth)
      return(-1);
    if (dPosY > dbiHeight)
      return(-1);
  
    if (dPosX < 0)
    {
      sPosY -= dPosX;
      Height += dPosX;
      dPosX = 0;
    }
    if (dPosY < 0)
    {
      sPosX -= dPosY;
      Width += dPosY;
      dPosY = 0;
    }
    if ((Height + dPosX) > dbiWidth)
      Height = dbiWidth - dPosX;
    if ((Width + dPosY) > dbiHeight)
      Width = dbiHeight - dPosY;
  }

  if ((Height < 0) || (Width < 0))
    return(-1);

  /* point to source raster data */
  psRaster.p08 = ((uint8_t *)s) + R_BMP_Offset(s);

  /* point to first source pixel */
  psRaster.pDisp += (int32_t)sbiWidth * (int32_t)sPosY + (int32_t)sPosX;

  /* point to destination raster data */
  pdRaster.p08 = ((uint8_t *)d) + R_BMP_Offset(d);

  /* point to first destination pixel (maybe) */
  pdRaster.pDisp += (int32_t)dbiWidth * (int32_t)dPosY + (int32_t)dPosX;

  {
    int32_t dLineInc, dPixelInc;
    switch (Mode & 0xF)
    {
      default:
      case 0x00:   /* row start, pixel start,   PixelInc,   LineInc NO GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_0   */
      case 0x0E:   /*         0,           0,          1,       dW,    GAPI_CX_FLIPY,    GAPI_CX_FLIPX, GAPI_CX_ROT_180 */
        pdRaster.pDisp += (0 * (int32_t)dbiWidth) + (int32_t)0;
        dPixelInc = 1;
        dLineInc = dbiWidth;
        break;

      case 0x07:   /* row start, pixel start,   PixelInc,   LineInc NO GAPI_CX_FLIPY,    GAPI_CX_FLIPX, GAPI_CX_ROT_270 */                                                  
      case 0x09:   /*         0,           0,         dw,        1,    GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_90 */ 
        pdRaster.pDisp += (0 * (int32_t)dbiWidth) + (int32_t)0;
        dPixelInc = dbiWidth;
        dLineInc = 1;
        break;

      case 0x01:   /* row start, pixel start,   PixelInc,   LineInc NO GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_90 */                                                  
      case 0x0F:   /*         0,           H,         dw,       -1,    GAPI_CX_FLIPY,    GAPI_CX_FLIPX, GAPI_CX_ROT_270 */ 
        pdRaster.pDisp += (0 * (int32_t)dbiWidth) + (int32_t)Height;
        dPixelInc = dbiWidth;
        dLineInc = -1;
        break;

      case 0x04:   /* row start, pixel start,   PixelInc,   LineInc NO GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_270 */                                                  
      case 0x0A:   /*         0,           W,         -1,       dW,    GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_180 */ 
        pdRaster.pDisp += (0 * (int32_t)dbiWidth) + (int32_t)Width;
        dPixelInc = -1;
        dLineInc = dbiWidth;
        break;

      case 0x06:   /* row start, pixel start,   PixelInc,   LineInc NO GAPI_CX_FLIPY,    GAPI_CX_FLIPX, GAPI_CX_ROT_90 */                                                  
      case 0x08:   /*         H,           0,          1,      -dW,    GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_0 */ 
        pdRaster.pDisp += ((int32_t)Height * (int32_t)dbiWidth) + (int32_t)0;
        dPixelInc = 1;
        dLineInc = -dbiWidth;
        break;

      case 0x03:   /* row start, pixel start,   PixelInc,   LineInc NO GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_270 */                                                  
      case 0x0D:   /*         W,           0,        -dw,        1,    GAPI_CX_FLIPY,    GAPI_CX_FLIPX, GAPI_CX_ROT_90 */ 
        pdRaster.pDisp += ((int32_t)Width * (int32_t)dbiWidth) + (int32_t)0;
        dPixelInc = -dbiWidth;
        dLineInc = 1;
        break;

      case 0x02:   /* row start, pixel start,   PixelInc,   LineInc NO GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_180 */                                                  
      case 0x0C:   /*         H,           W,         -1,      -dW,    GAPI_CX_FLIPY,    GAPI_CX_FLIPX, GAPI_CX_ROT_0 */ 
        pdRaster.pDisp += ((int32_t)Height * (int32_t)dbiWidth) + (int32_t)Width;
        dPixelInc = -1;
        dLineInc = -dbiWidth;
        break;

      case 0x05:   /* row start, pixel start,   PixelInc,   LineInc NO GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_180 */                                                  
      case 0x0B:   /*         W,           H,        -dw,       -1,    GAPI_CX_FLIPY, NO GAPI_CX_FLIPX, GAPI_CX_ROT_270 */ 
        pdRaster.pDisp += ((int32_t)Width * (int32_t)dbiWidth) + (int32_t)Height;
        dPixelInc = -dbiWidth;
        dLineInc = -1;
        break;
    }

    {
      int16_t RowIndex, PixelIndex;

      for (RowIndex = 0; RowIndex<Height; RowIndex++, psRaster.pDisp+=sbiWidth, pdRaster.pDisp+=dLineInc)
      {
        Raster_t *pS = psRaster.pDisp, *pD = pdRaster.pDisp;
        for (PixelIndex = 0; PixelIndex < Width; PixelIndex++, pS++, pD+=dPixelInc)
        {
          *pD = *pS;
        }
      }
    }
  }
  return(0);
}



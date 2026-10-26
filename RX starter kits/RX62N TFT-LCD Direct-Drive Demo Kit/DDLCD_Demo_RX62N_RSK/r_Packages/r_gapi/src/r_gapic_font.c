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
FILE NAME    : GAPIc_Font.c

DESCRIPTION  : Font Support for Renesas Graphics API.
              Requires GAPI BMP functions

Copyright   : 2007 Renesas Electronics America
Copyright   : 2007 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/

/***********************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
01.02.2007 RTA-JMB First Release
02.15.2007 RTA-JMB Improved transparency handling
03.01.2007 RTA-JMB Added size flexibility to font BMPs
07.02.2007 RTA-MEV Rev 1.0
01.28.2008 RTA-JMB Revised to remove redundent API
***********************************************************************************/
#include "r_gapi_private.h"

const uint16_t CT8_grayscale[256]=
{
  0x0000,0x0000,0x0000,0x0000,0x0020,0x0020,0x0020,0x0020,0x0841,0x0841,0x0841,0x0841,0x0861,0x0861,0x0861,0x0861,
  0x1082,0x1082,0x1082,0x1082,0x10A2,0x10A2,0x10A2,0x10A2,0x18C3,0x18C3,0x18C3,0x18C3,0x18E3,0x18E3,0x18E3,0x18E3,
  0x2104,0x2104,0x2104,0x2104,0x2124,0x2124,0x2124,0x2124,0x2945,0x2945,0x2945,0x2945,0x2965,0x2965,0x2965,0x2965, 
  0x3186,0x3186,0x3186,0x3186,0x31A6,0x31A6,0x31A6,0x31A6,0x39C7,0x39C7,0x39C7,0x39C7,0x39E7,0x39E7,0x39E7,0x39E7, 
  0x4208,0x4208,0x4208,0x4208,0x4228,0x4228,0x4228,0x4228,0x4A49,0x4A49,0x4A49,0x4A49,0x4A69,0x4A69,0x4A69,0x4A69, 
  0x528A,0x528A,0x528A,0x528A,0x52AA,0x52AA,0x52AA,0x52AA,0x5ACB,0x5ACB,0x5ACB,0x5ACB,0x5AEB,0x5AEB,0x5AEB,0x5AEB, 
  0x630C,0x630C,0x630C,0x630C,0x632C,0x632C,0x632C,0x632C,0x6B4D,0x6B4D,0x6B4D,0x6B4D,0x6B6D,0x6B6D,0x6B6D,0x6B6D, 
  0x738E,0x738E,0x738E,0x738E,0x73AE,0x73AE,0x73AE,0x73AE,0x7BCF,0x7BCF,0x7BCF,0x7BCF,0x7BEF,0x7BEF,0x7BEF,0x7BEF, 
  0x8410,0x8410,0x8410,0x8410,0x8430,0x8430,0x8430,0x8430,0x8C51,0x8C51,0x8C51,0x8C51,0x8C71,0x8C71,0x8C71,0x8C71, 
  0x9492,0x9492,0x9492,0x9492,0x94B2,0x94B2,0x94B2,0x94B2,0x9CD3,0x9CD3,0x9CD3,0x9CD3,0x9CF3,0x9CF3,0x9CF3,0x9CF3, 
  0xA514,0xA514,0xA514,0xA514,0xA534,0xA534,0xA534,0xA534,0xAD55,0xAD55,0xAD55,0xAD55,0xAD75,0xAD75,0xAD75,0xAD75, 
  0xB596,0xB596,0xB596,0xB596,0xB5B6,0xB5B6,0xB5B6,0xB5B6,0xBDD7,0xBDD7,0xBDD7,0xBDD7,0xBDF7,0xBDF7,0xBDF7,0xBDF7, 
  0xC618,0xC618,0xC618,0xC618,0xC638,0xC638,0xC638,0xC638,0xCE59,0xCE59,0xCE59,0xCE59,0xCE79,0xCE79,0xCE79,0xCE79, 
  0xD69A,0xD69A,0xD69A,0xD69A,0xD6BA,0xD6BA,0xD6BA,0xD6BA,0xDEDB,0xDEDB,0xDEDB,0xDEDB,0xDEFB,0xDEFB,0xDEFB,0xDEFB, 
  0xE71C,0xE71C,0xE71C,0xE71C,0xE73C,0xE73C,0xE73C,0xE73C,0xEF5D,0xEF5D,0xEF5D,0xEF5D,0xEF7D,0xEF7D,0xEF7D,0xEF7D, 
  0xF79E,0xF79E,0xF79E,0xF79E,0xF7BE,0xF7BE,0xF7BE,0xF7BE,0xFFDF,0xFFDF,0xFFDF,0xFFDF,0xFFFF,0xFFFF,0xFFFF,0xFFFF, 
};

const uint16_t CT4_grayscale[16]=
{0x0000,0x1082,0x2104,0x3186,0x4228,0x52AA,0x632C,0x73AE,0x8C51,0x9CD3,0xAD55,0xBDD7,0xCE79,0xDEFB,0xEF7D,0xFFFF};

/**********************************************************************************
Function Name:   LCDGlyphFind
Description:  Find the Glyph information associated with the code
Arguments:   
      pF: Pointer to font structure
      code: value of code to look up.
Return value:   pointer to glyph record.
***********************************************************************************/
static GAPI_Eglyph_t const * LCDGlyphFind(GAPI_Font_t const *pF, uint32_t code)
{
  GAPI_Eglyph_t const *pG;
  int32_t Low=0, High = pF->base.num_glyphs-1, index;
  while(Low <= High)
  {
    index = (High+Low)/2;
    pG = &pF->glyphs[index];

    if (code == pG->char_code)
      return(pG);
      
    if (code < pG->char_code)
      High = index -1;
    else
      Low = index +1;
  }
  return(NULL); 
}

/**********************************************************************************
Function Name:   R_GAPI__UTF8decode
Description:  extract 32bit unicode value from UTF-8 stream
Arguments:   s: pointer to pointer to user string (POINTER IS UPDATED BY THIS ROUTINE)
Return value:   width of string.
***********************************************************************************/
static uint32_t R_GAPI__UTF8decode(uint8_t const ** pS)
{
  uint8_t const *s = *pS;
  /* step past first character */
  *pS += 1;
  if(*s < 0x80)
  {
    return((uint32_t)*s);
  }
  if((0xC2  <= *s ) && (*s <= 0xF4))
  {
    uint32_t value;
    int index;
    if(*s < 0xE0)
    {
      /* two byte sequence*/
      value = (((uint32_t)s[0] & 0x1F) << 6) | (((uint32_t)s[1] & 0x3F) << 0);
      index = 1;
    }
    else if (*s < 0xF0)
    {
      /* three byte sequence*/
      value = (((uint32_t)s[0] & 0x0F) << 12) | (((uint32_t)s[1] & 0x3F) << 6) | (((uint32_t)s[2] & 0x3F) << 0);
      index = 2;
    }
    else
    {
      /* four byte sequence*/
      value = (((uint32_t)s[0] & 0x07) << 18) | (((uint32_t)s[1] & 0x3F) << 12) | (((uint32_t)s[2] & 0x3F) << 6) | (((uint32_t)s[3] & 0x3F) << 0);
      index = 3;
    }

    /* point to secondary bytes and check their range*/
    s++;
    for (; index > 0; index--, s++)
    {
      if((*s < 0x80) || (*s > 0xBF))
      {
        /* out of range, return error */
        *pS = s + 1;
        return(0x20);
      }
    }
    *pS = s;
    return(value);
  }
  /* shouldn't be here */
  return(0x20);
}

/**********************************************************************************
Function Name:   R_GAPI_FontHeight
Description:  Determine height of the font (in pixels)
Arguments:   Gdata: pointer to font information 
Return value:   max vertical size of font.
***********************************************************************************/
int16_t R_GAPI_FontHeight(GAPI_PUT_t const *Gdata)
{
  GAPI_Font_t const * pFont = *Gdata->ppFont;
  
  /* make sure we're pointing to proper endian font */
  if((pFont == NULL) || (pFont->base.signature != 0x65464e54))
    return(0);
    
  return((int16_t)(pFont->base.bbox.yMax - pFont->base.bbox.yMin));
}

/**********************************************************************************
Function Name:   R_GAPI_FontWidthString
Description:  Determine width of the supplied string (in pixels)
Arguments:   s: pointer to user string
              Gdata: pointer to font information 
Return value:   Width of the drawn string
***********************************************************************************/
int16_t R_GAPI_FontWidthString(uint8_t const * s, GAPI_PUT_t const *Gdata)
{
  uint32_t code;
  GAPI_Eglyph_t const *pG;
  GAPI_Font_t const * pFont = *Gdata->ppFont;
  int16_t PosX = -pFont->base.bbox.xMin;
  int16_t Max = 0;
  int16_t IncX = Gdata->SpacingX;
  
  /* make sure we're pointing to proper endian font */
  if((pFont == NULL) || (pFont->base.signature != 0x65464e54))
    return(0);
    
  if(IncX < 0)
    IncX = pFont->base.bbox.xMax - pFont->base.bbox.xMin;
  
  while ((code = R_GAPI__UTF8decode(&s)) != 0)
  {
    /* Ensure character is in font */
    pG = LCDGlyphFind(pFont, code);
    if (pG != NULL)
    {
      int16_t test = PosX + pG->left + pG->width;
      if(test > Max)
        Max = test;  /* this case will typically occur on every glyph */

      if(IncX == 0)
      {
        /* Use proportional font advancement*/
        PosX += pG->advance;
      }
      else
      {
        PosX += IncX;
      }
    }
  }
  if(Max < PosX) Max = PosX;
  return(Max);
}

/**********************************************************************************
Function Name:   R_GAPI_PutS
Description:  Places user string within destination BMP
Arguments:   s: pointer to user string
              Gdata: pointer to font information (also includes substitution color table
                and orientation information).
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
Return value:   position of dominant axis on return (can be used to append text if used as starting point).
***********************************************************************************/
int16_t R_GAPI_PutS( uint8_t const * s, GAPI_PUT_t const *Gdata, BMP_t *d, int16_t PosX, int16_t PosY)
{
  GAPI_Eglyph_t const *pG;
  GAPI_Font_t const * pFont = *Gdata->ppFont;
  uint16_t pCT16[2];
  BMP_ColorTable_t const *pCT = (BMP_ColorTable_t *)pCT16;
  uint32_t code;
  uint32_t tr = NO_TRANSPARENCY_COLOR;
  int16_t IncX, IncY;
  int16_t FillX = PosX, FillY = PosY;
  BMP_t *pBMPFont;

  /* make sure we're pointing to proper endian font */
  if((pFont == NULL) || (pFont->base.signature != 0x65464e54))
    return(0);
    
  pBMPFont = (void *)R_GAPI_PoolMemoryGet(256);
  if(NULL == pBMPFont)
    return(-1);

  if(Gdata->Options.Align_H != 0)
  {
    int16_t Width = R_GAPI_FontWidthString(s,Gdata);
    if(1 == Gdata->Options.Align_H)
      PosX -= Width/2;
    if(2 == Gdata->Options.Align_H)
      PosX -= Width;
  }
  
  if(Gdata->Options.Align_V != 0)
  {
    int16_t Height = R_GAPI_FontHeight(Gdata);
    if(1 == Gdata->Options.Align_V)
      PosY -= Height/2;
    if(2 == Gdata->Options.Align_V)
      PosY -= Height;
  }
  
  FillX = PosX;
  FillY = PosY;
  IncX = Gdata->SpacingX;
  if(IncX < 0)
    IncX = pFont->base.bbox.xMax - pFont->base.bbox.xMin;
  IncY = Gdata->SpacingY;
  if(IncY < 0)
    IncY = pFont->base.bbox.yMax - pFont->base.bbox.yMin;

  /* compensate for font baseline */
  PosX -= pFont->base.bbox.xMin;
  PosY -= pFont->base.bbox.yMin;
  
  /* initialize for 4bpp anti-aliased fonts */
  /* initialize for 8bpp anti-aliased fonts */
  {
    uint16_t Fgnd_transparent = Gdata->Options.Fgnd_transparent;
    /* handle backward compatibility with pre GAPI_T usage */
    if(0 == Fgnd_transparent)
      Fgnd_transparent = GAPI_T_MULC;
      
    /* convert color to 16bpp...so we don't have to on every glpyh */
    tr = TRANSPARENCY(Fgnd_transparent,R_GAPI_24_Disp(&Gdata->ct[1]));
  }

  /* initialize for non-anti-aliased 1bpp fonts */
  if ((pFont->base.render_style & RENDER_1BPP) == RENDER_1BPP)
  {
    /* convert to 16bpp...so we don't have to on every glpyh */
    int16_t index;
    for (index=0; index<2; index++)
      pCT16[index] = R_GAPI_24_Disp(&Gdata->ct[index]);
      
    /* handle transparency */
    if (Gdata->Options.Bgnd_transparent)
      tr = pCT16[0];

    if (Gdata->Options.Fgnd_transparent)
      tr = pCT16[0];
  }
  else if ((pFont->base.render_style & RENDER_4BPP) == RENDER_4BPP)
  {
    pCT = (BMP_ColorTable_t *)CT4_grayscale;  /* attach constant color table */
  }
  else if ((pFont->base.render_style & RENDER_8BPP) == RENDER_8BPP)
  {
    pCT = (BMP_ColorTable_t *)CT8_grayscale;  /* attach constant color table */
  }

  while ((code = R_GAPI__UTF8decode(&s)) != 0)
  {
    /* Ensure character is in font */
    pG = LCDGlyphFind(pFont, code);
    if (pG != NULL)
    {
      uint32_t pRaster = ((uint32_t)pG + pG->raster_offset);

      if((pG->raster_style & EFNT_8BPP) == EFNT_8BPP)
      {
        /* 8 BPP alpha mask */
        R_BMP_Init(pBMPFont, pRaster, BMP_8NQ_BPP, pG->pitch, pG->height);
        if((pG->raster_style & EFNT_RLE) == EFNT_RLE)
          pBMPFont->biCompression[0] = 1;
        setBMPF(pBMPFont,16BPP_CT);
      }
      else if((pG->raster_style & EFNT_4BPP) == EFNT_4BPP)
      {
        /* 4 BPP alpha mask */
        R_BMP_Init(pBMPFont, pRaster, BMP_4NQ_BPP, pG->pitch * 2, pG->height);
        if((pG->raster_style & EFNT_RLE) == EFNT_RLE)
          setBMPF(pBMPFont,4BPP_RLE);
        setBMPF(pBMPFont,16BPP_CT);
      }
      else
      {
        /* 1 BPP image */
        R_BMP_Init(pBMPFont, pRaster, BMP_1NQ_BPP, pG->pitch * 8, pG->height);
        setBMPF(pBMPFont,16BPP_CT);
      }  
        
      if (Gdata->Options.Bgnd_transparent == 0)
      {
        /* paint the background solid */
        int16_t Width = (pG->advance > (pG->left + pG->width)) ? pG->advance : (pG->left + pG->width);
        Width = Width + PosX - FillX;
        if (Width > 0)
        {
          (void)R_GAPI_Fill(d, FillX, FillY, Width, R_GAPI_FontHeight(Gdata), &Gdata->ct[0]);
          FillX += Width;
        }
      }

      if((IncX == 0) && (IncY == 0))
      {
        /* Use proportional font advancement */
        /* Extract font data and write to destination using requested color table */
        (void)R_GAPI_CopySub( pBMPFont, d, PosX+pG->left, PosY+pG->bottom, 0, 0, pG->width, pG->height, pCT, tr);
        PosX += pG->advance;
      }
      else
      {
        /* use fixed font advancement override */
        (void)R_GAPI_CopySub( pBMPFont, d, PosX+(IncX- (pG->width+pG->left))/2, PosY+pG->bottom, 0, 0, pG->width, pG->height, pCT, tr);
        PosX += IncX;
        PosY += IncY;
      }
    }
  }
  (void)R_GAPI_PoolMemoryFree((void *)pBMPFont);
  /* Return position of dominant axis */
  return(PosX);
}

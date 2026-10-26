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
FILE NAME    : r_GAPI_Copy.c

DESCRIPTION  : BMP copy Support for Renesas Graphics API.

Copyright   : 2007 Renesas Electronics America
Copyright   : 2007 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/

#include <string.h>
#include "r_gapi_private.h"

/**********************************************************************************
Function Name:   copy_pixel565
Description:  Helper function to blend a 16bpp(565) dest with a 16bpp(565) src 
Arguments:   dest: 16bpp source pixel
              src: value to blend with 0=BackPixel, 0xFF=ForePixel
              tr32: information about how to blend/copy
Return value:   resultant pixel
***********************************************************************************/

static inline uint16_t mul_pixel565(uint16_t Pixel1, uint16_t Pixel2)
{
  uint16_t out;
  out  = (uint16_t)((uint32_t)((Pixel1 & 0xF800) * (Pixel2 & 0xF800)) >> 16) & 0xF800;
  out += (uint16_t)((uint32_t)((Pixel1 & 0x07E0) * (Pixel2 & 0x07E0)) >> 11) & 0x07E0;
  out += (uint16_t)((uint32_t)((Pixel1 & 0x001F) * (Pixel2 & 0x001F)) >>  5) & 0x001F;
  return(out);
}

static uint16_t copy_pixel565_NONE(uint16_t dest, uint16_t src, uint32_t tr32)
{
  return(src);
}

static uint16_t copy_pixel565_MAGIC(uint16_t dest, uint16_t src, uint32_t tr32)
{
    if(src == (uint16_t)tr32)
      return(dest);
    else
      return(src);
}

static uint16_t copy_pixel565_MUL(uint16_t dest, uint16_t src, uint32_t tr32)
{
  /* out = (src * dest) */
  switch (src)
  {
    case 0:
      return(0);
    case 0xFFFF:
      return(dest);
    default:
      return(mul_pixel565(src, dest));
  }
}

static uint16_t copy_pixel565_MULC(uint16_t dest, uint16_t src, uint32_t tr32)
{
  /* factor in transparency value 
   out = (src * tr_value) + ((1 - src) * dest) */
  switch (src)
  {
    case 0:
      return(dest);
    case 0xFFFF:
      return((uint16_t)tr32);
    default:
    {
      uint16_t out;
      out  = mul_pixel565(dest, 0xFFFF-src);
      out += mul_pixel565((uint16_t)tr32, src);
      return(out);
    }
  }
}

static uint16_t copy_pixel565_MULT(uint16_t dest, uint16_t src, uint32_t tr32)
{
  /* factor in transparency value 
   out = (src * tr_value) + ((1 - tr_value) * dest) */
  switch ((uint16_t)tr32)
  {
    case 0:
      return(dest);
    case 0xFFFF:
      return(src);
    default:
    {
      uint16_t out;
      out  = mul_pixel565(dest, 0xFFFF-(uint16_t)tr32);
      out += mul_pixel565(src, (uint16_t)tr32);
      return(out);
    }
  }
}

static uint16_t copy_pixel565_MULS(uint16_t dest, uint16_t src, uint32_t tr32)
{
  /* create shadow mask 
   out = (src * tr_value * dest) + ((1 - src) * dest) */
  if(0==src)
    return(dest);
  if(0xFFFF == src)
    return(mul_pixel565(dest, (uint16_t)tr32));
  {
    uint16_t out;
    out  = mul_pixel565(dest, 0xFFFF-src);
    out += mul_pixel565(src, mul_pixel565(dest, (uint16_t)tr32));
    return(out);
  }
}

static uint16_t copy_pixel565_MULK(uint16_t dest, uint16_t src, uint32_t tr32)
{
  /* out = (src * tr_value) */
  return(mul_pixel565((uint16_t)tr32, src));
}

static uint16_t copy_pixel565_FILL(uint16_t dest, uint16_t src, uint32_t tr32)
{
  /* out = (src * tr_value) */
  return((uint16_t)tr32);
}

typedef  uint16_t (*copy_pixel_func_type)(uint16_t dest, uint16_t src, uint32_t tr32);

static copy_pixel_func_type copy_pixel_chooser(uint32_t tr32)
{
  switch(tr32 >> 16)
  {
    default:
    case GAPI_T_NONE:
      return(copy_pixel565_NONE);
    case GAPI_T_MAGIC:
      return(copy_pixel565_MAGIC);
    case GAPI_T_MUL:
      return(copy_pixel565_MUL);
    case GAPI_T_MULC:
      return(copy_pixel565_MULC);
    case GAPI_T_MULK:
      return(copy_pixel565_MULK);
    case GAPI_T_FILL:
      return(copy_pixel565_FILL);
    case GAPI_T_MULT:
      return(copy_pixel565_MULT);
    case GAPI_T_MULS:
      return(copy_pixel565_MULS);
  }
}

static inline uint16_t copy_pixel_565(uint16_t dest, uint16_t src, uint32_t tr32, copy_pixel_func_type func)
{
  if (GAPI_T_NONE == (tr32 >> 16))
    return(src);
  else if(GAPI_T_MAGIC == (tr32 >> 16))
  {
    if(src == (uint16_t)tr32)
      return(dest);
    else
      return(src);
  }
  else if(GAPI_T_FILL == (tr32 >> 16))
    return((uint16_t)tr32);
  else 
    return(func( dest, src, tr32));
}

static inline uint16_t copy_pixel_index(uint16_t dest, uint8_t src, uint16_t const *const ct, uint32_t tr32, copy_pixel_func_type func)
{
  if (GAPI_T_NONE == (tr32 >> 16))
    return(ct[src]);
  else if(GAPI_T_MAGIC == (tr32 >> 16))
  {
    if(src == (uint16_t)tr32)
      return(dest);
    else
      return(ct[src]);
  }
  else 
    return(func( dest, ct[src], tr32));
}

/**********************************************************************************
Function Name:   CopyLine_01_Disp
Description:  Helper function to extract width pixels within one line of source 1bpp image to one Display line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              count: number of pixels to copy
              ct: color table to use
              tr32: transparency color for copy
Return value:   none
***********************************************************************************/
const uint8_t BitMask[]= {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
static int32_t CopyLine_01_Disp(pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t count, uint16_t const *const ct, const uint32_t tr32)
{
  copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);
  uint8_t bits = (uint8_t)(sPosX % 8);
  uint8_t pixel;

  /* offset to start positions */
  psr.p08 += (sPosX/8);
  pdr.pDisp += dPosX;

  for (pixel = *psr.p08; count > 0; count--, pdr.pDisp++, bits++)
  {
    uint8_t src;
    bits &= 0x07;

    if ((pixel & BitMask[bits]) == 0x00)
      src = 0;
    else
      src = 1;

    *pdr.pDisp = copy_pixel_index(*pdr.pDisp,src,ct,tr32, copy_func);

    if (bits == 0x07)
    {
      psr.p08++;         /* increment source pointer if done processing pixel byte */
      pixel = *psr.p08;  /* load next pixel byte */
    }
  }
  return(0);
}

/**********************************************************************************
Function Name:   CopyLine_04_Disp
Description:  Helper function to extract width pixels within one line of source 4bpp image to one Display line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              count: number of pixels to copy
              ct: color table to use
              tr32: transparency color for copy
Return value:   none
***********************************************************************************/
static int32_t CopyLine_04_Disp(pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t count, uint16_t const *const ct, const uint32_t tr32)
{
  copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);

  uint8_t bits = (uint8_t)(sPosX % 2);
  uint8_t pixel;
  /* offset to start positions */
  psr.p08 += (sPosX/2);
  pdr.pDisp += dPosX;

  for (; count > 0; count--, pdr.pDisp++, bits++)
  {
    pixel = *psr.p08;                      /* load pixel byte to temporary */

    if ((bits & 0x01) == 0x00)
    {
      pixel >>= 4;                    /* shift data down if high pixel bits */
    }
    else
    {
      pixel &= 0x0F;                  /* mask data if low pixel bits */
      psr.p08++;                           /* increment source pointer if done processing this pixel byte */
    }

    *pdr.pDisp = copy_pixel_index(*pdr.pDisp,pixel,ct,tr32, copy_func);
  }
  return(0);
}

/**********************************************************************************
Function Name:   AddLine_04rle_P
Description:  Helper function to offset number of "lines" into RLE bpp4 source image
Arguments:   psr: pointer to source BMP line
              Height: number of lines to seek end of
Return value:   number of source bytes used
              NOTE: THIS IS NOT STANDARD 4BPP RLE SUPPORT
***********************************************************************************/
static int32_t AddLine_04rle_P(const pRaster_t psr, int16_t Height)
{
  uint8_t const *ps = psr.p08;

  /* support 4bpp RLE compressed BMP */
  /* advance to Y offset line */
  for (; Height > 0; ps++)
  {
    if (*ps == 0x00)
    {
      /* found end of line marker */
      Height--;
    }
    if (*ps == 0x01)
    {
      /* end of BMP...nothing written */
      return(0);
    }
  } /* for */
  return((int32_t)(ps - psr.p08));
}

/**********************************************************************************
Function Name:   CopyLine_04rle_P_Disp
Description:  Helper function to blend width pixels within one line of source 
              Proprietary Run Length Encoded 4bpp image to one Display line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              Width: number of pixels to copy
              ct: color table to use for alpha mask
              tr32: foreground color for alpha mask (if used) or NO_TRANSPARENCY_COLOR
Return value:   number of source bytes used
              NOTE: THIS IS NOT STANDARD 4BPP RLE SUPPORT
***********************************************************************************/
static int32_t CopyLine_04rle_P_Disp(const pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t Width, uint16_t const *const ct, const uint32_t tr32)
{
  copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);
  uint8_t const *ps = psr.p08;
  int16_t IndexW = 0;
  /* advance destination to X offset */
  pdr.pDisp += dPosX;

  /* cycle through RLE records */
  for(;;)
  {
    switch (*ps)
    {
      case 0:
      {
        /* new line...fill to end with 0 value  */
        for (; IndexW < (sPosX+Width); IndexW++, pdr.pDisp++)
          *pdr.pDisp = copy_pixel_index(*pdr.pDisp,0,ct,tr32, copy_func);

        /* increment source pointer */
        ps += 1;

        /* line complete, so return */
        return((int32_t)(ps - psr.p08));
      }
      case 1:
      {
        /* end of BMP */
        return(0L);
      }
      case 2:
      {
        /* offset x,y (not sure how to handle this because of line skipping) */
        ps += 1;
        break;
      }
      default:
      {
        int16_t start, stop;
        uint8_t RLEr[16];

        if (*ps < 0x10)
        {
          /* discrete pixels (in 2x4 bit pairs) */
          start = 0;
          stop = (int16_t)(*ps & 0x0F);
        }
        else
        {
          /* repeat record */
          start = 0;
          stop = (int16_t)(*ps >> 4) & 0x0F;
        }

        /* check if we'll start copying in this record */
        if ((IndexW < sPosX) && ((IndexW+stop) >= sPosX))
          start = (sPosX - IndexW);

        /* check if we'll stop copying in this record */
        if ((IndexW < (sPosX+Width)) && ((IndexW+stop) >= (sPosX+Width)))
          stop -= (IndexW + stop) - (sPosX+Width);

        /* update the output raster...if we're still copying */
        if (IndexW < (sPosX+Width))
        {
          if (*ps < 0x10)
          {
            /* discrete pixels (in 2x4 bit pairs) */
            uint8_t index;
            /* expand RLE record for easy access */
            for (index = 0; index < 8; index++)
            {
              RLEr[index*2] = (ps[1+index] >> 4) & 0x0F;
              RLEr[index*2+1] = (ps[1+index] ) & 0x0F;
            }
            /* increment index position and pointer...extra + 1 for "0:size" */
            index = *ps & 0x0F;
            IndexW += index;
            ps += (((index-1)/2)+1+1);
          }
          else
          {
            /* repeat record */
            uint8_t index;
            /* expand RLE record for easy access */
            for (index = 0; index < 16; index++)
            {
              RLEr[index] = *ps & 0x0F;
            }
            /* increment index position and source pointer */
            IndexW += (*ps >> 4) & 0x0F;
            ps += 1;
          }

          for ( ; start<stop; start++, pdr.pDisp++)
            *pdr.pDisp = copy_pixel_index(*pdr.pDisp,RLEr[start],ct,tr32, copy_func);
        }
        else
        {
          /* not updating raster, but still must update source pointer */
          if (*ps < 0x10)
            ps += ((((*ps & 0x0F)-1)/2)+1+1);
          else
            ps += 1;
        }

        break;
      } /* default */

    } /* case */
  } /* for */
}

/**********************************************************************************
Function Name:   AddLine_04rle
Description:  Helper function to offset number of "lines" into RLE bpp4 source image
Arguments:   psr: pointer to source BMP line
              Height: number of lines to seek end of
Return value:   number of source bytes used
***********************************************************************************/
static int32_t AddLine_04rle(const pRaster_t psr, int16_t Height)
{
  uint8_t const *ps = psr.p08;

  /* support 8bpp RLE compressed BMP */
  /* advance to Y offset line */
  for (; Height > 0; )
  {
    if (ps[0] != 0)
    {
      /* repeat pixel record */
      ps += 2;
    }
    else
    {
      /* escape sequences */
      switch (ps[1])
      {
        case 0:
        {
          /* new line */
          ps += 2;
          Height--;
          break;
        }
        case 1:
        {
          /* end of BMP...nothing written */
          return(0);
        }
        case 2:
        {
          /* offset x,y (not sure how to handle this because of line skipping) */
          ps += 2;
          break;
        }
        default:
        {
          /* discrete pixels (in 16 bit quads) extra + 1 for "0:size" */
          ps += (((ps[1]-1)/4)+1+1)*2;
          break;
        } /* default */
      } /* switch */
    } /* if/else */
  } /* for */
  return((int32_t)(ps - psr.p08));
}

/**********************************************************************************
Function Name:   CopyLine_04rle_Disp
Description:  Helper function to extract width pixels within one line of source 
              Run Length Encoded 8bpp image to one 16bpp line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              Width: number of pixels to copy
              ct: color table to use
              tr32: transparency mode for copy
Return value:   number of source bytes used
***********************************************************************************/
static int32_t CopyLine_04rle_Disp(const pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t Width, uint16_t const *const ct, const uint32_t tr32)
{
  copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);
  uint8_t const *ps = psr.p08;
  int16_t IndexW = 0;
  
  /* advance destination to X offset */
  Raster_t *pd = pdr.pDisp + dPosX;

  /* cycle through RLE records */
  for(;;)
  {
    /* repeat pixel record */
    if (ps[0] != 0)
    {
      int16_t start = 0, stop = (int16_t)ps[0];

      /* check if we'll start copying in this record */
      if ((IndexW < sPosX) && ((IndexW+stop) >= sPosX))
        start = (sPosX - IndexW);

      /* check if we'll stop copying in this record */
      if ((IndexW < (sPosX+Width)) && ((IndexW+stop) >= (sPosX+Width)))
        stop -= (IndexW + stop) - (sPosX+Width);

      /* update the output raster...if we're still copying */
      if (IndexW < (sPosX+Width))
      {
        /* copy pixels */
        uint8_t src[2]={ps[1] >> 4, ps[1] & 0xF};
        for ( ; start<stop; start++, pd++)
          *pd = copy_pixel_index(*pd, src[start % 2], ct, tr32, copy_func);
      }

      /* increment index position and source pointer */
      IndexW += ps[0];
      ps += 2;
    }
    else
    {
      /* escape sequences */
      switch (ps[1])
      {
        case 0:
        {
          /* new line...fill to end with 0 value */
          for (; IndexW < (sPosX+Width); IndexW++, pd++)
            *pd = copy_pixel_index(*pd, 0, ct, tr32, copy_func);

          /* increment source pointer */
          ps += 2;

          /* line complete, so return */
          return((int32_t)(ps - psr.p08));
        }
        case 1:
        {
          /* end of BMP */
          return(0L);
        }
        case 2:
        {
          /* offset x,y (not sure how to handle this because of line skipping) */
          ps += 2;
          break;
        }
        default:
        {
          /* discrete pixels (in 16 bit quads) */
          int16_t start = 0, stop = (int16_t)ps[1];

          /* check if we'll start copying in this record */
          if ((IndexW < sPosX) && ((IndexW+stop) >= sPosX))
            start = (sPosX - IndexW);

          /* check if we'll stop copying in this record */
          if ((IndexW < (sPosX+Width)) && ((IndexW+stop) >= (sPosX+Width)))
            stop -= (IndexW + stop) - (sPosX+Width);

          /* update the output raster...if we're still copying */
          if (IndexW < (sPosX+Width))
          {
            for ( ; start<stop; start++, pd++)
            {
              uint8_t src[2]={ps[(start/2)+2] >> 4, ps[(start/2)+2] & 0xF};
              *pd = copy_pixel_index(*pd, src[start%2], ct, tr32, copy_func);
            }
          }
  
          /* increment index position and pointer...extra + 1 for "0:size" */
          IndexW += ps[1];
          ps += (((ps[1]-1)/4)+1+1)*2;
          break;
        } /* default */
      } /* switch */
    } /* if */
  } /* for */
}

/**********************************************************************************
Function Name:   CopyLine_08_Disp
Description:  Helper function to extract width pixels within one line of source 8bpp image to one Display line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              count: number of pixels to copy
              ct: color table to use
              tr32: transparency color for copy
Return value:   none
***********************************************************************************/
static int32_t CopyLine_08_Disp(pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t count, uint16_t const *const ct, const uint32_t tr32)
{
  copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);

  /* offset to start position */
  psr.p08 += sPosX;
  pdr.pDisp += dPosX;

  for (; count > 0; count--, psr.p08++, pdr.pDisp++)
    *pdr.pDisp = copy_pixel_index(*pdr.pDisp, *psr.p08,ct,tr32, copy_func);
    
  return(0);
}

/**********************************************************************************
Function Name:   CopyLine_Disp_Disp
Description:  Helper function to extract width pixels within one line of source Display image to one Display line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              count: number of pixels to copy
              ct: color table to use
              tr32: transparency color for copy
Return value:   none
***********************************************************************************/
//lint -e{715} suppress ct not used
static int32_t CopyLine_Disp_Disp(pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t count, uint16_t const *const ct, const uint32_t tr32)
{
  /* offset to start position */
  psr.pDisp += sPosX;
  pdr.pDisp += dPosX;

  if (tr32 == NO_TRANSPARENCY_COLOR)
  {
    /* using memcpy takes advantage of  movm instructions */
    (void)memcpy(pdr.pDisp, psr.pDisp, count*sizeof(Raster_t));
  }
  else
  {
    /* copy with transparency */
    copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);
    for (; count > 0; count--, psr.pDisp++, pdr.pDisp++)
    {
      /* write pixel */
      *pdr.pDisp = copy_pixel_565(*pdr.pDisp, *psr.pDisp, tr32, copy_func);
    }
  }
  return(0);
}

/**********************************************************************************
Function Name:   CopyLine_16_Disp
Description:  Helper function to extract width pixels within one line of source 16bpp image to one Display line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              count: number of pixels to copy
              ct: color table to use
              tr32: transparency color for copy
Return value:   none
***********************************************************************************/
static int32_t CopyLine_16_Disp(pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t count, uint16_t const *const ct, const uint32_t tr32)
{
  copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);
  
  /* offset to start position */
  pdr.pDisp += dPosX;
  psr.p16 += sPosX;

  #ifdef __LIT
    /* optimize little endian 16bpp transfers */
    if ((tr32 == NO_TRANSPARENCY_COLOR) && (sizeof(Raster_t)==sizeof(short)))
    {
      /* using memcpy takes advantage of  movm instructions */
      (void)memcpy(pdr.pDisp, psr.pDisp, count*sizeof(Raster_t));
      return(0);
    }
  #endif
  for (; count > 0; count--, psr.p16++, pdr.pDisp++)
  {
    /* write pixel */
    *pdr.pDisp = copy_pixel_565(*pdr.pDisp, (psr.p08[1]<<8)+psr.p08[0], tr32, copy_func);
  }
  return(0);
}

/**********************************************************************************
Function Name:   CopyLine_24_Disp
Description:  Helper function to extract width pixels within line of source 24bpp image to one Display line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              count: number of pixels to copy
              ct: color table to use
              tr32: transparency color for copy
Return value:   none
***********************************************************************************/
//lint -e{715} suppress ct not used
static int32_t CopyLine_24_Disp(pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t count, uint16_t const *const ct, const uint32_t tr32)
{
  copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);
  /* offset to start position */
  pdr.pDisp += dPosX;
  psr.p24 += sPosX;
  
  for (; count > 0; count--, psr.p24++, pdr.pDisp++)
    *pdr.pDisp = copy_pixel_565(*pdr.pDisp, R_GAPI_24_Disp( psr.pCT ), tr32, copy_func);

  return(0);
}

/**********************************************************************************
Function Name:   CopyLine_32_Disp
Description:  Helper function to extract width pixels within line of source 32bpp (8888) image to one Display line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              count: number of pixels to copy
              ct: color table to use
              tr32: transparency color for copy
Return value:   none
***********************************************************************************/
//lint -e{715} suppress ct not used
static int32_t CopyLine_32_Disp(pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t count, uint16_t const *const ct, const uint32_t tr32)
{
  copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);
  /* keep transparency type only */
  uint32_t tr_type = tr32 & 0xFFFF0000;
  
  /* offset to start position */
  pdr.pDisp += dPosX;
  psr.p32 += sPosX;
  for (; count > 0; count--, psr.p32++, pdr.pDisp++)
    *pdr.pDisp = copy_pixel_565(*pdr.pDisp, R_GAPI_24_Disp( psr.pCT ), tr_type + CT8_grayscale[psr.pCT[0].channel[3]], copy_func);

  return(0);
}

/**********************************************************************************
Function Name:   AddLine_08rle
Description:  Helper function to offset number of "lines" into RLE bpp8 source image
Arguments:   psr: pointer to source BMP line
              Height: number of lines to seek end of
Return value:   number of source bytes used
***********************************************************************************/
static int32_t AddLine_08rle(const pRaster_t psr, int16_t Height)
{
  uint8_t const *ps = psr.p08;

  /* support 8bpp RLE compressed BMP */
  /* advance to Y offset line */
  for (; Height > 0; )
  {
    if (ps[0] != 0)
    {
      /* repeat pixel record */
      ps += 2;
    }
    else
    {
      /* escape sequences */
      switch (ps[1])
      {
        case 0:
        {
          /* new line */
          ps += 2;
          Height--;
          break;
        }
        case 1:
        {
          /* end of BMP...nothing written */
          return(0);
        }
        case 2:
        {
          /* offset x,y (not sure how to handle this because of line skipping) */
          ps += 2;
          break;
        }
        default:
        {
          /* discrete pixels (in 16 bit pairs) extra + 1 for "0:size" */
          ps += (((ps[1]-1)/2)+1+1)*2;
          break;
        } /* default */
      } /* switch */
    } /* if/else */
  } /* for */
  return((int32_t)(ps - psr.p08));
}

/**********************************************************************************
Function Name:   CopyLine_08rle_Disp
Description:  Helper function to extract width pixels within one line of source 
              Run Length Encoded 8bpp image to one 16bpp line at destination
Arguments:   psr: pointer to source BMP line
              sPosX: offset within source line to copy from
              pdr: pointer to destination BMP line
              dPosX: offset within destination line to copy to
              Width: number of pixels to copy
              ct: color table to use
              tr32: transparency mode for copy
Return value:   number of source bytes used
***********************************************************************************/
static int32_t CopyLine_08rle_Disp(const pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t Width, uint16_t const *const ct, const uint32_t tr32)
{
  copy_pixel_func_type copy_func = copy_pixel_chooser(tr32);
  uint8_t const *ps = psr.p08;
  int16_t IndexW = 0;
  
  /* advance destination to X offset */
  Raster_t *pd = pdr.pDisp + dPosX;

  /* cycle through RLE records */
  for(;;)
  {
    /* repeat pixel record */
    if (ps[0] != 0)
    {
      int16_t start = 0, stop = (int16_t)ps[0];

      /* check if we'll start copying in this record */
      if ((IndexW < sPosX) && ((IndexW+stop) >= sPosX))
        start = (sPosX - IndexW);

      /* check if we'll stop copying in this record */
      if ((IndexW < (sPosX+Width)) && ((IndexW+stop) >= (sPosX+Width)))
        stop -= (IndexW + stop) - (sPosX+Width);

      /* update the output raster...if we're still copying */
      if (IndexW < (sPosX+Width))
      {
        /* copy pixels */
        uint8_t src=ps[1];
        for ( ; start<stop; start++, pd++)
          *pd = copy_pixel_index(*pd, src, ct, tr32, copy_func);
      }

      /* increment index position and source pointer */
      IndexW += ps[0];
      ps += 2;
    }
    else
    {
      /* escape sequences */
      switch (ps[1])
      {
        case 0:
        {
          /* new line...fill to end with 0 value */
          for (; IndexW < (sPosX+Width); IndexW++, pd++)
            *pd = copy_pixel_index(*pd, 0, ct, tr32, copy_func);

          /* increment source pointer */
          ps += 2;

          /* line complete, so return */
          return((int32_t)(ps - psr.p08));
        }
        case 1:
        {
          /* end of BMP */
          return(0L);
        }
        case 2:
        {
          /* offset x,y (not sure how to handle this because of line skipping) */
          ps += 2;
          break;
        }
        default:
        {
          /* discrete pixels (in 16 bit pairs) */
          int16_t start = 0, stop = (int16_t)ps[1];

          /* check if we'll start copying in this record */
          if ((IndexW < sPosX) && ((IndexW+stop) >= sPosX))
            start = (sPosX - IndexW);

          /* check if we'll stop copying in this record */
          if ((IndexW < (sPosX+Width)) && ((IndexW+stop) >= (sPosX+Width)))
            stop -= (IndexW + stop) - (sPosX+Width);

          /* update the output raster...if we're still copying */
          if (IndexW < (sPosX+Width))
          {
            for ( ; start<stop; start++, pd++)
              *pd = copy_pixel_index(*pd, ps[start+2], ct, tr32, copy_func);
          }
  
          /* increment index position and pointer...extra + 1 for "0:size" */
          IndexW += ps[1];
          ps += (((ps[1]-1)/2)+1+1)*2;
          break;
        } /* default */
      } /* switch */
    } /* if */
  } /* for */
}

/**********************************************************************************
Function Name:   BMPCopySub
Description:  BMP to BMP copy with additional hooks
Arguments:   s: pointer to source BMP
              d: pointer to destination BMP
              dPosX: starting X coordinate within destination BMP (leftmost = 0)
              dPosY: starting Y coordinate within destination BMP (bottommost = 0)
              sPosX: starting X coordinate within source BMP (leftmost = 0)
              sPosY: starting Y coordinate within source BMP (bottommost = 0)
              Width: "width" of source area to copy
              Height: "height" of source area to copy
              ct: Pointer to color table to use (allows substituion of source color tables)
              tr: Transparency color to use if NO_TRANSPARENCY_COLOR no transparency handling is used
Return value:   0:OK, Not 0: NG
***********************************************************************************/
GAPI_Resp_t R_GAPI_CopySub( BMP_t const *const s, BMP_t *const d, int16_t dPosX, int16_t dPosY, int16_t sPosX, int16_t sPosY, int16_t Width, int16_t Height, BMP_ColorTable_t const *const ct, uint32_t tr32 )
{
  const int16_t sbiHeight = R_BMP_Height(s);
  int16_t sbiWidth = R_BMP_Width(s);

  const int16_t dbiHeight = R_BMP_Height(d);
  int16_t dbiWidth = R_BMP_Width(d);

  const uint32_t sFormat = R_BMP_Format(s);
  const uint32_t dFormat = R_BMP_Format(d);

  pRaster_t psRaster, pdRaster;
  int32_t (*copy_func)(const pRaster_t psr, const int16_t sPosX, pRaster_t pdr, const int16_t dPosX, int16_t count, uint16_t const *const ct, const uint32_t tr) = NULL;

  if((sFormat == 0) || (dFormat == 0))
    return(-1);
    
  /* only handling display native destination */
  if ((BMP_BPP_DISP | BMP_BPC_DISP) != dFormat)
    return(-1);

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

  if ((Height < 0) || (Width < 0))
    return(-1);

  /* convert width in pixels to width in bytes */
  dbiWidth = (int16_t)((((int32_t)dbiWidth * (int32_t)d->biBitCount[0] - 1L)/8L) + 1L);
  sbiWidth = (int16_t)((((int32_t)sbiWidth * (int32_t)s->biBitCount[0] - 1L)/8L) + 1L);
  
  /* create a quad byte line width for "standard" BMPs */
  if (!isBMPF(d, NO_QUAD))
    dbiWidth = (((dbiWidth - 1)/4) + 1) * 4;
  
  if (!isBMPF(s, NO_QUAD))
    sbiWidth = (((sbiWidth - 1)/4) + 1) * 4;

  /* point to source raster data */
  psRaster.p08 = ((uint8_t *)s) + R_BMP_Offset(s);

  /* point to destination raster data */
  pdRaster.p08 = ((uint8_t *)d) + R_BMP_Offset(d);

  /* point to first destination line */
  pdRaster.p08 += (int32_t)dbiWidth * (int32_t)dPosY;

  /* determine how to handle source file */
  switch (sFormat)
  {
    case BMP_BPP_1|BMP_BPC_0565|BMP_INDEX:
    case BMP_BPP_1|BMP_BPC_0888|BMP_INDEX:
    {
      /* Standard 1BPP raster data */
      copy_func = CopyLine_01_Disp;
      break;
    }

    case BMP_BPP_4|BMP_BPC_0888|BMP_INDEX:
    case BMP_BPP_4|BMP_BPC_0565|BMP_INDEX:
    {
      /* Standard 4BPP raster data */
      copy_func = CopyLine_04_Disp;
      break;
    }
    
    case BMP_BPP_4|BMP_BPC_0888|BMP_INDEX|BMP_RLE_P|BMP_RLE:
    case BMP_BPP_4|BMP_BPC_0565|BMP_INDEX|BMP_RLE_P|BMP_RLE:
    {
      /* Proprietary 4BPP RLE raster data (for alpha mask) */
      copy_func = CopyLine_04rle_P_Disp;
      /* point to first source line */
      psRaster.p08 += AddLine_04rle_P(psRaster, sPosY);
      break;
    }
    
    case BMP_BPP_4|BMP_BPC_0888|BMP_INDEX|BMP_RLE:
    case BMP_BPP_4|BMP_BPC_0565|BMP_INDEX|BMP_RLE:
    {
      /* Standard 4BPP RLE raster data */
      copy_func = CopyLine_04rle_Disp;
      /* point to first source line */
      psRaster.p08 += AddLine_04rle(psRaster, sPosY);
      break;
    }
    
    case BMP_BPP_8|BMP_BPC_0565|BMP_INDEX:
    case BMP_BPP_8|BMP_BPC_0888|BMP_INDEX:
    {
      /* Standard 8BPP raster data */
      copy_func = CopyLine_08_Disp;
      break;
    }
    
    case BMP_BPP_8|BMP_BPC_0565|BMP_INDEX|BMP_RLE:
    case BMP_BPP_8|BMP_BPC_0888|BMP_INDEX|BMP_RLE:
    {
      /* Standard 8BPP RLE raster data */
      copy_func = CopyLine_08rle_Disp;
      /* point to first source line */
      psRaster.p08 += AddLine_08rle(psRaster, sPosY);
      break;
    }
    
    case BMP_BPP_16|BMP_BPC_0565:
    {
      copy_func = CopyLine_16_Disp;
      break;
    }
    
    case BMP_BPP_DISP|BMP_BPC_DISP:
    {
      copy_func = CopyLine_Disp_Disp;
      break;
    }
    
    case BMP_BPP_24|BMP_BPC_0888:
    {
      copy_func = CopyLine_24_Disp;
      break;
    }
    
    case BMP_BPP_32|BMP_BPC_8888:
    {
      copy_func = CopyLine_32_Disp;
      break;
    }
    
    default:
      return(-1);
  }
  
  /* point to first source line (if not RLE) */
  if (BMP_RLE != (sFormat & BMP_RLE))
      psRaster.p08 += (int32_t)sbiWidth * (int32_t)sPosY;
      

  {
    uint16_t *pCT16 = (uint16_t *) ct;
    uint16_t *pLocalCT16 = NULL;
    uint16_t index;
    
    /* Create a 16bpp color table (if not already 16bpp color table)
      ...this greatly accelerates most BMP operations */
    if ((BMP_INDEX == (sFormat & BMP_INDEX)) &&
        (BMP_BPC_0888 == (sFormat & BMP_BPC_MASK))) 
    {
      uint16_t ct_size=2;
      if(BMP_BPP_4 == (sFormat & BMP_BPP_MASK))
        ct_size = 16;
        
      if(BMP_BPP_8 == (sFormat & BMP_BPP_MASK))
        ct_size = 256;
        
      // Acquire scratchpad memory to create a 16bpp color table
      pLocalCT16 = (void *)R_GAPI_PoolMemoryGet(1024);

      if(pLocalCT16 == NULL)
        return(-1);
        
      /* assign 16bpp color table to this memory */
      pCT16 = pLocalCT16;
      /* convert the color table */
      for (index=0; index<ct_size; index++)
        pCT16[index] = R_GAPI_24_Disp(&ct[index]);
    }
  
    for ( ; Height > 0; Height-- )
    { 
      /* copy width count of pixels */
      const int32_t value = copy_func(psRaster, sPosX, pdRaster, dPosX, (int16_t)Width, pCT16, tr32);

      /* increment source pointer */
      if (BMP_RLE == (sFormat & BMP_RLE))
        psRaster.p08 += value;    /* RLE */
      else
        psRaster.p08 += sbiWidth; /* non-RLE */

      /* increment destination pointer */
      pdRaster.p08 += dbiWidth;
    }
    (void)R_GAPI_PoolMemoryFree((void *)pLocalCT16);
  }

  return(0);
}


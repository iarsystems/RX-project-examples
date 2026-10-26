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
FILE NAME    : r_GAPIu_BMP.c

DESCRIPTION  : BMP Information Support for Renesas Graphics API.

Copyright   : 2010 Renesas Electronics America
Copyright   : 2010 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/

#include "r_gapi_private.h"

static void BMP_Write32(uint32_t value, uint32_t *d);

/**********************************************************************************
Function Name:   R_BMP_Init
Description:  Create a BMP header in destination
Arguments:   d: pointer to destination BMP
              r: pointer to raster
              bpp: bits per pixel of BMP 
              (it is assumed color table will be outside of BMP...in RAM)
              Width: width of BMP in pixels
              Height: height of BMP in pixels
Return value:   0:OK, Not 0: NG
***********************************************************************************/

static const BMP_t BMP_header=
{
  {0x42, 0x4D},                 /* bfSignature[2]:            "BM"                                    */
  {0x48, 0x58, 0x02, 0x00},     /* bfFileSize[4]:             0x00025848 <153,672 bytes files size>   */
  {0x00, 0x00, 0x00, 0x00},     /* bfReserved[4]:             0x00000000                              */
  {0x3A, 0x00, 0x00, 0x00},     /* bfDataOffset[4]:           0x0000003A <58 bytes offset to raster>  */
  {0x38, 0x00, 0x00, 0x00},     /* biSize[4]:                 0x00000038 <56 bytes header size>       */
  {0x40, 0x01, 0x00, 0x00},     /* biWidth[4]:                0x00000140 <320 bytes wide>             */
  {0xF0, 0x00, 0x00, 0x00},     /* biHeight[4]:               0x000000F0 <240 bytes high>             */
  {0x01, 0x00},                 /* biPlanes[2]:                   0x0001 <1 plane>                    */
  {0x10, 0x00},                 /* biBitCount[2]:                 0x0010 <16bpp>                      */
  {0x00, 0x00, 0x00, 0x00},     /* biCompression[4]:          0x00000000 <BIT fields...no compress>   */
  {0x02, 0x58, 0x02, 0x00},     /* biImageSize[4]:            0x00025802 <153,602 bytes raster size>  */
  {0x12, 0x0B, 0x00, 0x00},     /* biXPixelsPerMeter[4]:      0x00000B12 <2834 pixel/m, 72 pixel/inch */
  {0x12, 0x0B, 0x00, 0x00},     /* biYPixelsPerMeter[4]:      0x00000B12 <2834 pixel/m, 72 pixel/inch */
  {0x00, 0x00, 0x00, 0x00},     /* biClrUsed[4]:              0x00000000 <0 index colors used>        */
  {0x00, 0x00, 0x00, 0x00},     /* biClrImportant[4]:         0x00000000 <0 index colors important>   */
  {                             /* Table_type biColorTable[4]:           <Bit field RGB definition>   */
    {0x00, 0xF8, 0x00, 0x00},   /* 5 bits of RED */
    {0xE0, 0x07, 0x00, 0x00},   /* 6 bits of GREEN */
    {0x1F, 0x00, 0x00, 0x00},   /* 5 bits of BLUE */
    {0x00, 0x00, 0x00, 0x00},   /* 0 bits of ALHA */
  }            
};

GAPI_Resp_t R_BMP_Init(BMP_t *const d, uint32_t r, const uint8_t bpp, const int16_t sWidth, const int16_t sHeight)
{
  uint32_t value;
  uint16_t Width = (uint16_t)sWidth;
  uint16_t Height = (uint16_t)sHeight;
  
  /* copy over a baseline of header */
  *d = BMP_header;

  /* check bpp */
  switch(bpp)
  {
    case BMP_DISP_BPP:            /* "proprietary" output display format */
      d->biBitCount[0] = sizeof(Raster_t)*8;
      setBMPF(d,DISPLAY);          /* stored "native" display format */
      break;
    case BMP_8NQ_BPP:             /* "proprietary" 8bpp font format */
      d->biBitCount[0] = 8;
      setBMPF(d,NO_QUAD);         /* stored with no quad restriction */
      break;
    case BMP_4NQ_BPP:             /* "proprietary" 4bpp font format */
      d->biBitCount[0] = 4;
      setBMPF(d,NO_QUAD);         /* stored with no quad restriction */
      break;
    case BMP_1NQ_BPP:             /* "proprietary" 1bpp font format */
      d->biBitCount[0] = 1;
      setBMPF(d,NO_QUAD);         /* stored with no quad restriction */
      break;

    case 24:                      /* standard 24bpp */
      d->biBitCount[0] = 24;
      break;

    case 16:
      d->biBitCount[0] = 16;
      d->biCompression[0] = 0x3;   /* standard 16bpp 5:6:5 */
      break;

    case 8:                      /* standard 8bpp index */
      d->biBitCount[0] = 24;
      break;

    case 4:                      /* standard 4bpp index */
      d->biBitCount[0] = 24;
      break;

    case 1:                      /* standard 1bpp index */
      d->biBitCount[0] = 24;
      break;

    default:
      return(-1);
  }
  
  BMP_Write32(Width,(uint32_t *) d->biWidth);
  
  BMP_Write32(Height,(uint32_t *) d->biHeight);
  
  /* determine number of longwords in file size */
  value = ((((uint32_t)Width * (uint32_t)Height * (uint32_t)bpp) + ((uint32_t)sizeof(BMP_header) * 8) -1)/32)+1;
  /* number of bytes in file size */
  value *= 4UL;
  BMP_Write32(value,(uint32_t *) d->bfFileSize);
 
  value -= sizeof(BMP_header);
  BMP_Write32(value,(uint32_t *) d->biImageSize);

  value = (uint32_t)r - (uint32_t)d;
  BMP_Write32(value,(uint32_t *) d->bfDataOffset);
  return(0);
}

/**********************************************************************************
Function Name:   BMP_xxxx
Description:  Provide information about a BMP
Arguments:   pBmp: pointer to BMP
Return value:   Attribute requested from BMP
***********************************************************************************/
typedef union
{
  uint32_t w;
  uint8_t b[4];
} tWordWrite;

static void BMP_Write32(uint32_t value, uint32_t *d)
{
  tWordWrite temp;
  temp.b[0] = (uint8_t)(value >> 0);
  temp.b[1] = (uint8_t)(value >> 8);
  temp.b[2] = (uint8_t)(value >>16);
  temp.b[3] = (uint8_t)(value >>24);
  *d = temp.w;
}

int16_t R_BMP_Height(BMP_t const *const pBmp)
{
  if(pBmp == NULL)
    return(0);
  return((int16_t)(((uint16_t)pBmp->biHeight[1] << 8) + (uint16_t)pBmp->biHeight[0]));
}

int16_t R_BMP_Width(BMP_t const *const pBmp)
{
  if(pBmp == NULL)
    return(0);
  return((int16_t)(((uint16_t)pBmp->biWidth[1] << 8) + (uint16_t)pBmp->biWidth[0]));
}

int32_t R_BMP_Offset(BMP_t const *const pBmp)
{
  if(pBmp == NULL)
    return(0);
  return((int32_t)(((uint32_t)pBmp->bfDataOffset[3] << 24)+((uint32_t)pBmp->bfDataOffset[2] << 16)+((uint32_t)pBmp->bfDataOffset[1] << 8)+((uint32_t)pBmp->bfDataOffset[0] << 0)));
}

uint32_t R_BMP_FileSize(BMP_t const *const pBmp)
{
  if(pBmp == NULL)
    return(0);
  return(((uint32_t)pBmp->bfFileSize[3] << 24)+((uint32_t)pBmp->bfFileSize[2] << 16)+((uint32_t)pBmp->bfFileSize[1] << 8)+((uint32_t)pBmp->bfFileSize[0] << 0));
}

BMP_ColorTable_t const *R_BMP_Ct(BMP_t const *const pBmp)
{
  return(pBmp->biColorTable);
}

uint32_t R_BMP_IndexColors(BMP_t const *const pBmp)
{
  uint32_t biClrImportant;
  if(pBmp == NULL)
    return(0);

  biClrImportant = ((uint32_t)pBmp->biClrImportant[3] << 24)+((uint32_t)pBmp->biClrImportant[2] << 16)+((uint32_t)pBmp->biClrImportant[1] << 8)+((uint32_t)pBmp->biClrImportant[0] << 0);
  if (0 == biClrImportant)
  {
    /* Check if this is an indexed format */
    if(pBmp->biBitCount[0] <= 8)
      biClrImportant = 1UL << pBmp->biBitCount[0];
  }
  return(biClrImportant);
}

static const BMP_ColorTable_t bitfields_table[][4]=
{
  {{0x00, 0xF8, 0x00, 0x00}, {0xE0, 0x07, 0x00, 0x00}, {0x1F, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, /* BMP_BPC_0565 */
  {{0x00, 0x7C, 0x00, 0x00}, {0xE0, 0x03, 0x00, 0x00}, {0x1F, 0x00, 0x00, 0x00}, {0x00, 0x80, 0x00, 0x00}}, /* BMP_BPC_1555 */
  {{0x00, 0x7C, 0x00, 0x00}, {0xE0, 0x03, 0x00, 0x00}, {0x1F, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, /* BMP_BPC_0555 (also if no biBitfield flag) */
  {{0x00, 0x0F, 0x00, 0x00}, {0xF0, 0x00, 0x00, 0x00}, {0x0F, 0x00, 0x00, 0x00}, {0x00, 0xF0, 0x00, 0x00}}, /* BMP_BPC_4444 */
  {{0x00, 0x0F, 0x00, 0x00}, {0xF0, 0x00, 0x00, 0x00}, {0x0F, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, /* BMP_BPC_0444 */
  {{0x00, 0x00, 0x00, 0xFF}, {0x00, 0x00, 0xFF, 0x00}, {0x00, 0xFF, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, /* BMP_BPC_0888 */
  {{0x00, 0x00, 0xFF, 0x00}, {0x00, 0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0xFF}}, /* BMP_BPC_8888 (also if no biBitfield flag*/
};

static int bitfield_test(BMP_ColorTable_t const * const s, int16_t vector)
{
  int index;
  for (index = 0; index < 4; index++)
  {
    if(*s[index].channel != *bitfields_table[vector][index].channel)
      return(-1);
  }
  return (0);
}

uint32_t R_BMP_Format(BMP_t const *const pBmp)
{
  uint32_t format = 0;
  if(pBmp == NULL)
    return(0);
  /* check bits per pixel */
  switch (pBmp->biBitCount[0])
  {
    case 1:
      format += BMP_BPP_1;
      format += BMP_INDEX;
      
      /* check for 16 bpp color table */
      if(isBMPF(pBmp,16BPP_CT))
        format += BMP_BPC_0565;
      else
        format += BMP_BPC_0888;

      break;
      
    case 4:
      format += BMP_BPP_4;
      format += BMP_INDEX;
      
      /* check for 16 bpp color table */
      if(isBMPF(pBmp,16BPP_CT))
        format += BMP_BPC_0565;
      else
        format += BMP_BPC_0888;

      /* check for proprietary RLE compression */   
      if (isBMPF(pBmp,4BPP_RLE))
        format += BMP_RLE_P + BMP_RLE;

      if (2 == pBmp->biCompression[0])
        format += BMP_RLE;
      break;
      
    case 8:
      format += BMP_BPP_8;
      format += BMP_INDEX;
      
      /* check for 16 bpp color table */
      if(isBMPF(pBmp,16BPP_CT))
        format += BMP_BPC_0565;
      else
        format += BMP_BPC_0888;

      if (1 == pBmp->biCompression[0])
        format += BMP_RLE;
      break;
      
    case 16:
      if(isBMPF(pBmp,DISPLAY))
      {
        format += BMP_BPP_DISP;
        format += BMP_BPC_DISP;
        break;
      }
      
      format += BMP_BPP_16;
      if(0 == pBmp->biCompression[0])
        format += BMP_BPC_0555;
      else if(3 == pBmp->biCompression[0])
      {
        if(0 == bitfield_test(pBmp->biColorTable, 0))
          format += BMP_BPC_0565;
        else if(0 == bitfield_test(pBmp->biColorTable, 1))
          format += BMP_BPC_1555;
        else if(0 == bitfield_test(pBmp->biColorTable, 2))
          format += BMP_BPC_0555;
        else if(0 == bitfield_test(pBmp->biColorTable, 3))
          format += BMP_BPC_4444;
        else if(0 == bitfield_test(pBmp->biColorTable, 4))
          format += BMP_BPC_0444;
        else
          return(0);
      }
      else
        return(0);
      break;
      
    case 24:
      format += BMP_BPP_24;
      format += BMP_BPC_0888;
      break;
      
    case 32:
      if(isBMPF(pBmp,DISPLAY))
      {
        format += BMP_BPP_DISP;
        format += BMP_BPC_DISP;
        break;
      }
      
      format += BMP_BPP_32;
      if(0 == pBmp->biCompression[0])
        format += BMP_BPC_8888;
      else if(3 == pBmp->biCompression[0])
      {
        if(0 == bitfield_test(pBmp->biColorTable, 5))
          format += BMP_BPC_0888;
        else if(0 == bitfield_test(pBmp->biColorTable, 6))
          format += BMP_BPC_8888;
        else
          return(0);
      }
      else
        return(0);
      
      break;
  }
  return(format);
}

/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/

#ifndef R_GAPIU_UTILITY_H
#define R_GAPIU_UTILITY_H
/***********************************************************************************
FILE NAME    : r_GAPI_utility.h

DESCRIPTION  : Utility structure definitions and function prototypes for Renesas Graphics API.

Copyright   : 2010 Renesas Electric America
Copyright   : 2010 Renesas Electric Corporation.
          All Rights Reserved
***********************************************************************************/
/* Typedef fundemental data types that may change between compilers/families */
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

typedef int16_t GAPI_Resp_t;
typedef struct GFont_struct GAPI_Font_t;
typedef struct BMP_struct BMP_t;


/* Data structure of BMP color table entry */
#define BMP_CT_BLUE 0
#define BMP_CT_GREEN 1
#define BMP_CT_RED 2
typedef struct 
{
  uint8_t channel[4];
} BMP_ColorTable_t;

/* BMP header structure */
struct BMP_struct
{
  uint8_t bfSignature[2];
  uint8_t bfFileSize[4];
  uint8_t bfReserved[4];
  uint8_t bfDataOffset[4];
  
  uint8_t biSize[4];
  uint8_t biWidth[4];
  uint8_t biHeight[4];
  uint8_t biPlanes[2];
  uint8_t biBitCount[2];
  uint8_t biCompression[4];
  uint8_t biImageSize[4];
  uint8_t biXPixelsPerMeter[4];
  uint8_t biYPixelsPerMeter[4];
  uint8_t biClrUsed[4];
  uint8_t biClrImportant[4];
  BMP_ColorTable_t biBitMasks[4];
};

/* Function prototypes */
/* Values passed back by R_BMP_Format can be evaluated with these macros */
/* bits per pixel in raster */
#define BMP_BPP_1     0x00000001
#define BMP_BPP_4     0x00000002
#define BMP_BPP_8     0x00000003
#define BMP_BPP_16    0x00000004
#define BMP_BPP_24    0x00000005
#define BMP_BPP_32    0x00000006
#define BMP_BPP_DISP  0x00000007    /* whatever format the display is */
#define BMP_BPP_MASK  0x0000000F

/* bits per color alpha, red, green blue */
#define BMP_BPC_0888    0x00000010
#define BMP_BPC_8888    0x00000020
#define BMP_BPC_0444    0x00000030
#define BMP_BPC_4444    0x00000040
#define BMP_BPC_0565    0x00000050
#define BMP_BPC_DISP    0x00000060  /* whatever format the display is */
#define BMP_BPC_0555    0x00000070
#define BMP_BPC_1555    0x00000080
#define BMP_BPC_MASK    0x000000F0

#define BMP_INDEX       0x00000100
#define BMP_RLE         0x00000200
#define BMP_RLE_P       0x00000400  /* Proprietary RLE for 4 bit fonts */
uint32_t R_BMP_Format(BMP_t const *const pBmp);

/* BPP values to initialize proprietary BMP formats (for R_BMP_Init) */
#define BMP_DISP_BPP 0x90
#define BMP_8NQ_BPP 0x88
#define BMP_4NQ_BPP 0x84
#define BMP_1NQ_BPP 0x81
GAPI_Resp_t R_BMP_Init(BMP_t *const d, uint32_t r, const uint8_t bpp, const int16_t Width, const int16_t Height);

int16_t R_BMP_Height(BMP_t const *const pBmp);
int16_t R_BMP_Width(BMP_t const *const pBmp);
int32_t R_BMP_Offset(BMP_t const *const pBmp);
uint32_t R_BMP_FileSize(BMP_t const *const pBmp);
uint32_t R_BMP_IndexColors(BMP_t const *const pBmp);
BMP_ColorTable_t const *R_BMP_Ct(BMP_t const *const pBmp);

/* define a function for GAPI to allocate memory */
uint8_t * R_GAPI_PoolMemoryGet(uint16_t size);
uint8_t R_GAPI_PoolMemoryFree(uint8_t *pPool);


/**********************************************************************************
Function Name:   R_GAPI_24_Disp
Description:  converts 24bpp to 16bpp
              useful for generating transparency color
Arguments:   p24: pointer to source pixel
Return value:   16bpp equivalent
***********************************************************************************/
#ifdef HW_BYTE_SWAPPED_BUS
static inline uint16_t R_GAPI_24_Disp( BMP_ColorTable_t const *const p24 )
{
  uint16_t Tout;
  BMP_ColorTable_t local_ct = *p24;
  Tout  = (((uint16_t)local_ct.channel[BMP_CT_GREEN]) << 11) & 0xE000u;
  Tout |= (((uint16_t)local_ct.channel[BMP_CT_GREEN]) >>  5) & 0x0007u;
  Tout |= (((uint16_t)local_ct.channel[BMP_CT_BLUE] ) <<  5) & 0x1F00u;
  Tout |= (((uint16_t)local_ct.channel[BMP_CT_RED]  ) <<  0) & 0x00F8u;

  return(Tout);
}
#else
static inline uint16_t R_GAPI_24_Disp( BMP_ColorTable_t const *const p24 )
{
  uint16_t Tout;
  BMP_ColorTable_t local_ct = *p24;
  Tout  = (((uint16_t)local_ct.channel[BMP_CT_GREEN]) <<  3) & 0x07E0u;
  Tout |= (((uint16_t)local_ct.channel[BMP_CT_BLUE] ) >>  3) & 0x001Fu;
  Tout |= (((uint16_t)local_ct.channel[BMP_CT_RED]  ) <<  8) & 0xF800u;

  return(Tout);
}
#endif

#endif

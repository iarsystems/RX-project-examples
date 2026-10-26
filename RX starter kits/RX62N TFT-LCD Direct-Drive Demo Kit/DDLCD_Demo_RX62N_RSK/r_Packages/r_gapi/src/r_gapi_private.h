/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/

#ifndef R_GAPI_PRIVATE_H
#define R_GAPI_PRIVATE_H
/***********************************************************************************
FILE NAME    : r_GAPI_internal.h

DESCRIPTION  : Internal Structure definitions and function prototypes for Renesas Graphics API.

Copyright   : 2007 Renesas Electronics America
Copyright   : 2007 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/
#include "r_gapiu_utility.h"
#include "r_gapic_core.h"
#include "r_gapiw_wrapper.h"
#include "r_gapic_font.h"
#include "config_r_gapi.h"

extern const uint16_t CT8_grayscale[256];
extern const uint16_t CT4_grayscale[16];

/* font data structure */
struct GFont_struct
{
  GAPI_Efont_t base;
  GAPI_Eglyph_t glyphs[1];  /* this will really be an array of as many glyphs in font */
};

/* internal BMP flags...stored in biCompression[1] */
#define BMP_DISPLAY 0x80
#define BMP_NO_QUAD 0x40
#define BMP_16BPP_CT 0x10
#define BMP_4BPP_RLE 0x08
#define isBMPF(pB, test) ((pB->biCompression[1] & BMP_##test) == BMP_##test)
#define setBMPF(pB, test) pB->biCompression[1] |= BMP_##test

typedef struct 
{
  uint8_t Blue;
  uint8_t Green;
  uint8_t Red;
} P24bpp_t;

typedef union 
{
  uint8_t *p08;
  Raster_t *pDisp;
  P24bpp_t *p24;
  BMP_ColorTable_t *pCT;
  uint16_t *p16;
  uint32_t *p32;
}pRaster_t;

#endif

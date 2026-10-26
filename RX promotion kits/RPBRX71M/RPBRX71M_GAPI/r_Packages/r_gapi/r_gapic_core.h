/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/

#ifndef R_GAPIC_CORE_H
#define R_GAPIC_CORE_H
/***********************************************************************************
FILE NAME    : r_GAPI_core.h

DESCRIPTION  : Core Structure definitions and function prototypes for Renesas Graphics API.

Copyright   : 2010 Renesas Electric America
Copyright   : 2010 Renesas Electric Corporation.
          All Rights Reserved
***********************************************************************************/

/* structure definition for font information for "gputs" routine */
#define GAPI_F_SPACE_PROPORTIONAL 0
#define GAPI_F_SPACE_BBOX -1
#define GAPI_F_ALIGN_H_LEFT 0
#define GAPI_F_ALIGN_H_CENTER 1
#define GAPI_F_ALIGN_H_RIGHT 2
#define GAPI_F_ALIGN_V_BOTTOM 0
#define GAPI_F_ALIGN_V_CENTER 1
#define GAPI_F_ALIGN_V_TOP 2
#define GAPI_F_BACK_SOLID 0
#define GAPI_F_BACK_TRANSPARENT 1
#define GAPI_F_FORE_SOLID 0
#define GAPI_F_FORE_TRANSPARENT 1

typedef struct 
{
  GAPI_Font_t const **ppFont;     /* pointer to font pointer */
  BMP_ColorTable_t ct[2];         /* color table 0=BGND, 1=FGND */
  int16_t SpacingX;              /* 0=font specified, > 0 space between characters -1 set spacing to bbox values */
  int16_t SpacingY;              /* 0=font specified, > 0 space between characters, -1 set spacing to bbox values */
  struct 
  {
    uint32_t Align_H: 2;          /* 0=Left justify, 1: center justify, 2: right justify */
    uint32_t Align_V: 2;          /* 0=Bottom justify, 1= center justify, 2: top justify */
    uint32_t Reserved_1: 4;       /* Reserved for future usage */
    uint32_t Reserved_2: 4;       /* Reserved for future usage */
    uint32_t Bgnd_transparent:4;  /* 0=background solid, 1=transparent. */
    uint32_t Fgnd_transparent:16; /* 0=foreground solid, (1=transparent...only 2 color fonts) or can use GAPI_T_xxx */
  } Options;
}GAPI_PUT_t;

#define TRANSPARENCY(mode,value) (((uint32_t)mode << 16) + value)
#define GAPI_T_SRC_INVERT 0x8000   /* flag to invert source color on copy */
#define GAPI_T_MAGIC 0             /* transparency mode uses supplied magic value */
#define GAPI_T_NONE  1             /* no transparency operation is performed */
#define GAPI_T_MULK  2             /* dest = src x value (value is 5:6:5 constant multiplier) */
#define GAPI_T_MUL   3             /* dest = dest x src (color filters...5:6:5 channel by channel multiply) */
#define GAPI_T_MULC  4             /* dest = (src x value) + (1-src) x dest (colorized alpha masks) */
#define GAPI_T_FILL  5             /* transparency value is treated as solid fill */
#define GAPI_T_MULT  6             /* dest = (src x value) + (1-value) x dest (transparency blend) */
#define GAPI_T_MULS  7             /* dest = (src * tr_value * dest) + ((1 - src) * dest) (shadow creation) */
#define GAPI_TI_MAGIC GAPI_T_SRC_INVERT+GAPI_T_MAGIC
#define GAPI_TI_NONE  GAPI_T_SRC_INVERT+GAPI_T_NONE
#define GAPI_TI_MULK  GAPI_T_SRC_INVERT+GAPI_T_MULK
#define GAPI_TI_MUL   GAPI_T_SRC_INVERT+GAPI_T_MUL 
#define GAPI_TI_MULC  GAPI_T_SRC_INVERT+GAPI_T_MULC
#define GAPI_TI_FILL  GAPI_T_SRC_INVERT+GAPI_T_FILL
#define GAPI_TI_MULT  GAPI_T_SRC_INVERT+GAPI_T_MULT
#define GAPI_TI_MULS  GAPI_T_SRC_INVERT+GAPI_T_MULS

/* Function prototypes */
/* Flags to control R_GAPI_CopyXlate */
#define GAPI_CX_ROT_0    0x0000
#define GAPI_CX_ROT_90   0x0001
#define GAPI_CX_ROT_180  0x0002
#define GAPI_CX_ROT_270  0x0003
#define GAPI_CX_ROT_MASK 0x0003
#define GAPI_CX_FLIPX    0x0004
#define GAPI_CX_FLIPY    0x0008
GAPI_Resp_t R_GAPI_CopyXlate( BMP_t const *const s, BMP_t *const d, int16_t dPosX, int16_t dPosY, int16_t sPosX, int16_t sPosY, int16_t Width, int16_t Height, uint16_t Mode );

GAPI_Resp_t R_GAPI_CopySub( BMP_t const *const s, BMP_t *const d, int16_t dPosX, int16_t dPosY, int16_t sPosX, int16_t sPosY, int16_t Width, int16_t Height, BMP_ColorTable_t const *const ct, uint32_t tr );
GAPI_Resp_t R_GAPI_FillGradient(BMP_t *const d, const int16_t dPosX, const int16_t dPosY, int16_t Width, int16_t Height, BMP_ColorTable_t const *const ct, int16_t angle);
GAPI_Resp_t R_GAPI_CalcGradientCTN(BMP_ColorTable_t const *const ct, uint8_t mode, BMP_ColorTable_t * ct_out, int16_t N);
GAPI_Resp_t R_GAPI_CalcShadeCT(BMP_ColorTable_t const * pS, BMP_ColorTable_t const *const ct, BMP_ColorTable_t * ct_out);
int16_t R_GAPI_PutS( uint8_t const * s, GAPI_PUT_t const *Gdata, BMP_t *d, int16_t PosX, int16_t PosY);
int16_t R_GAPI_FontHeight(GAPI_PUT_t const *Gdata);
int16_t R_GAPI_FontWidthString(uint8_t const * s, GAPI_PUT_t const *Gdata);


#endif

/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/

#ifndef R_GAPIW__WRAPPER_H
#define R_GAPIW__WRAPPER_H
/***********************************************************************************
FILE NAME    : r_GAPI_Wrapper.h

DESCRIPTION  : Structure definitions and function prototypes for GAPI function wrappers (helpers).

Copyright   : 2010 Renesas Electric America
Copyright   : 2010 Renesas Electric Corporation.
          All Rights Reserved
***********************************************************************************/
#define CT_WHITE   {0xFF, 0xFF, 0xFF, 0x00}
#define CT_BLACK   {0x00, 0x00, 0x00, 0x00}
#define CT_RED     {0x00, 0x00, 0xFF, 0x00}
#define CT_GREEN   {0x00, 0xFF, 0x00, 0x00}
#define CT_BLUE    {0xFF, 0x00, 0x00, 0x00}
#define CT_CYAN    {0xFF, 0xFF, 0x00, 0x00}
#define CT_MAGENTA {0xFF, 0x00, 0xFF, 0x00}
#define CT_YELLOW  {0x00, 0xFF, 0xFF, 0x00}
#define CT_GRAY_25 {0x40, 0x40, 0x40, 0x00}
#define CT_GRAY_50 {0x80, 0x80, 0x80, 0x00}
#define CT_GRAY_75 {0xC0, 0xC0, 0xC0, 0x00}

#define CT_COLOR_NO_CHANGE 0
#define CT_COLOR_LINEAR_CHANGE 1
#define CT_COLOR_MUL_CHANGE 2

#define CT_TEXT_NONE 0
#define CT_TEXT_CENTER 1
/* this macro will position insertion point of text from lower left of BMP by percent specified in PosX and PosY (0.0 <= Pos <= 0.99) */
#define CT_TEXT(PosX, PosY) ((((uint16_t)(PosX * 64.0) << 6) + (uint16_t)(PosY * 64.0)) << 2)+2

typedef struct
{
  BMP_ColorTable_t ct[2];      /* color table for gemerating gradient for colorizing button...[0]=black replace, [1]=white replace */
  GAPI_PUT_t font;             /* information for text placement */
  struct
  {
    uint16_t Colorize: 2;         /* colorize the passed image 0:no, 1:grayscale, 2:shade, 3: reserved */
    uint16_t Text: 14;            /* Text Insertion point 0:no text, 1:center of BMP, other...use  */
    uint16_t Transparency;        /* Refer to GAPI_T selections */
  } Options;
}GAPI_Scheme_t;

#define NO_TRANSPARENCY_COLOR TRANSPARENCY(GAPI_T_NONE,0)  /* when passed as "transparency" paramter, causes no transparency handling */

/* Function prototypes */
GAPI_Resp_t R_GAPI_ColorCopy(BMP_t const * s, BMP_t *d, int16_t PosX, int16_t PosY, GAPI_Scheme_t const *pScheme);
GAPI_Resp_t R_GAPI_Label(BMP_t const *const s, BMP_t *d, int16_t PosX, int16_t PosY, GAPI_Scheme_t const *pScheme, uint8_t const * label);
void R_GAPI_Scroll(BMP_t const *s, BMP_t *d, int16_t dPosX, int16_t dPosY, int16_t split, uint16_t type);

/**********************************************************************************
Function Name:   R_GAPI_Copy
Description:  simplified BMP to BMP copy routine (inline to minimize stack usage)
Arguments:   s: pointer to source BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t R_GAPI_Copy( BMP_t const *const s, BMP_t *d, int16_t PosX, int16_t PosY )
{
  /* pass through to sub handler */
  return(R_GAPI_CopySub(s, d, PosX, PosY, 0, 0, R_BMP_Width(s), R_BMP_Height(s), R_BMP_Ct(s), TRANSPARENCY(GAPI_T_NONE,0)));
}

/**********************************************************************************
Function Name:   R_GAPI_CopyTransparent
Description:  simplified BMP to BMP copy routine (inline to minimize stack usage)
Arguments:   s: pointer to source BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t R_GAPI_CopyTransparent( BMP_t const *const s, BMP_t *d, int16_t PosX, int16_t PosY )
{
  /* pass through to sub handler */
  return(R_GAPI_CopySub(s, d, PosX, PosY, 0, 0, R_BMP_Width(s), R_BMP_Height(s), R_BMP_Ct(s), TRANSPARENCY(GAPI_T_MAGIC,(uint16_t)(R_BMP_IndexColors(s)-1))));
}

/**********************************************************************************
Function Name:   R_GAPI_CopyMulK
Description:  simplified BMP to BMP copy routine with dimming (multiply by constant)
              inline to minimize stack usage
Arguments:   s: pointer to source BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              ct[0]: dimming value (constant multiplier)
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t R_GAPI_CopyMulK( BMP_t const *const s, BMP_t *d, int16_t PosX, int16_t PosY , BMP_ColorTable_t const *const ct )
{
  /* pass through to sub handler */
  return(R_GAPI_CopySub(s, d, PosX, PosY, 0, 0, R_BMP_Width(s), R_BMP_Height(s), R_BMP_Ct(s), TRANSPARENCY(GAPI_T_MULK,R_GAPI_24_Disp(&ct[0]))));
}

/**********************************************************************************
Function Name:   R_GAPI_CopyMulC
Description:  simplified BMP to BMP copy routine with anti-alias blending
              inline to minimize stack usage
Arguments:   s: pointer to source BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              ct[0]: dimming value (constant multiplier)
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t R_GAPI_CopyMulC( BMP_t const *const s, BMP_t *d, int16_t PosX, int16_t PosY , BMP_ColorTable_t const *const ct )
{
  /* pass through to sub handler */
  return(R_GAPI_CopySub(s, d, PosX, PosY, 0, 0, R_BMP_Width(s), R_BMP_Height(s), R_BMP_Ct(s), TRANSPARENCY(GAPI_T_MULC,R_GAPI_24_Disp(&ct[0]))));
}

/**********************************************************************************
Function Name:   R_GAPI_CopyMulT
Description:  simplified BMP to BMP copy routine with alpha blending
              inline to minimize stack usage
Arguments:   s: pointer to source BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              ct[0]: alpha value (independent r/g/b multipliers)
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t R_GAPI_CopyMulT( BMP_t const *const s, BMP_t *d, int16_t PosX, int16_t PosY , BMP_ColorTable_t const *const ct )
{
  /* pass through to sub handler */
  return(R_GAPI_CopySub(s, d, PosX, PosY, 0, 0, R_BMP_Width(s), R_BMP_Height(s), R_BMP_Ct(s), TRANSPARENCY(GAPI_T_MULT,R_GAPI_24_Disp(&ct[0]))));
}

/**********************************************************************************
Function Name:   R_GAPI_Fill
Description:  Fill area of BMP with a color
              inline to minimize stack usage
Arguments:   d: pointer to destination BMP
              dPosX: starting X coordinate within destination BMP (leftmost = 0)
              dPosY: starting Y coordinate within destination BMP (bottommost = 0)
              Width: Width of area to fill
              Height: Height of area to fill
              ct: pointer to fill color
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t R_GAPI_Fill(BMP_t *const d, const int16_t dPosX, const int16_t dPosY, int16_t Width, int16_t Height, BMP_ColorTable_t const *const ct)
{
  return(R_GAPI_CopySub(d, d, dPosX, dPosY, dPosX, dPosY, Width, Height, R_BMP_Ct(d), TRANSPARENCY(GAPI_T_FILL,R_GAPI_24_Disp(&ct[0]))));
}

/**********************************************************************************
Function Name:   R_GAPI_FillMulK
Description:  Transform area of BMP with a transparency
              inline to minimize stack usage
Arguments:   d: pointer to destination BMP
              dPosX: starting X coordinate within destination BMP (leftmost = 0)
              dPosY: starting Y coordinate within destination BMP (bottommost = 0)
              Width: Width of area to fill
              Height: Height of area to fill
              ct: pointer to multiply color
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t R_GAPI_FillMulK(BMP_t *const d, const int16_t dPosX, const int16_t dPosY, int16_t Width, int16_t Height, BMP_ColorTable_t const *const ct)
{
  return(R_GAPI_CopySub(d, d, dPosX, dPosY, dPosX, dPosY, Width, Height, R_BMP_Ct(d), TRANSPARENCY(GAPI_T_MULK,R_GAPI_24_Disp(&ct[0]))));
}

/**********************************************************************************
Function Name:   R_GAPI_Index
Description:  simplified BMP to BMP copy routine (inline to minimize stack usage)
Arguments:   s: pointer to source BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              index: Index into source BMP
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t R_GAPI_Index( BMP_t const *const s, BMP_t *const d, int16_t PosX, int16_t PosY, int16_t Index)
{
  int16_t Height = R_BMP_Height(s);
  /* pass through to sub handler */
  return(R_GAPI_CopySub(s, d, PosX, PosY, Height*Index, 0, Height, Height, R_BMP_Ct(s), TRANSPARENCY(GAPI_T_MAGIC,(uint16_t)(R_BMP_IndexColors(s)-1))));
}

/**********************************************************************************
Function Name:   R_GAPI_Button
Description:  Draw label in center of BMP
Arguments:   s: pointer to source button BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              pScheme: pointer to information about how to present the image
              Label: Text string to center on  button
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t R_GAPI_Button(BMP_t const *const s, BMP_t *d, int16_t PosX, int16_t PosY, GAPI_Scheme_t const *pScheme, uint8_t const * label)
{
  GAPI_Resp_t status;
  status = R_GAPI_ColorCopy(s,d,PosX,PosY,pScheme);
  if (0 != status)
    return(status);
  return(R_GAPI_Label(s,d,PosX,PosY,pScheme,label));
}

#endif

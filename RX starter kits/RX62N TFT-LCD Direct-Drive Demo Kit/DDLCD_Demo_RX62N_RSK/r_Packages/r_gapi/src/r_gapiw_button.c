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
FILE NAME    : r_GAPIw_Button.c

DESCRIPTION  : BMP Button wrapper for Renesas Graphics API.

Copyright   : 2010 Renesas Electronics America
Copyright   : 2010 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/
#include "r_gapi_private.h"

/**********************************************************************************
Function Name:   R_GAPI_ColorCopy
Description:  Draw Grayscale BMP Colorized
Arguments:   s: pointer to source button BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              pScheme: pointer to information about how to present the image
Return value:   0:OK, Not 0: NG
***********************************************************************************/
GAPI_Resp_t R_GAPI_ColorCopy(BMP_t const *s, BMP_t *d, int16_t PosX, int16_t PosY, GAPI_Scheme_t const *pScheme)
{
  BMP_ColorTable_t const * pCT=s->biColorTable;
  uint32_t transparency = NO_TRANSPARENCY_COLOR;
  GAPI_Resp_t status=0;
  BMP_ColorTable_t *pLocalCT;
  
  if((NULL==s) || (NULL==d))
    return(-1);

  // Acquire scratchpad memory
  pLocalCT = (void *)R_GAPI_PoolMemoryGet(1024);
  
  if(NULL == pLocalCT)
    return(-1);
     
  switch (pScheme->Options.Colorize)
  {
    case 1:
      // generate for grayscale color table
      if (status == 0) status = R_GAPI_CalcGradientCTN(pScheme->ct,0,pLocalCT,R_BMP_IndexColors(s));
      pCT = pLocalCT;
      break;
    
    case 2:
      // generate shaded color table
      if (status == 0) status = R_GAPI_CalcShadeCT(s->biColorTable,pScheme->ct,pLocalCT);
      pCT = pLocalCT;
      break;
  }

  switch(pScheme->Options.Transparency)
  {
    case GAPI_T_NONE:
      /* default no transparency */
      transparency = TRANSPARENCY(GAPI_T_NONE,0);
     break;
      
    case GAPI_T_MAGIC:
      /* magic color transparency (index only mode) */
      transparency = TRANSPARENCY(GAPI_T_MAGIC,R_BMP_IndexColors(s)-1);
      break;

    default:
      /* pass transparency command and value through */
      transparency = TRANSPARENCY(pScheme->Options.Transparency,R_GAPI_24_Disp(&pScheme->ct[1]));
      break;
  }
    
  // Paint the button
  if(status == 0) status = R_GAPI_CopySub(s, d, PosX, PosY, 0, 0, R_BMP_Width(s), R_BMP_Height(s), pCT, transparency);

  // Release scratchpad memory
  (void)R_GAPI_PoolMemoryFree((void *)pLocalCT);
  return (status);
}

/**********************************************************************************
Function Name:   R_GAPI_Label
Description:  Draw label in center of BMP
Arguments:   s: pointer to source button BMP
              d: pointer to destination BMP
              PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              pScheme: pointer to information about how to present the image
              Label: Text string to center on  button
Return value:   0:OK, Not 0: NG
***********************************************************************************/
GAPI_Resp_t R_GAPI_Label(BMP_t const *const s, BMP_t *d, int16_t PosX, int16_t PosY, GAPI_Scheme_t const *pScheme, uint8_t const * label)
{
  GAPI_Resp_t status=0;
  if((s==NULL) || (d==NULL))
    return(-1);
    
  if (pScheme->Options.Text != 0)
  {
    if(pScheme->Options.Text == 1)
    {
      PosX += R_BMP_Width(s)/2;
      PosY += R_BMP_Height(s)/2;
    }
    else
    {
      PosX += ((pScheme->Options.Text>>6) & 0x0F) * R_BMP_Width(s)/16;
      PosY += ((pScheme->Options.Text>>2) & 0x0F) * R_BMP_Height(s)/16;
    }
      
    if (0 != (status = R_GAPI_PutS(label, &pScheme->font, d, PosX, PosY)))
        return(status);
  }
  return (status);
}


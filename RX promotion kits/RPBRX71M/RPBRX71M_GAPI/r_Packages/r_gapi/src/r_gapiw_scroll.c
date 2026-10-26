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
FILE NAME    : r_GAPIw_scroll.c

DESCRIPTION  : Scroll Support for Renesas Graphics API.

Copyright   : 2007 Renesas Electronics America
Copyright   : 2007 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/

#include "r_gapi_private.h"

/**********************************************************************************
Function Name:   R_GAPI_Scroll
Description:  Paints the requested BMP by "splitting" image vertically or horizontally.
              Called repeatedely, this can create a "scrolling" effect.
Arguments:   s: pointer to source BMP                                            
              d: pointer to destination BMP                                       
              dPosX: starting X coordinate within destination BMP (leftmost = 0)  
              dPosY: starting Y coordinate within destination BMP (bottommost = 0)
              split: split location within bitmap                                 
              type: 0=horizontal split, 1=vertical split                                        
Return value:   none
***********************************************************************************/
void R_GAPI_Scroll(BMP_t const *s, BMP_t *d, int16_t dPosX, int16_t dPosY, int16_t split, uint16_t type)
{
  int16_t sbiHeight = R_BMP_Height(s);
  int16_t sbiWidth = R_BMP_Width(s);

  if((s==NULL) || (d==NULL))
    return;
    
  if (type == 0)
  {
    /* horizontal split */
    split = split % sbiWidth;
    (void)R_GAPI_CopySub(s, d, dPosX,                dPosY, split, 0, sbiWidth-split, sbiHeight,R_BMP_Ct(s), NO_TRANSPARENCY_COLOR );
    (void)R_GAPI_CopySub(s, d, dPosX+sbiWidth-split, dPosY, 0,     0, split,          sbiHeight,R_BMP_Ct(s) , NO_TRANSPARENCY_COLOR );
  }
  else
  {
    /* vertical split */
    split = split % sbiHeight;
    (void)R_GAPI_CopySub(s, d, dPosX, dPosY,                 0, split, sbiWidth, sbiHeight-split,R_BMP_Ct(s) , NO_TRANSPARENCY_COLOR );
    (void)R_GAPI_CopySub(s, d, dPosX, dPosY+sbiHeight-split, 0,     0, sbiWidth, split,          R_BMP_Ct(s) , NO_TRANSPARENCY_COLOR );
  }
}


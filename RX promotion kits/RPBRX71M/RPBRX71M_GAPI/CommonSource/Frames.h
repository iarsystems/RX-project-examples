/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :Frames.h                                              */
/*  DESCRIPTION :Frame Management I/F                                  */
/*                                                                     */
/*                                                                     */
/***********************************************************************/

#ifndef FRAMES_H_INCLUDED
#define FRAMES_H_INCLUDED

extern BMP_t *displayFrameBuf;
extern BMP_t *backFrameBuf;
extern BMP_t *workFrameBuf;


/**********************************************************************************
Function Prototypes
***********************************************************************************/

uint16_t R_GAPI_WorkDisplay(void);

/**********************************************************************************
Function Name:   BMPCopyBackDisplay
Description:  Copy region from backFrameBuf to displayFrameBuf 
Arguments:   PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              Width: Width of region
              Height: Height of region
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t BMPCopyBackDisplay( int16_t PosX, int16_t PosY, int16_t Width, int16_t Height )
{
  /* pass through to sub handler */
  return(R_GAPI_CopySub(backFrameBuf, displayFrameBuf, PosX, PosY, PosX, PosY, Width, Height, NULL, NO_TRANSPARENCY_COLOR));
}

/**********************************************************************************
Function Name:   BMPCopyBackWork
Description:  Copy region from backFrameBuf to workFrameBuf 
Arguments:   PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              Width: Width of region
              Height: Height of region
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t BMPCopyBackWork( int16_t PosX, int16_t PosY, int16_t Width, int16_t Height )
{
  /* pass through to sub handler */
  return(R_GAPI_CopySub(backFrameBuf, workFrameBuf, PosX, PosY, PosX, PosY, Width, Height, NULL, NO_TRANSPARENCY_COLOR));
}

/**********************************************************************************
Function Name:   BMPCopyWorkDisplay
Description:  Copy region from workFrameBuf to displayFrameBuf 
Arguments:   PosX: starting X coordinate within destination BMP (leftmost = 0)
              PosY: starting Y coordinate within destination BMP (bottommost = 0)
              Width: Width of region
              Height: Height of region
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t BMPCopyWorkDisplay( int16_t PosX, int16_t PosY, int16_t Width, int16_t Height )
{
  /* pass through to sub handler */
  return(R_GAPI_CopySub(workFrameBuf, displayFrameBuf, PosX, PosY, PosX, PosY, Width, Height, NULL, NO_TRANSPARENCY_COLOR));
}


/**********************************************************************************
Function Name:   BMPMove
Description:  Move BMP in displayFrameBuf (restore old, write new)
Arguments:   s: pointer to source Bmp
              PosXold: old BMP location
              PosYold: old BMP location
              PosXold: new BMP location
              PosYold: new BMP location
Return value:   0:OK, Not 0: NG
***********************************************************************************/
static inline GAPI_Resp_t BMPMove( BMP_t const *const s, int16_t PosXold, int16_t PosYold, int16_t PosXnew, int16_t PosYnew, GAPI_Scheme_t const *pScheme )
{
  GAPI_Resp_t status;
  int16_t Width = R_BMP_Width(s);
  int16_t Height = R_BMP_Height(s);
  status = BMPCopyBackWork(PosXold, PosYold, Width, Height);
  if (0 != status)
  {
    return(status);
  }
  status = BMPCopyBackWork(PosXnew, PosYnew, Width, Height);
  if (0 != status)
  {
    return(status);
  }
  status = R_GAPI_ColorCopy(s,workFrameBuf,PosXnew,PosYnew,pScheme);
  if (0 != status)
  {
    return(status);
  }
  status = BMPCopyWorkDisplay(PosXold, PosYold, Width, Height);
  if (0 != status)
  {
    return(status);
  }
  status = BMPCopyWorkDisplay(PosXnew, PosYnew, Width, Height);
  return(status);
}
/*******************************************************************************
End of function BMPMove
*******************************************************************************/

#endif    /* FRAMES_H_INCLUDED */  

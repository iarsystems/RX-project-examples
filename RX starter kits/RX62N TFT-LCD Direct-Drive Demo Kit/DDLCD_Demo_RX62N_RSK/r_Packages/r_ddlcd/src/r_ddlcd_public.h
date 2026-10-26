/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :Directlcd.h                                           */
/*  DATE        :Jan 23, 2008                                          */
/*  DESCRIPTION :Direct Drive Application Programming Interface        */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
01.24.2008 RTA-JMB Rev 1.0
************************************************************************/
#ifndef R_DDLCD_PUBLIC_H
#define R_DDLCD_PUBLIC_H

/* Typedef fundemental data types that may change between compilers/families */
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>

extern uint16_t DDLCD_Vcount;          // frame counter
extern uint16_t DDLCD_BusActive;       // flag indicating if we're running ExDMA on this line

typedef int16_t DDLCD_Resp_t;

// Function Prototypes

DDLCD_Resp_t R_DDLCD_Init(void);
int16_t R_DDLCD_SetLineSource(int16_t Region, int16_t LineCount, void *pSource, int16_t LineStep);
void * R_DDLCD_SetActiveRaster(uint16_t frame);
uint16_t R_DDLCD_GetActiveFrame(void);
int16_t R_DDLCD_SetFrameRate(int16_t rate);
int16_t R_DDLCD_GetFrameRate(void);
int16_t R_DDLCD_SetRasterOffset(int16_t x, int16_t y);
void R_DDLCD_Backlight(int state);
void R_DDLCD_Off(void);


#endif

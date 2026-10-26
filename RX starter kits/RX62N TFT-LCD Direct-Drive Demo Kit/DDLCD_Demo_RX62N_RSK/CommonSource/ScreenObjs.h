/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :ScreenObjs.h                                          */
/*  DESCRIPTION :Screen Objects I/F                                    */
/*                                                                     */
/***********************************************************************/

#ifndef SCREENOBJS_H_INCLUDED
#define SCREENOBJS_H_INCLUDED


typedef struct
{
  int16_t Value;
  int16_t ValueOld;
  int16_t Touched;
} SLIDERSTATE_type;

/**********************************************************************************
Function Prototypes
***********************************************************************************/

/* Generic Handlers */
int16_t ButtonHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, uint8_t const *pT,int16_t State );
int16_t IconHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, int16_t index, int16_t State );
int16_t SliderHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, BMP_t const *const pB, SLIDERSTATE_type *pState );
int16_t SliderSwitchHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, BMP_t const *const pB, uint8_t const *const pT[], SLIDERSTATE_type *pState );
int16_t DataBoxHandler(ScreenObj_t const *pS, EVENT_MSG const *pMsg, uint8_t const *pL, uint8_t const *pD );
void ButtonBack(ScreenObj_t const *pS, EVENT_MSG const *pMsg);

#endif    /* SCREENOBJS_H_INCLUDED */  

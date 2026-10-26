/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :ScreenMgr.h                                           */
/*  DESCRIPTION :Screen Management interfaces                          */
/*                                                                     */
/*                                                                     */
/***********************************************************************/

#ifndef SCREEN_MGR_H_INCLUDED
#define SCREEN_MGR_INCLUDED

/**********************************************************************************
Type Defs
***********************************************************************************/
typedef enum {
  MS_INVALID = 0,          // Invalid event
  MS_TOUCHSCREEN,          // Touchscreen event
  MS_KEYBOARD,             // Keyboard event
  MS_DISPLAY,              // Display event
  MS_MOUSE,                // Mouse event
  MS_IO,                   // Discrete I/O
} MessageSource_t;

typedef enum {
  ME_INVALID = 0,          // Invalid event ID
  ME_MOVE,                 // Move event ID
  ME_PRESS,                // Press event ID
  ME_RELEASE,              // Release event ID
  ME_DRAW,                 // Request object background draw
  ME_DRAW_WORK,            // Request object work draw (prior to switch to active display)
  ME_DRAW_DISPLAY,         // Request object foreground draw
  ME_SCREEN_CHANGE,        // Request screen change
  ME_SCREEN_PUSH,          // Request screen change, store previous
  ME_SCREEN_POP,           // Request screen change, restore previous

  ME_TIME_UPDATE,          // Request servicing of 1s behavior
  ME_USER_UPDATE           // Request servicing of user behavior
} MessageEvent_t;

struct SCREEN_TYPE;
typedef struct {
  MessageSource_t source;           // Message source
  MessageEvent_t  event;            // Message Event
  union
  {
    struct SCREEN_TYPE const *pS; // Screen pointers (used by ME_SCREEN_***)
    int16_t       coord[2];          // X/Y coordinate of an event (used by ME_MOVE, _PRESS, _RELEASE)
    uint32_t      value;       // time tick in seconds (used by ME_TIME_UPDATE, ME_USER_UPDATE)
  }param;
} EVENT_MSG; 

struct SCREEN_OBJ_TYPE
{
  BMP_t const * *ppBmp;
  GAPI_Scheme_t const *pScheme;
  void (*func)(struct SCREEN_OBJ_TYPE const *pS, EVENT_MSG const *pMsg);
  int16_t PosX;
  int16_t PosY;
};
typedef struct SCREEN_OBJ_TYPE ScreenObj_t;

struct SCREEN_TYPE
{
  void (*constructor)(struct SCREEN_TYPE const *pS);
  void (*destructor)(struct SCREEN_TYPE const *pS);
  ScreenObj_t const *objects;
};
typedef struct SCREEN_TYPE Screen_t;

/**********************************************************************************
Function Prototypes
***********************************************************************************/

/* This macro used to create screen x,y coordinates in terms of percentage (it will then scale with resolution). */
#define SX(x) (int16_t)((x * 16384L * DD_FRAME_WIDTH + (int32_t)(DD_FRAME_WIDTH/2)) / 16384L)
#define SY(y) (int16_t)((y * 16384L * DD_FRAME_HEIGHT + (int32_t)(DD_FRAME_HEIGHT/2)) / 16384L)

void ScreenMgrInit(void);
void ScreenUpdate(EVENT_MSG *pMsg);
int16_t ScreenTaskStart(portTickType (* func )(void)); /* RTOS_USAGE */
int16_t ScreenTaskStop(portTickType (* func )(void));  /* RTOS_USAGE */
void ScreenTasksStop(void);

void BasicConstructor(Screen_t const *pS);
void BasicDestructor(Screen_t const *pS);

#endif    /* SCREEN_MGR_INCLUDED */  

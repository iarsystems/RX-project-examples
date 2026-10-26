/* Adapted for use with IAR Embedded Workbench */
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
/***********************************************************************/
/*                                                                     */
/*  FILE        :EventMgr.c                                            */
/*  DATE        :Wen, Feb 20, 2008                                     */
/*  DESCRIPTION :Event Manager implementation for the RLCD2378 kit     */        
/*                                                                     */
/*  CPU TYPE    :H8S/2378R                                             */
/*                                                                     */
/***********************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
*
************************************************************************/
#include "Global.h"
#include "LCD_demo.h"
/* Event Manager include files */
#include "touchscreen.h"

static void TouchScreenCapture(void *pvParameters);  // The task of touchscreen capature
static void EventManager(void *pvParameters);        // The task of Event Manager

xQueueHandle     EventQueue;      /* RTOS_USAGE */
xTaskHandle      TouchScreenHnd;  /* RTOS_USAGE */
xTaskHandle      EventMgrHnd;     /* RTOS_USAGE */

uint16_t sErrorEverOccurred;

#pragma section = ".BResources"

/*****************************************************************************/
//
// FUNCTION    :EventManagerInit
// PARAMETERS  :void
// RETURNS     :void
//
// DESCRIPTION : Initialize the event manager
//
//
/*****************************************************************************/
void EventManagerInit(void)
{
  // An eventmsg queue will be allocated for the Event/Message manager(s)...32 events of EVENT_MSG type
  EventQueue = xQueueCreate(32, (unsigned portBASE_TYPE)sizeof(EVENT_MSG)); /* RTOS_USAGE */
  
  // To create the two tasks: one is for the touchscreen capature as the Producer of the Event Queue;
  // the other is for the Event Manager as the consumer of the Event Queue
  (void)xTaskCreate(TouchScreenCapture, "c:Screen", configMINIMAL_STACK_SIZE, NULL, mainDEMOTASKS_PRIORITY, &TouchScreenHnd); /* RTOS_USAGE */
  (void)xTaskCreate(EventManager, "e:EventMgr", configMINIMAL_STACK_SIZE+256, NULL, mainDEMOTASKS_PRIORITY+1, &EventMgrHnd);        /* RTOS_USAGE */
}

/*****************************************************************************/
//
// FUNCTION    :TouchScreenCapture
// PARAMETERS  :(void *)pvParameters
// RETURNS     :
//
// DESCRIPTION :
//
//
/*****************************************************************************/
static void TouchScreenCapture(void *pvParameters)
{
  POINT_TYPE cursorLocation;

  EVENT_MSG msg;
  int16_t retVal;

  portTickType xLastWakeTime, xFrequency = port_mS_to_TICK(10); /* RTOS_USAGE */

  cursorLocation.x = cursorLocation.y = 0;
  msg.source = MS_TOUCHSCREEN;
  
  xLastWakeTime = xTaskGetTickCount();  /* RTOS_USAGE */
  
  for ( ;; )
  {    
    // Wait for the next cycle.
    vTaskDelayUntil( &xLastWakeTime, xFrequency );  /* RTOS_USAGE */

    // Scanning input from touch panel; and if valid sending a an event message to the event manager
    msg.event = scanTouch(&cursorLocation);

    if (msg.event != ME_INVALID)
    {
      msg.param.coord[0] = cursorLocation.x;
      msg.param.coord[1] = cursorLocation.y;    
      
      retVal = (int16_t)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
      if (retVal != pdPASS)
         sErrorEverOccurred ++; // Tracking the failure frequency in sending event messages
    }

    if(PINi(TOUCH_CALIBRATION_START) == 0)
    {
      extern const Screen_t ScreenCalibrateData;
      while (PINi(TOUCH_CALIBRATION_START) == 0)
        ;
      // send a screen change command to the handler
      msg.source = MS_DISPLAY;
      msg.event = ME_SCREEN_PUSH;
      msg.param.pS = &ScreenCalibrateData;
      
      retVal = (int16_t)xQueueSend(EventQueue, (void *)&msg, 0UL); /* RTOS_USAGE */
      if (retVal != pdPASS)
         sErrorEverOccurred ++; // Tracking the failure frequency in sending event messages
    }
  }
}

/*****************************************************************************/
//
// FUNCTION    :EventManager
// PARAMETERS  :(void *)pvParameters
// RETURNS     :
//
// DESCRIPTION :
//
//
/*****************************************************************************/
static void ResourceCallback(ResourceState_t state, uint32_t pct, uint32_t of)
{
  uint8_t string[32];
  uint32_t complete = pct * 1000/of;
  if(pct > of)
    complete = 1000;
    
  switch(state)
  {
    case RESOURCE_VERIFYING:
      sprintf((char *)string, "Verify");
      break;

    case RESOURCE_READING:
      sprintf((char *)string, "Reading %d%%", complete/10);
      break;

    case RESOURCE_WRITING:
      sprintf((char *)string, "Writing %d%%", complete/10);
      break;

    case RESOURCE_ERASING:
      sprintf((char *)string, "Erasing %d%%", complete/10);
      break;

  }
  {
    static const BMP_ColorTable_t bar_ct[] = {CT_BLACK, CT_RED, CT_GREEN};
    static const GAPI_PUT_t bar_text = { &pfontTerminal, {CT_NONE,CT_BLACK}, 0, 0, {1,0,0,0,1,0}};

    int16_t height = R_GAPI_FontHeight(&bar_text);
    int16_t width = (complete * SX(0.8)/1000);
    int16_t posY = SY(1.0)- (height+2) * (1+state);
    (void)R_GAPI_Fill(workFrameBuf, SX(0.1), posY, width, height, &bar_ct[2]);
    (void)R_GAPI_Fill(workFrameBuf, SX(0.1)+width, posY, SX(0.8)-width, height, &bar_ct[1]);
    (void)R_GAPI_PutS(string, &bar_text, workFrameBuf,SX(0.5), posY);
    (void)BMPCopyWorkDisplay(SX(0.1),posY,SX(0.8),height);
  }
}
uint8_t ResourceWrite=0;
static void EventManager(void *pvParameters)
{
  EVENT_MSG msg;
  int16_t retVal;
 
  const portTickType xBlockTime = port_mS_to_TICK(10); /* RTOS_USAGE */

  FramesInit();    
  ScreenMgrInit();          // Initialize screen handler system
  TimerRTC_init();

  #if defined LOAD_RESOURCES_RSK2456
    /* Allow resources to be saved to external source...if necessary */
    if(0 != ResourceWrite)
      ResourceStore(0, __sectop(".BResources"), NULL);
      
    /* Bring in resources from external source...if necessary
       Normally we would load resources with LCD active, but because of hardware
       pin conflict on H8S/2456 RSK (LCD direct drive TPU & serial flash SSU) we will load here prior 
       to starting Direct Drive. */
    pResources = ResourceLoad(0, __sectop(".BResources"), NULL);
    SYSTEM.EXMSTPCRL.BIT.MSTP23 = 1;  
  #endif

  {
    R_DDLCD_ExMemoryInit();   // start external memory manager to block tasks accessing external bus as necessary
    (void)R_DDLCD_Init();     // Initialize the DD LCD display system
    
    while ( DDLCD_Vcount < 2) // wait for 2 frame before turning on LCD backlight and initializing memory
        ;

    R_DDLCD_Backlight(1);     // Turn on the backlight
  }
  
  {
    /* access internal resources */
    static const uint8_t IResources[]=
    {
      #include "iresources.h"
    };
    pIResources = ResourceVerify(IResources);
    pfontTerminal = FileFind(pIResources, "fontTerminal");
  }

  /* Let system know we're accessing External Memory */
  R_DDLCD_ExMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */

  #if defined LOAD_RESOURCES
  {
    int16_t rate = R_DDLCD_GetFrameRate();
    (void)R_DDLCD_SetFrameRate(rate/2);    // temporarily drop frame rate to speed up construction process
    /* Allow resources to be saved to external source...if necessary */
    if(0 != ResourceWrite)
      ResourceStore(0, __section_begin(".BResources"), ResourceCallback);
      
    /* Bring in resources from external source...if necessary */
    pResources = ResourceLoad(0, (uint8_t *)__section_begin(".BResources"), ResourceCallback);
    
    (void)R_DDLCD_SetFrameRate(rate);  // restore frame rate
  }
  #else
    /* verify resources to initialize pResources */
    pResources = ResourceVerify(__section_begin(".BResources"));
  #endif

  /* display splash screen if it exists */
  {
    BMP_t const *pSplash = FileFind(pResources, "_SplashScreen");
    portTickType start_time = xTaskGetTickCount(); /* RTOS_USAGE */
    if (pSplash != NULL)
    {
      uint16_t counter=0;
      
      (void)R_GAPI_Copy( pSplash, workFrameBuf, (DD_FRAME_WIDTH - R_BMP_Width(pSplash))/2, (DD_FRAME_HEIGHT-R_BMP_Height(pSplash))/2 );
      (void)R_GAPI_WorkDisplay();
      
      do
      {
        (void)xQueueReceive(EventQueue, &msg, xBlockTime); /* RTOS_USAGE */
      } while ((msg.event != ME_RELEASE) && ((xTaskGetTickCount() - start_time) < port_mS_to_TICK(4000))); /* RTOS_USAGE */
    }
  }
  R_DDLCD_ExMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
      
  {
    // Send a command to set initial screen to the queue
    extern Screen_t ScreenHomeData;
    msg.source = MS_DISPLAY;
    msg.event = ME_SCREEN_CHANGE;
    msg.param.pS = &ScreenHomeData;
    retVal = (int16_t)xQueueSend(EventQueue, (void *)&msg, 0UL);
    if (retVal != pdPASS) 
      sErrorEverOccurred ++; // Tracking the failure frequency in sending event messages
  }
  
  /* Lower priority to that of other tasks */
  vTaskPrioritySet(NULL,mainDEMOTASKS_PRIORITY);    /* RTOS_USAGE */
  
  for( ;; )
  {  
    // Check Event Queue...this call will suspend task for xBlockTime if no event to process
    retVal = (int16_t)xQueueReceive(EventQueue, &msg, xBlockTime); /* RTOS_USAGE */

    if (retVal == pdPASS)
    {
      /* Let system know we're accessing External Memory */
      R_DDLCD_ExMemoryAcquire(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */

      {
        ScreenUpdate(&msg);
      }

      R_DDLCD_ExMemoryRelease(xTaskGetCurrentTaskHandle()); /* RTOS_USAGE */
    }
  }
}


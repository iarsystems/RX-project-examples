/* Adapted to IAR Embedded Workbench */
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
/*  FILE        :ScreenMgr.c                                           */
/*  DESCRIPTION :Screen Management for LCD DD demo code                */        
/*                                                                     */
/*                                                                     */
/***********************************************************************/
#include "Global.h"
#include "LCD_demo.h"

typedef struct
{
  xTaskHandle handle;           /* RTOS_USAGE */
  portTickType (* func )(void); /* RTOS_USAGE */
  uint8_t running;
  volatile uint8_t stop;
} ScreenTask_type;

static ScreenTask_type ScreenTask_list[4];
static void ScreenTasks(void *pvParameters);

//#pragma location=".Resources"
//uncomment the following array declaration if the resources binary file is not 
//loaded into code flash space in (Properties>C/C++ Build>Settings>Linker Input
//  uint32_t Resources[0x80000];  // Label for resource loader usage

/**********************************************************************************
Function Name:   ScreenMgrInit
Description:  Initialize the screen tasks.
Arguments:   none.
Return value:   none
***********************************************************************************/
void ScreenMgrInit(void)
{
  int16_t index;

  for (index = 0; index < (int16_t)size_ra(ScreenTask_list); index++)
  {
    (void)xTaskCreate(ScreenTasks, "ScreenTasks", configMINIMAL_STACK_SIZE+256, &ScreenTask_list[index], mainDEMOTASKS_PRIORITY, &ScreenTask_list[index].handle); /* RTOS_USAGE */
  }
}
/*******************************************************************************
End of function ScreenMgrInit
*******************************************************************************/

/**********************************************************************************
Function Name:   ScreenTaskStart
Description:  Request a task to run...dynamically from a pool of tasks.
Arguments:   func : function to be run as a task.
Return value:   0: task slot found, 1: NG
***********************************************************************************/
int16_t ScreenTaskStart(portTickType (* func )(void))
{
  int16_t index;

  /* first scan to ensure task not already running */
  for (index=0; index < (int16_t)size_ra(ScreenTask_list); index++)
  {
    if(ScreenTask_list[index].func == func)
      {
			return (-1);
		}
  }

  for (index=0; index < (int16_t)size_ra(ScreenTask_list); index++)
  {
    /* find an idle task */
    if (NULL == ScreenTask_list[index].func)
    {
      /* Register the function */
      ScreenTask_list[index].func = func;

      /* And start it up */
      vTaskResume(ScreenTask_list[index].handle); /* RTOS_USAGE */
      return(0);
    }
  }

  /* No slot found */
  return(-1);
}
/*******************************************************************************
End of function ScreenTaskStart
*******************************************************************************/

/**********************************************************************************
Function Name:   ScreenTaskStop
Description:     Request a task to stop...from the pool of tasks.
Arguments:       func : function to be stopped.
Return value:    0: task slot found, 1: NG
***********************************************************************************/
int16_t ScreenTaskStop(portTickType (* func )(void))
{
    int16_t index;

    /* first scan to find task */
    for (index=0; index < (int16_t)size_ra(ScreenTask_list); index++)
    {
        if(ScreenTask_list[index].func == func)
        {
            while(ScreenTask_list[index].running)
            {
              /* wait for task to cooperatively stop */
              ScreenTask_list[index].stop = 1;
              vTaskDelay(2L);   /* RTOS_USAGE */
            }

            return(0);
        }
    }

    return(-1);
}
/*******************************************************************************
End of function ScreenTaskStop
*******************************************************************************/

/**********************************************************************************
Function Name:   ScreenTasksStop
Description:  Shut down all screen tasks (typically called as part of screen destructor).
              This will delay until all tasks are suspended.
Arguments:   none
Return value:   none
***********************************************************************************/
void ScreenTasksStop(void)
{
  int16_t index;
  for (index=0; index < (int16_t)size_ra(ScreenTask_list); index++)
  {
    while(ScreenTask_list[index].running)
    {
      /* wait for task to cooperatively stop */
      ScreenTask_list[index].stop = 1;
      vTaskDelay(2L);   /* RTOS_USAGE */
    }
  }
}
/*******************************************************************************
End of function ScreenTasksStop
*******************************************************************************/

/**********************************************************************************
Function Name:   ScreenTasks
Description:  Tasks that can be used by a screen on a dynamic basis.
              The list of tasks all use this function to start. At runtime function
              pointers are used to allow screen tasks to execute (via ScreenTaskStart function)
              The screen function must exit and pass back a delay value periodically.
              This allows the screen destructor to shut down the task when screen closes.
              A Screen task passing back a 0 delay will be suspended.
Arguments:   pvParameters is passed to allow task to access it's local status flags.
Return value:   None
***********************************************************************************/
static void ScreenTasks(void *pvParameters)
{
  ScreenTask_type *pS = (ScreenTask_type *)pvParameters;
  portTickType xLastWakeTime, xFrequency = port_mS_to_TICK(60); /* RTOS_USAGE */

    for (;;)
    {
        pS->running = 1;

        while ((NULL != pS->func) && (0 == pS->stop))
        {
            xFrequency = pS->func();

            if (0 == xFrequency)
            {
                /* This break is intentional */
                break;
            }

            vTaskDelayUntil( &xLastWakeTime, xFrequency );  /* RTOS_USAGE */
        }

        pS->running = 0;
        pS->stop = 0;
        pS->func = NULL;
        vTaskSuspend(NULL);                   /* RTOS_USAGE */
        xLastWakeTime = xTaskGetTickCount (); /* RTOS_USAGE */
    }
}
/*******************************************************************************
End of function ScreenTasks
*******************************************************************************/

/*******************************************************************************
* Function Name: ScreenObjUpdate
* Description  : Process indivdual screen objects.
* Arguments    : pS : Active Screen Structure
*                pMsg Message about which event has occurred
* Return Value : None
*******************************************************************************/
static void ScreenObjUpdate(Screen_t const *pS, EVENT_MSG *pMsg)
{
  ScreenObj_t const *pI = pS->objects;

  for (; NULL != pI->func; pI++)
  {
      pI->func(pI,pMsg);
  }
}
/*******************************************************************************
End of function ScreenObjUpdate
*******************************************************************************/

/*******************************************************************************
* Function Name: ScreenUpdate
* Description  : Process indivdual screen objects.
* Arguments    : pMsg Message about which event has occurred
* Return Value : None
*******************************************************************************/
static uint8_t ScreenIndex=0;
static Screen_t const *ScreenStack[8];

void ScreenUpdate(EVENT_MSG *pMsg)
{
  Screen_t const *pS = ScreenStack[ScreenIndex];

  /* Manage screen changes as requested */
  switch (pMsg->event)
  {
    case ME_SCREEN_CHANGE:
    {
      if (NULL != pS)
      {
          pS->destructor(pS);
      }

      pS=ScreenStack[ScreenIndex]=pMsg->param.pS;
      pS->constructor(pS);
      break;
    }
    case ME_SCREEN_PUSH:
    {
      if (ScreenIndex >= (uint8_t)size_ra(ScreenStack))
      {
          break;
      }

      ScreenIndex++;

      if (NULL != pS)
      {
        pS->destructor(pS);
      }

      pS=ScreenStack[ScreenIndex]=pMsg->param.pS;
      pS->constructor(pS);
      break;
    }
    case ME_SCREEN_POP:
    {
      if (ScreenIndex <= 0)
      {
          break;
      }

      ScreenIndex--;

      if (NULL != pS)
      {
          pS->destructor(pS);
      }

      pS=ScreenStack[ScreenIndex];
      pS->constructor(pS);
      break;
    }
    default:
    {
      if (NULL == pS)
      {
          break;
      }
    
      /* Call each Icon to allow them to update */
      ScreenObjUpdate(pS,pMsg);
      break;
    }
  }
}
/*******************************************************************************
End of function ScreenUpdate
*******************************************************************************/

/**********************************************************************************
  Generic Handlers
***********************************************************************************/
/* Shared event message */
static EVENT_MSG msg;

/**********************************************************************************
Function Name:   BasicConstructor
Description:  Typical behavior to apply during construction of screen.
Arguments:   pS: pointer to screen information
Return value:   none
***********************************************************************************/
void BasicConstructor(Screen_t const *pS)
{
  int16_t rate = R_DDLCD_GetFrameRate();

  /* temporarily drop frame rate to speed up construction process */
  (void)R_DDLCD_SetFrameRate(rate);

  /* send a draw command to the icons */
  msg.source = MS_DISPLAY;
  msg.event = ME_DRAW;
  ScreenObjUpdate(pS,&msg);

  /* Copy background to work frame */
  (void)R_GAPI_Copy(backFrameBuf, workFrameBuf,0,0);

  /* send a draw command to allow updates on work frame */
  msg.event = ME_DRAW_WORK;
  ScreenObjUpdate(pS,&msg);
  
  /* Make work active */
  (void)R_GAPI_WorkDisplay();

  /* send a draw command to the icons */
  msg.source = MS_DISPLAY;
  msg.event = ME_DRAW_DISPLAY;
  ScreenObjUpdate(pS,&msg);
  (void)R_DDLCD_SetFrameRate(rate);  // restore frame rate
}
/*******************************************************************************
End of function BasicConstructor
*******************************************************************************/

/**********************************************************************************
Function Name:   BasicDestructor
Description:  Typical behaviour to apply during construction of screen.
Arguments:   pS: pointer to screen information
Return value:   none
***********************************************************************************/
void BasicDestructor(Screen_t const *pS)
{
  /* Stop all screen tasks that we've started */
  ScreenTasksStop();
}
/*******************************************************************************
End of function BasicDestructor
*******************************************************************************/


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
/*  FILE        :EventMgr.c                                            */
/*  DATE        :Apr 05, 2016                                          */
/*  DESCRIPTION :Event Manager implementation for the RPB RX71M        */
/*                                                                     */
/*  CPU TYPE    :RX64M/RX71M                                           */
/*                                                                     */
/***********************************************************************/
/************************************************************************
 Revision History
 DD.MM.YYYY OSO-UID Description
 05.04.2016 Added Simulated Touch Task

 ************************************************************************/
#include "Global.h"
#include "LCD_demo.h"
/* Event Manager include files */
#include "touchscreen.h"
#include "r_touch_simulate.h"
#include "r_config_demo_mode.h"
#include "ad_dtc_RX71M.h"

/* Screen update frame rate:
 * 1 = Full frame rate
 * 2 = Half frame rate */
#define FRAME_RATE_DIV  (1U)

static void TouchScreenCapture (void *pvParameters);  // The task of touchscreen capture
static void EventManager (void *pvParameters);        // The task of Event Manager

xQueueHandle EventQueue; //  RTOS_USAGE
xTaskHandle TouchScreenHnd; //  RTOS_USAGE
xTaskHandle EventMgrHnd; //  RTOS_USAGE
xTaskHandle TouchSimHnd; //  RTOS_USAGE

uint16_t sErrorEverOccurred;

char g_demo_mode; /* Stores demonstration mode, derived from the configuration file */

#pragma section = ".BResources"

/*******************************************************************************
 * Function Name: EventManagerInit
 * Description  : Initialize the event manager.
 * Arguments    : none
 * Return Value : none
 *******************************************************************************/
void EventManagerInit (void)
{
    /* An eventmsg queue will be allocated for the Event/Message manager(s)
     ...32 events of EVENT_MSG type */
    /*  RTOS_USAGE */
    EventQueue = xQueueCreate(32, (unsigned portBASE_TYPE)sizeof(EVENT_MSG));

    /* To create the Three tasks: one is for the touch-screen capture as the
     Producer of the Event Queue; the other is for the Event Manager as
     the consumer of the Event Queue */
    (void) xTaskCreate(TouchScreenCapture, "c:Screen", configMINIMAL_STACK_SIZE, NULL, mainDEMOTASKS_PRIORITY,
            &TouchScreenHnd); //  RTOS_USAGE

    (void) xTaskCreate(EventManager, "e:EventMgr", configMINIMAL_STACK_SIZE+256, NULL, mainDEMOTASKS_PRIORITY+2,
            &EventMgrHnd); //  RTOS_USAGE

    (void) xTaskCreate(R_TOUCH_SimulateTask, "s:TouchSim", configMINIMAL_STACK_SIZE+256, NULL, mainDEMOTASKS_PRIORITY+2,
            &TouchSimHnd); //  RTOS_USAGE

}
/*******************************************************************************
 End of function EventManagerInit
 *******************************************************************************/

/*******************************************************************************
 * Function Name: TouchScreenSuspend
 * Description  : Halts any activity for the touch screen
 * Arguments    : (void *)pvParameters
 * Return Value : none
 *******************************************************************************/
void TouchScreenSuspend (void)
{
    vTaskSuspend(TouchScreenHnd);
    R_DDLCD_DispMemoryRelease(TouchScreenHnd);
}

/*******************************************************************************
 * Function Name: TouchScreenResume
 * Description  : Halts any activity for the touch screen
 * Arguments    : (void *)pvParameters
 * Return Value : none
 *******************************************************************************/
void TouchScreenResume (void)
{
    vTaskResume(TouchScreenHnd); //  RTOS_USAGE
}

/*******************************************************************************
 * Function Name: TouchScreenCapture
 * Description  : Reads the touch screen data.
 * Arguments    : (void *)pvParameters
 * Return Value : none
 *******************************************************************************/
static void TouchScreenCapture (void *pvParameters)
{
    DDLCD_Point_t cursorLocation;

    EVENT_MSG msg;
    int16_t retVal;

    cursorLocation.x = cursorLocation.y = 0;
    msg.source = MS_TOUCHSCREEN;

    /* Initialize the touch screen */
    r_touch_init();

    for (;;)
    {

        /* This call allows us to sequence the touch DTC start in the 'safe'
         * vertical blanking period. */
        {
            R_DDLCD_DispMemoryAcquire(xTaskGetCurrentTaskHandle());

            /* Scanning input from touch panel; and if valid sending a an event message to the event manager */
            msg.event = r_touch_scantouch(&cursorLocation);

            /*  RTOS_USAGE */
            R_DDLCD_DispMemoryRelease(xTaskGetCurrentTaskHandle());

            if (msg.event != ME_INVALID)
            {
                msg.param.coord[0] = cursorLocation.x;
                msg.param.coord[1] = cursorLocation.y;

                retVal = (int16_t) xQueueSend(EventQueue, (void * )&msg, 0UL); //  RTOS_USAGE

                /* Tracking the failure frequency in sending event messages */
                if (pdPASS != retVal)
                {
                    sErrorEverOccurred++;
                }
            }

            /* In the GAPI demonstration mode, monitor SW1 and is pressed
             * enter the touch screen calibration mode. This mode is not entered if in
             * business card demonstration mode*/
            if ((0 == SW1) && (DEMO_MODE_CARD != g_demo_mode))
            {
                extern const Screen_t ScreenCalibrateData;

                while (0 == SW1)
                {
                    /* Wait */
                }

                /* send a screen change command to the handler */
                msg.source = MS_DISPLAY;
                msg.event = ME_SCREEN_PUSH;
                msg.param.pS = &ScreenCalibrateData;

                /*  RTOS_USAGE */
                retVal = (int16_t) xQueueSend(EventQueue, (void * )&msg, 0UL); //  RTOS_USAGE

                /* Tracking the failure frequency in sending event messages */
                if (pdPASS != retVal)
                {
                    sErrorEverOccurred++;
                }
            }
        }

    }
}
/*******************************************************************************
 End of function TouchScreenCapture
 *******************************************************************************/

/*******************************************************************************
 * Function Name: ResourceCallback
 * Description  : Does an example task. Making this longer just to see how it
 *                wraps.
 * Arguments    : state -
 *                    action to take
 *                pct -
 *                    Pointer of where to put the output data
 *                of -
 *
 * Return Value : none
 *******************************************************************************/
static void ResourceCallback (ResourceState_t state, uint32_t pct, uint32_t of)
{
    uint8_t string[32];

    uint32_t complete = (pct * 1000) / of;

    if (pct > of)
    {
        complete = 1000;
    }

    switch (state)
    {
        case RESOURCE_VERIFYING :
            sprintf((char *) string, "Verify");
        break;

        case RESOURCE_READING :
            sprintf((char *) string, "Reading %d%%", complete / 10);
        break;

        case RESOURCE_WRITING :
            sprintf((char *) string, "Writing %d%%", complete / 10);
        break;

        case RESOURCE_ERASING :
            sprintf((char *) string, "Erasing %d%%", complete / 10);
        break;

        default :

            /* Do nothing */
        break;
    }
    {
        static const BMP_ColorTable_t bar_ct[] =
        { CT_BLACK, CT_RED, CT_GREEN };
        static const GAPI_PUT_t bar_text =
        { &pfontTerminal,
        { CT_NONE, CT_BLACK }, 0, 0,
        { 1, 0, 0, 0, 1, 0 } };

        int16_t height = R_GAPI_FontHeight(&bar_text);
        int16_t width = (int16_t) ((complete * SX(0.8)) / 1000);
        int16_t posY = (int16_t) (SY(1.0) - ((int16_t) (height + 2) * (int16_t) (1 + state)));
        (void) R_GAPI_Fill(workFrameBuf, SX(0.1), posY, width, height, &bar_ct[2]);
        (void) R_GAPI_Fill(workFrameBuf, (int16_t) (SX(0.1) + width), posY, (int16_t) (SX(0.8) - width), height,
                &bar_ct[1]);
        (void) R_GAPI_PutS(string, &bar_text, workFrameBuf, SX(0.5), posY);
        (void) BMPCopyWorkDisplay(SX(0.1), posY, SX(0.8), height);
    }
}
/*******************************************************************************
 End of function ResourceCallback
 *******************************************************************************/

uint8_t ResourceWrite = 0;

static void EventManager (void *pvParameters)
{
    DDLCD_Resp_t err;
    EVENT_MSG msg;
    int16_t retVal;
    uint8_t demo_change_requested = false; /* Used to indicate if user was pressing SW1 during boot */

    const portTickType xBlockTime = port_mS_to_TICK(8); //  RTOS_USAGE

    if (0 == SW1)
    {
        /* SW1 was pressed */
        demo_change_requested = true;
    }

    /*  Initialize screen handler system */
    FramesInit();
    ScreenMgrInit();
    TimerRTC_init();

    {
        R_DDLCD_DispMemoryInit();   // start external memory manager to block tasks accessing external bus as necessary
        err = R_DDLCD_Init();     // Initialize the DD LCD display system
        if (0 != err)
        {
            nop();                // Initialization error
        }

        while (DDLCD_Vcount < 2) // wait for 2 frame before turning on LCD backlight and initializing memory
        {
            /* Wait */
        }

        R_DDLCD_Backlight(1);     // Turn on the backlight
    }

    {
        /* access internal resources */
        static const uint8_t IResources[] =
        {
#include "iresources.h"
                };

        pIResources = ResourceVerify(IResources);
        pfontTerminal = FileFind(pIResources, "fontTerminal");
    }

    /* Let system know we're accessing Display Memory */

    /*  RTOS_USAGE */
    R_DDLCD_DispMemoryAcquire(xTaskGetCurrentTaskHandle());

    /* verify resources to initialize pResources */
    pResources = ResourceVerify(__section_begin(".BResources"));

    /* display splash screen if it exists, illuminating LEDs while it is displayed */
    {
        BMP_t const *pSplash = FileFind(pResources, "_SplashScreen");

        portTickType start_time = xTaskGetTickCount(); //  RTOS_USAGE
        if (NULL != pSplash)
        {
            (void) R_GAPI_Copy(pSplash, workFrameBuf, (int16_t) ((DD_FRAME_WIDTH - R_BMP_Width(pSplash)) / 2),
                    (int16_t) ((DD_FRAME_HEIGHT - R_BMP_Height(pSplash)) / 2));
            (void) R_GAPI_WorkDisplay();
            LED0 = LED_ON;
            LED1 = LED_ON;

            R_DDLCD_DispMemoryRelease(xTaskGetCurrentTaskHandle());
            do
            {
                (void) xQueueReceive(EventQueue, &msg, xBlockTime); //  RTOS_USAGE
            } while (((ME_RELEASE != msg.event) && ((xTaskGetTickCount() - start_time) < port_mS_to_TICK(4000)))
                    || (0 == SW1));

            LED0 = LED_OFF;
            LED1 = LED_OFF;
        }
    }

    {

        g_demo_mode = r_config_get_demo_mode();
        if (true == demo_change_requested)
        {
            if (DEMO_MODE_CARD == g_demo_mode)
            {
                g_demo_mode = DEMO_MODE_GAPI;
                r_config_set_demo_mode(g_demo_mode);
            }
            else
            {
                g_demo_mode = DEMO_MODE_CARD;
                r_config_set_demo_mode(g_demo_mode);
            }
        }

        /* Check if business demo mode has been set in the configuration file */
        if (DEMO_MODE_CARD == g_demo_mode)
        {
            /* Go to the business card demonstration screen */
            extern Screen_t g_screen_card_data;
            msg.param.pS = &g_screen_card_data;
            msg.source = MS_DISPLAY;
            msg.event = ME_SCREEN_CHANGE;
        }
        else
        {
            /* In all other events (file open failed, other mode set etc)
             * go straight to the GAPI demonstration home screen */
            extern Screen_t ScreenHomeData;
            msg.param.pS = &ScreenHomeData;
            msg.source = MS_DISPLAY;
            msg.event = ME_SCREEN_CHANGE;
        }

        retVal = (int16_t) xQueueSend(EventQueue, (void * )&msg, 0UL);
        if (pdPASS != retVal)
        {
            /* Tracking the failure frequency in sending event messages */
            sErrorEverOccurred++;
        }
    }

    /* Lower priority to that of other tasks */
    /*  RTOS_USAGE */
    vTaskPrioritySet(NULL, mainDEMOTASKS_PRIORITY + 1);

    for (;;)
    {
        /* Check Event Queue...this call will suspend
         task for xBlockTime if no event to process */
        /*  RTOS_USAGE */
        retVal = (int16_t) xQueueReceive(EventQueue, &msg, xBlockTime);

        if (pdPASS == retVal)
        {
            {
                ScreenUpdate(&msg);
            }
        }
    }
}
/*******************************************************************************
 End of function EventManager
 *******************************************************************************/


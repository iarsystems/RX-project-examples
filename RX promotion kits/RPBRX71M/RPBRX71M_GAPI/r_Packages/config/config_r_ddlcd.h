/******************************************************************************
 * DISCLAIMER
 * Please refer to http://www.renesas.com/disclaimer
 ******************************************************************************/
/*                This version of DDCD only supports internal SRAM memory
 (needs no external memory).

 Please note there are other variants of Renesas Direct Drive
 that supports external memory of various types.                */
/**
 * This file configures the Direct LCD Package Library
 *  Refer to the API manual for more details.
 *
 */
#ifndef CONFIG_R_DDLCD_H
    #define CONFIG_R_DDLCD_H

    #include "platform.h"

/**
 * This macro configures the usage of the "DispMemory" manager 
 * mechanisms of this package. To use the supplied manager, set 
 * this value to 1. To create your own manager, set to 0 (see 
 * user manual). 
 *  
 * Usage of this manager REQUIRES the r_FreeRTOS package also. 
 */

    #define DD_USE_DISPMEMORY_MANAGER         (1)

    #if DD_USE_DISPMEMORY_MANAGER == 1
/**
 * This macro controls the RTOS priority of the DispMemory manager
 * task. This value must be greater than any other task that is 
 * using the external bus. 
 */
        #define DD_DISPMEMORY_MANAGER_PRIORITY      ( tskIDLE_PRIORITY + 3 )
/**
 * This macro configures the data structure that tracks the 
 * tasks that are currently using the external bus. The value 
 * must be greater than the number of tasks that would accessing
 * the external bus at any point in time. 
 */
        #define DD_DISPMEMORY_MANAGER_MAX_TASKS   (16)
    #endif

/**
 * This configures the priority of the ISR that controls 
 * the DMAC unit. These ISR are very fast <2uS but must 
 * be serviced deterministically (this needs to be high 
 * priority) 
 */
    #define DD_INTERRUPT_PRIORITY           (15)

/**
 * This is IPL for the configured "DOTCLK" timer.
 *  
 * This is IPL for the configured "DOTCLK" timer. 
 * This signal is used to coordinate access to the external bus. 
 * LCD_BusActive is a global uint16_t that can be checked at this 
 * ISR to know if bus is blocked by DMAC (1) or not (0) 
 */
    #define DD_NOTIFY_INTERRUPT_PRIORITY    (2)

/**
 * configure LCD Direct Drive mode of operation
 * Only DD_MODE_SRAM_NOMUX is supported on with DMA from internal memory.
 */
    #define DD_MODE_SRAM_NOMUX

/**
 * Controls the presentation of data Top to Bottom on panel. 
 * Define this to invert 
 * Define this if need High to Low memory Lines 
 * (default is Low to High)
 */
    #define DD_INVERT_V_LINES

/**
 * Controls the presentation of data Left to Right on panel. 
 * Define this to invert 
 * Define this if need High to Low memory Pixels 
 * (default is Low to High)
 */
/* #define DD_INVERT_H_DOT */

/**
 * Controls the placement of data on the panel (refer to user 
 * manual for availability on a particular MCU) 
 * Define this to rotate. 
 * All panel definitions are same as data sheet, but grapics API 
 * interpretation of data is rotated. 
 * For 320x240 QVGA panel, the raster data would be treated as 
 * 240x320 by graphics API. 
 * Note: Rotation is not supported on the RPB RX71M, this define
 * and comment is included for info only.
 */
/* #define DD_PANEL_ROTATE */

/**
 * Number of RAM raster frames to allocate. 
 * the locations are accessed via LCDSetRaster Routines. 
 *  
 * GAPI Demo Code uses 3 
 * frames...displayFrameBuf,backFrameBuf,workFrameBuf. 
 *  
 * Set to zero to not have this package allocate the frames and 
 * to allocate elsewhere. Must be allocated in the "DD_RASTERS" 
 * section. 
 */
    #define DD_RASTER_FRAME_COUNT (3)

/**
 * Specify the maximum number of regions that the screen can be 
 * split into (see user manual)
 */
    #define DD_FRAME_REGIONS_MAX (1)

/**
 * Specify size of RAM frame in pixels...must be greater 
 * to contain the desired output image.
 */
    #define DD_FRAME_SIZE (DD_V_LINES_DISPLAY*DD_H_DOT_DISPLAY)

/**
 * Desired system behaviour. Dot clock frequency must be integer
 * multiple of BCLK and PCLK (see user manual). 
 */
    #define DD_DOT_CLOCK_FREQUENCY_DATA         (7500000L)
    #define DD_DOT_CLOCK_FREQUENCY_BLANK        (7500000L)

    #define DD_DESIRED_FRAME_RATE               (60)
    #define DD_MINIMUM_MCU_ACCESS_PCT           (5)

#endif

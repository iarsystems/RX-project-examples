/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/**
 * This file configures the Direct LCD Package Library
 *  Refer to the API manual for more details
 */
#ifndef CONFIG_R_DDLCD_H
#define CONFIG_R_DDLCD_H

/**
 * This macro configures the usage of the "exmemory" manager 
 * mechanisms of this package. To use the supplied manager, set 
 * this value to 1. To create your own manager, set to 0 (see 
 * user manual). 
 *  
 * Usage of this manager REQUIRES the r_FreeRTOS package also. 
 */

#define DD_USE_EXMEMORY_MANAGER         1

#if DD_USE_EXMEMORY_MANAGER == 1
/**
 * This macro controls the RTOS priority of the exmemory manager
 * task. This value must be greater than any other task that is 
 * using the external bus. 
 */
#define DD_EXMEMORY_MANAGER_PRIORITY      ( tskIDLE_PRIORITY + 3 )
/**
 * This macro configures the data structure that tracks the 
 * tasks that are currently using the external bus. The value 
 * must be greater than the number of tasks that would accessing
 * the external bus at any point in time. 
 */
#define DD_EXMEMORY_MANAGER_MAX_TASKS 16
#endif

/**
 * This configures the priority of the ISR that controls 
 * the ExDMAC unit. These ISR are very fast <2uS but must 
 * be serviced deterministically (this needs to be high 
 * priority) 
 */
#define DD_INTERRUPT_PRIORITY 6

/**
 * This is IPL for the configured "DOTCLK" timer.
 *  
 * This is IPL for the configured "DOTCLK" timer. 
 * This signal is used to coordinate access to the external bus. 
 * LCD_BusActive is a global uint16_t that can be checked at this 
 * ISR to know if bus is blocked by ExDMAC (1) or not (0) 
 */
#define DD_NOTIFY_INTERRUPT_PRIORITY 2

/**
 * configure Raster_t based on width of Display Frame RAM 
 */
//typedef uint32_t Raster_t;
typedef uint16_t Raster_t;

/**
 * configure LCD Direct Drive mode of operation (refer to user manual)  
 */
//#define DD_MODE_SRAM
//#define DD_MODE_SRAM_NOMUX
#define DD_MODE_SDRAM
//#define DD_MODE_SDRAM_CLUSTER

/**
 * Controls the presentation of data Top to Bottom on panel. 
 * Define this to invert 
 * Define this if need High to Low memory Lines 
 * (default is Low to High)
 */
//#define DD_INVERT_V_LINES

/**
 * Controls the presentation of data Left to Right on panel. 
 * Define this to invert 
 * Define this if need High to Low memory Pixels 
 * (default is Low to High)
 */
//#define DD_INVERT_H_DOT

/**
 * Controls the placement of data on the panel (refer to user 
 * manual for availability on a particular MCU) 
 * Define this to rotate. 
 * All panel definitions are same as data sheet, but grapics API 
 * interpretation of data is rotated. 
 * For 320x240 QVGA panel, the raster data would be treated as 
 * 240x320 by graphics API. 
 * Define to allow user image to be rotated in transfer to the 
 * panel. 
 */
//#define DD_PANEL_ROTATE

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
#define DD_RASTER_FRAME_COUNT 3

/**
 * Specify the maximum number of regions that the screen can be 
 * split into (see user manual)
 */
#define DD_FRAME_REGIONS_MAX 1

/**
 * Specify size of RAM frame...each dimension must be greater 
 * than or equal to panel dimension. 
 */
#define DD_FRAME_HEIGHT DD_V_LINES_DISPLAY
#define DD_FRAME_WIDTH DD_H_DOT_DISPLAY

/**
 * Desired system behavior. Dot clock frequency must be integer 
 * multiple of BCLK and PCLK (see user manual). 
 */
#define DD_DOT_CLOCK_FREQUENCY_DATA 16000000L
#define DD_DOT_CLOCK_FREQUENCY_BLANK 12000000L

#define DD_DESIRED_FRAME_RATE 60
#define DD_MINIMUM_MCU_ACCESS_PCT 30

#endif

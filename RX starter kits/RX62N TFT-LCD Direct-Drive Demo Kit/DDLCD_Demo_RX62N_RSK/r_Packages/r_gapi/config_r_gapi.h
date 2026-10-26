/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/

/**
 * This file configures the GAPI Package Library
 *  Refer to the API manual for more details
 */
#ifndef CONFIG_R_GAPI_H
#define CONFIG_R_GAPI_H

/* configure Raster_t based on width of Display Frame RAM */
//typedef uint32_t Raster_t;
typedef uint16_t Raster_t;

/* configure number of 256/1024 byte pools allocated for R_GAPI_PoolMemory 
   setting this to zero will cause the GAPI Pool mechanism to not be build (supply your own) */
#define GAPI_MEMORY_POOLS 10

#endif

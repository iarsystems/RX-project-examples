/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************/
/**********************************************************************************
FILE NAME    : r_DDLCDExMemory.h

DESCRIPTION  : Interface to external memory manager
**********************************************************************************/
/**********************************************************************************/
#ifndef R_DDLCD_EXMEMORY_H
#define R_DDLCD_EXMEMORY_H

/**********************************************************************************
Function Prototypes
***********************************************************************************/
void R_DDLCD_ExMemoryInit(void);
void R_DDLCD_ExMemoryAcquire(xTaskHandle handle); /* RTOS_USAGE */
void R_DDLCD_ExMemoryRelease(xTaskHandle handle); /* RTOS_USAGE */

#endif


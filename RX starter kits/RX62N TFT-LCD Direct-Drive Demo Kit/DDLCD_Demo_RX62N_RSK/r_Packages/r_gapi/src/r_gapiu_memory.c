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
/***********************************************************************************
FILE NAME    : r_GAPIu_Memory.c

DESCRIPTION  : Memory Support for Renesas Graphics API.
              Provides a basic pool memory handler

Copyright   : 2007 Renesas Electronics America
Copyright   : 2007 Renesas Electronics Corporation.
          All Rights Reserved
***********************************************************************************/
/************************************************************************
Revision History
DD.MM.YYYY OSO-UID Description
31.05.2009 RTA-JMB Split from ExMemory.c
*
************************************************************************/
#include "r_gapi_private.h"
#include "r_FreeRTOS.h"

/* Section definition for external memory used for pool(s) */
enum{POOL_256,POOL_1024,POOLS};

#pragma location=".BGAPI_POOL"
  static uint8_t *pool_list[POOLS][GAPI_MEMORY_POOLS];  // usage list for the pools
  
#pragma location=".BGAPI_POOL_1"
  static uint8_t pool_256[GAPI_MEMORY_POOLS][256];      // for 256 byte blocks  
  
#pragma location=".BGAPI_POOL_1"  
  static uint8_t pool_1024[GAPI_MEMORY_POOLS][1024];    // for 1024 byte blocks  

/*****************************************************************************/
//
// FUNCTION    :R_GAPI_PoolMemoryGet
// PARAMETERS  :size...size of block being requested
// RETURNS     :Pointer to memory...NULL if not allocated
//
// DESCRIPTION :Requests a block of memory for an externally allocated memory pool.
//
/*****************************************************************************/
uint8_t * R_GAPI_PoolMemoryGet(uint16_t size)
{
  uint8_t list_index, index;
  uint8_t *pPool;
  _Bool return_flag = false;

  {
    static _Bool initialized = false;
    if (initialized == false)
    {
      // Initialize the memory pool list (once on first access)
      for (list_index = 0; list_index < POOLS; list_index++)
        for(index=0; index<GAPI_MEMORY_POOLS; index++)
          pool_list[list_index][index] = NULL;
       initialized = true;
    }
  }
      
  switch (size)
  {
    case 256:
      list_index = POOL_256;
      pPool = pool_256[0];
      break;

    case 1024:
      list_index = POOL_1024;
      pPool = pool_1024[0];
      break;

    default:
      return(NULL);
  }

  // Search for an open slot
  for(index=0; index<GAPI_MEMORY_POOLS; index++)
  {
    taskENTER_CRITICAL(); /* RTOS_USAGE */
    if (NULL == pool_list[list_index][index])
    {
      // Found an open slot
      pool_list[list_index][index] = &pPool[(uint32_t)index * (uint32_t)size];
      return_flag = true;
    }
    taskEXIT_CRITICAL();  /* RTOS_USAGE */
    if (return_flag)
      return(pool_list[list_index][index]);
  }
  // No open slots
  return (NULL);
}

/*****************************************************************************/
//
// FUNCTION    :R_GAPI_PoolMemoryFree
// PARAMETERS  :Pointer to memory
// RETURNS     :0=ok, non 0 = NG
//
// DESCRIPTION :Free a block of memory for an externally allocated memory pool.
//
/*****************************************************************************/
uint8_t R_GAPI_PoolMemoryFree(uint8_t *pPool)
{
  uint8_t list_index, index;

  // Search for matching pointer
  for (list_index = 0; list_index < POOLS; list_index++)
  {
    for(index=0; index<GAPI_MEMORY_POOLS; index++)
    {
      if (pPool == pool_list[list_index][index])
      {
        // Found pointer
        pool_list[list_index][index] = NULL;
        return (0);
      }
    }
  }
  // No matching pointer
  return (1);
}



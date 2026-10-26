/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/******************************************************************************
* File Name    : r_irq_rx_if.h
* Version      : 1.00  
* Device(s)    : Renesas RX class
* Tool-Chain   : Renesas RX Standard Toolchain  
* H/W Platform : 
* Description  : Definitions and declarations for the IRQ support module API.
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 20.03.2013 1.00    First Release
******************************************************************************/
#include "platform.h"
#include "r_irq_rx_config.h"
#include "r_irq_private.h"
#ifndef IRQ_RX_IF_H /* Multiple inclusion protection. */
#define IRQ_RX_IF_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* User specific IRQ configuration options */

/******************************************************************************
Typedef definitions
******************************************************************************/

/* Enumeration of various IRQ control command codes. */
typedef enum        
{
    IRQ_CMD_SET_PRIO = 0,
    IRQ_CMD_SET_TRIG,
    /* Reserved for future commands. */
    IRQ_CMD_LIST_END   /* This is not a command. Do not use it as a command or change it. */
}irq_cmd_t;

/* Enumeration of IRQ interrupt priority settings. */
typedef enum
{
	IRQ_PRI_0 = 0,
	IRQ_PRI_1,
	IRQ_PRI_2,
	IRQ_PRI_3,
	IRQ_PRI_4,
	IRQ_PRI_5,
	IRQ_PRI_6,
	IRQ_PRI_7,
	IRQ_PRI_8,
	IRQ_PRI_9,
	IRQ_PRI_10,
	IRQ_PRI_11,
	IRQ_PRI_12,
	IRQ_PRI_13,
	IRQ_PRI_14,
	IRQ_PRI_15,
} irq_prio_t;

/* Enumerated type for trigger type: low level, rising edge, falling edge, both edges. */
typedef enum
{
    IRQ_TRIG_LOWLEV     = 0x00,
    IRQ_TRIG_FALLING    = 0x04,
    IRQ_TRIG_RISING     = 0x08,
    IRQ_TRIG_BOTH_EDGE  = 0x0C

} irq_trigger_t;

typedef struct irq_init_block_s *irq_handle_t;

/* IRQ API error codes */
typedef enum _irq_err
{
    IRQ_SUCCESS=0,
    IRQ_ERR_BAD_NUM,        /* Invalid IRQ number. */
    IRQ_ERR_NOT_OPENED,     /* IRQ not yet opened. */
    IRQ_ERR_NOT_CLOSED,     /* IRQ still open from previous open. */
    IRQ_ERR_UNKNOWN_CMD,    /* Control command is not recognized. */
    IRQ_ERR_INVALID_ARG,    /* Argument is not valid for parameter. */
    IRQ_ERR_NULL_PTR,       /* Received null pointer; missing required argument. */
    IRQ_ERR_LOCK,           /* The lock procedure failed. */
} irq_err_t;

extern const mcu_lock_t g_irq_hdw_locks[];


/******************************************************************************
Functions Prototypes
******************************************************************************/
/*  Initializes the IRQ registers, enables interrupts, provides handle for other API functions. */
irq_err_t   R_IRQ_Open (uint8_t          irq_number,
                        irq_trigger_t    trigger,
                        irq_prio_t       priority,
                        irq_handle_t    *phandle,
                        void     (*const pcallback)(void *pargs));

/* Handles special hardware or software operations for the IRQ. */
irq_err_t   R_IRQ_Control(irq_handle_t  const phandle,
                          irq_cmd_t     const cmd,
                          void               *pcmd_data);

/* Fully disables the IRQ designated by the handle. */
irq_err_t   R_IRQ_Close(irq_handle_t phandle);

/* Reads the current level of the pin assigned to the specified IRQ. */
irq_err_t   R_IRQ_ReadInput(irq_handle_t  const phandle,
                            bool               *plevel);


/* Enables or disables the ICU interrupt for the specified IRQ. */
irq_err_t   R_IRQ_InterruptEnable (irq_handle_t  const phandle,
                                   bool                enable);
/* Returns the version number of the module. */
uint32_t  R_IRQ_GetVersion(void);

#endif /* IRQ_RX_IF_H    */



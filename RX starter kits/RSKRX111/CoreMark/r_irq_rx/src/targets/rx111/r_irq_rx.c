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
/***********************************************************************************************************************
* File Name    : r_irq_rx.c
* Version      : 1.00
* Device(s)    : Renesas RX class MCU
* Tool-Chain   : Renesas RX Standard Toolchain
* H/W Platform :
* Description  : Implementation of IRQ support API for RX111
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 20.03.2013  1.00    First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <stdlib.h>
#include "platform.h"
#include "r_irq_rx_if.h"
#include "r_irq_rx_config.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define IRQ_VECT_BASE   VECT_ICU_IRQ0   /* IRQ vector numbers are all offsets from here. */
#define IRQ_IPR_BASE    IPR_ICU_IRQ0    /* IRQ priority register locations are all offsets from here. */

#define IRQ_TRIGGER_MASK 0xF3            /* Reserved bits in IRQ detection sense register. */
#define IRQ_MAX_PRIORITY 15
/***********************************************************************************************************************
Exported global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

#if IRQ_CFG_USE_IRQ0 == 1
irq_callback p_callback_irq0;
#if defined(__IAR_SYSTEMS_ICC__)
#define IRQ_PORT_IRQ0_PIDR_BYTE 0x08C043
const irq_init_block_t g_irq0_handle =
{
    0,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ0,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ0,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ0,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq0,                    /* pointer to callback function pointer. */
    (void *)IRQ_PORT_IRQ0_PIDR_BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#else
const irq_init_block_t g_irq0_handle =
{
    0,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ0,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ0,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ0,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq0,                    /* pointer to callback function pointer. */
    &(uint8_t)IRQ_PORT_IRQ0.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#endif
#endif

#if IRQ_CFG_USE_IRQ1 == 1
irq_callback p_callback_irq1;
#if defined(__IAR_SYSTEMS_ICC__)
#define IRQ_PORT_IRQ1_PIDR_BYTE 0x08C043
const irq_init_block_t g_irq1_handle =
{
    1,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ1,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ1,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ1,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq1,                    /* pointer to callback function pointer. */
    (void *)IRQ_PORT_IRQ1_PIDR_BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#else
const irq_init_block_t g_irq1_handle =
{
    1,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ1,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ1,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ1,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq1,                    /* pointer to callback function pointer. */
    &(uint8_t)IRQ_PORT_IRQ1.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#endif
#endif

#if IRQ_CFG_USE_IRQ2 == 1
irq_callback p_callback_irq2;
const irq_init_block_t g_irq2_handle =
{
    2,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ2,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ2,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ2,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq2,                    /* pointer to callback function pointer. */
    &(uint8_t)IRQ_PORT_IRQ2.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ3 == 1
irq_callback p_callback_irq3;
const irq_init_block_t g_irq3_handle =
{
    3,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ3,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ3,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ3,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq3,                    /* pointer to callback function pointer. */
    &(uint8_t)IRQ_PORT_IRQ3.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ4 == 1
irq_callback p_callback_irq4;
#if defined(__IAR_SYSTEMS_ICC__)
#define IRQ_PORT_IRQ4_PIDR_BYTE 0x08C04E
const irq_init_block_t g_irq4_handle =
{
    4,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ4,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ4,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ4,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq4,                    /* pointer to callback function pointer. */
    (void *)IRQ_PORT_IRQ4_PIDR_BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#else
const irq_init_block_t g_irq4_handle =
{
    4,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ4,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ4,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ4,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq4,                    /* pointer to callback function pointer. */
    &(uint8_t)IRQ_PORT_IRQ4.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#endif
#endif

#if IRQ_CFG_USE_IRQ5 == 1
irq_callback p_callback_irq5;
const irq_init_block_t g_irq5_handle =
{
    5,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ5,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ5,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ5,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq5,                    /* pointer to callback function pointer. */
    &(uint8_t)IRQ_PORT_IRQ5.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ6 == 1
irq_callback p_callback_irq6;
const irq_init_block_t g_irq6_handle =
{
    6,     //uint8_t  const irq_num;
    (uint8_t)IRQ_IEN_MASK_IRQ6,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ6,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ6,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq6,                    /* pointer to callback function pointer. */
    &(uint8_t)IRQ_PORT_IRQ6.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#endif

#if IRQ_CFG_USE_IRQ7 == 1
irq_callback p_callback_irq7;
const irq_init_block_t g_irq7_handle =
{
    7,                                   /* IRQ number. */
    (uint8_t)IRQ_IEN_MASK_IRQ7,          /* Bit position of interrupt enable bit in interrupt enable register. */
    (uint8_t)IRQ_CFG_FILT_PLCK_IRQ7,     /* PCLK divisor setting for the input pin digital filter. */
    (uint8_t)IRQ_CFG_FILT_EN_IRQ7,       /* Filter enable setting (on or off) for the input pin digital filter. */
    &p_callback_irq7,                    /* pointer to callback function pointer. */
    &(uint8_t)IRQ_PORT_IRQ7.PIDR.BYTE,   /* Pointer to the I/O port input data register for this IRQ. */
    IRQ_PORT_BIT_IRQ0,                   /* I/O port input data bit mask for this IRQ. */
};
#endif


static const irq_handle_t g_irq_handles[] =
{
    #if IRQ_CFG_USE_IRQ0 == 1
    (irq_handle_t)&g_irq0_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ1 == 1
    (irq_handle_t)&g_irq1_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ2 == 1
    (irq_handle_t)&g_irq2_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ3 == 1
    (irq_handle_t)&g_irq3_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ4 == 1
    (irq_handle_t)&g_irq4_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ5 == 1
    (irq_handle_t)&g_irq5_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ6 == 1
    (irq_handle_t)&g_irq6_handle,
    #else
    NULL,
    #endif
    #if IRQ_CFG_USE_IRQ7 == 1
    (irq_handle_t)&g_irq7_handle,
    #else
    NULL,
    #endif
};

static uint8_t g_irq_opened[IRQ_MAX_IRQ_NUM + 1] = {0};

/***********************************************************************************************************************
Function definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_IRQ_Open
* Description  : Initializes the IRQ registers, enables interrupts, provides handle for other API functions.
* Arguments    : irq_number -
*                    Number of the IRQ to be initialized
*                trigger -
*                    Enumerated type for trigger type: low level, rising edge, falling edge, both edges.
*                irq_number -
*                    Number of the IRQ to be initialized.
*                phandle -
*                    Pointer to a handle for IRQ. Handle value will be set by this function.
*                pcallback -
*                    Pointer to function called from interrupt
* Return Value : IRQ_SUCCESS -
*                    Successful; IRQ initialized.
*                IRQ_ERR_BAD_NUM -
*                    IRQ number is invalid or unavailable.
*                IRQ_ERR_ NOT_CLOSED -
*                    IRQ currently in operation; Perform R_IRQ_Close() first.
*                IRQ_ERR_NULL_PTR -
*                    phandle pointer is NULL.
*                IRQ_ERR_INVALID_ARG -
*                    An invalid argument value was passed.
*                IRQ_ERR_LOCK -
*                    The lock could not be acquired.
***********************************************************************************************************************/
irq_err_t   R_IRQ_Open (uint8_t          irq_number,
                        irq_trigger_t    trigger,
                        irq_prio_t       priority,
                        irq_handle_t    *phandle,
                        void        (*const pcallback)(void *pargs))
{
    irq_err_t ret = IRQ_SUCCESS;

#if IRQ_CFG_REQUIRE_LOCK == 1
    bool        lock_result = false;
#endif

#if IRQ_CFG_CHECK_ARGS == 1
    if ((IRQ_MAX_IRQ_NUM < irq_number) || (NULL == g_irq_handles[irq_number]))
    {
        return IRQ_ERR_BAD_NUM;
    }
    if (g_irq_opened[irq_number]) /* Check for IRQ already opened. */
    {
        return  IRQ_ERR_NOT_CLOSED;
    }
    if ((trigger & IRQ_TRIGGER_MASK) || (priority > IRQ_MAX_PRIORITY))
    {
        return IRQ_ERR_INVALID_ARG;
    }
    if ((NULL == phandle) || (NULL == pcallback))
    {
        return IRQ_ERR_NULL_PTR;
    }
#endif

#if IRQ_CFG_REQUIRE_LOCK == 1
    /* Attempt to acquire lock for this IRQ. Prevents reentrancy conflict. */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_IRQ0 + irq_number));

    if(false == lock_result)
    {
        return IRQ_ERR_LOCK; /* The open function is currently locked. */
    }
#endif

    irq_handle_t  my_handle = g_irq_handles[irq_number];

    /* Each IRQ has its own enable bit in the same interrupt enable register,
     * so the address of IER register IER_ICU_IRQ0 works for all IRQs. */
    ICU.IER[IER_ICU_IRQ0].BYTE &= ~(my_handle->ien_bit_mask);   /* Disable interrupt request. */
    ICU.IRQFLTE0.BYTE          &= ~(my_handle->ien_bit_mask);   /* Disable digital filter.  */

    /* Set up the digital filter sampling clock divisor IRQFLTC0.FCLKSELi[1:0] bits for the IRQ. */
    ICU.IRQFLTC0.WORD &= ~(0x0003 << (irq_number * 2));                         /* First clear them. */
    ICU.IRQFLTC0.WORD |= ((uint16_t)(my_handle->filt_clk_div) << (irq_number * 2)); /* Now set them. */

    /* 4. Make or confirm the I/O port settings. */

    /* Set the method of detection for the interrupt in the IRQCRi.IRQMD[1:0] bits. */
    ICU.IRQCR[irq_number].BYTE = (uint8_t)trigger;

    /* Clear the corresponding IRn.IR flag to 0 (if edge detection is in use). */
    if(IRQ_TRIG_LOWLEV != trigger)
    {
        ICU.IR[IRQ_VECT_BASE + irq_number].BYTE = 0;
    }

    /* Set the IRQFLTEn.FLTENi bit (digital filter enabled) according to user configuration. */
    ICU.IRQFLTE0.BYTE |= ((my_handle->filt_enable) << irq_number) & (my_handle->ien_bit_mask);

    /* Set the interrupt priority level. */
    ICU.IPR[IRQ_IPR_BASE + irq_number].BYTE = (uint8_t)(priority & 0x0F);

    *(my_handle->p_irq_callback) = pcallback; /* Assign the callback function pointer. */

    *phandle = my_handle;   /* Return a pointer to a handle for the IRQ. */

    /* Set the IERm.IENj bit to 1 (interrupt request enabled). */
    ICU.IER[IER_ICU_IRQ0].BYTE |= my_handle->ien_bit_mask;   /* Enable interrupt request. */

    g_irq_opened[irq_number] = 1;      /* Flag that IRQ has now been opened. */

#if IRQ_CFG_REQUIRE_LOCK == 1
    /* Release lock for this IRQ. */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_IRQ0 + irq_number));
#endif
    return ret;
} /* End function R_IRQ_Open() */


/***********************************************************************************************************************
* Function Name: R_IRQ_Control
* Description  : Handles special hardware or software operations for the IRQ.
* Arguments    : phandle -
*                    Pointer to the handle for the IRQ.
*                cmd -
*                    Enumerated command code:  enable filtering, disable filtering, set filter clock, set priority
*                pcmd_data -
*                    Pointer to the command-data structure parameter of type void that is used to reference the location
*                    of any data specific to the command that is needed for its completion.
* Return Value : IRQ_SUCCESS -
*                    Command successfully completed.
*                IRQ_ERR_BAD_NUM -
*                    IRQ number is invalid or unavailable.
*                IRQ_ERR_ NOT_OPEN -
*                    The IRQ has not been opened.  Perform R_IRQ_Open() first
*                IRQ_ERR_UNKNOWN_CMD-
*                    Control command is not recognized.
*                IRQ_ERR_NULL_PTR -
*                    pcmd_data  pointer or phandle pointer is NULL
*                IRQ_ERR_INVALID_ARG -
*                    An element of the pcmd_data structure contains an invalid value.
*                IRQ_ERR_LOCK -
*                    The lock could not be acquired.
***********************************************************************************************************************/
irq_err_t   R_IRQ_Control(irq_handle_t  const phandle,
                          irq_cmd_t     const cmd,
                          void               *pcmd_data)
{
    irq_err_t ret = IRQ_SUCCESS;
    irq_prio_t    *prio_cmd_dat;
    irq_trigger_t *trig_cmd_dat;
    uint8_t irq_number = 0;         /* Place for dereferenced IRQ number. */
    uint8_t ien_temp;				/* For saving interrupt enable state. */
#if IRQ_CFG_REQUIRE_LOCK == 1
    bool        lock_result = false;
#endif

    /* Validate input arguments. */
#if IRQ_CFG_CHECK_ARGS == 1
    if ((NULL == phandle) || (NULL == pcmd_data))
    {
        return IRQ_ERR_NULL_PTR;
    }

    irq_number = irq_number;
    if (IRQ_MAX_IRQ_NUM < phandle->irq_num)
    {
        return IRQ_ERR_BAD_NUM;
    }

    if (!(g_irq_opened[irq_number])) /* Check for IRQ not opened. */
    {
        return  IRQ_ERR_NOT_OPENED;
    }
    if (IRQ_CMD_LIST_END <= cmd)
    {
        return IRQ_ERR_UNKNOWN_CMD;
    }
#endif

#if IRQ_CFG_REQUIRE_LOCK == 1
    /* Attempt to acquire lock for this IRQ. Prevents reentrancy conflict. */
    lock_result = R_BSP_HardwareLock((mcu_lock_t)(BSP_LOCK_IRQ0 + irq_number));

    if(false == lock_result)
    {
        return IRQ_ERR_LOCK; /* The open function is currently locked. */
    }
#endif


    ien_temp = ICU.IER[IER_ICU_IRQ0].BYTE;
    ICU.IER[IER_ICU_IRQ0].BYTE &= ~(phandle->ien_bit_mask);   /* Disable interrupt request. */

    if(IRQ_CMD_SET_PRIO == cmd)
    {
        prio_cmd_dat = (irq_prio_t*)pcmd_data;

        #if IRQ_CFG_CHECK_ARGS == 1
        if (*prio_cmd_dat > IRQ_MAX_PRIORITY)
        {
            return IRQ_ERR_INVALID_ARG;
        }
        #endif

        /* Set the interrupt priority level. */
        ICU.IPR[IRQ_IPR_BASE + irq_number].BYTE = (uint8_t)(*prio_cmd_dat & 0x0F);
    }
    else if(IRQ_CMD_SET_TRIG == cmd)
    {
        trig_cmd_dat = (irq_trigger_t*)pcmd_data;

        #if IRQ_CFG_CHECK_ARGS == 1
        if (*trig_cmd_dat & IRQ_TRIGGER_MASK)
        {
            return IRQ_ERR_INVALID_ARG;
        }
        #endif

        /* Set the method of detection for the interrupt in the IRQCRi.IRQMD[1:0] bits. */
        ICU.IRQCR[irq_number].BYTE = (uint8_t)(*trig_cmd_dat & IRQ_TRIGGER_MASK);

        /* Clear the corresponding IRn.IR flag to 0 (if edge detection is in use). */
        if(IRQ_TRIG_LOWLEV != *trig_cmd_dat)
        {
            ICU.IR[IRQ_VECT_BASE + irq_number].BYTE = 0;
        }
    }
    else
    {
     /* Nothing else. New commands would go here. */
    }

    ICU.IER[IER_ICU_IRQ0].BYTE = ien_temp;   /* Restore original interrupt enable state. */

#if IRQ_CFG_REQUIRE_LOCK == 1
    /* Release lock for this IRQ. */
    R_BSP_HardwareUnlock((mcu_lock_t)(BSP_LOCK_IRQ0 + irq_number));
#endif
    return ret;
} /* End function R_IRQ_Control() */


/***********************************************************************************************************************
* Function Name: R_IRQ_Close
* Description  : Fully disables the IRQ designated by the handle.
* Arguments    : phandle -
*                    Pointer to the handle for the IRQ.
*                proc_id -
*                    Calling process identifier
* Return Value : IRQ_SUCCESS -
*                    Successful; IRQ closed.
*                IRQ_ERR_BAD_NUM -
*                    IRQ number is invalid or unavailable.
*                IRQ_ERR_ NOT_OPEN -
*                    The IRQ has not been opened so closing has no effect.
*                IRQ_ERR_NULL_PTR -
*                    A required pointer argument is NULL
*                IRQ_ERR_LOCK_NOT_OWN -
*                    The calling procedure does not own the lock.
***********************************************************************************************************************/
irq_err_t   R_IRQ_Close(irq_handle_t phandle)
{
    irq_err_t ret = IRQ_SUCCESS;

    /* Validate input arguments. */
#if IRQ_CFG_CHECK_ARGS == 1
    if (NULL == phandle)
    {
        return IRQ_ERR_NULL_PTR;
    }
    if (IRQ_MAX_IRQ_NUM < phandle->irq_num)
    {
        return IRQ_ERR_BAD_NUM;
    }
    if (!(g_irq_opened[phandle->irq_num])) /* Check for IRQ not opened. */
    {
        return  IRQ_ERR_NOT_OPENED;
    }
#endif
    /* Each IRQ has its own enable bit in the same interrupt enable register,
     * so the address of IER register IER_ICU_IRQ0 works for all IRQs. */
    ICU.IER[IER_ICU_IRQ0].BYTE &= ~(phandle->ien_bit_mask);   /* Disable interrupt request. */
    ICU.IRQFLTE0.BYTE          &= ~(phandle->ien_bit_mask);   /* Disable digital filter.  */

    /* TODO: Disable interrupt source setting in MPC register for input pin. */
    g_irq_opened[phandle->irq_num] = 0;      /* Flag that IRQ has now been closed. */

    return ret;
} /* End function R_IRQ_Close() */


/***********************************************************************************************************************
* Function Name: R_IRQ_ReadInput
* Description  : Reads the current level of the pin assigned to the specified IRQ.
* Arguments    : phandle -
*                    Handle pointer for the IRQ.
*                plevel -
*                    Pointer to storage for the returned value of the input level.
* Return Value : IRQ_SUCCESS -
*                    Operation successfully completed.
*                IRQ_ERR_BAD_NUM -
*                    IRQ number is invalid or unavailable.
*                IRQ_ERR_ NOT_OPEN -
*                    The IRQ has not been opened.  Perform R_IRQ_Open() first
*                IRQ_ERR_NULL_PTR -
*                    phandle pointer is NULL
***********************************************************************************************************************/
irq_err_t   R_IRQ_ReadInput(irq_handle_t const phandle, bool *plevel)
{
    irq_err_t ret = IRQ_SUCCESS;
    /* Validate input arguments. */
#if IRQ_CFG_CHECK_ARGS == 1
    if ((NULL == phandle) || (NULL == plevel))
    {
        return IRQ_ERR_NULL_PTR;
    }
    if (IRQ_MAX_IRQ_NUM < phandle->irq_num)
    {
        return IRQ_ERR_BAD_NUM;
    }
    if (!(g_irq_opened[phandle->irq_num])) /* Check for IRQ not opened. */
    {
        return  IRQ_ERR_NOT_OPENED;
    }
#endif

    *plevel = (bool)(*phandle->irq_in_port & phandle->irq_port_bit);

    return ret;
} /* End function R_IRQ_ReadInput() */


/***********************************************************************************************************************
* Function Name: R_IRQ_InterruptEnable
* Description  : Enables or disables the ICU interrupt for the specified IRQ.
* Arguments    : phandle -
*                    Handle pointer for the IRQ.
*                enable -
*                    true  = enable the interrupt. false = disable interrupt.
* Return Value : IRQ_SUCCESS -
*                    Operation successfully completed.
*                IRQ_ERR_BAD_NUM -
*                    IRQ number is invalid or unavailable.
*                IRQ_ERR_ NOT_OPEN -
*                    The IRQ has not been opened.  Perform R_IRQ_Open() first
*                IRQ_ERR_NULL_PTR -
*                    phandle pointer is NULL
***********************************************************************************************************************/
irq_err_t   R_IRQ_InterruptEnable (irq_handle_t const phandle, bool enable)
{
    irq_err_t ret = IRQ_SUCCESS;
    /* Validate input arguments. */
#if IRQ_CFG_CHECK_ARGS == 1
    if (NULL == phandle)
    {
        return IRQ_ERR_NULL_PTR;
    }
    if (IRQ_MAX_IRQ_NUM < phandle->irq_num)
    {
        return IRQ_ERR_BAD_NUM;
    }
    if (!(g_irq_opened[phandle->irq_num])) /* Check for IRQ not opened. */
    {
        return  IRQ_ERR_NOT_OPENED;
    }
#endif

    if(enable)
    {
        ICU.IER[IER_ICU_IRQ0].BYTE |= phandle->ien_bit_mask;   /* Enable interrupt request. */
        if(ICU.IER[IER_ICU_IRQ0].BYTE) /* dummy read to complete pipelining. */
        {
            nop();
        }
    }
    else
    {
        ICU.IER[IER_ICU_IRQ0].BYTE &= ~(phandle->ien_bit_mask);   /* Disable interrupt request. */
        if(ICU.IER[IER_ICU_IRQ0].BYTE) /* dummy read to complete pipelining. */
        {
            nop();
        }
    }
    return ret;
} /* End function R_IRQ_InterruptEnable() */

/***********************************************************************************************************************
* Function Name: R_IRQ_GetVersion
* Description : Returns the version of this module. The version number is
* encoded where the top 2 bytes are the major version number and
* the bottom 2 bytes are the minor version number.
* For example, Rev 4.25 would be 0x00040019.
* NOTE: This function is inlined using #pragma inline directive.
* Arguments : none
* Return Value : Version Number
***********************************************************************************************************************/
#if defined(__RENESAS__)
#pragma inline(R_IRQ_GetVersion)
#endif
uint32_t R_IRQ_GetVersion(void)
{
	uint32_t version_number = 0;
	/* Bring in major version number. */
	version_number = ((uint16_t)IRQ_RX_VERSION_MAJOR) << 16;
	/* Bring in minor version number. */
	version_number |= (uint16_t)IRQ_RX_VERSION_MINOR;
	return version_number;
}

/***********************************************************************************************************************
* Description  : IRQ interrupt handler routines.
***********************************************************************************************************************/
#if IRQ_CFG_USE_IRQ0 == 1

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector= VECT(ICU, IRQ0)
__interrupt void irq0_isr(void)
#else
#pragma interrupt (irq0_isr(vect = VECT(ICU, IRQ0)))
static void irq0_isr(void)
#endif
{
    if((FIT_NO_FUNC != *(g_irq0_handle.p_irq_callback)) && (NULL != *(g_irq0_handle.p_irq_callback)))
    {
        (*(g_irq0_handle.p_irq_callback))((void*)&(g_irq0_handle.irq_num));
    }
} /* end irq0_isr */
#endif

#if IRQ_CFG_USE_IRQ1 == 1

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector= VECT(ICU, IRQ1)
__interrupt void irq1_isr(void)
#else
#pragma interrupt (irq1_isr(vect = VECT(ICU, IRQ1)))
static void irq1_isr(void)
#endif
{
    if((FIT_NO_FUNC != *(g_irq1_handle.p_irq_callback)) && (NULL != *(g_irq1_handle.p_irq_callback)))
    {
        (*(g_irq1_handle.p_irq_callback))((void*)&(g_irq1_handle.irq_num));
    }
} /* end irq1_isr */
#endif

#if IRQ_CFG_USE_IRQ2 == 1

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector= VECT(ICU, IRQ2)
__interrupt void irq2_isr(void)
#else
#pragma interrupt (irq2_isr(vect = VECT(ICU, IRQ2)))
static void irq2_isr(void)
#endif
{
    if((FIT_NO_FUNC != *(g_irq2_handle.p_irq_callback)) && (NULL != *(g_irq2_handle.p_irq_callback)))
    {
        (*(g_irq2_handle.p_irq_callback))((void*)&(g_irq2_handle.irq_num));
    }
} /* end irq2_isr */
#endif

#if IRQ_CFG_USE_IRQ3 == 1

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector= VECT(ICU, IRQ3)
__interrupt void irq3_isr(void)
#else
#pragma interrupt (irq3_isr(vect = VECT(ICU, IRQ3)))
static void irq3_isr(void)
#endif
{
    if((FIT_NO_FUNC != *(g_irq3_handle.p_irq_callback)) && (NULL != *(g_irq3_handle.p_irq_callback)))
    {
        (*(g_irq3_handle.p_irq_callback))((void*)&(g_irq3_handle.irq_num));
    }
} /* end irq3_isr */
#endif

#if IRQ_CFG_USE_IRQ4 == 1

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector= VECT(ICU, IRQ4)
__interrupt void irq4_isr(void)
#else
#pragma interrupt (irq4_isr(vect = VECT(ICU, IRQ4)))
static void irq4_isr(void)
#endif
{
    if((FIT_NO_FUNC != *(g_irq4_handle.p_irq_callback)) && (NULL != *(g_irq4_handle.p_irq_callback)))
    {
        (*(g_irq4_handle.p_irq_callback))((void*)&(g_irq4_handle.irq_num));
    }
} /* end irq4_isr */
#endif

#if IRQ_CFG_USE_IRQ5 == 1

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector= VECT(ICU, IRQ5)
__interrupt void irq5_isr(void)
#else
#pragma interrupt (irq5_isr(vect = VECT(ICU, IRQ5)))
static void irq5_isr(void)
#endif
{
    if((FIT_NO_FUNC != *(g_irq5_handle.p_irq_callback)) && (NULL != *(g_irq5_handle.p_irq_callback)))
    {
        (*(g_irq5_handle.p_irq_callback))((void*)&(g_irq5_handle.irq_num));
    }
} /* end irq5_isr */
#endif

#if IRQ_CFG_USE_IRQ6 == 1

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector= VECT(ICU, IRQ6)
__interrupt void irq6_isr(void)
#else
#pragma interrupt (irq6_isr(vect = VECT(ICU, IRQ6)))
static void irq6_isr(void)
#endif
{
    if((FIT_NO_FUNC != *(g_irq6_handle.p_irq_callback)) && (NULL != *(g_irq6_handle.p_irq_callback)))
    {
        (*(g_irq6_handle.p_irq_callback))((void*)&(g_irq6_handle.irq_num));
    }
} /* end irq6_isr */
#endif

#if IRQ_CFG_USE_IRQ7 == 1

#if defined(__IAR_SYSTEMS_ICC__)
#pragma vector= VECT(ICU, IRQ7)
__interrupt void irq7_isr(void)
#else
#pragma interrupt (irq7_isr(vect = VECT(ICU, IRQ7)))
static void irq7_isr(void)
#endif
{
    if((FIT_NO_FUNC != *(g_irq7_handle.p_irq_callback)) && (NULL != *(g_irq7_handle.p_irq_callback)))
    {
        (*(g_irq7_handle.p_irq_callback))((void*)&(g_irq7_handle.irq_num));
    }
} /* end irq7_isr */
#endif


/***********************************************************************************************************************
end  r_irq_rx.c
***********************************************************************************************************************/

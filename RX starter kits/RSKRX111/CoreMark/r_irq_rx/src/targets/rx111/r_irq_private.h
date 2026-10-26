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
* File Name    : r_irq_private.h
* Version      : 1.00
* Device(s)    : Renesas RX class MCU
* Tool-Chain   : Renesas RX Standard Toolchain
* H/W Platform :
* Description  : Private definitions for the RX FIT IRQ support module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY  Version Description
*         : 20.03.2013  1.00    First Release
***********************************************************************************************************************/
#ifndef R_IRQ_PRIVATE_H_
#define R_IRQ_PRIVATE_H_

#include "platform.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef void(*irq_callback)(void *pargs);

typedef enum
{
    IRQ_BIT0 = 0x01,
    IRQ_BIT1 = 0x02,
    IRQ_BIT2 = 0x04,
    IRQ_BIT3 = 0x08,
    IRQ_BIT4 = 0x10,
    IRQ_BIT5 = 0x20,
    IRQ_BIT6 = 0x40,
    IRQ_BIT7 = 0x80,
} irq_8bit_mask_t;

/* Definition of  structure */
typedef struct irq_init_block_s
{
    uint8_t const irq_num;
    uint8_t const ien_bit_mask;    /* Bit mask for the interrupt enable register bit for this IRQ. */
    uint8_t const filt_clk_div;    /* PCLK divisor setting for the input pin digital filter. */
    uint8_t const filt_enable;     /* Filter enable setting (on or off) for the input pin digital filter. */
    irq_callback *const p_irq_callback; /* pointer to callback function pointer. */
    uint8_t const *irq_in_port;    /* Pointer to the I/O port input data register for this IRQ. */
    irq_8bit_mask_t irq_port_bit;  /* I/O port input data bit mask for this IRQ. */
} irq_init_block_t;

/******************************************************************************
Macro definitions
******************************************************************************/
/* Version Number of API. */
#define IRQ_RX_VERSION_MAJOR           (1)
#define IRQ_RX_VERSION_MINOR           (0)

/* Bit position of interrupt enable bits in interrupt enable register. */
#define IRQ_IEN_MASK_IRQ0   (0x01)
#define IRQ_IEN_MASK_IRQ1   (0x02)
#define IRQ_IEN_MASK_IRQ2   (0x04)
#define IRQ_IEN_MASK_IRQ3   (0x08)
#define IRQ_IEN_MASK_IRQ4   (0x10)
#define IRQ_IEN_MASK_IRQ5   (0x20)
#define IRQ_IEN_MASK_IRQ6   (0x40)
#define IRQ_IEN_MASK_IRQ7   (0x80)

#if defined(BSP_MCU_RX111)
#define IRQ_MAX_IRQ_NUM    7
#endif

#endif /* R_IRQ_PRIVATE_H_ */

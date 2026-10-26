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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : hwsetup.c
* Device(s)    : RX
* H/W Platform : RSK+RX62N
* Description  : Defines the initialization routines used each time the MCU is restarted.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 31.10.2011 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* I/O Register and board definitions */
#include "platform.h"

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
/* MCU I/O port configuration function delcaration */
static void output_ports_configure(void);

/* Interrupt configuration function delcaration */
static void interrupts_configure(void);

/* MCU peripheral module configuration function declaration */
static void peripheral_modules_enable(void);


/***********************************************************************************************************************
* Function name: hardware_setup
* Description  : Contains setup functions called at device restart
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void hardware_setup(void)
{
    output_ports_configure();
    interrupts_configure();
    peripheral_modules_enable();
}

/***********************************************************************************************************************
* Function name: operating_frequency_set
* Description  : Configures the clock settings for each of the device clocks
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
void operating_frequency_set (void)
{
    /* Used for constructing value to write to SCKCR register. */
    uint32_t temp_clock = 0;

    /* 
    DEFAULT SETTINGS:
    Clock Description              Frequency
    ----------------------------------------
    Input Clock Frequency..............12MHz
    Internal Clock Frequency...........96MHz
    Peripheral Clock Frequency.........48MHz
    External Bus Clock Frequency.......12MHz */
    
    /* SCKCR - System Clock Control Register 
    b27:b23 ICK[3:0] = 0, system clock = EXTAL * 8 = 96 MHz
    b23     PSTOP1 = 1, BCLK pin output disabled
    b22     PSTOP0 = 1, SDCLK pin output disabled
    b19:b16 BCK[3:0] = 3, BCLK set to lowest multiplier (x1)to reduce power
    b11:b8  PCK[3:0] = 1, PCK = EXTAL * 4 = 48 MHz
    */

    /* Figure out setting for ICK bits. */
#if   BSP_CFG_ICK_MUL == 8
    /* Do nothing since ICK bits should be 0. */
#elif BSP_CFG_ICK_MUL == 4
    temp_clock |= 0x01000000;
#elif BSP_CFG_ICK_MUL == 2
    temp_clock |= 0x02000000;
#elif BSP_CFG_ICK_MUL == 1
    temp_clock |= 0x03000000;
#else
    #error "Error! Invalid setting for BSP_CFG_ICK_MUL in r_bsp_config.h"
#endif

    /* Figure out setting for BCK bits. */
#if   BSP_CFG_BCK_MUL == 8
    /* Do nothing since BCK bits should be 0. */
#elif BSP_CFG_BCK_MUL == 4
    temp_clock |= 0x00010000;
#elif BSP_CFG_BCK_MUL == 2
    temp_clock |= 0x00020000;
#elif BSP_CFG_BCK_MUL == 1
    temp_clock |= 0x00030000;
#else
    #error "Error! Invalid setting for BSP_CFG_BCK_MUL in r_bsp_config.h"
#endif

    /* Figure out setting for BCK bits. */
#if   BSP_CFG_PCK_MUL == 8
    /* Do nothing since BCK bits should be 0. */
#elif BSP_CFG_PCK_MUL == 4
    temp_clock |= 0x00000100;
#elif BSP_CFG_PCK_MUL == 2
    temp_clock |= 0x00000200;
#elif BSP_CFG_PCK_MUL == 1
    temp_clock |= 0x00000300;
#else
    #error "Error! Invalid setting for BSP_CFG_PCK_MUL in r_bsp_config.h"
#endif

    /* Configure PSTOP1 bit for BCLK output. */
#if BSP_CFG_BCLK_OUTPUT == 0    
    /* Set PSTOP1 bit */
    temp_clock |= 0x00800000;
#elif BSP_CFG_BCLK_OUTPUT == 1
    /* Clear PSTOP1 bit */
    temp_clock &= ~0x00800000;
#elif BSP_CFG_BCLK_OUTPUT == 2
    /* Clear PSTOP1 bit */
    temp_clock &= ~0x00800000;
    /* Set BCLK divider bit */
    SYSTEM.BCKCR.BIT.BCLKDIV = 1;
#else
    #error "Error! Invalid setting for BSP_CFG_BCLK_OUTPUT in r_bsp_config.h"
#endif

    /* Configure PSTOP0 bit for SDCLK output. */
#if BSP_CFG_SDCLK_OUTPUT == 0    
    /* Set PSTOP0 bit */
    temp_clock |= 0x00400000;
#elif BSP_CFG_SDCLK_OUTPUT == 1
    /* Clear PSTOP0 bit */
    temp_clock &= ~0x00400000;
#else
    #error "Error! Invalid setting for BSP_CFG_SDCLK_OUTPUT in r_bsp_config.h"
#endif

    SYSTEM.SCKCR.LONG = temp_clock;
}

/***********************************************************************************************************************
* Function name: output_ports_configure
* Description  : Configures the port and pin direction settings, and sets the pin outputs to a safe level.
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void output_ports_configure(void)
{
    /* Enable LEDs. */
    /* Start with LEDs off. */
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;

    /* Set LED pins as outputs. */
    LED0_DDR = 1;
    LED1_DDR = 1;
    LED2_DDR = 1;
    LED3_DDR = 1;

    /* Enable switches. */
    /* Set pins as inputs. */
    SW1_DDR = 0;
    SW2_DDR = 0;
    SW3_DDR = 0;

    /* Set Input Buffer Control registers for switches. */
    SW1_ICR = 1;
    SW2_ICR = 1;
    SW3_ICR = 1;

    /* Setup pins for on-board SPI flash. */
    /* Choose A pin group and enable RSPIB pins */    
    IOPORT.PFHSPI.BYTE = 0x0E;

    /* MISOB-A */
    PORT3.DDR.BIT.B0 = 0;
    PORT3.ICR.BIT.B0 = 1;
    /* SSLB0-A */
    PORT3.DR.BIT.B1  = 1;
    PORT3.DDR.BIT.B1 = 1;
    /* RSPCKB-A */
    PORT2.DR.BIT.B7  = 1;
    PORT2.DDR.BIT.B7 = 1;
    /* MOSIB-A */
    PORT2.DR.BIT.B6  = 1;
    PORT2.DDR.BIT.B6 = 1;

    /* Setup pins for on-board I2C EEPROM. */
    /* SDA0 */
    PORT1.DDR.BIT.B3 = 0;
    PORT1.ICR.BIT.B3 = 1;
    /* SCL0 */
    PORT1.DDR.BIT.B2 = 0;
    PORT1.ICR.BIT.B2 = 1;
    
    /* Configure the pin connected to the ADC Pot as an input */
    PORT4.DDR.BIT.B0 = 0;
}

/***********************************************************************************************************************
* Function name: interrupts_configure
* Description  : Configures interrupts used
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void interrupts_configure(void)
{
    /* Add code here to setup additional interrupts */
}

/***********************************************************************************************************************
* Function name: peripheral_modules_enable
* Description  : Enables and configures peripheral devices on the MCU
* Arguments    : none
* Return value : none
***********************************************************************************************************************/
static void peripheral_modules_enable(void)
{
    /* Add code here to enable peripherals used by the application */
}

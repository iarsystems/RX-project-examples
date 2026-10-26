/* Adapted to IAR Embedded Workbench */
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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : hwsetup.c
 * Device(s)    : RX
 * H/W Platform : RPBRX71M
 * Description  : Defines the initialization routines used each time the MCU is restarted.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 07.03.2016 0.01     First Release
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/
/* I/O Register and board definitions */
#include "platform.h"

/***********************************************************************************************************************
 Private global variables and functions
 ***********************************************************************************************************************/
/* MCU I/O port configuration function declaration */
static void output_ports_configure (void);

/* Interrupt configuration function declaration */
static void interrupts_configure (void);

/* MCU peripheral module configuration function declaration */
static void peripheral_modules_enable (void);

/* LCD setup - backlight and data bus setup */
static void lcd_init (void);

/* Setup MAX3355 */
static void max3355_init (void);

/* Setup ECC RAM */
static void ecc_ram_init (void);

/* Data Transfer Controller Setup */
static void dtc_init (void);

/* Serial SPI flash initialisation */
static void serial_flash_init (void);

#define WAIT_STATE_VALUE        0x08080000

/***********************************************************************************************************************
 * Function name: hardware_setup
 * Description  : Contains setup functions called at device restart
 * Arguments    : none
 * Return value : none
 ***********************************************************************************************************************/
void hardware_setup (void)
{
    output_ports_configure();
    interrupts_configure();
    peripheral_modules_enable();
    ecc_ram_init();
    lcd_init();
    max3355_init();
    dtc_init();
    serial_flash_init();
    bsp_non_existent_port_init();
}

/***********************************************************************************************************************
 * Function name: output_ports_configure
 * Description  : Configures the port and pin direction settings, and sets the pin outputs to a safe level.
 * Arguments    : none
 * Return value : none
 ***********************************************************************************************************************/
static void output_ports_configure (void)
{
    /* Enable LED. */
    /* Start with LEDs off. */
    LED0 = LED_OFF;
    LED1 = LED_OFF;

    /* Set LED pins as outputs. */
    LED0_PDR = 1;
    LED1_PDR = 1;

    /* Enable switches. */
    /* Set pins as inputs. */
    SW1_PDR = 0;

    /* Set port mode registers for switches. */
    SW1_PMR = 0;

    /* Set to high drive on all I/O */
    PORT0.DSCR.BYTE = 0xAF;
    PORT2.DSCR.BYTE = 0xFF;
    PORT5.DSCR.BYTE = 0xFF;
    PORT9.DSCR.BYTE = 0xFF;
    PORTA.DSCR.BYTE = 0xFF;
    PORTB.DSCR.BYTE = 0xFF;
    PORTC.DSCR.BYTE = 0xFF;
    PORTD.DSCR.BYTE = 0xFF;
    PORTE.DSCR.BYTE = 0xFF;
    PORTG.DSCR.BYTE = 0xFF;

}

/***********************************************************************************************************************
 * Function name: interrupts_configure
 * Description  : Configures interrupts used
 * Arguments    : none
 * Return value : none
 ***********************************************************************************************************************/
static void interrupts_configure (void)
{
    /* Set Group AL0/BL0/BL1 priority level */
    IPR(ICU,GROUPBL0)= 0x0F;
    IPR(ICU,GROUPBL1) = 0x0F;

    /* Enable group AL0/BL0/BL1 interrupt */
    IEN(ICU,GROUPBL0) = 1U;
    IEN(ICU,GROUPBL1) = 1U;

    /* Add code here to setup additional interrupts */
}

/***********************************************************************************************************************
 * Function name: peripheral_modules_enable
 * Description  : Enables and configures peripheral devices on the MCU
 * Arguments    : none
 * Return value : none
 ***********************************************************************************************************************/
static void peripheral_modules_enable (void)
{
    /* Protect off for MSTP access */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Enable hardware as necessary */
    MSTP(DTC) = 0; /* enable DTC */
    MSTP(EXDMAC) = 0; /* enable ExDMA */
    MSTP(MTU0) = 0; /* enable TPU 0-5 */
    MSTP(CRC) = 0; /* enable CRC 0-5 */
    MSTP(RIIC2) = 0; /* Enable RIIC2 module */
    MSTP(RSPI0) = 0; /* Enable RSPI0 module */
    MSTP(S12AD) = 0; /* Enable ADC */
    MSTP(ECCRAM) = 0; /* Enable ECC RAM */

    MTU.TOERA.BYTE = 0xFF; /* turn on all OE based outputs */

    /* Protect on */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Add code here to enable peripherals used by the application */
}

/*******************************************************************************
 * Function name: lcd_init
 * Description  : Setup peripherals and pins for LCD drive
 * Arguments    : none
 * Return value : none
 *******************************************************************************/
static void lcd_init (void)
{
    /* Unlock MPC registers to enable writing to them. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Protect off for MSTP access */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Start with LCD backlight on */
    LCD_BACKLIGHT = 1;
    LCD_BACKLIGHT_PDR = 1;

    /* Turn on external data bus - used for Display */
    SYSTEM.SYSCR0.WORD = 0x5A03; /* enable the external bus...must also set key and internal ROME bit */
    MPC.PFBCR0.BYTE = 0x10; /* Enable PE[0..7] as D8-15 */

    BSC.CS3CR.BIT.EXENB = 1; /* Enable CS3 space, used for LCD data bus access */
    BSC.CS3CR.BIT.EMODE = 1; /* Endian is not the same as the operating mode, used for LCD data bus access */

#ifdef LCD_16BIT
    BSC.CS3CR.BIT.BSIZE = 0; /* 16-bit external access */
#else
    BSC.CS3CR.BIT.BSIZE = 2; /* 8-bit external access */
#endif /* LCD_16BIT */

    BSC.CS3WCR1.LONG = WAIT_STATE_VALUE; /* set the wait states */
    BSC.CS3WCR2.LONG = 0;

    MPC.PFBCR1.BYTE = 0x00; /* Disable SDRAM pins */

    /* Protect on */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Lock MPC registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);

}
/*******************************************************************************
 End of function lcd_init
 *******************************************************************************/

/*******************************************************************************
 * Function name: max3355_init
 * Description  : Enables the MAX3355 voltage generator when ID pin shows
 * 				 a slave device is connected. Used for production testing.
 *                Note: Further enhancement in this section is to initialise the
 *                input pins.
 *                This code only currently supports operation as a USB slave.
 * Arguments    : none
 * Return value : none
 *******************************************************************************/
static void max3355_init (void)
{
    /* Assert USB0_VBUSED (OFFVBUS) */
    MAX3355_VEN = 1;
    MAX3355_VEN_PDR = 1;

    /* Disable Shutdown mode */
    MAX3355_EXIC = 1;
    MAX3355_EXIC_PDR = 1;
}
/*******************************************************************************
 End of function max3355_init
 *******************************************************************************/

/*******************************************************************************
 * Function name: ecc_ram_init
 * Description  : Setup ECC RAM.
 * Arguments    : none
 * Return value : none
 *******************************************************************************/
static void ecc_ram_init (void)
{
    /* Unlock MPC registers to enable writing to them. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Protect off for MSTP access */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    SYSTEM.SYSCR1.BIT.ECCRAME = 1; /* Enable ECC RAM */

    /* Protect on */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Lock MPC registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);

}
/*******************************************************************************
 End of function ecc_ram_init
 *******************************************************************************/

/*******************************************************************************
 * Function name: dtc_init
 * Description  : Setup DTC, ready for touch screen ADC captures
 * Arguments    : none
 * Return value : none
 *******************************************************************************/
#pragma section=".DTC_VECT"
static void dtc_init (void)
{
    /* Unlock MPC registers to enable writing to them. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Protect off for MSTP access */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* enable DTC */
    DTC.DTCVBR = __section_begin(".DTC_VECT");
    DTC.DTCCR.BYTE = 0;
    DTC.DTCADMOD.BYTE = 0;
    DTC.DTCST.BYTE = 1;

    /* Protect on */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Lock MPC registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);

}
/*******************************************************************************
 End of function dtc_init
 *******************************************************************************/

/***********************************************************************************************************************
 * Function name: serial_flash_init
 * Description  : Sets up hardware for access to the serial flash
 * Arguments    : none
 * Return value : none
 ***********************************************************************************************************************/
static void serial_flash_init (void)
{
    /* Unlock MPC registers to enable writing to them. */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC);

    /* Protect off for MSTP access */
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Connectivity for W25Q32VSSIG */
    /*   CSZ PC.4    GPIO
     CLK PC.5    SCK8
     DI_IO1 PC.6    RXD8
     DO_IO1 PC.7    TXD8
     */

    /* Start with LCD backlight on */
    SPI_CS = 1; /* Start with CS high */
    SPI_CS_PMR = 1; /* Set to general IO*/
    SPI_CS_PDR = 1; /* Set to output */

    /* MISO is an input */
    PORTC.PMR.BIT.B7 = 0; /* General I/O */
    PORTC.PDR.BIT.B7 = 0; /* Make it an input */
    MPC.PC7PFS.BIT.PSEL = 0xD; /* configure for RSPI */
    PORTC.PMR.BIT.B7 = 1; /* Enable peripheral */

    /* MOSI is an output */
    PORTC.PMR.BIT.B6 = 0; /* General I/O */
    PORTC.PDR.BIT.B6 = 1; /* Make it an output */
    MPC.PC6PFS.BIT.PSEL = 0xD; /* configure for RSPI */
    PORTC.PMR.BIT.B6 = 1; /* Enable peripheral */

    /* SCK is an output, idles high */
    PORTC.PMR.BIT.B5 = 0; /* General I/O */
    PORTC.PDR.BIT.B5 = 1; /* Make it an output */
    MPC.PC5PFS.BIT.PSEL = 0xD; /* configure for RSPI */
    PORTC.PMR.BIT.B5 = 1; /* Enable peripheral */

    /* Protect on */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_CGC_SWR);

    /* Lock MPC registers. */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_MPC);
}
/*******************************************************************************
 End of function serial_flash_init
 *******************************************************************************/

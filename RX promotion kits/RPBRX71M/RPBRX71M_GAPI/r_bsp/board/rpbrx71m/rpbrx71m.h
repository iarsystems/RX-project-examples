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
 * File Name    : rpbrx71m.h
 * H/W Platform : RPBRX71M
 * Description  : Basic hardware platform definitions/
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 07.03.2016 1.00     First Release
 ***********************************************************************************************************************/

#ifndef RPBRX71M_H
    #define RPBRX71M_H

/* Local defines */
    #define LED_ON              (1)
    #define LED_OFF             (0)
    #define SET_BIT_HIGH        (1)
    #define SET_BIT_LOW         (0)
    #define SET_BYTE_HIGH       (0xFF)
    #define SET_BYTE_LOW        (0x00)

/* Switches */
    #define SW_ACTIVE           0
    #define SW1                 PORT1.PIDR.BIT.B2
    #define SW1_PDR             PORT1.PDR.BIT.B2
    #define SW1_PMR             PORT1.PMR.BIT.B2

/* LEDs */
    #define LED0                PORTA.PODR.BIT.B0
    #define LED0_PDR            PORTA.PDR.BIT.B0

    #define LED1                PORTA.PODR.BIT.B1
    #define LED1_PDR            PORTA.PDR.BIT.B1

/* LCD Backlight */
    #define LCD_BACKLIGHT       PORTA.PODR.BIT.B5
    #define LCD_BACKLIGHT_PDR   PORTA.PDR.BIT.B5

/* LCD Reset Signal */
    #define LCD_RESET           PORTA.PODR.BIT.B7
    #define LCD_RESET_PDR       PORTA.PDR.BIT.B7

/* SPI Flash Slave select. */
    #define SPI_CS        	    PORTC.PODR.BIT.B4
    #define SPI_CS_PMR			PORTC.PMR.BIT.B4
    #define SPI_CS_PDR			PORTC.PDR.BIT.B4

/* Serial Flash Access */
    #define SERIAL_FLASH_PAGE_SIZE (0x1)
    #define SERIAL_FLASH_RSPI_CHANNEL 0

/* MAX3355 USB OTG Charge Pump */
    #define MAX3355_VEN 	    PORT2.PODR.BIT.B4
    #define MAX3355_VEN_PMR		PORT2.PMR.BIT.B4
    #define MAX3355_VEN_PDR		PORT2.PDR.BIT.B4

    #define MAX3355_EXIC 	    PORT2.PODR.BIT.B1
    #define MAX3355_EXIC_PMR	PORT2.PMR.BIT.B1
    #define MAX3355_EXIC_PDR	PORT2.PDR.BIT.B1

/* Touch */
    #define OUT_TS_X            PORT4.PODR.BIT.B4
    #define OUT_TS_Y            PORT4.PODR.BIT.B5
    #define RES_TOUCH           /* Using resistive touch panel */

/* I/O Port registers associated with touch */
/* Touch port pins - Port Output Data Registers */
    #define TOUCH_PORTO_YU		PORT4.PODR.BIT.B3
    #define TOUCH_PORTO_YL		PORT4.PODR.BIT.B1
    #define TOUCH_PORTO_XL		PORT4.PODR.BIT.B2
    #define TOUCH_PORTO_XR		PORT4.PODR.BIT.B0

/* Touch port pins - Port Input Data Registers */
    #define TOUCH_PORTI_YU		PORT4.PIDR.BIT.B3
    #define TOUCH_PORTI_YL		PORT4.PIDR.BIT.B1
    #define TOUCH_PORTI_XL		PORT4.PIDR.BIT.B2
    #define TOUCH_PORTI_XR		PORT4.PIDR.BIT.B0

/* Touch port pins - Port Direction Registers */
    #define TOUCH_PORT_YU_PDR	PORT4.PDR.BIT.B3
    #define TOUCH_PORT_YL_PDR	PORT4.PDR.BIT.B1
    #define TOUCH_PORT_XL_PDR	PORT4.PDR.BIT.B2
    #define TOUCH_PORT_XR_PDR	PORT4.PDR.BIT.B0

/* Touch port pins - Port Pullup Control Registers */
    #define TOUCH_PORT_YU_PUP	PORT4.PCR.BIT.B3
    #define TOUCH_PORT_YL_PUP	PORT4.PCR.BIT.B1
    #define TOUCH_PORT_XL_PUP	PORT4.PCR.BIT.B2
    #define TOUCH_PORT_XR_PUP	PORT4.PCR.BIT.B0

/* LCD Panel Colour Depth - select just one of the following depending
 * on the LCD connectivity. */
    #define LCD_16BIT
/* #define LCD_8BIT */

/* Touchscreen hardware connections */
    #define OUT_TS_XL_PORT  PORT4    /* driver port/pin for terminal resistor at left of screen */
    #define OUT_TS_XL_PIN   4
    #define OUT_TS_XR_PORT  PORT4    /* driver port/pin for terminal resistor at right of screen */
    #define OUT_TS_XR_PIN   4
    #define OUT_TS_YL_PORT  PORT4    /* driver port/pin for terminal resistor at bottom of screen */
    #define OUT_TS_YL_PIN   5
    #define OUT_TS_YU_PORT  PORT4    /* driver port/pin for terminal resistor at top of screen */
    #define OUT_TS_YU_PIN   5

    #define ADC_TS_XL (2)     /* channel connected to terminal resistor at left of screen */
    #define ADC_TS_XR (0)     /* channel connected to terminal resistor at right of screen */
    #define ADC_TS_YL (1)     /* channel connected to terminal resistor at bottom of screen */
    #define ADC_TS_YU (3)     /* channel connected to terminal resistor at top of screen */

#endif /* RPBRX71M_H */

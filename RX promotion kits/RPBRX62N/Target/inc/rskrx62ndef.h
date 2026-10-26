/* Adapted for use with IAR Embedded Workbench */

/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
  Copyright (C) 2008. Renesas Technology Corp., All Rights Reserved.
*******************************************************************************
* File Name    : rsksh7216def.h
* Version      : 1.00
* Description  : RSK 7216 board specific settings
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.10.2009 1.00    First Release
******************************************************************************/

#ifndef RSKRX62N_H
#define RSKRX62N_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/

/* System Clock Settings */
#define     INPUTCLK        (12000000)
#define     ICLK            (INPUTCLK * 8)
#define     PCLK            (INPUTCLK * 4)
#define     BCLK            (INPUTCLK * 2)

#define     CMT0_CLK_SELECT (512)

/* General Values */
#define    LED_ON      (0)
#define   LED_OFF      (1)
#define   SET_BIT_HIGH  (1)
#define   SET_BIT_LOW    (0)
#define   SET_BYTE_HIGH  (0xFF)
#define   SET_BYTE_LOW  (0x00)

#define    HID_ON      (1)
#define   HID_OFF      (0)

/* Define switches to be polled if not available as interrupts */
#define    SW_ACTIVE    FALSE
#define   SW1       PORT0.DR.BIT.B0
#define   SW2       PORT0.DR.BIT.B1
#define     SW3             PORT0.DR.BIT.B7

/* LEDs */
#define    LED0      PORTA.DR.BIT.B2
#define    LED1      PORTA.DR.BIT.B1
#define    LED2      PORTA.DR.BIT.B0

/******************************************************************************
Variable Externs
******************************************************************************/

/******************************************************************************
Functions Prototypes
******************************************************************************/



/* RSKRX62N_H */
#endif    


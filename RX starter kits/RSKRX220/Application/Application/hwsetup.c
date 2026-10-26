/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : hwsetup.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines the hardware initialisation routines used each time 
*                 the MCU is restarted. 
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/

/* Includes assembly level definitions */
#include <machine.h>

/* Following header file provides C99 type definitions. */
#include <stdint.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Switch handler function definitions */
#include "switch.h"

/* Contains declarations for the functions defined in this file */
#include "hwsetup.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/* MCU clock configuration function declaration */
static void configure_operating_frequency (void);

/* MCU I/O port configuration function declaration */
static void configure_output_ports (void);

/* Interrupt configuration function declaration */
static void configure_interrupts (void);

/* MCU peripheral module configuration function declaration */
static void enable_peripheral_modules (void);

/*******************************************************************************
* Function Name: hardware_setup
* Description  : Contains all the setup functions called at device restart
* Arguments    : none
* Return Value : none
*******************************************************************************/
void hardware_setup (void)
{
    configure_operating_frequency();
    configure_output_ports();
    configure_interrupts();
    enable_peripheral_modules();
}
/*******************************************************************************
* End of function hardware_setup
*******************************************************************************/

/*******************************************************************************
* Function Name: configure_operating_frequency
* Description  : Configures the clock settings for each of the device clocks
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void configure_operating_frequency (void)
{
    /* Declare and initialise a loop count variable */
    volatile uint16_t i = 0;
        
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;          
    
    /* Stop sub-clock */
    SYSTEM.SOSCCR.BYTE = 0x01u;          
                                        
    /* Specify a wait state greater than 10ms at 20MHz (13.1 ms) */ 
    SYSTEM.MOSCWTCR.BYTE = 0x0Eu;        
            
    /* Configure the EXTAL pin */
    PORT3.PDR.BIT.B6 = 0x0;

    /* Configure the XTAL pin */
    PORT3.PDR.BIT.B7 = 0x0;       

    /* Turn on EXTAL */
    SYSTEM.MOSCCR.BYTE = 0x00;  
               
    /* Wait over 12ms */
    for (i = 0; i < 2075u; i++)
    {
        /* Do nothing during delay */
        nop();
    }
        
    /* Configure the clocks as follows -
    Clock Description              Frequency
    ----------------------------------------
    FlashIF Clock......................20MHz
    System Clock.......................20MHz
    External Bus Clock.................20MHz    
    Peripheral Clock B.................20MHz
    Peripheral Clock D.................20MHz */             
    SYSTEM.SCKCR.LONG = 0x00001010u;     
       
    /* Set the clock source to main oscillator */
    SYSTEM.SCKCR3.BIT.CKSEL = 0x2u;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;      
}
/*******************************************************************************
* End of function configure_operating_frequency
*******************************************************************************/

/*******************************************************************************
* Function Name: configure_output_ports
* Description  : Configures the port and pin direction settings, and sets the
*                pin outputs to a safe level.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void configure_output_ports (void)
{
    /* Configure the LED pins (LED0-LED3) as outputs */
    LED0_PORT_DIR = 0x1u;
    LED1_PORT_DIR = 0x1u;
    LED2_PORT_DIR = 0x1u;
    LED3_PORT_DIR = 0x1u;

    /* Set LED pin outputs as high (all LEDs off) */
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;
    
    /* Configure the debug LCD data pins (DLCD7-DLCD4) as outputs */
    LCD_DATA_DIR_PORT = DATA_PORT_MASK;
            
    /* Set debug LCD data pin (DLCD7-DLCD4) outputs as low */
    LCD_DATA_PORT = 0x00;
    
    /* Configure the debug LCD control pin (DLCDRS) and enable pin (DLCDE)
       as output */
    RS_PIN_DIR = 0x1u;
    E_PIN_DIR  = 0x1u;
            
    /* Set the debug LCD control pin (DLCDRS) and enable pin (DLCDE) as output
       low */
    RS_PIN = 0x0;
    E_PIN  = 0x0;   
}
/*******************************************************************************
* End of function configure_output_ports
*******************************************************************************/

/*******************************************************************************
* Function Name: configure_interrupts
* Description  : Configures the interrupts used
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void configure_interrupts (void)
{
    /* Configure switch interrupts */
    initialise_switch_interrupts();
}
/*******************************************************************************
* End of function configure_interrupts
*******************************************************************************/

/*******************************************************************************
* Function Name: enable_peripheral_modules
* Description  : Enables and configures peripheral devices on the MCU
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void enable_peripheral_modules (void)
{
    /* Peripherals initialised in specific initialisation functions */
}
/*******************************************************************************
* End of function enable_peripheral_modules
*******************************************************************************/


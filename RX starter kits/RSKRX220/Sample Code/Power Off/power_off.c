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
* File Name     : power_off.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines power off functions.
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/

/* Defines machine level functions used in this file */
#include <machine.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/

/* Declares functions defined in this file */
#include "power_off.h"

/* Defines switch functions used in this file */
#include "switch.h"

/* Defines device registers used in this file */
#include <iorx220.h>

/* Defines device LEDs and switches */
#include "rskrx220def.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/*******************************************************************************
* Global Variables 
*******************************************************************************/

/* Switches ready to enter standby ready flag variable (imported) */
extern volatile bool g_switch_standby_ready;

/* Switch status flag variable (imported) */
extern volatile uint8_t g_switch_flag;

/* Prepare to enter standby flag variable */
volatile bool gEnterStandby = false;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/* Switch callback function prototype declaration */
static void switchcb_poweroff (void);

/******************************************************************************
* Function Name: init_power_off
* Description  : Function sets the switch callback function.
* Arguments    : none
* Return Value : none
******************************************************************************/
void init_power_off (void)
{
    /* Set the switch release callback function to switchcb_poweroff */
    set_switch_release_callback(switchcb_poweroff);
}
/******************************************************************************
* End of function init_power_off
******************************************************************************/ 

/******************************************************************************
* Function Name: switchcb_poweroff
* Description  : Switch release callback function, which is executed by the
*                switch IRQ ISR. Function clears the standby flag on switches
*                2 or 3 being released, and toggles it on switch 1. Also 
*                clears the switch flag.
* Arguments    : none
* Return Value : none
******************************************************************************/
static void switchcb_poweroff (void)
{
    /* Check if the switch pressed was switch 1 */
    if (SWITCHPRESS_1 == g_switch_flag)
    {
        /* Toggle the enter standby flag */
        gEnterStandby ^= 0x1u;
    }
    else
    {
        /* Set the enter standby flag false */
        gEnterStandby = false;
    }
    
    /* Clear the switch press flag */
    g_switch_flag &= 0x0Fu;
}
/******************************************************************************
* End of function switchcb_poweroff
******************************************************************************/ 

/******************************************************************************
* Function Name: enter_standby_power_off
* Description  : This function configures the MCU to run from the internal 
*                HOCO, and switches the MCU into standy mode. When the MCU
*                wakes from standby, it returns to using the main clock.
* Arguments    : none
* Return Value : none
******************************************************************************/
void enter_standby_power_off (void)
{
    /* Turn user LEDs 0 to 2 */
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    
    /* Turn on LED3 to indicate program is waiting for user to release all
       switches */
    LED3 = LED_ON;
    
    /* Wait for switches to be ready to enter standby */
    while (!g_switch_standby_ready)
    {
        /* Do nothing until switches are ready */
    }
    
    /* Turn off LED3 */
    LED3 = LED_OFF;
    
    /* Display MCU mode as 'Standby' on the debug LCD */
    Display_LCD(LCD_LINE2, " Standby");
   
    /* Disable register protection */
    SYSTEM.PRCR.WORD = PROTECTOFF;  
            
    /* Start oscillating the HOCO. This is required as the MCU cannot
       resume from standby using the main crystal oscillator. */
    SYSTEM.HOCOCR.BIT.HCSTP = 0x0;
    
    /* Wait for the HOCO to stabilise */
    uint16_t delay = CLK_DELAY;
    while (delay--)
    {
        /* Do nothing until oscillation is stable */
    }
    
    /* Switch system clock to HOCO */
    SYSTEM.SCKCR3.BIT.CKSEL = 0x1u;
        
    /* Wait for the HOCO to stabilise */
    delay = CLK_DELAY;
    while (delay--)
    {
        /* Do nothing until clock is ready */
    }
  
    /* Set SSBY bit to enable software standby after executing wait() */
    SYSTEM.SBYCR.BIT.SSBY = 0x1u;
    
    /* Wait until SSBY bit has been written successfully */
    while (!SYSTEM.SBYCR.BIT.SSBY)
    {
        /* Do nothing until standby bit is set */
    }
    
    /* Re-enable register protection */
    SYSTEM.PRCR.WORD = PROTECTON; 

    /* Execute break instruction (which in turn excutes the wait instruction)
       This enters the MCU into sleep mode. */
    brk();
    
    /* The MCU will return here when it wakes from sleep mode */
    
    /* Disable register protection */
    SYSTEM.PRCR.WORD = PROTECTOFF;  
  
    /* Switch system clock to main clock */
    SYSTEM.SCKCR3.BIT.CKSEL = 0x2u;
    
    /* Re-enable register protection */
    SYSTEM.PRCR.WORD = PROTECTON;  
    
    /* Wait for clock to stabilise */
    delay = CLK_DELAY;
    while (delay--)
    {
        /* Do nothing until clock is stable */
    }
 
    /* Turn on all the user LEDs */
    LED0 = LED_ON;
    LED1 = LED_ON;
    LED2 = LED_ON;
    LED3 = LED_ON;
    
    /* Re-initialise the debug LCD */
    Init_LCD();
    
    /* Display current MCU mode on the debug LCD */
    Display_LCD(LCD_LINE1, "PowerOff");
    Display_LCD(LCD_LINE2, " Active ");
}
/******************************************************************************
* End of function enter_standby_power_off
******************************************************************************/ 

/******************************************************************************
* Function Name: flash_leds_power_off
* Description  : Function toggles the LEDs and waits in a delay while loop. 
*                When the function is called repeatedly in a while(1) loop, it
*                visibly flashes the user LEDs.
* Arguments    : none
* Return Value : none
******************************************************************************/
void flash_leds_power_off (void)
{
    /* Toggle the user LEDs */
    LED0 = (uint8_t) ~LED0;
    LED1 = (uint8_t) ~LED1;
    LED2 = (uint8_t) ~LED2;
    LED3 = (uint8_t) ~LED3;
        
    /* Create a time delay using a while loop */
    uint32_t delay = 0xFFFFFu;
    while (delay--)
    {
        /* Delay to make LED flash visible */
    }
}
/******************************************************************************
* End of function flash_leds_power_off
******************************************************************************/ 


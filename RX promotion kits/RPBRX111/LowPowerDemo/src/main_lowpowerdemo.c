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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : main.c
* Version       : 1.00
* Device        : R5F51115AxFM
* Tool Chain    : CCRX
* H/W Platform  : RPBRX111
* Description   : Main file
*******************************************************************************/
/*******************************************************************************
* History       : 12/08/2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Include machine.h to use the nop() function */
#include    <machine.h>

/* Standard integer types with specified widths definitions */
#include    <stdint.h>     

/* Register address definitions */
#include <iorx111.h>

/* Hardware platform definitions */
#include    "rsk_defines.h"

/* Analog to digital and user interface function prototypes */
#include    "s12ad.h"

/* Clock Mode setup function prototypes */
#include    "setupclocks.h"

/* RAM based CPU exercise function prototypes */
#include    "powertests.h"

/* Prototypes for functions in this file */
#include    "main_lowpowerdemo.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* For use in demonstrating CPU load and operation of DMA transfer */
uint32_t  g_destination[8];

/*******************************************************************************
* Function Name : main
* Description   : The main program function. This provides user interface for
*                 the selection of operation modes by adjusting the
*                 potentiometer and selecting with SW1. It then calls functions
*                 to configure the selected operation and clock modes before
*                 transferring execution to the ram-based PerformPowerTests
*                 function.
* Argument      : none
* Return value  : none
*******************************************************************************/
void main (void)
{
    /* Variable to store the selected CPU load setting */
    uint8_t  cpu_calc;

    /* Variable to store the selected Clock Mode setting */
    uint8_t  clock_mode;
    
    /* Initialise hardware */
    initialise();

    /* Enter loop to present main menu selection until an
       option other than RTC_MENU is chosen. This allows for
       setting RTC options then returning to main menu to select
       clock mode */
    do
    {
        /* Display LED welcome pattern then wait for user input
        using potentiometer and SW1 for "Main menu" selection:
        +====================================================+
        |        MAIN MENU OPTIONS USER INTERFACE MENU       |
        +====================================================+
        | Adjust potentiometer RV1 then press SW1 to select. |
        | Corresponding value will be displayed on LEDs      |
        | using binary.                                      |
        |                    (msb)  LED2  LED1  LED0 (lsb)   |
        |   (0) RTC_MENU             0     0     0           |
        |   (1) SUB_32k              0     0     1           |
        |   (2) LOCO_4M              0     1     0           |
        |   (3) HOCO_32M             0     1     1           |
        |   (4) SW_SLEEP             1     0     0           |
        |   (5) DP_SLEEP             1     0     1           |
        |   (6) SW_STBY              1     1     0           |
        +---------------------------------------------------*/
        clock_mode = welcome_menu();
        
        /* If RTC_MENU option is chosen then
           Collect user input for RTC options */
        if (RTC_MENU == clock_mode)
        {
            /* Wait for user input using potentiometer and SW1
               for RTC options selection:
            +====================================================+
            |            RTC OPTIONS USER INTERFACE MENU         |
            +====================================================+
            | Adjust potentiometer RV1 then press SW1 to select. |
            |                    (msb)  LED2  LED1  LED0 (lsb)   |
            |   (1) Output RTC           0     0     1           |
            |   (2) Reset RTC            0     1     0           |
            |   (3) Start RTC            0     1     1           |
            |   (4) Stop RTC             1     0     0           |
            +---------------------------------------------------*/
            rtc_menu();
        }
    }
    while (RTC_MENU == clock_mode);
       
   
    /*===================================================+
    |          LED0 FLASH USER INTERFACE                 |
    +====================================================+
    | Flash LED0, Clock Mode times to confirm selection  |
    +---------------------------------------------------*/
    flash_led_value(LED0MASK, clock_mode);

    /* Wait for user input using potentiometer and SW1 
       for CPU Load options selection:
    +====================================================+
    |           CPU LOAD USER INTERFACE MENU             |
    +====================================================+
    | Adjust potentiometer RV1 then press SW1 to select. |
    |                    (lsb)  LED0  LED1  LED2 (msb)   |
    |   (1) CPU_MIN              1     0     0           |
    |   (2) CPU_TYP              0     1     0           |
    |   (3) CPU_MAX              1     1     0           |
    +---------------------------------------------------*/
    cpu_calc = cpu_load_menu();
        
    /*===================================================+
    |            LED1 FLASH USER INTERFACE               |
    +====================================================+
    | Flash LED1, cpu_calc times to confirm selection     |
    +---------------------------------------------------*/
    flash_led_value(LED1MASK, cpu_calc);

    /* Configure the peripherals for the selected Clock Mode */
    configure_peripherals(cpu_calc);
    
    /* Set the clock mode */
    set_clock_mode(clock_mode, cpu_calc);

    /* Setup the power tests according to clock_mode */
    configure_power_tests(clock_mode);
    
    /* Call code that exercises the CPU so current measurements
       can be taken. Goes into full power mode then awaits SW1
       press to enter selected low power mode:
    +====================================================+
    |            POWER TEST USER INTERFACE               |
    +====================================================+
    | Now measure high speed power of selected mode      |
    | Press SW1 to enter low power mode                  |
    | Then measure for low power mode                    |
    | Press RES1 to reset                                |
    +---------------------------------------------------*/
    perform_power_tests(clock_mode, cpu_calc);

    /* perform_power_tests should never return */
    while (1)
    {

    }
}
/******************************************************************************
* End of function main
******************************************************************************/

/******************************************************************************
* Function Name : initialise
* Description   : Sets port directions, internal pull ups etc
* Argument      : none
* Return value  : none
******************************************************************************/
void initialise (void)
{
    /* Enable access to System Registers */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Configure power rails */
    IDLE_EN = POWER_ON;
    POT_EN = POWER_ON;

    /* Configure the power control pins */
    IDLE_EN_PDR = 0x1u;
    CHG_EN_ODR = 0x1u;

    /* Pull up the power mode pins */
    PORT3.PCR.BYTE = 0x07u;

    /* Set the unused port drivers */
    PORT0.PDR.BYTE |= 0x28u;
    PORT1.PDR.BYTE |= 0xa0u;
    PORT2.PDR.BYTE |= 0xc0u;
    PORT4.PDR.BYTE |= 0x5fu;
    PORT5.PDR.BYTE |= 0x10u;
    PORTA.PDR.BYTE |= 0x5au;
    PORTB.PDR.BYTE |= 0xe9u;
    PORTC.PDR.BYTE |= 0xfcu;
    PORTE.PDR.BYTE |= 0xf7u;

    /* Set LED Port Direction Registers */
    LED0_PDR = OUTPUT;
    LED1_PDR = OUTPUT;
    LED2_PDR = OUTPUT;
    LED3_PDR = OUTPUT;

    /* LED3 not used, switch it off */
    LED3 = LED_OFF;

    /* Gain access to the Port Function Select Registers */
    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;

    /* Initialise the A2D */
    init_s12ad();

}
/******************************************************************************
* End of function initialise
******************************************************************************/

/******************************************************************************
* Function Name : welcome_menu
* Description   : Allows user selection of 0 to 6 by adjusting RV1.
*                 Corresponding value will be displayed on LEDs using binary.
*                                  (lsb)  LED2  LED1  LED0 (msb)
*                 (0) RTC_MENU             0     0     0
*                 (1) SUB_32k              0     0     1
*                 (2) LOCO_4M              0     1     0
*                 (3) HOCO_32M             0     1     1
*                 (4) SW_SLEEP             1     0     0
*                 (5) DP_SLEEP             1     0     1
*                 (6) SW_STBY              1     1     0
* Argument      : none
* Return value  : uint8_t selected menu item number [range 0 to 6]
******************************************************************************/
uint8_t welcome_menu (void)
{
    /* Repeat loop counter */
    volatile uint8_t repeat = 15;

    /* Uniquely Flash LEDs to indicate first menu */
    while (repeat--)
    {
        /* Delay loop */
        settle(650);

        /* Output pattern on LEDs */
        LED0 = (uint8_t) !((repeat) % 3);
        LED1 = (uint8_t) !((repeat + 1) % 3);
        LED2 = (uint8_t) !((repeat + 2) % 3);

        /* Delay loop */
        settle(650);
    };

    /* Await option selection from user to specify which clock mode
     * to demonstrate. The options are selected by using the 
     * potentiometer RV1 and accepted by pressing SW1.
     * The LEDs 0 1 2 count up in binary from 0 (all off) to 7 (all on).
     * Return the selected option value */
    return (get_user_input(0, 6));
}
/******************************************************************************
* End of function welcome_menu
******************************************************************************/

/******************************************************************************
* Function Name : cpu_load_menu
* Description   : Allows user selection of 1 to 3 by adjusting RV1.
*                 Corresponding value will be displayed on LEDs using binary.
*                                  (msb)  LED2  LED1  LED0 (lsb)
*                 (1) CPU_MIN              0     0     1
*                     (Stop all modules)
*                 (2) CPU_TYP              0     1     0
*                     (Enable all modules) 
*                 (3) CPU_MAX              0     1     1
*                     (Enable all modules, DMA, MTU & CMT Timers
* Argument      : none
* Return value  : uint8_t selected menu item number
******************************************************************************/
uint8_t cpu_load_menu (void)
{
     /* Await option selection from user to specify which CPU load
     * to demonstrate. The options are selected by using the 
     * potentiometer RV1 and accepted by pressing SW1.
     * There are 3 options:
     * (1) CPU_MIN
     * (2) CPU_TYP
     * (3) CPU_MAX
     * Return the selected option value */
    return (get_user_input(1, 3));
}
/******************************************************************************
* End of function cpu_load_menu
******************************************************************************/

/******************************************************************************
* Function Name : rtc_menu
* Description   : Allows selection of 1 to 4 by adjusting RV1.
*                 Corresponding value will be displayed on LEDs using binary.
*                                  (msb)  LED2  LED1  LED0 (lsb)
*                 (1) Output RTC           0     0     1
*                 (2) Reset RTC            0     1     0
*                 (3) Start RTC            0     1     1
*                 (4) Stop RTC             1     0     0
* Argument      : none
* Return value  : none
******************************************************************************/
void rtc_menu (void)
{
    /* Variable to store RTC Hours value */
    volatile uint8_t  hours;

    /* Variable to store RTC Minutes value */
    volatile uint8_t  mins;

    /* Variable to store RTC Seconds value */
    volatile uint8_t  secs;

    /* Variable for RTC Menu response */
    uint8_t rtc_menu_option;
    
     /* Await option selection from user to specify which CPU load
     * to demonstrate. The options are selected by using the 
     * potentiometer RV1 and accepted by pressing SW1.
     * There are 4 options:
     * (1) Output RTC
     * (2) Reset RTC
     * (3) Start RTC
     * (4) Stop RTC */
    rtc_menu_option = get_user_input(1, 4);

    /* Perform selected RTC Menu operation */
    switch (rtc_menu_option)
    {
        /* Code to output the RTC value to the LEDs */
        case OUTPUT_RTC:
        {
            /* Store Hours setting from RTC  */
            hours = (uint8_t) ((RTC.RHRCNT.BIT.HR10 * 10) +
                    (RTC.RHRCNT.BIT.HR1));

            /* Store Minutes setting from RTC  */
            mins = (uint8_t) ((RTC.RMINCNT.BIT.MIN10 * 10) +
                    (RTC.RMINCNT.BIT.MIN1));

            /* Store Seconds setting from RTC  */
            secs = (uint8_t) ((RTC.RSECCNT.BIT.SEC10  *10) +
                    (RTC.RSECCNT.BIT.SEC1));
        
            /* Flash LED0 Hours times, to indicate Hours */
            flash_led_value(LED0MASK, hours);

            /* Flash LED1 Mins times, to indicate Minutes */
            flash_led_value(LED1MASK, mins);
        
            /* Flash LED2 Secs times, to indicate Seconds */
            flash_led_value(LED2MASK, secs);
            break;
        }
            
        /* Code to reset the RTC value */
        case RESET_RTC:
        {
            /* Reset RTC Hours count */
            RTC.RHRCNT.BYTE = 0;

            /* Reset RTC Minutes count */
            RTC.RMINCNT.BYTE = 0;

            /* Reset RTC Seconds count */
            RTC.RSECCNT.BYTE = 0;
            break;    
        }
        
        /* Code to start the RTC */    
        case START_RTC:
        {
            /* Disable RTC Interrupts */
            RTC.RCR1.BYTE = 0;

            /* Enable 32-kHz clock, high drive */
            RTC.RCR3.BIT.RTCDV = 0x1;
            RTC.RCR3.BIT.RTCEN = 0x1;

            /* Allow RTC clock to settle */
            settle(5000);
    
            /* Configure and start the RTC */
            RTC.RCR2.BIT.START = 0x01;

            /* Enable RTC Interrupts */
            RTC.RCR1.BYTE = 1;
            break;
        }
            
        /* Code to stop the RTC */      
        case STOP_RTC:
        {
            /* Disable RTC Interrupts */
            RTC.RCR1.BYTE = 0;

            /* Stop RTC */
            RTC.RCR2.BIT.START = 0;

            /* Disable 32-kHz clock */
            RTC.RCR3.BIT.RTCEN = 0;    
            break;
        }
        /* Default case - should never be used */    
        default:
        {
            break;
        }
    }
}
/******************************************************************************
* End of function rtc_menu
******************************************************************************/

/******************************************************************************
* Function Name : configure_peripherals
* Description   : Sets up peripherals according to cpu_calc parameter
*                 Puts all peripherals in module stop mode cpu_calc == CPU_MIN
*                 Sets up DMA & CMT / MTU timers for cpu_calc == CPU_MAX
* Argument      : uint8_t cpu_calc indicates CPU load setting
* Return value  : none
******************************************************************************/
void configure_peripherals (const uint8_t cpu_calc)
{
    /* Shut off LEDs for measurement*/
    LED0_OFF;
    LED1_OFF;
    LED2_OFF;
    
    /* Disable AD input from potentiometer */
    MPC.P40PFS.BIT.ASEL = 0;
    PORT4.PMR.BIT.B0 = 0;

    /* Shut off potentiometer power */
    POT_EN = POWER_OFF;
    POT_EN_PDR = 0x0;

    switch (cpu_calc)
    {
        case CPU_TYP:
        {
            /* Ensure unused peripherals are out of module stop mode */
            SYSTEM.MSTPCRA.LONG = 0xEFF77DFF;
            SYSTEM.MSTPCRB.LONG = 0xBB5AF2AF;
            SYSTEM.MSTPCRC.LONG = 0x7FF7FFFE;
            break;
        }
        case CPU_MAX:
        {
            /* Ensure unused peripherals are out of module stop mode */
            SYSTEM.MSTPCRA.LONG = 0xEFF77DFF;
            SYSTEM.MSTPCRB.LONG = 0xBB5AF2AF;
            SYSTEM.MSTPCRC.LONG = 0x7FF7FFFE;

            /* Start the MTU timers */
            MTU.TSTR.BYTE = 0xC7; 

            /* Run the CMT Timer */
            CMT.CMSTR0.WORD = 0x0003;
            break;
        }
        default:
        case CPU_MIN:
        {
            /* Ensure all peripherals are in module stop mode */
            SYSTEM.MSTPCRA.LONG = 0xffffffff;
            SYSTEM.MSTPCRB.LONG = 0xffffffff;
            /* ...but do not turn off the RAM */
            SYSTEM.MSTPCRC.LONG = 0x7ffffffe;
            break;
        }
    }
}
/******************************************************************************
* End of function configure_peripherals
******************************************************************************/

/******************************************************************************
* Function Name : configure_power_tests
* Description   : Configures low power mode to enter upon wait instruction.
*                 Sets whether output ports will be enabled.
*                 Sets whether HOCO will be powered.
*                 Sets whether DMA transfers are enabled.
* Argument      : uint8_t clock_mode indicates Clock Mode setting
* Return value  : none
******************************************************************************/
void configure_power_tests (const uint8_t clock_mode)
{   
    /* Only needs additional configuration if clock_mode is set
       to a sleep, or software standby mode */
    if ((SW_SLEEP == clock_mode) || (SW_STBY == clock_mode) ||
            (DP_SLEEP == clock_mode))
    {
        /* Configure wait() mode based upon clock_mode */
        if (SW_STBY == clock_mode)
        {
            /* Software Standby Mode with Output Ports Enabled */
            /* Transition to Standby on wait() instruction */    
            SYSTEM.SBYCR.BIT.SSBY = 0x1;
            SYSTEM.MSTPCRC.BIT.DSLPE = 0x0;

            /* Allow time for settings to propagate before continuing */
            settle(5000);
        }
        else if (DP_SLEEP == clock_mode)
        {
            /* Transition to deep sleep mode upon WAIT instruction execution. */
            SYSTEM.SBYCR.BIT.SSBY = 0x0;
            SYSTEM.MSTPCRC.BIT.DSLPE = 0x1;
        }
        else
        {
            /* Sleep Mode with Output Ports Enabled */
            /* Transition to Sleep mode on wait() instruction */
            SYSTEM.SBYCR.BIT.SSBY = 0x0;
            SYSTEM.MSTPCRC.BIT.DSLPE = 0x0;
        }   
    }
}
/******************************************************************************
* End of function configure_power_tests
******************************************************************************/

/******************************************************************************
* Outline       : flash_led_value
* Description   : Flashes LEDs according to arguments
* Argument      : uint8_t LEDs - Mask for which LEDs
*                                b2: LED2, b1: LED1, b0: LED0
*               : uint8_t repeat - How many times to flash
* Return value  : none
******************************************************************************/
void flash_led_value (const uint8_t value, uint8_t repeat)
{
    /* Use argument to determine number of times to flash LEDs */
    while (repeat--)
    {
        /* Switch LEDs Off */
        LED0_OFF;
        LED1_OFF;
        LED2_OFF;
        
        /* Delay loop for keeping LEDs Off */
        settle(2000);

        /* Switch on LEDs specified by LEDs bitfield argument */
        LED0 = (uint8_t) !(value & LED0MASK);
        LED1 = (uint8_t) !(value & LED1MASK);
        LED2 = (uint8_t) !(value & LED2MASK);
        
        /* Delay loop for keeping LEDs On*/
        settle(2000);
    }
    
}
/******************************************************************************
* End of function flash_led_value
******************************************************************************/

/******************************************************************************
* Function Name : settle
* Description   : Provides delay loop for debounce and settle times
* Argument      : uint16_t Loops
* Return value  : none
******************************************************************************/
void settle (uint16_t loops)
{
    /* Delay loop */
    for (; loops; loops--)
    {
        nop();
    }    
}
/******************************************************************************
* End of function settle
******************************************************************************/




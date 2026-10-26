/* Adapted for use with IAR Embedded Workbench */
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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name     : power_modes_demo.c
* Version       : 1.0
* H/W Platform  : YRDKRX63N
* Description   : Empty application project
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <iorx63n.h>
#include <intrinsics.h>

#include "pmdemo_switches.h"
#include "lcd.h"


/*******************************************************************************
Macro definitions
*******************************************************************************/
#define PSW_USER_MODE_BIT      0x100000
#define BRK_VECTOR             0
#define SWITCH_DEBOUNCE_DELAY 20
#define LED_TOGGLE_DELAY       8


/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/
void sleep_mode (void);
void standby_mode(void);
void menu_display (void);
void green_leds_on (void);
void red_leds_on (void);
void all_leds_off (void);
void pmdemo_delay (uint32_t tick_counter);


/******************************************************************************
* Function name: power_modes_demo
* Description  : 
* Arguments    : none
* Return value : none
******************************************************************************/
void power_modes_demo(void)
{   
  /* Start off indicating that no switch has been pressed */
  g_pressed_switch = 0;
  
  /* Make sure the RTC is running */
  RTC.RCR2.BIT.START = 1;
  
  /* Display the main menu & enable switches for menu */
  menu_display();
  
  /* Arm the switch interrupts */
  g_pressed_switch = 0;
  
  /* Clear pending interrupts for all switches */
  switches_interrupts_clear();
  
  /* Enable SW1 and SW2 */
  switches_enable(SW1_MASK | SW2_MASK);
  
  /* Flash green LED's while waiting for a switch to be pressed */
  while (0 == g_pressed_switch)
  {
    green_leds_on();
    pmdemo_delay(LED_TOGGLE_DELAY);
    
    all_leds_off();
    pmdemo_delay(LED_TOGGLE_DELAY);
  }
  
  /* One of the switches was pressed, go to the proper low power mode */
  red_leds_on();
  
  /* Update display */
  lcd_display(LCD_LINE6, "            ");
  lcd_display(LCD_LINE7, "            ");
  
  switch (g_pressed_switch)
  {
  case 1:
    lcd_display(LCD_LINE4, "SWITCHING TO");
    lcd_display(LCD_LINE5, " SLEEP MODE ");
    break;
    
  case 2:
    lcd_display(LCD_LINE4, "SWITCHING TO");
    lcd_display(LCD_LINE5, " STDBY MODE ");
    break;
    
  default:
    lcd_display(LCD_LINE4, "INVALID MODE");
    lcd_display(LCD_LINE5, "????????????");
    break;
  }       
  
  /* All switches must be released for 1/2 second before continuing */
  do
  {
    pmdemo_delay(SWITCH_DEBOUNCE_DELAY);
  } while (0 == switches_released());
  
  /* Update display and LED's to show low power mode */
  lcd_display(LCD_LINE4, "   NOW IN   ");
  lcd_display(LCD_LINE7, " ANY SWITCH ");
  lcd_display(LCD_LINE8, "  WAKES UP  ");
  all_leds_off();
  
  /* To switch to low power modes, we need to:
     1. Disable interrupts globally (they'll be re-enabled by the wait instruction
     2. Clear pending interrupts for all switches
     3. Enable interrupts for switches in ICU
     4. Setup for entry into sleep/standby/deep standby mode
     5. Enter the low power state
  */

  /* 1. Disable interrupts globally (intrinsic function).
        This works fine since we are in supervisor mode by default. */
  __disable_interrupt();

  /* 2. Clear pending interrupts for all switches */
  switches_interrupts_clear();

  /* 3. Enable interrupts for switches in ICU */
  switches_enable(SW1_MASK | SW2_MASK | SW3_MASK);                 
  
  /* 4. Setup for entry into sleep/standby/deep standby mode */
  switch (g_pressed_switch)
  {
  case 1:
    sleep_mode();           /* Sleep   mode exited by IRQ interrupt on SW1-3 */
    break;
    
  case 2:
    standby_mode();         /* Standby mode exited by IRQ interrupt on SW1-3 */
    break;
    
  default:
    sleep_mode();           /* Sleep   mode exited by IRQ interrupt on SW1-3 */
    break;    
  }
  
  /* 6. Enter the selected low power state (intrinsic function) */
  __wait_for_interrupt();
  
  /* When the wait instruction is executed, the global interrupt flag is set 
     to enable interrupts that were disabled in step 2 above. The MCU is then 
     switched into low power mode. It can be returned to the run state by a  
     number of interrupt sources depending on the lowpower mode.       
     The code below this point executes after the MCU has woken up from the 
     low power mode.
  */
    
  /* Update the LED's and the display to indicate we're running again */
  green_leds_on();
  lcd_display(LCD_LINE4, "  WOKE UP!  ");
  lcd_display(LCD_LINE5, "            ");
  lcd_display(LCD_LINE7, "            ");
  lcd_display(LCD_LINE8, "            ");
  
  /* Debounce the switch that woke us up */
  do
  {
    pmdemo_delay(SWITCH_DEBOUNCE_DELAY);
  } while (0 == switches_released());

} /* End of function power_modes_demo() */


/******************************************************************************
* Function name: sleep_mode
*
* Description  : Configure the MCU for sleep mode
*
*                In sleep mode, the CPU stops operating but the contents of its 
*                internal registers are retained. Other peripheral functions do 
*                not stop. When the WDT is used, the WDT stops counting when 
*                sleep mode is entered.
*                Sleep mode is entered after the next WAIT instruction is executed.
*
*                Sleep mode is canceled by:
*                    any interrupt,
*                    the reset signal from the RES# pin,
*                    a power-on reset,
*                    a voltage monitoring reset,
*                    or a reset caused by an IWDT underflow.
*
* Arguments    : none
* Return value : none
******************************************************************************/
void sleep_mode (void)
{
  /* Shifts to sleep mode or all-module clock stop mode after the WAIT 
  instruction is executed. */
  SYSTEM.SBYCR.BIT.SSBY = 0; 
} /* End of function () */


/******************************************************************************
* Function name: standby_mode
*
* Description  : Configure the MCU for standby mode.
*
*                In this mode, the CPU, on-chip peripheral functions, and all 
*                the oscillator functions stop. However, the contents of the CPU 
*                internal registers, on-chip RAM data, on-chip peripheral
*                functions, and the states of the I/O ports are retained.
*                Whether the address bus and bus control signals are placed in 
*                the high-impedance or the output state is retained can be 
*                specified by the SBYCR.OPE bit.
*                Software standby mode allows significant reduction in power 
*                consumption because the oscillator stops in this mode.
*                Software standby mode is entered after the next WAIT instruction 
*                is executed.
*                Software standby mode is released by:
*                    an external pin interrupt (the NMI or IRQ0 to IRQ15),
*                    peripheral interrupts:
*                        the RTC alarm,
*                        RTC interval,
*                        IWDT,
*                        USB suspend/resume,
*                        voltage-monitoring 1,
*                        and voltage-monitoring 2 interrupts,
*                    an RES# pin reset,
*                    a power-on reset,
*                    a voltage-monitoring reset,
*                    or an independent watchdog timer reset.
*
* Arguments    : none
* Return value : none
******************************************************************************/
void standby_mode (void)
{
  /* Shifts to software standby mode after the WAIT instruction is executed. */
  SYSTEM.SBYCR.BIT.SSBY = 1;
  
  /* In software standby mode or deep software standby mode, the address bus 
  and bus control signals are set to the high-impedance state. */    
  SYSTEM.SBYCR.BIT.OPE = 0;    
  
  /* Transition to software standby mode (but not deep software standby mode) */
  SYSTEM.DPSBYCR.BIT.DPSBY = 0;
  
  /* Disable oscillator stop detection and its corresponding interrupt.
  This has to be done in order to enter the software standby mode. */
  SYSTEM.OSTDCR.BYTE = 0x00; 
  
  /* Wait for clock to stabilize. Set a wait time to 65536 states. This is
  done for the RX62N but not necessary for the RX63N. */                                     
  //SYSTEM.SBYCR.BIT.STS = 0x0c; 
} /* End of function standby_mode() */


/******************************************************************************
* Function name: pmdemo_delay
* Description  : Uses 64-Hz counter of RTC to generate approximate delays.
* Arguments    : tick_counter -
*                   the number of 32Hz clock pulses to delay. 
* Return value : none
******************************************************************************/
void pmdemo_delay(uint32_t tick_counter)
{    
  do
  {
    while ((RTC.R64CNT.BYTE & 0x02) == 2) /* Wait for F32HZ bit to go low. */
    {
    }
    
    while ((RTC.R64CNT.BYTE & 0x02) == 0) /* Wait for F32HZ bit to go high. */
    {
    }
  } while (--tick_counter);
} /* End of function pmdemo_delay() */


/******************************************************************************
* Function name: menu_display
* Description  : Shows the application menu on the LCD
* Arguments    : none
* Return value : none
******************************************************************************/
void menu_display (void)
{
  lcd_clear();
  
  /* Display message on LCD */
  lcd_display(LCD_LINE1, "RX63N POWER ");
  lcd_display(LCD_LINE2, "MODES DEMO  ");
  
  lcd_display(LCD_LINE4, "SWITCH  MODE");
  lcd_display(LCD_LINE5, "SW1    SLEEP");
  lcd_display(LCD_LINE6, "SW2    STDBY");
} /* End of function menu_display() */


/******************************************************************************
* Function name: green_leds_on
* Description  :
* Arguments    : none
* Return value : none
******************************************************************************/
void green_leds_on (void)
{
  PORTD.PODR.BYTE = 0xC7;
  PORTE.PODR.BYTE = 0x01;
} /* End of function green_leds_on() */


/******************************************************************************
* Function name: red_leds_on
* Description  :
* Arguments    : none
* Return value : none
******************************************************************************/
void red_leds_on (void)
{
  PORTD.PODR.BYTE = (uint8_t) ~0xC7;
  PORTE.PODR.BYTE = (uint8_t) ~0x01;
} /* End of function red_leds_on() */

    
/******************************************************************************
* Function name: all_leds_off
* Description  :
* Arguments    : none
* Return value : none
******************************************************************************/
void all_leds_off (void)
{
  PORTD.PODR.BYTE = (uint8_t) 0xFF;
  PORTE.PODR.BYTE = (uint8_t) 0x0F;
} /* End of function all_leds_off() */

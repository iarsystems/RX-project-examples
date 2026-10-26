/* Copyright (c) 2010 IAR Systems AB.
 *
 * IAR Embedded Workbench tutorial
 * RX610 - Varying power consumption example
 *
 * Overview:
 * This example demonstrates how different clock frequencies and standby modes
 * affect the power consumption. To monitor the power consumption open the
 * "Timeline" window in the "Emulator" menu.
 *
 * SW3 - Wake up from standby mode + Switch between clock speeds x2 and x8
 * SW2 - Enter All-Module Standy Mode
 * SW1 - Enter Software Standby Mode
 *
 * Description:
 * When the CPU is running, four LEDs will sweep back and forth. Pressing down
 * the SW3 button will make the System Clock and the Peripheral Module Clock run
 * with a clock multiplier of x8, instead of x2 (default). This will increase
 * the power consumption and the LEDs will sweep at a higher rate. To switch
 * back to the default value x2 press down the same button (SW3) again.
 *
 * Pressing down the SW2 button puts the CPU in the All-Module Stop Mode. In
 * this mode the CPU, the on-chip RAM and the peripheral modules will be
 * stopped. To wake them up and to continue the code execution press down the
 * SW3 button. The falling edge on SW3 will generate an interrupt which will
 * wake up the CPU.
 *
 * The SW1 button puts the CPU in another, deeper, sleep mode called Software
 * Standby Mode, in which the oscillator also will be stopped. Pressing down SW3
 * will wake it up.
 *
 * For more information about different sleep modes look in the RX610 hardware
 * manual (section "Low Power Consumption").
 *
 * Code details:
 * Since the SW3 button is used to toggle a setting (choosing between different
 * clock speeds in this case) some sort of button debouncing is needed. In this
 * example this is accomplished by using a variable 'SW3_cnt', which is
 * incremented by a timer interrupt routine and reset each time a falling edge
 * on SW3 has been detected. When 'SW3_cnt' finally reaches up some value
 * ('DEBOUNCE_TIME_MS_10') a variable 'SW3_pressed' is set to 1, indicating that
 * a stable press-down has been detected.
 *
 * Debouncing is not needed for the SW1 and SW2 buttons since they will just
 * make the CPU enter sleep mode. No interrupts are tied to SW1 or SW2, so any
 * falsely detected button presses won't cause the CPU to wake up again.
 *
 *          
 */

#include "iorx610.h"
#include "intrinsics.h"
#include "lcd.h"

/* Used for reading the buttons connected to P00, P01 and P13 */
#define SW1     PORT0.PORT.BIT.B0
#define SW1_DDR PORT0.DDR.BIT.B0
#define SW1_ICR PORT0.ICR.BIT.B0
#define SW2     PORT0.PORT.BIT.B1
#define SW2_DDR PORT0.DDR.BIT.B1
#define SW2_ICR PORT0.ICR.BIT.B1
#define SW3     PORT1.PORT.BIT.B3
#define SW3_DDR PORT1.DDR.BIT.B3
#define SW3_ICR PORT1.ICR.BIT.B3

/* The four LEDs are connected to P83, P84, P33 and P34 */
#define LED0     PORT8.DR.BIT.B3
#define LED0_DDR PORT8.DDR.BIT.B3
#define LED1     PORT8.DR.BIT.B4
#define LED1_DDR PORT8.DDR.BIT.B4
#define LED2     PORT3.DR.BIT.B3
#define LED2_DDR PORT3.DDR.BIT.B3
#define LED3     PORT3.DR.BIT.B4
#define LED3_DDR PORT3.DDR.BIT.B4

/* Button debouncing defines */
#define DEBOUNCE_TIME_MS_10 3 //30ms
#define DEBOUNCE_STOP       DEBOUNCE_TIME_MS_10 + 1

/* Different timer settings at different Peripheral Module Clock speeds */
#define CMT1_CMCOR_AT_X2  488
#define CMT1_CMCOR_AT_X8  4*CMT1_CMCOR_AT_X2

volatile unsigned char SW3_cnt = DEBOUNCE_STOP;
volatile unsigned char SW3_pressed = 0;

/* Timer used to sweep the LEDs (executed once every second) */
#pragma vector = VECT_CMT0_CMI0
__interrupt void isr_cmt0(void)
{
  static unsigned char led_i = 0;
  static signed char dir = 1;
  
  // Light up one of the LEDs
  LED0 = (led_i!=0);
  LED1 = (led_i!=1);
  LED2 = (led_i!=2);
  
  // Need to change sweeping direction?
  if (led_i==0 && dir==-1)     dir= 1;
  else if (led_i==2 && dir==1) dir=-1;
  
  // Move to the next LED
  led_i += dir;

  ICU.IR[70].BIT.IR = 0; // clear interrupt request flag
}

/* Timer used for handling button debouncing */
#pragma vector = VECT_CMT1_CMI1
__interrupt void isr_cmt1(void)
{
  // A stable button press down has been detected if we've been able to
  // increment the 'SW3_cnt' variable all the way to 'DEBOUNCE_TIME_MS_10'.
  // been detected for some time.
  if (SW3_cnt == DEBOUNCE_TIME_MS_10) {
    if (!SW3) { // SW3 still pressed down?
      SW3_pressed = 1;
    }
    SW3_cnt = DEBOUNCE_STOP; // park the 'SW3_cnt' counter
  }
  else if (SW3_cnt < DEBOUNCE_TIME_MS_10) {
    SW3_cnt++;
  }
  
  ICU.IR[74].BIT.IR = 0; // clear interrupt request flag
}

/* Interrupt routine for SW3 (falling edge) */
#pragma vector = VECT_ICU_IRQ3
__interrupt void isr_sw3(void)
{
  // Reset the 'SW3_cnt' variable used in the debouncing interrupt routine
  SW3_cnt = 0;
  
  ICU.IR[67].BIT.IR = 0; // clear interrupt request flag
}

/* Software sleep */
void sw_sleep()
{
  SYSTEM.SBYCR.BIT.SSBY = 1;
  SYSTEM.DPSBYCR.BIT.DPSBY = 0;
  
  // confirm last register write before executing "wait" instruction
  while (SYSTEM.DPSBYCR.BIT.DPSBY != 0)
    ;
  __wait_for_interrupt();
}

/* All-module sleep */
void all_module_sleep()
{
  SYSTEM.SBYCR.BIT.SSBY = 0;
  SYSTEM.MSTPCRA.LONG = 0xFFFFFFFF; // disable all modules
  SYSTEM.MSTPCRB.LONG = 0xFFFFFFFF; // disable all modules
  SYSTEM.MSTPCRA.BIT.ACSE = 1;
  
  // confirm last register write before executing "wait" instruction
  while (SYSTEM.MSTPCRA.BIT.ACSE != 1)
    ;
  __wait_for_interrupt();
}

/* Switches between System and Peripheral Module Clock x2 and x8 */
void switch_between_clk_x2_and_x8()
{
  if (SYSTEM.SCKCR.BIT.ICK == 0x02) { // x2? Switch to x8
    SYSTEM.SCKCR.BIT.ICK = 0x00;    // System Clock            = x8
    SYSTEM.SCKCR.BIT.PCK = 0x00;    // Peripheral Module Clock = x8
    CMT1.CMCOR = CMT1_CMCOR_AT_X8;  // Make sure the debounce timer runs at 100 Hz
    DisplayLCD(LCD_LINE1, "Clock   ");
    DisplayLCD(LCD_LINE2, "spd: x8 ");
  } else { // switch to x2
    SYSTEM.SCKCR.BIT.PCK = 0x02;    // Peripheral Module Clock = x2
    SYSTEM.SCKCR.BIT.ICK = 0x02;    // System Clock            = x2
    CMT1.CMCOR = CMT1_CMCOR_AT_X2;  // Make sure the debounce timer runs at 100 Hz
    DisplayLCD(LCD_LINE1, "Clock   ");
    DisplayLCD(LCD_LINE2, "spd: x2 ");
  }
}

/* Initialise registers */
void init()
{
  // set up LCD pins
  SetupLCDPorts();
  
  // initialise the LCD
  InitialiseLCD();
  
  // set up output pins
  LED0_DDR = 1; //LED0 (output)
  LED1_DDR = 1; //LED1 (output)
  LED2_DDR = 1; //LED2 (output)
  LED3_DDR = 1; //LED3 (output)
  
  // set up input pins
  SW1_DDR = 0;  //SW1 (input)
  SW2_DDR = 0;  //SW2 (input)
  SW3_DDR = 0;  //SW3 (input)
  SW1_ICR = 1;  //SW1 (enable input buffer)
  SW2_ICR = 1;  //SW2 (enable input buffer)
  SW3_ICR = 1;  //SW3 (enable input buffer)

  // enable modules
  SYSTEM.MSTPCRA.BIT.MSTPA15 = 0; // CMT timers 0 and 1 module

  // set up CMT timer 0
  CMT0.CMCR.BIT.CKS = 3;          // 25MHz/512 = 48.8kHz
  CMT0.CMCR.BIT.CMIE = 1;         // enable peripheral interrupt source
  CMT0.CMCOR = 48828;             // 1 Hz operation
  
  // set up CMT timer 1
  CMT1.CMCR.BIT.CKS = 3;          // 25MHz/512 = 48.8kHz
  CMT1.CMCR.BIT.CMIE = 1;         // enable peripheral interrupt source
  CMT1.CMCOR = CMT1_CMCOR_AT_X2;  // 100 Hz operation

  // set up timer interrupts
  ICU.IER[3].BIT.IEN4 = 1;        // enable timer 0 interrupt
  ICU.IER[3].BIT.IEN5 = 1;        // enable timer 1 interrupt
  ICU.IPR[0x04].BIT.IPR = 1;      // LED flash timer (level 1)
  ICU.IPR[0x05].BIT.IPR = 2;      // Debounce timer (level 2)

  // start timers
  CMT.CMSTR0.BIT.STR0 = 1;
  CMT.CMSTR0.BIT.STR1 = 1;
  
  // set up SW3 (P13) switch interrupt (IRQ3)
  ICU.IER[8].BIT.IEN3 = 1;        // enable SW3 interrupt
  ICU.IPR[0x23].BIT.IPR = 3;      // set interrupt priority (level 3)
  IOPORT.PFCR9.BIT.ITS3 = 1;      // assign P13 to IRQ3
  ICU.IRQER[3].BIT.IRQEN = 1;     // set IRQ3 to be detected as an external int. src.
  ICU.SSIER.BIT.SSI3 = 1;         // allow IRQ3 to wake up the ICU from SW. standby mode
  ICU.IRQCR[3].BIT.IRQMD = 1;     // trigger interrupt on falling edge
}

/* Main program. */
void main(void)
{
  // set up everything
  init();
  
  // display LCD start-up message
  DisplayLCD(LCD_LINE1, "Power   ");
  DisplayLCD(LCD_LINE2, "    demo");

  // enable global interrupts
  __enable_interrupt();

  while(1) {
    // SW3 - Switching between System and Peripheral Module Clock x2 and x8
    if (SW3_pressed) {
      switch_between_clk_x2_and_x8();
      SW3_pressed = 0; // SW3 press handled
    }
    
    // SW2 - Enter All-Module Clock Stop Mode
    if (!SW2) {
      DisplayLCD(LCD_LINE1, "All-mod."); DisplayLCD(LCD_LINE2, "Standby ");
      all_module_sleep();
      SYSTEM.MSTPCRA.BIT.MSTPA15 = 0; // Re-enable the timer unit
    }
    
    // SW1 - Enter Software Standby Mode
    if (!SW1) {
      DisplayLCD(LCD_LINE1, "Software"); DisplayLCD(LCD_LINE2, "Standby ");
      sw_sleep();
    }
  }
}

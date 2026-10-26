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
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name		: main.c
* Version 		: 1.00
* Device 		: R5F52108DFP
* Tool-Chain 	: RX Family C Compiler
* H/W Platform	: RPBRX210
* Description   : Main file
*******************************************************************************/
/*******************************************************************************
* History 		: 13.08.2011  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Include machine.h to use the nop() function */
#include    <machine.h>
/* Standard integer types with specified widths definitions */
#include    <stdint.h>     
/* Register address definitions */
#include <iodefine.h>
//#include    "iodefine.h" old
/* Hardware platform definitions */
#include    "rsk_defines.h"
/* Analogue to digital and user interface function prototypes */
#include    "s12ad.h"
/* Clock Mode setup function prototypes */
#include    "setupclocks.h"
/* RAM based CPU exercise function prototypes */
#include    "PowerTests.h"
/* Prototypes for functions in this file */
#include     "main.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* For use in demonstrating CPU load and operation of DMA transfer */
uint32_t  g_Destination[8];

/*******************************************************************************
* Outline       : main
* Description   : The main program function. Provides user interface for 
*                 selection of operation modes by using the potentiometer
*                 and SW1. Calls functions to configure selected operation and
*                 clock modes before transferring execution to the ram-based
*                 PerformPowerTests function.
*  Operation    : These instructions are for executing the preprogrammed
*                 tutorial code.
*                 1. J15: Fit to disable debugger.
*                    Note this will need to be removed to reprogram.
*
*                 2. Fit J11 (connect pins 2-3): Power Source Select USB power.
*
*                 3. Replace J6 with an ammeter for CPU current measurement.
*
*                 4. Plug in the USB cable between the PC and the RPB to
*                    power the board and execute the pre-programmed demo code.
*                    Note that the USB connector is on the underside of the
*                    board. The power LED indicator should light.
*
*                 5. The programmed code will now execute, Firstly the
*                    LEDs will display a moving pattern.
*
*                 6. Next the user must select a value with the pot / SW1
*                    that corresponds to the options below:
*                    +====================================================+
*                    |        MAIN MENU OPTIONS USER INTERFACE MENU       |
*                    +====================================================+
*                    | Adjust potentiometer RV1 then press SW1 to select. |
*                    | Corresponding value will be displayed on LEDs      |
*                    | using binary. LED0 is the lsb but is on the left   |
*                    | side of the RPB, hence:                            |
*                    |                    (lsb)  LED0  LED1  LED2 (msb)   |
*                    |   (0) RTC_MENU             0     0     0           |
*                    |   (1) SUB_32k              1     0     0           |
*                    |   (2) LOCO_125k            0     1     0           |
*                    |   (3) HOCO_1M              1     1     0           |
*                    |   (4) HOCO_50M             0     0     1           |
*                    |   (5) SW_SLEEP             1     0     1           |
*                    |   (6) SW_STBY              0     1     1           |
*                    |   (7) SW_DP_STBY           1     1     1           |
*                    +----------------------------------------------------/
*
*                    7a. If option 0 was selected then the user should select
*                       a value for the RTC options:
*                        +====================================================+
*                        |            RTC OPTIONS USER INTERFACE MENU         |
*                        +====================================================+
*                        | Adjust potentiometer RV1 then press SW1 to select. |
*                        |                    (lsb)  LED0  LED1  LED2 (msb)   |
*                        |   (1) Output RTC           1     0     0           |
*                        |   (2) Reset RTC            0     1     0           |
*                        |   (3) Start RTC            1     1     0           |
*                        |   (4) Stop RTC             0     0     1           |
*                        +----------------------------------------------------/
*                        (will return to step 5 after selection)
*
*                    7b. Otherwise, a clock mode was selected. LED0 will flash
*                        the selected number of times to confirm the selection.
*
*                    8. The user must next select the CPU load option in the
*                       same way:
*                       +====================================================+
*                       |           CPU LOAD USER INTERFACE MENU             |
*                       +====================================================+
*                       | Adjust potentiometer RV1 then press SW1 to select. |
*                       |                    (lsb)  LED0  LED1  LED2 (msb)   |
*                       |   (1) CPU_MIN              1     0     0           |
*                       |   (2) CPU_TYP              0     1     0           |
*                       |   (3) CPU_MAX              1     1     0           |
*                       +----------------------------------------------------/
*
*                   9. LED1 will flash the selected number of times to confirm.
*
*                   10. At this point the CPU enters a full power loop in the 
*                       selected mode. Current measurements can be taken
*                       across J6 at this point.
*
*                   11. Press SW1 to put the CPU into the selected low power
*                       mode and again current measurements can be taken
*                       across J6.
*
*                   12. To exit the low power mode and restart, press RES1.
*
* Argument      : none
* Return value  : none
*******************************************************************************/
void main(void)
{
    /* Variable to store the selected CPU load setting */
    uint8_t  cpu_calc;
    /* Variable to store the selected Clock Mode setting */
    uint8_t  clock_mode;
    
    /* Initialise hardware */
    Initialise();

    /* Enter loop to Present main menu selection until an 
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
        | using binary. LED0 is the lsb but is on the left   |
        | side of the RPB, hence:                            |
        |                    (lsb)  LED0  LED1  LED2 (msb)   |
        |   (0) RTC_MENU             0     0     0           |
        |   (1) SUB_32k              1     0     0           |
        |   (2) LOCO_125k            0     1     0           |
        |   (3) HOCO_1M              1     1     0           |
        |   (4) HOCO_50M             0     0     1           |
        |   (5) SW_SLEEP             1     0     1           |
        |   (6) SW_STBY              0     1     1           |
        |   (7) SW_DP_STBY           1     1     1           |
        +---------------------------------------------------*/
        clock_mode = WelcomeMenu();
        
        /* If RTC_MENU option is chosen then
           Collect user input for RTC options */
        if ( RTC_MENU == clock_mode)
           {
                /* Wait for user input using potentiometer and SW1 
                   for RTC options selection:
                +====================================================+
                |            RTC OPTIONS USER INTERFACE MENU         |
                +====================================================+
                | Adjust potentiometer RV1 then press SW1 to select. |
                |                    (lsb)  LED0  LED1  LED2 (msb)   |
                |   (1) Output RTC           1     0     0           |
                |   (2) Reset RTC            0     1     0           |
                |   (3) Start RTC            1     1     0           |
                |   (4) Stop RTC             0     0     1           |
                +---------------------------------------------------*/
            RTCMenu();
        }
    }while ( RTC_MENU == clock_mode);
       
   
    /*===================================================+
    |          LED0 FLASH USER INTERFACE                 |
    +====================================================+
    | Flash LED0, Clock Mode times to confirm selection  |
    +---------------------------------------------------*/
    FlashLEDValue( LED0Mask, clock_mode );

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
    cpu_calc = CPULoadMenu();
        
    /*===================================================+
    |            LED1 FLASH USER INTERFACE               |
    +====================================================+
    | Flash LED1, cpu_calc times to confirm selection     |
    +---------------------------------------------------*/
    FlashLEDValue( LED1Mask, cpu_calc );

    /* Configure the peripherals for the selected Clock Mode */
    ConfigurePeripherals(cpu_calc);
    
    /* Set the clock mode */
    Setclock_mode(clock_mode, cpu_calc);

    /* Setup the power tests according to clock_mode */
    ConfigurePowerTests(clock_mode);
    
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
    PerformPowerTests(clock_mode, cpu_calc);

    /* PerformPowerTests should never return */
    while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/

/******************************************************************************
* Outline 		: Initialise
* Description 	: Sets port directions, internal pull ups etc
* Argument  	: none
* Return value  : none
******************************************************************************/
void Initialise( void )
{
    /* Enable access to System Registers */
    SYSTEM.PRCR.WORD = 0xA503;                                    
    
    /* Set LED Port Direction Registers */                              
    LED0_PDR = OUTPUT;                                      
    LED1_PDR = OUTPUT;                                      
    LED2_PDR = OUTPUT;                               
    
    /* Gain access to the Port Function Select Registers */
    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;
    
    /* Ensure that SW1 port is set as an input pin */
    SW1_PDR = INPUT;

    /* Initialise the A2D */
    InitS12AD();

    /* Set the unused port pull ups */    
    PORT0.PCR.BYTE = 0xA8;
    PORT1.PCR.BYTE = 0x10;
    PORT2.PCR.BYTE = 0xFF;
    PORT3.PCR.BYTE = 0xDC;
    PORT4.PCR.BYTE = 0xFE;
    PORT5.PCR.BYTE = 0x3F;
    PORTA.PCR.BYTE = 0xFF;
    PORTB.PCR.BYTE = 0xFF;
    PORTC.PCR.BYTE = 0x7F;
    PORTD.PCR.BYTE = 0xFF;
    PORTE.PCR.BYTE = 0xFF;
    PORTH.PCR.BYTE = 0x0F;
    PORTJ.PCR.BYTE = 0x0A;
    
    /* Set SCL and SDA (P1.2 & P1.3) as inputs */
    /* Remove next four lines to use IIC */
    PORT1.PDR.BIT.B2 = INPUT;
    PORT1.PDR.BIT.B3 = INPUT;
    PORT1.PCR.BIT.B2 = 1;
    PORT1.PCR.BIT.B3 = 1;
}
/******************************************************************************
* End of function Initialise
******************************************************************************/

/******************************************************************************
* Outline 		: WelcomeMenu - Flashes LEDs &allows selection of mode
* Description 	: Allows user selection of 0 to 7 by adjusting RV1.
*                 Corresponding value will be displayed on LEDs using binary.
*                 LED0 is the lsb but is on the left side of the RPB, hence:
*                                  (lsb)  LED0  LED1  LED2 (msb)
*                 (0) RTC_MENU             0     0     0
*                 (1) SUB_32k              1     0     0
*                 (2) LOCO_125k            0     1     0
*                 (3) HOCO_1M              1     1     0
*                 (4) HOCO_50M             0     0     1
*                 (5) SW_SLEEP             1     0     1
*                 (6) SW_STBY              0     1     1
*                 (7) SW_DP_STBY           1     1     1
* Argument  	: none
* Return value  : uint8_t selected menu item number [range 0 to 7]
******************************************************************************/
uint8_t WelcomeMenu( void )
{
    /* repeat loop counter */
    volatile uint8_t repeat = 15;

    /* Uniquely Flash LEDs to indicate first menu */
    while( repeat-- )
    {
        /* Delay loop */
        Settle(650);

        /* Output pattern on LEDs */
        LED0 = (uint8_t) !((repeat) % 3);
        LED1 = (uint8_t) !((repeat+1) % 3);
        LED2 = (uint8_t) !((repeat+2) % 3);

        /* Delay loop */
        Settle(650);
    };

    /* Await option selection from user to specify which clock mode
     * to demonstrate. The options are selected by using the 
     * potentiomenter RV1 and accepted by pressing SW1.
     * The LEDs 0 1 2 count up in binary from 0 (all off) to 7 (all on).
     * Return the selected option value */
    return (GetUserInput(0, 7));   
}
/******************************************************************************
* End of function WelcomeMenu
******************************************************************************/

/******************************************************************************
* Outline 		: CPULoadMenu - Allows user selection of CPU Load options
* Description 	: Allows user selection of 1 to 3 by adjusting RV1.
*                 Corresponding value will be displayed on LEDs using binary.
*                 LED0 is the lsb but is on the left side of the RPB, hence:
*                                  (lsb)  LED0  LED1  LED2 (msb)
*                 (1) CPU_MIN              1     0     0
*                     (Stop all modules)
*                 (2) CPU_TYP              0     1     0
*                     (Enable all modules) 
*                 (3) CPU_MAX              1     1     0
*                     (Enable all modules, DMA, MTU & CMT Timers
* Argument  	: none
* Return value  : uint8_t selected menu item number
******************************************************************************/
uint8_t CPULoadMenu( void )
{
     /* Await option selection from user to specify which CPU load
     * to demonstrate. The options are selected by using the 
     * potentiomenter RV1 and accepted by pressing SW1.
     * There are 3 options:
     * (1) CPU_MIN
     * (2) CPU_TYP
     * (3) CPU_MAX
     * Return the selected option value */
    return (GetUserInput(1, 3));
}
/******************************************************************************
* End of function CPULoadMenu
******************************************************************************/

/******************************************************************************
* Outline 		: RTCMenu - Allows user control of Real Time Clock options
* Description 	: Allows selection of 1 to 4 by adjusting RV1.
*                 Corresponding value will be displayed on LEDs using binary.
*                 LED0 is the lsb but is on the left side of the RPB, hence:
*                                  (lsb)  LED0  LED1  LED2 (msb)
*                 (1) Output RTC           1     0     0
*                 (2) Reset RTC            0     1     0
*                 (3) Start RTC            1     1     0
*                 (4) Stop RTC             0     0     1
* Argument  	: none
* Return value  : none
******************************************************************************/
void RTCMenu( void )
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
     * potentiomenter RV1 and accepted by pressing SW1.
     * There are 4 options:
     * (1) Output RTC
     * (2) Reset RTC
     * (3) Start RTC
     * (4) Stop RTC */
    rtc_menu_option = GetUserInput(1, 4);

    /* Perform selected RTC Menu operation */
    switch (rtc_menu_option)
    {
        /* Code to output the RTC value to the LEDs */
        case OUTPUT_RTC:
            /* Store Hours setting from RTC  */
            hours = (uint8_t) ((RTC.RHRCNT.BIT.HR10*10) + (RTC.RHRCNT.BIT.HR1));
            /* Store Minutes setting from RTC  */
            mins = (uint8_t) ((RTC.RMINCNT.BIT.MIN10*10) + (RTC.RMINCNT.BIT.MIN1));
            /* Store Seconds setting from RTC  */
            secs = (uint8_t) ((RTC.RSECCNT.BIT.SEC10*10) + (RTC.RSECCNT.BIT.SEC1));
        
            /* Flash LED0 Hours times, to indicate Hours */
            FlashLEDValue( LED0Mask, hours );
        
            /* Flash LED1 Mins times, to indicate Minutes */
            FlashLEDValue( LED1Mask, mins );
        
            /* Flash LED2 Secs times, to indicate Seconds */
            FlashLEDValue( LED2Mask, secs );
            break;
            
        /* Code to reset the RTC value */
        case RESET_RTC:
            /* Reset RTC Hours count */
            RTC.RHRCNT.BYTE = 0;
            /* Reset RTC Minutes count */
            RTC.RMINCNT.BYTE = 0;
            /* Reset RTC Seconds count */
            RTC.RSECCNT.BYTE = 0;
            break;    
        
        /* Code to start the RTC */    
        case START_RTC:
            /* Disable RTC Interrupts */
            RTC.RCR1.BYTE = 0;
            /* Enable 32-kHz clock */
            RTC.RCR3.BIT.RTCEN = 1;              

            /* Allow RTC clock to settle */
            Settle(5000);
    
            /* Enable RTC input from Subclock & set standard drive ability */
            RTC.RCR2.BIT.START = 0x0D;
            /* Enable RTC Interrupts */
            RTC.RCR1.BYTE = 1;
            break;
            
        /* Code to stop the RTC */      
        case STOP_RTC:
            /* Disable RTC Interrupts */
            RTC.RCR1.BYTE = 0;
            /* Disable 32-kHz clock */
            RTC.RCR3.BIT.RTCEN = 0;    
            break;
        /* Default case - should never be used */    
        default:
            break;
    }
}
/******************************************************************************
* End of function RTCMenu
******************************************************************************/

/******************************************************************************
* Outline 		: ConfigurePeripherals  
* Description 	: Sets up peripherals according to cpu_calc parameter
*                 Puts all peripherals in module stop mode cpu_calc == CPU_MIN
*                 Sets up DMA & CMT / MTU timers for cpu_calc == CPU_MAX
* Argument  	: uint8_t cpu_calc indicates CPU load setting
* Return value  : none
******************************************************************************/
void ConfigurePeripherals(uint8_t cpu_calc)
{
    /* Will now run the power measurement code so set the I/O that were used */
    /* as output I/O for the LEDs to input I/O                               */
    LED0_PDR = INPUT;
    LED1_PDR = INPUT;
    LED2_PDR = INPUT;
    
    switch ( cpu_calc )
    {
        case CPU_TYP:
            /* Ensure unused peripherals are out of module stop mode */
            SYSTEM.MSTPCRA.LONG = 0x43753dcf;
            SYSTEM.MSTPCRB.LONG = 0x395df8af;
            SYSTEM.MSTPCRC.LONG = 0xf3f7fffe;
            break;
        case CPU_MAX:
            /* Ensure unused peripherals are out of module stop mode */
            SYSTEM.MSTPCRA.LONG = 0x43753dcf;
            SYSTEM.MSTPCRB.LONG = 0x395df8af;
            SYSTEM.MSTPCRC.LONG = 0xf3f7fffe;    
        
            /* Init DMAC */
            /* This will transfer the MTU2_0 TCNT0 value to a RAM location  */
            /* Activation is under Software Control */
            /* Source */
            DMAC0.DMSAR = (uint32_t)&g_Product;
        
            /* Destination */
            DMAC0.DMDAR = (uint32_t)&g_Destination[0];
        
            /* max transfers */
            DMAC0.DMCRA = 65535;
        
             /* Normal Mode */
            DMAC0.DMTMD.BIT.MD = 0;
        
            /* Long transfer */
            DMAC0.DMTMD.BIT.SZ = 2;
        
            /* Interrupt Request */
            DMAC0.DMTMD.BIT.DCTG = 1;
        
            /* Source is fixed */
            DMAC0.DMAMD.BIT.SM = 0;
        
            /* Destination is fixed */
            DMAC0.DMAMD.BIT.DM = 0;
        
            /* MTU TGRA ISR Vector Number */
            ICU.DMRSR0 = 114;
        
            /* Enables DMA transfer */
            DMAC0.DMCNT.BIT.DTE = 1;
        
            /* DMACA activation is enabled */                     
            DMAC.DMAST.BIT.DMST = 1;

            /* Configure MTU0 to trigger DMAC0 */
            IEN(MTU0,TGIA0) = 1;
            MTU0.TIER.BIT.TGIEA = 1;

            /* Start the MTU timers */
            MTU.TSTR.BYTE = 0xC7; 

            /* Run the CMT Timers */
            CMT.CMSTR0.WORD = 0x0003;
            CMT.CMSTR1.WORD = 0x0003;
            break;
        default:
        case CPU_MIN:
            /* Ensure all peripherals are in module stop mode */
            SYSTEM.MSTPCRA.LONG = 0xffffffff;
            SYSTEM.MSTPCRB.LONG = 0xffffffff;
            /* ...but do not turn off the RAM */
            SYSTEM.MSTPCRC.LONG = 0xfffffffe;          
            break;
    }
}
/******************************************************************************
* End of function ConfigurePeripherals
******************************************************************************/

/******************************************************************************
* Outline 		: ConfigurePowerTests  
* Description 	: Configures low power mode to enter upon wait instruction.
*                 Sets whether output ports will be enabled.
*                 Sets whether HOCO will be powered.
*                 Sets whether DMA transfers are enabled.
* Argument  	: uint8_t clock_mode indicates Clock Mode setting
* Return value  : none
******************************************************************************/
void ConfigurePowerTests( uint8_t clock_mode )
{   
    /* Only needs additional configuration if clock_mode is set
       to a sleep, or software standby mode */
    if( (SW_SLEEP == clock_mode) ||
    (SW_STBY == clock_mode) || 
    (SW_DP_STBY == clock_mode) )
    {
        /* "Analog Function Select": Port 4.0 not analogue pin */
        MPC.P40PFS.BIT.ASEL = 0;    
           
        /* Set Port4.0 as a general I/O pin */
        PORT4.PMR.BIT.B0 = 0;              

        /* Configure wait() mode based upon clock_mode */
        if ((SW_STBY == clock_mode) || (SW_DP_STBY == clock_mode))
        {
            /* Software Standby Mode with Output Ports Enabled */
            /* Transition to Standby on wait() instruction */    
            SYSTEM.SBYCR.WORD = 0xC000;

            /* Flash HOCO Sofware Standby Control Register
               Cut power to on-chip ROM in software standby mode */
            SYSTEM.FHSSBYCR.BIT.SOFTCUT0 = 1;
        
            /* Cut power to HOCO in software standby mode.*/
            SYSTEM.FHSSBYCR.BIT.SOFTCUT1 = 1;
        
            /* Cut power to the voltage detection circuit (LVD) and enable
               power consumption reduction function by the power-on reset
               circuit (POR) in software standby mode. */
            SYSTEM.FHSSBYCR.BIT.SOFTCUT2 = 1;
        
            if (SW_DP_STBY == clock_mode)
            {
                /* Transition to deep sw standby mode upon WAIT instruction
                   execution. Do not retain IO Port values. LVD operates at 
                   deep sw standby mode and POR operates in low power 
                   consumption operation mode */
                SYSTEM.DPSBYCR.BYTE = 0x83;   
                    
                /* Disable DMA transfer */
                DMAC0.DMCNT.BIT.DTE = 0;   
                     
                /* DMA Controller activation is disabled */
                DMAC.DMAST.BIT.DMST = 0;    
                
                /* Allow time for settings to propagate before continuing */
                Settle(5000);
            }
        }
        else
        {
            /* Sleep Mode with Output Ports Enabled */
            /* Transition to Sleep mode on wait() instruction */
            SYSTEM.SBYCR.WORD = 0x4000;                                             
        }   
    }
}
/******************************************************************************
* End of function ConfigurePowerTests
******************************************************************************/

/******************************************************************************
* Outline 		: FlashLEDValue  
* Description 	: Flashes LEDs according to arguments
* Argument  	: uint8_t LEDs - Mask for which LEDs
*                                b2: LED2, b1: LED1, b0: LED0
*               : uint8_t repeat - How many times to flash
* Return value  : none
******************************************************************************/
void FlashLEDValue( uint8_t value, uint8_t repeat )
{
    /* Use argument to determine number of times to flash LEDs */
    while( repeat-- )
    {
        /* Switch LEDs Off */
        LED0_OFF;
        LED1_OFF;
        LED2_OFF;
        
        /* Delay loop for keeping LEDs Off */
        Settle(2000);

        /* Switch on LEDs specified by LEDs bitfield argument */
        LED0 = (uint8_t) !(value & LED0Mask);
        LED1 = (uint8_t) !(value & LED1Mask);
        LED2 = (uint8_t) !(value & LED2Mask);
        
        /* Delay loop for keeping LEDs On*/
        Settle(2000);
    };
    
}
/******************************************************************************
* End of function FlashLEDValue
******************************************************************************/

/******************************************************************************
* Outline 		: Settle  
* Description 	: Provides delay loop for debounce and settle times
* Argument  	: uint16_t Loops
* Return value  : none
******************************************************************************/
void Settle( uint16_t loops )
{
    /* Delay loop */
    for(; loops; loops--)
    {
        nop();
    }    
}
/******************************************************************************
* End of function Settle
******************************************************************************/
/* End of file */
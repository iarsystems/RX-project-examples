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
* File Name    : r_bsp_config_reference.c
* Device(s)    : RX62T
* Description  : The file r_bsp_config.h is used to configure your BSP. r_bsp_config.h should be included
*                somewhere in your package so that the r_bsp code has access to it. This file (r_bsp_config_reference.h)
*                is just a reference file that the user can use to make their own r_bsp_config.h file.
************************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 12.03.2012 1.00    First Release        
*         : 03.05.2012 1.10    Added USER_BOOT_ENABLE option.
*         : 26.06.2012 1.15    Added MCU product part number section. Added option for ROM Code Protect. Added callback 
*                              function options for exception interrupts.
*         : 16.07.2012 1.20    Fixed bug with exception callback function names.
*                              macro options.
*         : 09.08.2012 1.30    Added IO_LIB_ENABLE macro.
***********************************************************************************************************************/
#ifndef R_BSP_CONFIG_REF_HEADER_FILE
#define R_BSP_CONFIG_REF_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* Enter the product part number for your MCU. This information will be used to obtain information about your MCU such 
   as package and memory size. 
   To help parse this information, the part number will be defined using multiple macros.
   R 5 F 56 2T A B D FH
   | | | |  |  | | | |  Macro Name              Description
   | | | |  |  | | | |__MCU_PART_PACKAGE      = Package type, number of pins, and pin pitch
   | | | |  |  | | |____not used              = Products with wide temperature range
   | | | |  |  | |______MCU_PART_CAN_INCLUDED = CAN included/not included
   | | | |  |  |________MCU_PART_MEMORY_SIZE  = ROM, RAM, and Data Flash Capacity
   | | | |  |___________MCU_PART_GROUP        = Group name  
   | | | |______________MCU_PART_SERIES       = Series name
   | | |________________MCU_PART_MEMORY_TYPE  = Type of memory (Flash, ROMless)
   | |__________________not used              = Renesas MCU
   |____________________not used              = Renesas semiconductor product. 
   */

/* Package type. Set the macro definition based on values below:
   Character(s) = Value for macro = Package Type/Number of Pins/Pin Pitch
   FH           = 0x0             = LQFP/112/0.65
   FP           = 0x1             = LQFP/100/0.50
   FF           = 0x2             = LQFP/80/0.65
   FM           = 0x3             = LQFP/64/0.50
*/
#define MCU_PART_PACKAGE        (0x1)

/* Whether CAN is included or not. 
   Character(s) = Value for macro = Description
   A            = true            = 5V MCU, CAN x 1 channel
   B            = true            = 3V MCU, CAN x 1 channel
   C            = false           = 5V MCU, No CAN
   D            = false           = 3V MCU, No CAN
*/
#define MCU_PART_CAN_INCLUDED   (true)

/* ROM, RAM, and Data Flash Capacity. 
   Character(s) = Value for macro = ROM Size/Ram Size/Data Flash Size
   A            = 0xA             = 256KB/16KB/32KB
   7            = 0x7             = 128KB/8KB/8KB
   6            = 0x6             = 64KB/8KB/8KB
*/
#define MCU_PART_MEMORY_SIZE    (0xA)

/* Group name. 
   Character(s) = Value for macro = Description
   2T           = 0x0             = RX62T Group
*/
#define MCU_PART_GROUP          (0x0)

/* Series name. 
   Character(s) = Value for macro = Description
   56           = 0x0             = RX600 Series
*/  
#define MCU_PART_SERIES         (0x0)

/* Memory type. 
   Character(s) = Value for macro = Description
   F            = 0x0             = Flash memory version
*/
#define MCU_PART_MEMORY_TYPE    (0x0)

/* The 'BSP_DECLARE_STACK' macro is checked so that the stack is only declared in one place (resetprg.c). Every time a 
   '#pragma stacksize' is encountered, the stack size is increased. This prevents multiplication of stack size. */
#if defined(BSP_DECLARE_STACK)
/* User Stack size in bytes. The Renesas RX toolchain sets the stack size using the #pragma stacksize directive. */
#pragma stacksize su=0x400
/* Interrupt Stack size in bytes. The Renesas RX toolchain sets the stack size using the #pragma stacksize directive. */
#pragma stacksize si=0x400
#endif

/* Heap size in bytes. */
#define HEAP_BYTES              (0x400)

/* After reset MCU will operate in Supervisor mode. To switch to User mode, set this macro to '1'. For more information
   on the differences between these 2 modes see the CPU >> Processor Mode section of your MCU's hardware manual.
   0 = Stay in Supervisor mode.
   1 = Switch to User mode.
*/
#define RUN_IN_USER_MODE        (0)

/* When using User Boot Mode the reset vector is fetched from a different address than Single-Chip Mode. This option
   puts the reset address in the User Boot Reset Vector so that it will be fetched correctly.
   0 = Single-Chip or USB Boot Mode (Nothing is placed in User Boot Reset Vector address)
   1 = User Boot Mode
*/
#define USER_BOOT_ENABLE        (0)

/* Set your desired ID code. NOTE, leave at the default (all 0xFF's) if you do not wish to use an ID code. If you set 
   this value and program it into the MCU then you will need to remember the ID code because the debugger will ask for 
   it when trying to connect. Note that the E1/E20 will ignore the ID code when programming the MCU during debugging.
   If you set this value and then forget it then you can clear the ID code by connecting up in serial boot mode using 
   FDT. The ID Code is 16 bytes long. The macro below define the ID Code in 4-byte sections. */
/* Lowest 4-byte section, address 0xFFFFFFA0. From MSB to LSB: Control Code, ID code 1, ID code 2, ID code 3. */
#define ID_CODE_LONG_1          (0xFFFFFFFF)
/* 2nd ID Code section, address 0xFFFFFFA4. From MSB to LSB: ID code 4, ID code 5, ID code 6, ID code 7. */
#define ID_CODE_LONG_2          (0xFFFFFFFF)
/* 3rd ID Code section, address 0xFFFFFFA8. From MSB to LSB: ID code 8, ID code 9, ID code 10, ID code 11. */
#define ID_CODE_LONG_3          (0xFFFFFFFF)
/* 4th ID Code section, address 0xFFFFFFAC. From MSB to LSB: ID code 12, ID code 13, ID code 14, ID code 15. */
#define ID_CODE_LONG_4          (0xFFFFFFFF)

/* This macro lets other modules no if a RTOS is being used.
   0 = RTOS is not used. 
   1 = RTOS is used.
*/
#define RTOS_USED               (0)

/* Clock configuration options.
   The input clock frequency is specified and then the system clocks are set by specifying the multipliers used. The
   multiplier settings are used to set the clock registers in resetprg.c. If a 12M.5Hz clock is used and the 
   ICLK is 100MHz, PCLK is 50MHz, and BCLK is 12.5MHz then the settings would be:
   XTAL_HZ = 12500000
   ICK_MUL = 8       : (XTAL_HZ * ICK_MUL) = 100MHz
   PCK_MUL = 4       : (XTAL_HZ * PCK_MUL) = 50MHz
   BCK_MUL = 1       : (XTAL_HZ * BCK_MUL) = 12.5MHz
*/
/* XTAL - Input clock frequency in Hz */
#define XTAL_HZ                 (12500000)
/* System Clock Multiplier (ICK).
   Available Multipliers: x1, x2, x4, x8. */
#define ICK_MUL                 (8)
/* Peripheral Clock Multiplier (PCK).
   Available Multipliers: x1, x2, x4. */
#define PCK_MUL                 (4)

/* Below are callback functions that can be used for detecting MCU exceptions, undefined interrupt sources, and 
   bus errors. If the user wishes to be alerted of these events then they will need to define the macro as a 
   function to be called when the event occurs. For example, if the user wanted the function 
   excep_undefined_instr_isr() to be called when an undefined interrupt source ISR is triggered then they would
   do the following:
   #define UNDEFINED_INT_ISR_CALLBACK   undefined_interrupt_cb
   If the user does not wish to be alerted of these events then they should comment out the macros.
   
   NOTE: When a callback function is called it will be called from within a ISR. This means that the function
         will essentially be an interrupt and will hold off other interrupts that occur in the system while it
         is executing. For this reason, it is recommended to keep these callback functions short as to not
         decrease the real-time response of your system.
*/
/* Callback for Supervisor Instruction Violation Exception. */
//#define EXCEP_SUPERVISOR_ISR_CALLBACK           supervisor_instr_cb

/* Callback for Undefined Instruction Exception. */
//#define EXCEP_UNDEFINED_INSTR_ISR_CALLBACK      undefined_instr_cb

/* Callback for Floating Point Exception. */
//#define EXCEP_FPU_ISR_CALLBACK                  floating_point_cb

/* Callback for Non-maskable Interrupt. */
//#define NMI_ISR_CALLBACK                        nmi_cb

/* Callback for all undefined interrupt vectors. User can set a breakpoint in this function to determine which source
   is creating unwanted interrupts. */
//#define UNDEFINED_INT_ISR_CALLBACK              undefined_interrupt_cb

/* Callback for Bus Error Interrupt. */
//#define BUS_ERROR_ISR_CALLBACK                  bus_error_cb

/* ROM Code Protection is a feature that can prohibit PROM programmers from reading or writing flash memory.
   There are 3 different options that can be chosen:
   0    = ROM code protection 1 = Both reading and writing to the user area and user boot area are prohibited.
   1    = ROM code protection 2 = Reading from the user area and user boot area are prohibited.
   Else = Protection disabled   = Reading and writing are allowed.
*/
#define ROM_CODE_PROTECT_VALUE  (0xFFFFFFFF)

/* Initializes C input & output library functions.
   0 = Disable I/O library initialization in resetprg.c. If you are not using stdio then use this value.
   1 = Enable I/O library initialization in resetprg.c. This is default and needed if you are using stdio. */
#define IO_LIB_ENABLE           (1)

#endif /* R_BSP_CONFIG_REF_HEADER_FILE */




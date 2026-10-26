/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************
* File Name     : Flash_API_RX600_UserConfig.c
* Version     : 2.0
* Device      : RX600 Series
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSKRX62N, RSKRX610, YRDKRX62N
* Description  : Flash programming for the RX600 Group. This file has options
*                to let the user customize parts of the Flash API.
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 21.12.2009 1.00    First Release
*         : 13.01.2010 1.10    Made function names and variables RAPI compliant
*         : 11.02.2010 1.20    Fixed other RAPI issues and fixed I flag issue
*         : 29.04.2010 1.30    Added support for RX621/N Group. Moved most
*                              device specific data to header file.
*         : 26.05.2010 1.40    Added support for RX62T Group
*         : 28.07.2010 1.41    Fixed bug when performing a blank check on an
*                              entire data flash block.  Also declared 
*                              functions not in the API as 'static'.
*      : 23.08.2010 1.42    Updated source to raise the coding standard, to
*                 meet GSCE & RSK standards.
*         : 15.02.2011 1.43    Fixed bug in blank check routine when handling
*                              input arguments and moved _Flash_Init() to
*                              _Enter_PE_Mode() function.
*         : 21.04.2011 2.00    Added BGO capabilities for data flash. Made 
*                              some more changes to fit coding standard. Added
*                              ability to do ROM to ROM or DF to DF transfers.
*                              Added the ability to use the lock bit feature
*                              on ROM blocks.  Added BGO capabilities for
*                              ROM operations.
******************************************************************************/

#ifndef _FLASH_API_USER_H
#define _FLASH_API_USER_H

/******************************************************************************
DEFINES
******************************************************************************/

/******************************************************************************
 SPECIFY YOUR DEVICE FAMILY              
******************************************************************************/
/* You need to specify which device family you are using since different
   families have different memory layouts which lends to minor differences
   in some registers. */   
//#define RX610       1
/* RX62N and RX621 are the same part except the N has ethernet */
//#define RX62N     1
//#define RX621     1
#define RX62T     1

/******************************************************************************
 SPECIFY YOUR CPU CLOCK SPEED              
******************************************************************************/
/* You need to specify the speed of your System clock (ICLK)
   so that the correct timeout times can be calculated.
   The value should be in MHz.    
   Max System Clock for RX600 is 100MHz, but 48MHz is needed for USB on
   devices that have it so the ICLK will be 96MHz in that case if you are
   using USB. */
#if defined(RX610) || defined(RX62T)
    #define     ICLK_FREQUENCY  100
#elif defined(RX62N) || defined(RX621)
    #define     ICLK_FREQUENCY  96  
#endif

/******************************************************************************
 SPECIFY YOUR PERIPHERAL CLOCK SPEED              
******************************************************************************/
/* You need to specify the speed of your Peripheral clock (PCLK)
   so that the correct programming speed can be calculated
   for communicating to the flash controller
   The value should be in MHz.
   Max Peripheral Clock for RX600 is 50MHz, but USB needs 48MHz */
#if defined(RX610) || defined(RX62T)
    #define PCLK_FREQUENCY 50  
#elif defined(RX62N) || defined(RX621)
    #define PCLK_FREQUENCY 48  
#endif 

/******************************************************************************
 ENABLE ROM PROGRAMMING              
******************************************************************************/
/* When uncommented, code is included to program the User ROM program area of
   Flash. Since this code must be executed from within RAM, the sections
   'PFRAM'(ROM) and 'RPFRAM'(RAM) must be added to the linker settings. Also
   the linker option '-rom=PFRAM=RPFRAM' must be added. Finally, the 
   initialization of the 'RPFRAM' must be added to the 'dbsct.c' file as such:
   { __sectop("PFRAM"), __secend("PFRAM"), __sectop("RPFRAM") } */
#define ENABLE_ROM_PROGRAMMING

/******************************************************************************
 ENABLE ROM to ROM or DF to DF programs              
******************************************************************************/
/* If you are doing ROM to ROM writes or DF to DF writes then this DEFINE
   needs to be uncommented. This is necessary because when programming a ROM
   area the MCU cannot read from ROM and when programming a DF area the MCU
   cannot read from the DF. To get by this, if this DEFINE is uncommented 
   then data that is moving from either ROM to ROM or DF to DF is first 
   buffered in a RAM array, then written. If you are never doing one of these
   operations then you can comment out this DEFINE and save yourself 
   256 bytes of RAM. */
#define FLASH_TO_FLASH   

/******************************************************************************
 ENABLE BGO & NON-BLOCKING DATA FLASH OPERATIONS
******************************************************************************/
/* If this is defined then the flash ready interrupt will be used and 
   FlashAPI routines that deal with the data flash will exit after the
   operation has been started instead of polling for it to finish. */
//#define DATA_FLASH_BGO

/* If using interrupts then you will need to specify a processor
   interrupt priority level for the DF flash routines to run at. After setting 
   this IPL make sure to set any interrupts that access the data flash 
   at a equal or lower value. If you do not set this properly and an
   interrupt occurs that uses the data flash, during a data flash 
   program/erase operation, then a FCU error will occur. Set any interrupts 
   that you want to continue to occur during data flash program/erase 
   operations to a value greater than this number. */
#define DATA_FLASH_OPERATION_PIPL  3

/******************************************************************************
 ENABLE BGO & NON-BLOCKING ROM OPERATIONS
 EXTRA CARE SHOULD BE TAKEN IF YOU ENABLE ROM_BGO. SINCE PROGRAM/ERASE 
 FUNCTIONS WILL BE NON-BLOCKING THE API WILL RETURN BEFORE THE ROM OPERATION
 HAS FINISHED. THIS MEANS THAT THE USER CODE THAT CALLS THE FLASH API 
 FUNCTION MUST BE IN RAM OR EXTERNAL MEMORY. THIS WOULD ALSO MEAN THAT THE
 USER HAS TO RELOCATE THE INTERRUPT VECTOR TABLE TO SOMEWHERE OTHER THAN
 ROM OR DATA FLASH. IF THE USER ATTEMPTS TO ACCESS ROM DURING A ROM PROGRAM/
 ERASE OPERATION THE FLASH CONTROL UNIT WILL THROW AN ERROR.
******************************************************************************/
/* If this is defined then the flash ready interrupt will be used and 
   FlashAPI routines that deal with the on-chip ROM will exit after the
   operation has been started instead of polling for it to finish. */
//#define ROM_BGO

/* If using interrupts then you will need to specify a processor
   interrupt priority level for the flash routines to run at. After setting 
   this IPL make sure to set any interrupts that access ROM
   at a equal or lower value. If you do not set this properly and an
   interrupt occurs that access ROM, during a ROM program/erase operation,
   then a FCU error will occur. Set any interrupts that you want to
   continue to occur during ROM program/erase operations to a value
   greater than this number. */
#define ROM_OPERATION_PIPL  5

/******************************************************************************
 DO NOT USE LOCK BIT PROTECTION              
******************************************************************************/
/* Each erasure block has a corresponding lock bit that can be used to 
   protect that block from being programmed/erased after the lock bit is
   set. The use of lock bits can be used or ignored. If the #define below
   is uncommented then lock bits will be ignored and programs/erases to a
   block will not be limited. This only applies to ROM since the DF does
   not have lock bits. */
//#define IGNORE_LOCK_BITS

#endif

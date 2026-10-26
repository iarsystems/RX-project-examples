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
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.  */
/******************************************************************************
* File Name    : r_Flash_API_RX200_UserConfig.c
* Version      : 1.00
* Device       : R5F52206
* Tool Chain   : RX Family C Compiler 1.2.1.0
* H/W Platform : RSKRX220
* Description  : Flash programming for the RX200 Group. This file has options
*                to let the user customize parts of the Flash API.
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 5.10.2011 1.00    First Release. Evolution from RX600 v2.1 Flash API
*         : 8.15.2012 1.01    Updated ROM_SIZE to the sizes supported by RX210
******************************************************************************/

#ifndef _FLASH_API_USER_H
#define _FLASH_API_USER_H

/******************************************************************************
DEFINES
******************************************************************************/

/******************************************************************************
 SPECIFY YOUR DEVICE FAMILY              
******************************************************************************/
#define RX210      1
/******************************************************************************
 SPECIFY THE SIZE OF USER ROM IN BYTES              
******************************************************************************/

//#define ROM_SIZE            524288      //512KB
//#define ROM_SIZE            393216     //384KB
//#define ROM_SIZE            262144     //256KB
#define ROM_SIZE            131072     //128KB

/******************************************************************************
 SPECIFY THE SIZE OF THE DATA FLASH AREA IN BYTES              
******************************************************************************/
#define DF_SIZE             8192

/******************************************************************************
 SPECIFY YOUR CPU CLOCK SPEED              
******************************************************************************/
/* You need to specify the speed of your System clock (ICLK)
   so that the correct timeout times can be calculated.
   The value should be in MHz.    
*/
#if defined(RX210)
    #define     ICLK_FREQUENCY  20  
#endif

/******************************************************************************
 SPECIFY YOUR FLASH CLOCK SPEED              
******************************************************************************/
/* You need to specify the speed of your Flash clock (FCLK)
   so that the correct programming speed can be calculated
   for communicating to the flash controller
   The value should be in MHz. Valid values for RX210 are 4 - 32 MHz
*/
#if defined(RX210)
    #define FLASH_CLOCK_FREQUENCY 20  
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
   the RAM needed to buffer one ROM write (RX210 = 128 bytes). */
#define FLASH_TO_FLASH   

/******************************************************************************
 ENABLE BGO & NON-BLOCKING DATA FLASH OPERATIONS
******************************************************************************/
/* If this is defined then the flash ready interrupt will be used and 
   FlashAPI routines that deal with the data flash will exit after the
   operation has been started instead of polling for it to finish. */
#define DATA_FLASH_BGO

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
//#define ROM_BGO       1

/******************************************************************************
 SET IPL OF FLASH READY INTERRUPT
******************************************************************************/
/* If using BGO then the Flash Ready Interrupt will need to have an IPL. */
#define FLASH_READY_IPL     5

/******************************************************************************
 ENABLE OR DISABLE LOCK BIT PROTECTION              
******************************************************************************/
/* Each erasure block has a corresponding lock bit that can be used to 
   protect that block from being programmed/erased after the lock bit is
   set. The use of lock bits can be used or ignored. If the #define below
   is uncommented then lock bits will be ignored and programs/erases to a
   block will not be limited. This only applies to ROM since the DF does
   not have lock bits. */
//#define IGNORE_LOCK_BITS

#endif
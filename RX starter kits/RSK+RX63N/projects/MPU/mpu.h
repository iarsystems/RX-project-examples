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
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : mpu.h
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Declares functions defined in mpu.c
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef MPU_H
#define MPU_H

/* MPU region access permissions. Bitwise OR multiple permissions together to
   apply multiple permissons. */
#define READ_ENABLE                 (0x4u)
#define WRITE_ENABLE                (0x2u)
#define EXEC_ENABLE                 (0x1u)
#define NO_ACCESS                   (0x0u)

/* Definition of the size of the array gMemoryBlock in the MEMORY_BLOCK 
   section (in bytes) */
#define MEMORY_BLOCK_BYTES          (256u)

/* MPU protection region start and end addresses. Note that the last address 
   nibble is truncated, as the MPU resolves 16byte minimum regions. */
#define REGION0_START_ADDR          (0x0000000u)
#define REGION0_END_ADDR            (0x00002FFu)           
#define REGION1_START_ADDR          (0x0000300u)
#define REGION1_END_ADDR            (0x0000380u)
#define REGION2_START_ADDR          (0x0000381u)
#define REGION2_END_ADDR            (0xFFFFFFFu)        

/* Parameters to pass to the SupervisorMode_MPU function */
#define NO_COMMAND                  (0x00u)
#define INITIALISE_MPU              (0xA1u)
#define ENABLE_MEMORY_PROTECTION    (0xB1u)
#define DISABLE_MEMORY_PROTECTION   (0xB2u)

/*******************************************************************************
* Global Function Prototypes
*******************************************************************************/
/* Initialise MPU function prototype */
void Init_MPU(void);
/* Switch poll function prototype */
void SwitchPoll_MPU(void);


/* end of multiple inclusion prevention macro */
#endif

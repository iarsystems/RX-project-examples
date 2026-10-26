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
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name    : resetprg.c
* Version     : 1.00
* Device     : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : YRDKRX62N
* Description   : Defines MCU behaviour after power on or reset
******************************************************************************/
/******************************************************************************
* History     : 22.01.2010 Ver. 1.00 First Release
*******************************************************************************/


/******************************************************************************
User Includes (Project level includes)
******************************************************************************/
/* Interrupt level control function definitions */
#include <machine.h>
/* Section initialisation function definitions */
#include  <_h_c_lib.h>
/* Defines variable types used throughout this file */
#include  "typedefine.h"
/* Defines the MCU stack sections */
#include  "stacksct.h"
/* Defines hardware setup functions used before main program start */
#include   "hwsetup.h"
//#include  <stddef.h>          // Remove the comment when you use errno
//#include   <stdlib.h>          // Remove the comment when you use rand()

/*Standard IO*/
#include "lowsrc.h"

/******************************************************************************
System Definitions & Global Variables
******************************************************************************/
#pragma inline_asm Change_PSW_PM_to_UserMode
static void Change_PSW_PM_to_UserMode(void);

#ifdef __cplusplus
extern "C" {
#endif
void PowerON_Reset_PC(void);
void main(void);
#ifdef __cplusplus
}
#endif

//#ifdef __cplusplus        // Use SIM I/O
//extern "C" {
//#endif
//extern void _INIT_IOLIB(void);
//extern void _CLOSEALL(void);
//#ifdef __cplusplus
//}
//#endif

/* Define processor word */
#define PSW_init  0x00010000
#define FPSW_init 0x00000100

//extern void srand(_UINT);    // Remove the comment when you use rand()
//extern _SBYTE *_s1ptr;        // Remove the comment when you use strtok()
    
//#ifdef __cplusplus        // Use Hardware Setup
//extern "C" {
//#endif
//extern void HardwareSetup(void);
//#ifdef __cplusplus
//}
//#endif
  
//#ifdef __cplusplus      // Remove the comment when you use global class object
//extern "C" {          // Sections C$INIT and C$END will be generated
//#endif
//extern void _CALL_INIT(void);
//extern void _CALL_END(void);
//#ifdef __cplusplus
//}
//#endif


/******************************************************************************
* Outline     : PowerOn_Reset_PC
* Description   : Power on reset program. Entry point after system powers on.
* Argument    : none
* Return value  : none
******************************************************************************/
#pragma entry PowerON_Reset_PC
void PowerON_Reset_PC(void)
{
  /* Configure sections */
#if __RENESAS_VERSION__ >= 0x01010000    
    set_intb((void *)__sectop("C$VECT"));
#else
    set_intb((unsigned long)__sectop("C$VECT"));
#endif    
  set_fpsw(FPSW_init);

  /* Copy initialised data from ROM to RAM */
  _INITSCT();

  #ifndef RELEASE
    _INIT_IOLIB();        // Use Standard I/O (printf)
  #endif

//  errno = 0;          // Remove the comment when you use errno
//  srand((_UINT)1);      // Remove the comment when you use rand()
//  _s1ptr = NULL;        // Remove the comment when you use strtok()
  
  /* Initialise MCU configuration and peripherals */  
  HardwareSetup();      // Use Hardware Setup
    nop();

//  _CALL_INIT();    // Remove the comment when you use global class object

  /* Set processor word */
  set_psw(PSW_init);      // Set Ubit & Ibit for PSW

  /* Enter user mode */
  Change_PSW_PM_to_UserMode();// Change PSW PMbit (SuperVisor->User)

  /* Call the main function */
  main();

//  _CLOSEALL();        // Use SIM I/O
  
//  _CALL_END();    // Remove the comment when you use global class object

  /* Execute a break command */
  brk();
}
/******************************************************************************
End of function PowerOn_ResetPC
******************************************************************************/


/******************************************************************************
* Outline     : Change_PWS_PM_to_UserMode
* Description   : Changes the MCU operating mode from supervisor to user.
* Argument    : none
* Return value  : none
******************************************************************************/
static void Change_PSW_PM_to_UserMode(void)
{
  MVFC   PSW,R1
  OR     #00100000h,R1
  PUSH.L R1
  MVFC   PC,R1
  ADD    #10,R1
  PUSH.L R1
  RTE
  NOP
  NOP
}
/******************************************************************************
End of function 
******************************************************************************/

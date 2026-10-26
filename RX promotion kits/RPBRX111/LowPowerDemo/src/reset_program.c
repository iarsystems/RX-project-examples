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
* File Name     : reset_program.c
* Version       : 1.00
* Device        : R5F51115AxFM
* Tool Chain    : CCRX
* H/W Platform  : RPBRX111
* Description   : Reset program
*******************************************************************************/
/*******************************************************************************
* History       : 12/08/2013  Ver. 1.00 First Release
*******************************************************************************/

#include    <machine.h>
//#include  <stddef.h>                  // Remove the comment when you use errno
//#include  <stdlib.h>                  // Remove the comment when you use rand()
#include    "typedefine.h"      // Define Types

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/

//#ifdef __cplusplus                // Use SIM I/O
//extern "C" {
//#endif
//extern void _INIT_IOLIB(void);
//extern void _CLOSEALL(void);
//#ifdef __cplusplus
//}
//#endif

#define PSW_init  0x00010000    // PSW bit pattern
#define FPSW_init 0x00000000    // FPSW bit base pattern

//extern void srand(_UINT);        // Remove the comment when you use rand()
//extern _SBYTE *_s1ptr;                // Remove the comment when you use strtok()
        
//#ifdef __cplusplus                // Use Hardware Setup
//extern "C" {
//#endif
//extern void HardwareSetup(void);
//#ifdef __cplusplus
//}
//#endif
    
//#ifdef __cplusplus            // Remove the comment when you use global class object
//extern "C" {                    // Sections C$INIT and C$END will be generated
//#endif
//extern void _CALL_INIT(void);
//extern void _CALL_END(void);
//#ifdef __cplusplus
//}
//#endif

int __low_level_init(void)
{ 
//    _INIT_IOLIB();                // Use SIM I/O

//    errno=0;                      // Remove the comment when you use errno
//    srand((_UINT)1);              // Remove the comment when you use rand()
//    _s1ptr=NULL;                  // Remove the comment when you use strtok()

//    _CALL_INIT();                 // Remove the comment when you use global class object

    set_psw(PSW_init);              // Set Ubit & Ibit for PSW
//    chg_pmusr();                  // Remove the comment when you need to change PSW PMbit (SuperVisor->User)

    return 1;
}

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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_usb_pmsc_apl.c
* Version      : 2.00
* Description  : USB Peripheral Mass-Storage Sample Code
*******************************************************************************/
/*******************************************************************************
* History    : DD.MM.YYYY Version Description
*            : 09.11.2012 1.00         First Release
*            : 30.11.2012 2.00
*******************************************************************************/


/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#ifdef  R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#pragma section rom C_smpl
#endif

#ifdef  RX_USB
#endif

#ifdef  RL78USB
#endif

#ifdef  RL78USB_SECTION
#pragma section @@CODE   P_SMP
#pragma section @@BASE   T_SMP
#pragma section @@CNST   C_SMP
#pragma section @@R_INIT D_SMP
#pragma section @@DATA   B_SMP
#pragma section @@INIT   R_SMP
#endif

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition */
#include "r_usb_api.h"                  /* USB API public function header */
#include "hw_resource.h"

#include "r_usb_pmsc_api.h"
#include "r_usb_class_usrcfg.h"
#include "r_usb_cmsc_define.h"          /* Peri Mass Storage Class Driver define */
#include "r_usb_pmsc_define.h"
#include "r_usb_pmsc_extern.h"

#include "r_usb_atapi_driver_config.h"

#if DEBUG_CONSOLE
    #include "string.h"
    #include <stdio.h>
#endif




/******************************************************************************
External variables and functions
******************************************************************************/
void            usb_papl_task_start(void);
void            usb_pmsc_MainLoop(void);
void            usb_pmsc_Open(void);
void            usb_pmsc_Close(void);
void            usb_pmsc_MainTask(void);
void            usb_pmsc_DummyFunction(usb_utr_t *utr_data);
uint8_t         usb_pmsc_media_format( void );
void            usb_pmsc_task_start( void );

void            usb_smpl_set_suspend_flag(uint8_t data);
uint8_t         usb_smpl_get_suspend_flag(void);

extern void     usb_pstd_usbdriver_start(void);
extern void     usb_apl_task_switch(void);
extern void     usb_pmsc_SmpAtapiTask(void);
extern void     usb_cpu_stop_mode(void);



/******************************************************************************
Renesas Peripheral Mass-Storage Sample Code functions
******************************************************************************/


/******************************************************************************
Function Name   : usb_papl_task_start
Description     : Set peripheral application task start conditions. 
Arguments       : none
Return value    : none
******************************************************************************/
void    usb_papl_task_start(void)
{
    #if DEBUG_CONSOLE
        /* _INIT_IOLIB() must be uncommented in resetprg.c for printf() to work with
         * the Debug Console. */
        printf("usb_papl_task_start\n");
    #endif
}   /* eof usb_papl_task_start() */


/******************************************************************************
Function Name   : usb_cstd_task_start
Description     : Start task CSTD; "common" basic USB low level task.
Arguments       : USB_UTR_t *ptr      : 
Return value    : none
******************************************************************************/
void usb_cstd_task_start( void )
{
    usb_pmsc_task_start();      /* Start perhiperal USB driver */
    
    usb_apl_task_switch();      /* Switch task for nonOS */
}   /* eof usb_cstd_task_start() */


/******************************************************************************
Function Name   : usb_pmsc_task_start
Description     : Set peripheral application task start conditions. 
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_task_start( void )
{
    R_usb_pmsc_Registration();     /* Peripheral Application Registration */

    R_usb_pstd_PcdChangeDeviceState(USB_DO_SETHWFUNCTION);

    R_usb_pmsc_driver_start();     /* Peripheral Class Driver Task Start Setting */
    usb_pstd_usbdriver_start();    /* Peripheral USB Driver Start Setting */
    usb_papl_task_start();         /* Peripheral Application Task Start Setting */
}   /* eof usb_pmsc_task_start() */


/******************************************************************************
Function Name   : usb_apl_task_switch
Description     : Task switch loop for nonOS version.
Argument        : none
Return          : none
******************************************************************************/
void usb_apl_task_switch(void)
{
    while( 1 )
    {
        /* Scheduler */
        if( USB_FLGSET == R_usb_cstd_Scheduler() )
        {
            R_usb_pstd_PcdTask();       /* PCD Task */
            usb_pmsc_SmpAtapiTask();    /* Execute ATAPI commands Process */
            usb_pmsc_Task();            /* Peripheral Mass Storage Class Task */
        }

        if ( usb_smpl_get_suspend_flag() == USB_YES )
        {
            usb_cpu_stop_mode();
        }
    }
}   /* eof usb_apl_task_switch() */


/******************************************************************************
Function Name   : usb_pmsc_Open
Description     : Sample Open
Argument        : none
Return          : none
******************************************************************************/
void usb_pmsc_Open(void)
{
}   /* eof usb_pmsc_Open() */


/******************************************************************************
Function Name   : usb_pmsc_Close
Description     : Sample Close
Argument        : none
Return          : none
******************************************************************************/
void usb_pmsc_Close(void)
{
}   /* eof usb_pmsc_Close() */


/******************************************************************************
Function Name   : usb_pmsc_DummyFunction
Description     : dummy function
Arguments       : usb_utr_t *utr_data   ; Not use
Return value    : none
******************************************************************************/
void usb_pmsc_DummyFunction(usb_utr_t *utr_data)
{
}   /* eof usb_pmsc_DummyFunction() */


/******************************************************************************
End  Of File
******************************************************************************/

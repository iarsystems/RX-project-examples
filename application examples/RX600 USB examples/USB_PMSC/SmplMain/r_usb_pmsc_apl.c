/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is  protected
* under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR  A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE  EXPRESSLY
* DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_usb_PMSC_apl.c
* Version      : 1.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Peripheral Mass-Storage Sample Code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2010 0.80    First Release
*         : 30.07.2010 0.90    Updated comments
*         : 02.08.2010 0.91    Updated comments
*         : 29.10.2010 1.00    Mass Production Release
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"     /* Type define */
#include "r_usb_usrconfig.h"
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cmacsystemcall.h"   /* uITRON system call macro */
#include "r_usb_cextern.h"          /* USB-FW global define */
#include "r_usb_ansi.h"
#include "r_usb_control.h"
#include "r_usb_cusb_bitdefine.h"

#include "r_usb_api.h"
#include "r_usb_pmsc_api.h"
#include "r_usb_cmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "hw_resource.h"

/******************************************************************************
External variables and functions
******************************************************************************/
void            usb_pmsc_MainLoop(void);
void            usb_pmsc_Open(void);
void            usb_pmsc_Close(void);
void            usb_pmsc_MainTask(void);

extern void usb_pstd_usbdriver_start( USB_UTR_t *ptr );
extern void R_usb_pmsc_driver_start( USB_UTR_t *ptr );
extern void usb_cpu_LcdDisp(uint8_t position, uint8_t *string);
extern void usb_pmsc_SmpAtapiTask(USB_VP_INT_t stacd);
extern void usb_pmsc_Task(USB_VP_INT_t stacd);

static void usb_apl_task_switch(void);
static void usb_pmsc_task_start( void );

/******************************************************************************
Renesas Peripheral Mass-Storage Sample Code functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_pmsc_pr_apl_title
Description     : Output Title
Argument        : none
Return          : none
******************************************************************************/
void usb_pmsc_pr_apl_title(void)
{
#ifdef USB_LCD_ENABLE
    usb_cpu_TargetLcdClear();
#endif /* USB_LCD_ENABLE */
    USB_PRINTF0("\n");
    USB_PRINTF0("++++++++++++++++++++++++++++++++++++++\n");
    USB_PRINTF0("+   PERI MASS STORAGE CLASS SAMPLE   +\n");
    USB_PRINTF0("+                                    +\n");
    USB_PRINTF0("+     RENESAS ELECTRONICS CORP.      +\n");
    USB_PRINTF0("+     RENESAS SOLUTIONS  CORP.       +\n");
    USB_PRINTF0("++++++++++++++++++++++++++++++++++++++\n");
    USB_PRINTF0("\n");
}   /* eof usb_pmsc_pr_apl_title() */

/******************************************************************************
Function Name   : usb_papl_task_start
Description     : Set peripheral application task start conditions. 
Arguments       : none
Return value    : none
******************************************************************************/
void    usb_papl_task_start( USB_UTR_t *ptr )
{
    R_usb_cstd_SetTaskPri(USB_PFLSH_TSK, USB_PRI_5);
}
/******************************************************************************
End of function usb_papl_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_task_start
Description     : Start task CSTD; "common" basic USB low level task.
Arguments       : USB_UTR_t *ptr      : 
Return value    : none
******************************************************************************/
void usb_cstd_task_start( void )
{
    usb_cstd_IdleTaskStart();   /* Idle Task Start */

    usb_pmsc_task_start();      /* Start Peripheral USB driver */

    usb_apl_task_switch();      /* Switch task for nonOS */
}
/******************************************************************************
End of function usb_cstd_task_start()
******************************************************************************/
/******************************************************************************
Function Name   : usb_pmsc_task_start
Description     : Set peripheral application task start conditions. 
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_task_start( void )
{
    USB_UTR_t   utr;
    USB_UTR_t   *ptr;

#ifdef  USB_LCD_ENABLE
    /* Display Debug LCD */
    usb_cpu_LcdDisp( LCD_POS_U0, "USB PMSC" );
    usb_cpu_LcdDisp( LCD_POS_D0, "Appstart" );
#endif /* USB_LCD_ENABLE */

    ptr = &utr;
    ptr->ip = USB_PERI_USBIP_NUM;           /* IP number(0or1) */
    if( USB_NOUSE_PP != ptr->ip )           /* USB IP number check */
    {
        ptr->ipp = R_usb_cstd_GetUsbIpAdr( ptr->ip );

        R_usb_pmsc_Registration( ptr );     /* Peripheral Application Registration */
        R_usb_pmsc_driver_start( ptr );     /* Peripheral Class Driver Task Start Setting */
        R_usb_pstd_usbdriver_start( ptr );  /* Peripheral USB Driver Start Setting */
        usb_papl_task_start( ptr );         /* Peripheral Application Task Start Setting */

        /* Initialize USB IP */
        R_usb_cstd_UsbIpInit( ptr, USB_PERI_PP );
    }
}
/******************************************************************************
End of function usb_pstd_task_start()
******************************************************************************/

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
        R_usb_cstd_Scheduler();

        if( USB_FLGSET == R_usb_cstd_CheckSchedule() )
        {
#ifdef  USB_PERI_MODE_PP
            /* USB Peripheral control driver Task */
            usb_pstd_PcdTask((USB_VP_INT_t)0);
            /* Execute ATAPI commands Process */
            usb_pmsc_SmpAtapiTask((USB_VP_INT_t)0);
            /* Peripheral Mass Storage Class Task */
            usb_pmsc_Task((USB_VP_INT_t)0);
#endif  /* USB_PERI_MODE_PP */
            /* Idle Task (sleep sample) */
            R_usb_cstd_IdleTask(0);
        }
    }
}
/******************************************************************************
End of function usb_apl_task_switch()
******************************************************************************/

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
End  Of File
******************************************************************************/

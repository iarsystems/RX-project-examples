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
* Copyright (C) 2010(2011,2012) Renesas Electronics Corpration
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : main.c
* Version      : 2.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : main process
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 29.07.2011 0.50    First Release
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>

#include "r_usb_ctypedef.h"     /* Type define */
#include "r_usb_usrconfig.h"
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cmacsystemcall.h"   /* uITRON system call macro */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cextern.h"
#include "hw_resource.h"

#include "r_usb_cusb_bitdefine.h"
/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_usb"


/******************************************************************************
External variables and functions
******************************************************************************/
extern  void usb_cstd_task_start( void );
extern  void usb_pcdc_pr_apl_title(void);
extern  uint8_t  usb_cpu_GetDPSRSTF(void);
extern  void     R_usb_cstd_SetTaskPri(uint8_t tasknum, uint8_t pri);
extern  void     R_usb_pstd_DeviceInformation(USB_UTR_t *ptr, uint16_t *tbl);
extern  void     usb_cpu_GoLpwrDeepStby(USB_UTR_t *ptr);
extern  void     usb_cpu_GoLpwrSleep(USB_UTR_t *ptr);

/* Condition compilation by the difference of the devices */
 #if (USB_CPU_LPW_PP == USB_LPWR_USE_PP)
extern uint16_t usb_gcpu_RemoteProcess;
 #endif /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */

/******************************************************************************
Private global variables and functions
******************************************************************************/
void    usb_cstd_IdleTaskStart(void);
void    usb_cstd_PrintKernelSysdwnInfo(signed long w, signed long er, signed long i1, signed long i2);

/******************************************************************************
Function Name   : usb_cstd_PrintKernelSysdwnInfo
Description     : System down Information print
Arguments       : signed long w         : error kind
                : signed long er        : error code
                : signed long i1        : system down information 1
                : signed long i2        : system down information 2
Return value    : none
******************************************************************************/
void usb_cstd_PrintKernelSysdwnInfo(signed long w, signed long er, signed long i1, signed long i2)
{
    USB_PRINTF0("\nkernel_sysdwn ==========\n");
    USB_PRINTF4("  %ld,%ld,%lx,%lx\n", w, er, i1, i2);
    USB_PRINTF0("========================\n");
}   /* eof usb_cstd_PrintKernelSysdwnInfo() */

/******************************************************************************
Function Name   : usb_cstd_PrAplTitle
Description     : Output Title on LCD or UART
Argument        : none
Return          : none
******************************************************************************/
void usb_cstd_PrAplTitle(void)
{
#ifdef USB_LCD_ENABLE
    usb_cpu_TargetLcdClear();
#endif /* USB_LCD_ENABLE */

    USB_PRINTF0("\n");
    USB_PRINTF0("+++++++++++++++++++++++++++++++++++++\n");
#if USB_FUNCSEL_USBIP0_PP == USB_HOST_PP || USB_FUNCSEL_USBIP1_PP == USB_HOST_PP
    USB_PRINTF0("+   HOST STANDARD FW SAMPLE         +\n");
#endif  /* USB_FUNCSEL_USBIP0_PP == USB_HOST_PP || USB_FUNCSEL_USBIP1_PP == USB_HOST_PP */
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP
    USB_PRINTF0("+   PERI STANDARD FW SAMPLE         +\n");
#endif  /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */
    USB_PRINTF0("+                                   +\n");
    USB_PRINTF0("+     RENESAS ELECTRONICS CORP.     +\n");
    USB_PRINTF0("+     RENESAS SOLUTIONS  CORP.      +\n");
    USB_PRINTF0("+++++++++++++++++++++++++++++++++++++\n");
    USB_PRINTF1("       DATE [%s] \n", __DATE__);
    USB_PRINTF1("       TIME [%s] \n", __TIME__);
}
/******************************************************************************
End of function usb_cstd_PrAplTitle()
******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_main_task
Description     : main task process
Arguments       : USB_VP_INT stacd      : task start code(not use)
Return value    : none
******************************************************************************/
void usb_cstd_main_task(USB_VP_INT stacd)
{
    usb_cpu_target_init();

    usb_pcdc_pr_apl_title();

    usb_cstd_task_start();
}
/******************************************************************************
End of function usb_cstd_main_task()
******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_IdleTaskStart
Description     : Idle Task Start process
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cstd_IdleTaskStart(void)
{
/* Condition compilation by the difference of the devices */
#if (USB_CPU_LPW_PP == USB_LPWR_USE_PP)
    R_usb_cstd_SetTaskPri(USB_IDL_TSK, USB_PRI_6);
    USB_SND_MSG(USB_IDL_MBX, 0);
#endif /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */
}
/******************************************************************************
End of function usb_cstd_IdleTaskStart()
******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_IdleTask
Description     : Idle Task (sleep sample)
Arguments       : USB_VP_INT stacd      : task start code(not use)
Return value    : none
******************************************************************************/
void usb_cstd_IdleTask(USB_VP_INT stacd)
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP == USB_HOST_PP || USB_FUNCSEL_USBIP1_PP == USB_HOST_PP
    /* nothing */
#else /* USB_FUNCSEL_USBIP0_PP == USB_HOST_PP || USB_FUNCSEL_USBIP1_PP == USB_HOST_PP */
/* Condition compilation by the difference of the devices */
 #if (USB_CPU_LPW_PP == USB_LPWR_USE_PP)
    void        usb_cpu_GoLpwrSleepMode(void);
    void        usb_cpu_GoDeepStbyMode(ptr);
    uint16_t    res[8], sts0;
    uint8_t     stby;
    USB_UTR_t utr0, *ptr0;

    USB_UTR_t   *mess;
    USB_ER_t    err;
    uint16_t    usb_idle;

    usb_idle = USB_OFF;

    ptr0 = (USB_UTR_t *)&utr0;

    ptr0->ip  = USB_PERI_USBIP_NUM;
    ptr0->ipp = usb_cstd_GetUsbIpAdr( ptr0->ip );

    /* Idle Task message receive (port0) */
    err = USB_TRCV_MSG(USB_IDL_MBX, (USB_MSG_t**)&mess, (USB_TM_t)0);
    if( (err != USB_E_OK) && (err != USB_E_TMOUT) )
    {
        /* USB_PRINTF1("### IdleTask rcv_msg error %ld\n", err); */
    }
    else
    {
        /* Send message to IDL_TSK */
        usb_idle = USB_ON;
        USB_SND_MSG(USB_IDL_MBX, 0);
    }

    if( usb_idle == USB_ON )
    {

/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP
        R_usb_pstd_DeviceInformation(ptr0, (uint16_t *)res);
#endif /* USB_FUNCSEL_USBIP0_PP == USB_PERI_PP || USB_FUNCSEL_USBIP1_PP == USB_PERI_PP */
/* Condition compilation by the difference of USB function */

        sts0 = res[0];
        if( (sts0 & USB_VBSTS) == 0 )
        {
            sts0 = USB_DETACHED;    /* Port0 detach */
        }
        else if( (sts0 & USB_DS_SPD_CNFG) == USB_DS_SPD_CNFG )
        {
            sts0 = USB_SUSPENDED;   /* Port0 suspend */
        }
        else
        {
            sts0 = USB_PORTOFF;
        }

        if( sts0 == USB_DETACHED )
        {
            /* Detach */
            usb_gcpu_RemoteProcess = USB_OFF;
            /* Get Deep Software Standby Reset Flag */
            stby = usb_cpu_GetDPSRSTF();
            if( stby == 0 )
            {
                /* Deep Standby shift */
                usb_cpu_GoLpwrDeepStby(ptr0);
            }
        }
        else if( sts0 == USB_SUSPENDED )
        {
            /* Suspend */
            usb_cpu_GoLpwrSleep(ptr0);
        }
        else
        {
            usb_gcpu_RemoteProcess = USB_OFF;
        }
    }
 #else /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */
 #endif /* (USB_CPU_LPW_PP == USB_LPWR_USE_PP) */
#endif /* USB_FUNCSEL_USBIP0_PP == USB_HOST_PP || USB_FUNCSEL_USBIP1_PP == USB_HOST_PP */
}
/******************************************************************************
End of function usb_cstd_IdleTask()
******************************************************************************/

/******************************************************************************
Function Name   : R_usb_cstd_IdleTask
Description     : Call Idle Task (sleep sample)
Arguments       : USB_VP_INT stacd      : task start code(not use)
Return value    : none
******************************************************************************/
void R_usb_cstd_IdleTask(USB_VP_INT stacd)
{
    usb_cstd_IdleTask( stacd );
}
/******************************************************************************
End of function R_usb_cstd_IdleTask()
******************************************************************************/

/******************************************************************************
End  Of File
******************************************************************************/

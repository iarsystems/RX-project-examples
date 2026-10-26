/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is  protected
* under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING
* BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
*    By using this software, you agree to the additional terms and conditions
* found by accessing the following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_usb_hmsc_apl.c
* Version      : 1.10
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Common to None and uITRON
* H/W Platform : Independent
* Description  : USB Host Sample Code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2010 0.80    First Release
*         : 30.07.2010 0.90    Updated comments
*         : 04.08.2010 0.91    Updated comments
*         : 29.10.2010 1.00    Mass Production Release
*         : 01.06.2011 1.10    Version 1.10 Release
*         : 05.14.2012 2.00    Comments tranlation (REA). Changed tabs to 4 spa-
*                              ces. SET EDITOR TO INSERT SPACES WHEN TAB PRESSED.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_usrconfig.h"        /* USB-H/W register set (user define) */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacsystemcall.h"   /* uITRON system call macro */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cextern.h"          /* USB-FW grobal define */
#include <stdlib.h>                 /* HMSC Sample use */
#include <stdio.h>                  /* HMSC Sample use */
#include "r_usb_hmsc_define.h"      /* HMSC define */
#include "r_TinyFAT.h"              /* TFAT header */
#include "r_usb_ansi.h"

#include "r_usb_hmsc_extern.h"
#include "hw_resource.h"
#include "r_usb_api.h"
#include "r_usb_hmsc_api.h"

/*****************************************************************************
Macro definitions
******************************************************************************/
#define USB_KEY_WAIT1               (uint16_t)0x1006
#define USB_DEMO_START              (uint16_t)0x1007
#define USB_KEY_WAIT2               (uint16_t)0x1008
#define USB_APL_CLR                 (uint16_t)0x1009
/* SW */
#define USB_NO_PUSH                 0u
#define USB_SW1_PUSH                1u
#define USB_SW2_PUSH                2u
#define USB_SW3_PUSH                4u
/* Demonstration */
#define USB_TRANSFER_START          0u
#define USB_TRANSFER_STOP           1u

/******************************************************************************
Section    <Section Definition> , "Data Sections"
******************************************************************************/
/* Condition compilation by the difference of hardware reference */
#ifdef USB_SDRAM_USE_PP
    #pragma section="_sdram"
#else   /* USB_SDRAM_USE_PP */
    #pragma section="_apl"
#endif  /* USB_SDRAM_USE_PP */

/******************************************************************************
Private global variables and functions
******************************************************************************/
uint16_t    usb_ghmsc_SmpAplProcess;
uint16_t    usb_shmsc_DriveOpenSeq = USB_SEQ_0;
uint16_t    usb_shmsc_demo_state = USB_TRANSFER_STOP;
uint8_t     usb_gBuf[512];
FATFS       usb_gFatfs;
uint16_t    usb_sLedwork1 = 0u;
uint16_t    usb_sLedwork2 = 1u;
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
int16_t     usb_hmsc_fn;
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

void        usb_hmsc_PrAplTitle(void);
void        usb_hmsc_MainTask(USB_VP_INT_t);
void        usb_hmsc_apl_init(USB_UTR_t *ptr);
void        R_usb_hmsc_AplClear(USB_UTR_t *ptr);
void        usb_shmsc_DemoStateChange(void);
void        usb_hmsc_SmplMessageSend(USB_UTR_t *ptr, uint16_t msginfo);
void        usb_hmsc_DummyFunction(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);
static void usb_hapl_registration(USB_UTR_t *ptr);

void        usb_cstd_task_start( void );
void        usb_hapl_task_start( USB_UTR_t *ptr );
static void usb_apl_task_switch(void);

void        usb_hmsc_SampleAplTask(void);
void        usb_hmsc_SampleAplSpecifiedPath(USB_CLSINFO_t *mess);

extern int16_t      usb_ghmsc_SmplClassSeq;
extern uint8_t      *usb_hmsc_SmplDeviceTable;
extern uint8_t      *usb_hmsc_SmplConfigTable;
extern uint8_t      *usb_hmsc_SmplInterfaceTable;
extern uint16_t     usb_hmsc_SmplDevaddr;
extern uint16_t     usb_hmsc_SmplSpeed;
extern int16_t      usb_ghmsc_SmplClassSeqWait;

uint16_t            usb_ghmsc_open = USB_NG;

const uint16_t usb_gapl_devicetpl[] =
{
    /* Number of tpl table */
    4,
    /* Reserved */
    0,
    /* Vendor ID  : no-check */
    USB_NOVENDOR,
    /* Product ID : no-check */
    USB_NOPRODUCT,
};

/******************************************************************************
External variables and functions
******************************************************************************/
extern  uint16_t            usb_ghmsc_RootDevaddr;
extern  USB_UTR_t           tfat_ptr;

extern void     usb_cpu_LcdDisp(uint8_t position, uint8_t *string);
extern void     usb_cpu_LedSet(uint8_t data);
extern void     usb_cpu_LedSetBit(uint8_t bit, uint8_t data);
extern uint16_t usb_cpu_GetKeyNo(void);
static void     usb_hmsc_task_start( void );
extern void usb_hmsc_hub_registration(USB_UTR_t *ptr);

/*****************************************************************************
Enumerated Types
******************************************************************************/

/* Host Std Sample application message command */
typedef enum
{
    USB_HMSC_OPEN = 1,
    USB_HMSC_DETACH,
    USB_HMSC_DEMO
}
USB_HMSC_ANSI_COMMAND;

/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_apl"

/******************************************************************************
Renesas Host MSC Sample Code functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_task_start
Description     : Start task processing.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cstd_task_start( void )
{
    /* Start the Idle task. */
    usb_cstd_IdleTaskStart();

    /* Start host-related USB drivers. */
    usb_hmsc_task_start();

    /* Non-OS task switching. */
    usb_apl_task_switch ();
}   /* eof usb_cstd_task_start() */

/******************************************************************************
Function Name   : usb_hmsc_task_start
Description     : Start task processing.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hmsc_task_start( void )
{
    /* The USB Communication Structure allocation for this MSC. */
    USB_UTR_t   utr;
    /* Pointer to the USB Communication Structure above. */
    USB_UTR_t*  ptr = &utr;

#ifdef  USB_LCD_ENABLE
    /* Display Debug LCD */
    usb_cpu_LcdDisp( LCD_POS_U0, "USB HMSC" );
    usb_cpu_LcdDisp( LCD_POS_D0, "Appstart" );
#endif /* USB_LCD_ENABLE */

    /* Determine which port is host. */
    ptr->ip = USB_HOST_USBIP_NUM;

    /* If port is used, register the driver etc. */
    if (USB_NOUSE_PP != ptr->ip )
    {
        ptr->ipp = R_usb_cstd_GetUsbIpAdr( ptr->ip ); /* Get the USB IP base address. */

        tfat_ptr.ip     = ptr->ip;          /* Set up USB IP number for TFAT system. */
        tfat_ptr.ipp    = ptr->ipp;         /* Set up USB IP base address for TFAT. */

        /* Set-up tasks and drivers to use the allocated Comm. structure (for MSC) above. */
        R_usb_hstd_usbdriver_start( ptr );  /* Host driver. */

        usb_hapl_registration( ptr );       /* Host application registration. */
        usb_hmsc_hub_registration(ptr);     /* Hub registration. */
        R_usb_hmsc_driver_start( ptr );     /* Host class driver. */
        usb_hapl_task_start( ptr );         /* Host application task. */

        /* Finally, init the HW with the Comm. struct. */
        R_usb_cstd_UsbIpInit( ptr, USB_HOST_PP );
    }
}   /* eof usb_hmsc_task_start() */

/******************************************************************************
Function Name   : usb_apl_task_switch
Description     : Non-OS task switch loop.
Argument        : none
Return          : none
******************************************************************************/
void usb_apl_task_switch(void)
{
    while( 1 )
    {
        /* Scheduler */
        R_usb_cstd_Scheduler();

        /* Check for any task processing requests flags. */
        if( USB_FLGSET == R_usb_cstd_CheckSchedule() )
        {
            R_usb_hstd_HcdTask((USB_VP_INT)0);          /* HCD Task */
            R_usb_hstd_MgrTask((USB_VP_INT)0);          /* MGR Task */
            R_usb_hhub_Task((USB_VP_INT)0);             /* HUB Task */

            usb_hmsc_Task();                        /* HMSC Task */
            usb_hmsc_StrgDriveTask();               /* HSTRG Task */
            usb_hmsc_SampleAplTask();               /* HMSC Sample Task  */
        }
        else
        {
            /* Idle task - background "sleep". */
            R_usb_cstd_IdleTask(0);
        }
    }
}
/******************************************************************************
End of function usb_apl_task_switch()
******************************************************************************/

/******************************************************************************
Function Name   : usb_hapl_task_start
Description     : Start up host USB application task.
Arguments       : USB_UTR_t *ptr        : The app's USB Comm. Structure.
Return value    : none
******************************************************************************/
void    usb_hapl_task_start( USB_UTR_t *ptr )
{
    /* Set task priority of HMSC sample application. */
    R_usb_cstd_SetTaskPri(USB_HMSCSMP_TSK, USB_PRI_4);

    /* Clear application using fresh USB Comm. Structure. */
    usb_hmsc_apl_init(ptr);
}
/******************************************************************************
End of function usb_hapl_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_hapl_registration
Description     : Callback registration.
Arguments       : USB_UTR_t *ptr        : The app's USB Comm. Structure.
Return value    : none
******************************************************************************/
void usb_hapl_registration(USB_UTR_t *ptr)
{
    USB_HCDREG_t    driver;

    /* Driver registration */
    driver.ifclass      = (uint16_t)USB_IFCLS_MAS;      /* Use Interface class for MSC. */
    driver.tpl          = (uint16_t*)&usb_gapl_devicetpl;   /* Target peripheral list. */
    driver.pipetbl      = (uint16_t*)&usb_ghmsc_DefEpTbl;   /* Pipe def. table address. */
    driver.classinit    = &R_usb_hmsc_Initialized;      /* Driver init. */
    driver.classcheck   = &R_usb_hmsc_ClassCheck;       /* Driver check. */
    driver.devconfig    = &R_usb_hmsc_DriveOpen;        /* Callback when device is configured. */
    driver.devdetach    = &R_usb_hmsc_DriveClose;       /* Callback when device is detached. */
    driver.devsuspend   = &usb_hmsc_DummyFunction;      /* Callback when device is suspended. */
    driver.devresume    = &usb_hmsc_DummyFunction;      /* Callback when device is resumed. */

    /* Host MSC class driver registration. */
    R_usb_hstd_DriverRegistration(ptr, &driver);
}   /* eof usb_hapl_registration() */

/******************************************************************************
Function Name   : usb_hmsc_PrAplTitle
Description     : Output Title
Argument        : none
Return          : none
******************************************************************************/
void usb_hmsc_PrAplTitle(void)
{
    /* Lcd clear */
    usb_cpu_TargetLcdClear();

    /* Print title */
    USB_PRINTF0("\n");
    USB_PRINTF0("+++++++++++++++++++++++++++++++++++++\n");
    USB_PRINTF0("+  HOST MASS STRAGE CLASS FW SAMPLE +\n");
    USB_PRINTF0("+                                   +\n");
    USB_PRINTF0("+     RENESAS ELECTRONICS CORP.     +\n");
    USB_PRINTF0("+     RENESAS SOLUTIONS  CORP.      +\n");
    USB_PRINTF0("+++++++++++++++++++++++++++++++++++++\n");
    USB_PRINTF1("       DATE [%s] \n", __DATE__);
    USB_PRINTF1("       TIME [%s] \n", __TIME__);
}   /* eof usb_hmsc_PrAplTitle() */

/******************************************************************************
Function Name   : usb_hmsc_apl_init
Description     : Clear application.
Arguments       : USB_UTR_t *ptr        : The app's USB Comm. Structure.
Return          : none
******************************************************************************/
void usb_hmsc_apl_init(USB_UTR_t *ptr)
{
    /* Demo stop */
    usb_shmsc_demo_state = USB_TRANSFER_STOP;
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    /* Clear ANSI-C IO file number. */
    usb_hmsc_fn = -1;
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

    /* Init this application's drive open sequence state. */
    usb_shmsc_DriveOpenSeq = USB_SEQ_0;
    usb_ghmsc_SmpAplProcess = USB_APL_CLR;
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    /* ANSI-C IO enumeration initialization sequence. */
    usb_ghmsc_SmplClassSeq = -1;
    /* Wait for the ANSI flag enumeration initialization sequence. */
    usb_ghmsc_SmplClassSeqWait = 0;
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

    /* Open the demo application's message transmission task. */
    usb_hmsc_SmplMessageSend(ptr, USB_HMSC_OPEN);
}   /* eof usb_hmsc_apl_init() */

/******************************************************************************
Function Name   : R_usb_hmsc_AplClear
Description     : Application clear
Arguments       : USB_UTR_t *ptr        : The app's USB Comm. Structure.
Return          : none
******************************************************************************/
void R_usb_hmsc_AplClear(USB_UTR_t *ptr)
{
    /* Demo stop */
    usb_shmsc_demo_state = USB_TRANSFER_STOP;

    /* Init this application's drive open sequence state. */
    usb_shmsc_DriveOpenSeq = USB_SEQ_0;
    usb_ghmsc_SmpAplProcess = USB_APL_CLR;
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    /* Host MSC file number check. */
    if( usb_hmsc_fn != -1 )
    {
        /* End of HMSC communication. */
        close(usb_hmsc_fn);
        usb_hmsc_fn = -1;
    }
    usb_ghmsc_open = USB_NG;

    /* ANSI-IO enumeration initialization sequence. */
    usb_ghmsc_SmplClassSeq = -1;

    /* Wait for the ANSI flag enumeration initialization sequence. */
    usb_ghmsc_SmplClassSeqWait = 0;

    /* Send detach notification to demo application tasks. */
    usb_hmsc_SmplMessageSend(ptr, USB_HMSC_DETACH);
#else  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    usb_hmsc_SampleAplSpecifiedPath(ptr);
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof R_usb_hmsc_AplClear() */

/******************************************************************************
Function Name   : usb_shmsc_DemoStateChange
Description     : Change demo state.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_shmsc_DemoStateChange(void)
{
    /* Application demo state. */
    if(usb_shmsc_demo_state == USB_TRANSFER_START)
    {
        /* Application demo state: Start->Stop. */
        usb_shmsc_demo_state = USB_TRANSFER_STOP;
    }
    else if(usb_shmsc_demo_state == USB_TRANSFER_STOP)
    {
        /* Application demo state: Stop->Start. */
        usb_shmsc_demo_state = USB_TRANSFER_START;
    }
    else
    {
    }
}   /* eof usb_shmsc_DemoStateChange() */

/******************************************************************************
Function Name   : usb_hmsc_DummyFunction
Description     : dummy function
Arguments       : USB_UTR_t *ptr    : The app's USB Comm. Structure.
                : uint16_t data1            : not use
                : uint16_t data2            : not use
Return value    : none
******************************************************************************/
void usb_hmsc_DummyFunction(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
}   /* eof usb_hmsc_DummyFunction() */

/******************************************************************************
Function Name   : usb_hmsc_SampleAplTask
Description     : Sample application; "hmscdemo.txt" read-write loop.
                : Write once the file "hmscdemo.txt" to the USB memory media.
                : Demo application reads data repeatedly. Demo start/stop switch
                : is SW2 for RSK.
Argument        : none
Return          : none
******************************************************************************/
void usb_hmsc_SampleAplTask(void)
{
    USB_UTR_t       *mess;
    USB_ER_t        err;
    FRESULT         res;
    USB_CLSINFO_t   *mes;
    uint16_t        addr,i;
    FIL             file;
    uint16_t        file_rw_cnt;
    uint16_t        key_data;

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    USB_UTR_t       *ptr;
#else  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    uint16_t        file_err = USB_OK;
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

    /* Check for incoming application messages. */
    err = R_USB_TRCV_MSG(USB_HMSCSMP_MBX, (USB_MSG_t**)&mess, (USB_TM_t)5000);
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    if(err == USB_E_OK && usb_hmsc_fn == -1)
    {
        /* Open the case processing application processing has not been completed. */
        ptr = mess;

        if(mess->msginfo != USB_HMSC_OPEN && mess->msginfo != USB_HMSC_DETACH)
        {
            mess->msginfo = USB_HMSC_DETACH;
        }

        /* Switch action depending on message. */
        switch(mess->msginfo)
        {
        case USB_HMSC_OPEN:
            /* Host MSC open. */
            usb_hmsc_fn = open((int8_t *)USB_CLASS_HMSC, 0, 0);
            /* Host MSC file number check. */
            if(usb_hmsc_fn == -1)
            {
                /* If host MSC open is not complete, send a message requesting to re-open. */
                usb_hmsc_SmplMessageSend(ptr, USB_HMSC_OPEN);
            }
            else
            {
                /* Board LCD. */
                usb_cpu_LcdDisp( LCD_POS_U0, "ATTACH  ");
                usb_cpu_LcdDisp( LCD_POS_D0, "        ");
            }
            break;

        case USB_HMSC_DETACH:
            /* Board LCD. */
            usb_cpu_LcdDisp( LCD_POS_U0, "DETACH  ");
            usb_cpu_LcdDisp( LCD_POS_D0, "        ");

            /* Open the demo application's message transmission task. */
            usb_hmsc_SmplMessageSend(ptr, USB_HMSC_OPEN);
            break;

        default:
            /* Board LCD. */
            usb_cpu_LcdDisp( LCD_POS_U0, "DETACH  ");
            break;
        }
        /* Release message memory from pool. */
        err = R_USB_REL_BLK(USB_HMSCSMP_MPL,(USB_MH_t)mess);
        if( err != USB_E_OK )
        {
            USB_PRINTF0("### USB Hsmp Task rel_blk error\n");
        }
    }
    else if(err == USB_OK && usb_ghmsc_open == USB_OK)
    {
        /* Open application process when processing has been completed. */
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    if( err != USB_OK )
    {
        return;
    }
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
        /* Poll RSK switch input. */
        key_data = usb_cpu_GetKeyNo();
        if (key_data == USB_SW2_PUSH)
        {
            /* RSK SW2 pressed. DEMO START/STOP. */
            usb_shmsc_DemoStateChange();
        }

        /* Switch action depending on message. */
        mes = (USB_CLSINFO_t *)mess;
        switch( mes->msginfo )
        {
        case USB_HMSC_DRIVE_OPEN:
            /* Message next time we come back here is.. */
            /* == 0 -> demo start */
            usb_ghmsc_SmpAplProcess = USB_DEMO_START;
            usb_hmsc_SampleAplSpecifiedPath(mes);
            break;

        case USB_KEY_WAIT1:
                /* Check demo status. */
            if( usb_shmsc_demo_state != 0 )
            {
                /* Message next time we come back here is.. */
                /* If is 1 -> wait. */
                usb_hmsc_SampleAplSpecifiedPath(mes);
            }
            else
            {
                /* Message next time we come back here is Mount device. */
                usb_ghmsc_SmpAplProcess = USB_HMSC_DRIVEMOUNT;
                usb_hmsc_SampleAplSpecifiedPath(mes);

                    /* Board LCD. */
                usb_cpu_LcdDisp( LCD_POS_U0, "MSC DEMO");
                usb_cpu_LcdDisp( LCD_POS_D0, "START   ");
            }
            break;

        case USB_DEMO_START:
                /* Switch on application drive open sequence state. */
            switch( usb_shmsc_DriveOpenSeq )
            {
            case USB_SEQ_0:
                    /* Increment application drive open sequence. */
                usb_shmsc_DriveOpenSeq++;

                /* Set device address. */
                addr = mes->keyword;

                /* Storage drive search. */
                R_usb_hmsc_StrgDriveSearch(mess, addr);
                break;

            case USB_SEQ_1:
                    /* Check storage drive search result. */
                if( mes->result != USB_DONE )
                {
                    /* RSK LED0 on  */
                    usb_cpu_LedSetBit((uint8_t)0, (uint8_t)1);
                }
                else
                {
                    /* Device status setting = attached. */
                    R_usb_hmsc_SetDevSts((uint16_t)USB_HMSC_DEV_ATT);

                    /* Application initialization sequence start. */

                    /* Increment application drive open sequence. */
                    usb_shmsc_DriveOpenSeq = USB_SEQ_0;

                    /* Send message to myself: Start demo key input wait. */
                    usb_ghmsc_SmpAplProcess = USB_KEY_WAIT1;
                    usb_hmsc_SampleAplSpecifiedPath(mes);

                    /* Board LCD. */
                    usb_cpu_LcdDisp( LCD_POS_U0, "MSC DEMO");
                    usb_cpu_LcdDisp( LCD_POS_D0, "READY   ");
                }
                break;
            default:
                    /* Reset application drive open sequence. */
                usb_shmsc_DriveOpenSeq = USB_SEQ_0;
                break;
            }
            break;

        case USB_HMSC_DRIVEMOUNT:
            /* File system media work area memory mount. */
            res = R_tfat_f_mount(0, &usb_gFatfs);
            if( res != TFAT_FR_OK )
            {
                USB_PRINTF1("R_tfat_f_mount error: %d\n", res);
            }
            /* Message next time we come back here is.. */
            usb_ghmsc_SmpAplProcess = USB_HMSC_FILEWRITE;
            usb_hmsc_SampleAplSpecifiedPath(mes);
            break;

        case USB_HMSC_FILEWRITE:
            usb_ghmsc_SmpAplProcess = USB_HMSC_FILEWRITE;
            /* Create a file object. */
            res = R_tfat_f_open(&file, "hmscdemo.txt", (TFAT_FA_CREATE_ALWAYS | TFAT_FA_WRITE));
            if( res != TFAT_FR_OK )
            {
                USB_PRINTF1("R_tfat_f_open error: %d\n", res);
            }

            /* Create a data set to write to file. */
            for( i = 0; i < 512; ++i ) {
                usb_gBuf[i] = 0x61;     /* 512 'a'. */
            }

            /* Write the data-set to a file. */
            res = R_tfat_f_write(&file, usb_gBuf, sizeof(usb_gBuf), &file_rw_cnt);
            if( res != TFAT_FR_OK )
            {
                USB_PRINTF1("R_tfat_f_write error: %d\n", res);
            }

            /* Close the file object. */
            res = R_tfat_f_close(&file);
            if( res != TFAT_FR_OK )
            {
                USB_PRINTF1("R_tfat_f_close error: %d\n", res);
            }
            /* Message next time we come back here is.. */
            usb_ghmsc_SmpAplProcess = USB_HMSC_FILEREAD;
            usb_hmsc_SampleAplSpecifiedPath(mes);
            break;

        case USB_KEY_WAIT2:
            /* Check demo status. */
            if( usb_shmsc_demo_state != 0 )
            {
                /* Message next time we come back here is.. */
                /* == 1 -> wait */
                usb_hmsc_SampleAplSpecifiedPath(mes);
            }
            else
            {
                /* Message next time we come back here is.. */
                /* == 0 -> demo start */
                usb_ghmsc_SmpAplProcess = USB_HMSC_FILEREAD;
                usb_hmsc_SampleAplSpecifiedPath(mes);

                /* Board LCD. */
                usb_cpu_LcdDisp( LCD_POS_U0, "MSC DEMO");
                usb_cpu_LcdDisp( LCD_POS_D0, "START   ");
            }
            break;

        case USB_HMSC_FILEREAD:
            if( usb_ghmsc_open == USB_OK )
            {
                usb_ghmsc_SmpAplProcess = USB_HMSC_FILEREAD;
                /* File open in read mode */
                res = R_tfat_f_open(&file, "hmscdemo.txt", (TFAT_FA_OPEN_ALWAYS | TFAT_FA_READ));
                if( res != TFAT_FR_OK )
                {
                    USB_PRINTF1("R_tfat_f_open error: %d\n", res);
                }
                else
                {
                    /* File access LED control. Shift LED position each successful 100 file reads. */
                    ++usb_sLedwork1;
                    if( usb_sLedwork1 > 100 )
                    {
                        usb_sLedwork1 = 0;
                        /* usb_sLedwork2 = 1,2,4,8 */
                        if( usb_sLedwork2 > 15 )
                        {
                            usb_sLedwork2 = 1;
                        }
                        /* RSK LED ON output shift in order; LED0 LED1 LED2 LED3. */
                        usb_cpu_LedSet(usb_sLedwork2);

                        /* Shift LED ON position. */
                        usb_sLedwork2 = usb_sLedwork2 << 1;
                    }
                }

                /* Move pointer in file object. */
                res = R_tfat_f_lseek(&file, 0);
                if( res != TFAT_FR_OK )
                {
                    USB_PRINTF1("R_tfat_f_lseek error: %d\n", res);
                }

                /* Read data from file. */
                res = R_tfat_f_read(&file, usb_gBuf, sizeof(usb_gBuf), &file_rw_cnt);
                if( res != TFAT_FR_OK )
                {
                    USB_PRINTF1("R_tfat_f_read error: %d\n", res);
                    usb_shmsc_demo_state = USB_TRANSFER_STOP;
#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
                    file_err = USB_NG;
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */
                }

                for( i = 0; i < file_rw_cnt; ++i )
                {
                    /* USB_PRINTF2("%d:[%02X]\n", i, usb_gBuf[i]);
                    USB_PRINTF1("%c", usb_gBuf[i]); */
                }

                /* Close the file object. */
                res = R_tfat_f_close(&file);
                if( res != TFAT_FR_OK )
                {
                    USB_PRINTF1("R_tfat_f_close error: %d\n", res);
                }

                /* Check demo status. */
                if( usb_shmsc_demo_state != 0 )
                {
                    /* Board LCD. 1 -> demo end. */
                    usb_cpu_LcdDisp( LCD_POS_D0, "STOP    ");
                    /* Message next time we come back here is.. */
                    usb_ghmsc_SmpAplProcess = USB_KEY_WAIT2;
                }
                else
                {
                    /* Message next time we come back here is.. */
                    /* == 0 -> read loop. */
                }
#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
                if( file_err != USB_NG )
                {
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */
                usb_hmsc_SampleAplSpecifiedPath(mes);
#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
                }
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */
            }
            break;

        case USB_APL_CLR:
            /* Board LCD. */
            usb_cpu_LcdDisp( LCD_POS_U0, "DETACH  ");
            usb_cpu_LcdDisp( LCD_POS_D0, "        ");
            break;

        default:
//          while( 1 );             /* Error */
            break;
        }
        /* Release message memory from pool. */
        err = R_USB_REL_BLK(USB_HMSCSMP_MPL,(USB_MH_t)mess);
        if( err != USB_E_OK )
        {
            USB_PRINTF0("### USB Hsmp Task rel_blk error\n");
        }
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    }
    else
    {
        return;
    }
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hmsc_SampleAplTask() */

/******************************************************************************
Function Name   : usb_hmsc_SmplMessageSend
Description     : Sample class init
Arguments       : USB_UTR_t *ptr        : The app's USB Comm. Structure.
                : uint16_t   msginfo         : Message Information Kind
Return          : none
******************************************************************************/
void usb_hmsc_SmplMessageSend( USB_UTR_t *ptr, uint16_t msginfo )
{
    USB_MH_t        p_blf;
    USB_ER_t        err;
    USB_CLSINFO_t   *cp;

    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_HMSCSMP_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_CLSINFO_t*)p_blf;

        cp->msginfo = msginfo;
        cp->ipp = ptr->ipp;
        cp->ip  = ptr->ip;
        cp->keyword = ptr->keyword;
        cp->result  = ptr->result;

        /* Send message */
        err = R_USB_SND_MSG( USB_HMSCSMP_MBX, (USB_MSG_t*)cp );
        if( err != USB_E_OK )
        {
            /* Error. Release message memory from pool. */
            err = R_USB_REL_BLK(USB_HMSCSMP_MPL,(USB_MH_t)p_blf);
            USB_PRINTF0("### usb_hmsc_SmplMessageSend function snd_msg error\n");
        }
    }
    else
    {
        /* error */
        USB_PRINTF0("### usb_hmsc_SmplMessageSend function pget_blk error\n");
        while( 1 );
    }
}   /* eof usb_hmsc_SmplMessageSend() */

/******************************************************************************
Function Name   : usb_hmsc_SampleAplSpecifiedPath
Description     : Next Process Selector
Argument        : USB_CLSINFO_t *mess
Return          : none
******************************************************************************/
void usb_hmsc_SampleAplSpecifiedPath(USB_CLSINFO_t *mess)
{
    USB_MH_t            p_blf;
    USB_ER_t            Err;
    USB_CLSINFO_t       *ptr;

    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_HMSCSMP_MPL, &p_blf) == USB_E_OK )
    {
        ptr = (USB_CLSINFO_t*)p_blf;
        ptr->msginfo    = usb_ghmsc_SmpAplProcess;
        ptr->keyword    = mess->keyword;
        ptr->result     = mess->result;

        /* Send message */
        Err = R_USB_SND_MSG(USB_HMSCSMP_MBX, (USB_MSG_t*)p_blf);
        if( Err != USB_E_OK )
        {
            Err = R_USB_REL_BLK(USB_HMSCSMP_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0("### SpecifiedPass function snd_msg error\n");
        }
    }
    else
    {
        USB_PRINTF0("### SpecifiedPass function pget_blk error\n");
    }
}   /* eof usb_hmsc_SampleAplSpecifiedPath() */

/******************************************************************************
End  Of File
******************************************************************************/

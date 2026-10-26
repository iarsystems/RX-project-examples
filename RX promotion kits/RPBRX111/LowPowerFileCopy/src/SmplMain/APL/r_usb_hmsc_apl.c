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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
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
*         : 04.06.2013 2.01    Modification for low power datasheet copy.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_usrconfig.h"        /* USB-H/W register set (user define) */
//#include "r_usb_cDefUSBIP.h"        /* USB-FW Library Header */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include <stdlib.h>                 /* HMSC Sample use */
#include <stdio.h>                  /* HMSC Sample use */
#include "r_usb_hmsc_define.h"      /* HMSC define */
#include "r_TinyFAT.h"              /* TFAT header */
#include "r_usb_hmsc_extern.h"
#include "hw_resource.h"
#include "r_usb_api.h"
#include "r_usb_hmsc_api.h"
#include "lcd.h"

/*****************************************************************************
Macro definitions
******************************************************************************/
/* Filename to write to USB stick (must be 8.3 format) */
#define HMSC_FILENAME               "RX111.pdf"

/* Main application state machine definitions */
#define USB_HMSC_DRIVE_OPEN         (usb_strct_t)0x10
#define USB_DEMO_START              (usb_strct_t)0x11
#define USB_HMSC_DRIVEMOUNT         (usb_strct_t)0x12
#define USB_HMSC_FILEWRITE          (usb_strct_t)0x13
#define USB_APL_CLR                 (usb_strct_t)0x14
#define USB_HMSC_DRIVE_CLOSE        (usb_strct_t)0x15
#define USB_APL_POWERDOWN           (usb_strct_t)0x16

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
#ifdef  R8CUSB
#pragma section program P_apl
#pragma section bss B_hmsc
#endif
#ifdef  RL78USB
#endif
#ifdef  RL78USB_SECTION
#pragma section @@CODE   P_APL
#pragma section @@BASE   T_APL
#pragma section @@DATA   B_APL
#endif

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

void        usb_hmsc_apl_init();
void        R_usb_hmsc_AplClear();
void        usb_shmsc_DemoStateChange(void);
static void usb_hapl_registration();
void        R_usb_hmsc_DriveClose(uint16_t addr, uint16_t data2);
void        R_usb_hmsc_DriveOpen(uint16_t addr, uint16_t data2);
void        usb_hmsc_task_start( void );
void        usb_hmsc_SampleAplTask(void);
void        usb_hmsc_SampleAplSpecifiedPath(usb_tskinfo_t *mess);
USB_STATIC void usb_hmsc_device_state(uint16_t data, uint16_t state);

int16_t     usb_ghmsc_open = USB_E_ERROR;

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
extern void         usb_cpu_LcdDisp(uint8_t position, uint8_t *string);
extern void         usb_cpu_led_set_data(uint8_t data);
extern void         usb_cpu_led_set_bit(uint8_t bit, uint8_t data);
extern uint8_t      usb_smpl_key_input(void);

extern void         usb_cstd_dummy_function(uint16_t data1, uint16_t data2);

extern uint8_t      usb_hstd_check_attach(void);
extern void         usb_hstd_vbus_control_off(void);

extern unsigned char   usb_data_file[];
extern const unsigned int    usb_data_size;

/******************************************************************************
Renesas Host MSC Sample Code functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_hmsc_task_start
Description     : Start task processing.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hmsc_task_start( void )
{
    /* Target board initialize */
    usb_cpu_target_init();


#ifdef  USB_PMOD_LCD_ENABLE
    /* Display Debug LCD */
    Display_LCD(0, "Low Power File Copy" );
    Display_LCD(1, "Connect USB Stick" );
#endif /* USB_LCD_ENABLE */

    /* USB-IP initialized */
    R_usb_hstd_ChangeDeviceState(USB_DO_INITHWFUNCTION);

    /* HCD driver open & registratuion */
    R_usb_hstd_HcdOpen();           /* HCD task, MGR task open */
    usb_hapl_registration();        /* Host application registration. */
    R_usb_hmsc_driver_start();      /* Host class driver. */
    usb_hmsc_apl_init();            /* Clear application. */

    /* Scheduler initialized */
    R_usb_hstd_ChangeDeviceState(USB_DO_SETHWFUNCTION);

    /* Wait for device connection */
    if (USB_LNST_DETACH == usb_hstd_check_attach())
    {
        /* Switch to subclock and stop main */
        usb_cpu_set_clock_subclock();

        /* Stay in sleep mode until woken by attach */
        while (USB_LNST_DETACH == usb_hstd_check_attach())
        {
            usb_cpu_deep_sleep_mode();
        }

        /* Stop the periodic timer */
        usb_cpu_periodic_disable();

        /* Switch back to main clock */
        usb_cpu_set_clock_pll();
    }

}   /* eof usb_hmsc_task_start() */

/******************************************************************************
Function Name   : usb_hapl_registration
Description     : Callback registration.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hapl_registration()
{
    usb_hcdreg_t    driver;

    /* Driver registration */
    driver.ifclass      = (uint16_t)USB_IFCLS_MAS;      /* Use Interface class for MSC. */
    driver.classcheck   = &R_usb_hmsc_ClassCheck;       /* Driver check. */
    driver.statediagram = &usb_hmsc_device_state;

    /* Host MSC class driver registration. */
    R_usb_hstd_DriverRegistration(&driver);
}   /* eof usb_hapl_registration() */

/******************************************************************************
Function Name   : R_usb_hmsc_DriveOpen
Description     : drive open
Arguments       : uint16_t addr :
                : uint16_t data2 :
Return value    : none
******************************************************************************/
void R_usb_hmsc_DriveOpen( uint16_t addr, uint16_t data2)
{
/* Condition compilation by the difference of the File system */
#ifdef USB_TFAT_USE_PP
    usb_ghmsc_open = USB_E_OK;

    R_USB_GET_SND(USB_HMSCSMP_MBX, USB_HMSC_DRIVE_OPEN, &usb_cstd_dummy_function, (usb_strct_t)addr);
#else   /* !USB_TFAT_USE_PP */
    uint16_t    i, max_drv, result;
    int16_t     err;

    R_usb_hmsc_StrgDriveSearch( addr);

    if( result == USB_E_OK )
    {
        R_usb_hmsc_SetDevSts((uint16_t)USB_HMSC_DEV_ATT);
        for( i = USB_DRIVE; i < USB_MAXDRIVE; i++ )
        {
            if( usb_hmsc_SmpCheckAddr(i) == addr )
            {
                if( (err = dev_att((int16_t)i)) != 0 )
                {
                }
            }
        }
    }
    usb_hmsc_DrivePrint();
#endif  /* USB_TFAT_USE_PP */
}   /* eof R_usb_hmsc_DriveOpen() */

/******************************************************************************
Function Name   : R_usb_hmsc_DriveClose
Description     : drive close
Arguments       : uint16_t addr :
                : uint16_t data2 :
Return value    : none
******************************************************************************/
void R_usb_hmsc_DriveClose( uint16_t addr, uint16_t data2)
{
/* Condition compilation by the difference of the File system */
#ifdef USB_TFAT_USE_PP
    R_usb_hmsc_AplClear();
    R_usb_hmsc_SetDevSts((uint16_t)USB_HMSC_DEV_DET);
    usb_hmsc_SmpFsiDriveClear( addr);

#else   /* !USB_TFAT_USE_PP */
    uint16_t    i, min_drv, max_drv;

    for( i = USB_DRIVE; i < USB_MAXDRIVE; i++ )
    {
        if( usb_hmsc_SmpCheckAddr(i) == addr )
        {
            usb_hmsc_Det((int16_t)i);
        }
    }

    R_usb_hmsc_SetDevSts( (uint16_t)USB_HMSC_DEV_DET);
    usb_hmsc_SmpFsiDriveClear( addr);
    usb_hmsc_DrivePrint();
#endif  /* USB_TFAT_USE_PP */
}   /* eof R_usb_hmsc_DriveClose() */

/******************************************************************************
Function Name   : usb_hmsc_apl_init
Description     : Clear application.
Arguments       : none
Return          : none
******************************************************************************/
void usb_hmsc_apl_init()
{
    /* Demo stop */
    usb_shmsc_demo_state = USB_TRANSFER_STOP;

    /* Init this application's drive open sequence state. */
    usb_shmsc_DriveOpenSeq = USB_SEQ_0;
    usb_ghmsc_SmpAplProcess = USB_APL_CLR;
}   /* eof usb_hmsc_apl_init() */

/******************************************************************************
Function Name   : R_usb_hmsc_AplClear
Description     : Application clear
Arguments       : none
Return          : none
******************************************************************************/
void R_usb_hmsc_AplClear()
{
    usb_tskinfo_t dummy_mess;

    usb_hmsc_apl_init();            /* Clear application */
    usb_ghmsc_open = USB_E_ERROR;
    usb_hmsc_SampleAplSpecifiedPath( (usb_tskinfo_t*)&dummy_mess );
}   /* eof R_usb_hmsc_AplClear() */

/******************************************************************************
Function Name   : usb_hmsc_SampleAplTask
Description     : Process the application state machine which mounts the USB
                  media and writes a file.
Argument        : none
Return          : none
******************************************************************************/
void usb_hmsc_SampleAplTask(void)
{
    usb_tskinfo_t   *mess;
    usb_er_t        err;
    FRESULT         res;
    usb_tskinfo_t   *mes;
    uint16_t        addr;
    FIL             file;
    uint16_t        file_rw_cnt;

    /* Check for incoming application messages. */
    err = R_USB_RCV_MSG(USB_HMSCSMP_MBX, (usb_msg_t**)&mess);
    if( err != USB_E_OK )
    {
        return;
    }

    /* Switch action depending on message. */
    mes = (usb_tskinfo_t *)mess;
    switch( mes->msginfo )
    {
    case USB_HMSC_DRIVE_OPEN:
        /* Make next state USB_DEMO_START */
        usb_ghmsc_SmpAplProcess = USB_DEMO_START;
        usb_hmsc_SampleAplSpecifiedPath(mes);

        /* Board LCD. */

#ifdef  USB_PMOD_LCD_ENABLE
        /* Board LCD. */
        Display_LCD(1, "USB Stick Attached");
#endif
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
            R_usb_hmsc_StrgDriveSearch(addr);
            break;

        case USB_SEQ_1:
            /* Check storage drive search result. */
            if( mes->keyword != USB_E_OK )
            {
                /* LED3 on to show error */
                usb_cpu_led_set_bit((uint8_t)3, (uint8_t)1);
            }
            else
            {
                /* Device status setting = attached. */
                R_usb_hmsc_SetDevSts((uint16_t)USB_HMSC_DEV_ATT);

                /* Application initialization sequence start. */

                /* Increment application drive open sequence. */
                usb_shmsc_DriveOpenSeq = USB_SEQ_0;

                /* Make next state USB_HMSC_DRIVEMOUNT */
                usb_ghmsc_SmpAplProcess = USB_HMSC_DRIVEMOUNT;
                usb_hmsc_SampleAplSpecifiedPath(mes);
#ifdef  USB_PMOD_LCD_ENABLE
                /* Board LCD. */
                Display_LCD(1, "Copy Ready      ");
#endif
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
            /* LED3 indicates error */
            usb_cpu_led_set_bit(LED_POS3, LED_ON);
        }

        /* Light LED to indicate device was mounted */
        usb_cpu_led_set_bit(LED_POS2, LED_ON);

        /* Make next state USB_HMSC_FILEWRITE */
        usb_ghmsc_SmpAplProcess = USB_HMSC_FILEWRITE;
        usb_hmsc_SampleAplSpecifiedPath(mes);
        break;

    case USB_HMSC_FILEWRITE:
        usb_ghmsc_SmpAplProcess = USB_HMSC_FILEWRITE;

        /* Board LCD. */
#ifdef  USB_PMOD_LCD_ENABLE
        /* Board LCD. */
        Display_LCD(1, "Copying Datasheet");
#endif

        /* Create a file object. */
        res = R_tfat_f_open(&file, HMSC_FILENAME, (TFAT_FA_CREATE_ALWAYS | TFAT_FA_WRITE));
        if( res != TFAT_FR_OK )
        {
            /* LED3 indicates error */
            usb_cpu_led_set_bit(LED_POS3, LED_ON);
        }

        /* Write the data to the file. */
        res = R_tfat_f_write(&file, usb_data_file, usb_data_size,
                &file_rw_cnt);
        if( res != TFAT_FR_OK )
        {
            /* LED3 indicates error */
            usb_cpu_led_set_bit(LED_POS3, LED_ON);
        }

        /* Close the file object. */
        res = R_tfat_f_close(&file);
        if( res != TFAT_FR_OK )
        {
            /* LED3 indicates error */
            usb_cpu_led_set_bit(LED_POS3, LED_ON);
        }

        /* Make the next state shutdown and detach the USB disk */
        usb_ghmsc_SmpAplProcess = USB_HMSC_DRIVE_CLOSE;
        usb_hmsc_SampleAplSpecifiedPath(mes);
        break;

    case USB_HMSC_DRIVE_CLOSE:
        /* Device status setting = detached. */
        R_usb_hmsc_SetDevSts((uint16_t)USB_HMSC_DEV_DET);

#ifdef  USB_PMOD_LCD_ENABLE
        /* Board LCD. */
        Display_LCD(1, "Copy Complete!     ");
        Display_LCD(2, "Sleeping.");
#endif

        /* Extinguish LED to mark copy done */
        usb_cpu_led_set_bit(LED_POS2, LED_OFF);

        /* Make the next state end the program */
        usb_ghmsc_SmpAplProcess = USB_APL_POWERDOWN;
        usb_hmsc_SampleAplSpecifiedPath(mes);
        break;

    case USB_APL_POWERDOWN:
        /* Switch to subclock, low speed operating mode */
        usb_cpu_set_clock_subclock();

        /* Shut down VBUS */
        usb_hstd_vbus_control_off();

        while (1)
        {
            /* Power down the CPU */
            usb_cpu_soft_stanby_mode();
        }
        break;

    case USB_APL_CLR:
#ifdef  USB_PMOD_LCD_ENABLE
        /* Board LCD. */
        Display_LCD(1, "USB Stick Detached");
#endif
        break;

    default:
        break;
    }
    /* Release message memory from pool. */
    err = R_USB_REL_BLK(mess->flag);
    if( err != USB_E_OK )
    {
    }
}   /* eof usb_hmsc_SampleAplTask() */

/******************************************************************************
Function Name   : usb_hmsc_SampleAplSpecifiedPath
Description     : Next Process Selector
Argument        : usb_utr_t *mess
Return          : none
******************************************************************************/
void usb_hmsc_SampleAplSpecifiedPath(usb_tskinfo_t *mess)
{
    R_USB_GET_SND(USB_HMSCSMP_MBX, usb_ghmsc_SmpAplProcess, &usb_cstd_dummy_function, (usb_strct_t)mess->keyword);
}   /* eof usb_hmsc_SampleAplSpecifiedPath() */

/******************************************************************************
End  Of File
******************************************************************************/
/******************************************************************************
Function Name   : usb_hmsc_device_state
Description     : Open / Close
Arguments       : uint16_t data             : Device address
            : uint16_t state            : Device state
Return value    : none
******************************************************************************/
USB_STATIC void usb_hmsc_device_state(uint16_t data, uint16_t state)
{
    switch( state )
    {
        case USB_STS_DETACH:
            R_usb_hmsc_DriveClose( data, 0 );   /* Callback when device is detached. */
        break;
        case USB_STS_ATTACH:
        break;
        case USB_STS_DEFAULT:
            R_usb_hmsc_Initialized();
        break;
        case USB_STS_CONFIGURED:
            R_usb_hmsc_DriveOpen( data, 0 );    /* Callback when device is configured. */
        break;
        case USB_STS_SUSPEND:
        break;
        case USB_STS_RESUME:
        break;
        case USB_STS_WAKEUP:
        break;
        case USB_STS_POWER:
        break;
        case USB_STS_PORTOFF:
        break;
        case USB_STALL_SUCCESS:
        break;
        default:
        break;
    }

}   /* eof usb_hsmpl_device_state() */

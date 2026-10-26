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
* Copyright (C) 2010(2011) Renesas Electronics Corpration
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name    : r_usb_hhid_apl.c
* Version      : 1.02
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB HID application code
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.07.2010 0.90    First Release
*         : 30.11.2010 0.95    USBSTDFW(Mass Production Release)
*         : 31.01.2011 1.00    Host HID class(Mass Production Release)
*         : 04.03.2011 1.01    usb_hhid_AppTask Mouse Keyboard process pack
*         : 17.06.2011 1.02    Support USB Basic Fw V110
*         : 31.05.2011 2.00    Support USB Basic FW v. 2.00.
******************************************************************************

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_usrconfig.h"        /* USB-H/W register set (user define) */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cmacsystemcall.h"   /* System call macro */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_cextern.h"          /* USB-FW grobal define */
#include "r_usb_hhid_define.h"      /* HID class define */
#include "r_usb_hhid_extern.h"
#include "r_usb_ansi.h"
#include "r_usb_control.h"
#include "r_usb_api.h"
#include "r_usb_hhid_api.h"
#include "r_usb_class_usrcfg.h"
#include "hw_resource.h"

/******************************************************************************
Constant macro definitions
******************************************************************************/
/* Switch define */
#define USB_HHID_SW_OFF                     0x00    /* No push */
#define USB_HHID_SW2_ON                     0x02    /* SW2 push */
#define USB_HHID_SW3_ON                     0x04    /* SW3 push */

/* LCD Display Position */
#define USB_HHID_DISP_X_AXIS_POSITION       0u      /* Mouse X axis */
#define USB_HHID_DISP_Y_AXIS_POSITION       4u      /* Mouse Y axis */
#define USB_HHID_DISP_KEY_POSITION          LCD_POS_D6  /* Keyborad Input Key */
#define USB_HHID_LCD_DISP_POSITION_LINE1    LCD_POS_U0  /* Line 1 */
#define USB_HHID_LCD_DISP_POSITION_LINE2    LCD_POS_D0  /* Line 2 */

/* Mouse Button Defines */
#define USB_HHID_LEFT_BUTTON                0x01    /* Left Button */
#define USB_HHID_RIGHT_BUTTON               0x02    /* Right Button */
#define USB_HHID_WHEEL_BUTTON               0x04    /* Wheel Button */

/* LED Position for USB Mouse */
#define USB_HHID_LED_POS_LEFT_BTN           0x00    /* Left Button */
#define USB_HHID_LED_POS_RIGHT_BTN          0x01    /* Right Button */
#define USB_HHID_LED_POS_WHEEL_BTN          0x02    /* Wheel Button */

/* USB Mouse no movement */
#define USB_HHID_MSE_NOT_MOVE               0x00

/* Mouse data Store Position Index */
#define USB_HHID_MSE_BTN_DATA_INDEX         0x00    /* Input Key */
#define USB_HHID_MSE_X_DATA_INDEX           0x01    /* X-axis */
#define USB_HHID_MSE_Y_DATA_INDEX           0x02    /* Y-axis */

#define USB_HHID_KBD_TRN_LEN                0x08    /* Keyboard mode USB Transaction Length */

#define USB_HHID_KBD_NOT_PUSH               0x00    /* USB Keyboard no key input */

/* Keyboard data Store Position Index */
#define USB_HHID_KBD_KEYCODE1_INDEX         0x02    /* Input Key */

/* USB HID Keyboard Usage ID */
#define USB_HHID_KBD_CODE_1                 0x1E    /* 1 */
#define USB_HHID_KBD_CODE_0                 0x27    /* 0 */
#define USB_HHID_KBD_CODE_ENTER             0x28    /* '\n' */
#define USB_HHID_KBD_CODE_BS                0x2A    /* 'BS' */
#define USB_HHID_KBD_CODE_SPACE             0x2C    /* ' ' */
#define USB_HHID_KBD_CODE_HYPHEN            0x2D    /* '-' */
#define USB_HHID_KBD_CODE_COLON             0x34    /* ':' */
#define USB_HHID_KBD_CODE_DOT               0x37    /* '.' */
#define USB_HHID_KBD_CODE_DIAGONAL          0x38    /* '/' */
#define USB_HHID_KBD_CODE_UNDERSCORE        0x87    /* '_' */
#define USB_HHID_KBD_CODE_YEN               0x89    /* '\' */

/* Keycode to ASCII code offset */
#define USB_HHID_KEYCODE_TO_ASCII_OFFSET0   0x3D    /* 'A'-'Z' */
#define USB_HHID_KEYCODE_TO_ASCII_OFFSET1   0x13    /* '1'-'9' */
#define USB_HHID_KEYCODE_TO_ASCII_OFFSET2   0x09    /* '0' */
#define USB_HHID_ASCII_BS                   0x08    /* BackSpace */

/* Set Report (Keyborad LED ) */
#define USB_HHID_NUM_LOCK                   0x01
#define USB_HHID_CAPS_LOCK                  0x02
#define USB_HHID_SCROLL_LOCK                0x04
#define USB_HHID_KBD_LED_CLEAR              0x00

/* Set Protocol */
#define USB_HHID_BOOT_PROTOCOL              0
#define USB_HHID_REPORT_PROTOCOL            1

/* ----- Host HID Application State ----- */
#define USB_HHID_APL_CLOSE                  0x81    /* Close State */
#define USB_HHID_APL_MODE_KBD               0x90    /* Keyboard Mode State */
#define USB_HHID_APL_MODE_MSE               0xA0    /* Mouse Mode State */
/* Host HID Application State Offset */
#define USB_HHID_APL_OFFSET_OPEN            1       /* Open State */
#define USB_HHID_APL_OFFSET_RX_START        2       /* Data receive Start State */
#define USB_HHID_APL_OFFSET_RX_WAIT         3       /* Data receive wait State */
#define USB_HHID_APL_OFFSET_RX              4       /* Data receive complete State */
#define USB_HHID_APL_OFFSET_SUSPEND         5       /* Suspend */
#define USB_HHID_APL_OFFSET_RESUME          6       /* Resume */
#define USB_HHID_APL_OFFSET_CTL_END         7       /* control transfer complete */

#define USB_HHID_APL_IDLE                   0
#define USB_HHID_APL_INIT                   USB_HHID_APL_OFFSET_OPEN
#define USB_HHID_APL_KBD_OPEN               (USB_HHID_APL_MODE_KBD + USB_HHID_APL_OFFSET_OPEN)
#define USB_HHID_APL_MSE_OPEN               (USB_HHID_APL_MODE_MSE + USB_HHID_APL_OFFSET_OPEN)
#define USB_HHID_APL_KBD_RX_START           (USB_HHID_APL_MODE_KBD + USB_HHID_APL_OFFSET_RX_START)
#define USB_HHID_APL_MSE_RX_START           (USB_HHID_APL_MODE_MSE + USB_HHID_APL_OFFSET_RX_START)
#define USB_HHID_APL_KBD_RX_WAIT            (USB_HHID_APL_MODE_KBD + USB_HHID_APL_OFFSET_RX_WAIT)
#define USB_HHID_APL_MSE_RX_WAIT            (USB_HHID_APL_MODE_MSE + USB_HHID_APL_OFFSET_RX_WAIT)
#define USB_HHID_APL_KBD_RX                 (USB_HHID_APL_MODE_KBD + USB_HHID_APL_OFFSET_RX)
#define USB_HHID_APL_MSE_RX                 (USB_HHID_APL_MODE_MSE + USB_HHID_APL_OFFSET_RX)
#define USB_HHID_APL_KBD_SUSPEND            (USB_HHID_APL_MODE_KBD + USB_HHID_APL_OFFSET_SUSPEND)
#define USB_HHID_APL_MSE_SUSPEND            (USB_HHID_APL_MODE_MSE + USB_HHID_APL_OFFSET_SUSPEND)
#define USB_HHID_APL_KBD_RESUME             (USB_HHID_APL_MODE_KBD + USB_HHID_APL_OFFSET_RESUME)
#define USB_HHID_APL_MSE_RESUME             (USB_HHID_APL_MODE_MSE + USB_HHID_APL_OFFSET_RESUME)
#define USB_HHID_APL_KBD_CTL_END            (USB_HHID_APL_MODE_KBD + USB_HHID_APL_OFFSET_CTL_END)
#define USB_HHID_APL_MSE_CTL_END            (USB_HHID_APL_MODE_MSE + USB_HHID_APL_OFFSET_CTL_END)

/* Application State Mode Mask */
#define USB_HHID_APL_MODE_MASK              0xf0

/******************************************************************************
External variables and functions
******************************************************************************/
#ifdef  USB_LCD_ENABLE
extern void             usb_cpu_LcdInit(void);
extern void             usb_cpu_LcdDisp(uint8_t pos, uint8_t *str);
#endif /* USB_LCD_ENABLE */

#ifdef  USB_LED_ENABLE
extern void             usb_cpu_LedSetBit(uint8_t bit, uint8_t data);
#endif /* USB_LED_ENABLE */

#ifdef USB_KEY_ENABLE
extern uint16_t         usb_cpu_GetKeyNo(void);
#endif /* USB_KEY_ENABLE */

extern void             usb_hhid_usb_hostdriver_setting(USB_UTR_t *ptr);

/*****************************************************************************
Enumerated Types
******************************************************************************/

/* Host Std Sample application message command */
typedef enum
{
    USB_HHID_OPEN = 1,
    USB_HHID_DETACH,
    USB_HHID_DEMO
}
USB_HHID_ANSI_COMMAND;

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_apl"

/******************************************************************************
Private global variables and functions
******************************************************************************/
void            usb_cstd_task_start( void );
void            usb_hhid_MainTask(USB_VP_INT stacd);
void            usb_hhid_prapl_title(void);
static void     usb_hhid_task_start( void );
static void     usb_hapl_task_start( USB_UTR_t *ptr );
static void     usb_apl_task_switch(void);
static void     usb_hhid_smpl_init(USB_UTR_t *ptr);
static void     usb_hhid_smpl_mse_data(uint8_t *data);
static void     usb_hhid_smpl_kbd_data(uint8_t *data);
static void     usb_hhid_smpl_resume_cb(uint16_t data1, uint16_t data2);

#ifdef USB_HHID_GET_REPORT_PIPE0
static void     usb_hhid_smpl_get_report_result(USB_UTR_t *mess, uint16_t data1, uint16_t data2);
#endif /* USB_HHID_GET_REPORT_PIPE0 */

static void     usb_hhid_smpl_val_to_str(uint8_t *data,int8_t val);
static void     usb_hhid_smp_status_set(uint16_t status);

#ifndef USB_HHID_GET_REPORT_PIPE0
static void     usb_hhid_smpl_data_trans_result(USB_UTR_t *mess);
#endif  /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */

static void     usb_hhid_set_protocol_result(USB_UTR_t *mess, uint16_t devadr, uint16_t data2);
static void     usb_hapl_registration(USB_UTR_t *ptr);
static void     usb_hhid_smpl_open(USB_UTR_t *ptr, uint16_t devadr, uint16_t data2);
static void     usb_hhid_smpl_close(USB_UTR_t *ptr, uint16_t data1, uint16_t data2);

static void     usb_hhid_smpl_kbd_led_ctl(USB_UTR_t *ptr, uint16_t devadr, uint16_t key_data);

static void     usb_hhid_smpl_set_report(USB_UTR_t *ptr, uint16_t devadr, uint8_t *p_data, uint16_t length,
                    USB_CB_t complete);
static void     usb_hhid_smpl_kbd_led_ctl_result(USB_UTR_t *mess, uint16_t data1, uint16_t data2);
void            usb_hhid_smpl_get_report(USB_UTR_t *ptr, uint16_t devadr, uint8_t *p_data, uint16_t length,
                    USB_CB_t complete);
void            usb_hhid_smpl_set_idle(USB_UTR_t *ptr, uint16_t devadr, uint16_t duration, USB_CB_t complete);
void            usb_hhid_smpl_get_idle(USB_UTR_t *ptr, uint16_t devadr, uint16_t *idle_rate, USB_CB_t complete);
void            usb_hhid_smpl_get_protocol(USB_UTR_t *ptr, uint16_t devadr, uint8_t *protocol, USB_CB_t complete);
void            usb_hhid_smpl_set_protocol(USB_UTR_t *ptr, uint16_t devadr, uint8_t protocol, USB_CB_t complete);
void            usb_hhid_smpl_get_hid_descriptor(USB_UTR_t *ptr, uint16_t devadr, uint8_t *p_data, USB_CB_t complete);
void            usb_hhid_smpl_get_report_descriptor(USB_UTR_t *ptr, uint16_t devadr, uint8_t *p_data, uint16_t length,
                    USB_CB_t complete);
void            usb_hhid_smpl_get_physical_descriptor(USB_UTR_t *ptr, uint16_t devadr, uint8_t index, uint8_t *p_data,
                    uint16_t length, USB_CB_t complete);

static uint8_t  usb_shhid_smpl_trn_data[USB_HHID_KBD_TRN_LEN];      /* Receive data for HID device. */
static uint8_t  usb_shhid_set_report_data[USB_HHID_KBD_TRN_LEN];
static uint16_t usb_shhid_apl_state;     /* Application seaquence */

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
static int16_t  usb_shhid_fn;               /* Host HID file number. */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

static int16_t  usb_shhid_smpl_devaddr;     /* Hid Device address */

static uint8_t  usb_shhid_KbdKey;           /* Keyboard input valus -> Display data for debug LCD */
static uint16_t usb_shhid_report_length;    /* Report Packet Length */
static uint16_t usb_shhid_active;           /* Host HID Driver Status (Active/Not Active) */

static USB_UTR_t    usb_ghhid_utr;          /* Host HID use USB IP no. and USB IP base address */

/******************************************************************************
Renesas Host Human Intergface Devices class Sample Code functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_task_start
Description     : Start task CSTD; "common" basic USB low level task.
Arguments       : USB_UTR_t *ptr      : 
Return value    : none
******************************************************************************/
void usb_cstd_task_start( void )
{
    usb_cstd_IdleTaskStart();   /* Start idle task */

    usb_hhid_task_start();      /* Start host USB driver */

    usb_apl_task_switch();      /* Start non-OS task switching (scheduler) */
}
/******************************************************************************
End of function usb_cstd_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_hhid_task_start
Description     : Set host application task start conditions. 
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hhid_task_start( void )
{
    /* Allocate USB Communication Structure for HHID. */
    USB_UTR_t   *ptr;

#ifdef  USB_LCD_ENABLE
    /* Display Debug LCD */
    usb_cpu_LcdDisp( USB_HHID_LCD_DISP_POSITION_LINE1, "USB HHID" );
    usb_cpu_LcdDisp( USB_HHID_LCD_DISP_POSITION_LINE2, "Appstart" );
#endif /* USB_LCD_ENABLE */

    ptr = &usb_ghhid_utr;                       /* Set USB IP no. and USB IP base address store pointer */
    ptr->ip = USB_HOST_USBIP_NUM;               /* IP number(0or1) */
    if( USB_NOUSE_PP != ptr->ip )               /* USB IP number check */
    {
        ptr->ipp = R_usb_cstd_GetUsbIpAdr( ptr->ip);    /* IP address (USB0 or USB1) */

        R_usb_hstd_usbdriver_start( ptr );      /* Host USB Driver Start Setting */
        usb_hapl_registration( ptr);                /* Host application registration */
        usb_hhid_hub_registration(ptr);         /* Hub registration */
        R_usb_hhid_driver_start( ptr);              /* Host class driver task start setting */
        usb_hapl_task_start( ptr);                  /* Host application task start setting */
        R_usb_cstd_UsbIpInit( ptr, USB_HOST_PP );    /* Initialize USB IP */
    }
}
/******************************************************************************
End of function usb_hhid_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_apl_task_switch
Description     : Task switch loop for Non-OS version.
Argument        : none
Return          : none
******************************************************************************/
void usb_apl_task_switch(void)
{
    while( 1 )
    {
        /* Scheduler */
        R_usb_cstd_Scheduler();

        /* Task schedule request check. */
        if( USB_FLGSET == R_usb_cstd_CheckSchedule() )
        {
            R_usb_hstd_HcdTask((USB_VP_INT)0);          /* HCD Task */
            R_usb_hstd_MgrTask((USB_VP_INT)0);          /* MGR Task */
            R_usb_hhub_Task((USB_VP_INT)0);             /* HUB Task */
            R_usb_hhid_task((USB_VP_INT)0);             /* HHID Task */
            /* Idle Task (sleep sample) */
            R_usb_cstd_IdleTask(0);
        }
        usb_hhid_MainTask((USB_VP_INT)0);               /* HHID Application Task */
    }
}
/******************************************************************************
End of function usb_apl_task_switch()
******************************************************************************/

/******************************************************************************
Function Name   : usb_hhid_smpl_open
Description     : Host HID class open function.
Argument        : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr          : Device address.
                : uint16_t  data2           : Not use
Return value    : none
******************************************************************************/
void usb_hhid_smpl_open(USB_UTR_t *ptr, uint16_t devadr, uint16_t data2)
{
    /* Device address check */
    if(devadr != 0)
    {
        /* Device address store */
        usb_shhid_smpl_devaddr = devadr;
        /* Host HID Pipe registration */
        R_usb_hhid_SetPipeRegistration(ptr, devadr);
        /* Host HID Driver Active */
        usb_shhid_active = USB_ON;
#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
        usb_shhid_apl_state = USB_HHID_APL_INIT; /* Set Init status for Host HID application. */
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */
    }
    else
    {
        USB_PRINTF0("driver opne error\n");
    }
}   /* eof usb_hhid_smpl_open() */

/******************************************************************************
Function Name   : usb_hapl_task_start
Description     : Set host application task start conditions. 
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
Return value    : none
******************************************************************************/
void    usb_hapl_task_start( USB_UTR_t *ptr )
{
    usb_hhid_smpl_init( ptr );

    /* Host Hid demo sample application task priority set */
    R_usb_cstd_SetTaskPri(USB_HHIDSMP_TSK, USB_PRI_4);
}
/******************************************************************************
End of function usb_hapl_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_hhid_smpl_close
Description     : Host Human Interface class close function.
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t data1
                : uint16_t data2
Return value    : none
******************************************************************************/
void usb_hhid_smpl_close(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    /* Pipe Information Table (or "endpoint table" initialization. */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    usb_ghhid_SmplEpTbl[0]  = (uint16_t)USB_NONE;
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    usb_ghhid_SmplEpTbl[1]  = (uint16_t)( USB_NONE | USB_BFREOFF | USB_NONE
                                            | USB_NONE | USB_SHTNAKOFF | USB_NONE | USB_NONE );
    usb_ghhid_SmplEpTbl[2]  = (uint16_t)(USB_NONE);
    usb_ghhid_SmplEpTbl[3]  = (uint16_t)(USB_NONE);
    usb_ghhid_SmplEpTbl[4]  = (uint16_t)(USB_NONE);
    usb_ghhid_SmplEpTbl[5]  = (uint16_t)(USB_CUSE);

    /* Host HID Driver Not Active */
    usb_shhid_active = USB_OFF;
}   /* eof usb_hhid_smpl_close() */

/******************************************************************************
Function Name   : usb_hapl_registration
Description     : registration of Host HID driver.
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
Return value    : none
******************************************************************************/
void usb_hapl_registration(USB_UTR_t *ptr)
{
    USB_HCDREG_t    driver;

    /* hdriver registration */
    driver.ifclass      = (uint16_t)USB_IFCLS_HID;                  /* Interface class : HID */
    driver.tpl          = (uint16_t*)&usb_gapl_devicetpl;           /* Target peripheral list */
    driver.pipetbl      = (uint16_t*)&usb_ghhid_SmplEpTbl[0];       /* Pipe Define Table address */
    driver.classinit    = (USB_CB_INFO_t)&usb_cstd_DummyFunction;   /* Driver init */
    driver.classcheck   = (USB_CB_CHECK_t)&R_usb_hhid_class_check;  /* Driver check */
    driver.devconfig    = (USB_CB_INFO_t)&usb_hhid_smpl_open;       /* Device configuered */
    driver.devdetach    = (USB_CB_INFO_t)&usb_hhid_smpl_close;      /* Device detach */
    driver.devsuspend   = (USB_CB_INFO_t)&usb_cstd_DummyFunction;   /* Device suspend */
    driver.devresume    = (USB_CB_INFO_t)&usb_hhid_smpl_resume_cb;  /* Device resume */
    driver.overcurrent  = (USB_CB_INFO_t)&usb_cstd_DummyFunction;   /* Device Over Current*/

    R_usb_hstd_DriverRegistration(ptr, &driver);
}   /* eof usb_hapl_registration() */

/******************************************************************************
Function Name   : usb_hhid_prapltitle
Description     : Output trace title.
Argument        : none
Return          : none
******************************************************************************/
void usb_hhid_prapl_title(void)
{
#ifdef  USB_LCD_ENABLE
    /* Lcd clear */
    usb_cpu_TargetLcdClear();
#endif /* USB_LED_ENABLE */

    /* Print title */
    USB_PRINTF0("\n");
    USB_PRINTF0("++++++++++++++++++++++++++++++++++++++++++++++++\n");
    USB_PRINTF0("+  HOST Human Interface Device CLASS FW SAMPLE +\n");
    USB_PRINTF0("+                                              +\n");
    USB_PRINTF0("+          RENESAS ELECTRONICS CORP.           +\n");
    USB_PRINTF0("+          RENESAS SOLUTIONS  CORP.            +\n");
    USB_PRINTF0("++++++++++++++++++++++++++++++++++++++++++++++++\n");
    USB_PRINTF1("           DATE [%s] \n", __DATE__);
    USB_PRINTF1("           TIME [%s] \n", __TIME__);
}   /* eof usb_hhid_prapl_title() */

/******************************************************************************
Function Name   : usb_hhid_smpl_init
Description     : Initialize the host HID application.
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
Return          : none
******************************************************************************/
void usb_hhid_smpl_init(USB_UTR_t *ptr)
{
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    usb_shhid_fn = -1;                              /* Initialize file number. */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    usb_shhid_apl_state = USB_HHID_APL_CLOSE;    /* Set close state for demo sample application. */
}   /* eof usb_hhid_smpl_init() */

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
/******************************************************************************
Function Name   : usb_hhid_MainTask
Description     : Host HID sample application main process.
Argument        : uint8_t key_data             : Switch Input data
Return value    : none
******************************************************************************/
void usb_hhid_MainTask(USB_VP_INT stacd)
{
    USB_UTR_t   *ptr;
    USB_ER_t    err;
    int16_t     data_len;
#ifndef USB_HHID_GET_REPORT_PIPE0
    int16_t     state;
#endif	/* USB_HHID_GET_REPORT_PIPE0 */
    uint8_t     key_data;
    uint8_t     protocol;

    ptr = (USB_UTR_t *)&usb_ghhid_utr;
#ifdef USB_KEY_ENABLE
    key_data = usb_cpu_GetKeyNo();              /* Input RSK SW */
#endif  /* USB_KEY_ENABLE */

    /* Host HID task status check */
    if( (USB_OFF == usb_shhid_active) && (USB_HHID_APL_CLOSE != usb_shhid_apl_state) )
    {
        /* Detach detection */
        if(usb_shhid_fn != -1)
        {
            close(usb_shhid_fn);                /* Close host HID class driver. */
            usb_shhid_fn = -1;                  /* Initialize file number. */
        }

#ifdef  USB_LCD_ENABLE
        /* LCD Detach Message Display */
        usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE1, "USB     ");
        usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE2, "DETACH  ");
#endif /* USB_LCD_ENABLE */

        usb_hhid_smpl_init(ptr);                /* Initialize application */
    }

    /* Branch HHID Application State */
    switch(usb_shhid_apl_state)
    {
    /* Close Statue */
    case USB_HHID_APL_CLOSE:
        /* Host HID Open */
        usb_shhid_fn = open((int8_t *)USB_CLASS_HHID, 0, 0);
        /* Check Host HID Open complete. */
        if(usb_shhid_fn != -1)
        {
            /* Branch bInterfaceProtocol - Interface Descriptor */
            protocol = R_usb_hhid_get_hid_protocol();
            switch(protocol)
            {
            /* Keyboard */
            case    USB_HHID_IFPRO_KBD:
#ifdef  USB_LCD_ENABLE
                /* Keyboard Open Message Display */
                usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE1, "USB KBD ");
                usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE2, "ATTACH  ");
#endif /* USB_LCD_ENABLE */
                /* Application seaquence Keyboard Mode open set */
                usb_shhid_apl_state = USB_HHID_APL_KBD_OPEN;
                /* Num Lock On */
                usb_hhid_smpl_kbd_led_ctl(ptr, usb_shhid_smpl_devaddr, USB_HHID_NUM_LOCK);
                break;
            /* Mouse */
            case    USB_HHID_IFPRO_MSE:
#ifdef  USB_LCD_ENABLE
                /* Mouse Open Message Display */
                usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE1, "USB MSE ");
                usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE2, "ATTACH  ");
#endif /* USB_LCD_ENABLE */
                /* Application seaquence Mouse Mode open set */
                usb_shhid_apl_state = USB_HHID_APL_MSE_OPEN;
                break;
            default:
                break;
            }
#ifndef USB_HHID_GET_REPORT_PIPE0
            /* Set call-back function */
            control( usb_shhid_fn, USB_CTL_RD_NOTIFY_SET, (void*)&usb_hhid_smpl_data_trans_result );
#endif  /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
        }
        break;

    /* Keyboard Mode Open State */
    case USB_HHID_APL_KBD_OPEN:
    /* Mouse Mode Open State */
    case USB_HHID_APL_MSE_OPEN:
        if(key_data == USB_HHID_SW2_ON)
        {
            /* Set keyboard mode boot protocol */
            /* Protocol:Boot */
            usb_hhid_smpl_set_protocol(ptr, usb_shhid_smpl_devaddr, USB_HHID_BOOT_PROTOCOL,
                &usb_hhid_set_protocol_result);
        }
        break;

    /* Report Receive start */
    case USB_HHID_APL_KBD_RX_START:
    case USB_HHID_APL_MSE_RX_START:
        /* Get Report Packet Length */
        usb_shhid_report_length = R_usb_hhid_GetReportLength();

        /* Mouse Mode data receive wait State set */
        usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX_WAIT);
#ifdef  USB_LCD_ENABLE
        /* Initial lcd driver */
        usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE1, "        ");
        usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE2, "        ");
#endif /* USB_LCD_ENABLE */

#ifndef USB_HHID_GET_REPORT_PIPE0
        /* Interrupt IN transfer request for Peripheral HID Device */
        read(usb_shhid_fn, (uint8_t*)usb_shhid_smpl_trn_data, usb_shhid_report_length);
        /* Read status check */
        err = control( usb_shhid_fn, USB_CTL_GET_RD_STATE, (void*)&state );
        if( ( err == -1 ) || ( state != USB_CTL_ERR_PROCESS_WAIT ) )
        {
            USB_PRINTF0("### usb_hhid_MainTask function bulk read error\n");
        }
#else   /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
        /* Report receive by class request GET_REPORT. */
        usb_hhid_smpl_get_report(ptr, usb_shhid_smpl_devaddr, (uint8_t *)&usb_shhid_smpl_trn_data,
            usb_shhid_report_length, (USB_CB_t)&usb_hhid_smpl_get_report_result);
#endif  /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
        break;


    /* Control transfer complete */
    case USB_HHID_APL_KBD_CTL_END:
            /* Keyboard Mode data receive wait State set */
        usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX_WAIT);
        break;

    /* Keyboard Mode data receive wait State */
    case USB_HHID_APL_KBD_RX_WAIT:
    /* Mouse Mode data receive wait State */
    case USB_HHID_APL_MSE_RX_WAIT:
        if(key_data == USB_HHID_SW3_ON)
        {
                err = R_usb_hhid_TransferEnd(ptr);  /* data transfer end */
            if( err == USB_E_OK )
            {
                /* Suspend State set */
                usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_SUSPEND);
                /* SUSPEND */
                R_usb_hhid_ChangeDeviceState(ptr, USB_DO_GLOBAL_SUSPEND);
            }
        }
        break;

    /* Keyboard Mode data receive complete State */
    case USB_HHID_APL_KBD_RX:
    /* Mouse Mode data receive complete State */
    case USB_HHID_APL_MSE_RX:
        /* Check USB RX data count. */
        err = control( usb_shhid_fn, USB_CTL_RD_LENGTH_GET, (void*)&data_len );
        if( ( err == -1 ) || ( data_len != usb_shhid_report_length ) )
        {
            /* Transfer NG */
            /* USB_PRINTF0("### usb_hhid_MainTask function int read notify error\n"); */
        }

        /* Call data processing function for right receive data type(Keyboard/Mouse) */
        if(usb_shhid_apl_state == USB_HHID_APL_KBD_RX)
        {
            /* Keyboard data receive process */
            usb_hhid_smpl_kbd_data((uint8_t*)&usb_shhid_smpl_trn_data); /* Keyboard data receive process */
        }
        else
        {
            /* Mouse data receive process */
            usb_hhid_smpl_mse_data((uint8_t*)&usb_shhid_smpl_trn_data); /* Mouse data receive process */
        }

        /* Suspend/Resume Sw(RSK Sw3) check */
        if(key_data == USB_HHID_SW3_ON)
        {
            /* Suspend State set */
            usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_SUSPEND);
            /* SUSPEND */
            R_usb_hhid_ChangeDeviceState(ptr, USB_DO_GLOBAL_SUSPEND);
        }
        else
        {
            /* data receive wait State set */
            usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX_WAIT);
#ifndef USB_HHID_GET_REPORT_PIPE0
            /* Interrupt IN transfer request for Peripheral HID Device */
            read(usb_shhid_fn, (uint8_t*)usb_shhid_smpl_trn_data, usb_shhid_report_length);
            /* Read status check */
            err = control( usb_shhid_fn, USB_CTL_GET_RD_STATE, (void*)&state );
            if( ( err == -1 ) || ( state != USB_CTL_ERR_PROCESS_WAIT ) )
            {
                USB_PRINTF0("### usb_hhid_MainTask function bulk read error\n");
            }
#else   /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
            /* Report receive by class request GET_REPORT. */
            usb_hhid_smpl_get_report(ptr, usb_shhid_smpl_devaddr, (uint8_t *)&usb_shhid_smpl_trn_data,
                usb_shhid_report_length, (USB_CB_t)&usb_hhid_smpl_get_report_result);
#endif  /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
        }
        break;

    /* Keyboard Mode SUSPEND State */
    case USB_HHID_APL_KBD_SUSPEND:
    /* Mouse Mode SUSPEND State */
    case USB_HHID_APL_MSE_SUSPEND:
        /* Suspend/Resume Sw(RSK Sw3) check */
        if (key_data == USB_HHID_SW3_ON)
        {
            /* RESUME */
            R_usb_hhid_ChangeDeviceState(ptr, USB_DO_GLOBAL_RESUME);
        }
        break;

    /* Keyboard Mode RESUME State */
    case USB_HHID_APL_KBD_RESUME:
        /* Num Lock On */
        usb_hhid_smpl_kbd_led_ctl(ptr, usb_shhid_smpl_devaddr, USB_HHID_NUM_LOCK);
    /* Mouse Mode RESUME State */
    case USB_HHID_APL_MSE_RESUME:
        /* data receive wait State set */
        usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX_WAIT);
#ifndef USB_HHID_GET_REPORT_PIPE0
        /* Interrupt IN transfer request for Peripheral HID Device */
        read(usb_shhid_fn, (uint8_t*)usb_shhid_smpl_trn_data, usb_shhid_report_length);
        /* Read status check */
        err = control( usb_shhid_fn, USB_CTL_GET_RD_STATE, (void*)&state );
        if( ( err == -1 ) || ( state != USB_CTL_ERR_PROCESS_WAIT ) )
        {
            USB_PRINTF0("### usb_hhid_MainTask function bulk read error\n");
        }
#else   /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
            /* Report receive by class request GET_REPORT. */
        usb_hhid_smpl_get_report(ptr, usb_shhid_smpl_devaddr, (uint8_t *)&usb_shhid_smpl_trn_data,
            usb_shhid_report_length, (USB_CB_t)&usb_hhid_smpl_get_report_result);
#endif  /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
        break;

    default:
        break;
    }
}   /* eof usb_hhid_MainTask() */
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

/******************************************************************************
Function Name   : usb_hhid_MainTask
Description     : Host HID sample application main process
Argument        : uint8_t key_data             : Switch Input data
Return value    : none
******************************************************************************/
void usb_hhid_MainTask(USB_VP_INT stacd)
{
    USB_UTR_t   *ptr;
    uint8_t     key_data;
    uint8_t     protocol;

    ptr = (USB_UTR_t *)&usb_ghhid_utr;
#ifdef USB_KEY_ENABLE
    key_data = usb_cpu_GetKeyNo();                          /* Input RSK SW */
#else   /* USB_KEY_ENABLE */
    key_data = USB_OFF;
#endif  /* USB_KEY_ENABLE */

    /* Host HID task status check */
    if( (USB_OFF == usb_shhid_active) && (USB_HHID_APL_CLOSE != usb_shhid_apl_state) )
    {
#ifdef  USB_LCD_ENABLE
        /* LCD Detach Message Display */
        usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE1, "USB     ");
        usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE2, "DETACH  ");
#endif /* USB_LCD_ENABLE */

        usb_hhid_smpl_init(ptr);                            /* Initialize application */
    }

    /* Branch HHID Application State */
    switch(usb_shhid_apl_state)
    {

    /* Close Statue */
    case USB_HHID_APL_CLOSE:
        break;

    case USB_HHID_APL_INIT:
        /* Branch bInterfaceProtocol - Interface Descriptor */
        protocol = R_usb_hhid_get_hid_protocol();
        switch(protocol)
        {
        /* Keyboard */
        case    USB_HHID_IFPRO_KBD:
#ifdef  USB_LCD_ENABLE
            /* Keyboard Open Message Display */
            usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE1, "USB KBD ");
            usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE2, "ATTACH  ");
#endif /* USB_LCD_ENABLE */
            /* Application seaquence Keyboard Mode open set */
            usb_shhid_apl_state = USB_HHID_APL_KBD_OPEN;
            /* Num Lock On */
            usb_hhid_smpl_kbd_led_ctl(ptr, usb_shhid_smpl_devaddr, USB_HHID_NUM_LOCK);
            break;
        /* Mouse */
        case    USB_HHID_IFPRO_MSE:
#ifdef  USB_LCD_ENABLE
            /* Mouse Open Message Display */
            usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE1, "USB MSE ");
            usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE2, "ATTACH  ");
#endif /* USB_LCD_ENABLE */
            /* Application seaquence Mouse Mode open set */
            usb_shhid_apl_state = USB_HHID_APL_MSE_OPEN;
            break;
        default:
            break;
        }
        break;

    /* Keyboard Mode Open State */
    case USB_HHID_APL_KBD_OPEN:
    /* Mouse Mode Open State */
    case USB_HHID_APL_MSE_OPEN:
        if(key_data == USB_HHID_SW2_ON)
        {
            /* Protocol:Boot */
            usb_hhid_smpl_set_protocol(ptr, usb_shhid_smpl_devaddr, USB_HHID_BOOT_PROTOCOL,
                &usb_hhid_set_protocol_result);
        }
        break;

    /* Report Receive start */
    case USB_HHID_APL_KBD_RX_START:
    case USB_HHID_APL_MSE_RX_START:
        /* Get report packet length */
        usb_shhid_report_length = R_usb_hhid_GetReportLength();
        /* Set mouse mode data receive wait state */
        usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX_WAIT);
#ifdef  USB_LCD_ENABLE
        /* Initial LCD message */
        usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE1, "        ");
        usb_cpu_LcdDisp(USB_HHID_LCD_DISP_POSITION_LINE2, "        ");
#endif /* USB_LCD_ENABLE */

#ifndef USB_HHID_GET_REPORT_PIPE0
        /* Interrupt IN transfer request for Peripheral HID Device */
        R_usb_hhid_PipeTransferExample(ptr, (uint8_t *)&usb_shhid_smpl_trn_data,
            (uint32_t)usb_shhid_report_length, (USB_CB_t)&usb_hhid_smpl_data_trans_result);
#else   /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
        /* Report receive by class request GET_REPORT. */
        usb_hhid_smpl_get_report(ptr, usb_shhid_smpl_devaddr, (uint8_t *)&usb_shhid_smpl_trn_data,
            usb_shhid_report_length, &usb_hhid_smpl_get_report_result);
#endif  /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
        break;

    /* Control transfer complete */
    case USB_HHID_APL_KBD_CTL_END:
            /* Set keyboard mode data receive wait state */
        usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX_WAIT);
        break;

    /* Keyboard mode data receive wait state */
    case USB_HHID_APL_KBD_RX_WAIT:
        
    /* Mouse mode data receive wait state */
    case USB_HHID_APL_MSE_RX_WAIT:
        /* Check suspend/resume switch (RSK SW3) */
        if(key_data == USB_HHID_SW3_ON)
        {
            R_usb_hhid_TransferEnd(ptr);    /* data transfer end */
            /* Set suspend state */
            usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_SUSPEND);
            /* SUSPEND */
            R_usb_hhid_ChangeDeviceState(ptr, USB_DO_GLOBAL_SUSPEND);
        }
        break;

    /* Keyboard Mode data receive complete State */
    case USB_HHID_APL_KBD_RX:

    /* Mouse Mode data receive complete State */
    case USB_HHID_APL_MSE_RX:
        /* Host HID task status check */
        if(usb_shhid_apl_state == USB_HHID_APL_KBD_RX)
        {
            /* Keyboard data receive process */
            usb_hhid_smpl_kbd_data((uint8_t*)&usb_shhid_smpl_trn_data); /* Keyboard data receive process */
        }
        else
        {
            /* Mouse data receive process */
            usb_hhid_smpl_mse_data((uint8_t*)&usb_shhid_smpl_trn_data); /* Mouse data receive process */
        }

        /* Suspend/Resume Sw(RSK Sw3) check */
        if(key_data == USB_HHID_SW3_ON)
        {
            /* Suspend State set */
            usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_SUSPEND);
            /* SUSPEND */
            R_usb_hhid_ChangeDeviceState(ptr, USB_DO_GLOBAL_SUSPEND);
        }
        else
        {
            /* data receive wait State set */
            usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX_WAIT);
#ifndef USB_HHID_GET_REPORT_PIPE0
            /* Interrupt IN transfer request for Peripheral HID Device */
            R_usb_hhid_PipeTransferExample(ptr, (uint8_t *)&usb_shhid_smpl_trn_data,
                (uint32_t)usb_shhid_report_length, (USB_CB_t)&usb_hhid_smpl_data_trans_result);
#else   /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
            /* Report receive by class request GET_REPORT. */
            usb_hhid_smpl_get_report(ptr, usb_shhid_smpl_devaddr, (uint8_t *)&usb_shhid_smpl_trn_data,
                usb_shhid_report_length, &usb_hhid_smpl_get_report_result);
#endif  /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
        }
        break;

    /* Keyboard Mode SUSPEND State */
    case USB_HHID_APL_KBD_SUSPEND:
    /* Mouse Mode SUSPEND State */
    case USB_HHID_APL_MSE_SUSPEND:
        /* Suspend/Resume Sw(RSK Sw3) check */
        if (key_data == USB_HHID_SW3_ON)
        {
            /* RESUME */
            R_usb_hhid_ChangeDeviceState(ptr, USB_DO_GLOBAL_RESUME);
        }
        break;

    /* Keyboard Mode RESUME State */
    case USB_HHID_APL_KBD_RESUME:
        /* Num Lock On */
        usb_hhid_smpl_kbd_led_ctl(ptr, usb_shhid_smpl_devaddr, USB_HHID_NUM_LOCK);
    /* Mouse Mode RESUME State */
    case USB_HHID_APL_MSE_RESUME:
        /* data receive wait State set */
        usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX_WAIT);
#ifndef USB_HHID_GET_REPORT_PIPE0
            /* Interrupt IN transfer request for Peripheral HID Device */
            R_usb_hhid_PipeTransferExample(ptr, (uint8_t *)&usb_shhid_smpl_trn_data,
                (uint32_t)usb_shhid_report_length, (USB_CB_t)&usb_hhid_smpl_data_trans_result);
#else   /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
            /* Report receive by class request GET_REPORT. */
        usb_hhid_smpl_get_report(ptr, usb_shhid_smpl_devaddr, (uint8_t *)&usb_shhid_smpl_trn_data,
            usb_shhid_report_length, &usb_hhid_smpl_get_report_result);
#endif  /* USB_HHID_GET_REPORT_PIPE0 == USB_OFF */
        break;

    default:
        break;
    }
}   /* eof usb_hhid_MainTask() */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

/******************************************************************************
Function Name   : usb_hhid_smpl_data_trans_result
Description     : data transfer complete process.
Argument        : USB_UTR_t *mess           : message
Return value    : none
******************************************************************************/
void usb_hhid_smpl_data_trans_result(USB_UTR_t *mess)
{
    /* Host HID task status check */
    if( (usb_shhid_apl_state == USB_HHID_APL_KBD_RX_WAIT) || (usb_shhid_apl_state == USB_HHID_APL_MSE_RX_WAIT))
    {
        /* Branch for Transfer Result */
        switch(mess->status)
        {
        /* data receive */
        case    USB_DATA_OK:
        case    USB_DATA_READING:
        case    USB_DATA_SHT:
            /* Host HID Application State receive complete */
            usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX);
            break;

        /* data transmission */
        case    USB_DATA_NONE:
        case    USB_DATA_WRITING:
            break;

        /* Control transfer stop(end) */
        case    USB_CTRL_END:
            /* Host HID Application State control transmission complete */
            usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_CTL_END);
            break;

        case    USB_DATA_OVR:
            /* Error Stop */
            while(1);
            break;

        case    USB_DATA_STALL:
            /* Error Stop */
            while(1);
            break;

        case    USB_DATA_STOP:
        case    USB_DATA_ERR:
            break;

        default:
            /* Error Stop */
            while(1);
            break;
        }
    }
}   /* eof usb_hhid_smpl_data_trans_result() */

/******************************************************************************
Function Name   : usb_hhid_set_protocol_result
Description     : Set Protocol Transfer complete process
Argument        : USB_UTR_t *mess           : message
Return value    : none
******************************************************************************/
void usb_hhid_set_protocol_result(USB_UTR_t *mess, uint16_t devadr, uint16_t data2)
{
    /* Host HID Task status check */
    if( (usb_shhid_apl_state == USB_HHID_APL_KBD_OPEN) || (usb_shhid_apl_state == USB_HHID_APL_MSE_OPEN))
    {
        /* Report Receive start */
        usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX_START);
    }
}   /* eof usb_hhid_set_protocol_result() */

/******************************************************************************
Function Name   : usb_hhid_smpl_mse_data
Description     : Mouse data receive process.
Argument        : uint8_t *data                :USB Mouse Data
Return value    : none
******************************************************************************/
void usb_hhid_smpl_mse_data(uint8_t *data)
{
    uint8_t     pdata[8];
    uint8_t     up_down;
    uint8_t     left_right;

    /* USB mouse move check */
    if((data[USB_HHID_MSE_X_DATA_INDEX] != USB_HHID_MSE_NOT_MOVE)
        || (data[USB_HHID_MSE_Y_DATA_INDEX] != USB_HHID_MSE_NOT_MOVE))
    {
        left_right  = data[USB_HHID_MSE_X_DATA_INDEX];          /* Set x-axis */
        up_down     = data[USB_HHID_MSE_Y_DATA_INDEX];          /* Set y-axis */
        usb_hhid_smpl_val_to_str((uint8_t *)&pdata,(int8_t)left_right);     /* Change Type to Character */
#ifdef  USB_LCD_ENABLE
        usb_cpu_LcdDisp(USB_HHID_DISP_X_AXIS_POSITION, (uint8_t *)&pdata);  /* LCD display x-axis */
#endif /* USB_LCD_ENABLE */
        usb_hhid_smpl_val_to_str((uint8_t *)&pdata,(int8_t)up_down);        /* Change Type to Character */
#ifdef  USB_LCD_ENABLE
        usb_cpu_LcdDisp(USB_HHID_DISP_Y_AXIS_POSITION, (uint8_t *)&pdata);  /* LCD display y-axis */
#endif /* USB_LCD_ENABLE */
    }

#ifdef  USB_LED_ENABLE
    /* USB MOUSE LEFT BUTTON off/on check */
    if ((data[USB_HHID_MSE_BTN_DATA_INDEX] & USB_HHID_LEFT_BUTTON) != USB_OFF)
    {
        /* LED ON - assign for LEFT BUTTON */
        usb_cpu_LedSetBit(USB_HHID_LED_POS_LEFT_BTN,(uint8_t)USB_ON);
    }
    else
    {
        /* LED OFF - assign for LEFT BUTTON */
        usb_cpu_LedSetBit(USB_HHID_LED_POS_LEFT_BTN,(uint8_t)USB_OFF);
    }

    /* USB MOUSE RIGHT BUTTON off/on check */
    if ((data[USB_HHID_MSE_BTN_DATA_INDEX] & USB_HHID_RIGHT_BUTTON) != USB_OFF)
    {
        /* LED ON - assign for RIGHT BUTTON */
        usb_cpu_LedSetBit(USB_HHID_LED_POS_RIGHT_BTN,(uint8_t)USB_ON);
    }
    else
    {
        /* LED OFF - assign for RIGHT BUTTON */
        usb_cpu_LedSetBit(USB_HHID_LED_POS_RIGHT_BTN,(uint8_t)USB_OFF);
    }

    /* USB MOUSE WHEEL BUTTON off/on check */
    if ((data[USB_HHID_MSE_BTN_DATA_INDEX] & USB_HHID_WHEEL_BUTTON) != USB_OFF)
    {
        /* LED ON - assign for WHEEL BUTTON */
        usb_cpu_LedSetBit(USB_HHID_LED_POS_WHEEL_BTN,(uint8_t)USB_ON);
    }
    else
    {
        /* LED OFF - assign for WHEEL BUTTON */
        usb_cpu_LedSetBit(USB_HHID_LED_POS_WHEEL_BTN,(uint8_t)USB_OFF);
    }
#endif /* USB_LED_ENABLE */
}   /* eof usb_hhid_smpl_mse_data() */

/******************************************************************************
Function Name   : usb_hhid_smpl_val_to_str
Description     : Convert value to string.
Argument        : uint8_t *data     :Output string data
                : int8_t val        :Input value
Return value    : none
******************************************************************************/
void usb_hhid_smpl_val_to_str(uint8_t *data,int8_t val)
{
    /* Output String data init */
    data[0] = ' ';
    data[1] = ' ';
    data[2] = ' ';
    data[3] = ' ';
    data[4] = 0x00;

    /* Minus check */
    if( val < 0 )
    {
        /* Sign inversion */
        val ^= 0xff;
        val += 1; 

        /* Value check hundreds place */
        if( val >= 100 )
        {
            data[0] = '-';  /* Minus Mark set */
        }
        else
        {
            /* Value check tens place */
            if( val >= 10 )
            {
                data[1] = '-';  /* Minus Mark set */
            }
            else
            {
                data[2] = '-';  /* Minus Mark set */
            }
        }
    }

    /* Value check hundreds place */
    if( val >= 100 )
    {
        data[1] = '1';                  /* Value set hundreds place */
        data[2] = '0';                  /* Value set tens place */
        val -= 100;                     /* Update Value */
    }

    /* Value check tens place */
    if( val >= 10 )
    {
        data[2] = 0x30 + (val / 10);    /* Value set tens place */
        val %= 10;                      /* Update Value */
    }

    data[3] = 0x30 + val;               /* Value set Place of one */
}   /* eof usb_hhid_smpl_val_to_str() */

/******************************************************************************
Function Name   : usb_hhid_smpl_kbd_data
Description     : Keyboard data receive process.
Argument        : uint8_t *data                :USB Keyboard Data
Return value    : none
******************************************************************************/
void usb_hhid_smpl_kbd_data(uint8_t *data)
{
#ifdef  USB_LCD_ENABLE
    uint8_t     pdata[4];   /* LCD Display data  */

    pdata[0]    = ' ';
    pdata[1]    = ' ';
    pdata[2]    = '\0';
#endif /* USB_LCD_ENABLE */

    /* Keyboard input data check */
    if((data[USB_HHID_KBD_KEYCODE1_INDEX] != USB_HHID_KBD_NOT_PUSH) 
        && (data[USB_HHID_KBD_KEYCODE1_INDEX]!= usb_shhid_KbdKey))
    {
        usb_shhid_KbdKey    = data[USB_HHID_KBD_KEYCODE1_INDEX];
        /* Keyboard input data check 'A' to 'Z' */
        if( usb_shhid_KbdKey < USB_HHID_KBD_CODE_1 )
        {
            /* ASCII CODE SET a to z */
            usb_shhid_KbdKey    += USB_HHID_KEYCODE_TO_ASCII_OFFSET0;
        }
        /* Keyboard input data check '1' to '9' */
        else if( usb_shhid_KbdKey < USB_HHID_KBD_CODE_0 )
        {
            /* ASCII CODE SET 1 to 9 */
            usb_shhid_KbdKey    += USB_HHID_KEYCODE_TO_ASCII_OFFSET1;
        }
        /* Keyboard input data check '0' */
        else if( usb_shhid_KbdKey == USB_HHID_KBD_CODE_0 )
        {
            /* ASCII CODE SET 0 */
            usb_shhid_KbdKey    += USB_HHID_KEYCODE_TO_ASCII_OFFSET2;
        }
        /* Keyboard input data check BackSpace */
        else if( usb_shhid_KbdKey == USB_HHID_KBD_CODE_BS )
        {
            /* ASCII CODE SET 'BS' */
            usb_shhid_KbdKey    = USB_HHID_ASCII_BS;
            USB_PRINTF0("\b");
        }
        /* Keyboard input data check Space' ' */
        else if( usb_shhid_KbdKey == USB_HHID_KBD_CODE_SPACE )
        {
            /* ASCII CODE SET ' ' */
            usb_shhid_KbdKey    = ' ';
        }
        /* Keyboard input data check '-' */
        else if( usb_shhid_KbdKey == USB_HHID_KBD_CODE_HYPHEN )
        {
            /* ASCII CODE SET '-' */
            usb_shhid_KbdKey    = '-';
        }
        /* Keyboard input data check Enter */
        else if(usb_shhid_KbdKey == USB_HHID_KBD_CODE_ENTER )
        {
            /* ASCII CODE SET '\n' */
            usb_shhid_KbdKey    = '\r';
            USB_PRINTF0("\n");
        }
        /* Keyboard input data check ':' */
        else if( usb_shhid_KbdKey == USB_HHID_KBD_CODE_COLON )
        {
            /* ASCII CODE SET ':' */
            usb_shhid_KbdKey    = ':';
        }
        /* Keyboard input data check '.' */
        else if( usb_shhid_KbdKey == USB_HHID_KBD_CODE_DOT )
        {
            /* ASCII CODE SET '.' */
            usb_shhid_KbdKey    = '.';
        }
        /* Keyboard input data check '/' */
        else if( usb_shhid_KbdKey == USB_HHID_KBD_CODE_DIAGONAL )
        {
            /* ASCII CODE SET '/' */
            usb_shhid_KbdKey    = '/';
        }
        /* Keyboard input data check '_' */
        else if( usb_shhid_KbdKey == USB_HHID_KBD_CODE_UNDERSCORE )
        {
            /* ASCII CODE SET '_' */
            usb_shhid_KbdKey    = '_';
        }
        /* Keyboard input data check '\' */
        else if( usb_shhid_KbdKey == USB_HHID_KBD_CODE_YEN )
        {
            /* ASCII CODE SET '\' */
            usb_shhid_KbdKey    = '\\';
        }
        else
        {
            usb_shhid_KbdKey    = '*';
        }
#ifdef  USB_LCD_ENABLE
        pdata[1]    = (uint8_t)usb_shhid_KbdKey;
        /* LCD display Keyboard input data */
        usb_cpu_LcdDisp(USB_HHID_DISP_KEY_POSITION, (uint8_t *)&pdata);
#endif /* USB_LCD_ENABLE */
    }
}   /* eof usb_hhid_smpl_kbd_data() */

/******************************************************************************
Function Name   : usb_hhid_smpl_resume_cb
Description     : Resume complete callback.
Arguments       : uint16_t data1            : Not used.
                : uint16_t data2            : Not used.
Return value    : none
******************************************************************************/
void usb_hhid_smpl_resume_cb(uint16_t data1, uint16_t data2)
{
    /* Resume State set */
    usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RESUME);
}   /* eof usb_hhid_smpl_resume_cb() */

/******************************************************************************
Function Name   : usb_hhid_smpl_kbd_led_ctl
Description     : USB Keyboard LED control(NumLock,CapsLock,ScrollLock)
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr  : Device address.
                : uint16_t mode             : Set light keyboard LED.(NUM_LOCK/CAPS_LOCK/SCROLL_LOCK)
Return value    : none
******************************************************************************/
void usb_hhid_smpl_kbd_led_ctl(USB_UTR_t *ptr, uint16_t devadr, uint16_t mode)
{
    usb_shhid_set_report_data[0] = (uint8_t)mode;

    /* Host HID class request SetReport */
    usb_hhid_smpl_set_report(ptr, devadr, (uint8_t *)&usb_shhid_set_report_data, 1, &usb_hhid_smpl_kbd_led_ctl_result);
}   /* eof usb_hhid_smpl_kbd_led_ctl() */

#ifdef USB_HHID_GET_REPORT_PIPE0
/******************************************************************************
Function Name   : usb_hhid_smpl_get_report_result
Description     : Debug Callbask(Get_Report Request)
Arguments       : USB_UTR_t *mess   : Message
Return value    : none
******************************************************************************/
void usb_hhid_smpl_get_report_result(USB_UTR_t *mess, uint16_t data1, uint16_t data2)
{
    /* Get_Report transfer complete check */
    if( mess->status == USB_CTRL_END )
    {
        /* Host HID Application State receive complete */
        usb_hhid_smp_status_set(USB_HHID_APL_OFFSET_RX);
    }
}   /* eof usb_hhid_smpl_get_report_result() */
#endif /* USB_HHID_GET_REPORT_PIPE0 */

/******************************************************************************
Function Name   : usb_hhid_smp_status_set
Description     : Host HID Sample Application Status Update
Argument        : uint16_t status           : Set Status code
Return          : none
******************************************************************************/
void usb_hhid_smp_status_set(uint16_t status)
{
    usb_shhid_apl_state &= USB_HHID_APL_MODE_MASK;
    usb_shhid_apl_state |= status;
}   /* eof usb_hhid_smp_status_set() */

/******************************************************************************
Function Name   : usb_hhid_smpl_get_hid_descriptor
Description     : GET HID Descriptor
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr      : Device address.
                : uint16_t  *p_data     : Descriptor data storage address
                : USB_CB_t  complete          : Callback function
Return value    : none
******************************************************************************/
void usb_hhid_smpl_get_hid_descriptor(USB_UTR_t *ptr, uint16_t devadr, uint8_t *p_data, USB_CB_t complete)
{
    USB_HHID_CLASS_REQUEST_PARM_t   class_req;

    class_req.bRequestCode = USB_HID_GET_HID_DESCRIPTOR;        /* Set class request setup packet bRequest */

    class_req.devadr = devadr;                                  /* Device address */
    class_req.ip = ptr->ip;                                     /* IP number(0or1) */
    class_req.ipp = ptr->ipp;                                   /* IP address(USB0orUSB1) */
    class_req.tranadr = p_data;                                 /* HID Descriptor data storage address */
    class_req.complete = complete;                              /* Set callback function */

    /* HID class request */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    control(CLASS_REQUEST_FN, USB_CTL_CLASS_REQUEST, (void*)&class_req );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_hhid_class_request( (void*)&class_req );              /* Issue host HID class request */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_smpl_get_hid_descriptor() */

/******************************************************************************
Function Name   : usb_hhid_smpl_get_report_descriptor
Description     : GET HID Report Descriptor
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr      : Device address.
                : uint16_t  *p_data     : Descriptor data storage address
                : uint16_t  length           : Descriptor length
                : USB_CB_t  complete         : Callback function
Return value    : none
******************************************************************************/
void usb_hhid_smpl_get_report_descriptor(USB_UTR_t *ptr, uint16_t devadr, uint8_t *p_data, uint16_t length,
    USB_CB_t complete)
{
    USB_HHID_CLASS_REQUEST_PARM_t   class_req;

    class_req.bRequestCode = USB_HID_GET_REPORT_DESCRIPTOR;     /*  Set class request setup packet bRequest */

    class_req.devadr = devadr;                                  /* Device address */
    class_req.ip = ptr->ip;                                     /* IP number(0or1) */
    class_req.ipp = ptr->ipp;                                   /* IP address(USB0orUSB1) */
    class_req.tranadr = p_data;                                 /* Report descriptor data storage address */
    class_req.tranlen = length;                                 /* Report descriptor length */
    class_req.complete = complete;                              /* Set callback function */

    /* HID class request */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    control(CLASS_REQUEST_FN, USB_CTL_CLASS_REQUEST, (void*)&class_req );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_hhid_class_request( (void*)&class_req );              /* Issue host HID class request */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_smpl_get_report_descriptor() */

/******************************************************************************
Function Name   : usb_hhid_smpl_get_physical_descriptor
Description     : Get HID physical descriptor.
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr          : Device address.
                : uint8_t index             : 
                : uint16_t *p_data          : Descriptor data storage address
                : uint16_t length           : Descriptor length
                : USB_CB_t complete         : Callback function
Return value    : none
******************************************************************************/
void usb_hhid_smpl_get_physical_descriptor(USB_UTR_t *ptr, uint16_t devadr, uint8_t index, uint8_t *p_data,
    uint16_t length, USB_CB_t complete)
{
    USB_HHID_CLASS_REQUEST_PARM_t   class_req;

    class_req.bRequestCode = USB_HID_GET_PHYSICAL_DESCRIPTOR;   /* Set class request setup packet bRequest */

    class_req.devadr = devadr;                                  /* Device address */
    class_req.ip = ptr->ip;                                     /* IP number(0or1) */
    class_req.ipp = ptr->ipp;                                   /* IP address(USB0orUSB1) */
    class_req.tranadr = p_data;                                 /* Physical descriptor data storage address */
    class_req.tranlen = length;                                 /* Physical descriptor length */
    class_req.complete = complete;                              /* Set callback function */
    class_req.index = index;                                    /* Descriptor index */

    /* HID class request */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    control(CLASS_REQUEST_FN, USB_CTL_CLASS_REQUEST, (void*)&class_req );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_hhid_class_request( (void*)&class_req );              /* Issue host HID class request */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_smpl_get_physical_descriptor() */

/******************************************************************************
Function Name   : usb_hhid_smpl_set_report
Description     : Host HID class request SetReport
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr          : Device address.
                : uint16_t  *p_data          : Command data top address
                : uint16_t  length           : Command length
                : USB_CB_t  complete         : Callback function
Return value    : none
******************************************************************************/
void usb_hhid_smpl_set_report(USB_UTR_t *ptr, uint16_t devadr, uint8_t *p_data, uint16_t length, USB_CB_t complete)
{
    USB_HHID_CLASS_REQUEST_PARM_t   class_req;

    class_req.bRequestCode = USB_HID_SET_REPORT;                /* Set class request setup packet bRequest */

    class_req.devadr = devadr;                                  /* Device address */
    class_req.ip = ptr->ip;                                     /* IP number(0or1) */
    class_req.ipp = ptr->ipp;                                   /* IP address(USB0orUSB1) */
    class_req.tranadr = p_data;                                 /* Command data top address */
    class_req.tranlen = length;                                 /* Command length */
    class_req.complete = complete;                              /* Set callback function */

    /* HID class request */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    control(CLASS_REQUEST_FN, USB_CTL_CLASS_REQUEST, (void*)&class_req );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_hhid_class_request( (void*)&class_req );              /* Issue host HID class request */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_smpl_set_report() */

/******************************************************************************
Function Name   : usb_hhid_smpl_get_report
Description     : Host HID class request GetReport
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr          : Device address.
                : uint16_t  *p_data          : Report storage address
                : uint16_t  length           : Report receive size
                : USB_CB_t  complete         : Callback function
Return value    : none
******************************************************************************/
void usb_hhid_smpl_get_report(USB_UTR_t *ptr, uint16_t devadr, uint8_t *p_data, uint16_t length, USB_CB_t complete)
{
    USB_HHID_CLASS_REQUEST_PARM_t   class_req;

    class_req.bRequestCode = USB_HID_GET_REPORT;            /* Class request setup packet bRequest Set */

    class_req.devadr = devadr;                              /* Device address */
    class_req.ip = ptr->ip;                                 /* IP number(0or1) */
    class_req.ipp = ptr->ipp;                               /* IP Address(USB0orUSB1) */
    class_req.tranadr = p_data;                             /* Report storage address */
    class_req.tranlen = length;                             /* Report receive size */
    class_req.complete = complete;                          /* Set callback function */

    /* HID class request */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    control(CLASS_REQUEST_FN, USB_CTL_CLASS_REQUEST, (void*)&class_req );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_hhid_class_request( (void*)&class_req );          /* Issue host HID class request */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_smpl_get_report() */

/******************************************************************************
Function Name   : usb_hhid_smpl_set_idle
Description     : Host HID class request SetIdle
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr          : Device address.
                : uint16_t  duration        : Report interval(4msec unit )
                : USB_CB_t  complete        : Callback function
Return value    : none
******************************************************************************/
void usb_hhid_smpl_set_idle(USB_UTR_t *ptr, uint16_t devadr, uint16_t duration, USB_CB_t complete)
{
    USB_HHID_CLASS_REQUEST_PARM_t   class_req;

    class_req.bRequestCode = USB_HID_SET_IDLE;                  /* Class request setup packet bRequest Set */

    class_req.devadr = devadr;                                  /* Device address set */
    class_req.ip = ptr->ip;                                     /* IP number(0or1) */
    class_req.ipp = ptr->ipp;                                   /* IP address(USB0orUSB1) */
    class_req.tranadr = NULL;                                   /* No-data control */
    class_req.tranlen = 0;                                      /* data stage transfer size 0 */
    class_req.duration = duration;                              /* Report interval */
    class_req.complete = complete;                              /* Set callback function */

    /* HID class request */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    control(CLASS_REQUEST_FN, USB_CTL_CLASS_REQUEST, (void*)&class_req );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_hhid_class_request( (void*)&class_req );              /* Issue host HID class request */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_smpl_set_idle() */

/******************************************************************************
Function Name   : usb_hhid_smpl_get_idle
Description     : Host HID class request GetIdle
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr          : Device address.
                : uint16_t  *idle_rate        : Report interval(4msec unit)storage address
                : USB_CB_t  complete          : Callback function
Return value    : none
******************************************************************************/
void usb_hhid_smpl_get_idle(USB_UTR_t *ptr, uint16_t devadr, uint16_t *idle_rate, USB_CB_t complete)
{
    USB_HHID_CLASS_REQUEST_PARM_t   class_req;

    class_req.bRequestCode = USB_HID_GET_IDLE;                  /* Class request setup packet bRequest Set */

    class_req.devadr = devadr;                                  /* Device address */
    class_req.ip = ptr->ip;                                     /* IP number(0or1) */
    class_req.ipp = ptr->ipp;                                   /* IP address(USB0orUSB1) */
    class_req.tranadr = idle_rate;                              /* Report interval storage address */
    class_req.tranlen = 1;                                      /* Idle rate length 1 byte */
    class_req.complete = complete;                              /* Set callback function */

    /* HID class request */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    control(CLASS_REQUEST_FN, USB_CTL_CLASS_REQUEST, (void*)&class_req );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_hhid_class_request( (void*)&class_req );              /* Issue host HID class request */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_smpl_get_idle() */

/******************************************************************************
Function Name   : usb_hhid_smpl_set_protocol
Description     : Host HID class request SetReport
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr          : Device address.
                : uint16_t  *protocol         : Protocol(0:Boot,1:Report)
                : USB_CB_t  complete          : Callback function
Return value    : none
******************************************************************************/
void usb_hhid_smpl_set_protocol(USB_UTR_t *ptr, uint16_t devadr, uint8_t protocol, USB_CB_t complete)
{
    USB_HHID_CLASS_REQUEST_PARM_t   class_req;

    class_req.bRequestCode = USB_HID_SET_PROTOCOL;              /* Class request setup packet bRequest Set */

    class_req.devadr = devadr;                                  /* Device address */
    class_req.ip = ptr->ip;                                     /* IP number(0or1) */
    class_req.ipp = ptr->ipp;                                   /* IP address(USB0orUSB1) */
    class_req.set_protocol = protocol;                          /* Protocol(0:Boot,1:Report) */
    class_req.tranadr = NULL;                                   /* No-data control */
    class_req.tranlen = 0;                                      /* data stage transfer size 0 */
    class_req.complete = complete;                              /* Set callback function */

    /* HID class request */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    control(CLASS_REQUEST_FN, USB_CTL_CLASS_REQUEST, (void*)&class_req );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_hhid_class_request( (void*)&class_req );              /* Issue host HID class request */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_smpl_set_protocol() */

/******************************************************************************
Function Name   : usb_hhid_smpl_get_report
Description     : Host HID class request GetReport
Arguments       : USB_UTR_t *ptr : USB system internal structure. Selects channel.
                : uint16_t devaddr          : Device address.
                : uint16_t  *protocol         : Protocol(0:Boot,1:Report)storage address
                : USB_CB_t  complete          : Callback function
Return value    : none
******************************************************************************/
void usb_hhid_smpl_get_protocol(USB_UTR_t *ptr, uint16_t devadr, uint8_t *protocol, USB_CB_t complete)
{
    USB_HHID_CLASS_REQUEST_PARM_t   class_req;

    class_req.bRequestCode = USB_HID_GET_PROTOCOL;              /* Class request setup packet bRequest Set */

    class_req.devadr = devadr;                                  /* Device address */
    class_req.ip = ptr->ip;                                     /* IP number(0or1) */
    class_req.ipp = ptr->ipp;                                   /* IP address(USB0orUSB1) */
    class_req.tranlen = 1;                                      /* Protocol Information size 1Byte */
    class_req.tranadr = protocol;                               /* Set Protocol(0:Boot,1:Report)storage address */
    class_req.complete = complete;                              /* Set callback function */

    /* HID class request */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    control(CLASS_REQUEST_FN, USB_CTL_CLASS_REQUEST, (void*)&class_req );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_hhid_class_request( (void*)&class_req );              /* Issue host HID class request */
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_hhid_smpl_get_protocol() */

/******************************************************************************
Function Name   : usb_hhid_smpl_kbd_led_ctl_result
Description     : CallBack Function
Argument        : USB_UTR_t *mess
Return value    : none
******************************************************************************/
void usb_hhid_smpl_kbd_led_ctl_result(USB_UTR_t *mess, uint16_t data1, uint16_t data2)
{
}   /* eof usb_hhid_smpl_kbd_led_ctl_result() */

/******************************************************************************
End  Of File
******************************************************************************/

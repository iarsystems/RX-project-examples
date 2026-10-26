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
* File Name    : r_usb_PHID_apl.c
* Version      : 1.10
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Common to None and uITRON
* H/W Platform : Independent
* Description  : USB HID application code
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 30.07.2010 0.90    First Release
*         : 30.11.2010 0.95    USBSTDFW(Mass Production Release)
*         : 31.03.2011 1.00    Mass Production Release
*         : 24.06.2011 1.10    Version 1.10 Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <string.h>
#include "r_usb_ctypedef.h"     /* Type define */
#include "r_usb_usrconfig.h"
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_ckernelid.h"        /* Kernel ID definition */
#include "r_usb_cmacsystemcall.h"   /* uITRON system call macro */
#include "r_usb_cextern.h"          /* USB-FW global define */

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
#include "r_usb_ansi.h"
#include "r_usb_control.h"
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

#include "r_usb_cusb_bitdefine.h"

#include "r_usb_api.h"

#include "r_usb_phid_define.h"
#include "r_usb_class_usrcfg.h"
#include "r_usb_phid_extern.h"

#include "rx_rsk_extern.h"
#include "hw_resource.h"

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_apl"

/******************************************************************************
Private global variables and functions
******************************************************************************/
static  void        usb_phid_apl_init(USB_UTR_t *ptr);
static  void        usb_phid_apl_clear(USB_UTR_t *ptr);
static  void        usb_phid_apl_key_on_wakeup(USB_UTR_t *ptr);
static  void        usb_phid_smpl_message_send(USB_UTR_t *ptr, uint16_t msginfo);
void                usb_phid_main_task(USB_VP_INT stacd);
static  void        usb_phid_apl_trans_cb(USB_UTR_t *mess);
static  void        usb_phid_task_start( void );
static  void        usb_apl_task_switch(void);
static  void        usb_phid_driver_registration(USB_UTR_t *ptr);
static  USB_ER_t    usb_phid_apl_open( USB_UTR_t *ptr, uint16_t data1, uint16_t data2 );
static  USB_ER_t    usb_phid_apl_close( USB_UTR_t *ptr, uint16_t data1, uint16_t data2 );

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
void                usb_phid_demo_mode_report( USB_UTR_t *ptr, uint8_t *recdata );
static  void        usb_phid_apl_create_report(uint8_t badc, uint8_t bswpressed, uint32_t adcvalue);
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
void                usb_phid_apl_keyboard_led_control(uint8_t leddata);
static  uint16_t    usb_phid_apl_kbd_data_set(uint8_t key_data, uint8_t *data);
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
static  uint16_t    usb_phid_apl_mse_data_set(uint8_t key_data, uint8_t *data);
#endif  /* Peripheral HID Application mouse mode */


#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
static int16_t          usb_sphid_fn;
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

static uint16_t         usb_gphid_connected;    /* PHID application enable */

/* Application seaquence */
/* [0:Port0] Peri=0x01-,Host=0x81-  [1:Port1] Peri=0x01-,Host=0x81- */
static  uint16_t        usb_gphid_application_state;

/* Common global area definition */
static  uint8_t         usb_sphid_smpl_trn_data[8];

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
static uint8_t          usb_sphid_keyboard_key;
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
static uint16_t         usb_sphid_mouse_up_down;
static uint16_t         usb_sphid_mouse_left_right;
static uint8_t          usb_sphid_check_switch;
static uint16_t         usb_sphid_mouse_time;
#endif  /* Peripheral HID Application mouse mode */

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
static uint8_t          usb_sphid_led0;
#endif  /* Peripheral HID Application demo mode */

/*****************************************************************************
Public global variables and functions
******************************************************************************/
void        usb_phid_prapl_title(void);

uint16_t    usb_phid_isconnected(void);
/******************************************************************************
Constant macro definitions
******************************************************************************/
#define NO_PUSH                     0
#define SW2_PUSH                    2
#define SW3_PUSH                    4

/* USB HID Keyboard Usage ID */
#define USB_PHID_KBD_CODE_A         0x04    /* a */
#define USB_PHID_KBD_CODE_Z         0x1D    /* z */
#define USB_PHID_KBD_CODE_1         0x1E    /* 1 */
#define USB_PHID_KBD_CODE_0         0x27    /* 0 */
#define USB_PHID_KBD_CODE_ENTER     0x28    /* '\n' */
#define USB_PHID_KBD_KEY_DATA_INDEX 0x00    /* Keyboard data store index */

/* HID Demo Defines */
#define USB_DEMO_REPORTLED          0x01    /* Toggle LED */
#define USB_DEMO_REPORTADC          0x02    /* Read ADC */
#define USB_DEMO_REPORTLCD          0x04    /* Update LCD */

/* USB Mouse Defines */
#define USB_MSE_SW_PUSH_THRESHOLD_LOW   0x0100  /* RSK Switch push time(count) threshold low. */
#define USB_MSE_SW_PUSH_THRESHOLD_HIGH  0x0400  /* RSK Switch push time(count) threshold high. */

/* Mouse Data Store Position Index */
#define USB_PHID_MSE_BTN_DATA_INDEX     0x00    /* Input Key */
#define USB_PHID_MSE_X_DATA_INDEX       0x01    /* X axis */
#define USB_PHID_MSE_Y_DATA_INDEX       0x02    /* Y axis */

#define USB_PHID_MSE_UP_DISTANSE_MIN    0x01    /* Mouse Y axis distanse +1 */
#define USB_PHID_MSE_UP_DISTANSE_MID    0x02    /* Mouse Y axis distanse +2 */
#define USB_PHID_MSE_UP_DISTANSE_MAX    0x04    /* Mouse Y axis distanse +4 */
#define USB_PHID_MSE_DOWN_DISTANSE_MIN  0xFF    /* Mouse Y axis distanse -1 */
#define USB_PHID_MSE_DOWN_DISTANSE_MID  0xFE    /* Mouse Y axis distanse -2 */
#define USB_PHID_MSE_DOWN_DISTANSE_MAX  0xFC    /* Mouse Y axis distanse -4 */
#define USB_PHID_MSE_RIGHT_DISTANSE_MIN 0x01    /* Mouse X axis distanse +1 */
#define USB_PHID_MSE_RIGHT_DISTANSE_MID 0x02    /* Mouse X axis distanse +2 */
#define USB_PHID_MSE_RIGHT_DISTANSE_MAX 0x04    /* Mouse X axis distanse +4 */
#define USB_PHID_MSE_LEFT_DISTANSE_MIN  0xFF    /* Mouse X axis distanse -1 */
#define USB_PHID_MSE_LEFT_DISTANSE_MID  0xFE    /* Mouse X axis distanse -2 */
#define USB_PHID_MSE_LEFT_DISTANSE_MAX  0xFC    /* Mouse X axis distanse -4 */

/* -------------------------- */
#define NUMB_CHARS_PER_LINE         8u

/* LED state (1 = ON, 0 = OFF) */
#define IN_BIT_LED                  0x01
/* ADC data is valid */
#define IN_BIT_ADC                  0x02
/* A switch has been pressed */
#define IN_BIT_SW                   0x04

/*****************************************************************************
Enumerated Types
******************************************************************************/
/* Peripheral HID Sample application message command */
typedef enum
{
    USB_PHID_INIT = 1,          /* Open */
    USB_PHID_CLOSE,             /* Close */
    USB_PHID_PERIODIC_PROCESS   /* Periodic processing */
}
USB_PHID_APL_COMMAND;

/* ----- Peripheral HID Application State ----- */
typedef enum
{
    USB_PHID_APL_CLOSE = 1,     /* Close State */
    USB_PHID_APL_OPEN,          /* Open State */
    USB_PHID_APL_TX_WAIT,       /* Data transfer wait State */
    USB_PHID_APL_TX,            /* Keyboard Mode 0 Data transfer State */
    USB_PHID_APL_TX_WAIT2,      /* Keyboard Mode 0 Data transfer wait State */
    USB_PHID_APL_TX_END         /* Keyboard Mode 0 Data transfer complete State */
}
USB_PHID_APL_STATE;

/******************************************************************************
External variables and functions
******************************************************************************/
#ifdef  USB_LCD_ENABLE
extern  void        usb_cpu_LcdDisp( uint8_t position, uint8_t *string );
extern  void        usb_cpu_LcdInit( void );
#endif /* USB_LCD_ENABLE */

/******************************************************************************
Renesas Abstracted USB Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_phid_prapl_title
Description     : Output Title
Argument        : none
Return          : none
******************************************************************************/
void usb_phid_prapl_title( void )
{
#ifdef  USB_LCD_ENABLE
    /* Lcd clear */
    usb_cpu_TargetLcdClear();
#endif /* USB_LCD_ENABLE */

    /* Print title */
    USB_PRINTF0( "\n" );
    USB_PRINTF0( "++++++++++++++++++++++++++++++++++++++++++++++++\n" );
    USB_PRINTF0( "+             PHID CLASS FW SAMPLE             +\n" );
    USB_PRINTF0( "+                                              +\n" );
    USB_PRINTF0( "+          RENESAS ELECTRONICS CORP.           +\n" );
    USB_PRINTF0( "+          RENESAS SOLUTIONS  CORP.            +\n" );
    USB_PRINTF0( "++++++++++++++++++++++++++++++++++++++++++++++++\n" );
    USB_PRINTF1( "           DATE [%s] \n", __DATE__ );
    USB_PRINTF1( "           TIME [%s] \n", __TIME__ );
}   /* eof usb_phid_prapl_title() */

/******************************************************************************
Function Name   : usb_papl_task_start
Description     : Set peripheral application task start conditions. 
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void    usb_papl_task_start( USB_UTR_t *ptr )
{
    /* PHID demo sample application task priority set */
    R_usb_cstd_SetTaskPri( USB_PHIDSMP_TSK, USB_PRI_6 );

#ifdef USB_ADC_ENABLE
    usb_cpu_AdInit();   /* 12bit A/D initialize */
#endif /* USB_ADC_ENABLE */

    /* Application data Initialize and Periodic process request */
    usb_phid_apl_init(ptr);
    usb_phid_smpl_message_send( ptr, USB_PHID_INIT );   /* Send Initial Message to PHID Application */
}
/******************************************************************************
End of function usb_papl_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_task_start
Description     : Start task CSTD; "common" basic USB low level task.
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cstd_task_start( void )
{
    usb_cstd_IdleTaskStart();   /* Idle Task Start */

    usb_phid_task_start();      /* Start Peripheral USB driver */

    usb_apl_task_switch();      /* Switch task for nonOS */
}
/******************************************************************************
End of function usb_cstd_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_phid_task_start
Description     : Set peripheral application task start conditions. 
Arguments       : none
Return value    : none
******************************************************************************/
void usb_phid_task_start( void )
{
    USB_UTR_t   utr;
    USB_UTR_t   *ptr;

#ifdef  USB_LCD_ENABLE
    /* Display Debug LCD */
    usb_cpu_LcdDisp( LCD_POS_U0, "USB HID " );
    usb_cpu_LcdDisp( LCD_POS_D0, "Appstart" );
#endif /* USB_LCD_ENABLE */

    ptr = &utr;                             /* Set USB IP no. and USB IP base address store pointer */
    ptr->ip = USB_PERI_USBIP_NUM;           /* IP number(0or1) */
    if( USB_NOUSE_PP != ptr->ip )           /* USB IP number check */
    {
        ptr->ipp = R_usb_cstd_GetUsbIpAdr( ptr->ip );   /* IP Address(USB0orUSB1) */

        usb_phid_driver_registration( ptr );            /* Peripheral Application Registration */
        usb_papl_task_start( ptr );                     /* Peripheral Application Task Start Setting */
        R_usb_phid_driver_start( ptr );                 /* Peripheral Class Driver Task Start Setting */
        R_usb_pstd_usbdriver_start( ptr );              /* Peripheral USB Driver Start Setting */

        /* Initialize USB IP */
        R_usb_cstd_UsbIpInit( ptr, USB_PERI_PP );
    }
}
/******************************************************************************
End of function usb_pstd_task_start()
******************************************************************************/


/******************************************************************************
Function Name   : usb_phid_driver_registration
Description     : Registration of peripheral human interface Devices Driver
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_driver_registration( USB_UTR_t *ptr )
{
    USB_PCDREG_t    pdriver;

    /* pdriver registration */
    /* Pipe Define Table address */
    pdriver.pipetbl     = &usb_gphid_EpPtr[0];
    /* Device descriptor Table address */
    pdriver.devicetbl   = (uint8_t*)&usb_gphid_SmplDeviceDescriptor;
    /* Qualifier descriptor Table address */
    pdriver.qualitbl    = (uint8_t*)&usb_gphid_SmplQualifierDescriptor;
    /* Configuration descriptor Table address */
    pdriver.configtbl   = (uint8_t**)&usb_gphid_ConPtr;
    /* Other configuration descriptor Table address */
    pdriver.othertbl    = (uint8_t**)USB_NULL;
    /* String descriptor Table address */
    pdriver.stringtbl   = (uint8_t**)&usb_gphid_StrPtr;
    /* Driver init */
    pdriver.classinit   = &usb_cstd_DummyFunction;
    /* Device default */
    pdriver.devdefault  = &usb_cstd_DummyFunction;
    /* Device configuered */
    pdriver.devconfig   = (USB_CB_INFO_t)&usb_phid_apl_open;
    /* Device detach */
    pdriver.devdetach   = (USB_CB_INFO_t)&usb_phid_apl_close;
    /* Device suspend */
    pdriver.devsuspend  = &usb_cstd_DummyFunction;
    /* Device resume */
    pdriver.devresume   = &usb_cstd_DummyFunction;
    /* Interface changed */
    pdriver.interface   = &usb_cstd_DummyFunction;
    /* Control Transfer */
    pdriver.ctrltrans   = (USB_CB_TRN_t)&R_usb_phid_control_transfer;

    /* Peripheral HID Driver registration. */
    R_usb_pstd_DriverRegistration( ptr, (USB_PCDREG_t*)&pdriver );
}   /* eof usb_phid_driver_registration() */

/******************************************************************************
Function Name   : usb_phid_apl_init
Description     : Initialize application
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return          : none
******************************************************************************/
void usb_phid_apl_init( USB_UTR_t *ptr )
{
    /* Application sequence : Open */
    usb_gphid_application_state = USB_PHID_APL_CLOSE;

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
    usb_sphid_keyboard_key = 0x00;    /* Keyboard data clear */
#endif  /* Peripheral HID Application keyboard mode */
}   /* eof usb_phid_apl_init() */

/******************************************************************************
Function Name   : usb_phid_apl_clear
Description     : Initialize application
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return          : none
******************************************************************************/
void usb_phid_apl_clear( USB_UTR_t *ptr )
{
#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
    usb_sphid_keyboard_key = 0x00;    /* Keyboard data clear */
#endif  /* Peripheral HID Application keyboard mode */

    /* Return Close Status */
    usb_gphid_application_state  = USB_PHID_APL_CLOSE;

#ifdef  USB_LCD_ENABLE
    usb_cpu_LcdDisp( LCD_POS_U0, "USB     " );
    usb_cpu_LcdDisp( LCD_POS_D0, "DETACH  " );
#endif /* USB_LCD_ENABLE */
}   /* eof usb_phid_apl_clear() */

/******************************************************************************
Function Name   : usb_apl_task_switch
Description     : Task switch loop for nonOS version.
Argument        : none
Return          : none
******************************************************************************/
void usb_apl_task_switch( void )
{
    while( 1 )
    {
        /* Scheduler */
        R_usb_cstd_Scheduler();

        /* Task schedule request check. */
        if( USB_FLGSET == R_usb_cstd_CheckSchedule() )
        {
#ifdef  USB_PERI_MODE_PP
            R_usb_pstd_PcdTask( (USB_VP_INT)0);             /* PCD Task */
            R_usb_phid_task(0);                             /* Peripheral HID Task */
            usb_phid_main_task( (USB_VP_INT)0 );            /* Peripheral HID demo sample application Task */
#endif  /* USB_PERI_MODE_PP */
            /* Idle Task (sleep sample) */
            R_usb_cstd_IdleTask(0);
        }
    }
}
/******************************************************************************
End of function usb_apl_task_switch()
******************************************************************************/

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
/******************************************************************************
Function Name   : usb_phid_main_task
Description     : Peripheral HID demo sample application Task
Arguments       : USB_VP_INT stacd          : RSK Switch input( When non-OS )
Return value    : none
******************************************************************************/
void usb_phid_main_task( USB_VP_INT stacd )
{
#if ( USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE ) || ( USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE )
    uint16_t    ret;
#endif /* ( USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE ) || ( USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE ) */
    USB_UTR_t   *mess;
    USB_ER_t    err = 0l;
    uint8_t     key_data;
    uint32_t    trnlen;
    uint16_t    device_information[8];
    USB_CTL_DEVICE_INFORMATION_t    dev_info;

    /* Message receive */
    err = R_USB_TRCV_MSG( USB_PHIDSMP_MBX, (USB_MSG_t**)&mess, (uint16_t)3000 );
    if( err == USB_E_OK )
    {
#ifdef USB_KEY_ENABLE
        /* Read RSK Switch input. */
        usb_cpu_AcquisitionKeyNo();
        key_data = usb_cpu_SingleKeyNo();
#else   /* USB_KEY_ENABLE */
        key_data = USB_OFF;
#endif  /* USB_KEY_ENABLE */

        /* Branch by message infomation. */
        switch( mess->msginfo )
        {
        case    USB_PHID_INIT:
            /* PHID Open */
            usb_sphid_fn = open( (int8_t *)USB_CLASS_PHID, 0, 0 );
            if( usb_sphid_fn == -1 )                    /* PHID Open check */
            {
                /* Make Other Task Processing Time */
                R_USB_DLY_TSK( 10 );

                /* Message send for USB_PHIDSMP_MBX */
                usb_phid_smpl_message_send( mess, USB_PHID_INIT );    /* Retry */
            }
            else
            {
                /* Call-Back function set */
                control( usb_sphid_fn, USB_CTL_WR_NOTIFY_SET, (void*)&usb_phid_apl_trans_cb );

                /* Message send for USB_PHIDSMP_MBX */
                usb_phid_smpl_message_send( mess, USB_PHID_PERIODIC_PROCESS );
            }
            break;

        case    USB_PHID_CLOSE:
            if( usb_sphid_fn != -1 )                    /* PHID Open check */
            {
                close( usb_sphid_fn );                  /* USB PHID close */
                usb_sphid_fn = -1;                      /* PHID file no. clear */
            }
            usb_phid_apl_clear( mess );                 /* Clear application */

            /* Message send for USB_PHIDSMP_MBX */
            usb_phid_smpl_message_send( mess, USB_PHID_INIT );
            break;

        case    USB_PHID_PERIODIC_PROCESS:
            if( usb_sphid_fn != -1 )                    /* PHID Open check */
            {
                /* Message send for USB_PHIDSMP_MBX */
                usb_phid_smpl_message_send( mess, USB_PHID_PERIODIC_PROCESS );

                /* Get USB Device information */
                dev_info.tbl = (uint16_t*)&device_information;
                control( usb_sphid_fn, USB_CTL_P_GET_DEVICE_INFO, (void*)&dev_info );

                /* Check Device State. Configured state ? */
                if( ( device_information[0] & USB_DVSQ ) == USB_DS_CNFG )
                {
                    /* Branch by the application sequence. */
                    switch( usb_gphid_application_state )
                    {
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
                    /* Demonstration Mode */
                    case    USB_PHID_APL_OPEN:
                        if( key_data != 0 )
                        {
                            /* Make report data. Return RSK Switch Input. */
                            usb_phid_apl_create_report( 0, 1, (uint32_t)key_data );

                            /* Get transfer(report) length by PHID mode. */
                            trnlen  = R_usb_phid_transfer_length();

                            /* Set transfer data. (Report data) */
                            memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, trnlen );

                            /* Demo mode Report transfer */
                            write( usb_sphid_fn, (uint8_t*)&usb_gphid_TransData, trnlen );

                            /* Application sequence : => USB_PHID_APL_TX_WAIT */
                            usb_gphid_application_state = USB_PHID_APL_TX_WAIT;
                        }
                    break;
                    case    USB_PHID_APL_TX_WAIT:
                    break;
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
                    /* Keyboard Mode */
                    case USB_PHID_APL_OPEN:
                        /* Keyboard data generates */
                        ret = usb_phid_apl_kbd_data_set( key_data, (uint8_t*)&usb_sphid_smpl_trn_data );
                        if( ret == USB_YES )
                        {
                            /* Get transfer(report) length by PHID mode. */
                            trnlen  = R_usb_phid_transfer_length();

                            /* Set transfer data. (Report data) */
                            memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, trnlen );

                            /* Keyboard mode Report transfer */
                            write( usb_sphid_fn, (uint8_t*)&usb_gphid_TransData, trnlen );

                            /* Update Application sequence. => USB_PHID_APL_MODE_KBD_TX_WAIT */
                            usb_gphid_application_state++;
                        }
                        break;

                    /* Wait data transfer complete */
                    case USB_PHID_APL_TX_WAIT:
                    break;

                    case USB_PHID_APL_TX:
                        /* 0 data generates */
                        memset( usb_sphid_smpl_trn_data, 0x00, sizeof(usb_sphid_smpl_trn_data) );

                        /* Get transfer(report) length by PHID mode. */
                        trnlen  = R_usb_phid_transfer_length();

                        /* Set transfer data. (Report data) */
                        memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, trnlen );

                        /* Keyboard mode Report transfer */
                        write( usb_sphid_fn, (uint8_t*)&usb_gphid_TransData, trnlen );

                        /* Update Application sequence. => USB_PHID_APL_MODE_KBD_TX_WAIT2 */
                        usb_gphid_application_state++;
                        break;

                    /* Wait data transfer complete */
                    case USB_PHID_APL_TX_WAIT2:
                    break;

                    case USB_PHID_APL_TX_END:
                        /* Application sequence : Keyboard mode open */
                        usb_gphid_application_state = USB_PHID_APL_OPEN;
                    break;
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
                    /* Mouse Mode */
                    case USB_PHID_APL_OPEN:
#ifdef USB_KEY_ENABLE
                        /* Read RSK Switch input. */
                        usb_cpu_AcquisitionKeyNo();
                        key_data = usb_cpu_KeepKeyNo();
#else   /* USB_KEY_ENABLE */
                        key_data = USB_OFF;
#endif  /* USB_KEY_ENABLE */

                        /* Mouse mode data update by the RSK Switch input. */
                        ret = usb_phid_apl_mse_data_set( key_data, (uint8_t*)&usb_sphid_smpl_trn_data );
                        if( ret == USB_YES )
                        {
                            /* Get transfer(report) length by PHID mode. */
                            trnlen  = R_usb_phid_transfer_length();

                            /* Set transfer data. (Report data) */
                            memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, trnlen );

                            /* Mouse mode Report transfer */
                            write( usb_sphid_fn, (uint8_t*)&usb_gphid_TransData, trnlen );

                            /* Application sequence : => USB_PHID_APL_TX_WAIT */
                            usb_gphid_application_state = USB_PHID_APL_TX_WAIT;
                        }
                    break;
                    case USB_PHID_APL_TX_WAIT:
                    break;
#endif  /* Peripheral HID Application mouse mode */

                    default:
                        break;
                    }
                }
                else if( ( device_information[0] & USB_DVSQ ) == USB_DS_SPD_CNFG )
                {
                    /* Suspend Configured */
                    if( key_data == SW3_PUSH )
                    {
                        usb_phid_apl_key_on_wakeup(mess);           /* Remote wakeup */
                    }
                }
            }
            break;

        default:
            break;
        }

        /* Task receive message memory pool release */
        err = R_USB_REL_BLK( USB_PHIDSMP_MPL, (USB_MH_t)mess );
        if( err != USB_OK )
        {
            /* error */
            USB_PRINTF0( "### USB PeripheralSampleClass rel_blk error\n" );
        }
    }
    else
    {
        /* USB_PRINTF0( "PHID sample apl rcv_msg error !\n" ); */
    }
}   /* eof usb_phid_main_task() */

#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
/******************************************************************************
Function Name   : usb_phid_main_task
Description     : Peripheral HID demo sample application Task
Arguments       : USB_VP_INT stacd          : RSK Switch input( When non-OS )
Return value    : none
******************************************************************************/
void usb_phid_main_task( USB_VP_INT stacd )
{
#if USB_PHID_APL_MODE != USB_PHID_DEMO_MODE
    uint16_t    ret;
#endif /* USB_PHID_APL_MODE != USB_PHID_DEMO_MODE */
    USB_UTR_t   *mess;
    USB_ER_t    err = 0l;
    uint8_t     key_data;
    uint32_t    trnlen;
    uint16_t    device_information[8];

    /* Message receive */
    err = R_USB_TRCV_MSG( USB_PHIDSMP_MBX, (USB_MSG_t**)&mess, (uint16_t)3000 );
    if( err == USB_E_OK )
    {
#ifdef USB_KEY_ENABLE
        /* Read RSK Switch input. */
        usb_cpu_AcquisitionKeyNo();
        key_data = usb_cpu_SingleKeyNo();
#else   /* USB_KEY_ENABLE */
        key_data = USB_OFF;
#endif  /* USB_KEY_ENABLE */

        /* Branch by message infomation. */
        switch( mess->msginfo )
        {
        case    USB_PHID_INIT:
            /* Check complete Enumeration for Host HID */
            if( USB_ON == usb_phid_isconnected() )
            {
                /* Message send for USB_PHIDSMP_MBX */
                usb_phid_smpl_message_send( mess, USB_PHID_PERIODIC_PROCESS );
            }
            else
            {
                /* Make Other Task Processing Time */
                R_USB_DLY_TSK( 10 );

                /* Message send for USB_PHIDSMP_MBX */
                usb_phid_smpl_message_send( mess, USB_PHID_INIT );    /* Retry */
            }
        break;

        case    USB_PHID_CLOSE:
            usb_phid_apl_clear( mess );                 /* Clear application */

            /* Message send for USB_PHIDSMP_MBX */
            usb_phid_smpl_message_send( mess, USB_PHID_INIT );
        break;

        case    USB_PHID_PERIODIC_PROCESS:
                /* Message send for USB_PHIDSMP_MBX */
                usb_phid_smpl_message_send( mess, USB_PHID_PERIODIC_PROCESS );

                /* Get USB Device information */
                R_usb_phid_DeviceInformation(mess, (uint16_t*)&device_information[0]);

                /* Check Device State. Configured state ? */
                if( ( device_information[0] & USB_DVSQ ) == USB_DS_CNFG )
                {
                    /* Branch by the application sequence. */
                    switch( usb_gphid_application_state )
                    {
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
                    /* Demonstration Mode */
                    case    USB_PHID_APL_OPEN:
                        if( key_data != 0 )
                        {
                            /* Make report data. Return RSK Switch Input. */
                            usb_phid_apl_create_report( 0, 1, (uint32_t)key_data );

                            /* Get transfer(report) length by PHID mode. */
                            trnlen  = R_usb_phid_transfer_length();

                            /* Set transfer data. (Report data) */
                            memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, trnlen );

                            /* Demo mode Report transfer */
                            R_usb_phid_send_data( mess, usb_gphid_TransData, trnlen,
                                 (USB_CB_t)&usb_phid_apl_trans_cb );

                            /* Application sequence : Demo mode open */
                            usb_gphid_application_state = USB_PHID_APL_TX_WAIT;
                        }
                    break;
                    case    USB_PHID_APL_TX_WAIT:
                    break;
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
                    /* Keyboard Mode */
                    case USB_PHID_APL_OPEN:
                        /* Keyboard data generates */
                        ret = usb_phid_apl_kbd_data_set( key_data, (uint8_t*)&usb_sphid_smpl_trn_data );
                        if( ret == USB_YES )
                        {
                            /* Get transfer(report) length by PHID mode. */
                            trnlen  = R_usb_phid_transfer_length();

                            /* Set transfer data. (Report data) */
                            memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, trnlen );

                            /* Keyboard mode Report transfer */
                            R_usb_phid_send_data( mess, usb_gphid_TransData, trnlen,
                                 (USB_CB_t)&usb_phid_apl_trans_cb );

                            /* Update Application sequence. => USB_PHID_APL_TX_WAIT */
                            usb_gphid_application_state = USB_PHID_APL_TX_WAIT;
                        }
                    break;

                    /* Wait data transfer complete */
                    case USB_PHID_APL_TX_WAIT:
                    break;

                    case USB_PHID_APL_TX:
                        /* 0 data generates */
                        memset( usb_sphid_smpl_trn_data, 0x00, sizeof(usb_sphid_smpl_trn_data) );

                        /* Get transfer(report) length by PHID mode. */
                        trnlen  = R_usb_phid_transfer_length();

                        /* Set transfer data. (Report data) */
                        memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, trnlen );

                        /* Keyboard mode Report transfer */
                        R_usb_phid_send_data( mess, usb_gphid_TransData, trnlen,
                             (USB_CB_t)&usb_phid_apl_trans_cb );

                        /* Update Application sequence. => USB_PHID_APL_TX_WAIT2 */
                        usb_gphid_application_state = USB_PHID_APL_TX_WAIT2;
                    break;

                    /* Wait data transfer complete */
                    case USB_PHID_APL_TX_WAIT2:
                    break;

                    case USB_PHID_APL_TX_END:
                        /* Application sequence : Keyboard mode open */
                        usb_gphid_application_state = USB_PHID_APL_OPEN;
                    break;
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
                    /* Mouse Mode */
                    case USB_PHID_APL_OPEN:
#ifdef USB_KEY_ENABLE
                        /* Read RSK Switch input. */
                        usb_cpu_AcquisitionKeyNo();
                        key_data = usb_cpu_KeepKeyNo();
#else   /* USB_KEY_ENABLE */
                        key_data = USB_OFF;
#endif  /* USB_KEY_ENABLE */

                        /* Mouse mode data update by the RSK Switch input. */
                        ret = usb_phid_apl_mse_data_set( key_data, (uint8_t*)&usb_sphid_smpl_trn_data );
                        if( ret == USB_YES )
                        {
                            /* Get transfer(report) length by PHID mode. */
                            trnlen  = R_usb_phid_transfer_length();

                            /* Set transfer data. (Report data) */
                            memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, trnlen );

                            /* Mouse mode Report transfer */
                            R_usb_phid_send_data( mess, usb_gphid_TransData, trnlen,
                                 (USB_CB_t)&usb_phid_apl_trans_cb );

                            /* Application sequence : => USB_PHID_APL_TX_WAIT */
                            usb_gphid_application_state = USB_PHID_APL_TX_WAIT;
                        }
                    break;
                    case USB_PHID_APL_TX_WAIT:
                    break;
#endif  /* Peripheral HID Application mouse mode */

                    default:
                    break;
                    }
                }
                else if( ( device_information[0] & USB_DVSQ ) == USB_DS_SPD_CNFG )
                {
                    /* Suspend Configured */
                    if( key_data == SW3_PUSH )
                    {
                        usb_phid_apl_key_on_wakeup(mess);           /* Remote wakeup */
                    }
                }
        break;

        default:
            break;
        }

        /* Task receive message memory pool release */
        err = R_USB_REL_BLK( USB_PHIDSMP_MPL, (USB_MH_t)mess );
        if( err != USB_OK )
        {
            /* error */
            USB_PRINTF0( "### USB PeripheralSampleClass rel_blk error\n" );
        }
    }
    else
    {
        USB_PRINTF0( "PHID sample apl rcv_msg error !\n" );
    }
}   /* eof usb_phid_main_task() */

#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

/******************************************************************************
Function Name   : usb_phid_apl_trans_cb
Description     : HID Host Tx complete Callback
Argument        : USB_UTR_t *mess           : message (not used)
Return          : none
******************************************************************************/
void usb_phid_apl_trans_cb( USB_UTR_t *mess )
{
#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
    usb_gphid_application_state++;                      /* Update Application sequence */
#else   /* USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE */
    usb_gphid_application_state = USB_PHID_APL_OPEN;    /* Update Application sequence */
#endif  /* USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE */
}   /* eof usb_phid_apl_trans_cb() */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
/******************************************************************************
Function Name   : usb_phid_apl_keyboard_led_control
Description     : RSK LED Off/On control for Keyboard mode.
Arguments       : uint8_t leddata
Return value    : none
******************************************************************************/
void usb_phid_apl_keyboard_led_control( uint8_t leddata )
{
#ifdef  USB_LED_ENABLE
    /* bit0 : Num lock */
    if( leddata & USB_BIT0 )
    {
        usb_cpu_LedSetBit( (uint8_t)(0), (uint8_t)USB_ON );     /* RSK LED0 ( Num lock ) ON */
    }
    else
    {
        usb_cpu_LedSetBit( (uint8_t)(0), (uint8_t)USB_OFF );    /* RSK LED0 ( Num lock ) OFF */
    }

    /* bit1 : Caps lock */
    if( leddata & USB_BIT1 )
    {
        usb_cpu_LedSetBit( (uint8_t)(1), (uint8_t)USB_ON );     /* RSK LED1 ( Caps lock ) ON */
    }
    else
    {
        usb_cpu_LedSetBit( (uint8_t)(1), (uint8_t)USB_OFF );    /* RSK LED1 ( Caps lock ) OFF */
    }

    /* bit2 : Scroll lock */
    if( leddata & USB_BIT2 )
    {
        usb_cpu_LedSetBit( (uint8_t)(2), (uint8_t)USB_ON );     /* RSK LED2 ( Scroll lock ) ON */
    }
    else
    {
        usb_cpu_LedSetBit( (uint8_t)(2), (uint8_t)USB_OFF );    /* RSK LED2 ( Scroll lock ) OFF */
    }

    /* bit3 : Compose */
    if( leddata & USB_BIT3 )
    {
        usb_cpu_LedSetBit( (uint8_t)(3), (uint8_t)USB_ON );     /* RSK LED3 ( Compose ) ON */
    }
    else
    {
        usb_cpu_LedSetBit( (uint8_t)(3), (uint8_t)USB_OFF );    /* RSK LED3 ( Compose ) OFF */
    }

    /* bit4 : Kana */
    if( leddata & USB_BIT4 )
    {
        usb_cpu_LedSetBit( (uint8_t)(4),(uint8_t)USB_ON);       /* RSK LED4 ( Kana ) ON */
    }
    else
    {
        usb_cpu_LedSetBit( (uint8_t)(4), (uint8_t)USB_OFF );    /* RSK LED4 ( Kana ) OFF */
    }
#endif /* USB_LED_ENABLE */
}   /* eof usb_phid_apl_keyboard_led_control() */

/******************************************************************************
Function Name   : usb_phid_apl_kbd_data_set
Description     : Keyborad mode data update by the RSK Switch input.
Arguments       : uint8_t key_data      : RSK Switch input
                : uint8_t *data         : Keyboard mode data set address.
Return value    : uint16_t  USB_YES : Keyboard mode data set. , USB_NO : Not data set.
******************************************************************************/
uint16_t usb_phid_apl_kbd_data_set( uint8_t key_data, uint8_t *data )
{
    uint16_t    ret;

    ret = USB_NO;

    /* Branch by the RSK Switch Input. */
    switch( key_data )
    {
    /* No key */
    case    NO_PUSH:
        break;
    
    /* SW2 a - z */
    case    SW2_PUSH:
        if( usb_sphid_keyboard_key == USB_PHID_KBD_CODE_Z )         /* Keyboard data check == "Z" */
        {
            usb_sphid_keyboard_key = USB_PHID_KBD_CODE_ENTER;       /* Keyboard data "Enter" set. */
        }
        else if( ( USB_PHID_KBD_CODE_A - 1 < usb_sphid_keyboard_key )
             && ( usb_sphid_keyboard_key < USB_PHID_KBD_CODE_Z ) )
        {
            /* Keyboard data update A to Y => B to Z */
            usb_sphid_keyboard_key++;
        }
        else
        {
            usb_sphid_keyboard_key = USB_PHID_KBD_CODE_A;           /* Keyboard data "A" set. */
        }
        data[2] = (uint8_t)usb_sphid_keyboard_key;                  /* Keyboard mode data store. */
        ret = USB_YES;                                              /* Keyboard mode data set. */
        break;
    
    /* SW3 0 - 9 */
    case    SW3_PUSH:
        if( usb_sphid_keyboard_key == USB_PHID_KBD_CODE_0 )         /* keyboard data check == "0" */
        {
            usb_sphid_keyboard_key = USB_PHID_KBD_CODE_ENTER;       /* keyboard data "Enter" set. */
        }
        else if( ( USB_PHID_KBD_CODE_1 - 1 < usb_sphid_keyboard_key )
             && ( usb_sphid_keyboard_key < USB_PHID_KBD_CODE_0 ) )
        {
            /* Keyboard data update 1 to 9 => 2 to 0 */
            usb_sphid_keyboard_key++;
        }
        else
        {
            usb_sphid_keyboard_key = USB_PHID_KBD_CODE_1;           /* keyboard data "1" set. */
        }
        data[2] = (uint8_t)usb_sphid_keyboard_key;                  /* Keyboard mode data store. */
        ret = USB_YES;                                              /* Keyboard mode data set. */
        break;

    default:
        break;
    }
    return  ret;
}   /* eof usb_phid_apl_kbd_data_set() */

#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
/******************************************************************************
Function Name   : usb_phid_demo_mode_report
Description     : RSK demo ( Toggle LED or Read ADC or Update LCD )
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint8_t *recdata      : Demo mode request data.( Set by the Set Request )
Return value    : none
******************************************************************************/
void usb_phid_demo_mode_report( USB_UTR_t *ptr, uint8_t *recdata )
{
    uint32_t    ad_data;
    uint32_t    trnlen;
    uint8_t     sz_line1[ NUMB_CHARS_PER_LINE + 1 ];
    uint8_t     sz_line2[ NUMB_CHARS_PER_LINE + 1 ];

#ifdef  USB_LED_ENABLE
    /* Toggle LED ? */
    if( 0 != ( recdata[0] & USB_DEMO_REPORTLED ) )
    {
        /* LED OFF/ON check */
        if( usb_sphid_led0 == USB_OFF ) 
        {
            /* LED ON */
            usb_cpu_LedSetBit( (uint8_t)(0), (uint8_t)USB_ON );         /* RSK LED0 ON */
            usb_sphid_led0 = USB_ON;                                    /* RSK LED0 status OFF => ON */
        }
        else
        {
            /* LED OFF */
            usb_cpu_LedSetBit( (uint8_t)(0), (uint8_t)USB_OFF );        /* RSK LED0 OFF */
            usb_sphid_led0 = USB_OFF;                                   /* RSK LED0 status ON => OFF */
        }
    }
#endif /* USB_LED_ENABLE */

    /* Read ADC ? */
    if( 0 != ( recdata[0] & USB_DEMO_REPORTADC ) )
    {
#ifdef USB_ADC_ENABLE
        ad_data = usb_cpu_AdData();                         /* Read A/D data */
#else   /* USB_ADC_ENABLE */
        ad_data = 0;                                        /* Dummy A/D data */
#endif  /* USB_ADC_ENABLE */
        ad_data = ad_data & 0x03FF;

        /* Send response. Return RSK A/D */
        usb_phid_apl_create_report( 1, 0, ad_data );        /* Make report data. */
        /* Get transfer(report) length by PHID mode. */
        trnlen  = R_usb_phid_transfer_length();

        /* Set transfer data. (Report data) */
        memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, trnlen );

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
        /* Demo mode Report transfer */
        write( usb_sphid_fn, (uint8_t*)&usb_gphid_TransData, trnlen );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
        /* Demo mode Report transfer */
        R_usb_phid_send_data( ptr, usb_gphid_TransData, trnlen, (USB_CB_t)&usb_phid_apl_trans_cb );
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    }

    /* Update LCD */
    if( 0 != ( recdata[0] & USB_DEMO_REPORTLCD ) )
    {
        /* RSK debug LCD display end code set */
        sz_line1[ NUMB_CHARS_PER_LINE ] = '\0';
        sz_line2[ NUMB_CHARS_PER_LINE ] = '\0';

        /* RSK debug LCD display code copy */
        memcpy( sz_line1, &recdata[1], NUMB_CHARS_PER_LINE );
        memcpy( sz_line2, &recdata[ 1 + NUMB_CHARS_PER_LINE ], NUMB_CHARS_PER_LINE );

#ifdef  USB_LCD_ENABLE
        /* RSK debug LCD display update */
        usb_cpu_LcdDisp( LCD_POS_U0, sz_line1 );
        usb_cpu_LcdDisp( LCD_POS_D0, sz_line2 );
#endif /* USB_LCD_ENABLE */
    }
}   /* eof usb_phid_demo_mode_report() */

/******************************************************************************
Function Name   : usb_phid_apl_create_report
Description     : Sample Out report (SetReport)
Arguments       : badc          : TRUE if providing new ADC value.
                : bswpressed    : TRUE if switch pressed.
                : adcvalue      : ADC Value.
Return value    : none
******************************************************************************/
void usb_phid_apl_create_report( uint8_t badc, uint8_t bswpressed, uint32_t adcvalue )
{
    /* Clear to start */
    usb_sphid_smpl_trn_data[0] = 0;

    /* Set LED Bit */
    if( usb_sphid_led0 == USB_ON )
    {
        usb_sphid_smpl_trn_data[0] |= IN_BIT_LED;          /* LED state (1 = ON) */
    }

    /* Set new ADC value bit ? */
    if( 0 != badc )
    {
        usb_sphid_smpl_trn_data[0] |= IN_BIT_ADC;          /* ADC data is valid */
    }

    /* Set switch toggled  bit ? */
    if( 0 != bswpressed )
    {
        usb_sphid_smpl_trn_data[0] |= IN_BIT_SW;           /* A switch has been pressed */
    }

    /* Set ADC Value ( even if not setting new ADC value bit ) */
    /* Little Endian */
    usb_sphid_smpl_trn_data[1] = (uint8_t)( adcvalue & 0xFF );
    usb_sphid_smpl_trn_data[2] = (uint8_t)( ( adcvalue >> 8 ) & 0xFF );
    usb_sphid_smpl_trn_data[3] = (uint8_t)( ( adcvalue >> 12 ) & 0xFF );
    usb_sphid_smpl_trn_data[4] = (uint8_t)( ( adcvalue >> 16 ) & 0xFF );
}   /* eof usb_phid_apl_create_report() */

#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
/******************************************************************************
Function Name   : usb_phid_apl_mse_data_set
Description     : Mouse mode data update by the RSK Switch input.
Arguments       : uint8_t key_data      : RSK Switch input
                : uint8_t *data         : Mouse mode data set address.
Return value    : uint16_t  USB_YES : Mouse mode data set. , USB_NO : Not data set.
******************************************************************************/
uint16_t usb_phid_apl_mse_data_set( uint8_t key_data, uint8_t *data )
{
    uint16_t    ret;

    ret = USB_NO;

    /* Branch by the RSK Switch Input. */
    switch ( key_data )
    {
    /* No key */
    case NO_PUSH:
        if( usb_sphid_mouse_time != 0 )                                     /* Check RSK Switch push time(count) */
        {
            /* Mouse mode report data clear */
            data[USB_PHID_MSE_BTN_DATA_INDEX]   = (uint8_t)0x00;
            data[USB_PHID_MSE_X_DATA_INDEX]     = (uint8_t)0x00;
            data[USB_PHID_MSE_Y_DATA_INDEX]     = (uint8_t)0x00;
            data[3] = (uint8_t)0x00;

            usb_sphid_mouse_time = 0;                                       /* RSK Switch push time(count) clear */

            /* X axial direction is switched */
            if( ( usb_sphid_check_switch & 0x01 ) == 0x01 )                 /* SW2 up/down check */
            {
                usb_sphid_mouse_up_down = usb_sphid_mouse_up_down ^ 0x01;   /* X axial direction is switch */
                usb_sphid_check_switch &= ~0x01;                            /* SW2 up/down clear */
            }

            /* Y axial direction is switched */
            if( ( usb_sphid_check_switch & 0x10 ) == 0x10 )                 /* SW3 left/right check */
            {
                usb_sphid_mouse_left_right ^= 0x01;                         /* Y axial direction is switch */
                usb_sphid_check_switch &= ~0x10;                            /* SW3 left/right clear */
            }

            ret = USB_YES;                                                  /* Mouse mode data set. */
        }
        break;

    /* SW2 up/down */
    case SW2_PUSH:
        /* Movement to lower direction */
        if( usb_sphid_mouse_up_down == 0x00 )
        {
#ifdef  USB_LED_ENABLE
            usb_cpu_LedSetBit( (uint8_t)(0), (uint8_t)USB_ON );             /* RSK LED0 ON */
#endif /* USB_LED_ENABLE */
            /* Check RSK Switch push time(count) */
            if( usb_sphid_mouse_time == USB_MSE_SW_PUSH_THRESHOLD_HIGH )
            {
                data[USB_PHID_MSE_Y_DATA_INDEX] = USB_PHID_MSE_UP_DISTANSE_MAX;                 /* YMove +1 */
            }
            else if( ( USB_MSE_SW_PUSH_THRESHOLD_LOW < usb_sphid_mouse_time )
                 && ( usb_sphid_mouse_time < USB_MSE_SW_PUSH_THRESHOLD_HIGH ) )
            {
                data[USB_PHID_MSE_Y_DATA_INDEX] = USB_PHID_MSE_UP_DISTANSE_MID;                 /* YMove +1 */
                usb_sphid_mouse_time++;                                     /* RSK Switch push time(count) update */
            }
            else
            {
                data[USB_PHID_MSE_Y_DATA_INDEX] = USB_PHID_MSE_UP_DISTANSE_MIN;                 /* YMove +1 */
                usb_sphid_mouse_time++;                                     /* RSK Switch push time(count) update */
            }
        }
        /* Movement to upper direction */
        else if( usb_sphid_mouse_up_down == 0x01 )
        {
#ifdef  USB_LED_ENABLE
            usb_cpu_LedSetBit( (uint8_t)(0), (uint8_t)USB_OFF );            /* RSK LED0 OFF */
#endif /* USB_LED_ENABLE */
            /* Check RSK Switch push time(count) */
            if( usb_sphid_mouse_time == USB_MSE_SW_PUSH_THRESHOLD_HIGH )
            {
                data[USB_PHID_MSE_Y_DATA_INDEX] = USB_PHID_MSE_DOWN_DISTANSE_MAX;                 /* YMove -1 */
            }
            else if( ( USB_MSE_SW_PUSH_THRESHOLD_LOW < usb_sphid_mouse_time )
                 && ( usb_sphid_mouse_time < USB_MSE_SW_PUSH_THRESHOLD_HIGH ) )
            {
                data[USB_PHID_MSE_Y_DATA_INDEX] = USB_PHID_MSE_DOWN_DISTANSE_MID;                 /* YMove -1 */
                usb_sphid_mouse_time++;                                     /* RSK Switch push time(count) update */
            }
            else
            {
                data[USB_PHID_MSE_Y_DATA_INDEX] = USB_PHID_MSE_DOWN_DISTANSE_MIN;                 /* YMove -1 */
                usb_sphid_mouse_time++;                                     /* RSK Switch push time(count) update */
            }
        }
        else
        {
        }
        usb_sphid_check_switch |= 0x01;                                     /* SW2 up/down push */
        ret = USB_YES;                                                      /* Mouse mode data set. */
        break;

    /* SW3 left/right */
    case SW3_PUSH:
        /* Movement to Right direction */
        if( usb_sphid_mouse_left_right == 0x00 )
        {
#ifdef  USB_LED_ENABLE
            usb_cpu_LedSetBit( (uint8_t)(1), (uint8_t)USB_ON );             /* RSK LED1 ON */
#endif /* USB_LED_ENABLE */
            /* Check RSK Switch push time(count) */
            if( usb_sphid_mouse_time == USB_MSE_SW_PUSH_THRESHOLD_HIGH )
            {
                data[USB_PHID_MSE_X_DATA_INDEX] = USB_PHID_MSE_RIGHT_DISTANSE_MAX;                 /* XMove +1 */
            }
            else if( ( USB_MSE_SW_PUSH_THRESHOLD_LOW < usb_sphid_mouse_time )
                 && ( usb_sphid_mouse_time < USB_MSE_SW_PUSH_THRESHOLD_HIGH ) )
            {
                data[USB_PHID_MSE_X_DATA_INDEX] = USB_PHID_MSE_RIGHT_DISTANSE_MID;                 /* XMove +1 */
                usb_sphid_mouse_time++;                                     /* RSK Switch push time(count) update */
            }
            else
            {
                data[USB_PHID_MSE_X_DATA_INDEX] = USB_PHID_MSE_RIGHT_DISTANSE_MIN;                 /* XMove +1 */
                usb_sphid_mouse_time++;                                     /* RSK Switch push time(count) update */
            }
        }
        /* Movement to Left direction */
        else if( usb_sphid_mouse_left_right == 0x01 )
        {
#ifdef  USB_LED_ENABLE
            usb_cpu_LedSetBit( (uint8_t)(1), (uint8_t)USB_OFF );            /* RSK LED1 OFF */
#endif /* USB_LED_ENABLE */
            /* Check RSK Switch push time(count) */
            if( usb_sphid_mouse_time == USB_MSE_SW_PUSH_THRESHOLD_HIGH )
            {
                data[USB_PHID_MSE_X_DATA_INDEX] = USB_PHID_MSE_LEFT_DISTANSE_MAX;                 /* XMove -1 */
            }
            else if( ( USB_MSE_SW_PUSH_THRESHOLD_LOW < usb_sphid_mouse_time )
                 && ( usb_sphid_mouse_time < USB_MSE_SW_PUSH_THRESHOLD_HIGH ) )
            {
                data[USB_PHID_MSE_X_DATA_INDEX] = USB_PHID_MSE_LEFT_DISTANSE_MID;                 /* XMove -1 */
                usb_sphid_mouse_time++;                                     /* RSK Switch push time(count) update */
            }
            else
            {
                data[USB_PHID_MSE_X_DATA_INDEX] = USB_PHID_MSE_LEFT_DISTANSE_MIN;                 /* XMove -1 */
                usb_sphid_mouse_time++;                                     /* RSK Switch push time(count) update */
            }
        }
        else
        {
        }
        usb_sphid_check_switch |= 0x10;                                     /* SW3 left/right push */
        ret = USB_YES;                                                      /* Mouse mode data set. */
        break;

    /* SW3+SW2 Left Click */
    case ( SW2_PUSH | SW3_PUSH ):
        if( usb_sphid_mouse_time == 0 )                                     /* Check RSK Switch push time(count) */
        {
            data[USB_PHID_MSE_BTN_DATA_INDEX] = 0x01;                       /* Set Left Click for report data */
            usb_sphid_mouse_time++;                                         /* RSK Switch push time(count) update */
            ret     = USB_YES;                                              /* Mouse mode data set. */
        }
        break;

    /* RSK Switch no push */
    default:
        break;
    }
    return  ret;
}   /* eof usb_phid_apl_mse_data_set() */

#endif  /* Peripheral HID Application mouse mode */

/******************************************************************************
Function Name   : usb_phid_apl_key_on_wakeup
Description     : Remote wakeup
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
Return value    : none
******************************************************************************/
void usb_phid_apl_key_on_wakeup( USB_UTR_t *ptr )
{
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    USB_CTL_CHANGE_DEVICE_STATE_t   device_state;

    device_state.complete = &usb_cstd_DummyFunction;            /* Callback function */
    device_state.msginfo = USB_DO_REMOTEWAKEUP;                 /* Request type */
    /* Change Device State => Remote wakeup */
    control( usb_sphid_fn, USB_CTL_P_CHG_DEVICE_STATE, (void*)&device_state );
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
    R_usb_phid_ChangeDeviceState( ptr, USB_DO_REMOTEWAKEUP );
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_phid_apl_key_on_wakeup() */

/******************************************************************************
Function Name   : usb_phid_smpl_message_send
Description     : Transfer message for Message BOX of demo sample application
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t   msginfo        : Message Information Kind
Return          : none
******************************************************************************/
void usb_phid_smpl_message_send( USB_UTR_t *ptr, uint16_t msginfo )
{
    USB_MH_t            p_blf;
    USB_ER_t            err;
    USB_CLSINFO_t       *cp;

    /* Get mem pool blk */
    if( R_USB_PGET_BLK( USB_PHIDSMP_MPL, &p_blf ) == USB_E_OK )
    {
        cp = (USB_CLSINFO_t*)p_blf;
        cp->msginfo = msginfo;                      /* Message Information Kind */

        cp->ipp = ptr->ipp;                         /* IP Address(USB0orUSB1) */
        cp->ip  = ptr->ip;                          /* IP number(0or1) */
        cp->msghead = (USB_MH_t)NULL;
        
        /* Send message */
        err = R_USB_SND_MSG( USB_PHIDSMP_MBX, (USB_MSG_t*)cp );
        if(err != USB_E_OK)
        {
            /* error */
            err = R_USB_REL_BLK( USB_PHIDSMP_MPL,(USB_MH_t)p_blf );
            USB_PRINTF0( "### usb_phid_smpl_message_send function snd_msg error\n" );
        }
    }
    else
    {
        /* error */
        USB_PRINTF0( "### usb_phid_smpl_message_send function pget_blk error\n" );
        while( 1 );
    }
}   /* eof usb_phid_smpl_message_send() */

/******************************************************************************
Function Name   : usb_phid_apl_open
Description     : USB Peripheral Human Interface Devices Class open function
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : USB_ER_t                  : USB_E_OK etc
******************************************************************************/
USB_ER_t usb_phid_apl_open( USB_UTR_t *ptr, uint16_t data1, uint16_t data2 )
{
    /* PHID Application process enable */
    usb_gphid_connected = USB_ON;

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
#ifdef  USB_LCD_ENABLE
        /* Display "USB DEM ATTACH" on LCD */
        usb_cpu_LcdDisp( LCD_POS_U0, "USB DEM " );
        usb_cpu_LcdDisp( LCD_POS_D0, "ATTACH  " );
#endif /* USB_LCD_ENABLE */
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
#ifdef  USB_LCD_ENABLE
        usb_cpu_LcdInit();                              /* Initialized RSK LCD */
        /* Display "USB KBD ATTACH" on LCD */
        usb_cpu_LcdDisp( LCD_POS_U0, "USB KBD " );
        usb_cpu_LcdDisp( LCD_POS_D0, "ATTACH  " );
#endif /* USB_LCD_ENABLE */
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
#ifdef  USB_LCD_ENABLE
        usb_cpu_LcdInit();                              /* Initialized RSK LCD */
        /* Display "USB MSE ATTACH" on LCD */
        usb_cpu_LcdDisp( LCD_POS_U0, "USB MSE " );
        usb_cpu_LcdDisp( LCD_POS_D0, "ATTACH  " );
#endif /* USB_LCD_ENABLE */
#endif  /* Peripheral HID Application mouse mode */

    usb_gphid_application_state = USB_PHID_APL_OPEN;    /* Update Application sequence */

    return USB_E_OK;
}   /* eof usb_phid_apl_open() */

/******************************************************************************
Function Name   : usb_phid_apl_close
Description     : USB Peripheral Human Interface Devices Class close function
Arguments       : USB_UTR_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : USB_ER_t                  : USB_E_OK etc
******************************************************************************/
USB_ER_t usb_phid_apl_close( USB_UTR_t *ptr, uint16_t data1, uint16_t data2 )
{
    /* Notify Sample class detatch */
    usb_phid_smpl_message_send( ptr, USB_PHID_CLOSE );

    /* PHID Application process disable */
    usb_gphid_connected = USB_OFF;

    return USB_E_OK;
}   /* eof usb_phid_apl_close() */

/******************************************************************************
Function Name   : usb_phid_isconnected
Description     : Get the USB cable connected state.
Arguments       : none
Return value    : USB_ON = Connected, USB_OFF = Disconnected.
******************************************************************************/
uint16_t usb_phid_isconnected( void )
{
    return usb_gphid_connected;
}   /* eof usb_phid_isconnected() */

/******************************************************************************
End  Of File
******************************************************************************/

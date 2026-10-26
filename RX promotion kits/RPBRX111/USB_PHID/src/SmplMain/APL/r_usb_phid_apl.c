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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_usb_phid_apl.c
* Version       : 2.10
* Description   : USB Peripheral HID class driver code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00    First Release
*           : 31.05.2013 2.10    Corresponded to RL78/L1C, RX111
*******************************************************************************/


/******************************************************************************
Section definitions
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_usbip.h"                /* Definition of the USB register & bit define */
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_api.h"                  /* USB API public function header */
#include "hw_resource.h"
#include "r_usb_phid_define.h"
#include "r_usb_phid_usrcfg.h"
#include "r_usb_phid_api.h"


/******************************************************************************
Constant macro definitions
******************************************************************************/
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
#define USB_MSE_SW_PUSH_THRESHOLD_LOW   200     /* RSK Switch push time(count) threshold low. */
#define USB_MSE_SW_PUSH_THRESHOLD_HIGH  400     /* RSK Switch push time(count) threshold high. */

/* USB Keyboard Defines */
#define USB_KBD_SW_TIME  5000                   /* RSK Switch push time(count) threshold. */
#define USB_KBD_SW_CONT  1000                   /* RSK Switch push time(count) threshold. */

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
Private global variables and functions
******************************************************************************/
/* variables */
USB_STATIC uint16_t             usb_gphid_connected;    /* PHID application enable */
USB_STATIC usb_utr_t            usb_gphid_apl_message_area1;
USB_STATIC usb_utr_t            usb_gphid_apl_message_area2;
USB_STATIC volatile uint8_t     apli_key_data[9];
USB_STATIC volatile uint16_t    usg_apl_g_ad_data;

/* Application seaquence */
/* [0:Port0] Peri=0x01-,Host=0x81-  [1:Port1] Peri=0x01-,Host=0x81- */
USB_STATIC  uint16_t    usb_gphid_application_state;

/* Common global area definition */
USB_STATIC  uint8_t     usb_sphid_smpl_trn_data[8];

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
USB_STATIC uint8_t      usb_sphid_keyboard_key;
USB_STATIC uint16_t     usb_sphid_key_time;
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
USB_STATIC uint16_t     usb_sphid_mouse_up_down;
USB_STATIC uint16_t     usb_sphid_mouse_left_right;
USB_STATIC uint8_t      usb_sphid_check_switch;
USB_STATIC uint16_t     usb_sphid_mouse_time;
#endif  /* Peripheral HID Application mouse mode */

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
USB_STATIC uint8_t      usb_sphid_led0;
#endif  /* Peripheral HID Application demo mode */

/* functions */
USB_STATIC void         usb_phid_apl_init(void);
USB_STATIC void         usb_phid_apl_clear(void);
USB_STATIC void         usb_phid_apl_key_on_wakeup(void);
USB_STATIC void         usb_phid_smpl_message_send(usb_utr_t *ptr, usb_strct_t msginfo);
USB_STATIC void         usb_phid_apl_trans_cb(usb_utr_t *mess);
USB_STATIC void         usb_phid_driver_registration(void);
USB_STATIC usb_er_t     usb_phid_apl_open( usb_utr_t *ptr, uint16_t data1, uint16_t device_state );
USB_STATIC usb_er_t     usb_phid_apl_close( usb_utr_t *ptr, uint16_t data1, uint16_t device_state );
USB_STATIC usb_er_t     usb_phid_apl_DummyFunction( usb_utr_t *ptr, uint16_t data1, uint16_t device_state );
USB_STATIC void         usb_phid_change_device_state( uint16_t data1, uint16_t device_state );
USB_STATIC void         usb_phid_apl_memcpy( uint8_t *dst, uint8_t *src, uint16_t length );
USB_STATIC void         usb_phid_apl_memset( uint8_t *dst, uint8_t data, uint16_t length );

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
USB_STATIC void         usb_phid_apl_create_report(uint8_t badc, uint8_t bswpressed, uint16_t adcvalue);
USB_STATIC uint16_t     usb_phid_apl_AdData( void );
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
USB_STATIC uint16_t     usb_phid_apl_kbd_data_set(uint8_t key_data, uint8_t *data);
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
USB_STATIC uint16_t     usb_phid_apl_mse_data_set(uint8_t key_data, uint8_t *data);
#endif  /* Peripheral HID Application mouse mode */


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */

/* functions */
void        usb_psmpl_main_init(void);
void        usb_phid_main_task(void);
uint16_t    usb_phid_disconnected(void);

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
void        usb_phid_demo_mode_report( uint8_t *recdata );
#endif  /* Peripheral HID Application demo mode */

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
void        usb_phid_apl_keyboard_led_control(uint8_t leddata);
#endif  /* Peripheral HID Application keyboard mode */


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */
extern uint8_t              usb_gphid_SmplDeviceDescriptor[];
extern uint16_t             usb_gphid_EpTbl[];
extern uint8_t              usb_gphid_Configuration[];
extern uint8_t              *usb_gphid_StrPtr[];
extern uint8_t              usb_gphid_TransData[];
extern volatile uint16_t    g_usb_pipe_toggle[];

/* functions */
extern void     usb_smpl_set_suspend_flag(uint8_t data);
extern uint8_t  usb_smpl_key_input(void);
extern uint16_t usb_smpl_ad_read(void);
extern void     usb_smpl_lcd_string( uint8_t position, uint8_t *string );
extern void     usb_cpu_led_set_bit(uint8_t ledpos, uint8_t data);


/******************************************************************************
Renesas Abstracted USB Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_psmpl_main_init
Description     : Initialize process
Arguments       : none
Return value    : none
******************************************************************************/
void usb_psmpl_main_init(void)
{
    /* Target board initialize */
    usb_cpu_target_init();

    /* USB-IP initialized */
    R_usb_pstd_PcdChangeDeviceState( USB_DO_INITHWFUNCTION );

    /* PCD driver open & registratuion */
    R_usb_pstd_PcdOpen();                       /* PCD task open */
    usb_phid_driver_registration();             /* Peripheral Application Registration */

    /* Scheduler initialized */
    R_usb_pstd_PcdChangeDeviceState( USB_DO_SETHWFUNCTION );

    /* Application data Initialize and Periodic process request */
    usb_phid_apl_init();

    /* Send Initial Message to PHID Application */
    usb_phid_smpl_message_send( &usb_gphid_apl_message_area1, (usb_strct_t)USB_PHID_INIT );

}   /* eof usb_psmpl_main_init() */


/******************************************************************************
Function Name   : usb_phid_driver_registration
Description     : Registration of peripheral human interface Devices Driver
Return value    : none
Return value    : none
******************************************************************************/
void usb_phid_driver_registration( void )
{
    usb_pcdreg_t    pdriver;

    /* pdriver registration */
    /* Pipe Define Table address */
    pdriver.pipetbl         = (uint16_t*)&usb_gphid_EpTbl;
    /* Device descriptorTable address */
    pdriver.devicetbl       = (uint8_t*)&usb_gphid_SmplDeviceDescriptor;
    /* Configuration descriptor Table address */
    pdriver.configtbl       = (uint8_t*)&usb_gphid_Configuration;
    /* String descriptor Table address */
    pdriver.stringtbl       = (uint8_t**)&usb_gphid_StrPtr[0];
    /* Device statediagram */
    pdriver.statediagram    = (usb_cbinfo_t)&usb_phid_change_device_state;
    /* Control Transfer */
    pdriver.ctrltrans       = R_usb_phid_control_callback;

    /* Peripheral HID Driver registration. */
    R_usb_pstd_DriverRegistration( (usb_pcdreg_t*)&pdriver );
    R_usb_phid_driver_start();                 /* Peripheral Class Driver Task Start Setting */
}   /* eof usb_phid_driver_registration() */


/******************************************************************************
Function Name   : usb_phid_change_device_state
Description     :
Return value    : none
Return value    : none
******************************************************************************/
void usb_phid_change_device_state( uint16_t data1, uint16_t device_state )
{
    usb_utr_t *message_ptr;


    message_ptr = &usb_gphid_apl_message_area2;

    switch ( device_state )
    {
    case USB_STS_ATTACH:
        usb_phid_apl_DummyFunction( message_ptr, data1, device_state );
        usb_smpl_set_suspend_flag( USB_NO );
    break;
    case USB_STS_CONFIGURED:
        usb_phid_apl_open( message_ptr, data1, device_state );
    break;
    case USB_STS_DETACH:
        usb_phid_apl_close( message_ptr, data1, device_state );
        usb_smpl_set_suspend_flag( USB_NO );
    break;
    case USB_STS_SUSPEND:
        usb_phid_apl_DummyFunction( message_ptr, data1, device_state );
        usb_smpl_set_suspend_flag( USB_YES );
    break;
    case USB_STS_RESUME:
        usb_phid_apl_DummyFunction( message_ptr, data1, device_state );
        usb_smpl_set_suspend_flag( USB_NO );
    break;
    case USB_STS_DEFAULT:
        usb_phid_apl_DummyFunction( message_ptr, data1, device_state );
    break;
    default:
        usb_phid_apl_DummyFunction( message_ptr, data1, device_state );
    break;
    }
}   /* eof usb_phid_change_device_state() */


/******************************************************************************
Function Name   : usb_phid_apl_init
Description     : Initialize application
Return value    : none
Return value    : none
******************************************************************************/
void usb_phid_apl_init(void)
{
    usb_smpl_set_suspend_flag( USB_NO );

//  All global variables are initialized specifying it.
//  However, an unnecessary variable to initialize ends comment by "//".
//unnecessary   usb_gphid_apl_message_area1;
//unnecessary   usb_gphid_apl_message_area2;
//unnecessary   apli_key_data[];
    usb_gphid_connected = USB_NO;       /* PHID application enable */
    usg_apl_g_ad_data   = 0;

    /* Application sequence : Open */
    usb_gphid_application_state = USB_PHID_APL_CLOSE;

#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
    usb_sphid_keyboard_key     = 0x00;    /* Keyboard data clear */
    usb_sphid_key_time         = 0;       /* Key input timer clear */
#endif  /* Peripheral HID Application keyboard mode */

#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
    usb_sphid_mouse_up_down    = 0;
    usb_sphid_mouse_left_right = 0;
    usb_sphid_check_switch     = 0;
    usb_sphid_mouse_time       = 0;
#endif  /* Peripheral HID Application mouse mode */

#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
    usb_sphid_led0             = USB_NO;
#endif  /* Peripheral HID Application demo mode */

    usb_phid_apl_memset( (uint8_t *)usb_sphid_smpl_trn_data,
                         (uint8_t)0x00, (uint16_t)sizeof(usb_sphid_smpl_trn_data) );

}   /* eof usb_phid_apl_init() */


/******************************************************************************
Function Name   : usb_phid_apl_clear
Description     : Initialize application
Return value    : none
Return value    : none
******************************************************************************/
void usb_phid_apl_clear( void )
{
    usb_phid_apl_init();
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
    usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"USB DEM " );
#endif  /* Peripheral HID Application demo mode */
#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
    usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"USB KBD " );
#endif  /* Peripheral HID Application keyboard mode */
#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
    usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"USB MSE " );
#endif  /* Peripheral HID Application mouse mode */
    usb_smpl_lcd_string(LCD_POS_D0, (uint8_t *)"DETACH  " );
}   /* eof usb_phid_apl_clear() */


/******************************************************************************
Function Name   : usb_phid_main_task
Description     : Peripheral HID demo sample application Task
Return value    : none
Return value    : none
******************************************************************************/
void usb_phid_main_task( void )
{
#if USB_PHID_APL_MODE != USB_PHID_DEMO_MODE
    uint16_t            ret;
#endif /* USB_PHID_APL_MODE != USB_PHID_DEMO_MODE */
    usb_utr_t           *mess;
    usb_er_t            err = 0l;
    volatile uint8_t    key_data;
    uint16_t            device_information[8];


    /* Message receive */
    err = R_USB_RCV_MSG( USB_PHIDSMP_MBX, (usb_msg_t **)&mess );
    if ( err != USB_E_OK )
    {
        return;
    }


    key_data = usb_smpl_key_input();

    /* Branch by message infomation. */
    switch( mess->msginfo )
    {
        case    USB_PHID_INIT:
            /* Check complete Enumeration for Host HID */
            if( USB_YES == usb_phid_disconnected() )
            {
                /* Message send for USB_PHIDSMP_MBX */
                usb_phid_smpl_message_send( mess, (usb_strct_t)USB_PHID_PERIODIC_PROCESS );
            }
            else
            {
                /* Message send for USB_PHIDSMP_MBX */
                usb_phid_smpl_message_send( mess, (usb_strct_t)USB_PHID_INIT );    /* Retry */
            }
        break;

        case    USB_PHID_CLOSE:
            usb_phid_apl_clear();                 /* Clear application */

            /* Message send for USB_PHIDSMP_MBX */
            usb_phid_smpl_message_send( mess, (usb_strct_t)USB_PHID_INIT );
        break;

        case    USB_PHID_PERIODIC_PROCESS:
            /* Message send for USB_PHIDSMP_MBX */
            usb_phid_smpl_message_send( mess, (usb_strct_t)USB_PHID_PERIODIC_PROCESS );

            /* Get USB Device information */
            R_usb_phid_DeviceInformation( (uint16_t*)&device_information[0] );

            /* Check Device State. Configured state ? */
            if( ( device_information[0] & USB_DVSQ ) == USB_DS_CNFG )
            {
                /* Branch by the application sequence. */
                switch( usb_gphid_application_state )
                {
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
                    /* Demonstration Mode */
                    case USB_PHID_APL_OPEN:
                        if( key_data != 0 )
                        {
                            /* Make report data. Return RSK Switch Input. */
                            usb_phid_apl_create_report( 0, 1, (uint16_t)key_data );

                            /* Set transfer data. (Report data) */
                            usb_phid_apl_memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, USB_INTEPMAXP );

                            /* Demo mode Report transfer */
                            R_usb_phid_send_data( usb_sphid_smpl_trn_data, USB_INTEPMAXP, (usb_cb_t)&usb_phid_apl_trans_cb );

                            /* Application sequence : Demo mode open */
                            usb_gphid_application_state = USB_PHID_APL_TX_WAIT;
                        }
                    break;

                    case USB_PHID_APL_TX_WAIT:
                    break;
#endif  /* Peripheral HID Application demo mode */


#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
                    /* Keyboard Mode */
                    case USB_PHID_APL_OPEN:
                        /* Keyboard data generates */
                        ret = usb_phid_apl_kbd_data_set( key_data, (uint8_t*)&usb_sphid_smpl_trn_data );
                        if( ret == USB_YES )
                        {
                            /* Set transfer data. (Report data) */
                            usb_phid_apl_memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, USB_INTEPMAXP );

                            /* Keyboard mode Report transfer */
                            R_usb_phid_send_data( usb_sphid_smpl_trn_data, USB_INTEPMAXP, (usb_cb_t)&usb_phid_apl_trans_cb );

                            /* Update Application sequence. => USB_PHID_APL_TX_WAIT */
                            usb_gphid_application_state = USB_PHID_APL_TX_WAIT;
                        }
                    break;

                    /* Wait data transfer complete */
                    case USB_PHID_APL_TX_WAIT:
                    break;

                    case USB_PHID_APL_TX:
                        /* 0 data generates */
                        usb_phid_apl_memset( (uint8_t *)usb_sphid_smpl_trn_data,
                                             (uint8_t)0x00,
                                             (uint16_t)sizeof(usb_sphid_smpl_trn_data) );

                        /* Set transfer data. (Report data) */
                        usb_phid_apl_memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, USB_INTEPMAXP );

                        /* Keyboard mode Report transfer */
                        R_usb_phid_send_data( usb_sphid_smpl_trn_data, USB_INTEPMAXP, (usb_cb_t)&usb_phid_apl_trans_cb );

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
                        /* Read RSK Switch input. */
                        key_data = usb_smpl_key_input();
                        apli_key_data[0] = 'S';
                        apli_key_data[1] = 'W';
                        apli_key_data[2] = (uint8_t)('0' + key_data);
                        apli_key_data[3] = ' ';
                        apli_key_data[4] = ' ';
                        apli_key_data[5] = ' ';
                        apli_key_data[6] = ' ';
                        apli_key_data[7] = ' ';
                        apli_key_data[8] = '\0';

                        usb_smpl_lcd_string(LCD_POS_D0, (uint8_t *)&apli_key_data[0] );

                        /* Mouse mode data update by the RSK Switch input. */
                        ret = usb_phid_apl_mse_data_set( key_data, (uint8_t*)&usb_sphid_smpl_trn_data );
                        if( ret == USB_YES )
                        {
                            /* Set transfer data. (Report data) */
                            usb_phid_apl_memcpy( usb_gphid_TransData, usb_sphid_smpl_trn_data, USB_INTEPMAXP );

                            /* Mouse mode Report transfer */
                            R_usb_phid_send_data( usb_sphid_smpl_trn_data, USB_INTEPMAXP, (usb_cb_t)&usb_phid_apl_trans_cb );

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
        break;

        default:
        break;
    }
}   /* eof usb_phid_main_task() */


/******************************************************************************
Function Name   : usb_phid_apl_trans_cb
Description     : HID Host Tx complete Callback
Argument        : usb_utr_t *mess           : message (not used)
Return value    : none
******************************************************************************/
void usb_phid_apl_trans_cb( usb_utr_t *mess )
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
    /* bit0 : Num lock */
    if( leddata & USB_BIT0 )
    {
        usb_cpu_led_set_bit( (uint8_t)LED_POS0, (uint8_t)LED_ON );
    }
    else
    {
        usb_cpu_led_set_bit( (uint8_t)LED_POS0, (uint8_t)LED_OFF );
    }

    /* bit1 : Caps lock */
    if( leddata & USB_BIT1 )
    {
        usb_cpu_led_set_bit( (uint8_t)LED_POS1, (uint8_t)LED_ON );
    }
    else
    {
        usb_cpu_led_set_bit( (uint8_t)LED_POS1, (uint8_t)LED_OFF );
    }


    /* bit2 : Scroll lock */
    if( leddata & USB_BIT2 )
    {
        usb_cpu_led_set_bit( (uint8_t)LED_POS2, (uint8_t)LED_ON );
    }
    else
    {
        usb_cpu_led_set_bit( (uint8_t)LED_POS2, (uint8_t)LED_OFF );
    }


#if 0
    /* bit3 : Compose */
    if( leddata & USB_BIT3 )
    {
        usb_cpu_LedSetBit( (uint8_t)(3), (uint8_t)USB_YES );     /* RSK LED3 ( Compose ) ON */
    }
    else
    {
        usb_cpu_LedSetBit( (uint8_t)(3), (uint8_t)USB_OFF );    /* RSK LED3 ( Compose ) OFF */
    }

    /* bit4 : Kana */
    if( leddata & USB_BIT4 )
    {
        usb_cpu_LedSetBit( (uint8_t)(4),(uint8_t)USB_YES);       /* RSK LED4 ( Kana ) ON */
    }
    else
    {
        usb_cpu_LedSetBit( (uint8_t)(4), (uint8_t)USB_OFF );    /* RSK LED4 ( Kana ) OFF */
    }
#endif
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
    uint16_t    ret, keydata;


    ret = USB_NO;

    /* Branch by the RSK Switch Input. */
    switch( key_data )
    {
        case NO_PUSH:   /* No key */
            usb_sphid_key_time =0;
        break;

        case SW2_PUSH:  /* SW2 a - z */
            keydata=0;
            if( usb_sphid_key_time == USB_0 )
            {
                keydata=1;
            }
            if( USB_KBD_SW_TIME < usb_sphid_key_time)
            {
                keydata=1;
                usb_sphid_key_time-=USB_KBD_SW_CONT;
            }
            usb_sphid_key_time++;

            if(keydata==1)
            {
                if( usb_sphid_keyboard_key == USB_PHID_KBD_CODE_Z )         /* Keyboard data check == "Z" */
                {
                    usb_sphid_keyboard_key = USB_PHID_KBD_CODE_ENTER;       /* Keyboard data "Enter" set. */
                }
                else if( ((USB_PHID_KBD_CODE_A - 1) < usb_sphid_keyboard_key)
                        && (usb_sphid_keyboard_key < USB_PHID_KBD_CODE_Z) )
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
            }
        break;


    }

    return  ret;
}   /* eof usb_phid_apl_kbd_data_set() */
#endif  /* Peripheral HID Application keyboard mode */


#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
/******************************************************************************
Function Name   : usb_phid_demo_mode_report
Description     : RSK demo ( Toggle LED or Read ADC or Update LCD )
Arguments       : uint8_t *recdata      : Demo mode request data.( Set by the Set Request )
Return value    : none
******************************************************************************/
void usb_phid_demo_mode_report( uint8_t *recdata )
{
    uint16_t    ad_data;
    uint8_t     sz_line1[ NUMB_CHARS_PER_LINE + 1 ];
    uint8_t     sz_line2[ NUMB_CHARS_PER_LINE + 1 ];


    /* Toggle LED ? */
    if( 0 != (recdata[0] & USB_DEMO_REPORTLED) )
    {
        /* LED OFF/ON check */
        if( usb_sphid_led0 == USB_NO )
        {
            /* LED ON */
            usb_cpu_led_set_bit( (uint8_t)LED_POS0, (uint8_t)LED_ON );
            usb_sphid_led0 = USB_YES;                                    /* RSK LED0 status OFF => ON */
        }
        else
        {
            /* LED OFF */
            usb_cpu_led_set_bit( (uint8_t)LED_POS0, (uint8_t)LED_OFF );
            usb_sphid_led0 = USB_NO;                                   /* RSK LED0 status ON => OFF */
        }
    }

    /* Read ADC ? */
    if( 0 != (recdata[0] & USB_DEMO_REPORTADC) )
    {
        ad_data = usb_phid_apl_AdData();                    /* Read A/D data */
//@@@        ad_data = (uint16_t)(ad_data & 0x03FF);

        /* Send response. Return RSK A/D */
        usb_phid_apl_create_report( 1, 0, ad_data );       /* Make report data. */

        /* Set transfer data. (Report data) */
        usb_phid_apl_memcpy( (uint8_t *)usb_gphid_TransData, (uint8_t *)usb_sphid_smpl_trn_data, USB_INTEPMAXP );

        /* Demo mode Report transfer */
        R_usb_phid_send_data( usb_sphid_smpl_trn_data, USB_INTEPMAXP, (usb_cb_t)&usb_phid_apl_trans_cb );
    }


    /* Update LCD */
    if( 0 != (recdata[0] & USB_DEMO_REPORTLCD) )
    {
        /* RSK debug LCD display end code set */
        sz_line1[ NUMB_CHARS_PER_LINE ] = '\0';
        sz_line2[ NUMB_CHARS_PER_LINE ] = '\0';

        /* RSK debug LCD display code copy */
        usb_phid_apl_memcpy( sz_line1, &recdata[1], NUMB_CHARS_PER_LINE );
        usb_phid_apl_memcpy( sz_line2, &recdata[ 1 + NUMB_CHARS_PER_LINE ], NUMB_CHARS_PER_LINE );

        /* RSK debug LCD display update */
        usb_smpl_lcd_string(LCD_POS_U0, sz_line1 );
        usb_smpl_lcd_string(LCD_POS_D0, sz_line2 );
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
void usb_phid_apl_create_report( uint8_t badc, uint8_t bswpressed, uint16_t adcvalue )
{
    /* Clear to start */
    usb_sphid_smpl_trn_data[0] = 0;

    /* Set LED Bit */
    if( usb_sphid_led0 == USB_YES )
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
    usb_sphid_smpl_trn_data[1] = (uint8_t)( adcvalue        & 0xFF);
    usb_sphid_smpl_trn_data[2] = (uint8_t)((adcvalue >>  8) & 0xFF);
    usb_sphid_smpl_trn_data[3] = (uint8_t)(0x00);
    usb_sphid_smpl_trn_data[4] = (uint8_t)(0x00);
}   /* eof usb_phid_apl_create_report() */


/******************************************************************************
Function Name   : usb_phid_apl_AdData
Description     : Get A/D data
Return value    : none
Return value    : uint16_t  : A/D DATA
******************************************************************************/
uint16_t usb_phid_apl_AdData( void )
{
//@@@    usg_apl_g_ad_data++;
    usg_apl_g_ad_data = usb_smpl_ad_read();
    return ( usg_apl_g_ad_data );
}   /* eof usb_phid_apl_AdData() */
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
    switch( key_data )
    {
        case NO_PUSH:   /* No key */
            if( usb_sphid_mouse_time != 0 )                                     /* Check RSK Switch push time(count) */
            {
                /* Mouse mode report data clear */
                data[ USB_PHID_MSE_BTN_DATA_INDEX ] = (uint8_t)0x00;
                data[ USB_PHID_MSE_X_DATA_INDEX ]   = (uint8_t)0x00;
                data[ USB_PHID_MSE_Y_DATA_INDEX ]   = (uint8_t)0x00;
                data[3] = (uint8_t)0x00;

                usb_sphid_mouse_time = 0;                                       /* RSK Switch push time(count) clear */

                /* X axial direction is switched */
                if( (usb_sphid_check_switch & 0x01) == 0x01 )                   /* SW2 up/down check */
                {
                    usb_sphid_mouse_up_down ^= 0x01;                            /* X axial direction is switch */
                    usb_sphid_check_switch &= ~0x01;                            /* SW2 up/down clear */
                }

                /* Y axial direction is switched */
                if( (usb_sphid_check_switch & 0x10) == 0x10 )                   /* SW3 left/right check */
                {
                    usb_sphid_mouse_left_right ^= 0x01;                         /* Y axial direction is switch */
                    usb_sphid_check_switch &= ~0x10;                            /* SW3 left/right clear */
                }

                ret = USB_YES;                                                  /* Mouse mode data set. */
            }
        break;

        case SW2_PUSH:      /* SW2 down/up */
            if( usb_sphid_mouse_up_down == 0x00 )   /* Movement to lower direction */
            {
                /* Check RSK Switch push time(count) */
                if( usb_sphid_mouse_time == USB_MSE_SW_PUSH_THRESHOLD_HIGH )
                {
                    data[ USB_PHID_MSE_Y_DATA_INDEX ] = USB_PHID_MSE_UP_DISTANSE_MAX;       /* YMove +1 */
                }
                else if( (USB_MSE_SW_PUSH_THRESHOLD_LOW < usb_sphid_mouse_time)
                        && (usb_sphid_mouse_time < USB_MSE_SW_PUSH_THRESHOLD_HIGH) )
                {
                    data[ USB_PHID_MSE_Y_DATA_INDEX ] = USB_PHID_MSE_UP_DISTANSE_MID;       /* YMove +1 */
                    usb_sphid_mouse_time++;                                 /* RSK Switch push time(count) update */
                }
                else
                {
                    data[ USB_PHID_MSE_Y_DATA_INDEX ] = USB_PHID_MSE_UP_DISTANSE_MIN;       /* YMove +1 */
                    usb_sphid_mouse_time++;                                 /* RSK Switch push time(count) update */
                }
            }
            else if( usb_sphid_mouse_up_down == 0x01 )  /* Movement to upper direction */
            {
                /* Check RSK Switch push time(count) */
                if( usb_sphid_mouse_time == USB_MSE_SW_PUSH_THRESHOLD_HIGH )
                {
                    data[ USB_PHID_MSE_Y_DATA_INDEX ] = USB_PHID_MSE_DOWN_DISTANSE_MAX;     /* YMove -1 */
                }
                else if( (USB_MSE_SW_PUSH_THRESHOLD_LOW < usb_sphid_mouse_time)
                        && (usb_sphid_mouse_time < USB_MSE_SW_PUSH_THRESHOLD_HIGH) )
                {
                    data[ USB_PHID_MSE_Y_DATA_INDEX ] = USB_PHID_MSE_DOWN_DISTANSE_MID;     /* YMove -1 */
                    usb_sphid_mouse_time++;             /* RSK Switch push time(count) update */
                }
                else
                {
                    data[ USB_PHID_MSE_Y_DATA_INDEX ] = USB_PHID_MSE_DOWN_DISTANSE_MIN;     /* YMove -1 */
                    usb_sphid_mouse_time++;             /* RSK Switch push time(count) update */
                }
            }
            else
            {
            }

            usb_sphid_check_switch |= 0x01;             /* SW2 up/down push */
            ret = USB_YES;                              /* Mouse mode data set. */
        break;

        case SW1_PUSH:      /* SW1 Click */
            if( usb_sphid_mouse_time == 0 )                     /* Check RSK Switch push time(count) */
            {
                data[ USB_PHID_MSE_BTN_DATA_INDEX ] = 0x01;     /* Set Left Click for report data */
                usb_sphid_mouse_time++;                         /* RSK Switch push time(count) update */
                ret = USB_YES;                                  /* Mouse mode data set. */
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
Return value    : none
Return value    : none
******************************************************************************/
void usb_phid_apl_key_on_wakeup( void )
{
    R_usb_phid_ChangeDeviceState( USB_DO_REMOTEWAKEUP );
}   /* eof usb_phid_apl_key_on_wakeup() */


/******************************************************************************
Function Name   : usb_phid_smpl_message_send
Description     : Transfer message to message box of demo sample application.
Arguments       : usb_utr_t *ptr  : USB internal structure. Selects USB channel.
                : uint16_t   msginfo        : Message Information Kind
Return value    : none
******************************************************************************/
void usb_phid_smpl_message_send( usb_utr_t *ptr, usb_strct_t msginfo )
{
    usb_er_t    err;


    ptr->msginfo = msginfo;     /* Message Information Kind */
    /* Send message */
    err = R_USB_SND_MSG( USB_PHIDSMP_MBX, (usb_msg_t*)ptr );
    if( err != USB_E_OK )
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE2);      /* error */
    }
}   /* eof usb_phid_smpl_message_send() */


/******************************************************************************
Function Name   : usb_phid_apl_open
Description     : USB Peripheral Human Interface Devices Class open function
Arguments       : usb_utr_t *ptr        : USB internal structure. Selects USB channel.
                : uint16_t data1        : Not use
                : uint16_t device_state : Not use
Return value    : usb_er_t              : USB_E_OK etc
******************************************************************************/
usb_er_t usb_phid_apl_open( usb_utr_t *ptr, uint16_t data1, uint16_t device_state )
{
    uint8_t     pipe_no;


    /* PHID Application process enable */
    usb_gphid_connected = USB_YES;

    for ( pipe_no = 0; pipe_no < (USB_MAX_PIPE_NO + 1); pipe_no++ )
    {
        g_usb_pipe_toggle[ pipe_no ] = USB_NULL;
    }


#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
    /* Display "USB DEM ATTACH" on LCD */
    usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"USB DEM " );
    usb_smpl_lcd_string(LCD_POS_D0, (uint8_t *)"ATTACH  " );
#endif  /* Peripheral HID Application demo mode */


#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
    /* Display "USB KBD ATTACH" on LCD */
    usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"USB KBD " );
    usb_smpl_lcd_string(LCD_POS_D0, (uint8_t *)"ATTACH  " );
#endif  /* Peripheral HID Application keyboard mode */


#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
    /* Display "USB MSE ATTACH" on LCD */
    usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"USB MSE " );
    usb_smpl_lcd_string(LCD_POS_D0, (uint8_t *)"ATTACH  " );
#endif  /* Peripheral HID Application mouse mode */

    usb_gphid_application_state = USB_PHID_APL_OPEN;    /* Update Application sequence */

    return USB_E_OK;
}   /* eof usb_phid_apl_open() */


/******************************************************************************
Function Name   : usb_phid_apl_close
Description     : USB Peripheral Human Interface Devices Class close function
Arguments       : usb_utr_t *ptr        : USB internal structure. Selects USB channel.
                : uint16_t data1        : Not use
                : uint16_t device_state : Not use
Return value    : usb_er_t        : USB_E_OK etc
******************************************************************************/
usb_er_t usb_phid_apl_close( usb_utr_t *ptr, uint16_t data1, uint16_t device_state )
{
    /* Notify Sample class detatch */
    usb_phid_smpl_message_send( ptr, (usb_strct_t)USB_PHID_CLOSE );

    /* PHID Application process disable */
    usb_gphid_connected = USB_NO;

    return USB_E_OK;
}   /* eof usb_phid_apl_close() */


/******************************************************************************
Function Name   : usb_phid_apl_DummyFunction
Description     : Application dummy function
Arguments       : usb_utr_t *ptr         : USB internal structure. Selects USB channel.
                : uint16_t data1         : Not use
                : uint16_t device_state  : Not use
Return value    : usb_er_t               : USB_E_OK etc
******************************************************************************/
usb_er_t usb_phid_apl_DummyFunction( usb_utr_t *ptr, uint16_t data1, uint16_t device_state )
{
    return USB_E_OK;
}   /* eof usb_phid_apl_DummyFunction() */


/******************************************************************************
Function Name   : usb_phid_disconnected
Description     : Get the USB cable connected state.
Return value    : none
Return value    : USB_YES = Connected, USB_OFF = Disconnected.
******************************************************************************/
uint16_t usb_phid_disconnected( void )
{
    return usb_gphid_connected;
}   /* eof usb_phid_disconnected() */


/******************************************************************************
Function Name   : usb_phid_apl_memcpy
Description     : memory copy
Arguments       : uint8_t *dst;
                : uint8_t *src;
                : uint16_t length;
Return value    : none
******************************************************************************/
void usb_phid_apl_memcpy( uint8_t *dst, uint8_t *src, uint16_t length )
{
    while ( length > 0 )
    {
        *dst++ = *src++;
        length--;
    }
}   /* eof usb_phid_apl_memcpy() */


/******************************************************************************
Function Name   : usb_phid_apl_memset
Description     : memory data set
Arguments       : uint8_t *dst;
                : uint8_t data;
                : uint16_t length;
Return value    : none
******************************************************************************/
void usb_phid_apl_memset( uint8_t *dst, uint8_t data, uint16_t length )
{
    while ( length > 0 )
    {
        *dst++ = data;
        length--;
    }
}   /* eof usb_phid_apl_memset() */


/******************************************************************************
End  Of File
******************************************************************************/

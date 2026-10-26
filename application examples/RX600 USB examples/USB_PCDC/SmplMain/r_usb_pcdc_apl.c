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
* File Name    : r_usb_PCDC_apl.c
* Version      : 1.10
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Peripheral Communications Devices Class Sample Code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 06.08.2010 0.91    First Release
*         : 31.08.2010 0.92    PCDC Updated
*         : 30.09.2010 0.93    PCDC Updated
*         : 02.12.2010 0.96    Message Output Timming adjustment
*         : 02.12.2010 0.96    Spelling modification
*         : 01.06.2011 1.10    Version 1.10 Release
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
#include "r_usb_ansi.h"
#include "r_usb_control.h"
#include "r_usb_cusb_bitdefine.h"

#include "r_usb_api.h"
#include "r_usb_pcdc_define.h"
#include "r_usb_pcdc_extern.h"
#include "r_usb_pcdc_api.h"
#include "hw_resource.h"

/******************************************************************************
Constant macro definitions
******************************************************************************/
#define     USB_PCDC_SW1_ON                     1
#define     USB_PCDC_SW2_ON                     2
#define     USB_PCDC_SW3_ON                     4

#define     USB_PCDC_MSG_TIMING                 41317

#define     USB_PCDC_APL_INST_SEQ_START         3
#define     USB_PCDC_APL_INST_SEQ_END           8

/* CDC Host data receive size */
/* FS:64 HS:512 */
/* Condition compilation by the difference of user define */
#if USB_SPEEDSEL_PP == USB_HS_PP
#define     USB_PCDC_APL_RX_SIZE                512
#else   /* USB_SPEEDSEL_PP == USB_HS_PP */
#define     USB_PCDC_APL_RX_SIZE                64
#endif  /* USB_SPEEDSEL_PP == USB_HS_PP */

/* Comm Port data receive size */
#define USB_PCDC_APL_SRX_SIZE                   64

/* setup packet table size (uint16_t * 5) */
#define     USB_CDC_SETUP_TBL_BSIZE             10

/*****************************************************************************
Enumerated Types
******************************************************************************/
/* Peripheral CDC Sample application message command */
typedef enum
{
    USB_PCDC_OPEN,
    /* Periodic processing */
    USB_PCDC_PERIODIC_PROCESS,
    /* Data receive from Host cdc */
    USB_PCDC_RX_COMP,
    /* Data send for Host cdc */
    USB_PCDC_TX_COMP,
    /* Notification  */
    USB_PCDC_STATUS_TX_COMP,
    USB_PCDC_CLOSE
}
USB_PCDC_APL_COMMAND;

/******************************************************************************
External variables and functions
******************************************************************************/
/* Serial Port driver functions */
extern void     usb_cpu_Sci_DataSend(uint16_t mode, void *tranadr, uint16_t length);
extern uint16_t usb_cpu_Sci_DataReceive(uint8_t *tranadr, uint16_t receive_length);
extern uint16_t usb_cpu_Sci_StxBuffStatus(void);
extern void     usb_cpu_Sci_Buffer_init(void);
extern uint16_t usb_cpu_Sci_CopyData_for_Echo(void);
extern uint16_t usb_cpu_Sci_GetSerialState(uint16_t *serial_state);

#ifdef USB_UART_ENABLE
extern void     usb_cpu_Sci_HW_init(void);
extern void     usb_cpu_Sci_enable(void);
extern void     usb_cpu_Sci_disable(void);
#endif /* USB_UART_ENABLE */

/* SW input driver functions */
#ifdef USB_KEY_ENABLE
extern uint16_t usb_cpu_GetKeyNo(void);
#endif /* USB_KEY_ENABLE */

#ifdef  USB_LCD_ENABLE
extern  void    usb_cpu_LcdDisp(uint8_t position, uint8_t *string);
#endif /* USB_LCD_ENABLE */

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
/* Abstract Control Model Notification - SerialState */
uint8_t usb_gpcdc_SerialState_Table[USB_CDC_SETUP_TBL_BSIZE]
    = { 0xA1,           /* bmRequestType */
        0x20,           /* bNotification:SERIAL_STATE */
        0x00,0x00,      /* wValue:Zero */
        0x00,0x00,      /* wIndex:Interface */
        0x02,0x00,      /* wLength:2 */
        0x00,0x00 };    /* Data:UART State bitmap */

/* Peri CDC application enable */
uint16_t usb_gcdc_connected;

void        usb_pcdc_main_task(USB_VP_INT_t);
uint16_t    usb_pcdc_is_connected(void);
void        usb_pcdc_pr_apl_title(void);

/******************************************************************************
Static variables and functions
******************************************************************************/
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
static  int16_t usb_spcdc_data_fn;
static  int16_t usb_spcdc_ctl_fn;
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
static  uint16_t usb_pcdc_get_rcv_data_cnt(USB_UTR_t *mess);
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */

static  void    usb_pcdc_rx_notification(USB_UTR_t *mess);
static  void    usb_pcdc_tx_notification(USB_UTR_t *mess);
static  void    usb_pcdc_state_notification(USB_UTR_t *mess);
static  void    usb_pcdc_apl_msg_out(USB_UTR_t *ptr, uint16_t message_no);
static  void    usb_pcdc_apl_init(USB_UTR_t *ptr);
static  void    usb_pcdc_smpl_message_send( USB_UTR_t *ptr, uint16_t msginfo);

static  void    usb_apl_task_switch(void);
static  void    usb_pcdc_task_start( void );
static  void    usb_papl_task_start( USB_UTR_t *ptr );
static  void    usb_pcdc_registration(USB_UTR_t *ptr);

/*Main menu text*/
//static  const uint8_t szWelcomeMsg[9][64] = 
uint8_t szWelcomeMsg[9][64] = 
{
    {
        "\r\nRenesas USB CDC Sample, Press Switch SW2\r\n"
    },
    {
        "\r\nStarting Echo:-\r\n"
    },
    {
        "\r\nFinished Echo.\r\n"
    },
    {
        "\r\n[Renesas Serial-USB converter.]\r\n\r\n "
    },
    {
        "Connect serial terminal (e.g. PC COM1) DB9 cable to PC and RSK,"
    },
    {
        "\r\n and USB cable to PC and to mini-B on RSK.\r\n    "
    },
    {
        "                       Support COM Speed 1200bps-115200bps\r\n"
    },
    {
        "\r\nSw2 - Show instructions\r\n"
    },
    {
        "\r\nSw3 - Start Echo of everthing typed <-> Stop Echo\r\n"
    },
};

/* USB TX Data */
static  uint8_t     usb_gpcdc_send_data[USB_PCDC_APL_SRX_SIZE + 4];
/* USB RX Data */
static  uint8_t     usb_gpcdc_receive_data[USB_PCDC_APL_RX_SIZE + 4];
/* Instructions output line sequence */
static  uint16_t    usb_gpcdc_explain_seq;
/* HOST Send wait flag */
static  uint16_t    usb_gpcdc_tx_wait_flag;
/* Notification Serial State sending flag */
static  uint16_t    usb_gpcdc_serial_state_tx;
/* Notification Serial State sending status */
static  uint16_t    usb_gpcdc_serial_state;

/* echo mode flag */
static  uint16_t    usb_gpcdc_echo_mode;
static  uint16_t    usb_gpcdc_sw2push;
/* Switch1 Push Request Message Output Timing Counter */
static  uint32_t    usb_gpcdc_msg_timing_cnt;
/* Receive flow control flag */
static  uint16_t    usb_gpcdc_stx_wait;
/* CDC Peripheral Active flag */
static  uint16_t    usb_gpcdc_active;

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_apl"

/******************************************************************************
Renesas Peripheral Communications Devices Class Sample Code functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_papl_task_start
Description     : Set peripheral application task start conditions. 
Arguments       : none
Return value    : none
******************************************************************************/
void    usb_papl_task_start( USB_UTR_t *ptr )
{
    /* PCDC demo sample application task priority set */
    R_usb_cstd_SetTaskPri(USB_PCDCSMP_TSK, USB_PRI_6);

#ifdef USB_UART_ENABLE
    /* ComPort Hw Initialize */
    usb_cpu_Sci_HW_init();
#endif /* USB_UART_ENABLE */

    /* Application data Initialize and Periodic process request */
    usb_pcdc_apl_init(ptr);
}
/******************************************************************************
End of function usb_papl_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_task_start
Description     : Start task CSTD; "common" basic USB low level task.
Arguments       : USB_UTR_t *ptr        : USB system internal structure.
Return value    : none
******************************************************************************/
void usb_cstd_task_start( void )
{
    usb_cstd_IdleTaskStart();   /* Idle Task Start */

    usb_pcdc_task_start();      /* Start Peripheral USB driver */

    usb_apl_task_switch();      /* Switch task for nonOS */
}
/******************************************************************************
End of function usb_cstd_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_pstd_task_start
Description     : Set peripheral application task start conditions. 
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pcdc_task_start( void )
{
    USB_UTR_t   utr;
    USB_UTR_t   *ptr;

#ifdef  USB_LCD_ENABLE
    /* Display Debug LCD */
    usb_cpu_LcdDisp( LCD_POS_U0, "USB CDC ");
    usb_cpu_LcdDisp( LCD_POS_D0, "Appstart");
#endif /* USB_LCD_ENABLE */

    ptr = &utr;                             /* Set USB IP no. and USB IP base address store pointer */
    ptr->ip = USB_PERI_USBIP_NUM;           /* IP number(0or1) */
    if( USB_NOUSE_PP != ptr->ip )           /* USB IP number check */
    {
        ptr->ipp = R_usb_cstd_GetUsbIpAdr( ptr->ip );   /* IP Address(USB0orUSB1) */

        usb_pcdc_registration( ptr );       /* Peripheral Application Registration */
        usb_papl_task_start( ptr );         /* Peripheral Application Task Start Setting */
        R_usb_pcdc_driver_start();          /* Peripheral Class Driver Task Start Setting */
        R_usb_pstd_usbdriver_start( ptr );  /* Peripheral USB Driver Start Setting */

        /* Initialize USB IP */
        R_usb_cstd_UsbIpInit( ptr, USB_PERI_PP );
    }
}
/******************************************************************************
End of function usb_pstd_task_start()
******************************************************************************/

/******************************************************************************
Function Name   : usb_pcdc_registration
Description     : Registration of peripheral Communications Devices Driver
Arguments       : USB_UTR_t *ptr        : USB system internal structure.
Return value    : none
******************************************************************************/
void usb_pcdc_registration(USB_UTR_t *ptr)
{
    USB_PCDREG_t    driver;

    /* Driver registration */
    /* Pipe Define Table address */
    driver.pipetbl      = &usb_gpcdc_EpPtr[0];
    /* Device descriptor Table address */
    driver.devicetbl    = (uint8_t*)&usb_gpcdc_DeviceDescriptor;
    /* Qualifier descriptor Table address */
    driver.qualitbl     = (uint8_t*)&usb_gpcdc_QualifierDescriptor;
    /* Configuration descriptor Table address */
    driver.configtbl    = (uint8_t**)&usb_gpcdc_ConPtr;
    /* Other configuration descriptor Table address */
    driver.othertbl     = (uint8_t**)&usb_gpcdc_ConPtrOther;
    /* String descriptor Table address */
    driver.stringtbl    = (uint8_t**)&usb_gpcdc_StrPtr;
    /* Driver init */
    driver.classinit    = &usb_cstd_DummyFunction;
    /* Device default */
    driver.devdefault   = &R_usb_pcdc_descriptor_change;
    /* Device configuered */
    driver.devconfig    = (USB_CB_INFO_t)&usb_pcdc_open;
    /* Device detach */
    driver.devdetach    = (USB_CB_INFO_t)&usb_pcdc_close;
    /* Device suspend */
    driver.devsuspend   = &usb_cstd_DummyFunction;
    /* Device resume */
    driver.devresume    = &usb_cstd_DummyFunction;
    /* Interfaced change */
    driver.interface    = &R_usb_pcdc_set_interface;
    /* Control Transfer */
    driver.ctrltrans    = (USB_CB_TRN_t)&R_usb_pcdc_usr_ctrl_trans_function;

    /* PCDC class driver registration  */
    R_usb_pstd_DriverRegistration(ptr, &driver);
}   /* eof usb_pcdc_registration() */

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

        /* Task schedule request check. */
        if( USB_FLGSET == R_usb_cstd_CheckSchedule() )
        {
#ifdef  USB_PERI_MODE_PP
            R_usb_pstd_PcdTask((USB_VP_INT)0);          /* PCD Task */
            /* Peripheral Communications Devices Class Task */
            R_usb_pcdc_task(0);
            /* Peripheral Communications Class Application Task */
            usb_pcdc_main_task(0);
#endif  /* USB_PERI_MODE_PP */
            /* Idle Task (sleep sample) */
            R_usb_cstd_IdleTask(0);
        }
    }
}   /* eof usb_apl_task_switch() */

/******************************************************************************
Function Name   : usb_pcdc_pr_apl_title
Description     : Output Title
Argument        : none
Return          : none
******************************************************************************/
void usb_pcdc_pr_apl_title(void)
{
#ifdef  USB_LCD_ENABLE
    usb_cpu_TargetLcdClear();
#endif /* USB_LCD_ENABLE */
    USB_PRINTF0("\n");
    USB_PRINTF0("+++++++++++++++++++++++++++++++++++++\n");
    USB_PRINTF0("+   PERI CDC FW SAMPLE              +\n");
    USB_PRINTF0("+                                   +\n");
    USB_PRINTF0("+     RENESAS ELECTRONICS CORP.     +\n");
    USB_PRINTF0("+     RENESAS SOLUTIONS  CORP.      +\n");
    USB_PRINTF0("+++++++++++++++++++++++++++++++++++++\n");
    USB_PRINTF1("       DATE [%s] \n", __DATE__);
    USB_PRINTF1("       TIME [%s] \n", __TIME__);
}   /* eof usb_pcdc_pr_apl_title() */

/******************************************************************************
Function Name   : usb_pcdc_sw_process
Description     : Switch Push Management process
                : - After connect,SW2 push Message output. Message stop by RSK SW2 push.
                : - SW2 : Transfer  operate description  Message 
                : - SW3 : Echo mode On/Off change
                :   ( When Echo mode Off, to USB serial converter)
Arguments       : USB_UTR_t *ptr        : USB system internal structure.
Return value    : none
******************************************************************************/
void usb_pcdc_sw_process(USB_UTR_t *ptr)
{
    uint16_t key_data;

    /* SW1-3 Polling */
    /*----------------------------------*/
    /* Initial message SW2Input request */
    /*----------------------------------*/
    /* Send Complete wait flag check */
    if((usb_gpcdc_tx_wait_flag == USB_OFF) && (usb_gpcdc_sw2push == USB_OFF))
    {
        /* Message output timming check */
        usb_gpcdc_msg_timing_cnt++;
        if(usb_gpcdc_msg_timing_cnt >= USB_PCDC_MSG_TIMING)
        {
            usb_gpcdc_msg_timing_cnt = 0;
            /* Show instructions Start */
            usb_pcdc_apl_msg_out(ptr, 0);
        }
    }
    /*----------------------------------*/
    /* SW Polling process               */
    /*----------------------------------*/
    /* Send Complete wait flag check */
    if(usb_gpcdc_tx_wait_flag == USB_OFF)
    {
        /* Switch Input */
#ifdef USB_KEY_ENABLE
        key_data = usb_cpu_GetKeyNo();
#else   /* USB_KEY_ENABLE */
        key_data = USB_OFF;
#endif  /* USB_KEY_ENABLE */
        switch(key_data)
        {
        case USB_PCDC_SW1_ON:
            break;
        case USB_PCDC_SW2_ON:
            /* SW2 ON */
            usb_gpcdc_sw2push = USB_ON;
            /* Set Operate description Message start no. */
            usb_gpcdc_explain_seq = USB_PCDC_APL_INST_SEQ_START;
            /* Show instructions Start */
            usb_pcdc_apl_msg_out(ptr, usb_gpcdc_explain_seq);
            break;

        case USB_PCDC_SW3_ON:
            /* Change Echo mode /USB serial conversion mode */
            if( usb_gpcdc_echo_mode == USB_OFF )
            {
                /* Show instructions Start */
                usb_pcdc_apl_msg_out(ptr, 1);
                /* Start Echo */
                usb_gpcdc_echo_mode = USB_ON;
#ifdef USB_UART_ENABLE
                /* Serial port disable */
                usb_cpu_Sci_disable();
#endif /* USB_UART_ENABLE */
            }
            else
            {
                /* Peri CDC Application Tx Rx data clear */
                usb_cpu_Sci_Buffer_init();
                /* Show instructions Start */
                usb_pcdc_apl_msg_out(ptr, 2);
                /* Stop Echo */
                usb_gpcdc_echo_mode = USB_OFF;
#ifdef USB_UART_ENABLE
                /* Serial port enable */
                usb_cpu_Sci_enable();
#endif /* USB_UART_ENABLE */
            }
            break;

        default:
            /* No switch push */
            /* Show instructions Continue */
            if(( usb_gpcdc_explain_seq >= USB_PCDC_APL_INST_SEQ_START )
                && (usb_gpcdc_explain_seq < USB_PCDC_APL_INST_SEQ_END))
            {
                /* Next operate description Message no. update */
                usb_gpcdc_explain_seq++;
                /* Show instructions Start */
                usb_pcdc_apl_msg_out(ptr, usb_gpcdc_explain_seq);
            }
            break;
        }
    }
}   /* eof usb_pcdc_sw_process() */

/******************************************************************************
Function Name   : usb_pcdc_serial_state_process
Description     : Serial error process
                : Transfer Class Notification SerialState when UART(SCI) error detection
                : ( Parity error / Framing error / Over run error )
Arguments       : USB_UTR_t *ptr        : USB system internal structure.
Return value    : none
******************************************************************************/
void usb_pcdc_serial_state_process(USB_UTR_t *ptr)
{
    /* SerialState(CDC PSTN Subclass UART State bitmap) */
    uint16_t    serial_stat;
    /* Serial error detect */
    uint16_t    serial_error;
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    USB_ER_t    err = 0l;
    int16_t     state;
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP   */

    /*----------------------------------*/
    /* Communications Port Error check  */
    /*----------------------------------*/
    /* Serial State Notification send flag check */
    if(usb_gpcdc_serial_state_tx == USB_OFF)
    {
        /* Serial error status read */
        serial_error = usb_cpu_Sci_GetSerialState(&serial_stat);
        if(serial_error)
        {
            /* SerialState notification when different error type to last notification error type */
            if(serial_stat != usb_gpcdc_serial_state)
            {
                /* Notifications Serial State */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
                /* UART State bitmap set */
                usb_gpcdc_SerialState_Table[8] = (uint8_t)(serial_stat & 0xff);
                usb_gpcdc_SerialState_Table[9] = (uint8_t)(serial_stat >> 8);

                /* Transfer Class Notification SerialState */
                write( usb_spcdc_ctl_fn, (uint8_t*)&usb_gpcdc_SerialState_Table[0], USB_CDC_SETUP_TBL_BSIZE );
                /* Write status check */
                err = control( usb_spcdc_ctl_fn, USB_CTL_GET_WR_STATE, (void*)&state );
                if( ( err == -1 ) || ( state != USB_CTL_ERR_PROCESS_WAIT ) )
                {
                    USB_PRINTF0("### usb_pcdc_serial_state_process function data write error\n");
                }
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP   */
                /* Transfer Class Notification SerialState */
                R_usb_pcdc_SerialStateNotification(ptr, serial_stat, (USB_CB_t)&usb_pcdc_state_notification);
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP   */
                /* Serial State Notification send flag set */
                usb_gpcdc_serial_state_tx = USB_ON;
                /* Serial Erros State Store */
                usb_gpcdc_serial_state = serial_stat;
            }
        }
        else
        {
            /* Serial Erros State Clear */
            usb_gpcdc_serial_state = 0;
        }
    }
}   /* eof usb_pcdc_serial_state_process() */

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
/******************************************************************************
Function Name   : usb_pcdc_main_task
Description     : Peri CDC Sample Application
Argument        : USB_VP_INT_t
Return          : none
******************************************************************************/
void usb_pcdc_main_task(USB_VP_INT_t start_code)
{
    USB_UTR_t   *mess;
    USB_ER_t    err = 0l;

    /* Receive Data Count for Comm Port */
    uint16_t    srcv_cnt;
    /* Receive Data Count for USB Host */
    int32_t     data_cnt;
    int16_t     state;

    /* Recieve message for message box of PCDC demo sample application */
    err = R_USB_TRCV_MSG(USB_PCDCSMP_MBX,(USB_MSG_t**)&mess,(uint16_t)3000);
    if( err == USB_E_OK )
    {
        /* Branch by message infomation. */
        switch(mess->msginfo)
        {

        case USB_PCDC_OPEN:
            /* PCDC(Data Class) Open */
            usb_spcdc_data_fn = open((int8_t *)USB_CLASS_PCDCD, 0, 0);
            if( usb_spcdc_data_fn == -1 )
            {
                /* Make Other Task Processing Time */
                R_USB_DLY_TSK(10);

                /*  Cdc Data Open retry */
                usb_pcdc_smpl_message_send( mess, USB_PCDC_OPEN );
            }
            else
            {
                /* PCDC(Communications Class) Open */
                usb_spcdc_ctl_fn = open((int8_t *)USB_CLASS_PCDCC, 0, 0); 
                if( usb_spcdc_ctl_fn != -1 )
                {
                    /* Call-Back function set */
                    control( usb_spcdc_data_fn, USB_CTL_RD_NOTIFY_SET, (void*)&usb_pcdc_rx_notification );
                    control( usb_spcdc_data_fn, USB_CTL_WR_NOTIFY_SET, (void*)&usb_pcdc_tx_notification );
                    control( usb_spcdc_ctl_fn, USB_CTL_WR_NOTIFY_SET, (void*)&usb_pcdc_state_notification );

                    /* Sw polling start */
                    usb_pcdc_smpl_message_send( mess, USB_PCDC_PERIODIC_PROCESS );
                }
            }

            /* Release the memory block for the sample application */
            err = R_USB_REL_BLK(USB_PCDCSMP_MPL,(USB_MH_t)mess);
            if( err != USB_OK )
            {
                /* error */
                USB_PRINTF0("### USB PeripheralSampleClass rel_blk error\n");
            }
            break;

        case USB_PCDC_PERIODIC_PROCESS:
            /* Check PCDC(Data Class & Communications Class) Open complete ? */
            if( (usb_spcdc_data_fn != -1) || (usb_spcdc_ctl_fn != -1) )
            {
                /* Sw polling repetition */
                usb_pcdc_smpl_message_send( mess, USB_PCDC_PERIODIC_PROCESS );

                /* Host CDC Connect check */
                if( usb_pcdc_is_connected() == USB_OFF )
                {
                    /* Peri CDC Not Active set */
                    usb_gpcdc_active = USB_OFF;
                }
                else if( usb_gpcdc_active == USB_OFF )
                {
                    /* Peri CDC Active set */
                    usb_gpcdc_active = USB_ON;
                    /* Host CDC Data receive start */
                    usb_gpcdc_stx_wait = USB_ON;
                }
                else
                {
                }

                /*----------------------------------*/
                /* Switch Push Management process   */
                /*----------------------------------*/
                usb_pcdc_sw_process(mess);              /* Input RSK SW & Transfer SW push request Message */

                /*----------------------------------*/
                /* Serial error process             */
                /*----------------------------------*/
                usb_pcdc_serial_state_process(mess);    /* UART(SCI) error check & SerialState transfer. */

                /*----------------------------------*/
                /* Communications Port receive check    */
                /*----------------------------------*/
                /* Send Complete wait flag check */
                if(usb_gpcdc_tx_wait_flag == USB_OFF)
                {
                    /* Echo mode Data copy */
                    if(usb_gpcdc_echo_mode)
                    {
                        /* Echo mode data copy Serial TX -> Serial RX */
                        usb_cpu_Sci_CopyData_for_Echo();
                    }
                    /* Comm port data receive */
                    srcv_cnt = usb_cpu_Sci_DataReceive( usb_gpcdc_send_data, USB_PCDC_APL_SRX_SIZE);
                    if( srcv_cnt != 0 )
                    {
                        /* Comm port receive data -> CDC Host send */
                        write( usb_spcdc_data_fn, (uint8_t*)&usb_gpcdc_send_data, srcv_cnt);
                        /* Write status check */
                        err = control( usb_spcdc_data_fn, USB_CTL_GET_WR_STATE, (void*)&state );
                        if( ( err == -1 ) || ( state != USB_CTL_ERR_PROCESS_WAIT ) )
                        {
                            USB_PRINTF0("### usb_pcdc_main_task function data write error\n");
                        }
                        /* Send Complete wait flag on */
                        usb_gpcdc_tx_wait_flag = USB_ON;
                    }
                }
                /*----------------------------------*/
                /* CDC Host Bulk In Request         */
                /*----------------------------------*/
                if( usb_gpcdc_stx_wait )                /* Check flow control flag */
                {
                    /* Check UART transfer buffer condition. */
                    if( usb_cpu_Sci_StxBuffStatus() == 0)
                    {
                        /* Common Read(BULK OUT) */
                        read( usb_spcdc_data_fn, (uint8_t*)&usb_gpcdc_receive_data, USB_PCDC_APL_RX_SIZE);
                        /* Read status check */
                        err = control( usb_spcdc_data_fn, USB_CTL_GET_RD_STATE, (void*)&state );
                        if( ( err == -1 ) || ( state != USB_CTL_ERR_PROCESS_WAIT ) )
                        {
                            USB_PRINTF0("### usb_pcdc_main_task function bulk read error\n");
                        }
                        /* Receive flow control flag off */
                        usb_gpcdc_stx_wait = USB_OFF;
                    }
                }
            }

            /* Release the memory block for the sample application */
            err = R_USB_REL_BLK(USB_PCDCSMP_MPL,(USB_MH_t)mess);
            if( err != USB_OK )
            {
                /* error */
                USB_PRINTF0("### USB PeripheralSampleClass rel_blk error\n");
            }
            break;

        case USB_PCDC_RX_COMP:
            /* USB receive complete */
            /* Check PCDC(Data Class & Communications Class) Open complete ? */
            if( (usb_spcdc_data_fn != -1) || (usb_spcdc_ctl_fn != -1) )
            {
                /*----------------------------------*/
                /* Get USB Receive Data count       */
                /*----------------------------------*/
                err = control( usb_spcdc_data_fn, USB_CTL_RD_LENGTH_GET, (void*)&data_cnt );
                if( ( err == -1 ) || ( data_cnt == -1 ) )
                {
                    /* Transfer NG */
                    USB_PRINTF0("### usb_pstd_MainTask function int read notify error\n");
                }
                /*----------------------------------*/
                /* CDC Host Bulk In receive check   */
                /*----------------------------------*/
                if(data_cnt > 0)
                {
                    /* Communications Port Data Send Process */
                    usb_cpu_Sci_DataSend(usb_gpcdc_echo_mode, &usb_gpcdc_receive_data , data_cnt);
                }
                /*----------------------------------*/
                /* CDC Host Bulk In Request         */
                /*----------------------------------*/
                if( usb_cpu_Sci_StxBuffStatus() == 0)
                {
                    /* Common Read(BULK OUT) */
                    read( usb_spcdc_data_fn, (uint8_t*)&usb_gpcdc_receive_data, USB_PCDC_APL_RX_SIZE);
                    /* Read status check */
                    err = control( usb_spcdc_data_fn, USB_CTL_GET_RD_STATE, (void*)&state );
                    if( ( err == -1 ) || ( state != USB_CTL_ERR_PROCESS_WAIT ) )
                    {
                        USB_PRINTF0("### usb_pcdc_main_task function bulk read error\n");
                    }
                }
                else
                {
                    /* Receive flow control flag on */
                    usb_gpcdc_stx_wait = USB_ON;
                }
            }
            break;

        case USB_PCDC_TX_COMP:
            /* USB transfer complete */
            /* Check PCDC(Data Class & Communications Class) Open complete ? */
            if( (usb_spcdc_data_fn != -1) || (usb_spcdc_ctl_fn != -1) )
            {
                /*----------------------------------*/
                /* Data send for Host cdc           */
                /*----------------------------------*/
                err = control( usb_spcdc_data_fn, USB_CTL_WR_LENGTH_GET, (void*)&data_cnt );
                if( ( err == -1 ) || ( data_cnt == -1 ) )
                {
                    /* Transfer NG */
                    USB_PRINTF0("### usb_pstd_MainTask function int read notify error\n");
                }

                /* Send Complete wait flag off */
                usb_gpcdc_tx_wait_flag = USB_OFF;
            }
            break;

        case USB_PCDC_STATUS_TX_COMP:
            /*------------------------------------------*/
            /* Serial State Notification send complete  */
            /*------------------------------------------*/
            /* Serial State Notification send flag clear */
            usb_gpcdc_serial_state_tx = USB_OFF;
            break;

        case USB_PCDC_CLOSE:
            if( usb_spcdc_data_fn != -1 )
            {
                close(usb_spcdc_data_fn);
                usb_spcdc_data_fn = -1;
            }
            if( usb_spcdc_ctl_fn != -1 )
            {
                close(usb_spcdc_ctl_fn);
                usb_spcdc_ctl_fn = -1;
            }

            /* Application data Initialize and Periodic process request */
            usb_pcdc_apl_init(mess);

            /* Release the memory block for the sample application */
            err = R_USB_REL_BLK(USB_PCDCSMP_MPL,(USB_MH_t)mess);
            if( err != USB_OK )
            {
                /* error */
                USB_PRINTF0("### USB PeripheralSampleClass rel_blk error\n");
            }
            break;

        default:
            /*----------------------------------*/
            /* Undefine message                 */
            /*----------------------------------*/
            USB_PRINTF1("### Undefine message:%02x\n",mess->msginfo);
            /* Release the memory block for the sample application */
            err = R_USB_REL_BLK(USB_PCDCSMP_MPL,(USB_MH_t)mess);
            if( err != USB_OK )
            {
                /* error */
                USB_PRINTF0("### USB PeripheralSampleClass rel_blk error\n");
            }
            break;
        }
    }
}   /* eof usb_pcdc_main_task() */

#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP   */
/******************************************************************************
Function Name   : usb_pcdc_main_task
Description     : Peri CDC Sample Application
Argument        : USB_VP_INT_t
Return          : none
******************************************************************************/
void usb_pcdc_main_task(USB_VP_INT_t start_code)
{
    USB_UTR_t   *mess;
    USB_ER_t    err = 0l;

    /* Receive Data Count for Comm Port */
    uint16_t    srcv_cnt;
    /* Receive Data Count for USB Host */
    int32_t     data_cnt;

    /* Recive message for message box of PCDC demo sample application. */
    err = R_USB_TRCV_MSG(USB_PCDCSMP_MBX,(USB_MSG_t**)&mess,(uint16_t)3000);
    if( err == USB_E_OK )
    {
        /* Branch by message infomation. */
        switch(mess->msginfo)
        {
        case USB_PCDC_PERIODIC_PROCESS:
            /* Host CDC Connect check */
            if( usb_pcdc_is_connected() == USB_OFF )
            {
                /* Peri CDC Not Active set */
                usb_gpcdc_active = USB_OFF;
            }
            else 
            {
                /* Sw polling repetition */
                usb_pcdc_smpl_message_send( mess, USB_PCDC_PERIODIC_PROCESS );
            }

            if( usb_gpcdc_active == USB_OFF )
            {
                /* Peri CDC Active set */
                usb_gpcdc_active = USB_ON;
                /* Host CDC Data receive start */
                usb_gpcdc_stx_wait = USB_ON;
            }

            /*----------------------------------*/
            /* Switch Push Management process   */
            /*----------------------------------*/
            usb_pcdc_sw_process(mess);              /* Input RSK SW & Transfer SW push request Message */

            /*----------------------------------*/
            /* Serial error process             */
            /*----------------------------------*/
            usb_pcdc_serial_state_process(mess);    /* UART(SCI) error check & SerialState transfer. */

            /*----------------------------------*/
            /* Communications Port receive check    */
            /*----------------------------------*/
            /* Send Complete wait flag check */
            if(usb_gpcdc_tx_wait_flag == USB_OFF)
            {
                /* Echo mode Data copy */
                if(usb_gpcdc_echo_mode)
                {
                    /* Echo mode data copy Serial TX -> Serial RX */
                    usb_cpu_Sci_CopyData_for_Echo();
                }
                /* Comm port data receive */
                srcv_cnt = usb_cpu_Sci_DataReceive( usb_gpcdc_send_data, USB_PCDC_APL_SRX_SIZE );
                if( srcv_cnt != 0 )
                {
                    /* Comm port receive data -> CDC Host send */
                    R_usb_pcdc_SendData(mess, usb_gpcdc_send_data, srcv_cnt, (USB_CB_t)&usb_pcdc_tx_notification);
                    /* Send Complete wait flag on */
                    usb_gpcdc_tx_wait_flag = USB_ON;
                }
            }
            /*----------------------------------*/
            /* CDC Host Bulk In Request         */
            /*----------------------------------*/
            if(usb_gpcdc_stx_wait)                  /* Check flow control flag */
            {
                /* Check UART transfer buffer condition. */
                if( usb_cpu_Sci_StxBuffStatus() == 0)
                {
                    /* Common Read(BULK OUT) */
                    R_usb_pcdc_ReceiveData(mess, (uint8_t *)&usb_gpcdc_receive_data, USB_PCDC_APL_RX_SIZE,
                        (USB_CB_t)&usb_pcdc_rx_notification);
                    /* Receive flow control flag off */
                    usb_gpcdc_stx_wait = USB_OFF;
                }
            }

            /* Release the memory block for the sample application */
            err = R_USB_REL_BLK(USB_PCDCSMP_MPL,(USB_MH_t)mess);
            if( err != USB_OK )
            {
                /* error */
                USB_PRINTF0("### USB PeripheralSampleClass rel_blk error\n");
            }
            break;

        case USB_PCDC_RX_COMP:
            /*----------------------------------*/
            /* Get USB Receive Data count       */
            /*----------------------------------*/
            data_cnt = usb_pcdc_get_rcv_data_cnt(mess);

            /*----------------------------------*/
            /* CDC Host Bulk In receive check   */
            /*----------------------------------*/
            if(data_cnt > 0)
            {
                /* Communications Port Data Send Process */
                usb_cpu_Sci_DataSend(usb_gpcdc_echo_mode, &usb_gpcdc_receive_data , data_cnt);
            }
            /*----------------------------------*/
            /* CDC Host Bulk In Request         */
            /*----------------------------------*/
            if( usb_cpu_Sci_StxBuffStatus() == 0)
            {
                /* Common Read(BULK OUT) */
                R_usb_pcdc_ReceiveData(mess, (uint8_t *)&usb_gpcdc_receive_data, USB_PCDC_APL_RX_SIZE,
                    (USB_CB_t)&usb_pcdc_rx_notification);
            }
            else
            {
                /* Receive flow control flag on */
                usb_gpcdc_stx_wait = USB_ON;
            }
            break;

        case USB_PCDC_TX_COMP:
            /*----------------------------------*/
            /* Data send for Host cdc           */
            /*----------------------------------*/
            /* Send Complete wait flag off */
            usb_gpcdc_tx_wait_flag = USB_OFF;
            break;

        case USB_PCDC_STATUS_TX_COMP:
            /*------------------------------------------*/
            /* Serial State Notification send complete  */
            /*------------------------------------------*/
            /* Serial State Notification send flag clear */
            usb_gpcdc_serial_state_tx = USB_OFF;
            break;

        case USB_PCDC_CLOSE:
            /* Application data Initialize and Periodic process request */
            usb_pcdc_apl_init(mess);

            /* Release the memory block for the sample application */
            err = R_USB_REL_BLK(USB_PCDCSMP_MPL,(USB_MH_t)mess);
            if( err != USB_OK )
            {
                /* error */
                USB_PRINTF0("### USB PeripheralSampleClass rel_blk error\n");
            }
            break;

        default:
            /*----------------------------------*/
            /* Undefine message                 */
            /*----------------------------------*/
            USB_PRINTF1("### Undefine message:%02x\n",mess->msginfo);

            /* Release the memory block for the sample application */
            err = R_USB_REL_BLK(USB_PCDCSMP_MPL,(USB_MH_t)mess);
            if( err != USB_OK )
            {
                /* error */
                USB_PRINTF0("### USB PeripheralSampleClass rel_blk error\n");
            }
            break;
        }
    }
}   /* eof usb_pcdc_main_task() */

/******************************************************************************
Function Name   : usb_pcdc_get_rcv_data_cnt
Description     : Get USB Receive Data count 
Arguments       : USBC_UTR_t
Return value    : USB receive data count
******************************************************************************/
uint16_t usb_pcdc_get_rcv_data_cnt(USB_UTR_t *mess)
{
    /* Receive Data Count for USB Host */
    uint16_t    rcv_cnt;

    /*----------------------------------*/
    /* Data receive from Host cdc       */
    /*----------------------------------*/
    if(mess->status == USB_DATA_OK)
    {
        /* Receive size = Request size */
        rcv_cnt = USB_PCDC_APL_RX_SIZE;
    }
    else if(mess->status == USB_DATA_SHT)
    {
        /* Receive size < Request size */
        rcv_cnt = USB_PCDC_APL_RX_SIZE - mess->tranlen;
    }
    else
    {
        rcv_cnt = 0;
    }

    return rcv_cnt;
}   /* eof usb_pcdc_get_rcv_data_cnt() */

#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP   */

/******************************************************************************
Function Name   : usb_pcdc_rx_notification
Description     : CDC Host Rx complete Callback
Argument        : USB_UTR_t *mess           : message
Return          : none
******************************************************************************/
void usb_pcdc_rx_notification(USB_UTR_t *mess)
{
    USB_ER_t        err;

    /* Set USB receive complete for APL notification message. */
    mess->msginfo = USB_PCDC_RX_COMP;
    /* Send message */
    err = R_USB_SND_MSG(USB_PCDCSMP_MBX, (USB_MSG_t*)mess);
    if( err != USB_E_OK )
    {
        /* Send Message failure */
        USB_PRINTF1("### usb_pcdc_rx_notification snd_msg error (%ld)\n", err);
    }
}   /* eof usb_pcdc_rx_notification() */

/******************************************************************************
Function Name   : usb_pcdc_tx_notification
Description     : CDC Host Tx complete Callback
Argument        : USB_UTR_t *mess           : message
Return          : none
******************************************************************************/
void usb_pcdc_tx_notification(USB_UTR_t *mess)
{
    USB_ER_t        err;

    /* Set USB transfer complete for APL notification message. */
    mess->msginfo = USB_PCDC_TX_COMP;
    /* Send message */
    err = R_USB_SND_MSG(USB_PCDCSMP_MBX, (USB_MSG_t*)mess);
    if( err != USB_E_OK )
    {
        /* Send Message failure */
        USB_PRINTF1("### usb_pcdc_tx_notification snd_msg error (%ld)\n", err);
    }
}   /* eof usb_pcdc_tx_notification() */

/******************************************************************************
Function Name   : usb_pcdc_SerialStatusTxCB
Description     : CDC Host Tx complete Callback
Argument        : USB_UTR_t *mess           : message
Return          : none
******************************************************************************/
void usb_pcdc_state_notification(USB_UTR_t *mess)
{
    USB_ER_t        err;

    /* Set SerialState transfer complete for APL notification message type. */
    mess->msginfo = USB_PCDC_STATUS_TX_COMP;
    /* Send message */
    err = R_USB_SND_MSG(USB_PCDCSMP_MBX, (USB_MSG_t*)mess);
    if( err != USB_E_OK )
    {
        /* Send Message failure */
        USB_PRINTF1("### usb_pcdc_state_notification snd_msg error (%ld)\n", err);
    }
}   /* eof usb_pcdc_state_notification() */

/******************************************************************************
Function Name   : usb_pcdc_apl_init
Description     : Peri CDC Application memory clear
Argument        : USB_UTR_t *ptr        : USB system internal structure.
Return          : none
******************************************************************************/
void usb_pcdc_apl_init(USB_UTR_t *ptr)
{
    /* echo mode flag clear */
    usb_gpcdc_echo_mode = USB_OFF;

    /* Clear SW2 push flag */
    usb_gpcdc_sw2push = 0;

    /* Clear SW2 push request Message transfer timming counter */
    usb_gpcdc_msg_timing_cnt = 0;

    /* Peri CDC Not Active set */
    usb_gpcdc_active = USB_OFF;

    /* Clear data for USB TX and RX */
    memset(usb_gpcdc_send_data,0,USB_PCDC_APL_SRX_SIZE);
    memset(usb_gpcdc_receive_data,0,USB_PCDC_APL_RX_SIZE);
    /* Instructions output line sequence */
    usb_gpcdc_explain_seq = 0;
    /* HOST Send wait flag */
    usb_gpcdc_tx_wait_flag = 0;

    /* Peri CDC Application Tx Rx data clear */
    usb_cpu_Sci_Buffer_init();

#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    /*  Cdc Application Initial Message send */
    usb_pcdc_smpl_message_send( ptr, USB_PCDC_OPEN );
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP */
}   /* eof usb_pcdc_apl_init() */

/******************************************************************************
Function Name   : usb_pcdc_apl_msg_out
Description     : Terminal Output message send for Host CDC
Argument        : USB_UTR_t *ptr        : USB system internal structure.
                : uint16_t message_no   : Terminal Output message No.
Return          : none
******************************************************************************/
void usb_pcdc_apl_msg_out(USB_UTR_t *ptr, uint16_t message_no)
{
    uint16_t len;

    /* Message length get */
    len = strlen((char*)szWelcomeMsg[message_no]);
    /* Message send for Host CDC */
#if USB_ANSIIO_PP == USB_ANSIIO_USE_PP
    write( usb_spcdc_data_fn, (uint8_t*)&szWelcomeMsg[message_no], len);
#else   /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP   */
    R_usb_pcdc_SendData(ptr, (uint8_t *)&szWelcomeMsg[message_no], len, (USB_CB_INFO_t)&usb_pcdc_tx_notification);
#endif  /* USB_ANSIIO_PP == USB_ANSIIO_USE_PP   */

    /* Host CDC Send complete wait flag on */
    usb_gpcdc_tx_wait_flag = USB_ON;
}   /* eof usb_pcdc_apl_msg_out() */

/******************************************************************************
Function Name   : usb_pcdc_smpl_message_send
Description     : Transfer message for Message BOX of demo sample application
Argument        : USB_UTR_t *ptr            : IP Information(Mode,IP No.,Reg Address)
                : uint16_t   msginfo         : Message Information Kind
Return          : none
******************************************************************************/
void usb_pcdc_smpl_message_send( USB_UTR_t *ptr, uint16_t msginfo)
{
    USB_MH_t            p_blf;
    USB_ER_t            err;
    USB_CLSINFO_t       *cp;

    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PCDCSMP_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_CLSINFO_t*)p_blf;
        cp->msginfo = msginfo;

        cp->ipp = ptr->ipp;
        cp->ip  = ptr->ip;
        cp->msghead = (USB_MH_t)NULL;
        
        /* Send message */
        err = R_USB_SND_MSG( USB_PCDCSMP_MBX, (USB_MSG_t*)cp );
        if( err != USB_E_OK )
        {
            /* error */
            err = R_USB_REL_BLK(USB_PCDCSMP_MPL,(USB_MH_t)p_blf);
            USB_PRINTF0("### usb_pcdc_smpl_message_send function snd_msg error\n");
        }
    }
    else
    {
        /* error */
        USB_PRINTF0("### usb_pcdc_smpl_message_send function pget_blk error\n");
        while( 1 );
    }
}   /* eof usb_pcdc_smpl_message_send() */

/******************************************************************************
Function Name   : usb_pcdc_open
Description     : Peripheral Communications Devices Class open function
Arguments       : USB_UTR_t *ptr            : IP Information(Mode,IP No.,Reg Address)
                : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : USB_ER_t                  : USB_E_OK etc
******************************************************************************/
USB_ER_t usb_pcdc_open(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    /* Peri CDC Application process enable */
    usb_gcdc_connected = USB_ON;

#if USB_ANSIIO_PP != USB_ANSIIO_USE_PP
    /* Sample class attach */
    usb_pcdc_smpl_message_send( ptr, USB_PCDC_PERIODIC_PROCESS );
#endif  /* USB_ANSIIO_PP != USB_ANSIIO_USE_PP */

    return USB_E_OK;
}   /* eof usb_pcdc_open() */


/******************************************************************************
Function Name   : usb_pcdc_close
Description     : Peripheral Communications Devices Class close function
Arguments       : USB_UTR_t *ptr            : IP Information(Mode,IP No.,Reg Address)
                : uint16_t data1            : Not use
                : uint16_t data2            : Not use
Return value    : USB_ER_t                  : USB_E_OK etc
******************************************************************************/
USB_ER_t usb_pcdc_close(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    /* Sample class detatch */
    usb_pcdc_smpl_message_send( ptr, USB_PCDC_CLOSE );

    /* Peri CDC Application process disable */
    usb_gcdc_connected = USB_OFF;

    return USB_E_OK;
}   /* eof usb_pcdc_close() */

/******************************************************************************
Function Name   : usb_pcdc_is_connected
Description     : Get the USB cable connected state.
Arguments       : none
Return value    : USB_ON = Connected, USB_OFF = Disconnected.
******************************************************************************/
uint16_t usb_pcdc_is_connected(void)
{
    return usb_gcdc_connected;
}   /* eof usb_pcdc_is_connected() */

/******************************************************************************
End  Of File
******************************************************************************/

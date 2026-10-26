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
* File Name    : r_usb_vendor_hapl.c
* Version      : 2.01
* Description  : USB Host vendor sample application code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 31.03.2012 1.80    Basic FW mini Ver2.00(ES Version)
*           : 30.11.2012 2.00    Basic FW mini Ver2.00 Release
*           : 31.01.2013 2.01    Corresponded to RL78/L1C
*******************************************************************************/

/******************************************************************************
Section definitions
******************************************************************************/
#ifdef  R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#pragma section rom C_smpl
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
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_usbip.h"                /* Definition of the USB register & bit define */
#include "r_usb_api.h"                  /* USB API public function header */
#include "r_usb_vendor_api.h"
#include "r_usb_vendor_apl.h"
#include "hw_resource.h"


/******************************************************************************
Macro definitions
******************************************************************************/
/* There is no macro definition. */


/******************************************************************************
Typedef definitions
******************************************************************************/
/* There is no typedef definition. */


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */
USB_STATIC usb_tskinfo_t    g_usb_hsmpl_Message;
USB_STATIC uint16_t         g_usb_hsmpl_Sequence;
USB_STATIC usb_addr_t       g_usb_hsmpl_DevAddr;
USB_STATIC usb_port_t       g_usb_hsmpl_DevPort;
USB_STATIC uint8_t          g_usb_hsmpl_UsePipe[USB_MAX_PIPE_NO+1];

/* functions */
USB_STATIC void         usb_hsmpl_driver_registration(void);
USB_STATIC void         usb_hsmpl_class_check(uint8_t **table);
USB_STATIC void         usb_hsmpl_device_state(uint16_t data, uint16_t state);
USB_STATIC void         usb_hsmpl_application(usb_utr_t *mess);
USB_STATIC void         usb_hsmpl_transfer_start(usb_pipe_t pipe);
USB_STATIC void         usb_hsmpl_transfer_result(usb_utr_t *mess);
USB_STATIC void         usb_hsmpl_task_operate(usb_strct_t msginfo);
USB_STATIC void         usb_hsmpl_keysequence(usb_tskinfo_t *mess);
USB_STATIC void         usb_hsmpl_status_result(uint16_t data1, uint16_t data2);
USB_STATIC void         usb_hsmpl_tranfer_all(void);
USB_STATIC void         usb_hsmpl_transfer_end_all(void);


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */
uint32_t    totalcount1;
uint32_t    totalcount2;

/* functions */
void        usb_hsmpl_main_init(void);
void        usb_hsmpl_keyprocess(uint8_t keydata);
void        usb_hsmpl_apl_task(void);


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */
extern usb_utr_t    g_usb_SmplTrnMsg[];
extern usb_leng_t   g_usb_SmplTrnSize[];
extern uint16_t     g_usb_SmplTrnCnt[];
extern uint8_t      *g_usb_SmplTrnPtr[];

/* functions */
extern void         usb_smpl_data_init(void);
extern void         usb_smpl_counter_init(void);
extern void         usb_smpl_lcd_pipe_count(usb_pipe_t pipe);
extern void         usb_smpl_lcd_numeric(uint8_t pos, uint8_t data);
extern void         usb_smpl_lcd_string(uint8_t pos, uint8_t *str);



/******************************************************************************
Renesas Host Sample Code functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_hsmpl_main_init
Description     : Initialize process
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hsmpl_main_init(void)
{
    /* Target board initialize */
    usb_cpu_target_init();

    /* USB-IP initialized */
    R_usb_hstd_ChangeDeviceState(USB_DO_INITHWFUNCTION);

    /* HCD driver open & registratuion */
    R_usb_hstd_HcdOpen();                               /* HCD task, MGR task open */
    usb_hsmpl_driver_registration();                    /* Sample driver registration */

    /* Scheduler initialized */
    R_usb_hstd_ChangeDeviceState(USB_DO_SETHWFUNCTION);
}   /* eof usb_hsmpl_main_init() */


/******************************************************************************
Function Name   : usb_hsmpl_apl_task
Description     : Host Class Sample Task
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hsmpl_apl_task(void)
{
    usb_tskinfo_t   *mess;
    uint16_t        information[8];

    /* Receive message */
    if (R_USB_RCV_MSG(USB_HSMP_MBX, (usb_msg_t**)&mess) != USB_E_OK)
    {
        R_usb_hstd_DeviceInformation(g_usb_hsmpl_DevAddr, information);
        if (information[1] == USB_STS_DETACH)
        {
        }
        else
        {
        }
        return;
    }

    switch( mess->msginfo )
    {
        case USB_SMPL_TASK:
            usb_hsmpl_application((usb_utr_t *)mess);
        break;
        case USB_SMPL_KEYPROCESS:
            usb_hsmpl_keysequence((usb_tskinfo_t*)mess);
        break;
        default:
        break;
    }
}   /* eof usb_hsmpl_apl_task() */


/******************************************************************************
Function Name   : usb_hsmpl_keyprocess
Description     : Host Sample Key data processing
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hsmpl_keyprocess(uint8_t keydata)
{

    switch(keydata)
    {
        case 1:         /* Suspend */
            if (g_usb_hsmpl_Sequence >= USB_SEQ_10)
            {
                return;
            }
            usb_smpl_lcd_string(8, (uint8_t *)"TranStop");
            g_usb_hsmpl_Sequence        = USB_SEQ_10;
        break;
        case 2:         /* Resume */
            if (g_usb_hsmpl_Sequence != USB_SEQ_15)
            {
                return;
            }
            g_usb_hsmpl_Sequence        = USB_SEQ_20;
            usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
        break;
        case 3:         /* VBUS control */
            g_usb_hsmpl_Sequence    = USB_SEQ_30;
            usb_smpl_lcd_string(8, (uint8_t *)"TranStop");
            usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
        break;
        case 4:
        break;
        default:
        break;
    }
}   /* eof usb_hsmpl_keyprocess() */


/******************************************************************************
Function Name   : usb_hsmpl_keysequence
Description     : Host Sample Key data processing
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_keysequence(usb_tskinfo_t *mess)
{
    uint16_t    information[8];
    usb_pipe_t  i;

    /* Manager Mode Change */
    switch( g_usb_hsmpl_Sequence )
    {
    case USB_SEQ_00:
    break;

    case USB_SEQ_10:
        for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
        {
            if (g_usb_hsmpl_UsePipe[i] == USB_YES)
            {
                return;
            }
        }
        usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
        g_usb_hsmpl_Sequence++;
        break;
    case USB_SEQ_11:
        usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
        g_usb_hsmpl_Sequence++;
        break;
    case USB_SEQ_12:
        usb_smpl_lcd_string(8, (uint8_t *)"TestWait");
        if (R_USB_WAI_MSG(USB_HSMP_MBX, (usb_msg_t*)&g_usb_hsmpl_Message, 35000) != USB_E_OK)
        {
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE1);
        }
        g_usb_hsmpl_Sequence++;
    break;
    case USB_SEQ_13:
        usb_smpl_lcd_string(8, (uint8_t *)"TstStart");
//      R_usb_hstd_MgrChangeDeviceState( usb_hsmpl_status_result, USB_DO_SELECTIVE_SUSPEND, g_usb_hsmpl_DevAddr );
        R_usb_hstd_DeviceInformation(g_usb_hsmpl_DevAddr, information);
        g_usb_hsmpl_DevPort = (usb_port_t)information[0];
        R_usb_hstd_MgrChangeDeviceState( usb_hsmpl_status_result, USB_DO_GLOBAL_SUSPEND, g_usb_hsmpl_DevPort );
    break;
    case USB_SEQ_14:
        usb_smpl_lcd_string(8, (uint8_t *)"Suspend ");
        g_usb_hsmpl_Sequence            = USB_SEQ_15;
    break;
    case USB_SEQ_15:
    break;

    case USB_SEQ_20:
        usb_smpl_lcd_string(8, (uint8_t *)" Resume ");
//      R_usb_hstd_MgrChangeDeviceState( usb_hsmpl_status_result, USB_DO_SELECTIVE_RESUME, g_usb_hsmpl_DevAddr );
        R_usb_hstd_MgrChangeDeviceState( usb_hsmpl_status_result, USB_DO_GLOBAL_RESUME, g_usb_hsmpl_DevPort );
    break;
    case USB_SEQ_21:
        if (R_USB_WAI_MSG(USB_HSMP_MBX, (usb_msg_t*)&g_usb_hsmpl_Message, 35000) != USB_E_OK)
        {
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE2);
        }
        g_usb_hsmpl_Sequence++;
    break;
    case USB_SEQ_22:
        usb_smpl_lcd_string(8, (uint8_t *)"        ");
        if (R_USB_WAI_MSG(USB_HSMP_MBX, (usb_msg_t*)&g_usb_hsmpl_Message, 35000) != USB_E_OK)
        {
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE3);
        }
        g_usb_hsmpl_Sequence++;
    break;
    case USB_SEQ_23:
        usb_hsmpl_tranfer_all();
        g_usb_hsmpl_Sequence        = USB_SEQ_00;
        g_usb_hsmpl_DevPort         = USB_NO;
    break;

    case USB_SEQ_30:
        usb_smpl_lcd_string(8, (uint8_t *)"TranStop");
        for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
        {
            if (g_usb_hsmpl_UsePipe[i] == USB_YES)
            {
                usb_hsmpl_transfer_end_all();
                return;
            }
        }
        g_usb_hsmpl_Sequence++;
        usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
    break;

    case USB_SEQ_31:
        usb_smpl_lcd_string(8, (uint8_t *)"VBUS off");
        R_usb_hstd_DeviceInformation(g_usb_hsmpl_DevAddr, information);
        g_usb_hsmpl_DevPort = (usb_port_t)information[0];
        R_usb_hstd_MgrChangeDeviceState( usb_hsmpl_status_result, USB_DO_PORT_DISABLE, g_usb_hsmpl_DevPort );
    break;
    case USB_SEQ_32:
        usb_smpl_lcd_string(8, (uint8_t *)" detach ");
        g_usb_hsmpl_Sequence            = USB_SEQ_00;
    break;

    default:
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE4);
    break;
    }
}   /* eof usb_hsmpl_keysequence() */


/******************************************************************************
Function Name   : usb_hsmpl_application
Description     : Sample Main Task Initialize
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_application(usb_utr_t *mess)      /* nonOS */
{
    usb_pipe_t  i;

    switch( mess->status )
    {
        case USB_DATA_OK:               /* IN (data receive), OUT (data transmission) */
        case USB_DATA_SHT:              /* IN (data receive) */
        case USB_DATA_OVR:              /* IN (data receive) */
            if (g_usb_SmplTrnCnt[mess->pipenum])
            {
                g_usb_SmplTrnCnt[mess->pipenum]--;
                if (g_usb_SmplTrnCnt[mess->pipenum])
                {
                    if (g_usb_hsmpl_Sequence < USB_SEQ_10)
                    {
                        /* next data transfer start */
                        usb_hsmpl_transfer_start( mess->pipenum );
                    }
                    else
                    {
                        g_usb_hsmpl_UsePipe[mess->pipenum]  = USB_NO;
                        usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
                    }
                }
                else
                {
                    g_usb_hsmpl_UsePipe[mess->pipenum]  = USB_NO;
                }
            }
        break;
        case USB_DATA_STALL:        /* STALL */
            /* Clear STALL is necessary. */
        break;
        case USB_DATA_ERR:          /* transfer error */
        break;
        case USB_DATA_DTCH:         /* detach */
        break;
        case USB_DATA_STOP:
            /* data transfer end */
        break;
        default:
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE5);
        break;
    }

    totalcount1 = 0;
    for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
    {
        totalcount1 |= g_usb_SmplTrnCnt[i];
    }
    if (totalcount1 == 0)
    {
        totalcount2++;
        if (g_usb_hsmpl_Sequence < USB_SEQ_10)
        {
            /* Couner Initialized */
            usb_smpl_counter_init();
            usb_hsmpl_tranfer_all();
            usb_smpl_lcd_numeric(LCD_POS_U6, (uint8_t)totalcount2);
        }
    }
}   /* eof usb_hsmpl_application() */


/******************************************************************************
Function Name   : usb_hsmpl_driver_registration
Description     : Host Sample Driver Registration
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_driver_registration(void)
{
    usb_hcdreg_t    driver;
    usb_pipe_t      i;

    for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
    {
        g_usb_hsmpl_UsePipe[i]      = USB_NO;
    }

    /* Driver registration */
    driver.ifclass      = USB_IFCLS_VEN;            /* Vendor class */
    driver.classcheck   = &usb_hsmpl_class_check;
    driver.statediagram = &usb_hsmpl_device_state;
    R_usb_hstd_DriverRegistration(&driver);
    R_usb_hvndr_DriverStart();
}   /* eof usb_hsmpl_driver_registration() */


/******************************************************************************
Function Name   : usb_hsmpl_class_check
Description     : Class Check
Arguments       : uint16_t **table          : Data Table Pointer
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_class_check(uint8_t **table)
{
    R_usb_hvndr_ClassCheck(table);
    g_usb_hsmpl_DevAddr = (usb_addr_t)(*table[2]);
}   /* eof usb_hsmpl_class_check() */


/******************************************************************************
Function Name   : usb_hsmpl_device_state
Description     : Open / Close
Arguments       : uint16_t data             : Device address
                : uint16_t state            : Device state
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_device_state(uint16_t data, uint16_t state)
{
    R_usb_hvndr_StateCallback(data, state);
    switch( state )
    {
        case USB_STS_DETACH:
            if (g_usb_hsmpl_Sequence == USB_SEQ_00)
            {
            }
            else if (g_usb_hsmpl_Sequence < USB_SEQ_10)
            {
                /* NG responds to MGR when enumerating. */
                g_usb_hsmpl_Sequence    = USB_SEQ_00;
            }
            else
            {
                /* The sequence is being processed by the key operation.  */
                g_usb_hsmpl_Sequence    = USB_SEQ_00;
            }
            usb_hsmpl_transfer_end_all();
#if USB_PORTSEL_PP == USB_1PORT_PP
            usb_smpl_lcd_string(0, (uint8_t *)"DETACH 1");
#endif  /* USB_PORTSEL_PP == USB_1PORT_PP */
#if USB_PORTSEL_PP == USB_2PORT_PP
            usb_smpl_lcd_string(0, (uint8_t *)"DETACH 2");
#endif  /* USB_PORTSEL_PP == USB_2PORT_PP */
        break;
        case USB_STS_DEFAULT:
            usb_smpl_lcd_string(0, (uint8_t *)"DEFAULT ");
            usb_smpl_lcd_string(8, (uint8_t *)"        ");
        break;
        case USB_STS_ADDRESS:
        break;
        case USB_STS_CONFIGURED:
            totalcount1 = 0;
            totalcount2 = 0;
            if (data == g_usb_hsmpl_DevAddr)
            {
                /* device address set */
                R_usb_hvndr_PipeRegistration();
                usb_smpl_data_init();
                g_usb_hsmpl_DevPort = USB_NO;
                usb_hsmpl_tranfer_all();
            }
            usb_smpl_lcd_string(0, (uint8_t *)"CONFIG  ");
        break;
        case USB_STS_SUSPEND:
            usb_smpl_lcd_string(0, (uint8_t *)"SUSPEND ");
        break;
        case USB_STS_RESUME:
            usb_smpl_data_init();
            usb_smpl_lcd_string(0, (uint8_t *)"RESUME  ");
        break;
        case USB_STS_WAKEUP:
            if (data == g_usb_hsmpl_DevAddr)
            {
                g_usb_hsmpl_Sequence        = USB_SEQ_21;
                usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
            }
            usb_smpl_lcd_string(8, (uint8_t *)"REMOTE  ");
        break;
        default:
        break;
    }
}   /* eof usb_hsmpl_device_state() */


/******************************************************************************
Function Name   : usb_hsmpl_transfer_start
Description     : Data Transfer
Arguments       : uint16_t pipe             : Pipe No
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_transfer_start(usb_pipe_t pipe)
{
    /* PIPE Transfer set */
    g_usb_SmplTrnMsg[pipe].pipenum  = pipe;
    g_usb_SmplTrnMsg[pipe].tranadr  = g_usb_SmplTrnPtr[pipe];
    g_usb_SmplTrnMsg[pipe].tranlen  = g_usb_SmplTrnSize[pipe];
    g_usb_SmplTrnMsg[pipe].setup    = (uint16_t*)USB_NULL;
    g_usb_SmplTrnMsg[pipe].complete = (usb_cb_t)&usb_hsmpl_transfer_result;
    R_usb_hvndr_TransferStart((usb_utr_t*)&g_usb_SmplTrnMsg[pipe]);
    g_usb_hsmpl_UsePipe[pipe]       = USB_YES;
    usb_smpl_lcd_pipe_count(pipe);
}   /* eof usb_hsmpl_transfer_start() */


/******************************************************************************
Function Name   : usb_hsmpl_transfer_result
Description     : Data Transfer Callback Function 
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_transfer_result(usb_utr_t *mess)
{
    usb_er_t    err;

    mess->msginfo = USB_SMPL_TASK;

    err = R_USB_SND_MSG(USB_HSMP_MBX, (usb_msg_t*)mess);
    if (err != USB_E_OK)
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE6);
    }
}   /* eof usb_hsmpl_transfer_result() */


/******************************************************************************
Function Name   : usb_hsmpl_tranfer_all
Description     : User Data Trasfer Start
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_tranfer_all(void)
{
    if (g_usb_SmplTrnCnt[USB_PIPE4])
    {
        usb_hsmpl_transfer_start(USB_PIPE4);
    }
    if (g_usb_SmplTrnCnt[USB_PIPE5])
    {
        usb_hsmpl_transfer_start(USB_PIPE5);
    }
    usb_hsmpl_transfer_start(USB_PIPE6);
    usb_hsmpl_transfer_start(USB_PIPE7);
}   /* eof usb_hsmpl_tranfer_all() */


/******************************************************************************
Function Name   : usb_hsmpl_transfer_end_all
Description     : ALL pipe transfer end
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_transfer_end_all(void)
{
    usb_pipe_t      i;

    for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
    {
        g_usb_hsmpl_UsePipe[i]      = USB_NO;
        R_usb_hvndr_TransferStop(i);
    }
}   /* eof usb_hsmpl_transfer_end_all() */


/******************************************************************************
Function Name   : usb_hsmpl_task_operate
Description     : 
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_task_operate(usb_strct_t msginfo)
{
    /* Class check of enumeration sequence move to class function */
    g_usb_hsmpl_Message.msginfo = msginfo;
    if (R_USB_SND_MSG(USB_HSMP_MBX, (usb_msg_t*)&g_usb_hsmpl_Message) != USB_E_OK)
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE7);
    }
}   /* eof usb_hsmpl_task_operate() */


/******************************************************************************
Function Name   : usb_hsmpl_status_result
Description     : 
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_hsmpl_status_result(uint16_t data1, uint16_t data2)
{
    g_usb_hsmpl_Sequence++;
    switch(data2)
    {
    case    USB_STS_SUSPEND:            /* suspend */
        usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
    break;
    case    USB_STS_RESUME:             /* resume */
        usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
    break;
    case    USB_STS_POWER:              /* port enable */
    case    USB_STS_PORTOFF:            /* port disable */
        usb_hsmpl_task_operate(USB_SMPL_KEYPROCESS);
    break;

    default:
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE8);
    break;
    }
}   /* eof usb_hsmpl_status_result() */


/******************************************************************************
End of file
******************************************************************************/

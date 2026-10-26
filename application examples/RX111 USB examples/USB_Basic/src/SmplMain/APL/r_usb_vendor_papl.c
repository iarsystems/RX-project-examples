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
* File Name    : r_usb_vendor_papl.c
* Version      : 2.01
* Description  : USB Peripheral vendor sample application code.
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
USB_STATIC usb_tskinfo_t    g_usb_psmpl_Message;                        /* Pcd Task recive message */

/* functions */
USB_STATIC void         usb_psmpl_keysequence(usb_tskinfo_t *mess);
USB_STATIC void         usb_psmpl_application(usb_utr_t *mess);
USB_STATIC void         usb_psmpl_driver_registration(void);
USB_STATIC void         usb_psmpl_device_state(uint16_t data, uint16_t state);
USB_STATIC void         usb_psmpl_control_transfer(usb_request_t *data1, uint16_t data2);
USB_STATIC void         usb_psmpl_transfer_start(usb_pipe_t Pipe);
USB_STATIC void         usb_psmpl_transfer_result(usb_utr_t *mess);
USB_STATIC void         usb_psmpl_tranfer_all(void);
USB_STATIC void         usb_psmpl_transfer_end_all(void);


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */
uint32_t    totalcount1;
uint32_t    totalcount2;

/* functions */
void        usb_psmpl_main_init(void);
void        usb_psmpl_apl_task(void);
void        usb_psmpl_keyprocess(uint8_t keydata);


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */
extern uint8_t      g_usb_psmpl_DeviceDescriptor[];
extern uint8_t      g_usb_psmpl_ConfigurationF_1[];
extern uint8_t      *g_usb_psmpl_StringPtr[];
extern uint16_t     g_usb_psmpl_EpTbl1[];
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
extern void         usb_smpl_set_suspend_flag(uint8_t data);

extern void        usb_cpu_deep_sleep_mode(void);
extern void        usb_cpu_soft_stanby_mode(void);


/******************************************************************************
Renesas Peripheral Sample Code functions
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
    R_usb_pstd_PcdChangeDeviceState(USB_DO_INITHWFUNCTION);

    /* PCD driver open & registratuion */
    R_usb_pstd_PcdOpen();                               /* PCD task open */
    usb_psmpl_driver_registration();                    /* Sample driver registration */

    /* Scheduler initialized */
    R_usb_pstd_PcdChangeDeviceState(USB_DO_SETHWFUNCTION);

}   /* eof usb_psmpl_main_init() */


/******************************************************************************
Function Name   : usb_psmpl_apl_task
Description     : Main Task
Arguments       : none
Return value    : none
******************************************************************************/
void usb_psmpl_apl_task(void)
{
    usb_tskinfo_t   *mess;

    /* Receive message */
    if (R_USB_RCV_MSG(USB_PSMP_MBX, (usb_msg_t**)&mess) != USB_E_OK)
    {
        return;
    }

    switch( mess->msginfo )
    {
        case USB_SMPL_TASK:
            usb_psmpl_application((usb_utr_t *)mess);
        break;
        case USB_SMPL_KEYPROCESS:
            usb_psmpl_keysequence((usb_tskinfo_t*)mess);
        break;
        default:
        break;
    }
}   /* eof usb_psmpl_apl_task() */


/******************************************************************************
Function Name   : usb_psmpl_keyprocess
Description     : Host Sample Key data processing
Arguments       : none
Return value    : none
******************************************************************************/
void usb_psmpl_keyprocess(uint8_t keydata)
{
    g_usb_psmpl_Message.msginfo = USB_SMPL_KEYPROCESS;
    g_usb_psmpl_Message.keyword = (usb_strct_t)keydata;
    if (R_USB_SND_MSG(USB_PSMP_MBX, (usb_msg_t*)&g_usb_psmpl_Message) != USB_E_OK)
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE9);
    }
}   /* eof usb_psmpl_keyprocess() */


/******************************************************************************
Function Name   : usb_psmpl_keysequence
Description     : Peri Sample Key data processing
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_keysequence(usb_tskinfo_t *mess)
{
    uint16_t table[4];

    /* Manager Mode Change */
    switch(mess->keyword)
    {
        case 0:
        break;
        case 1:     /* remote wakeup */
            R_usb_pstd_DeviceInformation(table);
            if (table[3] == USB_YES)
            {
                R_usb_pstd_PcdChangeDeviceState(USB_DO_REMOTEWAKEUP);
                usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"WAKEUP  ");
            }
        break;
        case 2:     /* port disable */
            R_usb_pstd_PcdChangeDeviceState(USB_DO_PORT_DISABLE);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"PORT OFF");
        break;
        case 3:     /* port enable */
            R_usb_pstd_PcdChangeDeviceState(USB_DO_PORT_ENABLE);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"PORT ON ");
        break;
        case 4:     /* Forced ending */
            usb_psmpl_transfer_end_all();
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"TransEnd");
        break;

        default:
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE10);
        break;
    }
}   /* eof usb_psmpl_keysequence() */


/******************************************************************************
Function Name   : usb_psmpl_application
Description     : Sample Main Task Initialize
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_application(usb_utr_t *mess)      /* nonOS */
{
    //usb_pipe_t  i;

    switch( mess->status )
    {
        case USB_DATA_OK:               /* IN (data receive), OUT (data transmission) */
        case USB_DATA_SHT:              /* IN (data receive) */
        case USB_DATA_OVR:              /* IN (data receive) */
#if	0
            if (g_usb_SmplTrnCnt[mess->pipenum])
            {
                g_usb_SmplTrnCnt[mess->pipenum]--;
                if (g_usb_SmplTrnCnt[mess->pipenum])
                {
                    /* next data transfer start */
                    usb_psmpl_transfer_start( mess->pipenum );
                }
            }
#else
    	    usb_psmpl_transfer_start( mess->pipenum );
#endif
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
            USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE11);
        break;
    }

#if	0
    totalcount1 = 0;
    for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
    {
        totalcount1 |= g_usb_SmplTrnCnt[i];
    }
    if (totalcount1 == 0)
    {
        totalcount2++;
        if (totalcount2 > 99)
        {
            totalcount2 = 0;
        }
        usb_smpl_counter_init();
        usb_psmpl_tranfer_all();
        usb_smpl_lcd_numeric(LCD_POS_U6, (uint8_t)totalcount2);
    }
#endif
}   /* eof usb_psmpl_application() */


/******************************************************************************
Function Name   : usb_psmpl_driver_registration
Description     : Peripheral Sample Driver Registration
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_driver_registration(void)
{
    usb_pcdreg_t    driver;

    /* Driver registration */
    driver.pipetbl      = g_usb_psmpl_EpTbl1;
    driver.devicetbl    = g_usb_psmpl_DeviceDescriptor;
    driver.configtbl    = g_usb_psmpl_ConfigurationF_1;
    driver.stringtbl    = g_usb_psmpl_StringPtr;
    driver.statediagram = &usb_psmpl_device_state;
    driver.ctrltrans    = &usb_psmpl_control_transfer;
    R_usb_pstd_DriverRegistration(&driver);
    R_usb_pvndr_DriverStart();
}   /* eof usb_psmpl_driver_registration() */


/******************************************************************************
Function Name   : usb_psmpl_device_state
Description     : Device State CallBack Check
Arguments       : uint16_t data
                : uint16_t state            : Device state
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_device_state(uint16_t data, uint16_t state)
{
//    uint16_t        info;
//    uint16_t        information[4];

    R_usb_pvndr_StateCallback(data, state);
    switch( state )
    {
        case USB_STS_DETACH:
            usb_psmpl_transfer_end_all();
            usb_smpl_set_suspend_flag(USB_NO);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"DETACH P");
#ifdef USB_STOP_MODE_ENABLE
  #ifdef __RX
            usb_cpu_soft_stanby_mode();
  #endif  /* __RX */
#endif /* USB_STOP_MODE_ENABLE */
        break;
        case USB_STS_DEFAULT:
            usb_psmpl_transfer_end_all();
            usb_smpl_set_suspend_flag(USB_NO);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"DEFAULT ");
            usb_smpl_lcd_string(8, (uint8_t *)"        ");
        break;
        case USB_STS_ADDRESS:
        break;
        case USB_STS_CONFIGURED:
            totalcount1 = 0;
            totalcount2 = 0;
            usb_smpl_data_init();
            usb_psmpl_tranfer_all();
            usb_smpl_set_suspend_flag(USB_NO);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"CONFIG  ");
        break;
        case USB_STS_SUSPEND:
            usb_smpl_set_suspend_flag(USB_YES);
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"SUSPEND ");
        break;
        case USB_STS_RESUME:
            usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"RESUME  ");
            usb_smpl_set_suspend_flag(USB_NO);
        break;
        default:
        break;
    }
}   /* eof usb_psmpl_device_state() */


/******************************************************************************
Function Name   : usb_psmpl_control_transfer
Description     :
Arguments       : usb_request_t *data1
                : uint16_t      data2       : Not Use
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_control_transfer(usb_request_t *request, uint16_t data)
{
    R_usb_pvndr_ControlCallback(request, data);
}   /* eof usb_psmpl_control_transfer() */


/******************************************************************************
Function Name   : usb_psmpl_transfer_start
Description     : Data Transfer
Arguments       : uint16_t pipe             : Pipe No
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_transfer_start(usb_pipe_t pipe)
{
    /* PIPE Transfer set */
    g_usb_SmplTrnMsg[pipe].pipenum  = pipe;
    g_usb_SmplTrnMsg[pipe].tranadr  = g_usb_SmplTrnPtr[pipe];
    g_usb_SmplTrnMsg[pipe].tranlen  = g_usb_SmplTrnSize[pipe];
    g_usb_SmplTrnMsg[pipe].setup    = (uint16_t*)USB_NULL;
    g_usb_SmplTrnMsg[pipe].complete = (usb_cb_t)&usb_psmpl_transfer_result;
    R_usb_pvndr_TransferStart((usb_utr_t*)&g_usb_SmplTrnMsg[pipe]);
    usb_smpl_lcd_pipe_count(pipe);
}   /* eof usb_psmpl_transfer_start() */


/******************************************************************************
Function Name   : usb_psmpl_transfer_result
Description     : Data Transfer Callback Function
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_transfer_result(usb_utr_t *mess)
{
    usb_er_t    err;

    mess->msginfo = USB_SMPL_TASK;

    err = R_USB_SND_MSG(USB_PSMP_MBX, (usb_msg_t*)mess);
    if (err != USB_E_OK)
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE12);
    }
}   /* eof usb_psmpl_transfer_result() */


/******************************************************************************
Function Name   : usb_psmpl_tranfer_all
Description     : User Data Trasfer Start
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_tranfer_all(void)
{
#ifndef  USB_LSPERI_PP
    usb_psmpl_transfer_start(USB_PIPE4);
    usb_psmpl_transfer_start(USB_PIPE5);
#endif  /* USB_LSPERI_PP */
    usb_psmpl_transfer_start(USB_PIPE6);
    usb_psmpl_transfer_start(USB_PIPE7);
}   /* eof usb_psmpl_tranfer_all() */


/******************************************************************************
Function Name   : usb_psmpl_transfer_end_all
Description     : ALL pipe transfer end
Arguments       : none
Return value    : none
******************************************************************************/
USB_STATIC void usb_psmpl_transfer_end_all(void)
{
#ifndef  USB_LSPERI_PP
    R_usb_pvndr_TransferStop(USB_PIPE4);
    R_usb_pvndr_TransferStop(USB_PIPE5);
#endif  /* USB_LSPERI_PP */
    R_usb_pvndr_TransferStop(USB_PIPE6);
    R_usb_pvndr_TransferStop(USB_PIPE7);
}   /* eof usb_psmpl_transfer_end_all() */


/******************************************************************************
End of file
******************************************************************************/

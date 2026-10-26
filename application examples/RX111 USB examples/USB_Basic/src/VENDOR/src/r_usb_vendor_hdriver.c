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
* File Name    : r_usb_vendor_hdriver.c
* Version      : 2.01
* Description  : USB Host vendor class driver function code.
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


/******************************************************************************
Macro definitions
******************************************************************************/
#define USB_SMPL_DATASIZE               512
#define USB_VENDORID                (0x0000u)       /* Vendor  ID */
#define USB_PRODUCTID               (0x00FFu)       /* Product ID */


/******************************************************************************
Typedef definitions
******************************************************************************/
/* There is no typedef definition. */


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */

/* functions */


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */
uint16_t            g_usb_hvndr_Sequence;
uint8_t             *g_usb_hvndr_DevTbl;
uint8_t             *g_usb_hvndr_CnfgTbl;
uint8_t             *g_usb_hvndr_InterfaceTable;
usb_addr_t          g_usb_hvndr_DevAddr;
usb_port_t          g_usb_hvndr_DevPort;
uint16_t            g_usb_hvndr_Speed;
usb_tskinfo_t       g_usb_hvndr_Message;                    /* Pcd Task recive message */
uint8_t             g_usb_hvndr_CtrlData[USB_SMPL_DATASIZE];
usb_hcdrequest_t    g_usb_hvndr_CtrlReq;
usb_utr_t           g_usb_hvndr_CtrlMsg;
usb_utr_t           g_usb_hvndr_TrnMsg[USB_MAX_PIPE_NO+1];
usb_leng_t          g_usb_hvndr_TrnSize[USB_MAX_PIPE_NO+1];
uint8_t             *g_usb_hvndr_TrnPtr[USB_MAX_PIPE_NO+1];
uint16_t            g_usb_hvndr_PipeCtr[USB_MAX_PIPE_NO+1];
usb_cb_t            g_usb_hvndr_Callback[USB_MAX_PIPE_NO+1];
uint8_t             g_usb_hvndr_UsePipe[USB_MAX_PIPE_NO+1];

uint16_t g_usb_hvndr_DefEpTbl[] =
{
    /* PIPE4 */
    USB_PIPE4,
    USB_NULL|USB_BFREOFF|USB_DBLBOFF|USB_SHTNAKOFF,
    USB_NULL,   USB_NULL,
    /* PIPE5 */
    USB_PIPE5,
    USB_NULL|USB_BFREOFF|USB_DBLBOFF|USB_SHTNAKOFF,
    USB_NULL,   USB_NULL,
    /* PIPE6 */
    USB_PIPE6,
    USB_NULL,
    USB_NULL,   USB_NULL,
    /* PIPE7 */
    USB_PIPE7,
    USB_NULL,
    USB_NULL,   USB_NULL,
/* Pipe end */
    USB_PDTBLEND,
};

const uint16_t g_usb_hvndr_TplTbl[] =
{
    1,                      /* Number of list */
    0,                      /* Reserved */
    USB_VENDORID,           /* Vendor ID  : no-check */
    USB_PRODUCTID           /* Product ID : no-check */
};

/* functions */
void            g_usb_hvndr_task(void);
void            usb_hvndr_state_diagram(uint16_t data, uint16_t state);
void            usb_hvndr_enumaration(usb_tskinfo_t *mess);
void            usb_hvndr_get_config(usb_addr_t addr, uint16_t length, usb_cb_t complete);
void            usb_hvndr_get_string(usb_addr_t addr, uint16_t index, usb_cb_t complete);
void            usb_hvndr_control_result(usb_utr_t *mess);
uint8_t         usb_hvndr_check_descriptor(uint8_t *table, uint16_t spec);
uint8_t         usb_hvndr_pipe_info(uint8_t *table, uint8_t length );
void            usb_hvndr_open_close(uint16_t data1, uint16_t device_state);
usb_er_t        usb_hvndr_data_transfer(usb_pipe_t Pipe);
void            usb_hvndr_transfer_result(usb_utr_t *mess);
void            usb_hvndr_dummy_function(void);


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */

/* functions */



/******************************************************************************
Renesas Host Sample Vendor Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : g_usb_hvndr_task
Description     : Sample vendor class driver
Arguments       : none
Return value    : none
******************************************************************************/
void    g_usb_hvndr_task(void)
{
    usb_utr_t       *mess;
    usb_pipe_t      pipe;

    if (R_USB_RCV_MSG(USB_HVEN_MBX, (usb_msg_t**)&mess) != USB_E_OK)
    {
        return;
    }

    switch( mess->msginfo )
    {
        case USB_SMPL_CHECKREQUEST:
            usb_hvndr_enumaration((usb_tskinfo_t*)mess);
        break;

        case USB_SMPL_TRANSFER_START:
            pipe    = mess->pipenum;
            g_usb_hvndr_TrnPtr[pipe]        = mess->tranadr;
            g_usb_hvndr_TrnSize[pipe]   = mess->tranlen;
            g_usb_hvndr_Callback[pipe]  = mess->complete;
            if (usb_hvndr_data_transfer(pipe) == USB_E_ERROR)
            {
                g_usb_hvndr_TrnSize[pipe]   = 0;
            }
        break;
        case USB_SMPL_TRANSFER_STOP:
            R_usb_hstd_TransferEnd(mess->pipenum, USB_DO_TRANSFER_STP);
            R_USB_REL_BLK(mess->flag);
        break;

        case USB_SMPL_TRANSFER_END:
            pipe    = mess->pipenum;
            switch(mess->status)
            {
                case USB_DATA_OK:
                case USB_DATA_SHT:
                case USB_DATA_OVR:
                    g_usb_hvndr_PipeCtr[pipe]   = mess->pipectr;
                case USB_DATA_STOP:         /* Forced ending */
                    g_usb_hvndr_TrnSize[pipe]   = 0;
                    ((void (*)(usb_utr_t*))*g_usb_hvndr_Callback[pipe])(mess);
                break;
                case USB_DATA_DTCH:         /* detach */
                break;
                case USB_DATA_TMO:          /* time out */
                case USB_DATA_ERR:
                default:
                break;
            }   
        break;
        default:
        break;
    }
}   /* eof g_usb_hvndr_task() */

/******************************************************************************
Function Name   : usb_hvndr_state_diagram
Description     : Device State CallBack Check
Arguments       : uint16_t state
Return value    : uint16_t data
******************************************************************************/
void usb_hvndr_state_diagram(uint16_t data, uint16_t state)
{
    uint16_t    i;

    switch( state )
    {
        case USB_STS_DETACH:
            for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
            {
                g_usb_hvndr_TrnSize[i]  = 0;
                g_usb_hvndr_UsePipe[i] = USB_NO;
            }
            g_usb_hvndr_DefEpTbl[0]     = USB_PIPE4;
            g_usb_hvndr_DefEpTbl[1]     = USB_NULL|USB_BFREOFF|USB_DBLBOFF|USB_SHTNAKOFF;
            g_usb_hvndr_DefEpTbl[2]     = USB_NULL;
            g_usb_hvndr_DefEpTbl[3]     = USB_NULL;
            g_usb_hvndr_DefEpTbl[4]     = USB_PIPE5;
            g_usb_hvndr_DefEpTbl[5]     = USB_NULL|USB_BFREOFF|USB_DBLBOFF|USB_SHTNAKOFF;
            g_usb_hvndr_DefEpTbl[6]     = USB_NULL;
            g_usb_hvndr_DefEpTbl[7]     = USB_NULL;
            g_usb_hvndr_DefEpTbl[8]     = USB_PIPE6;
            g_usb_hvndr_DefEpTbl[9]     = USB_NULL;
            g_usb_hvndr_DefEpTbl[10]    = USB_NULL;
            g_usb_hvndr_DefEpTbl[11]    = USB_NULL;
            g_usb_hvndr_DefEpTbl[12]    = USB_PIPE7;
            g_usb_hvndr_DefEpTbl[13]    = USB_NULL;
            g_usb_hvndr_DefEpTbl[14]    = USB_NULL;
            g_usb_hvndr_DefEpTbl[15]    = USB_NULL;
        break;
        case USB_STS_DEFAULT:
        break;
        case USB_STS_ADDRESS:
        break;
        case USB_STS_CONFIGURED:
        break;
        case USB_STS_SUSPEND:
        break;
        case USB_STS_RESUME:
        break;
        case USB_STS_WAKEUP:
        break;
        default:
        break;
    }
}   /* eof usb_hvndr_state_diagram() */


/******************************************************************************
Function Name   : usb_hvndr_enumaration
Description     : Class Enumeration
Arguments       : usb_clsinfo_t *mess       : Class Info Data Pointer
Return value    : none
******************************************************************************/
void usb_hvndr_enumaration(usb_tskinfo_t *mess)
{
/* Condition compilation by the difference of useful function */
    totallen_t  device_id, length2;
    uint16_t    checkerr,  length1;
    uint8_t     retval;
    uint8_t     string, i;
//    uint8_t     md;
//    uint8_t     pdata[10];

    checkerr = mess->status;

    /* Manager Mode Change */
    switch( g_usb_hvndr_Sequence )
    {
        case USB_SEQ_00:
            device_id.WORD.BYTE.up      =  g_usb_hvndr_DevTbl[USB_DEV_VENDOR_HI];
            device_id.WORD.BYTE.dn      =  g_usb_hvndr_DevTbl[USB_DEV_VENDOR_LO];

            /* Check TPL */
            for( i = 0, checkerr = USB_ERROR; i < g_usb_hvndr_TplTbl[0]; i++ )
            {
                if ((g_usb_hvndr_TplTbl[(i * 2) + 2] == USB_NOVENDOR) ||
                    (g_usb_hvndr_TplTbl[(i * 2) + 2] == device_id.WORD.w))
                {
                    device_id.WORD.BYTE.up      =  g_usb_hvndr_DevTbl[USB_DEV_PRODUCT_HI];
                    device_id.WORD.BYTE.dn      =  g_usb_hvndr_DevTbl[USB_DEV_PRODUCT_LO];
                    if ((g_usb_hvndr_TplTbl[(i * 2) + 3] == USB_NOPRODUCT) ||
                        (g_usb_hvndr_TplTbl[(i * 2) + 3] == device_id.WORD.w))
                    {
                        checkerr = USB_YES;
                    }
                }
            }
            if( checkerr == USB_ERROR )
            {
                break;
            }

            /* Check interface class */
            length1 = 0;
            length2.WORD.BYTE.up        =  g_usb_hvndr_CnfgTbl[USB_CON_TOTALLENG_HI];
            length2.WORD.BYTE.dn        =  g_usb_hvndr_CnfgTbl[USB_CON_TOTALLENG_LO];
            if( length2.WORD.w > USB_CONFIGSIZE )
            {
                length2.WORD.w = USB_CONFIGSIZE;
            }

            /* Search within configuration total-length */
            checkerr = USB_ERROR;
            while( length1 < length2.WORD.w )
            {               
                if( g_usb_hvndr_CnfgTbl[length1+1] == USB_DT_INTERFACE )
                {
                    g_usb_hvndr_InterfaceTable  = (uint8_t*)&g_usb_hvndr_CnfgTbl[length1];
                    if( (uint16_t)g_usb_hvndr_CnfgTbl[length1+USB_INTF_CLASS] == USB_IFCLS_VEN )
                    {
                        checkerr = USB_YES;
                        usb_hvndr_get_config(g_usb_hvndr_DevAddr,length2.WORD.w,(usb_cb_t)&usb_hvndr_control_result);
                        break;
                    }
                }
                length1 += g_usb_hvndr_CnfgTbl[length1];
            }
        break;

        case USB_SEQ_01:
            checkerr = USB_YES;
            /* Configuration Descriptor check */
            retval = usb_hvndr_check_descriptor(g_usb_hvndr_CnfgTbl, USB_DT_CONFIGURATION);
            if( retval == USB_ERROR )
            {
                /* Descriptor error */
                checkerr = USB_ERROR;
                break;
            }

            /* Interface Descriptor check */
            retval = usb_hvndr_check_descriptor(g_usb_hvndr_InterfaceTable, USB_DT_INTERFACE);
            if( retval == USB_ERROR )
            {
                /* Descriptor error */
                checkerr = USB_ERROR;
                break;
            }
            /* String descriptor */
            usb_hvndr_get_string(g_usb_hvndr_DevAddr, (uint16_t)0,(usb_cb_t)&usb_hvndr_control_result);
        break;

        case USB_SEQ_02:
            /* String descriptor check */
            if( checkerr == (usb_er_t)USB_DATA_STALL )
            {
                checkerr = USB_ERROR;
            }
            else if( checkerr != (usb_er_t)USB_CTRL_END )
            {
                checkerr = USB_ERROR;
            }
            else
            {
                checkerr = USB_YES;
                string = g_usb_hvndr_DevTbl[USB_DEV_IPRODUCT];
                usb_hvndr_get_string(g_usb_hvndr_DevAddr, (uint16_t)string,(usb_cb_t)&usb_hvndr_control_result);
            }
        break;

        case USB_SEQ_03:
            /* String descriptor check (iProduct) */
            if( checkerr == (usb_er_t)USB_DATA_STALL )
            {
                checkerr = USB_ERROR;
            }
            else if( checkerr != (usb_er_t)USB_CTRL_END )
            {
                checkerr = USB_ERROR;
            }
            else
            {
            }
            /* Pipe Information table set */
            retval = usb_hvndr_pipe_info(g_usb_hvndr_InterfaceTable, (uint8_t)g_usb_hvndr_CnfgTbl[2]);
            if( retval == USB_ERROR )
            {
                /* Pipe Information table error */
                checkerr = USB_ERROR;
                break;
            }
            if( R_USB_WAI_MSG(USB_HVEN_MBX, (usb_msg_t*)&g_usb_hvndr_Message, 1000) != USB_E_OK )
            {
                USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE3);
            }
            checkerr = USB_YES;
        break;

        case USB_SEQ_04:
            if( R_USB_WAI_MSG(USB_HVEN_MBX, (usb_msg_t*)&g_usb_hvndr_Message, 1000) != USB_E_OK )
            {
                USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE4);
            }
            checkerr = USB_YES;
        break;

        case USB_SEQ_05:
            R_usb_hstd_ReturnEnuMGR(USB_YES);   /* Return to MGR */
            for( i = USB_MIN_PIPE_NO; i <= USB_MAX_PIPE_NO; i++ )
            {
                g_usb_hvndr_PipeCtr[i]  = 0;
            }
            checkerr = USB_YES;
        break;
        default:
        break;
    }

    switch( checkerr )
    {
        case USB_YES:               /* Driver open */
            g_usb_hvndr_Sequence++;
        break;
        case USB_ERROR:             /* Descriptor error */
            R_usb_hstd_ReturnEnuMGR(USB_NO);    /* Return to MGR */
        break;
        default:
            R_usb_hstd_ReturnEnuMGR(USB_NO);    /* Return to MGR */
        break;
    }
}   /* eof usb_hvndr_enumaration() */


/******************************************************************************
Function Name   : usb_hvndr_get_config
Description     : Set GetConfigurationDescriptor
Arguments       : usb_addr_t addr           : device address
                : uint16_t length           : descriptor length
                : usb_cb_t complete     : callback function
Return value    : none
******************************************************************************/
void usb_hvndr_get_config(usb_addr_t addr, uint16_t length, usb_cb_t complete)
{
    uint16_t        i;

    /* Get Configuration Descriptor */
    g_usb_hvndr_CtrlReq.WORD.BYTE.bmRequestType = USB_REQUEST_TYPE(USB_DEV_TO_HOST,USB_STANDARD,USB_DEVICE);
    g_usb_hvndr_CtrlReq.WORD.BYTE.bRequest      = USB_GET_DESCRIPTOR;
    g_usb_hvndr_CtrlReq.wValue      = USB_CONF_DESCRIPTOR;
    g_usb_hvndr_CtrlReq.wIndex      = 0x0000;
    g_usb_hvndr_CtrlReq.wLength     = length;
    if( g_usb_hvndr_CtrlReq.wLength > USB_SMPL_DATASIZE )
    {
        g_usb_hvndr_CtrlReq.wLength = USB_SMPL_DATASIZE;
    }

    g_usb_hvndr_CtrlReq.Address     = (uint16_t)addr;

    for( i = 0; i < g_usb_hvndr_CtrlReq.wLength; i++ )
    {
        g_usb_hvndr_CtrlData[i] = 0xFF;
    }

    g_usb_hvndr_CtrlMsg.tranadr     = (void*)g_usb_hvndr_CtrlData;
    g_usb_hvndr_CtrlMsg.complete    = complete;
    g_usb_hvndr_CtrlMsg.tranlen     = (usb_leng_t)g_usb_hvndr_CtrlReq.wLength;
    g_usb_hvndr_CtrlMsg.pipenum     = USB_PIPE0;
    g_usb_hvndr_CtrlMsg.setup       = (void*)&g_usb_hvndr_CtrlReq;
    R_usb_hstd_TransferStart(&g_usb_hvndr_CtrlMsg);
}   /* eof usb_hvndr_get_config() */


/******************************************************************************
Function Name   : usb_hvndr_get_string2
Description     : Get String descriptor
Arguments       : usb_addr_t addr           : device address
                : uint16_t index            : descriptor index
                : usb_cb_t complete         : callback
Return value    : none
******************************************************************************/
void usb_hvndr_get_string(usb_addr_t addr, uint16_t index, usb_cb_t complete)
{
    uint16_t        i;

    g_usb_hvndr_CtrlReq.WORD.BYTE.bmRequestType = USB_REQUEST_TYPE(USB_DEV_TO_HOST,USB_STANDARD,USB_DEVICE);
    g_usb_hvndr_CtrlReq.WORD.BYTE.bRequest  = USB_GET_DESCRIPTOR;
    g_usb_hvndr_CtrlReq.wValue      = (uint16_t)(USB_STRING_DESCRIPTOR + index);

    if( index == 0 )
    {
        g_usb_hvndr_CtrlReq.wIndex  = 0x0000;
        g_usb_hvndr_CtrlReq.wLength = 0x0004;
    }
    else
    {
        /* Set LanguageID */
        g_usb_hvndr_CtrlReq.wIndex  = (uint16_t)(g_usb_hvndr_CtrlData[2]);
        g_usb_hvndr_CtrlReq.wIndex  |= (uint16_t)((uint16_t)(g_usb_hvndr_CtrlData[3]) << 8);
        g_usb_hvndr_CtrlReq.wLength = USB_SMPL_DATASIZE;
    }


    g_usb_hvndr_CtrlReq.Address     = (uint16_t)addr;

    for( i = 0; i < g_usb_hvndr_CtrlReq.wLength; i++ )
    {
        g_usb_hvndr_CtrlData[i] = 0xFF;
    }

    g_usb_hvndr_CtrlMsg.tranadr     = (void*)g_usb_hvndr_CtrlData;
    g_usb_hvndr_CtrlMsg.complete    = complete;
    g_usb_hvndr_CtrlMsg.tranlen     = (usb_leng_t)g_usb_hvndr_CtrlReq.wLength;
    g_usb_hvndr_CtrlMsg.pipenum     = USB_PIPE0;
    g_usb_hvndr_CtrlMsg.setup       = (void*)&g_usb_hvndr_CtrlReq;
    R_usb_hstd_TransferStart(&g_usb_hvndr_CtrlMsg);
}   /* eof usb_hvndr_get_string2() */


/******************************************************************************
Function Name   : usb_hvndr_control_result
Description     : Host Class Sample Check
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
void usb_hvndr_control_result(usb_utr_t *mess)
{
    usb_er_t            err;
    
    g_usb_hvndr_Message.msginfo = USB_SMPL_CHECKREQUEST;
    g_usb_hvndr_Message.status  = mess->status;

    /* Class check of enumeration sequence move to class function */
    err = R_USB_SND_MSG(USB_HVEN_MBX, (usb_msg_t*)&g_usb_hvndr_Message);
    if( err != USB_E_OK )
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE5);
    }
}   /* eof usb_hvndr_control_result() */


/******************************************************************************
Function Name   : usb_hvndr_check_descriptor
Description     : check descriptor
Arguments       : uint8_t *table            : table
                : uint16_t spec             : spec
Return value    : none
******************************************************************************/
uint8_t usb_hvndr_check_descriptor(uint8_t *table, uint16_t spec)
{
    /* Check Descriptor */
    if( table[1] == spec )
    {
        switch( table[1] )
        {
        case USB_DT_DEVICE:             /* Device Descriptor */
        case USB_DT_CONFIGURATION:      /* Configuration Descriptor */
        case USB_DT_STRING:             /* String Descriptor */
        case USB_DT_INTERFACE:          /* Interface Descriptor */
        case USB_DT_ENDPOINT:           /* Endpoint Descriptor */
        case USB_DT_DEVICE_QUALIFIER:   /* Device Qualifier Descriptor */
        case USB_DT_OTHER_SPEED_CONF:   /* Other Speed Configuration Descriptor */
        case USB_DT_INTERFACE_POWER:    /* Interface Power Descriptor */
        case USB_DT_HUBDESCRIPTOR:      /* HUB Descriptor */
        break;
        default:                        /* Not Descriptor */
            return USB_ERROR;
        }
        return USB_YES;
    }
    return USB_ERROR;
}   /* eof usb_hvndr_check_descriptor() */


/******************************************************************************
Function Name   : usb_hvndr_pipe_info
Description     : Data Transfer
Arguments       : uint8_t *table            : Descriptor Table
                : uint8_t  length           : Descriptor Table length
Return value    : uint16_t                  : DIR_H_IN / DIR_H_OUT / USB_ERROR
******************************************************************************/
uint8_t usb_hvndr_pipe_info(uint8_t *table, uint8_t length )
{
    uint16_t        *ptr;

    /* Check Endpoint Descriptor */
    ptr     = g_usb_hvndr_DefEpTbl;

    /* Check Descriptor */
    if( table[1] != USB_DT_INTERFACE )  /* Configuration Descriptor */
    {       
        return USB_ERROR;
    }

    /* Check Endpoint Descriptor */
    table += table[0];
    for ( ; *ptr != USB_PDTBLEND; ptr += USB_EPL)
    {
        if (R_usb_hstd_ChkPipeInfo(ptr, table) != USB_E_ERROR)
        {
            ptr[2]  |= USB_ADDR2DEVSEL(g_usb_hvndr_DevAddr);
            table += table[0];
            g_usb_hvndr_UsePipe[*ptr]   = USB_YES;
        }
    }
    return USB_YES;
}   /* eof usb_hvndr_pipe_info() */





/******************************************************************************
Function Name   : usb_hvndr_data_transfer
Description     : Data Transfer
Arguments       : uint16_t pipe             : Pipe No
                : usb_leng_t size           : Size
                : uint8_t  *table           : Trasfer Data Start address
Return value    : usb_er_t                  : error
******************************************************************************/
usb_er_t usb_hvndr_data_transfer(usb_pipe_t pipe)
{
    usb_er_t    err;

    /* PIPE Transfer set */
    g_usb_hvndr_TrnMsg[pipe].pipenum    = pipe;
    g_usb_hvndr_TrnMsg[pipe].tranadr    = g_usb_hvndr_TrnPtr[pipe];
    g_usb_hvndr_TrnMsg[pipe].tranlen    = g_usb_hvndr_TrnSize[pipe];
    g_usb_hvndr_TrnMsg[pipe].pipectr    = g_usb_hvndr_PipeCtr[pipe];
    g_usb_hvndr_TrnMsg[pipe].setup  = USB_NULL;
    g_usb_hvndr_TrnMsg[pipe].complete   = (usb_cb_t)&usb_hvndr_transfer_result;
    err = R_usb_hstd_TransferStart((usb_utr_t *)&g_usb_hvndr_TrnMsg[pipe]);
    return err;
}   /* eof usb_hvndr_data_transfer() */


/******************************************************************************
Function Name   : usb_hvndr_transfer_result
Description     : Data Transfer Callback Function 
Arguments       : usb_utr_t *mess           : Utr Pointer
Return value    : none
******************************************************************************/
void usb_hvndr_transfer_result(usb_utr_t *mess)
{
    usb_er_t        err;

    mess->msginfo = USB_SMPL_TRANSFER_END;

    err = R_USB_SND_MSG(USB_HVEN_MBX, (usb_msg_t*)mess);
    if( err != USB_E_OK )
    {
        USB_DEBUG_HOOK(USB_DEBUG_HOOK_CLASS | USB_DEBUG_HOOK_CODE6);
    }
}   /* eof usb_hvndr_transfer_result() */


/******************************************************************************
Function Name   : usb_hvndr_dummy_function
Description     : Vendor dummy function
Arguments       : none
Return value    : none
******************************************************************************/
void usb_hvndr_dummy_function(void)
{
}   /* eof usb_hvndr_dummy_function() */


/******************************************************************************
End of file
******************************************************************************/

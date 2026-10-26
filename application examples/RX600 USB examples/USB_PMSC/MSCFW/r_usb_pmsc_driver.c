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
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************
* File Name    : r_usb_PMSCdriver.c
* Version      : 1.00
* Device(s)    : Renesas SH-Series, RX-Series
* Tool-Chain   : Renesas SuperH RISC engine Standard Toolchain
*              : Renesas RX Standard Toolchain
* OS           : Common to None and uITRON 4.0 Spec
* H/W Platform : Independent
* Description  : USB Host and Peripheral Interrupt code
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 17.03.2010 0.80    First Release
*         : 30.07.2010 0.90    Updated comments
*         : 02.08.2010 0.91    Updated comments
*         : 29.10.2010 1.00    Mass Production Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
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
#include "r_usb_pmsc_api.h"
#include "r_usb_cmsc_define.h"      /* USB  Mass Storage Class Header */
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_catapi_define.h"    /* Peripheral ATAPI Device extern define */
#include "r_usb_pmsc_extern.h"      /* Peri Mass Storage Class Extern */


/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/


/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#ifdef      USB_SDRAM_USE_PP
    #pragma section="_cbw_csw"
#else   /* USB_SDRAM_USE_PP */
    #pragma section="_pmsc"
#endif  /* USB_SDRAM_USE_PP */

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Mass Storage Device Class CBW */
#pragma location = "usb_gpmsc_cbw"
USB_MSC_CBW_t   usb_gpmsc_Cbw;
/* Mass Storage Device Class CSW */
#pragma location = "usb_gpmsc_csw"
USB_MSC_CSW_t   usb_gpmsc_Csw;

/******************************************************************************
Section    <Section Difinition> , "Project Sections"
******************************************************************************/
#pragma section="_pmsc"

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Mass Storage Device Class sequence */
uint8_t         usb_gpmsc_Seq;
/* Mass Storage Device Class CBW Tag */
uint32_t        usb_gpmsc_Tag;
/* Mass Storage Device Class Transfer Length(CBW) */
uint32_t        usb_gpmsc_Dtl;
/* Mass Storage Device Class */
USB_PMSC_CBM_t  usb_gpmsc_Message;
/* Pipe Direction from Host to Device */
uint16_t        usb_gpmsc_Outpipe;
/* Pipe Direction from Device to Host */
uint16_t        usb_gpmsc_Inpipe;


/******************************************************************************
Renesas Abstracted Peripheral Mass Storage Class Driver API functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_pmsc_Task
Description     : Peripheral Mass Storage Class main task
Arguments       : USB_VP_INT_t stacd        : start code
Return value    : none
******************************************************************************/
void usb_pmsc_Task(USB_VP_INT_t stacd)
{
    USB_UTR_t   *mess;
    USB_ER_t    err = 0l;
    uint8_t     uc_lun = 0;     /* Logical Unit Number */

    /* Pipe Transfer */
    err = R_USB_TRCV_MSG(USB_PMSC_MBX,(USB_MSG_t**)&mess,(uint16_t)3000);
    if( err == USB_E_OK )
    {
        if( mess->msginfo == USB_MASS_STORAGE_RESET )
        {
            /* Please add the processing with the system. */
            R_usb_pmsc_SetConfig(mess);
            /* CallBack function (SND_MSG) */
            (*mess->complete)(mess,0,0);
            err = R_USB_REL_BLK(USB_PMSC_MPL,(USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1(
                    "### pmscTask RESET rel_blk error (%ld)\n"
                    , err);
            }
        }
        else if( mess->msginfo == USB_GET_MAX_LUN )
        {
            R_usb_pstd_ControlRead(mess, (uint32_t)USB_MSC_LUN_LENGTH, &uc_lun);
            /* CallBack function (SND_MSG) */
            (*mess->complete)(mess,0,0);
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                USB_PRINTF1(
                    "### pmscTask MAX_LUN rel_blk error (%ld)\n"
                    , err);
            }
        }
        else if( mess->msginfo == USB_PMSC_REQ_TRANS_END )
        {
            /* Request End Process */
            R_usb_pstd_ControlEnd(mess, USB_CTRL_END);
            err = R_USB_REL_BLK(USB_PMSC_MPL,(USB_MH_t)mess);
            if(err != USB_E_OK)
            {
                USB_PRINTF0(
                "### Peripheral Mass Storage process rel_blk error\n");
            }
        }
        else if( mess->msginfo == USB_PMSC_USB_EXECUTE )
        {
            /* CSW(NG) transfer */
            usb_pmsc_CswSet((uint8_t)USB_MSC_CSW_NG, usb_gpmsc_Dtl);
            R_usb_pmsc_DataTrans(mess, usb_gpmsc_Inpipe
                , (uint32_t)USB_MSC_CSW_LENGTH
                , (uint8_t *)&usb_gpmsc_Csw
                , (USB_CB_t)&R_usb_pmsc_TransResult);
            usb_gpmsc_Seq = USB_PMSC_PCSWSND;
            err = R_USB_REL_BLK(USB_PMSC_MPL,(USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                /* Memory block release failure */
                USB_PRINTF0(
                "### Peripheral Mass Storage process rel_blk error\n");
            }
        }
        else if( mess->msginfo == USB_PMSC_ERR_CSW_NG )
        {
            /* CSW(NG) transfer */
            usb_pmsc_CswSet((uint8_t)USB_MSC_CSW_NG, usb_gpmsc_Dtl);
            R_usb_pmsc_DataTrans(mess, usb_gpmsc_Inpipe
                , (uint32_t)USB_MSC_CSW_LENGTH
                , (uint8_t*)&usb_gpmsc_Csw
                , &R_usb_pmsc_TransResult);
            usb_gpmsc_Seq = USB_PMSC_PCSWSND;
            err = R_USB_REL_BLK(USB_PMSC_MPL,(USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                /* Memory block release failure */
                USB_PRINTF0(
                "### Peripheral Mass Storage process rel_blk error\n");
            }
        }
        else if( mess->msginfo == USB_PMSC_ERR_PHASE_ERR )
        {
            /* CSW transfer */
            usb_pmsc_CswSet((uint8_t)USB_MSC_CSW_PHASE_ERR
                , usb_gpmsc_Dtl);
            R_usb_pmsc_DataTrans(mess, usb_gpmsc_Inpipe
                , (uint32_t)USB_MSC_CSW_LENGTH
                , (uint8_t*)&usb_gpmsc_Csw, &R_usb_pmsc_TransResult);
            usb_gpmsc_Seq = USB_PMSC_PCSWSND;
            err = R_USB_REL_BLK(USB_PMSC_MPL,(USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                /* Memory block release failure */
                USB_PRINTF0(
                "### Peripheral Mass Storage process rel_blk error\n");
            }
        }
        else if( mess->msginfo == USB_PMSC_ERR_SET_STALL )
        {
            /* Set Stall */
            R_usb_pstd_SetStall(mess, (USB_CB_INFO_t)&usb_pmsc_ErrPhaseErr
                , (uint16_t)usb_gpmsc_Inpipe);
            err = R_USB_REL_BLK(USB_PMSC_MPL,(USB_MH_t)mess);
            if( err != USB_E_OK )
            {
                /* Memory block release failure */
                USB_PRINTF0(
                "### Peripheral Mass Storage process rel_blk error\n");
            }
        }
        else
        {
            switch( usb_gpmsc_Seq )
            {
/* Normal  pmsc_seq Conditions */
            case USB_PMSC_PCBWRCV:
                if( mess->msginfo == USB_PMSC_USB2PMSC )
                {
                    switch( mess->status )
                    {
                    /* Normal Status Condotion */
                    case USB_DATA_OK:
                    case USB_DATA_SHT:
                    case USB_DATA_OVR:
                        /* Valid check */
                        usb_gpmsc_Seq
                            = usb_pmsc_CheckValid(mess->tranlen);
                        /* Meaningful check */
                        usb_gpmsc_Seq = usb_pmsc_CheckMeaning(usb_gpmsc_Seq);
                        if( usb_gpmsc_Seq == USB_PMSC_PCHECK )
                        {
                            /*  Set data length */
                            usb_gpmsc_Dtl
                                = (uint32_t)usb_gpmsc_Cbw.dCBWDTL_Hi
                                << 24;
                            usb_gpmsc_Dtl
                                |= (uint32_t)usb_gpmsc_Cbw.dCBWDTL_MH
                                << 16;
                            usb_gpmsc_Dtl
                                |= (uint32_t)usb_gpmsc_Cbw.dCBWDTL_ML
                                << 8;
                            usb_gpmsc_Dtl
                                |= (uint32_t)usb_gpmsc_Cbw.dCBWDTL_Lo;
                            /* Analyze CBWCB Command */
                            usb_pmsc_SmpAtapiAnalyzeCbwCb(usb_gpmsc_Cbw.CBWCB);
                            usb_gpmsc_Seq
                                = usb_pmsc_CommandCheck(usb_gpmsc_Seq);
                        }
                        /* pmsc_seq classification */
                        switch( usb_gpmsc_Seq )
                        {
                        case USB_PMSC_PDATASND:
                        case USB_PMSC_PDATARCV:
                        case USB_PMSC_PCSWSND:
                            usb_pmsc_SmpAtapiCommandExecute(mess,
                                usb_gpmsc_Cbw.CBWCB, mess->status,
                                &usb_pmsc_AtapiTransResult);
                            break;
                        case USB_PMSC_PERROR0:
                            usb_pmsc_Error0(mess);
                            break;
                        case USB_PMSC_PERROR1:
                            usb_pmsc_Error1(mess);
                            break;
                        case USB_PMSC_PERROR2:
                            usb_pmsc_Error2(mess);
                            break;
                        case USB_PMSC_PERROR3:
                            usb_pmsc_Error3(mess);
                            break;
                        case USB_PMSC_PERROR4:
                            usb_pmsc_Error4(mess);
                            break;
                        case USB_PMSC_PERROR5:
                            usb_pmsc_Error5(mess);
                            break;
                        default:
                            break;
                        }
                        break;
                    /* Abnormal Status Conditions */
                    case USB_DATA_TMO:
                        break;
                    case USB_DATA_STOP:
                        USB_PRINTF0(
                            "### USB_PMSC_PCBWRCV detach error\n");
                        R_USB_DLY_TSK(20);
                        break;
                    case USB_DATA_ERR:
                    case USB_DATA_NONE:
                    default:
                        USB_PRINTF0("### USB_PMSC_PCBWRCV error\n");
                        USB_PRINTF1(
                            "### pmsc Status error:%x\n",mess->status);
                        USB_PRINTF1(
                            "### pmsc Sequence :%x\n",usb_gpmsc_Seq);
                        USB_PRINTF1(
                            "### pmsc msginfo:%x\n",mess->msginfo);
                        break;
                    }
                }
                else
                {
                    /* Abnormal Condition */
                    USB_PRINTF1("### pmsc msginfo error in CBWRCV:%x\n"
                        ,mess->msginfo);
                }
                break;
            case USB_PMSC_PDATARCV:
                if( mess->msginfo == USB_PMSC_USB2PMSC )
                {
                    switch( mess->status )
                    {
                    /* Normal Status Condotion */
                    case USB_DATA_OK:
                    case USB_DATA_SHT:
                    case USB_DATA_OVR:
                        usb_pmsc_SmpAtapiCommandExecute(mess,
                            usb_gpmsc_Cbw.CBWCB, mess->status
                            , &usb_pmsc_AtapiTransResult);
                        break;
                    /* Abnormal Stauts Conditions */
                    case USB_DATA_TMO:
                        USB_PRINTF0(
                            "### USB_PMSC_PDATARCV timeout error\n");
                        break;
                    case USB_DATA_STOP:
                        USB_PRINTF0(
                            "### USB_PMSC_PDATARCV detach error\n");
                        R_USB_DLY_TSK(20);
                        break;
                    case USB_DATA_ERR:
                    case USB_DATA_NONE:
                    default:
                        USB_PRINTF0("### USB_PMSC_PDATARCV error\n");
                        USB_PRINTF1("### pmsc Status error:%x\n"
                            , mess->status);
                        USB_PRINTF1("### pmsc Sequence :%x\n"
                            , usb_gpmsc_Seq);
                        USB_PRINTF1("### pmsc msginfo:%x\n"
                            , mess->msginfo);
                        break;
                    }
                }
                else
                {
                    /* Next treatment after ATAPI Command Execute */
                    usb_pmsc_UsbExecute(mess);
                }
                break;
            case USB_PMSC_PDATASND:
                if( mess->msginfo == USB_PMSC_USB2PMSC )
                {
                    switch( mess->status )
                    {
                    /* Normal Status Condotion */
                    case USB_DATA_NONE:
                        usb_pmsc_SmpAtapiCommandExecute(mess,
                            usb_gpmsc_Cbw.CBWCB, mess->status
                            , &usb_pmsc_AtapiTransResult);
                        break;
                    /* Abnormal Status Conditions */
                    case USB_DATA_TMO:
                        USB_PRINTF0(
                            "### USB_PMSC_PDATASND timeout error\n");
                        break;
                    case USB_DATA_STOP:
                        USB_PRINTF0(
                            "### USB_PMSC_PDATASND detach error\n");
                        R_USB_DLY_TSK(20);
                        break;
                    case USB_DATA_SHT:
                    case USB_DATA_OVR:
                    case USB_DATA_ERR:
                    case USB_DATA_OK:
                    default:
                        USB_PRINTF0("### USB_PMSC_PDATASND error\n");
                        USB_PRINTF1("### pmsc Status error:%x\n"
                            , mess->status);
                        USB_PRINTF1("### pmsc Sequence :%x\n"
                            , usb_gpmsc_Seq);
                        USB_PRINTF1("### pmsc msginfo:%x\n"
                            , mess->msginfo);
                        break;
                    }
                }
                else
                {
                    /* Next treatment after ATAPI Command Execute */
                    usb_pmsc_UsbExecute(mess);
                }
                break;
            case USB_PMSC_PCSWSND:
                if( mess->msginfo == USB_PMSC_USB2PMSC )
                {
                    switch( mess->status )
                    {
                    /* Normal  Status Condotion */
                    case USB_DATA_NONE:
                        R_usb_pmsc_SetConfig(mess);
                        break;
                    /* Abnormal Status Conditions */
                    case USB_DATA_TMO:
                        USB_PRINTF0(
                            "### USB_PMSC_PCSWSND timeout error\n");
                        break;
                    case USB_DATA_STOP:
                        USB_PRINTF0(
                            "### USB_PMSC_PCSWSND detach error\n");
                        R_USB_DLY_TSK(20);
                        break;
                    case USB_DATA_SHT:
                    case USB_DATA_OVR:
                    case USB_DATA_ERR:
                    case USB_DATA_OK:
                    default:
                        USB_PRINTF0("### USB_PMSC_PCSWSND error\n");
                        USB_PRINTF1("### pmsc Status error:%x\n"
                            , mess->status);
                        USB_PRINTF1("### pmsc Sequence :%x\n"
                            , usb_gpmsc_Seq);
                        USB_PRINTF1("### pmsc msginfo:%x\n"
                            , mess->msginfo);
                        break;
                    }
                }
                else
                {
                    /* Next treatment after ATAPI Command Execute */
                    usb_pmsc_UsbExecute(mess);
                }
                break;
            /* Abnormal pmsc_seq Conditions */
            case USB_PMSC_PDATASNDWAIT:
            case USB_PMSC_PDATARCVWAIT:
            case USB_PMSC_PERROR0:
            case USB_PMSC_PERROR1:
            case USB_PMSC_PERROR2:
            case USB_PMSC_PERROR3:
            case USB_PMSC_PERROR4:
            case USB_PMSC_PERROR5:
            case USB_PMSC_PCHECK:
            default:
                USB_PRINTF1("### pmsc Sequence error:%x\n"
                    , usb_gpmsc_Seq);
                break;
            }
        }
    }
    else
    {
        /* USB_PRINTF0("Peripheral strage driver rcv_msg error !\n"); */
        return;
    }
}   /* eof usb_pmsc_Task() */

/******************************************************************************
Function Name   : usb_pmsc_CheckValid
Description     : Valid CBW Check
Arguments       : uint32_t length           : length
Return value    : uint8_t                   : result
******************************************************************************/
uint8_t usb_pmsc_CheckValid(uint32_t length)
{
    uint8_t     result;

    result = USB_PMSC_PCHECK;
    usb_gpmsc_Tag = usb_gpmsc_Cbw.dCBWTag;

    /*  CBW packet valiable check    */
    if( usb_gpmsc_Cbw.dCBWSignature != USB_MSC_CBW_SIGNATURE )
    {
        result = USB_PMSC_PERROR0;
    }

    /* CBW Packet Length Check */
    if( length != 0ul )
    {
        result = USB_PMSC_PERROR0;
    }

    return result;
}   /* eof usb_pmsc_CheckValid() */

/******************************************************************************
Function Name   : usb_pmsc_CheckMeaning
Description     : Check the meaning of received CBW
Arguments       : uint8_t seq               : sequence
Return value    : uint8_t                   : result
******************************************************************************/
uint8_t usb_pmsc_CheckMeaning(uint8_t seq)
{
    uint8_t     result;

    if( seq != USB_PMSC_PCHECK )
    {
        return seq;
    }
    result = USB_PMSC_PCHECK;

    /*  Check reserved bit   */
    if( usb_gpmsc_Cbw.bmCBWFlags.reserved7 != 0 )
    {
        result = USB_PMSC_PERROR1;
    }
    /*  Check reserved bit   */
    if( usb_gpmsc_Cbw.bCBWLUN.reserved4 != 0 )
    {
        result = USB_PMSC_PERROR1;
    }
    /*  Check reserved bit   */
    if( usb_gpmsc_Cbw.bCBWCBLength.reserved3 != 0 )
    {
        result = USB_PMSC_PERROR1;
    }

    /*  Check Logical Unit Number   */
    if( usb_gpmsc_Cbw.bCBWLUN.bCBWLUN > 0 )
    {
        result = USB_PMSC_PERROR1;
    }

    /*  Check Command Block Length  */
    if( usb_gpmsc_Cbw.bCBWCBLength.bCBWCBLength > usb_gpmsc_CbwCbLength )
    {
        result = USB_PMSC_PERROR1;
    }
    return result;
}   /* eof usb_pmsc_CheckMeaning() */

/******************************************************************************
Function Name   : usb_pmsc_UsbExecute
Description     : Next treatment after ATAPI Command Execute
Arguments       : USB_UTR_t *mess           : message
Return value    : none
******************************************************************************/
void usb_pmsc_UsbExecute(USB_UTR_t *mess)
{
    uint32_t    gul_pmsc_size;
    void        *gvp_pmsc_adr;

    switch( mess->status )
    {
    /* Command Execute -> Pass! */
    case USB_PMSC_CMD_COMPLETE:
        /* CSW(OK) transfer */
        usb_pmsc_CswSet((uint8_t)USB_MSC_CSW_OK, usb_gpmsc_Dtl);
        R_usb_pmsc_DataTrans(mess, usb_gpmsc_Inpipe, (uint32_t)USB_MSC_CSW_LENGTH
            , (uint8_t*)&usb_gpmsc_Csw, &R_usb_pmsc_TransResult);
        usb_gpmsc_Seq = USB_PMSC_PCSWSND;
        break;

    /* Command Execute -> Fail */
    case USB_PMSC_CMD_FAILED:
        if( usb_gpmsc_Dtl != 0ul )
        {   /* Zero or short packet */
            R_usb_pstd_SetStall(mess, (USB_CB_INFO_t)&usb_pmsc_UsbExecuteCont
                , usb_gpmsc_Inpipe);
        }
        else
        {
            /* CSW(NG) transfer */
            usb_pmsc_CswSet((uint8_t)USB_MSC_CSW_NG, usb_gpmsc_Dtl);
            R_usb_pmsc_DataTrans(mess, usb_gpmsc_Inpipe
                , (uint32_t)USB_MSC_CSW_LENGTH, (uint8_t*)&usb_gpmsc_Csw
                , &R_usb_pmsc_TransResult);
            usb_gpmsc_Seq = USB_PMSC_PCSWSND;
        }
        break;

    /* Set Send /Recieve Data parameters */
    case USB_PMSC_CMD_CONTINUE:
        switch( usb_gpmsc_Seq )
        {
        case USB_PMSC_PDATARCV:     /* OUT Data */
            gul_pmsc_size   = mess->tranlen;
            gvp_pmsc_adr    = mess->tranadr;
            /* Data receive start */
            R_usb_pmsc_DataTrans(mess, usb_gpmsc_Outpipe, gul_pmsc_size
                , (uint8_t*)gvp_pmsc_adr, &R_usb_pmsc_TransResult);
            usb_gpmsc_Seq = USB_PMSC_PDATARCV;
            break;
        case USB_PMSC_PDATASND:     /* IN Data */
            gul_pmsc_size   = mess->tranlen;
            gvp_pmsc_adr    = mess->tranadr;
            /* Data transfer start */
            R_usb_pmsc_DataTrans(mess, usb_gpmsc_Inpipe, gul_pmsc_size
                ,(uint8_t*)gvp_pmsc_adr, &R_usb_pmsc_TransResult);
            usb_gpmsc_Seq = USB_PMSC_PDATASND;
            break;

        case USB_PMSC_PDATARCVWAIT:
        case USB_PMSC_PDATASNDWAIT:
        case USB_PMSC_PCSWSND:
        default:
            USB_PRINTF0("### usb_pmsc_UsbExecute PMSC error:\n");
            break;
        }
        break;

    case USB_PMSC_CMD_ERROR:
        USB_PRINTF2("### usb_pmsc_UsbExecute CMD error1:%d, %x\n"
            , usb_gpmsc_Seq,mess->status);
        USB_PRINTF1("### usb_gpmsc_Message:%08x\n", mess->msghead);
        USB_PRINTF3("### msginfo:%04x, keyword:%04x, status:%04x\n"
            , mess->msginfo,mess->keyword,mess->status);
        USB_PRINTF2("### *tranadr:%08x, tranlen:%d\n", &mess->tranadr
            , mess->tranlen);
        USB_PRINTF0("### error\n");
        break;
    default:

        USB_PRINTF2("### usb_pmsc_UsbExecute CMD error2:%d, %x\n"
            , usb_gpmsc_Seq,mess->status);
        USB_PRINTF1("### usb_gpmsc_Message:%08x\n",mess->msghead);
        USB_PRINTF3("### msginfo:%04x, keyword:%04x, status:%04x\n"
            , mess->msginfo,mess->keyword,mess->status);
        USB_PRINTF2("### *tranadr:%08x, tranlen:%d\n", &mess->tranadr
            , mess->tranlen);
        USB_PRINTF0("### error\n");
        break;
    }
}   /* eof usb_pmsc_UsbExecute() */

/******************************************************************************
Function Name   : usb_pmsc_CommandCheck
Description     : Check Analyze_CBWCB result
Arguments       : uint8_t seq               : sequence
Return value    : uint8_t                   : result
******************************************************************************/
uint8_t usb_pmsc_CommandCheck(uint8_t seq)
{
    uint8_t     uc_case, result, uc_pmsc_case;

    if( seq != USB_PMSC_PCHECK )
    {
        return seq;
    }

    result = USB_PMSC_PCHECK;
    /* Mass Storage Device Class Command case */
    uc_pmsc_case = USB_MSC_CASE00;

    /* Command check */
    switch( usb_gpmsc_Message.ar_rst )
    {
        case USB_ATAPI_NO_DATA:             /* No data commnad */
            uc_case = USB_MSC_DNXX;
            break;
        case USB_ATAPI_SND_DATAS:           /* Send data command */
            uc_case = USB_MSC_DIXX;
            break;
        case USB_ATAPI_RCV_DATAS:           /* Receive data command */
            uc_case = USB_MSC_DOXX;
            break;
        case USB_ATAPI_NOT_SUPPORT:         /* Not Support Command */
            if( usb_gpmsc_Dtl == 0ul )
            {
                result = USB_PMSC_PERROR5;  /* No STALL Pipe */
            }
            else
            {
                if( usb_gpmsc_Cbw.bmCBWFlags.CBWdir == 1 )
                {
                    result = USB_PMSC_PERROR1;  /* IN Pipe Stall */
                }
                else
                {
                    result = USB_PMSC_PERROR2;  /* OUT Pipe Stall */
                }
            }
            break;
        case USB_ATAPI_CMD_CONTINUE:
        case USB_ATAPI_CMD_COMPLETE:
        case USB_ATAPI_ERROR:
        default:
            USB_PRINTF0("### Unexpcted Command Check Error\n");
            result = USB_PMSC_PERROR1;
            break;
    }
    if( result == USB_PMSC_PCHECK )
    {
        /* Check 13 case */
        uc_pmsc_case = usb_pmsc_CheckCase13(usb_gpmsc_Message.ul_size
            , &uc_case);
        /* Decide response according to 13 case */
        result = usb_pmsc_TransferMatrix(uc_pmsc_case);
    }
    return result;
}   /* eof usb_pmsc_CommandCheck() */

/******************************************************************************
Function Name   : usb_pmsc_CheckCase13
Description     : 13 case check
Arguments       : uint32_t ul_size          : size
                : uint8_t *uc_case          : case
Return value    : uint8_t                   : result
******************************************************************************/
uint8_t usb_pmsc_CheckCase13(uint32_t ul_size, uint8_t *uc_case)
{
    uint8_t     result;

    if( usb_gpmsc_Dtl == 0ul )
    {
        *uc_case |= (uint8_t)USB_MSC_XXHN;  /* Host No Data */
    }
    else if( usb_gpmsc_Cbw.bmCBWFlags.CBWdir != 0 )
    {
        *uc_case |= (uint8_t)USB_MSC_XXHI;  /* Host Recieved(IN) Data */
    }
    else
    {
        *uc_case |= (uint8_t)USB_MSC_XXHO;  /* Host Send(OUT) Data */
    }

/* 13cases */
    switch( *uc_case )
    {
    case USB_MSC_DNHN:  /* Device No Data */
        result = USB_MSC_CASE01;
        break;
    case USB_MSC_DNHI:  /* Device No Data & Host Recieved(IN) Data */
        result = USB_MSC_CASE04;
        break;
    case USB_MSC_DNHO:  /* Device No Data & Host Send(OUT) Data */
        result = USB_MSC_CASE09;
        break;
    case USB_MSC_DIHN:  /* Device Send(IN) Data & Host No Data */
        result= USB_MSC_CASE02;
        break;
    case USB_MSC_DIHI:  /* Device Send(IN) Data & Host Recieved(IN) Data */
        if( usb_gpmsc_Dtl > (ul_size) )
        {
            result = USB_MSC_CASE05;
        }
        else
        {
            if( usb_gpmsc_Dtl == (ul_size) )
            {
                result= USB_MSC_CASE06;
            }
            else
            {
                result= USB_MSC_CASE07;
            }
        }
        break;
    case USB_MSC_DIHO:  /* Device Send(IN) Data & Host Send(OUT) Data */
        result= USB_MSC_CASE10;
        break;
    case USB_MSC_DOHN:  /* Device Recieved(OUT) Data & Host No Data */
        result= USB_MSC_CASE03;
        break;
    case USB_MSC_DOHI:  /* Device Recieved(OUT) Data
                                    & Host Recieved(IN) Data */
        result=  USB_MSC_CASE08;
        break;
    case USB_MSC_DOHO:  /* Device Recieved(OUT) Data
                                    & Host Send(OUT) Data */
        if( usb_gpmsc_Dtl > (ul_size ) )
        {
            result = USB_MSC_CASE11;
        }
        else
        {
            if( usb_gpmsc_Dtl == (ul_size ) )
            {
                result = USB_MSC_CASE12;
            }
            else
            {
                result = USB_MSC_CASE13;
            }
        }
        break;
    default:
        USB_PRINTF0("### (Error) Not Found 13 Case \n");
        result = USB_MSC_CASE00;
        break;
    }

    return result;
}   /* eof usb_pmsc_CheckCase13() */

/******************************************************************************
Function Name   : usb_pmsc_TransferMatrix
Description     : Decide response according to 13 case
Arguments       : uint8_t uc_case           : case
Return value    : uint8_t                   : result
******************************************************************************/
uint8_t usb_pmsc_TransferMatrix(uint8_t uc_case)
{
    uint8_t     result;

    switch ( uc_case )
    {
    case USB_MSC_CASE01:            /* CSW transfer */
        result = USB_PMSC_PCSWSND;  /* Transfer Command Status Wrapper */
        break;
    case USB_MSC_CASE05:
    case USB_MSC_CASE06:            /* Data Transfer(IN) */
        result = USB_PMSC_PDATASND; /* Data Transport */
        break;
    case USB_MSC_CASE12:            /* Data Transfer(OUT) */
        result = USB_PMSC_PDATARCV; /* Data Transport */
        break;
    case USB_MSC_CASE04:            /* CSW transfer */
        result = USB_PMSC_PERROR1;  /* Data Transfer(IN) */
        break;
    case USB_MSC_CASE09:            /* CSW transfer */
    case USB_MSC_CASE11:            /* Data Transfer(OUT) */
        result = USB_PMSC_PERROR2;
        break;
    case USB_MSC_CASE02:
    case USB_MSC_CASE03:
    case USB_MSC_CASE07:
    case USB_MSC_CASE08:
        result = USB_PMSC_PERROR3;
        break;
    case USB_MSC_CASE10:
    case USB_MSC_CASE13:
        result = USB_PMSC_PERROR4;
        break;
    case USB_MSC_CASE00:
    default:
        usb_gpmsc_Dtl = 0x00ul;
        result = USB_PMSC_PERROR4;  /* Internal Device Error */
        USB_PRINTF0("### (Error) Not CASE1-13 \n");
        break;
    }
    return result;
}   /* eof usb_pmsc_TransferMatrix() */

/******************************************************************************
Function Name   : usb_pmsc_CswSet
Description     : Create CSW
Arguments       : uint8_t ar_resp           : status
                : uint32_t ul_size          : size
Return value    : none
******************************************************************************/
void usb_pmsc_CswSet(uint8_t ar_resp, uint32_t ul_size)
{
    /* Set CSW parameter */
    usb_gpmsc_Csw.dCSWSignature         = USB_MSC_CSW_SIGNATURE;
    usb_gpmsc_Csw.dCSWTag               = usb_gpmsc_Tag;
    usb_gpmsc_Csw.dCSWDataResidue_Lo    = (uint8_t)ul_size;
    usb_gpmsc_Csw.dCSWDataResidue_ML    = (uint8_t)(ul_size >> 8);
    usb_gpmsc_Csw.dCSWDataResidue_MH    = (uint8_t)(ul_size >> 16);
    usb_gpmsc_Csw.dCSWDataResidue_Hi    = (uint8_t)(ul_size >> 24);
    usb_gpmsc_Csw.bCSWStatus            = ar_resp;
}   /* eof usb_pmsc_CswSet() */

/******************************************************************************
Function Name   : usb_pmsc_MassStrageReset
Description     : Process Mass storage reset request
Arguments       : uint16_t value            : value
                : uint16_t index            : index
                : uint16_t length           : length
Return value    : none
******************************************************************************/
void usb_pmsc_MassStrageReset(USB_UTR_t *ptr, uint16_t value, uint16_t index
        , uint16_t length)
{
    if( (value == 0) && (length == 0) )
    {
        USB_PRINTF0("### Mass Storage Reset Request \n");
        usb_cstd_SetBuf(ptr, (uint16_t)USB_PIPE0);
        /* Peripheral Mass Storage Reset request */
        R_usb_pmsc_MassStrageReset(ptr, &usb_pmsc_ReqTransEnd);
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );        /* Req Error */
    }
}   /* eof usb_pmsc_MassStrageReset() */

/******************************************************************************
Function Name   : usb_pmsc_GetMaxLun
Description     : Process Get max LUN request
Arguments       : uint16_t value            : value
                : uint16_t index            : index
                : uint16_t length           : length
Return value    : none
******************************************************************************/
void usb_pmsc_GetMaxLun(USB_UTR_t *ptr, uint16_t value, uint16_t index, uint16_t length)
{
    if( value == 0x0000 )
    {
        if( length == USB_MSC_LUN_LENGTH )
        {
            USB_PRINTF0("*** GetMaxLun Request \n");
            /* Peripheral Get Max LUN request */
            R_usb_pmsc_GetMaxLun(ptr, &usb_cstd_DummyFunction);
        }
        else
        {
            /* Set Stall */
            usb_pstd_SetStallPipe0( ptr );    /* Req Error */
        }
    }
    else
    {
        /* Set Stall */
        usb_pstd_SetStallPipe0( ptr );        /* Req Error */
    }
}   /* eof usb_pmsc_GetMaxLun() */

/******************************************************************************
Function Name   : usb_pmsc_Error0
Description     : Error case 0
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_Error0(USB_UTR_t *ptr)
{
    USB_PRINTF0("### ERROR0 \n");
}   /* eof usb_pmsc_Error0() */

/******************************************************************************
Function Name   : usb_pmsc_Error1
Description     : Error case 1
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_Error1(USB_UTR_t *ptr)
{
    USB_PRINTF0("### ERROR1 \n");
    /* Send Stall */
    R_usb_pstd_SetStall(ptr, (USB_CB_INFO_t)&usb_pmsc_ErrCswNg
        , usb_gpmsc_Inpipe);
}   /* eof usb_pmsc_Error1() */

/******************************************************************************
Function Name   : usb_pmsc_Error2
Description     : Error case 2
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_Error2(USB_UTR_t *ptr)
{
    USB_PRINTF0("### ERROR2 \n");
    /* Set Stall */
    R_usb_pstd_SetStall(ptr, (USB_CB_INFO_t)&usb_pmsc_ErrCswNg
        , usb_gpmsc_Outpipe);
}   /* eof usb_pmsc_Error2() */

/******************************************************************************
Function Name   : usb_pmsc_Error3
Description     : Error case 3
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_Error3(USB_UTR_t *ptr)
{
    USB_PRINTF0("### ERROR3 \n");
    usb_gpmsc_Dtl = 0x00ul;
    R_usb_pstd_SetStall(ptr, (USB_CB_INFO_t)&usb_pmsc_ErrPhaseErr
        , usb_gpmsc_Inpipe);
}   /* eof usb_pmsc_Error3() */

/******************************************************************************
Function Name   : usb_pmsc_Error4
Description     : Error case 4
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_Error4(USB_UTR_t *ptr)
{
    USB_PRINTF0("### ERROR4 \n");
    usb_gpmsc_Dtl = 0x00ul;
    R_usb_pstd_SetStall(ptr, (USB_CB_INFO_t)&usb_pmsc_ErrSetStall
        , usb_gpmsc_Outpipe);
}   /* eof usb_pmsc_Error4() */

/******************************************************************************
Function Name   : usb_pmsc_Error5
Description     : Error case 5
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_Error5(USB_UTR_t *ptr)
{
    USB_PRINTF0("### ERROR5 \n");
    /* CSW(NG) transfer */
    usb_pmsc_CswSet((uint8_t)USB_MSC_CSW_NG, usb_gpmsc_Dtl);
    R_usb_pmsc_DataTrans(ptr, usb_gpmsc_Inpipe, (uint32_t)USB_MSC_CSW_LENGTH
        , (uint8_t*)&usb_gpmsc_Csw, &R_usb_pmsc_TransResult);
    usb_gpmsc_Seq = USB_PMSC_PCSWSND;
}   /* eof usb_pmsc_Error5() */

/******************************************************************************
Function Name   : R_usb_pmsc_driver_start
Description     : USB Peri Initialize process
Argument        : none
Return          : none
******************************************************************************/
void R_usb_pmsc_driver_start( USB_UTR_t *ptr )
{
    R_usb_cstd_SetTaskPri(USB_PMSC_TSK, USB_PRI_4);
}
/******************************************************************************
End of function R_usb_pmsc_driver_start
******************************************************************************/

/******************************************************************************
Function Name   : usb_cstd_usbip_info_setting
Description     : Setting USBIP Number & USBIP address to strucre.
Arguments       : USB_UTR_t *ptr      :
Return value    : none
******************************************************************************/
void    usb_cstd_usbip_info_setting( USB_UTR_t *ptr )
{
/* Condition compilation by the difference of USB function */
#if USB_FUNCSEL_USBIP0_PP == USB_PERI_PP
    ptr->ip  = USB_USBIP_0;
#else   /**/
    ptr->ip  = USB_USBIP_1;
#endif  /**/
    ptr->ipp = R_usb_cstd_GetUsbIpAdr( ptr->ip );
}   /* eof usb_cstd_usbip_info_setting() */

/******************************************************************************
End  Of File
******************************************************************************/

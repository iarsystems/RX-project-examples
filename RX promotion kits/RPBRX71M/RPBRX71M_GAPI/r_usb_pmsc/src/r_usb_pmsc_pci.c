/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014(2015) Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_usb_pmsc_pci.c
* Description  : USB Host and Peripheral Interrupt code
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.01.2014 1.00 First Release
*         : 30.01.2015 1.01    Added RX71M.
*         : 30.09.2015 1.11    Added RX63N.
***********************************************************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_basic_if.h"
#include "r_usb_api.h"
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_pmsc_config.h"

/******************************************************************************
Constant macro definitions
******************************************************************************/


/******************************************************************************
External variables and functions
******************************************************************************/


/******************************************************************************
Private global variables and functions
******************************************************************************/
uint8_t                 usb_gpmsc_CbwCbLength;


/******************************************************************************
Function Name   : usb_pmsc_UsbExecuteCont
Description     : Execute Continue
Arguments       : uint16_t dat1             : not use
                : uint16_t dat2             : not use
Return value    : none
******************************************************************************/
void usb_pmsc_UsbExecuteCont(USB_UTR_t *ptr, uint16_t dat1, uint16_t dat2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_USB_EXECUTE;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;
        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0("### Peripheral UsbExecute snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral UsbExecute pget_blk error\n");
    }
}   /* eof usb_pmsc_UsbExecuteCont() */

/******************************************************************************
Function Name   : usb_pmsc_ErrCswNg
Description     : Error CSW
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_ErrCswNg(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_ERR_CSW_NG;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL,(USB_MH_t)p_blf);
            USB_PRINTF0("### Peripheral Error snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral Error pget_blk error\n");
    }
}   /* eof usb_pmsc_ErrCswNg() */

/******************************************************************************
Function Name   : usb_pmsc_ErrPhaseErr
Description     : Phase error
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_ErrPhaseErr(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_ERR_PHASE_ERR;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0("### Peripheral Error snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral Error pget_blk error\n");
    }
}   /* eof usb_pmsc_ErrPhaseErr() */

/******************************************************************************
Function Name   : usb_pmsc_ErrSetStall
Description     : Set stall
Arguments       : none
Return value    : none
******************************************************************************/
void usb_pmsc_ErrSetStall(USB_UTR_t *ptr, uint16_t data1, uint16_t data2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_ERR_SET_STALL;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0("### Peripheral Error2 snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral Error2 pget_blk error\n");
    }
}   /* eof usb_pmsc_ErrSetStall() */

/******************************************************************************
Function Name   : usb_pmsc_ReqTransEnd
Description     : Request transmit end
Arguments       : uint16_t dat1             : not use
                : uint16_t dat2             : not use
Return value    : none
******************************************************************************/
void usb_pmsc_ReqTransEnd(USB_UTR_t *ptr, uint16_t dat1, uint16_t dat2)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;
    
    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_PMSC_REQ_TRANS_END;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)NULL;
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0( "### Peripheral Mass Storage Reset request snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0( "### Peripheral Mass Storage Reset request pget_blk error\n");
    }
}   /* eof usb_pmsc_ReqTransEnd() */

/******************************************************************************
Function Name   : usb_pmsc_Cont
Description     : Request send message
Arguments       : USB_CB_t complete    : callback
Return value    : none
******************************************************************************/
void usb_pmsc_Cont(USB_UTR_t *ptr, USB_CB_t complete)
{
    USB_MH_t    p_blf;
    USB_ER_t    err = USB_ERROR;
    USB_UTR_t   *cp;

    /* Get mem pool blk */
    if( R_USB_PGET_BLK(USB_PMSC_MPL, &p_blf) == USB_E_OK )
    {
        cp = (USB_UTR_t*)p_blf; 
        cp->msghead = (USB_MH_t)NULL;
        cp->msginfo = (uint16_t)USB_GET_MAX_LUN;
        cp->keyword = (uint16_t)0u;
        cp->status  = (uint16_t)0u;
        cp->complete = (USB_CB_t)complete;  
        cp->ip = ptr->ip;
        cp->ipp = ptr->ipp;

        /* Send message */
        err = R_USB_SND_MSG( USB_PMSC_MBX, (USB_MSG_t*)p_blf );
        if( err != USB_E_OK )
        {
            /* Send message failure */
            err = R_USB_REL_BLK(USB_PMSC_MPL, (USB_MH_t)p_blf);
            USB_PRINTF0( "### Peripheral Get Max LUN request snd_msg error\n");
        }
    }
    else
    {
        /* Get memory block failure */
        USB_PRINTF0("### Peripheral Get Max LUN request pget_blk error\n");
    }
}   /* eof usb_pmsc_Cont() */

/******************************************************************************
End  Of File
******************************************************************************/

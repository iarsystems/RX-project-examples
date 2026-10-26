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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : r_media_driver_api.c
* Version      : 2.00
* Description  : This is the API interface between the application and
*                block media device drivers.
*******************************************************************************/
/*******************************************************************************
* History    : DD.MM.YYYY Version Description
*            : 09.11.2012 1.00         First Release
*            : 30.11.2012 2.00
*******************************************************************************/


/******************************************************************************
Section definitions
******************************************************************************/
#ifdef R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#endif

#ifdef RL78USB
#endif

#ifdef RL78USB_SECTION
#pragma section @@CODE   P_SMP
#pragma section @@BASE   T_SMP
#pragma section @@DATA   B_SMP
#endif


/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "r_media_driver_api.h"

/*******************************************************************************
global variables and functions
*******************************************************************************/
media_driver_t *g_MediaDriverList = USB_NULL;


/*******************************************************************************
Private Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Function Name: R_MEDIA_Initialize
* Description  : This function initializes data structures and variables that 
*                are used by the media device to support its operation for the 
*                first time.
* Arguments    : 
* Return value : result -
*                    MEDIA_RET_OK:      Success
*                    MEDIA_RET_PARERR:  Invalid parameter error.
*                    MEDIA_RET_OP_FAIL: Any other failures
*******************************************************************************/
BOOL_t R_MEDIA_Initialize(struct media_driver_s *p_media_driver)
{
    BOOL_t result;
    
    if (USB_NULL != p_media_driver)
    {
        g_MediaDriverList = p_media_driver;
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    return   result;
} /* End of function R_MEDIA_Initialize(). */

/*******************************************************************************
* Function Name: R_MEDIA_Open
* Description  : This function 
* Arguments    : none
* Return value : result -
*                    MEDIA_RET_OK:      Success
*                    MEDIA_RET_PARERR:  Invalid parameter error.
*                    MEDIA_RET_OP_FAIL: Any other failures
*******************************************************************************/
media_ret_t R_MEDIA_Open(void)
{
    media_ret_t result;

    result = g_MediaDriverList->pf_media_open();

    return   result;
} /* End of function R_MEDIA_Open(). */


/*******************************************************************************
* Function Name: R_MEDIA_Close
* Description  : This function 
* Arguments    : none
* Return value : result -
*                    MEDIA_RET_OK:      Success
*                    MEDIA_RET_PARERR:  Invalid parameter error.
*                    MEDIA_RET_OP_FAIL: Any other failures
*******************************************************************************/
media_ret_t R_MEDIA_Close(void)
{
    media_ret_t result;

    result = g_MediaDriverList->pf_media_close();

    return   result;
} /* End of function R_MEDIA_Close(). */


/*******************************************************************************
* Function Name: R_MEDIA_Read
* Description  : This function reads data from a specified location of the 
*                storage medium
* Arguments    : p_wbuffer -
*                    Pointer to the read data buffer.
*              : start_block -
*                    Start block number.
*              : block_count -
*                    Number of blocks to read.
* Return value : result -
*                    MEDIA_RET_OK:         Success
*                    MEDIA_RET_PARERR:  Invalid parameter error
*                    MEDIA_RET_RWERR:   Read/Write Error
*                    MEDIA_RET_NOTRDY:  Not Ready 
*                    MEDIA_RET_OP_FAIL: Any other failures
*******************************************************************************/
media_ret_t R_MEDIA_Read(uint8_t *p_rbuffer, uint32_t start_block, uint8_t  block_count)
{
    media_ret_t result;

    result = g_MediaDriverList->pf_media_read(p_rbuffer, start_block, block_count);

    return result;
} /* End of function R_MEDIA_Read(). */


/*******************************************************************************
* Function Name: R_MEDIA_Write
* Description  : This function writes data to a specified location of the 
*                storage medium
* Arguments    : p_wbuffer -
*                    Pointer to the write data buffer.
*              : start_block -
*                    Start block number.
*              : block_count -
*                    Number of blocks to write.
* Return value : result -
*                    MEDIA_RET_OK:      Success
*                    MEDIA_RET_PARERR:  Invalid parameter error
*                    MEDIA_RET_RWERR:   Read/Write Error
*                    MEDIA_RET_WRPRT:   Write Protected 
*                    MEDIA_RET_NOTRDY:  Not Ready 
*                    MEDIA_RET_OP_FAIL: Any other failures
*******************************************************************************/
media_ret_t R_MEDIA_Write(uint8_t *p_wbuffer, uint32_t start_block, uint8_t  block_count)
{
    media_ret_t result;

    result = g_MediaDriverList->pf_media_write(p_wbuffer, start_block, block_count);

    return result;
} /* End of function R_MEDIA_Write(). */


/*******************************************************************************
* Function Name: r_disk_ioctl
* Description  : This function provides a generalized means to pass special 
*                command and control instructions to the media driver and for
*                the driver to return information.
* Arguments    : ioctl_cmd -
*                    Control command code.
*              : ioctl_data -
*                    Data transfer buffer
* Return value : result -
*                   MEDIA_RET_OK:       Success
*                   MEDIA_RET_PARERR:   Invalid parameter error
*                   MEDIA_RET_NOTRDY:   Not Ready 
*                   MEDIA_RET_OP_FAIL:  Any other failures
*******************************************************************************/
media_ret_t R_MEDIA_Ioctl(ioctl_cmd_t ioctl_cmd, void *ioctl_data)
{
    media_ret_t result;

    result = g_MediaDriverList->pf_media_ctrl(ioctl_cmd, ioctl_data);

    return result;
} /* End of function R_MEDIA_Ioctl(). */


/******************************************************************************
End  Of File
******************************************************************************/

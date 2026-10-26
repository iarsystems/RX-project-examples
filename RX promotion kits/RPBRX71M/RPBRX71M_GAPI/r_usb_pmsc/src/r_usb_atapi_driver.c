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
* File Name    : r_usb_atapi_driver.c
* Description  : USB Host and Peripheral Interrupt code
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 04.01.2014 1.00 First Release
*         : 30.01.2015 1.01    Added RX71M.
*         : 30.09.2015 1.11    Added RX63N.
***********************************************************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include "r_usb_basic_if.h"
#include "r_usb_api.h"
#include "r_usb_pmsc_define.h"      /* Peri Mass Storage Class Driver define */
#include "r_usb_patapi_define.h"    /* Peripheral ATAPI Device extern define */

#include "r_media_driver_if.h"      /* Interface to block media driver routines. */
#include "r_usb_pmsc_config.h"
#include "r_usb_atapi_driver_config.h" /* Configuration file for this module. */

/*******************************************************************************
Section    <Section Difinition> , "Project Sections"
*******************************************************************************/
#ifdef USE_RAM

#pragma error "EWRX: Only tested with NOT_USE_MEDIA at the moment"

#endif    /* USE_RAM */

/*******************************************************************************
Constant macro definitions
*******************************************************************************/
/* Logical Block Unit Size */

/*******************************************************************************
External variables and functions
*******************************************************************************/
USB_ER_t usb_pmsc_SmpAtapiCloseMedia(void);

void     usb_pmsc_SmpAtapiAnalyzeCbwCb(uint8_t *cbwcb);
void     usb_pmsc_SmpAtapiTask(USB_VP_INT_t);
void     usb_pmsc_SmpAtapiCommandExecute(USB_UTR_t *ptr, uint8_t *cbw, uint16_t status, USB_CB_t complete);
 
static void     pmsc_atapi_get_read_data(uint32_t *size , uint8_t **buff); 
static void     pmsc_atapi_get_read_memory(uint32_t *size , uint8_t **buff);
static void     pmsc_atapi_write_data(uint32_t size);
static void     pmsc_atapi_get_write_memory(uint32_t *size , uint8_t **buff); 
                                          
static uint8_t  pmsc_atapi_request_search(uint8_t uc_OpCode, uint8_t uc_data);

extern USB_PMSC_CBM_t   usb_gpmsc_Message;
extern uint32_t         usb_gpmsc_Dtl;

/*******************************************************************************
Private global variables and functions
*******************************************************************************/
#ifdef NOT_USE_MEDIA
static media_ret_t dummy_disk_read(uint8_t lun, uint8_t* read_buffer, uint32_t sector, uint8_t sector_count);
static media_ret_t dummy_disk_write(uint8_t lun, uint8_t* write_buffer, uint32_t sector, uint8_t sector_count);
media_ret_t dummy_disk_open(uint8_t lun);
media_ret_t dummy_disk_close(uint8_t lun);
static media_ret_t dummy_disk_ioctl(uint8_t lun, uint8_t command_code, uint32_t* data_buffer);
#endif    /* NOT_USE_MEDIA */

uint8_t   usb_gpmsc_MediaArea[USB_ATAPI_BLOCK_UNIT];

/* Preinitialized data used to fill out responses to the host. These are data 
fields defined by the standard for the CBWs and CSWs. The categories are identi-
fied by the comments above each data field. They are part of the ATAPI protocol 
and explained within ATAPI spec. */
 uint8_t g_pmsc_atapi_rd_dat_tbl[] =
{
/* Request Sense data */
    /* Flags */
    0x70,0x00,0x05,0x00,
    0x00,0x00,0x00,0x0A,
    0x00,0x00,0x00,0x00,
    0x24,0x00,0x00,0x00,
    0x00,0x00,
/* Inquiry data */
    0x00,0x80,0x00,0x01,
    /* Flags */
    0x20,0x00,0x00,0x00,
    /* Vendor Information */
    'R','e','n','e','s','a','s',' ',
    /* Product Information */
    'M','a','s','s',' ','S','t','o','r','a','g','e',' ',' ',' ',' ',
    /* Product Revision Level */
    '1','.','0','0',        
/* Read Format Capacity data */
    /* Capacity List Header Length */
    0x00,0x00,0x00,0x10,
    /* Current/Maximum Capacity Descriptor.   */
//    0x00,0x00,0x00,0x00,0x02,0x00,0x02,0x00,
    /* Number of Blocks. */
    0x00,0x00,0x00,0x00,  
    /* Block Length in Bytes */  
    0x02,0x00,0x02,0x00,      
    /* Formattable Capacity Descriptor */
//    0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,
    /* Number of Blocks. */
    0x00,0x00,0x00,0x00,
    /* Block Length in Bytes */
    0x00,0x00,0x02,0x00,
/* Read Capacity data */
    /* Last Logical Block Address (default) */
    0x00,0x00,0x00,0x00,
    /* Default Block Length in Bytes */
    0x00,0x00,0x00,0x00,
    
/* Mode Sense data (Mode Parameter Header) */
    /* Mode Parameter Header (8byte) */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* Mode Sense data (ReadWrite Error Recovery Page 12byte) */
    /* Mode Parameter Header (8byte) */
    0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,
    /* ReadWrite Error Recovery Page (12byte) */
    0x01,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* Mode Sense data (Cashing Page 12byte) */
    /* Mode Parameter Header (8byte) */
    0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,
    /* Cashing Page (12byte) */
    0x08,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* Mode Sense data (Removable Block Access Capacities Page 12byte) */
    /* Mode Parameter Header (8byte) */
    0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,
    /* Removable Block Access Capacities Page (12byte) */
    0x1B,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* Mode Sense data (Timer & Protect Page) */
    /* Mode Parameter Header (8byte) */
    0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
    /* Timer & Protect Page (8byte) */
    0x1C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,
/* Mode Sense data (All Page) */
    /* Mode Parameter Header (8byte) */
    0x00,0x24,0x00,0x00,0x00,0x00,0x00,0x00,
    /* ReadWrite Error Recovery Page (12byte) */
    0x01,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    /* Cashing Page (12byte) */
    0x08,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    /* Removable Block Access Capacities Page (12byte) */
    0x1B,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/* Mode Sense data (Page code error) */
    /* Mode Parameter Header (8byte) */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

#ifdef NOT_USE_MEDIA
/* to store the filesystem driver function pointers */
struct media_driver_s g_DummyMediaDriver = 
{
    DUMMY_DISK_LUN,
    &dummy_disk_open,
    &dummy_disk_close,
    &dummy_disk_read,
    &dummy_disk_write,
    &dummy_disk_ioctl
};
#endif    /* NOT_USE_MEDIA */


/*******************************************************************************
Section    <Section Difinition> , "Project Sections"
*******************************************************************************/
/*
*CONVERTER REPLACED|REMOVED:
*#pragma section

*/

USB_PMSC_CDB_t          *g_pmsc_atapi_cbw_cb;
USB_UTR_t               g_pmsc_atapi_msg;    /* PMSC <--> ATAPI */

/* Lengths of data fields in the g_pmsc_atapi_rd_dat_tbl[] preinitialized data array. */
const uint16_t g_pmsc_atapi_data_size[] =
{
    0,
    18,     /* Request Sense data */
    36,     /* Inquiry data */
    20,     /* Read Format Capacity data */
    8,      /* Read Capacity data */
    8,      /* Mode Sense data (Mode Parameter Header) */
    20,     /* Mode Sense data (ReadWrite Error Recovery Page) */
    20,     /* Mode Sense data (Cashing Page) */
    20,     /* Mode Sense data (Removable Block Access Capacities Page) */
    8,      /* Optional command (not support) */
    44,     /* Mode Sense data (All Page) */
    8      /* Mode Sense data (Page code error) */
};

/* Byte offsets to data fields in the table below (g_pmsc_atapi_rd_dat_tbl[]). */
const uint16_t g_pmsc_atapi_rd_dat_idx[] =
{
    0,
    0,      /* Request Sense data */
    18,     /* Inquiry data */
    54,     /* Read Format Capacity data */
    74,     /* Read Capacity data */
    82,     /* Mode Sense data (Mode Parameter Header) */
    90,     /* Mode Sense data (ReadWrite Error Recovery Page) */
    110,    /* Mode Sense data (Cashing Page) */
    130,    /* Mode Sense data (Removable Block Access Capacities Page) */
    210,    /* Optional command (not support) */
    166,    /* Mode Sense data (All Page) */
    210    /* Mode Sense data (Page code error) */
};

/* ATAPI ommand codes. */
const uint8_t   g_pmsc_atapi_req_idx[] =
{
    5,      /* Total command number */
    0x03,   /* Request Sense data */
    0x12,   /* Inquiry data */
    0x23,   /* Read Format Capacity data */
    0x25,   /* Read Capacity data */
    0x5A,   /* Mode Sense data */
    0x00   /* Dummy data */
};

/*  Data SetUp Flag */
uint8_t     g_pmsc_atapi_setup_flag;

volatile uint8_t    g_cur_LUN = ATAPI_MEDIA_LUN;      /* The currently active Logical Unit Number. */ 
       
volatile uint32_t   g_cur_LBA;          /* the current Logical Block Address */

/*******************************************************************************
Renesas Abstracted ATAPI Driver API functions
*******************************************************************************/

/*******************************************************************************
* Function Name: usb_pmsc_SmpAtapiAnalyzeCbwCb
* Description  : Analyze ATAPI commands
* Arguments    : uint8_t *cbwcb            : CBW
* Return value : none
*******************************************************************************/
void usb_pmsc_SmpAtapiAnalyzeCbwCb(uint8_t *cbwcb)
{
    uint32_t    ar_rst;  /* Used to pass back result codes. */
    uint32_t    ar_size; /* Used to return the number of bytes requested to be transferred. */ 
    uint8_t     indx;

    ar_rst = USB_ATAPI_NOT_SUPPORT;
    ar_size = 0ul;
    g_pmsc_atapi_setup_flag = 0u;

    g_pmsc_atapi_cbw_cb = (USB_PMSC_CDB_t*)cbwcb;
    /* ATAPI Request Search */
    indx = pmsc_atapi_request_search(g_pmsc_atapi_cbw_cb->s_usb_ptn0.uc_OpCode, 
                                          g_pmsc_atapi_cbw_cb->s_usb_ptn0.uc_data);

    switch( g_pmsc_atapi_cbw_cb->s_usb_ptn0.uc_OpCode)
    {
        case USB_ATAPI_READ10:
            /* Set Request Parameter */
            /* Number of blocks requested. */
            ar_size = (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.us_Length_Hi << 8
                    | (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.us_Length_Lo;
                
            /* Convert block count to byte count. */
            ar_size = ar_size * (uint32_t)USB_ATAPI_BLOCK_UNIT;
        
            if (ar_size != 0ul)
            {
                ar_rst = USB_ATAPI_SND_DATAS;
            }
            else
            {
                ar_rst = USB_ATAPI_NO_DATA;
            }
        break;
    
        case USB_ATAPI_WRITE10:
        case USB_ATAPI_WRITE_AND_VERIFY:
            /* Set Request Parameter */
            /* Number of blocks requested. */            
            ar_size = (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.us_Length_Hi << 8
                       | (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.us_Length_Lo;
                       
            /* Convert block count to byte count. */
            ar_size = ar_size * (uint32_t)USB_ATAPI_BLOCK_UNIT;
            
            if (ar_size != 0ul)
            {
                ar_rst = USB_ATAPI_RCV_DATAS;
            }
            else
            {
                ar_rst = USB_ATAPI_NO_DATA;
            }
        break;

        case USB_ATAPI_TEST_UNIT_READY:
        case USB_ATAPI_PREVENT_ALLOW:
        case USB_ATAPI_START_STOP_UNIT:
        case USB_ATAPI_SEEK:
        case USB_ATAPI_VERIFY10:
            ar_rst = USB_ATAPI_NO_DATA;
            ar_size = 0ul;
        break;

        case USB_ATAPI_INQUIRY:
        case USB_ATAPI_REQUEST_SENSE:
            /* Set Request Parameter */
            ar_size = g_pmsc_atapi_cbw_cb->s_usb_ptn12.uc_Allocation;
            
            if (ar_size > g_pmsc_atapi_data_size[indx])
            {
                ar_size = g_pmsc_atapi_data_size[indx];
            }
            
            if ( ar_size != 0ul)
            {
                ar_rst = USB_ATAPI_SND_DATAS;
            }
            else
            {
                ar_rst = USB_ATAPI_NO_DATA;
            }
        break;
        
        case USB_ATAPI_MODE_SENSE10:
        case USB_ATAPI_READ_FORMAT_CAPACITY:
            /* Set Request Parameter */
            ar_size = (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.us_Length_Hi << 8
                    | (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.us_Length_Lo;
                    
            if (ar_size > g_pmsc_atapi_data_size[indx])
            {
                ar_size = g_pmsc_atapi_data_size[indx];
            }
            if (ar_size != 0ul)
            {
                ar_rst = USB_ATAPI_SND_DATAS;
            }
            else
            {
                ar_rst = USB_ATAPI_NO_DATA;
            }
        break;
        
        case USB_ATAPI_READ_CAPACITY:
            ar_rst  = USB_ATAPI_SND_DATAS;
            ar_size = g_pmsc_atapi_data_size[indx];
        break;

        case USB_ATAPI_MODE_SELECT10:
            /* Set Request Parameter */
            ar_size = (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.us_Length_Hi << 8
                    | (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.us_Length_Lo;
                    
            if (ar_size != 0ul)
            {
                ar_rst = USB_ATAPI_RCV_DATAS;
            }
            else
            {
                ar_rst = USB_ATAPI_NO_DATA;
            }
        break;
        
        case USB_ATAPI_FORMAT_UNIT:
            ar_rst = USB_ATAPI_RCV_DATAS;
            ar_size = g_pmsc_atapi_data_size[indx];
        break;

        default:
            USB_PRINTF0("### CBW Command Not Support\n");
        break;
    }

    usb_gpmsc_Message.ar_rst = ar_rst;
    
    /* Assign the current byte count requested by host in this cycle. */
    usb_gpmsc_Message.ul_size = ar_size; 
}
/* End of function usb_pmsc_SmpAtapiAnalyzeCbwCb(). */


/*******************************************************************************
* Function Name: usb_pmsc_SmpAtapiTask
* Description  : Execute ATAPI commands
* Arguments    : USB_VP_INT_t stacd        : start code
* Return value : none
*******************************************************************************/
void usb_pmsc_SmpAtapiTask(USB_VP_INT_t stacd)
{
    /* Transfer Address */
    static void  *atapi_rw_buff;
    /* The amount of real data transferred */
    static uint32_t    real_data_count;
    /* Transfer Size */
    static uint32_t    this_transfer_size;

    USB_UTR_t   *mess;
    uint32_t    residue_size;
    uint16_t    status;
    uint16_t    usb_result;
    USB_ER_t    err = (USB_ER_t)0u;

    err = R_USB_RCV_MSG(USB_PFLSH_MBX, (USB_MSG_t**)&mess);
    if (err != USB_E_OK)
    {
        /* USB_PRINTF1("pmsc_AtapiTask rcv_msg error (%ld)\n", err); */
        return;
    }

    g_pmsc_atapi_cbw_cb = (USB_PMSC_CDB_t*)(mess->tranadr);
    usb_result = mess->status;

    /* Data & Status Transfer */
    if (0 == g_pmsc_atapi_setup_flag)
    {   /* 1st Transfer Sequence */       
        switch( g_pmsc_atapi_cbw_cb->s_usb_ptn0.uc_OpCode)
        {
            /* Analyze opcode  */
            /* Send Data Commands (from Device to Host) */ 
            case USB_ATAPI_READ10:
            case USB_ATAPI_INQUIRY:
            case USB_ATAPI_REQUEST_SENSE:
            case USB_ATAPI_MODE_SENSE10:
            case USB_ATAPI_READ_FORMAT_CAPACITY:
            case USB_ATAPI_READ_CAPACITY:
                if (usb_gpmsc_Message.ul_size != 0ul)
                {
                    /* Returns with pointers to the expected data and the expected size. */
                    pmsc_atapi_get_read_data(&this_transfer_size, (uint8_t**)&atapi_rw_buff);
                    
                    /* If the expected size was larger than the read buffer (1 block), change the size to max of 1 block. */ 
                    if (this_transfer_size > USB_ATAPI_BLOCK_UNIT)                        
                    {   /* Divide Size for READ10 */                 
                        this_transfer_size = USB_ATAPI_BLOCK_UNIT;
                    }
                   
                    real_data_count = 0ul;  /* Clear this to initialize for new transfer cycle. */
                    status = (uint16_t)USB_PMSC_CMD_CONTINUE;
                    g_pmsc_atapi_setup_flag = 1;
                    
                }
                else
                {
                    status = (uint16_t)USB_PMSC_CMD_COMPLETE;
                    g_pmsc_atapi_setup_flag = 0;
                }
            break;
            
            /* Receive Data Commands (from Host to Device) */
            case USB_ATAPI_WRITE10:
            case USB_ATAPI_WRITE_AND_VERIFY:
            case USB_ATAPI_MODE_SELECT10:
            case USB_ATAPI_FORMAT_UNIT:           
                if (usb_gpmsc_Message.ul_size != 0ul)
                {
                    /* Retrieve the location and size of the write buffer. */
                    pmsc_atapi_get_write_memory(&this_transfer_size, (uint8_t**)&atapi_rw_buff);
                                                   
                    if (this_transfer_size > USB_ATAPI_BLOCK_UNIT)
                    {
                        /* Divide Size for WRITE10 & WRITE_AND_VERIFY*/                          
                        this_transfer_size = USB_ATAPI_BLOCK_UNIT;
                    }

                    real_data_count = 0ul;
                    status = (uint16_t)USB_PMSC_CMD_CONTINUE;
                    g_pmsc_atapi_setup_flag = 1u; // Flag that the command has been initialized.
                }
                else
                {
                    status = (uint16_t)USB_PMSC_CMD_COMPLETE;
                    g_pmsc_atapi_setup_flag = 0u;
                }
                
            break;
            
            /* No Send or Receive Data Commands */
            case USB_ATAPI_TEST_UNIT_READY:
            case USB_ATAPI_START_STOP_UNIT:
            case USB_ATAPI_SEEK:
            case USB_ATAPI_VERIFY10:
                status = (uint16_t)USB_PMSC_CMD_COMPLETE;
                g_pmsc_atapi_setup_flag = 0;
            break;

            case USB_ATAPI_PREVENT_ALLOW:
                status = (uint16_t)USB_PMSC_CMD_FAILED;
                g_pmsc_atapi_setup_flag = 0;
            break;
        
            default:
                status = (uint16_t)USB_PMSC_CMD_ERROR;
                USB_PRINTF0(
                    "### Command Execute error (1st sequence) \n");
                g_pmsc_atapi_setup_flag = 0;
            break;
        }
    }
    else
    {   /* Transfer Loop */
        switch (g_pmsc_atapi_cbw_cb->s_usb_ptn0.uc_OpCode)
        {   /* Analyze opcode  */
            /* Send Data Commands (from Device to Host) */ 
            case USB_ATAPI_INQUIRY:
            case USB_ATAPI_REQUEST_SENSE:
            case USB_ATAPI_MODE_SENSE10:
            case USB_ATAPI_READ_FORMAT_CAPACITY:
            case USB_ATAPI_READ_CAPACITY:
            case USB_ATAPI_READ10:
                if (USB_DATA_NONE == usb_result)
                {   /* Previous Transfer OK */
                    real_data_count += this_transfer_size;                
                    residue_size = usb_gpmsc_Message.ul_size - real_data_count;

                    if (0ul == residue_size)
                    {   /* All data in Device transferred */
                        if (usb_gpmsc_Dtl == usb_gpmsc_Message.ul_size)
                        {   /* case 6 */
                            status = (uint16_t)USB_PMSC_CMD_COMPLETE;
                        }
                        else
                        {   /* case 5 & 7*/
                            status = (uint16_t)USB_PMSC_CMD_FAILED;
                        }
                        usb_gpmsc_Dtl = usb_gpmsc_Dtl - usb_gpmsc_Message.ul_size;
                        g_pmsc_atapi_setup_flag = 0;
                    }
                    else
                    {   /* More data to transfer. */
                        pmsc_atapi_get_read_data(&this_transfer_size, (uint8_t**)&atapi_rw_buff);
                        
                        if (this_transfer_size > USB_ATAPI_BLOCK_UNIT)
                        {                       
                            this_transfer_size = USB_ATAPI_BLOCK_UNIT;
                        }                                                     

                        if( residue_size < this_transfer_size )
                        {
                            this_transfer_size = residue_size;
                        }
                        status = (uint16_t)USB_PMSC_CMD_CONTINUE;                              
                    }               
                }
                else
                {   /* Previous Transfer Fail */
                    status = (uint16_t)USB_PMSC_CMD_FAILED;
                    g_pmsc_atapi_setup_flag = 0;
                }
            break;
                
            /* Receive Data Commands (from Host to Device) */
            case USB_ATAPI_WRITE_AND_VERIFY:
            case USB_ATAPI_MODE_SELECT10:
            case USB_ATAPI_FORMAT_UNIT:
            case USB_ATAPI_WRITE10:
                if (USB_DATA_OK == usb_result)/* Previous Transfer OK */ 
                {                        
                    /* Deduct the amount transferred from the total requested. */
                    residue_size = usb_gpmsc_Message.ul_size - real_data_count;
                                                                                                               
                    if (0ul == residue_size)
                    {   /* All data in Device recieved */
                        if (usb_gpmsc_Dtl == usb_gpmsc_Message.ul_size)
                        {   /* case 12 */
                            status = (uint16_t)USB_PMSC_CMD_COMPLETE;
                        }
                        else
                        {   /* case 11 & 13*/
                            status = (uint16_t)USB_PMSC_CMD_FAILED;
                        }
                        
                        usb_gpmsc_Dtl = usb_gpmsc_Dtl - usb_gpmsc_Message.ul_size;
                        g_pmsc_atapi_setup_flag = 0;
                    }
                    else
                    {   /* Transfer Loop for USB_ATAPI_WRITE10 */
                        /* Write data is waiting in buffer. */                             

                        if (residue_size < this_transfer_size)
                        {
                            this_transfer_size = residue_size;                                
                        }
                        
                        /* Go do the actual media data transfer. */
                        pmsc_atapi_write_data(this_transfer_size);

                        /* Update the count of data transferred so far. */
                        real_data_count = real_data_count + this_transfer_size;
                        
                        if((usb_gpmsc_Message.ul_size - real_data_count) > 0)
                        {
                            status = (uint16_t)USB_PMSC_CMD_CONTINUE;
                        }
                        else
                        {   /* All data in Device recieved */
                            if (usb_gpmsc_Dtl == usb_gpmsc_Message.ul_size)
                            {   /* case 12 */
                                status = (uint16_t)USB_PMSC_CMD_COMPLETE;
                            }
                            else
                            {   /* case 11 & 13*/
                                status = (uint16_t)USB_PMSC_CMD_FAILED;
                            }
                        
                            usb_gpmsc_Dtl = usb_gpmsc_Dtl - usb_gpmsc_Message.ul_size;
                            g_pmsc_atapi_setup_flag = 0;
                        }                            
                    }
                }
                else
                {   /* Previous Transfer Fail */
                    status = (uint16_t)USB_PMSC_CMD_FAILED;
                    g_pmsc_atapi_setup_flag = 0;
                }
            break;
            
            /* No Send / Receive Data Commands  -> Error case */
            case USB_ATAPI_TEST_UNIT_READY:
            case USB_ATAPI_PREVENT_ALLOW:
            case USB_ATAPI_START_STOP_UNIT:
            case USB_ATAPI_SEEK:
            case USB_ATAPI_VERIFY10:
            default:
                status = (uint16_t)USB_PMSC_CMD_ERROR;
                USB_PRINTF0("### Command Execute error (Transfer Loop) \n");
                g_pmsc_atapi_setup_flag = 0;
            break;
        }
    }       
    /* Set mess members */
    mess->tranadr = atapi_rw_buff;
    mess->tranlen = this_transfer_size;
    mess->status  = status;

    /* CallBack function (SND_MSG) */
    (mess->complete)(mess,0,0);
}
/* End of function usb_pmsc_SmpAtapiTask(). */


/*******************************************************************************
* Function Name: pmsc_atapi_get_read_data
* Description  : Get read data
* Arguments    : uint32_t *size            : data size
*                : uint8_t **buff            : data address
* Return value : none
*******************************************************************************/
static void pmsc_atapi_get_read_data(uint32_t *size, uint8_t **buff)
{
    uint8_t indx;
    uint32_t num_blocks;  // Number of contiguous blocks in the media device.
    uint32_t block_length;  // The length in bytes of each logical block.
    
    indx = 0x00;

    if (USB_ATAPI_READ10 == g_pmsc_atapi_cbw_cb->s_usb_ptn0.uc_OpCode)
    {      
        /* Get the location and size of available read buffer. */      
        pmsc_atapi_get_read_memory(size, buff); /* TODO: make this function test for available memory. */ 
    
        if (0 == g_pmsc_atapi_setup_flag)
        {        
            /* Get the LBA from the command data block structure. It's in Big Endian format. */
            g_cur_LBA = (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.ul_LogicalBlock0 << 24 
                      | (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.ul_LogicalBlock1 << 16 
                      | (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.ul_LogicalBlock2 <<  8
                      | (uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.ul_LogicalBlock3;
        }              
        /* Read will be limited to max one block at a time. */                        
        R_MEDIA_Read(g_cur_LUN, &usb_gpmsc_MediaArea[0], g_cur_LBA, 1);      
        g_cur_LBA++;    /* Anticipate sequencial block reads. */         
    }
    else
    {
        /* Find the location in read message data table that corresponds to the current command. */
        indx = pmsc_atapi_request_search(g_pmsc_atapi_cbw_cb->s_usb_ptn0.uc_OpCode, 
                                              g_pmsc_atapi_cbw_cb->s_usb_ptn0.uc_data);

        /* Query the media device for total block count. */
        R_MEDIA_Ioctl(g_cur_LUN, MEDIA_IOCTL_GET_NUM_BLOCKS, &num_blocks); 
    
        /* Query the media device for block size in bytes. */    
        R_MEDIA_Ioctl(g_cur_LUN, MEDIA_IOCTL_GET_BLOCK_SIZE, &block_length);

        switch( g_pmsc_atapi_cbw_cb->s_usb_ptn0.uc_OpCode)
        {

            case USB_ATAPI_READ_FORMAT_CAPACITY: 
                /* Current/Maximum Capacity Descriptor */
                /* Total Number of blocks in storage media. */
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 4] = (num_blocks >> 24) & (uint8_t)0xFF;  //MSB
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 5] = (num_blocks >> 16) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 6] = (num_blocks >>  8) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 7] = num_blocks         & (uint8_t)0xFF;  //LSB
                 
                /* Descriptor Block Length field. */
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 8]  = 0; // reserved byte
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 9]  = (block_length >> 16) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 10] = (block_length >>  8) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 11] = block_length & (uint8_t)0xFF;  //LSB
 
              
                /* Formattable Capacity Descriptor */
                /* Total Number of blocks in storage media. */
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 12] = (num_blocks >> 24) & (uint8_t)0xFF;  //MSB
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 13] = (num_blocks >> 16) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 14] = (num_blocks >>  8) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 15] = num_blocks         & (uint8_t)0xFF;  //LSB
                
                /* Descriptor Block Length field. */ 
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 16] = 0; // reserved byte
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 17] = (block_length >> 16) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 18] = (block_length >>  8) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 19] = block_length & (uint8_t)0xFF;  //LSB
  
            break;
            
            case USB_ATAPI_READ_CAPACITY: /* READ CAPACITY Data. */
                /* Fill in READ CAPACITY Data Last Logical Block Address field. 0-based. */
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx]    ] 
                                        = ((num_blocks - 1ul) >> 24) & (uint8_t)0xFF;   //MSB
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 1] 
                                        = ((num_blocks - 1ul) >> 16) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 2]  
                                        = ((num_blocks - 1ul) >>  8) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 3]  
                                        =  (num_blocks - 1ul)        & (uint8_t)0xFF;  //LSB 

                /* Fill in READ CAPACITY Data Block Length field. */
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 4] 
                                        = (block_length >> 24) & (uint8_t)0xFF;   //MSB
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 5] 
                                        = (block_length >> 16) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 6]  
                                        = (block_length >>  8) & (uint8_t)0xFF;
                g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx] + 7]  
                                        =  block_length        & (uint8_t)0xFF;  //LSB 
                                       
            break;
            
            default:
            break;
        }
        
        /* Pass back the size information. */
        if (usb_gpmsc_Message.ul_size < g_pmsc_atapi_data_size[indx])
        {
            *size = usb_gpmsc_Message.ul_size;
        }
        else
        {
            *size = g_pmsc_atapi_data_size[indx];
        }
        *buff = &g_pmsc_atapi_rd_dat_tbl[g_pmsc_atapi_rd_dat_idx[indx]];
    }
}
/* End of function pmsc_atapi_get_read_data(). */


/*******************************************************************************
* Function Name: pmsc_atapi_get_read_memory
* Description  : Get pointer of Read data from device
* Arguments    : uint32_t *size            : data size
*                : uint8_t **buff            : data address
* Return value : none
*******************************************************************************/
static void pmsc_atapi_get_read_memory(uint32_t *size, uint8_t **buff)
{
    *size = usb_gpmsc_Message.ul_size;
    *buff = (uint8_t*)(&usb_gpmsc_MediaArea[0]);
}
/* End of function pmsc_atapi_get_read_memory(). */


/*******************************************************************************
* Function Name: pmsc_atapi_write_data
* Description  : Write the data to the device
* Arguments    : uint32_t *size            : data size
*                : uint8_t **buff            : data address
* Return value : none
*******************************************************************************/
static void pmsc_atapi_write_data(uint32_t size)
{             
    /* Write will be limited to max one block at a time */
    R_MEDIA_Write(g_cur_LUN, &usb_gpmsc_MediaArea[0], g_cur_LBA, 1);
      
    g_cur_LBA++;  // anticipate sequential write
}
/* End of function pmsc_atapi_write_data(). */


/*******************************************************************************
* Function Name: pmsc_atapi_get_write_memory
* Description  : Get pointer of write data to device
* Arguments    : size -
*                    Pointer to variable specifying the number of bytes of data
*              : buff -          
*                    Pointer to pointer to the data starting address
* Return value : none
*******************************************************************************/
static void pmsc_atapi_get_write_memory(uint32_t *size, uint8_t **buff)
{
    /* Get the LBA from the command data block structure. It's in Big Endian format. */
    g_cur_LBA = ((uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.ul_LogicalBlock0 << 24) 
              | ((uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.ul_LogicalBlock1 << 16) 
              | ((uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.ul_LogicalBlock2 <<  8)
              | ((uint32_t)g_pmsc_atapi_cbw_cb->s_usb_ptn4569.ul_LogicalBlock3);

    *size = usb_gpmsc_Message.ul_size;
    *buff = &usb_gpmsc_MediaArea[0];
}
/* End of function pmsc_atapi_get_write_memory(). */


/*******************************************************************************
* Function Name: usb_pmsc_SmpAtapiCloseMedia
* Description  : Media Driver close
* Arguments    : none
* Return value : USB_ER_t                  : error
*******************************************************************************/
USB_ER_t usb_pmsc_SmpAtapiCloseMedia(void)
{
    return USB_E_OK;
}
/* End of function usb_pmsc_SmpAtapiCloseMedia(). */


/*******************************************************************************
* Function Name: usb_pmsc_SmpAtapiCommandExecute
* Description  : ATAPI Command Transfer
* Arguments    : uint8_t *cbw              : CBW
*              : uint16_t status           : status
*              : USB_CB_t complete         : callback
* Return value : USB_ER_t                  : error
*******************************************************************************/
void usb_pmsc_SmpAtapiCommandExecute(USB_UTR_t *ptr, 
                                     uint8_t *cbw, 
                                     uint16_t status, 
                                     USB_CB_t complete)
{
    USB_ER_t    err = USB_ERROR;

    /* Command Transfer set */
    g_pmsc_atapi_msg.msghead     = (USB_MH_t)NULL;
    g_pmsc_atapi_msg.tranadr     = cbw;
    g_pmsc_atapi_msg.status      = status;
    g_pmsc_atapi_msg.complete    = complete;
    g_pmsc_atapi_msg.ip  = ptr->ip;
    g_pmsc_atapi_msg.ipp = ptr->ipp;

    err = R_USB_SND_MSG(USB_PFLSH_MBX, (USB_MSG_t*)&g_pmsc_atapi_msg);
    if( err != USB_E_OK )
    {
        /* Send message failure */
        USB_PRINTF1("### pmsc_AtapiCommandExecute snd_msg error (%ld)\n"
            , err);
    }
}
/* End of function usb_pmsc_SmpAtapiCommandExecute(). */


/*******************************************************************************
* Function Name: pmsc_atapi_request_search
* Description  : ATAPI Request Search
* Arguments    : uint8_t op_code           : CBW
*              : uint8_t uc_data           : status
* Return value : uint8_t                   : index
*******************************************************************************/
static uint8_t pmsc_atapi_request_search(uint8_t op_code, uint8_t uc_data)
{
    uint8_t     indx;

    for( indx = g_pmsc_atapi_req_idx[0]; (indx != 0)
        && (g_pmsc_atapi_req_idx[indx] != op_code); indx--)
    {
        /* do nothing */
    }

    if (USB_ATAPI_MODE_SENSE10 == op_code)
    {
        switch( uc_data & (uint8_t)(0x3F) )
        {   /* Page code check */
        /* Mode Parameter Header */
        case 0x00:              break;
        /* ReadWrite Error Recovery Page */
        case 0x01:  indx += 1;  break;
        /* Cashing Page */
        case 0x08:  indx += 2;  break;
        /* Removable Block Access Capacities Page */
        case 0x1B:  indx += 3;  break;
        /* Timer & Protect Page */
        case 0x1C:  indx += 4;  break;
        /* All Page */
        case 0x3F:  indx += 5;  break;
        /* Page code error */
        default:    indx += 6;  break;
        }
    }
    return indx;
}
/* End of function pmsc_atapi_request_search(). */

#ifdef NOT_USE_MEDIA
/*******************************************************************************
* Function Name: dummy_disk_read
* Description  :  
* Argument     :     (uint8_t lun, the logical unit (device number) to read from
*                    uint8_t* read_buffer,     destination buffer for read data
*                    uint32_t sector,         the starting sector number
*                    uint8_t sector_count)    number of sectors to be read
*
* Return value :     R_RES_OK  = 0     Successful
*                    R_RES_ERROR = 1  R/W Error
* Note:    This function does not check for destination buffer overrun.
*          Make absolutely sure that enough memory has been allocated to accept 
*          the data that will be read. This is especially important when the 
*          sector count is greater than 1, because typically only 1 sector is 
*          read at a time.
*******************************************************************************/
static media_ret_t dummy_disk_read(uint8_t lun, 
                                  uint8_t* read_buffer, 
                                  uint32_t sector, 
                                  uint8_t sector_count)
{
    return MEDIA_RET_OK;
} /* end dummy_disk_read */


/*******************************************************************************
* Function Name: dummy_disk_write
* Description  :  
* Argument     :    uint8_t lun,  the logical unit (device number) to write to
*                    const uint8_t* write_buffer,  source for data to be written from 
*                    uint32_t sector,         the starting sector number
*                    uint8_t sector_count     number of sectors to be written
*                     
* Return value :     R_RES_OK  = 0     Successful
*                    R_RES_ERROR = 1  R/W Error
*******************************************************************************/
static media_ret_t dummy_disk_write(uint8_t lun, 
                                   uint8_t* write_buffer, 
                                   uint32_t sector, 
                                   uint8_t sector_count)
{
    return MEDIA_RET_OK;
} /* end dummy_disk_write */


/*******************************************************************************
* Function Name: dummy_disk_open
* Description  :  
* Argument     : (uint8_t lun)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/
media_ret_t dummy_disk_open(uint8_t lun)
{
    // TODO: actually open the device if possible    
    return     MEDIA_RET_OK;
} /* end dummy_disk_initialize */

/*******************************************************************************
* Function Name: dummy_disk_close
* Description  :  
* Argument     : (uint8_t lun)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/
media_ret_t dummy_disk_close(uint8_t lun)
{
    // TODO: actually close the device if possible
    return MEDIA_RET_OK;
} /* end dummy_disk_initialize */

/*******************************************************************************
* Function Name: dummy_disk_ioctl
* Description  :  
* Argument     : (uint8_t lun, uint8_t command_code, uint32_t* data_buffer)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/
static media_ret_t dummy_disk_ioctl(uint8_t lun, 
                                   uint8_t command_code, 
                                   uint32_t* data_buffer)
{
    return MEDIA_RET_OK;
} /* end dummy_disk_ioctl */

#endif    /* NOT_USE_MEDIA */

/*******************************************************************************
End  Of File
*******************************************************************************/

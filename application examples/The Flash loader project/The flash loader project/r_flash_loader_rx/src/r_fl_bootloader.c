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
* File Name    : r_fl_bootloader.c
* Version      : 3.00
* Description  : Bootloader to run out of User Boot Area.  Part of the
*                FlashLoader project.
******************************************************************************/  
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 05.04.2010 1.00    First Release
*         : 08.04.2010 1.01    Added bypass checking and added User
*                              Boot reset vector
*         : 22.03.2011 2.00    First Release for YRDK
*         : 02.03.2012 3.00    Made CS v4.0 compliant. Now uses r_cmt_rx 
*                              package to trigger the FL state machine. Fixed
*                              bugs found in fl_process_write_buffer() 
*                              function.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines intrinsic functions of the MCU */
#include <machine.h>
/* Prototype for memcpy() */
#include <string.h>
/* Used for fixed-width types. */
#include <stdint.h>
/* Used for booleans. */
#include <stdbool.h>
/* r_bsp Support. */
#include "platform.h"
/* Flash Loader project includes. */
#include "r_fl_includes.h"
/* Function prototypes for erasing/programming MCU flash */
#include "r_flash_api_rx_if.h"
/* Create timer to call Flash Loader state machine. */
#include "r_cmt_rx_if.h"
/* Used for CRC calculations. */
#include "r_crc_rx_if.h"

/******************************************************************************
Macro definitions
******************************************************************************/
/* Reset vector */
#define MCU_RESET_VECTOR        (0xFFFFFFFC)
#define JUMP_TO_APPLICATION     ((void (*)(void))*((uint32_t *)MCU_RESET_VECTOR))

/* Frequency at which to call the Flash Loader state machine. */
#define FL_CMT_FREQUENCY        (20)

/******************************************************************************
Private global variables and functions
******************************************************************************/
static bool fl_write_new_image(uint8_t image_index);
static bool fl_process_write_buffer(uint32_t address, uint8_t * data, uint32_t bytes);
static bool fl_flush_write_buffer(void);
static void fl_trigger_sm(void * pdata);

/* Holds current write address in MCU's flash */
static uint32_t g_fl_flash_write_addr;
/* Used for holding program page for writing to MCU's flash */
static uint8_t  g_fl_flash_write_buffer[ROM_PROGRAM_SIZE];
/* Current position in g_fl_flash_write_buffer */
static uint32_t g_fl_write_buffer_index;
/* Points to info on current application */
static fl_image_header_t * g_pfl_cur_app_header;
/* Flag that tells the state machine to process. */
static bool     g_sm_process;

#if defined(__IAR_SYSTEMS_ICC__)
#pragma section = "APPHEADER_1"
#pragma location = "APPHEADER_1"
__no_init __root const fl_image_header_t g_fl_cur_app_header;
#endif

/******************************************************************************
* Function Name: main
* Description  : The main loop for the bootloader. This function will check to
*                see if any new images are available. If a valid image is found
*                then it will be programmed in to the User Application space.
*                It will then jump to the newly programmed application. If a 
*                image is not found then the function will start the 
*                Downloader state machine and wait for a new image.
* Arguments    : none
* Return Value : none
******************************************************************************/
void main(void)
{    
    /* Index of load image to use */
    int32_t  image_to_load;
    /* Which CMT channel is used for state machine. */
    uint32_t cmt_channel;
    
    /* If a request to bypass the bootloader has been made then skip
       checking and wait for a new image */
    if(fl_check_bootloader_bypass() == false)
    {
        /* Bypass not requested, perform normal checking */
       
        /* Initialize to -1, no valid image */
        image_to_load = -1;
    
        /* Initialize resources needed for using external memory */
        fl_mem_init();
        
        /* Initialize CRC code. */
        R_CRC_Init();
       
        /* Initialize pointer to current app's load image header */
#if defined(__IAR_SYSTEMS_ICC__)
        g_pfl_cur_app_header = (fl_image_header_t *)__section_begin("APPHEADER_1");
#else        
        g_pfl_cur_app_header = (fl_image_header_t *)__sectop("APPHEADER_1");     
#endif    
        /* Get info on any load images that are stored.  Put this data in
           g_fl_load_image_headers[] */
        fl_get_load_image_headers();           
    
        /* Get last downloaded image to use */
        image_to_load = fl_get_latest_image();
    
        /* Was a valid load image header found? */
        if(image_to_load == -1)
        {
            /* No valid image found in external memory */
            /* Check to see if valid image header is already in MCU flash */
            if(g_pfl_cur_app_header->valid_mask == FL_LI_VALID_MASK)
            {
                /* Valid image header in MCU flash, validate the whole image */
                if( fl_check_application() == g_pfl_cur_app_header->raw_crc )
                {               
                    /* Valid image in MCU flash, jump to it */            
                    JUMP_TO_APPLICATION();
                }
                /* Else, the image was not successfully validated, wait for new
                   load image */
            }     
        }
        else
        {
            /* Check to see if there is a current image in MCU flash */
            if( (g_pfl_cur_app_header->valid_mask == FL_LI_VALID_MASK) &&
                (g_pfl_cur_app_header->image_ID == g_fl_load_image_headers[image_to_load].image_ID) &&
                (g_pfl_cur_app_header->version_num == g_fl_load_image_headers[image_to_load].version_num) )
            {
                /* Valid image was found but it is same as the one already in flash.
                   Check to make sure image in MCU flash is valid, if so then jump
                   to it.  If not, then program in the load image. */ 
                if( fl_check_application() == g_pfl_cur_app_header->raw_crc )
                {               
                    /* Valid image in MCU flash, jump to it */            
                    JUMP_TO_APPLICATION();
                }           
            }
                
            /* Verify load image is complete and error free */
            if( fl_verify_load_image((uint32_t)image_to_load) == true )
            {                
                /* Load image is valid, program in new image */
                fl_write_new_image((uint8_t)image_to_load);
            }
                
            /* Verify image in MCU flash and jump to it */        
            if( fl_check_application() == g_pfl_cur_app_header->raw_crc )
            {               
                /* Valid image in MCU flash, jump to it */            
                JUMP_TO_APPLICATION();
            }
        }
    }
    /* Either bypass was requested or a valid image was not found */
    
    /* Signal to outside world that we are waiting on a new image */
    fl_signal();
        
    /* No valid images were found, setup communications and 
       wait for a new image to come in */
    R_FL_DownloaderInit();
    
    /* Initialize state machine process flag. */
    g_sm_process = false;

    /* Create periodic timer to drive Flash Loader state machine. */
    R_CMT_CreatePeriodic(FL_CMT_FREQUENCY, fl_trigger_sm, &cmt_channel);
    
    while(1) 
    {
        /* Call state machine after flag has been set. */
        if (true == g_sm_process)
        {
            /* Trigger state machine. */
            R_FL_StateMachine();

            g_sm_process = false;
        }
    }       
}

/******************************************************************************
* Function Name: fl_write_new_image
* Description  : Write load image into MCU flash.
* Arguments    : image_index - 
*                    Which load image to use
* Return value : true - 
*                    Image programmed successfully
*                false - 
*                    Error occurred
******************************************************************************/
static bool fl_write_new_image(uint8_t image_index)
{
    uint32_t i;
    bool check;
    uint8_t ret;
    uint32_t cur_spi_address;
    uint32_t bytes_read;
    fl_block_header_t * bheader;
    
#ifdef FLASH_API_RX_CFG_COPY_CODE_BY_API
    /* Before calling any other Flash API functions the API code needs to be 
       copied to RAM. This can still be done the 'old way' by editting dbsct.c
       if desired. */
    R_FlashCodeCopy();
#endif
    
    /* Start off by erasing flash */
    for(i = 0; i < ROM_NUM_BLOCKS; i++)
    {
        /* Erase Block 'i' */
        ret = R_FlashErase((uint8_t)i);
        
        /* Check for errors */
        if( ret != FLASH_SUCCESS )
        {
            return false;
        }
    }
    
    /* Now we can program flash */
                
    /* Get location of first load block in memory */
    cur_spi_address = g_fl_load_image_headers[image_index].start_address;
    
    /* Initialize bytes_read. */
    bytes_read = sizeof(fl_image_header_t);   
    
    /* Initialize buffer variables */
    g_fl_flash_write_addr = 0;
    g_fl_write_buffer_index = 0;
        
    /* We have to check off of bytes read.  We cannot check the address
       because the MCU might have aligned blocks to erase boundaries so
       the memory address will not work.  Also you have to remember to
       make the 'next_block_address' field in block headers all 0xFF's 
       since that is what was used when the image CRC was calculated. */
    while( bytes_read < g_fl_load_image_headers[image_index].load_image_size)
    {
        /* Read block header */
        fl_mem_read(cur_spi_address, &g_fl_rx_buffer[0], sizeof(fl_block_header_t));
        
        bheader = (fl_block_header_t *)&g_fl_rx_buffer[0];
        
        /* Move cur_spi_address to block's data */
        cur_spi_address += sizeof(fl_block_header_t);
        
        /* Read block data */
        fl_mem_read(cur_spi_address, &g_fl_rx_buffer[sizeof(fl_block_header_t)], (uint16_t)bheader->data_size);
        
        /* Update bytes_read */
        bytes_read += sizeof(fl_block_header_t) + bheader->data_size;
        
        /* Process block's data */
        check = fl_process_write_buffer( bheader->flash_address,
                                       &g_fl_rx_buffer[sizeof(fl_block_header_t)], 
                                       bheader->data_size);
                                       
        /* Check for errors */
        if( check != true )
        {
            return false;
        }                               
        
        /* Update address for next block */
        cur_spi_address = bheader->next_block_address;        
    }
    
    /* Flush out last buffer */
    fl_flush_write_buffer();
    
    return true;
}
/******************************************************************************
End of function fl_write_new_image
******************************************************************************/

/******************************************************************************
* Function Name: fl_process_write_buffer
* Description  : Takes block data and writes it to the MCU flash.  Since
*                program pages can span blocks, to flush the buffer in 
*                the end, fl_flush_write_buffer needs to be called.
* Arguments    : address - 
*                    Address of where this data goes in flash
*                data - 
*                    Pointer to data
*                bytes - 
*                    Number of bytes of data
* Return value : true - 
*                    Data processed successfully
*                false - 
*                    Error occurred
******************************************************************************/
static bool fl_process_write_buffer(uint32_t address, uint8_t * data, uint32_t bytes)
{
    uint32_t bytes_to_write;
    bool check;
    uint8_t ret;
    
    /* Check to see if this data will be in the same programming page
       as the last input data, or if a flush is needed. */
    if( g_fl_flash_write_addr == 0 ||
        (address < 
        (((uint64_t)g_fl_flash_write_addr + ROM_PROGRAM_SIZE) & 
        (~(ROM_PROGRAM_SIZE - 1)))) )
    {
        /* This data can continue on with the previous buffer */
        /* Fill in any space between this buffer and the last with 0xFF's */
        for(; g_fl_write_buffer_index < (address & (ROM_PROGRAM_SIZE-1));
            g_fl_write_buffer_index++)
        {
            g_fl_flash_write_buffer[g_fl_write_buffer_index] = 0xFF;
        }
        
        /* Set flash address */
        g_fl_flash_write_addr = address;        
    }
    else
    {
        /* This data will start a new buffer */
        
        /* Flush previous buffer */
        check = fl_flush_write_buffer();
        
        /* Check for errors */
        if( check != true )
        {
            return false;
        }
                      
        /* Start new buffer */
        g_fl_flash_write_addr = address;
        
        /* If buffer does not start on program boundary, then fill
           empty space */
        for(g_fl_write_buffer_index = 0; //do nothing
            g_fl_write_buffer_index < (address & (ROM_PROGRAM_SIZE-1));
            g_fl_write_buffer_index++)
        {
            g_fl_flash_write_buffer[g_fl_write_buffer_index] = 0xFF;
        }
    }
        
    /* Process buffer while there are still bytes to process */
    while(bytes > 0)
    {
    
        /* Either fill buffer if there are enough bytes left, 
           or just use what is left */
        bytes_to_write = (uint32_t)min((int32_t)bytes, (int32_t)(ROM_PROGRAM_SIZE - g_fl_write_buffer_index));       
    
        /* Fill buffer with input data */
        memcpy(&g_fl_flash_write_buffer[g_fl_write_buffer_index], data, bytes_to_write);
    
        /* Update buffer index */
        g_fl_write_buffer_index += bytes_to_write;
        
        /* Update bytes left in input buffer, and data pointer */
        bytes -= bytes_to_write;
        data += bytes_to_write;
        
        /* Write buffer is full, flush it */
        if(g_fl_write_buffer_index >= ROM_PROGRAM_SIZE)
        {
            /* Write buffer */
            ret = R_FlashWrite( g_fl_flash_write_addr & (~(ROM_PROGRAM_SIZE-1)),
                                  (uint32_t)&g_fl_flash_write_buffer[0],
                                  ROM_PROGRAM_SIZE);
                          
            /* Check for errors */
            if( ret != FLASH_SUCCESS )
            {
                return false;
            }                           
                          
            /* Re-init write index */                              
            g_fl_write_buffer_index = 0;                              
        }
        
        /* Update flash write address */
        g_fl_flash_write_addr += bytes_to_write;
    }
    
    return true;
}
/******************************************************************************
End of function fl_process_write_buffer
******************************************************************************/

/******************************************************************************
* Function Name: fl_flush_write_buffer
* Description  : Write last buffer to flash, if needed
* Arguments    : none
* Return value : true - 
*                    Successful
*                false - 
*                    Not Successful
******************************************************************************/
static bool fl_flush_write_buffer(void)
{
    uint8_t check;
    
    /* If write buffer index is 0 then there is no reason to flush */
    if(g_fl_write_buffer_index != 0)
    {
        /* Fill in any empty areas with 0xFF's */
        for(; g_fl_write_buffer_index < ROM_PROGRAM_SIZE;
            g_fl_write_buffer_index++)
        {
            g_fl_flash_write_buffer[g_fl_write_buffer_index] = 0xFF;
        }
        
        /* Write buffer */
        check = R_FlashWrite( g_fl_flash_write_addr & (~(ROM_PROGRAM_SIZE-1)),
                              (uint32_t)&g_fl_flash_write_buffer[0],
                              ROM_PROGRAM_SIZE);

        /* Check for errors */
        if( check != FLASH_SUCCESS )
        {
            return false;
        } 
    }
    
    return true;
}
/******************************************************************************
End of function fl_flush_write_buffer
******************************************************************************/

/******************************************************************************
* Function Name: fl_trigger_sm
* Description  : Sets flag to tell state machine to process again.
* Arguments    : channel -
*                    Which CMT channel this callback is for.
* Return value : none
******************************************************************************/
static void fl_trigger_sm (void * pdata)
{  
    /* Create periodic timer to call Flash Loader state machine. */
    g_sm_process = true;
}
/******************************************************************************
End of function fl_trigger_sm
******************************************************************************/


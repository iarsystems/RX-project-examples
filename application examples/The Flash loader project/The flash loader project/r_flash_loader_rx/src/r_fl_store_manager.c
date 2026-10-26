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
* File Name    : r_fl_store_manager.c
* Version      : 3.00
* Description  : High level memory management is done here.  This file will
*                not change unless you want to change file operations.
******************************************************************************/  
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 05.04.2010 1.00    First Release
*         : 22.03.2011 2.00    First Release for YRDK
*         : 23.02.2012 3.00    Made compliant with CS v4.0. Fixed bug in 
*                              fl_store_block_init() function when data member
*                              spanned programming pages.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines standard macros used in this file */
#include <stddef.h>
/* Defines intrinsic functions of the MCU */
#include <machine.h>
/* Fixed width types support. */
#include <stdint.h>
/* Used for bool. */
#include <stdbool.h>

/* Flash Loader project includes. */
#include "r_fl_includes.h"
/* Uses r_crc_rx package for CRC calculations. */
#include "r_crc_rx_if.h"

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Which file block is being used for storage */
static uint8_t     g_fl_store_block_num;
/* Current write address in external memory */
static uint32_t    g_fl_cur_store_addr;
/* Previous block record address in external memory */
static uint32_t    g_fl_prev_store_addr;
/* Address used to delete sectors */
static uint32_t    g_fl_cur_delete_addr;
/* Address of data to write to SPI flash */
static uint8_t *   g_fl_cur_data_addr;
/* Number of bytes of data to write to SPI flash */
static uint32_t    g_fl_cur_data_bytes;

/******************************************************************************
* Function Name: fl_store_init
* Description  : Finds open location in external memory to store new 
*                load image and initializes store variables
* Arguments    : none
* Return value : true - 
*                    Block found
*                false - 
*                    No empty block found
******************************************************************************/
bool fl_store_init(void)
{
    uint32_t i;
    
    /* Find open file block in external memory */
    for(i = 0; i < FL_CFG_MEM_NUM_LOAD_IMAGES; i++)
    {
        if(g_fl_load_image_headers[i].valid_mask != FL_LI_VALID_MASK)
        {
            /* Found open block to use */
            g_fl_store_block_num = i;
            
            /* Set address to start from */
            g_fl_cur_store_addr = g_fl_li_mem_info.addresses[i];
            
            /* Init g_fl_prev_store_addr */
            g_fl_prev_store_addr = g_fl_cur_store_addr;               
                        
            /* Delete external memory section that is going to be used */
            fl_start_erase_load_block(g_fl_store_block_num);
                        
            return true;
        }
    }
    
    /* If you get here then all blocks were taken so return an error */
    return false;    
}
/******************************************************************************
End of function fl_store_init
******************************************************************************/


/******************************************************************************
* Function Name: fl_store_image_header
* Description  : Writes image header into memory
* Arguments    : fl_image_pointer -
*                    Pointer to Load Image Header
* Return value : none
******************************************************************************/
void fl_store_image_header(fl_image_header_t *fl_image_pointer)
{
    /* Update start_address in image */
    fl_image_pointer->start_address = g_fl_cur_store_addr + sizeof(fl_image_header_t);
        
    /* Store Load Image Header at beginning of block */
    fl_store_block_init((uint8_t *)fl_image_pointer, sizeof(fl_image_header_t));
        
    while( fl_store_block_continue() == true ) 
    {
        /* Should only take 1 function call to finish */
    }
    
    /* Init g_fl_prev_store_addr */
    g_fl_prev_store_addr = g_fl_cur_store_addr;    
}
/******************************************************************************
End of function fl_store_image_header
******************************************************************************/


/******************************************************************************
* Function Name: fl_start_erase_load_block
* Description  : Starts the erasure of a Load Block section
* Arguments    : block -
*                    Load Block section to erase
* Return value : true - 
*                    Completed successfully
*                false - 
*                    Invalid block number
******************************************************************************/
bool fl_start_erase_load_block(uint8_t block)
{
    g_fl_store_block_num = block;
    
    /* Check to make sure block is in range */
    if(g_fl_store_block_num >= FL_CFG_MEM_NUM_LOAD_IMAGES)
    {
        /* Error, should not a valid load image */
        return false;
    }
    
    /* Set g_fl_cur_delete_addr at the beginning of the section */
    g_fl_cur_delete_addr = g_fl_li_mem_info.addresses[g_fl_store_block_num];   
    
    return true;
}
/******************************************************************************
End of function fl_start_erase_load_block
******************************************************************************/

/******************************************************************************
* Function Name: fl_continue_erase_load_block
* Description  : Continues and finishes the erasure of a Load Block section
* Arguments    : none
* Return value : true - 
*                    More to erase
*                false - 
*                    Erase is complete
******************************************************************************/
bool fl_continue_erase_load_block(void)
{
    /* Only continue if memory is not busy */
    if( fl_mem_get_busy() != 0 )
    {
        /* Memory is still busy, come back later */
        return true;
    }
    
    /* This while will work on the top most block because the g_fl_li_mem_info.addresses
       array has a top entry that represents the top of the external memory. */
    if(g_fl_cur_delete_addr < g_fl_li_mem_info.addresses[g_fl_store_block_num + 1])
    {        
        /* Erase current sector */
        fl_mem_erase(g_fl_cur_delete_addr, FL_MEM_ERASE_SECTOR);
        
        /* Update erase address to next sector */
        g_fl_cur_delete_addr += g_fl_li_mem_info.erase_size;
    }
    else
    {
        return false;
    }      
    
    return true;
}
/******************************************************************************
End of function fl_continue_erase_load_block
******************************************************************************/

/******************************************************************************
* Function Name: fl_is_store_busy
* Description  : Returns whether the memory is busy or not
* Arguments    : none
* Return value : true - 
*                    The memory is busy
*                false - 
*                    The memory is not busy
******************************************************************************/
bool fl_is_store_busy(void)
{
    /* Return whether memory is busy or not */
    return fl_mem_get_busy();
}
/******************************************************************************
End of function fl_is_store_busy
******************************************************************************/

/******************************************************************************
* Function Name: fl_store_retry_init
* Description  : Starts the process of finding the last valid block in a 
*                corrupt load image.  This function also initializes 
*                variables needed for storing data later.
* Arguments    : image_index - 
*                    Which block to test
* Return value : none
******************************************************************************/
void fl_store_retry_init(uint32_t image_index)
{    
    /* No need to search for open block since this is retry */
    g_fl_store_block_num = image_index;
        
    /* Get location of first block in external memory */
    g_fl_cur_store_addr = g_fl_load_image_headers[image_index].start_address;
    
    /* Init previous address */
    g_fl_prev_store_addr = g_fl_cur_store_addr;
}
/******************************************************************************
End of function fl_store_retry_init
******************************************************************************/

/******************************************************************************
* Function Name: fl_store_retry_continue
* Description  : Find last valid block in corrupt load image.  
* Arguments    : seq_id - 
*                    Pointer to return current sequence ID
* Return value : true - 
*                    This block was good, keep going
*                false - 
*                    This block was bad, use previous
******************************************************************************/
bool fl_store_retry_continue(uint16_t * seq_id)    
{    
    fl_block_header_t * bheader;
    uint16_t            crc_value;
        
    /* Read block header */
    fl_mem_read(g_fl_cur_store_addr, &g_fl_rx_buffer[0], sizeof(fl_block_header_t));

    bheader = (fl_block_header_t *)&g_fl_rx_buffer[0];

    /* Check valid mask */
    if(bheader->valid_mask != FL_BH_VALID_MASK)
    {
        /* This block is invalid */
        return false;
    }

    /* Check sequence ID */
    if(bheader->sequence_ID != *seq_id)
    {
        /* This block is invalid */
        return false;
    }

    /* Read data portion and check against CRC */
    fl_mem_read(g_fl_cur_store_addr+sizeof(fl_block_header_t), 
                &g_fl_rx_buffer[sizeof(fl_block_header_t)], 
                bheader->data_size);          

    /* Compute CRC-16 on data and check against stored CRC */
    R_CRC_Compute( FL_CRC_SEED, 
                   &g_fl_rx_buffer[sizeof(fl_block_header_t)], 
                   bheader->data_size,
                   &crc_value);
    
    if (crc_value != bheader->data_crc)                        
    {
        /* This block is invalid */
        return false;
    }
    
    /* Update address for next block */
    //g_fl_cur_store_addr += sizeof(fl_block_header_t) + bheader->data_size;
    if(bheader->next_block_address == 0xFFFFFFFF)
    {
        /* This block is invalid */
        return false;
    }
    
    /* Store previous address */
    g_fl_prev_store_addr = g_fl_cur_store_addr;
    
    /* Next block address appears valid */
    g_fl_cur_store_addr = bheader->next_block_address;
    
    /* This block was valid */
    
    /* Update sequence ID */
    *seq_id = (uint16_t)((*seq_id) + 1);

    return true;

}
/******************************************************************************
End of function fl_store_retry_continue
******************************************************************************/

/******************************************************************************
* Function Name: fl_store_retry_get_block
* Description  : Depending on the type of memory being used, the last good
*                block that can be used will be the last one that is
*                underneath the current erase boundary.  This function finds
*                that block.
* Arguments    : seq_id - 
*                    Pointer to return sequence ID
* Return value : true - 
*                    Need to start over
*                false - 
*                    Can save some data
******************************************************************************/      
bool fl_store_retry_get_block(uint16_t * seq_id)    
{
    uint32_t masked_addr;
    fl_block_header_t * bheader;

    /* This part will depend on what kind of external memory you are using. 
       In this example we are using a 16Mb (2MB) SPI Flash with an erasing 
       granularity of a sector which is 512Kb (64KB).  This means that 
       we have to mask the address to the next lowest 64KB boundary because
       we will have to erase the sector.  All of the valid records in that
       sector will be erased.  For this example I made sure the load images
       were partioned such that each 64KB block in the external memory held 
       a new load file block.  If this is not the case then you will need
       to take extra care to also bring in the block that crosses the erase
       boundary.  The efficiency of the download retrying will
       depend largely on the erase size of your external memory */
    masked_addr = g_fl_cur_store_addr & (~(g_fl_li_mem_info.erase_size-1));

    /* Check to see if we need to restart from the beginning */
    if(masked_addr == g_fl_li_mem_info.addresses[g_fl_store_block_num])
    {
        /* If the current store address equals the start address of the block
           then we need to start over */
           
        g_fl_cur_store_addr = masked_addr;   
           
        /* Erase start of section so it will be seen as usuable */
        fl_mem_erase(g_fl_cur_store_addr, FL_MEM_ERASE_SECTOR);
                
        *seq_id = 0;

        return true;
    }
    else if(masked_addr == g_fl_cur_store_addr)
    {       
        /* In this case the block that failed was the first of the new erase sector.
           This means we can just use it's sequence ID */
                   
        /* Init g_fl_prev_store_addr */
        g_fl_prev_store_addr = g_fl_cur_store_addr;
         
        /* Delete off bad blocks */
        g_fl_cur_delete_addr = g_fl_cur_store_addr;

        return false;
    
    }
    else
    {
        /* Else, find the last good block, we have at least saved the
           first erase block sector */
        
        g_fl_cur_store_addr = masked_addr;
           
        /* Init g_fl_prev_store_addr */
        g_fl_prev_store_addr = g_fl_cur_store_addr;
         
        /* Delete off bad blocks */
        g_fl_cur_delete_addr = g_fl_cur_store_addr;

        /* Read block header */
        fl_mem_read(g_fl_cur_store_addr, &g_fl_rx_buffer[0], sizeof(fl_block_header_t));

        bheader = (fl_block_header_t *)&g_fl_rx_buffer[0];
    
        *seq_id = bheader->sequence_ID;

        return false;
    }
}
/******************************************************************************
End of function fl_store_retry_init
******************************************************************************/

/******************************************************************************
* Function Name: fl_store_block_init
* Description  : Initializes variables to store block record in 
*                external memory
* Arguments    : data_addr - 
*                    Start address of data to store
*                data_size - 
*                    Number of bytes of data to store
* Return value : none
******************************************************************************/
void fl_store_block_init(uint8_t * data_addr, uint32_t data_size)
{
    uint32_t next_page;
    uint32_t bytes_left_in_page;
    
    /* Initialize variables for use later */
    g_fl_cur_data_addr = data_addr;
    g_fl_cur_data_bytes = data_size;    
    
    /* If this is first time then do not need to check for erase boundary
       or write address into previous block header (there is no previous
       block header). */
    if(g_fl_cur_store_addr != g_fl_prev_store_addr)
    {                            
        /* Check to see if this block will go over an erase boundary.
           If it does then shift it to the erase boundary.  The 
           '- 1' on the right side of the test comes from making sure
           that the data will not fit in perfectly.  Before, if the data
           fit in perfectly, then it would move the last block to the next
           erase sector when it did not need to.
           
           Example:
           g_fl_cur_store_addr = 0xF800
           g_fl_cur_data_bytes = 0x800
           g_fl_cur_store_addr + g_fl_cur_data_bytes = 0x10000 (new erase sector)
           by subtracting one it brings the total to 0xFFFF which is still 
           recognized as the same erase sector.
            */
        if( ( ((uint32_t)g_fl_cur_store_addr) & (~(g_fl_li_mem_info.erase_size-1)) ) != 
            ( (((uint32_t)g_fl_cur_store_addr) + g_fl_cur_data_bytes - 1) & (~(g_fl_li_mem_info.erase_size-1)) ) )
        {
            g_fl_cur_store_addr = ((((uint32_t)g_fl_cur_store_addr) + g_fl_cur_data_bytes) & 
                                   (~(g_fl_li_mem_info.erase_size-1)));
        }
        
        /* It could be that next_address_field spans across a programming page.
           In this case 2 writes will need to be performed. */
        next_page = (g_fl_prev_store_addr + g_fl_li_mem_info.max_program_size) & 
                    (~(g_fl_li_mem_info.max_program_size-1));  
        
        /* Make sure that when we write the 'next_block_address' field will
           fit in the same page. If not checked this can cause problems. */
        if( ((g_fl_prev_store_addr + offsetof(fl_block_header_t, next_block_address)) <
            next_page) && 
            ((g_fl_prev_store_addr + offsetof(fl_block_header_t, next_block_address) + 
              sizeof(g_fl_cur_store_addr)) > next_page) )
        {
            /* Two writes will be needed because the 'next_address_field' spans
               two programming pages */

            /* How many bytes are left in the current page. */
            bytes_left_in_page = next_page-(g_fl_prev_store_addr+ offsetof(fl_block_header_t, next_block_address));

            /* Issue write to fill last of first block */               
            fl_mem_write((g_fl_prev_store_addr + offsetof(fl_block_header_t, next_block_address)),
                         (uint8_t *)&g_fl_cur_store_addr, 
                         bytes_left_in_page);
                         
            /* Issue second write to write the rest of the address */
            fl_mem_write(next_page,
                         (uint8_t *)(((uint32_t)&g_fl_cur_store_addr) + bytes_left_in_page), 
                         (sizeof(g_fl_cur_store_addr) - bytes_left_in_page));
        }      
        else
        {
            /* Only one write is needed */
                      
            /* Write address for this block in previous block header */
            fl_mem_write((g_fl_prev_store_addr + offsetof(fl_block_header_t, next_block_address)), 
                         (uint8_t *)&g_fl_cur_store_addr, 
                         sizeof(g_fl_cur_store_addr));
        }
    
        /* Update g_fl_prev_store_addr */
        g_fl_prev_store_addr = g_fl_cur_store_addr;
    }
}
/******************************************************************************
End of function fl_store_block_init
******************************************************************************/

/******************************************************************************
* Function Name: fl_store_block_continue
* Description  : Stores block record in external memory
* Arguments    : none
* Return value : true - 
*                    More data to program
*                false - 
*                    All data has been programmed
******************************************************************************/
bool fl_store_block_continue(void)
{
    uint32_t bytes_to_program;
    
    bytes_to_program = 0;
            
    /* Send write to memory. If you are on a page boundary then
       either program the whole page or how many bytes are left.
       Using the min() macro for this.  If you are not on a page
       boundary then program up to the boundary or how many bytes
       are left if that is less. */
    if( (g_fl_cur_store_addr & (g_fl_li_mem_info.max_program_size-1)) == 0)
    {
        /* Write is on a page boundary so either write whole page or bytes left */
        bytes_to_program = (uint32_t)min(g_fl_li_mem_info.max_program_size, g_fl_cur_data_bytes);            
    }
    else 
    {
        /* Write is not on a page boundary, write either up to page or rest of data */
        /* For writing up to page I am using:
           -Add page size and mask off page size - 1.  This gives me the next highest page address.
           -Subtract the next highest page address from the current address to get how many bytes 
            are left in the current page */
        bytes_to_program = (uint32_t)min( ((g_fl_cur_store_addr+g_fl_li_mem_info.max_program_size) & 
                                           (~(g_fl_li_mem_info.max_program_size-1)) ) - 
                                          g_fl_cur_store_addr, 
                                          g_fl_cur_data_bytes);
    }
            
    /* Program bytes */
    fl_mem_write(g_fl_cur_store_addr, (uint8_t *)g_fl_cur_data_addr, bytes_to_program);     
    
    /* Update bytes left and data pointer */
    g_fl_cur_data_bytes -= bytes_to_program;
    g_fl_cur_data_addr += bytes_to_program;
    g_fl_cur_store_addr += bytes_to_program;
    
    if(g_fl_cur_data_bytes > 0)
    {
        /* There is more left to program */
        return true;
    }
    else
    {
        /* There is no more to program */
        return false;
    }
}
/******************************************************************************
End of function fl_store_block_continue
******************************************************************************/

/******************************************************************************
* Function Name: fl_store_finish
* Description  : Writes 'successfully_stored' in Load File Header.  The value
*                that is written is the max of the other 'successfully_stored'
*                fields in the other image headers plus 1.  This way we can
*                discern that this was the last downloaded file.  Also 
*                compare against the current running image.
* Arguments    : none
* Return value : none
******************************************************************************/
void fl_store_finish(void)
{
    int32_t latest_image;
    
    /* Find the last downloaded entry */
    latest_image = fl_get_latest_image();
                       
    /* Increment by 1 so that this is now the latest image */
    if(latest_image >= 0)
    {                     
        /* Image was found, increment it's value by 1 and use it */
        g_fl_load_image_headers[g_fl_store_block_num].successfully_stored = \
            g_fl_load_image_headers[latest_image].successfully_stored + 1;
    }
    else
    {
        /* No load images were found, use 0 */
        g_fl_load_image_headers[g_fl_store_block_num].successfully_stored = 0;
    }

#if defined(__IAR_SYSTEMS_ICC__)
#pragma diag_suppress = Pa039
#endif
    /* Write 'successfully_stored' field which will let us tell when
       this image was downloaded in comparison to other load images */
    fl_mem_write( (g_fl_li_mem_info.addresses[g_fl_store_block_num] + \
                  offsetof(fl_image_header_t, successfully_stored) ),
                  (uint8_t *)&g_fl_load_image_headers[g_fl_store_block_num].successfully_stored, 
                  sizeof(g_fl_load_image_headers[g_fl_store_block_num].successfully_stored) );
#if defined(__IAR_SYSTEMS_ICC__)
#pragma diag_default = Pa039
#endif    
}
/******************************************************************************
End of function fl_store_finish
******************************************************************************/

/******************************************************************************
* Function Name: fl_get_latest_image
* Description  : Returns index for the last downloaded load image
* Arguments    : none
* Return value : -1 - 
*                    No load image found
*                >0 - 
*                    The index of the latest load image
******************************************************************************/
int32_t fl_get_latest_image(void)
{
    uint32_t i;
    uint32_t curLoadedTime;
    int32_t  image_to_load;

    /* Initialize values. */    
    image_to_load = -1;
    curLoadedTime = 0;
    
    for(i = 0; i < FL_CFG_MEM_NUM_LOAD_IMAGES; i++)
    {
        if(g_fl_load_image_headers[i].valid_mask == FL_LI_VALID_MASK && \
           g_fl_load_image_headers[i].successfully_stored != FL_LI_NOT_SUCCESSFULLY_STORED)      
        {
            /* Valid image found, see if it is the latest */
            if( (image_to_load == -1) || (curLoadedTime < g_fl_load_image_headers[i].successfully_stored) )
            {
                /* This image is the newest found so far */
                image_to_load = i;
                
                /* Use this to compare against other images */
                curLoadedTime = g_fl_load_image_headers[i].successfully_stored;
            }
        }
    }
    
    return image_to_load;
}
/******************************************************************************
End of function fl_get_latest_image
******************************************************************************/

/******************************************************************************
* Function Name: fl_get_load_image_headers
* Description  : Obtains Load Image Header if available from external memory
* Arguments    : none
* Return value : none
******************************************************************************/
void fl_get_load_image_headers(void)
{
    uint32_t i;
        
    /* Get load image headers from external memory */
    for(i = 0; i < FL_CFG_MEM_NUM_LOAD_IMAGES; i++)
    {
        fl_mem_read(g_fl_li_mem_info.addresses[i], (uint8_t *)&g_fl_load_image_headers[i], sizeof(fl_image_header_t));
    }
}
/******************************************************************************
End of function fl_get_load_image_headers
******************************************************************************/

/******************************************************************************
* Function Name: fl_find_matching_image
* Description  : Searches for matching image in external memory
* Arguments    : ptr - 
*                    Image Header to try and match against
* Return value : -1 - 
*                    Image not found
*                >0 - 
*                    Index of image in g_fl_load_image_headers
******************************************************************************/
int32_t fl_find_matching_image(fl_image_header_t * ptr)
{
    int32_t i;
        
    /* Look through headers for a match */
    for(i = 0; i < FL_CFG_MEM_NUM_LOAD_IMAGES; i++)
    {                
        /* First check valid mask */
        if(g_fl_load_image_headers[i].valid_mask == FL_LI_VALID_MASK)
        {
            /* Make sure image IDs match */
            if(g_fl_load_image_headers[i].image_ID == ptr->image_ID)
            {
                /* See if version numbers match, if so then this might be retry */
                if(g_fl_load_image_headers[i].version_num == ptr->version_num)
                {
                    /* Match found return array index */
                    return i;
                }
            }
        }                                
    }
    
    return -1;
}
/******************************************************************************
End of function fl_find_matching_image
******************************************************************************/

/******************************************************************************
* Function Name: fl_verify_load_image
* Description  : Verifies that a load image is complete and error free
* Arguments    : image_index - 
*                    Which load image block to check
* Return value : true - 
*                    Image is complete and correct
*                false - 
*                    Image has errors
******************************************************************************/
bool fl_verify_load_image(uint32_t image_index)
{
    uint32_t cur_address;
    uint32_t bytes_read;
    uint16_t calc_crc; 
    uint16_t cur_seq_id;
    fl_block_header_t * bheader;
    
    /* First check load image header valid mask */
    if( g_fl_load_image_headers[image_index].valid_mask != FL_LI_VALID_MASK )
    {
        /* Valid mask was not correct */
        return false;
    }
    
    /* Check 'successfully_stored' (should not be 0xFFFFFFFF) */
    if( g_fl_load_image_headers[image_index].successfully_stored == 0xFFFFFFFF )
    {
        /* 'successfully_stored' should not be 0xFFFFFFFF */
        return false;
    }
    
    /* Get location of first block in external memory */
    cur_address = g_fl_load_image_headers[image_index].start_address;
    
    /* Initialize bytes_read. */
    bytes_read = sizeof(fl_image_header_t);
                  
    /* 'calc_crc' is initially the seed */
    calc_crc = FL_CRC_SEED;        
    
    /* First sequence ID should be 0 */
    cur_seq_id = 0;         
    
    /* Check blocks of the image. */
    /* We have to check off of bytes read.  We cannot check the address
       because the MCU might have aligned blocks to erase boundaries so
       the memory address will not work.  Also you have to remember to
       make the 'next_block_address' field in block headers all 0xFF's 
       since that is what was used when the image CRC was calculated. */
    while( bytes_read < g_fl_load_image_headers[image_index].load_image_size)
    {            
        /* Read block header */
        fl_mem_read(cur_address, &g_fl_rx_buffer[0], sizeof(fl_block_header_t));     
        
        bheader = (fl_block_header_t *)&g_fl_rx_buffer[0];   
        
        /* Do some checks to make sure the block is valid. If it is not valid and
           we try to use it, then bad things can happen. */
        if(bheader->valid_mask != FL_BH_VALID_MASK)
        {
            /* This block is not valid therefore the image is not valid */
            return false;
        }

        /* Check the sequence ID */
        if(bheader->sequence_ID != cur_seq_id++)
        {
            /* This block is not valid therefore the image is not valid */
            return false;
        }
        
        /* Update address */
        cur_address += sizeof(fl_block_header_t);
        
        /* Read block data */
        fl_mem_read(cur_address,
                    &g_fl_rx_buffer[sizeof(fl_block_header_t)],
                    bheader->data_size);
                    
        /* Update address for next block header.  Make sure this line
           executes before the next one where the value is reset. */
        cur_address = bheader->next_block_address;
        
        /* Make 'next_block_address' all 0xFF's (like it was when the host app
           calculated the CRC for the image) */
        bheader->next_block_address = 0xFFFFFFFF;
        
        /* Update bytes_read */
        bytes_read += sizeof(fl_block_header_t) + bheader->data_size;
        
        /* Perform CRC */
        R_CRC_Compute(calc_crc, 
                      &g_fl_rx_buffer[0], 
                      bheader->data_size + sizeof(fl_block_header_t),
                      &calc_crc);
                
    
    }
    
    /* Compare calculated CRC to the CRC in the load header */
    if( calc_crc == g_fl_load_image_headers[image_index].image_crc )
    {
        /* Image is valid */
        return true;
    }
    else 
    {
        /* Image is invalid */
        return false;
    }
}
/******************************************************************************
End of function fl_verify_load_image
******************************************************************************/




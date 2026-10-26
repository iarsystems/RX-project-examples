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
* File Name    : r_fl_downloader.c
* Version      : 3.00
* Description  : Contains the FlashLoader state machine.  This file should
*                not be changed unless you are changing part of the protocol.
******************************************************************************/  
/******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 05.04.2010 1.00    First Release
*         : 22.03.2011 2.00    First Release for YRDK
*         : 02.03.2012 3.00    Made compliant with CS v4.0. State machine is
*                              now triggered by user supplied timer. Uses
*                              r_crc_rx package.
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Functions prototypes for dealing with strings (used for memcpy) */
#include <string.h>
/* Included for definitions for 'true' and 'false' */
#include <stdbool.h>
/* Flash Loader project includes. */
#include "r_fl_includes.h"
/* Uses r_crc_rx package for CRC calculations. */
#include "r_crc_rx_if.h"
/* Used for re-entrancy protection with state machine. */
#include "platform.h"

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
/* Declares transmit and receive buffers.  The receive buffer
   is for commuincations to the host as well as when reading
   the external memory.  This is done to save space. */
uint8_t g_fl_tx_buffer[5]; 
uint8_t g_fl_rx_buffer[FL_CFG_DATA_BLOCK_MAX_BYTES+sizeof(fl_block_header_t)];
/* Data structure to hold load image headers */
fl_image_header_t   g_fl_load_image_headers[FL_CFG_MEM_NUM_LOAD_IMAGES];

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Keeps state of FlashLoader downloader state machine */
static fl_receive_states_t g_fl_receive_cur_state;
/* Used to hold load image header in case of retry.  For further
   description, look where it is used later in this file. */
static fl_image_header_t g_fl_cur_image_header;
/* Current Sequence ID */
static uint16_t g_fl_cur_seq_id;
/* How many bytes needed to start next state of state machine */
static uint16_t g_fl_rx_bytes_needed;
/* Current processing location in g_fl_rx_buffer[] */
static uint16_t g_fl_rx_index;
/* Holds how many bytes have been received in ISR */
static uint16_t g_fl_rx_bytes_received;
/* Points to info on current application */
static fl_image_header_t * g_pfl_cur_app_header;
/* Lock used for re-entrancy protection. */
static BSP_CFG_USER_LOCKING_TYPE g_fl_lock;

#if FL_CFG_TIMEOUT_ENABLE == 1
/* Counter used to detect a timeout */
static uint32_t g_fl_timeout_counter;
#endif

static void fl_receive_reset(void);
#if defined(__IAR_SYSTEMS_ICC__)
#pragma section = "APPHEADER_1"
#endif

/******************************************************************************
* Function Name: R_FL_DownloaderInit
* Description  : Initializes communications for FlashLoader operations
* Arguments    : none
* Return value : none
******************************************************************************/
void R_FL_DownloaderInit(void)
{
    /* Initialize state machine */
    g_fl_receive_cur_state = FL_STATE_START;
    
    /* Initialize pointer to current app's load image header */
#if defined(__IAR_SYSTEMS_ICC__)
    g_pfl_cur_app_header = (fl_image_header_t *)__section_begin("APPHEADER_1");
#else
    g_pfl_cur_app_header = (fl_image_header_t *)__sectop("APPHEADER_1");
#endif    
    /* Initialize resources needed for using external memory */
    fl_mem_init();
    
    /* Initialize communications interface to talk to host */
    fl_com_init();

    /* Initialize CRC. */
    R_CRC_Init();
    
    /* Initialize bytes needed to 1 to capture any communications */
    g_fl_rx_bytes_needed = 1;
    
    /* Initialize processing location in receive buffer */
    g_fl_rx_index = 0;        
        
    /* Initialize communications interface to receive data */
    fl_com_receive(&g_fl_rx_buffer[0], 0);      
    
    #if FL_CFG_TIMEOUT_ENABLE == 1
    /* Initialize the timeout value */
    g_fl_timeout_counter = FL_CFG_TIMEOUT_TICKS;
    #endif
    
}
/******************************************************************************
End of function FL_Downloader_Init
******************************************************************************/

/******************************************************************************
* Function Name: fl_receive_reset
* Description  : Re-initialize reception
* Arguments    : none
* Return value : none
******************************************************************************/
static void fl_receive_reset(void)
{       
    /* Re-initialize communications interface to receive data */
    fl_com_receive(&g_fl_rx_buffer[0], 0);   
    
    /* Reset index to beginning of receive buffer */
    g_fl_rx_index = 0;    
    
    /* Set this value to 0 so that state machine does not re-enter after
       this function has been called */
    g_fl_rx_bytes_received = 0;   
}
/******************************************************************************
End of function fl_receive_reset
******************************************************************************/

/******************************************************************************
* Function Name: R_FL_StateMachine
* Description  : This is the state machine that runs the FL project. This 
*                function should be called on a periodic basis by the user.
* Arguments    : none
* Return value : none
******************************************************************************/
void R_FL_StateMachine(void)
{       
    /* Booleans used in state machine */
    uint8_t fl_sm_error; 
    uint8_t fl_sm_reset;
    /* Used to test for timeout */
    uint16_t new_bytes_received;
    /* Pointer for looking at new block header */
    fl_block_header_t * fl_block_pointer;
    /* CRC calculations. */
    uint16_t crc_value;
    
    int32_t i;

    /* Attempt to acquire lock. This protects this function against 
       re-entrancy. */
    if (false == R_BSP_SoftwareLock(&g_fl_lock))
    {
        /* Another process is already in this function. */
        return;
    }
        
    /* Initialize error and reset state machine flags */
    fl_sm_error = false;
    fl_sm_reset= false;
    
    /* Get how many bytes have been received */
    new_bytes_received = fl_com_bytes_received();
    
    #if FL_CFG_TIMEOUT_ENABLE == 1
    
    /* Do some timeout checking */
    if( (g_fl_receive_cur_state != FL_STATE_START) &&
        (g_fl_rx_bytes_needed != 0) )
    {
        /* Check to see if there is any update */
        if(new_bytes_received == g_fl_rx_bytes_received)
        {
            /* Decrement timeout counter */
            g_fl_timeout_counter--;
            
            /* Reset state machine if timeout occurs */
            if(g_fl_timeout_counter == 0)
            {
                /* Send error back to host and reset state machine*/
                fl_sm_error = true;
                
                /* Reset the timer */
                g_fl_timeout_counter = FL_CFG_TIMEOUT_TICKS;
            }
        }
        else 
        {
            /* Reset the timer */
            g_fl_timeout_counter = FL_CFG_TIMEOUT_TICKS;
        }
    }
    
    #endif
    
    /* Update how many bytes have been recieved */
    g_fl_rx_bytes_received = new_bytes_received;
        
    /* See if enough data has come in to call the state machine */
    if((g_fl_rx_bytes_received - g_fl_rx_index) >= g_fl_rx_bytes_needed)
    {    
        /* Operations depend on current state in state machine */
        switch(g_fl_receive_cur_state)
        {        
            case FL_STATE_START: 
            
                /* Check for FlashLoader initialization */
                if(g_fl_rx_buffer[g_fl_rx_index++] == FL_COM_INIT_1)
                {
                    /* Move to next state */
                    g_fl_receive_cur_state = FL_STATE_INIT_1;
                    g_fl_rx_bytes_needed = 1;
                }
                
                /* Reset reception if needed */
                if( g_fl_rx_index >= sizeof(g_fl_rx_buffer) )           
                {
                    fl_receive_reset();
                }
                            
                break;
        
            case FL_STATE_INIT_1:
                /* Check for FlashLoader initialization - confirmation */
                if(g_fl_rx_buffer[g_fl_rx_index++] == FL_COM_INIT_2)
                {
                    /* Move to next state */
                    g_fl_receive_cur_state = FL_STATE_INIT_2;
                    g_fl_rx_bytes_needed = 1;
                    
                }
                else 
                {
                    /* If next byte did not match then start over */
                    fl_sm_reset= true;
                }
            
                break;
            
            case FL_STATE_INIT_2:
                /* FL init is done, now awaiting command */
                /* This byte will determine operation to be performed */
                switch(g_fl_rx_buffer[g_fl_rx_index++]){
                
                    case FL_COM_OP_NEW_IMAGE:                
                                                            
                        /* Reset g_fl_cur_seq_id */
                        g_fl_cur_seq_id = 0;
                                                                
                        /* Don't interrupt again until full header is received */
                        g_fl_rx_bytes_needed = sizeof(fl_image_header_t);
                    
                        /* Restart receive so there is enough room for headers to
                           be received, or erase block */
                        fl_receive_reset();
                                            
                        /* Move to next state, wait for image header */
                        g_fl_receive_cur_state = FL_STATE_OP_NEW_IMAGE;
                        
                        /* Send Acknowledgement */
                        g_fl_tx_buffer[0] = FL_COM_ACK;
                        fl_com_transmit(&g_fl_tx_buffer[0], 1);
                   
                        break;
                
                    case FL_COM_OP_INFO_REQ:
                
                        /* Send info on current running app, and stored app if it exists */
                    
                        /* First send how many bytes to expect */
                        /* Use FL_NUM_LOAD_IMAGES + 1 to include current application information */
                        g_fl_tx_buffer[0] = sizeof(fl_image_header_t) * (FL_CFG_MEM_NUM_LOAD_IMAGES + 1);
                        fl_com_transmit(&g_fl_tx_buffer[0], 1);                    
                    
                        /* Send current running app info */
                        fl_com_transmit((uint8_t *)g_pfl_cur_app_header, sizeof(fl_image_header_t));                    
                    
                        /* Send info on any load images stored in external memory */
                        /* Get info from external memory */
                        fl_get_load_image_headers();
                        /* Send load image info */
                        for(i = 0; i < FL_CFG_MEM_NUM_LOAD_IMAGES; i++)
                        {
                            fl_com_transmit((uint8_t *)&g_fl_load_image_headers[i], sizeof(fl_image_header_t));                    
                        }
                    
                        /* Reset state machine */
                        fl_sm_reset= true;
                    
                        break;
                
                    case FL_COM_OP_ERASE_BLOCK:
                
                        /* Host requested to erase a load block */
                                                                                
                        /* Don't interrupt again until which block to erase is received */
                        g_fl_rx_bytes_needed = 1;
                        
                        /* Restart receive so there is enough room for headers to
                           be received, or erase block */
                        fl_receive_reset();
                    
                        /* Move to next state, wait for which block to erase */
                        g_fl_receive_cur_state = FL_STATE_OP_ERASE_BLOCK_START;
                        
                        /* Send Acknowledgement */
                        g_fl_tx_buffer[0] = FL_COM_ACK;
                        fl_com_transmit(&g_fl_tx_buffer[0], 1);
                   
                        break;
                
                    default:
                        /* Error - no known command */
                        fl_sm_error = true;
                        break;
                }
    
                break;
        
            case FL_STATE_OP_ERASE_BLOCK_START:
        
                /* Which block to erase should have been downloaded */
            
                /* Check for validity */
                if(g_fl_rx_buffer[g_fl_rx_index] < FL_CFG_MEM_NUM_LOAD_IMAGES)
                {
                    /* Valid load block, erase it */
                    fl_start_erase_load_block(g_fl_rx_buffer[g_fl_rx_index++]);
                    
                    /* An erase is now ongoing, so let us back in */
                    g_fl_rx_bytes_needed = 0;
                                        
                    /* Move to next state, continue to erase  */
                    g_fl_receive_cur_state = FL_STATE_OP_ERASE_BLOCK_RUNNING;
                
                }
                else
                {
                    /* Not a valid load block #, send error */                                   
                    /* Reset state machine */
                    fl_sm_error = true;
                }
            
                break;
                
            case FL_STATE_OP_ERASE_BLOCK_RUNNING:
                
                /* Check to see if erase is done */
                if( fl_continue_erase_load_block() == false)
                {
                    /* Erase has finished */
                    /* Send Acknowledgement */
                    g_fl_tx_buffer[0] = FL_COM_ACK;
                    
                    /* Send response */
                    fl_com_transmit(&g_fl_tx_buffer[0], 1);
                    
                    /* Reset state machine */
                    fl_sm_reset= true;
                }
                            
                break;            
                        
            case FL_STATE_OP_NEW_IMAGE:
                        
                /* New Load Image Header should have been downloaded */
                
                /* In the case that a retry has to start from scratch it will
                   need the g_fl_rx_buffer space to read in data from the external
                   memory.  You could use another buffer, but doing it this way
                   saves room.  To make sure I don't lose the file header, I 
                   make this fl_image_header_t here to hold the data. */
                memcpy(&g_fl_cur_image_header, &g_fl_rx_buffer[0], sizeof(fl_image_header_t));                                
                
                /* Update Index */
                g_fl_rx_index += sizeof(fl_image_header_t);
            
                /* Check valid mask */
                if(g_fl_cur_image_header.valid_mask != FL_LI_VALID_MASK)
                {
                    fl_sm_error = true;     
                    break;          
                }
            
                /* Check that max block size is okay */
                if(g_fl_cur_image_header.max_block_size > FL_CFG_DATA_BLOCK_MAX_BYTES)
                {
                    /* The block size is too large, the current size of the receive
                       buffer cannot hold it */
                    g_fl_tx_buffer[0] = FL_COM_REP_BLOCK_TOO_LARGE;
                    fl_com_transmit(&g_fl_tx_buffer[0], 1);
                    fl_sm_error = true;
                    break;
                }
            
                /* Check if image will fit in external memory block */
                if(g_fl_cur_image_header.load_image_size > ((uint32_t)(FL_CFG_MEM_MAX_LI_SIZE_BYTES)) )
                {
                    /* Max size per image */
                    uint32_t size = FL_CFG_MEM_MAX_LI_SIZE_BYTES;
                
                    /* The load image is too large for us to store */
                    g_fl_tx_buffer[0] = FL_COM_REP_IMAGE_TOO_LARGE;
                    /* Put max space per image */
                    g_fl_tx_buffer[1] = (uint8_t)(size & (0x000000FF));
                    g_fl_tx_buffer[2] = (uint8_t)((size & (0x0000FF00))>>8);
                    g_fl_tx_buffer[3] = (uint8_t)((size & (0x00FF0000))>>16);
                    g_fl_tx_buffer[4] = (uint8_t)((size & (0xFF000000))>>24);
                    fl_com_transmit(&g_fl_tx_buffer[0], 5);
                    fl_sm_error = true;
                    break;
                }
            
                /* Compare new image to that of running image */
                if(g_fl_cur_image_header.image_ID == g_pfl_cur_app_header->image_ID)
                {
                    /* Check to make sure image sent in isn't already running */
                    if(g_fl_cur_image_header.version_num == g_pfl_cur_app_header->version_num)
                    {
                        /* Send back that requested version is already running */
                        g_fl_tx_buffer[0] = FL_COM_REP_ALREADY_RUNNING;
                        fl_com_transmit(&g_fl_tx_buffer[0], 1);                    
                                           
                        /* Reset state machine */
                        fl_sm_reset= true;
                    
                        break;
                    } 
                }
                /* At this point the new requested image is either a different version or a
                   new app. Now check to make sure this image is not already loaded 
                   in external memory */
               
                /* Get info from external memory */
                fl_get_load_image_headers();
                                    
                /* Check for matching image in external memory */
                i = fl_find_matching_image(&g_fl_cur_image_header);
            
                if(i >= 0)
                {
                    /* If returns > -1 then matching image was found, now check validity */
                    if(g_fl_load_image_headers[i].successfully_stored != FL_LI_NOT_SUCCESSFULLY_STORED)
                    {
                        /* Image is OK, just needs to be flashed */
                        g_fl_tx_buffer[0] = FL_COM_REP_NEEDS_FLASH;
                        fl_com_transmit(&g_fl_tx_buffer[0], 1);
                
                        /* Reset state machine */
                        fl_sm_reset= true;
                
                        break;                
                    }
                    else
                    {
                        /* Start testing with first block */
                        g_fl_cur_seq_id = 0;
                        
                        /* Initialize for finding last good block */
                        fl_store_retry_init((uint32_t)i);

                        /* Continue erasing until done */
                        g_fl_receive_cur_state = FL_STATE_RETRY_INIT;                                                                            
                    }
            
                }
                else 
                {
                    /* If -1 was returned then need to find open file block */
                    
                    /* Try to find open block */
                    g_fl_receive_cur_state = FL_STATE_NEW_IMAGE;
                                           
                }

                /* Let us back in next time */
                g_fl_rx_bytes_needed = 0;
                                                
                break;        
                
            case FL_STATE_NEW_IMAGE:
                    
                /* Search for open file block, if found write Load File Header */
                if(fl_store_init() == false)
                {
                    /* No empty store blocks were found */
                    g_fl_tx_buffer[0] = FL_COM_REP_NO_ROOM;
                    fl_com_transmit(&g_fl_tx_buffer[0], 1);
                    fl_sm_error = true;
                    break;
                }         

                /* Continue erasing until done */
                g_fl_receive_cur_state = FL_STATE_NEW_IMAGE_ERASING;    
                                                    
                break;        

            case FL_STATE_NEW_IMAGE_ERASING:
                
                /* Check to see if erase is done */
                if( fl_continue_erase_load_block() == false)
                {            
                    /* See if this is a new image or a retry */
                    if(g_fl_cur_seq_id == 0)
                    {
                        /* This is a new image so we need to store the 
                           image header */
                        fl_store_image_header(&g_fl_cur_image_header);
                    }
                    /* Else, this is a retry, so the image header is
                       already there. */                                            
                            
                    /* Move on to prepare for receive data */
                    g_fl_receive_cur_state = FL_STATE_RECEIVING_INIT;                                
                }
            
                break;
            
            case FL_STATE_RETRY_INIT:
            
                /* Image found but corrupt, need to retry from last good block.
                   This function will start the process of finding the last  
                   spot where the download can start again and will delete
                   off the corrupt data */
                if( fl_store_retry_continue(&g_fl_cur_seq_id) == false )
                {            
                    /* Last good block was found.  Due to erase boundaries,
                       now find where the download can restart from. */
                    g_fl_receive_cur_state = FL_STATE_RETRY_GET_BLOCK;                                                    
                }
                                
                break;    
                
            case FL_STATE_RETRY_GET_BLOCK:
                
                /* This function will return the last good block that can be used
                   based upon the memory that is being used to hold the images */
                if(fl_store_retry_get_block(&g_fl_cur_seq_id) == true)
                {
                    /* We have to start over */
                    g_fl_receive_cur_state = FL_STATE_RETRY_FINISH;                                                    
                }
                else
                {
                    /* We can use some of the data we already have, delete off 
                       the 'bad' data */
                    g_fl_receive_cur_state = FL_STATE_NEW_IMAGE_ERASING;        
                }
                                                
                break;                
            
            case FL_STATE_RETRY_FINISH:
            
                /* Check to make sure store manager can continue */
                if(fl_is_store_busy() == false)
                {                    
                    /* Reload info from external memory to reflect new open spot */
                    fl_get_load_image_headers();
                
                    /* Try to find open block */
                    g_fl_receive_cur_state = FL_STATE_NEW_IMAGE;
                }
                                                
                break;                                                                                
                
            case FL_STATE_RECEIVING_INIT:
                                                
                /* Don't interrupt again until you have first block header */
                g_fl_rx_bytes_needed = sizeof(fl_block_header_t);           
            
                /* Restart receive so there is enough room for headers and data
                   to be received */
                fl_receive_reset();
            
                /* Now ready to receive file blocks */
                g_fl_receive_cur_state = FL_STATE_RECEIVING_HEADER;                                
                
                /* Send acknowledgement to host and then the starting block # */
                g_fl_tx_buffer[0] = FL_COM_ACK;
                g_fl_tx_buffer[1] = (uint8_t)(g_fl_cur_seq_id & (0x00FF));
                g_fl_tx_buffer[2] = (uint8_t)((g_fl_cur_seq_id & (0xFF00)) >> 8);
                fl_com_transmit(&g_fl_tx_buffer[0], 3);
                                
                break;                
        
            case FL_STATE_RECEIVING_HEADER:
        
                /* See if we received new header, or if we are done */
                if(g_fl_rx_buffer[g_fl_rx_index] == FL_COM_DONE)
                {
                    /* Image has been transferred successfully. */
                    /* Write 'successfully_stored' in Load File Header */
                    fl_store_finish();
                
                    /* Send Done signal back */                              
                    g_fl_tx_buffer[0] = FL_COM_DONE;
                    fl_com_transmit(&g_fl_tx_buffer[0], 1);

                    /* Reset state machine */
                    fl_sm_reset= true;
                    
                    while (fl_com_send_status() == false)
                    {
                        /* Make sure last transmit finished, sometimes the reset 
                           could occur so fast that MCU did not have time to 
                           transmit a 'Finished' command back to the host. */
                    }

                    /* Reset the MCU */
                    fl_reset();

                }
                else 
                {            
                    /* Receiving header for block record */
                    fl_block_pointer = (fl_block_header_t *)&g_fl_rx_buffer[g_fl_rx_index];
            
                    /* Update index */
                    g_fl_rx_index += sizeof(fl_block_header_t);
            
                    /* Check valid mask */
                    if(fl_block_pointer->valid_mask != FL_BH_VALID_MASK)
                    {
                        fl_sm_error = true;
                        break;
                    }
            
                    /* Check sequence ID */
                    if(fl_block_pointer->sequence_ID != g_fl_cur_seq_id)
                    {
                        fl_sm_error = true;
                        break;
                    }
                
                    /* Send Acknowledgement */                              
                    g_fl_tx_buffer[0] = FL_COM_ACK;
                    fl_com_transmit(&g_fl_tx_buffer[0], 1);
            
                    /* Don't interrupt again until data portion is downloaded */
                    g_fl_rx_bytes_needed = fl_block_pointer->data_size;
        
                    /* Now ready to receive block data */
                    g_fl_receive_cur_state = FL_STATE_RECEIVING_DATA;
            
                }
        
                break;                
        
            case FL_STATE_RECEIVING_DATA:
                        
                /* Now have downloaded data portion */
            
                /* Receiving header for block record */
                fl_block_pointer = (fl_block_header_t *)&g_fl_rx_buffer[0];
            
                /* Compute CRC. */
                R_CRC_Compute( FL_CRC_SEED, 
                               &g_fl_rx_buffer[sizeof(fl_block_header_t)], 
                               fl_block_pointer->data_size,
                               &crc_value);
            
                /* Check CRC on data */
                if (crc_value != fl_block_pointer->data_crc )
                {
                    /* There was a communications error, tell host to resend block */               
                    g_fl_tx_buffer[0] = FL_COM_RESEND;
                    
                    /* Tell host to resend block */
                    g_fl_receive_cur_state = FL_STATE_RECEIVE_NEXT;

                }
                else
                {
                    /* CRC matched up to data, store block in external memory */
                    fl_store_block_init(&g_fl_rx_buffer[0], 
                                        fl_block_pointer->data_size + sizeof(fl_block_header_t));

                    /* Now ready to start storing the data */
                    g_fl_receive_cur_state = FL_STATE_STORING_DATA;
                
                }
                                    
                break;
                
            case FL_STATE_STORING_DATA:
            
                /* Continue storing received data */
                if( fl_store_block_continue() == false)
                {
                    /* Received data has been stored */
                    
                    /* Move to next sequence_ID */
                    g_fl_cur_seq_id++;
                
                    /* Send acknowledgement and move to next block */
                    g_fl_tx_buffer[0] = FL_COM_ACK;
                    
                    /* Tell host to send next block */
                    g_fl_receive_cur_state = FL_STATE_RECEIVE_NEXT;                    
                }
            
                break;
                
            case FL_STATE_RECEIVE_NEXT:
            
                g_fl_tx_buffer[1] = (uint8_t)(g_fl_cur_seq_id & (0x00FF));
                g_fl_tx_buffer[2] = (uint8_t)((g_fl_cur_seq_id & (0xFF00)) >> 8);
                fl_com_transmit(&g_fl_tx_buffer[0], 3); 

            
                /* Don't interrupt again until you have block header */
                g_fl_rx_bytes_needed = sizeof(fl_block_header_t);
                
                /* Restart receive so there is enough room for headers and data
                   to be received */
                fl_receive_reset();
        
                /* Receive header again */
                g_fl_receive_cur_state = FL_STATE_RECEIVING_HEADER;        
            
                break;                
        
            default: 
                break;
        }
          
    }
    
    /* Check for any errors or requests to reset (operation finished) */
    if(fl_sm_error == true || fl_sm_reset== true)
    {
        
        if(fl_sm_error == true)
        {
            /* Send that there was an error and restart state machine */
            g_fl_tx_buffer[0] = FL_COM_ERROR;
            fl_com_transmit(&g_fl_tx_buffer[0], 1);
        }
    
        /* Reset state machine */
        g_fl_receive_cur_state = FL_STATE_START;
        
        /* Initialize bytes needed to 1 to capture any communications */
        g_fl_rx_bytes_needed = 1;

        /* Initialize processing location in receive buffer */
        g_fl_rx_index = 0;
        
        /* Re-init receive */
        fl_com_receive(&g_fl_rx_buffer[0], 0);          
        
    }   

    /* Release lock. */
    R_BSP_SoftwareUnlock(&g_fl_lock);    
        
}
/******************************************************************************
End of function R_FL_StateMachine
******************************************************************************/


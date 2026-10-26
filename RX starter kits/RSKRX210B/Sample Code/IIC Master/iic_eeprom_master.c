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
* http://www.renesas.com/disclaimer
*******************************************************************************/
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : iic_eeprom_master.c 
* Version       : 1.00
* Device        : R5F5210B
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX210B
* Description   : Defines IIC functions used in this sample. The functions
*                 defined are to operate the IIC EEPROM with 11 bit memory 
*                 address (A10-A0).
*******************************************************************************/
/*******************************************************************************
* History       : 14.05.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/
/* Defines string functions used in this file */
#include <string.h>
/* IIC RPDL function definitions */
#include "r_pdl_iic.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Provides declarations of functions defined in this file */
#include "iic_eeprom_master.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Maximum EEPROM write exceeded flag declaration */
volatile bool gEEPROM_MaxWriteExceeded = false;
/* Maximum EEPROM memory address exceeded flag declaration */
volatile bool gEEPROM_MaxAddressExceeded = false;

/*******************************************************************************
* Outline      : Init_EEPROM_Master
* Description  : This function configures the IIC unit to operate in bus master
*                mode. See the 'iic_eeprom_master.h' header file for 
*                configuration options.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_EEPROM_Master(void)
{
    /* Declare error flag */
    bool err = true;

    /* Configure the applicable IIC pins */
    err &=  R_IIC_Set
            (
                PDL_IIC_PIN_SDA | PDL_IIC_PIN_SCL
            );

    /* Configure the IIC unit to operate as bus master */
    err &=  R_IIC_Create
            (
                IIC_CHANNEL,
                PDL_IIC_MODE_IIC|PDL_IIC_INT_PCLK_DIV_32,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                PDL_NO_DATA,
                IIC_TRANSFER_DATA_RATE,
                (EEPROM_SCL_RISE_TIME << 16) | EEPROM_SCL_FALL_TIME
            );          
                
    /* Halt in while loop when RPDL errors detected */  
    while(!err);
}
/*******************************************************************************
* End of function Init_EEPROM_Master
*******************************************************************************/

/*******************************************************************************
* Outline      : Write_EEPROM_Master
* Description  : This function writes to the EEPROM device. This function will
*                either perform a page or a byte write, depending on the amount
*                of data to write. The function checks if the impending write
*                will cross a page boundary, and splits the write into two to
*                prevent the EEPROM looping the write back to the start of the
*                page. See the iic_eeprom.h file for configuration options.
* Argument     : uint16_t address       - The start address of the EEPROM write.
*                uint8_t * write_data   - A pointer to the array of data to
*                                         the EEPROM.
*                uint8_t bytes          - The number of bytes to write.
* Return value : none
* Note         : 1. The Init_EEPROM function must be called prior to calling
*                   this function.
*                2. After calling this function, use the function CheckStatus_
*                   EEPROM to check if the transfer was successful.
*                3. If any of the write data would exceed the device's address
*                   range, the operation will not occur and CheckStatus_EEPROM
*                   return false.
*                4. If the input parameters are not valid, the EEPROM write will
*                   not happen and the CheckStatus_EEPROM function will return
*                   false.
*******************************************************************************/
void Write_EEPROM_Master(uint16_t address, uint8_t * write_data, uint8_t bytes)
{
    /* Declare error flag */
    bool err = true;

    /* Check if number of bytes to write is greater than EEPROM page size */
    if(bytes > EEPROM_MAX_WRITE)
    {
        /* Number of bytes has exceeded maximum EEPROM page size, set 
           flag to true */
        gEEPROM_MaxWriteExceeded = true;
    }
    /* Check if write operation would exceed EEPROM maximum storage */
    else if((address + bytes) > EEPROM_MAX_ADDRESS)
    {
        /* Write operation would use address exceeding the EEPROM maximum
           address, set the address exceeded flag to true */
        gEEPROM_MaxAddressExceeded = true;
    }
    /* Write operation input parameters are valid */ 
    else
    {   
        /* Declare EEPROM device address container variables */
        uint8_t first_dev_addr, second_dev_addr = EEPROM_DEVICE_ADDRESS;

        /* Declare upper page boundary address container variable */
        uint16_t page_boundary_addr = (uint16_t)((address & 0x0FF0) + 0x10);
        
        /* Insert the upper 3 address bits into the device addresses */
        first_dev_addr = (uint8_t)(EEPROM_DEVICE_ADDRESS |\
                     ((address & 0x0700) >> 7));
        second_dev_addr |= (uint8_t)((page_boundary_addr & 0x0F00) >> 7);
                        
        /* Check if impending write will cross a page boundary */
        if((0x0FF0 & (address + bytes)) - (address & 0x0FF0))
        {
            /* Declare two write buffers, to split the write up either side 
               of the EEPROM page boundary */
            uint8_t first_write_buffer[17];
            uint8_t second_write_buffer[17];
            
            /* Declare number of bytes to write container variables for both 
               halves of the write split */
            uint8_t first_num_bytes = 0x00, second_num_bytes = 0x00;
            
            /* Initialise the contents of the write buffers to 0x00 */
            memset(first_write_buffer, 0x00, 17u);
            memset(second_write_buffer, 0x00, 17u);
            
            /* Set the number of bytes to write before and after 
               the boundary */
            first_num_bytes = (uint8_t)(page_boundary_addr - address);
            second_num_bytes = (uint8_t)(bytes - first_num_bytes);
            
            /* Insert the lower 8 bits of the EEPROM memory start addresses 
               into both write buffers */
            first_write_buffer[0] = (uint8_t) (address & 0x00FF);
            second_write_buffer[0] = (uint8_t) (page_boundary_addr & 0x00FF);
            
            /* Fill first write buffer with the first half of the 
               write data */
            strncat((char*)(first_write_buffer + 1u), (char*)write_data,\
                              (uint32_t)first_num_bytes);
            
            /* Fill second write buffer with the second half of the data */
            strncat((char*)(second_write_buffer + 1u), (char*)(write_data + \
                     first_num_bytes), (uint32_t)second_num_bytes);
                    
            /* Increment number of bytes to write for both halves to 
               accommodate for the EEPROM memory addresses at the start of 
               each write */
            first_num_bytes++;
            second_num_bytes++;
            
            /* Write first half of the write split to the EEPROM */
            err &=  R_IIC_MasterSend
                    (
                        IIC_CHANNEL,
                        PDL_NO_DATA,
                        (uint16_t)first_dev_addr,
                        (uint8_t *)first_write_buffer,
                        (uint16_t)(first_num_bytes),
                        PDL_NO_FUNC,
                        0
                    );  
            
            /* Wait for last EEPROM write activity to complete */
            err &=  R_CMT_CreateOneShot
                    (
                        CMT_CHANNEL,
                        PDL_NO_DATA,
                        EEPROM_WRITE_WAIT,
                        PDL_NO_FUNC,
                        0
                    );
            
            /* Write second half of the write split to the EEPROM */
            err &=  R_IIC_MasterSend
                    (
                        IIC_CHANNEL,
                        PDL_NO_DATA,
                        (uint16_t)second_dev_addr,
                        (uint8_t *)second_write_buffer,
                        (uint16_t)second_num_bytes,
                        PDL_NO_FUNC,
                        0
                    );                      
        } 
        
        /* Impending write does not cross over an EEPROM page boundary,
           complete operation in a single write */
        else
        {
            /* Declare write buffer, and number of bytes to write temporary
               variables */
            uint8_t write_buffer[17u], num_bytes = (uint8_t)(bytes);
            
            /* Set the contents of the write buffer to 0x00 */
            memset(write_buffer, 0x00, 17u);
                        
            /* Append contents of data_write to the data buffer */
            strncat((char*)(write_buffer + 1u), (char*)write_data, \
                        (uint32_t)num_bytes);

            /* Increment the number of bytes to write to accommodate for the
               EEPROM memory address to begin the write from */
            num_bytes++;
            
            /* Insert the EEPROM memory address into the write buffer */
            write_buffer[0] = (uint8_t) (address & 0x00FF);

            /* Write to the IIC EEPROM */
            err &=  R_IIC_MasterSend
                    (
                        IIC_CHANNEL,
                        PDL_NO_DATA,
                        (uint16_t)first_dev_addr,
                        (uint8_t *)write_buffer,
                        (uint16_t)num_bytes,
                        PDL_NO_FUNC,
                        0
                    );      
        }
        
        /* Create oneshot delay to allow all write activity to finish */
        err &=  R_CMT_CreateOneShot
                (
                    CMT_CHANNEL,
                    PDL_NO_DATA,
                    EEPROM_WRITE_WAIT,
                    PDL_NO_FUNC,
                    0
                );
    }
            
    /* Halt in while loop when RPDL errors detected */
    while(!err);
}           
/*******************************************************************************
* End of function Write_EEPROM_Master
*******************************************************************************/

/*******************************************************************************
* Outline      : CheckStatus_EEPROM_Master
* Description  : This function checks EEPROM read/write error flags and the IIC
*                registers to determine whether the last IIC read/write
*                operation succeeded or failed. If the function returns true,
*                last read/write operation is assumed to have succeeded, but no
*                verification has been made.
* Argument     : none
* Return value : Bool - true  : The last IIC transaction is assumed to have been
*                               successful.
*                       false : The last IIC transaction was unsuccessful.
* Note         : 1. Call this function after performing a read or write, to
*                   check if it was successful.
*                2. If invalid input parameters are used for a read or write
*                   operation, the IIC operation will not happen and this
*                   function will return false. The error flags will be reset
*                   at the end of this function.
*******************************************************************************/                
bool CheckStatus_EEPROM_Master(void)
{
    /* Declare error flags */
    bool err = true, return_value;
    
    /* Declare IIC status flags container variable */
    uint32_t status_flags = 0u;
            
    /* Check status of last IIC data transmit */
    err &=  R_IIC_GetStatus
            (
                IIC_CHANNEL,
                &status_flags,
                PDL_NO_PTR,
                PDL_NO_PTR
            );
    
    /* Check if maximum number of bytes to write was exceeded */
    if(gEEPROM_MaxWriteExceeded)
    {
        /* Number of bytes to write exceeded EEPROM page size, return false */
        return_value = false;
    }
            
    /* Check if a timeout occurred, or a NACK was received. */
    else if(status_flags & 0x00001100)
    {
        /* Last IIC operation failed, return false */
        return_value = false;
    }
    
    /* Check if a stop condition was not received */
    else if (!(status_flags & 0x00000800))
    {
        /* Last IIC operation failed, return false */
        return_value = false;
    }
            
    /* No fail conditions detected, transfer assumed successful */
    else
    {
        /* Transfer assumed successful, return true */
        return_value = true;
    }
    
    /* Set error flags to false */
    gEEPROM_MaxWriteExceeded = false;
    gEEPROM_MaxAddressExceeded = false;
    
    /* Return operation success/failure */
    return return_value;
}       
/*******************************************************************************
* End of function CheckStatus_EEPROM_Master
*******************************************************************************/

/*******************************************************************************
* Outline      : Read_EEPROM_Master
* Description  : This function reads the data at the location of the address
*                passed, and copies the data to the output array passed. The
*                function will read the entire address range in one call, as 
*                long as the read operation does not request data outside of 
*                the EEPROM device's maximum address range. The function first
*                performs a dummy write to set the EEPROM's pointer, then 
*                performs a sequential read until all the data has been read.
* Argument     : uint16_t address      - Address to start the read from.
*                uint8_t * data_output - Pointer to the array to hold the read
*                                        data.
*                uint16_t num_bytes    - The number of bytes to read.
* Return value : none
* Note         : 1. Use the CheckStatus_EEPROM function after performing a read
*                   in order check if the read operation was successful.
*                2. If invalid input parameters are used the read operation 
*                   will not happen, and the CheckStatus_EEPROM function will
*                   return false. 
*******************************************************************************/
void Read_EEPROM_Master(uint16_t address, uint8_t * data_output, 
        uint16_t num_bytes)
{
    /* Declare error flag */
    bool err = true;
    
    if((address + num_bytes) > EEPROM_MAX_ADDRESS)
    {
        /* Read operation would use address exceeding the EEPROM maximum
           address, set the address exceeded flag to true */
        gEEPROM_MaxAddressExceeded = true;
    }
    /* Read operation parameters are valid */
    else
    {
        /* Declare EEPROM device address container variable */
        uint16_t dummy_write_dev_addr = EEPROM_DEVICE_ADDRESS;
        
        /* Declare dummy write data buffer, and initialise as the lower 8bits
           of the read address */
        uint8_t dummy_write_data = (uint8_t) (address & 0x00FF);
                
        /* Insert the upper 3 address bits into the device address */
        dummy_write_dev_addr |= (address & 0x0700) >> 7;

        /* Send dummy write to EEPROM to set memory address pointer */
        err &=  R_IIC_MasterSend
                (
                    IIC_CHANNEL,
                    PDL_IIC_STOP_DISABLE,
                    dummy_write_dev_addr,
                    &dummy_write_data,
                    1,
                    PDL_NO_FUNC,
                    0
                );      
    
        /* Create oneshot delay to allow all write activity to finish */
        err &=  R_CMT_CreateOneShot
                (
                    CMT_CHANNEL,
                    PDL_NO_DATA,
                    EEPROM_WRITE_WAIT,
                    PDL_NO_FUNC,
                    0
                ); 
            
        /* Request data from IIC EEPROM */
        err &=  R_IIC_MasterReceive
                (
                    IIC_CHANNEL,
                    PDL_NO_DATA,
                    EEPROM_DEVICE_ADDRESS,
                    data_output,
                    num_bytes,
                    PDL_NO_FUNC,
                    0
                );
    }
    
    /* Halt in while loop when RPDL errors detected */
    while(!err);
}           
/*******************************************************************************
* End of function Read_EEPROM_Master
*******************************************************************************/

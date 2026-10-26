/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software.  By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 *******************************************************************************/

/*******************************************************************************
 * File Name     : Hex.c
 * Version       : 1.00
 * Device(s)     : R5F51138AxFP
 * Tool-Chain    : IAR Systems ICCRX
 * Description   : Decode hex file from buffer and program to Flash
 * Creation Date : 30/10/2014
 *******************************************************************************/

/***********************************************************************************************************************
 Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/
#include <string.h>
#include "r_cg_macrodriver.h"
#include "r_rsk_utility.h"
#include "r_cg_userdefine.h"
#include "hex.h"
#include "crc.h"

/***********************************************************************************************************************
 Global variables and functions
 ***********************************************************************************************************************/
#define HEX_DATA_SIZE                 (256)

static void update_checksum (const uint8_t byte);
static uint8_t calc_checksum (void);
static void reset_checksum (void);

/* buffer storing decoded data ready to program to flash */
static uint8_t hex_buf[HEX_DATA_SIZE];

static hex_type_t hex_record_type = HEX_NONE;
static uint8_t hex_file_error = FALSE;
static uint16_t hex_checksum = 0;
static uint32_t address = 0;
static uint32_t start_address = 0;
static uint8_t num_data_bytes = 0;

/* perror_text is used to point to an error description string    */
/* in the event of an error.                                    */
/* It is set by the macro perror_text = (char *) &x) where 'x'     */
/* is the string in quotes. It is used by function              */
/* get_hex_error_text to report the error to the calling function. */
/* defining MINIMISE_CODE_SPACE may be used to remove this      */
/* reporting from the compiled code. See r_cg_userdefine.h      */
static char * perror_text = NULL;

/***********************************************************************************************************************
 * Function Name: is_file_hex
 * Description  : This function expects to be passed the first byte of a file and determines whether or not it is an
 *                hex file, signified by a ':'
 * Arguments    : char byte
 * Return Value : uint8_t TRUE  - Hex format
 *                        FALSE - not Hex format
 ***********************************************************************************************************************/
uint8_t is_file_hex (char byte)
{
    if (':' == byte)
    {
        return (TRUE);
    }
    return (FALSE);
}

/***********************************************************************************************************************
 * End of function is_file_hex
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: get_hex_type
 * Description  : This function decides what to do with a complete HEX line just imported.
 * Arguments    : none
 * Return Value : uint8_t RECORD_FINISHED - import completed
 *                        RECORD_FLASH    - update flash with current flash buffer contents
 *                        RECORD_NEXT     - no action - not a code line, or FLASH not required
 ***********************************************************************************************************************/
uint8_t get_hex_type (void)
{

    /* Data available in buffer */
    if (HEX_DATA == hex_record_type)
    {
        return (RECORD_FLASH);
    }

    /* end of file reached */
    if (HEX_FILE_END == hex_record_type)
    {
        return (RECORD_FINISHED);
    }
    return (RECORD_NEXT);
}

/***********************************************************************************************************************
 * End of function get_hex_type
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: import_hex_file
 * Description  : This function decodes the Hex data byte by byte, obtaining address and placing data
 *                in a buffer.
 *                For a useful description of the structure of an Hex file, see
 *                http://en.wikipedia.org/wiki/Intel_HEX or
 *                http://www.keil.com/support/docs/1584/
 * Arguments    : uint8_t byte - pointer to the current byte
 * Return Value : uint8_t FALSE - line incomplete. TRUE line complete
 ***********************************************************************************************************************/
uint8_t import_hex_file (const uint8_t byte)
{
    static uint8_t * pdata_ptr;
    static uint8_t step = 0;
    static uint8_t loop_ctr = 0;
    static uint16_t byte_count = 0;
    static uint8_t read_checksum = 0;
    static uint8_t calculated_checksum = 0;
    static uint16_t addr_lower_word = 0;
    static uint8_t ignore_data_bytes_count = 0;
    static uint32_t line_count = 0;

    /* for hex records with extended segment addressing */
    static uint16_t extended_seg_address = 0;

    /* for hex records with extended linear addressing */
    static uint16_t address_upper_word = 0;

    /* flag to indicate whether extended linear or segment addressing is active */
    static uint8_t extended_linear_addressing_mode = FALSE;

    uint8_t data;

    switch (step)
    {

        /* start of line */
        case 0 :

            /* Hex files always start a line with an ':' */
            if (':' == byte)
            {
                line_count++; /* increment the line counter */

                /* set pointer to start of buffer to contain actual decoded bytes from record */
                pdata_ptr = hex_buf;

                /* clear buffer */
                memset(hex_buf, '\0', HEX_DATA_SIZE);

                /* next step will be number of bytes record */
                step = 1;

                /* number of ascii bytes to receive for the 'Number of bytes' record */
                loop_ctr = 2;

                /* reset the 'Number of bytes' record before getting data */
                byte_count = 0;

                /* calculated checksum is over count, address and data fields */
                reset_checksum();
            }

            /* a '\n' can be received as first byte if the last received line terminates */
            /* as '\r\n' so allow just one of them to be received but no more */
            else if (('\n' == byte) && (!loop_ctr))
            {
                loop_ctr = 1;
            }
            else
            {

                /* invalid start character received */
                hex_file_error = TRUE;
                perror_text = (char *) &"HEX files start with ':'";
                loop_ctr = 0;
            }
        break;

            /* Number of Bytes in record. 1 byte in ASCII hex - 2 loops*/
        case 1 :
            loop_ctr--;

            /*move value in byte_count up one nibble */
            byte_count = (uint8_t) (byte_count << 4);

            /* extract real number from ASCII hex data */
            data = get_nibble_from_ascii(byte);

            /* value in data is valid if between 0 and 15 inclusive */
            if (data < 16)
            {
                byte_count = (uint8_t) (byte_count + data);
            }
            else
            {
                hex_file_error = TRUE;
                perror_text = (char *) &"Invalid ASCII character";
            }

            /* check for complete number of bytes value */
            if (!loop_ctr)
            {

                /* update the running counter for the checksum calculation */
                update_checksum(byte_count);

                /* next group of bytes will contain the start address of the block */
                step = 2;

                /*reset the address value prior to receiving bytes */
                addr_lower_word = 0;

                /* address is a 16bit number so 4 bytes of ASCII will be received */
                loop_ctr = 4;

                /* get record size */
                num_data_bytes = byte_count;

                /* because hex data is in hex ASCII i.e. 2 bytes per actual byte, */
                /* double the total number of bytes to be read */
                byte_count = (byte_count << 1);

                /* byte_count is not used for the address and record type fields */
                /* but it is used to help process the checksum calculations     */
                /* so it needs to be increased to take into account the address */
                /* and record type fields which is a constant number of bytes   */
                byte_count = (byte_count + (uint8_t) 6u);
            }
        break;

            /* Address in ASCII hex, 16 bit record, i.e. 4 ASCII characters */
        case 2 :

            /* dec counter for number of bytes in address block */
            loop_ctr--;

            /* dec counter for total number of bytes */
            byte_count--;

            /* move current address bytes up one nibble to accommodate new byte */
            addr_lower_word <<= 4;

            /* running out of bytes means bad data */
            if (!byte_count)
            {
                hex_file_error = TRUE;
                perror_text = (char *) &"Not enough bytes in line";
            }

            /* extract real number from ASCII hex data */
            data = get_nibble_from_ascii(byte);

            /* if data is valid then put it in the lowest nibble of the address total */
            if (data < 16)
            {
                addr_lower_word += data;
            }
            else
            {
                hex_file_error = TRUE;
                perror_text = (char *) &"Invalid ASCII character";
            }

            /* update the running counter for the checksum calculation if a */
            /* whole byte is received                                       */
            if (0 == (byte_count & 0x01))
            {
                update_checksum((uint8_t) addr_lower_word);
            }

            /* check if address block completely received */
            if (!loop_ctr)
            {

                /* next step will be record type */
                step = 3;

                /* record type consists of 2 ASCII characters */
                loop_ctr = 2;
            }
        break;

            /* record type */
        case 3 :

            /* dec counter for number of bytes in address block */
            loop_ctr--;

            /* dec counter for total number of bytes */
            byte_count--;

            /* records go 0-5, so leading character should always be 0 */
            if (loop_ctr && ('0' != byte))
            {
                hex_file_error = TRUE;
                perror_text = (char *) &"First record character must be '0'";
            }

            if (!loop_ctr)
            {

                /* record type of Hex line is determined by a number here, from 0 to 9 */

                /*Record   Description                     */
                /* 00      Data sequence + 16bit address   */
                /* 01      End of File                     */
                /* 02      Extended segment address record */
                /* 04      Extended linear address record  */
                /* 05      Start linear address record     */

                hex_record_type = HEX_NONE;
                if ('0' == byte)
                {
                    hex_record_type = HEX_DATA;
                }
                if ('1' == byte)
                {
                    hex_record_type = HEX_FILE_END;
                }
                if ('2' == byte)
                {
                    hex_record_type = HEX_EXTENDED_SEG_ADDR;
                }
                if ('3' == byte)
                {
                    hex_record_type = HEX_START_SEGMENT_ADDRESS;
                }
                if ('4' == byte)
                {
                    hex_record_type = HEX_EXTENDED_LINEAR_ADDR;
                }
                if ('5' == byte)
                {
                    hex_record_type = HEX_START_LINEAR_ADDR;
                }

                if (HEX_NONE == hex_record_type)
                {
                    hex_file_error = TRUE;
                    perror_text = (char *) &"Invalid record type";
                }

                /* update checksum counter */
                update_checksum(get_nibble_from_ascii(byte));

                /* move to next step - reading in data */
                if (byte_count)
                {
                    step = 4;
                    loop_ctr = 0;
                    (*pdata_ptr) = 0;
                }
                else
                {

                    /* skip to checksum if no data */
                    step = 5;
                    read_checksum = 0;
                    loop_ctr = 2;
                }
            }
        break;

            /* Data in ASCII hex */
        case 4 :

            /* Decrement counter for total number of bytes */
            byte_count--;

            /* move data byte up one nibble to accommodate new byte */
            (*pdata_ptr) = (uint8_t) ((*pdata_ptr) << 4);

            /* extract real number from ASCII hex data */
            data = get_nibble_from_ascii(byte);

            /* if data is valid then add it to the buffer */
            /* containing the actual bytes to be flashed  */
            if (data < 16)
            {
                (*pdata_ptr) = (uint8_t) ((*pdata_ptr) + data);

                /* if byte_count is even then a whole byte has been received */
                if (0 == (byte_count & 0x01))
                {

                    /* update the running counter for the checksum calculation */
                    update_checksum(*pdata_ptr);

                    /* if the current data is being ignored, ignore it */
                    if (ignore_data_bytes_count)
                    {
                        ignore_data_bytes_count--;
                    }

                    /* increment buffer pointer if not ignoring */
                    else
                    {
                        pdata_ptr++;
                    }

                    /* check bounds of buffer */
                    if (pdata_ptr > (uint8_t *) (hex_buf + HEX_DATA_SIZE))
                    {
                        pdata_ptr = hex_buf;
                        hex_file_error = TRUE;
                        perror_text = (char *) &"Overrun buffer";
                    }
                }

                /* check for end of data */
                if (!byte_count)
                {
                    step = 5;
                    read_checksum = 0;
                    loop_ctr = 2;
                }
            }
            else
            {

                /* invalid data byte */
                hex_file_error = TRUE;
                perror_text = (char *) &"Invalid ASCII character";
            }
        break;

            /* verify checksum */
        case 5 :
            loop_ctr--;

            if (get_nibble_from_ascii(byte) > 15)
            {
                hex_file_error = TRUE;

                perror_text = (char *) &"Invalid ASCII character";

            }

            /* add received ASCII nibble to checksum */
            read_checksum = (uint8_t) (read_checksum << 4);
            read_checksum = (uint8_t) (read_checksum + get_nibble_from_ascii(byte));

            if (!loop_ctr)
            {

                /* The checksum is the LSB of the one's complement of the sum of data values */
                calculated_checksum = calc_checksum();
                if (read_checksum != calculated_checksum)
                {
                    hex_file_error = TRUE;

                    perror_text = (char *) &"Incorrect Checksum";

                }
                step = 6;
            }
        break;

            /* carriage return signals end of line. '\r\n' or just '\r' are acceptable */
        case 6 :

            /* HEX line finished */
            /* reset various parameters ready for next line */
            step = 0;

            /* last byte should be a carriage return */
            if ('\r' == byte)
            {

                /* straight HEX_DATA lines have a 16 bit address only        */
                /* so use the 16 bit address received and add it to the      */
                /* current upper 16bits value to generate the 32 bit address */
                if (HEX_DATA == hex_record_type)
                {
                    if (TRUE == extended_linear_addressing_mode)
                    {
                        address = (((uint32_t) address_upper_word << 16) + addr_lower_word);
                    }
                    else
                    {
                        address = (((uint32_t) extended_seg_address << 4) + addr_lower_word);
                    }

                    /* ensure that data is within range */
                    if (((address < APP_START) || (address > APP_END)) || ((address + num_data_bytes) > APP_END))
                    {
                        hex_file_error = TRUE;
                        perror_text = (char *) &"Addressing out of range";
                    }
                }

                /* End of file. Reset processing variables */
                if (HEX_FILE_END == hex_record_type)
                {
                    extended_seg_address = 0;
                    address_upper_word = 0;
                    extended_linear_addressing_mode = FALSE;
                    step = 0;
                    loop_ctr = 0;
                    byte_count = 0;
                    pdata_ptr = hex_buf;
                }

                /* With a extended segment address mode record, the extended  */
                /* segment address is updated. The address of subsequent data */
                /* lines is then added to this segment address multiplied by  */
                /* 16.                                                        */
                if (HEX_EXTENDED_SEG_ADDR == hex_record_type)
                {
                    extended_seg_address = (uint16_t) (hex_buf[0] << 8);
                    extended_seg_address += hex_buf[1];
                    extended_linear_addressing_mode = FALSE;
                }

                /* get the start address of the application code */
                /* not simply using a uint16_t pointer to get the value because */
                /* the uint16_t pointer needs to be aligned to even bytes where */
                /* the buffer may not be */
                if (HEX_START_SEGMENT_ADDRESS == hex_record_type)
                {
                    start_address = (uint16_t) (hex_buf[0] << 8);
                    start_address += hex_buf[1];
                }

                /* get the new upper 16 bits of the 32bit address.              */
                /* not simply using a uint16_t pointer to get the value because */
                /* the uint16_t pointer needs to be aligned to even bytes where */
                /* the buffer may not be */
                if (HEX_EXTENDED_LINEAR_ADDR == hex_record_type)
                {
                    address_upper_word = (uint16_t) (hex_buf[0] << 8);
                    address_upper_word += hex_buf[1];
                    extended_linear_addressing_mode = TRUE;
                }

                /* finished */
                return (TRUE);
            }

            /* any other character received is an error */
            perror_text = (char *) &"Line should end with \\r\\n";
            hex_file_error = TRUE;
        break;

        default :
            step = 0;
            loop_ctr = 0;
            byte_count = 0;
            pdata_ptr = hex_buf;
            hex_file_error = TRUE;
            address_upper_word = 0;
            extended_linear_addressing_mode = FALSE;
    }

    if (TRUE == hex_file_error)
    {
        return (FALSE);
    }
    return (UNDECIDED);
}

/***********************************************************************************************************************
 * End of function import_hex_file
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: get_hex_error_text
 * Description  : returns error text from hex decoding
 * Arguments    : None
 * Return Value : char * pointer to perror_text
 * *********************************************************************************************************************/
char *get_hex_error_text (void)
{
    return (perror_text);
}

/***********************************************************************************************************************
 * End of function get_hex_error_text
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: get_hex_data
 * Description  : Returns pointer to the decoded data in buffer hex_buf
 * Arguments    : None
 * Return Value : char * pointer to decoded data line
 ***********************************************************************************************************************/
char *get_hex_data (void)
{
    return ((char *) hex_buf);
}

/***********************************************************************************************************************
 * End of function get_hex_data
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: get_hex_num_data_bytes
 * Description  : Returns number of bytes in data line
 * Arguments    : None
 * Return Value : uint8_t
 ***********************************************************************************************************************/
uint8_t get_hex_num_data_bytes (void)
{
    return (num_data_bytes);
}

/***********************************************************************************************************************
 * End of function get_hex_num_data_bytes
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: get_hex_address
 * Description  : Returns destination address of data in hex buffer
 * Arguments    : None
 * Return Value : uint32_t
 ***********************************************************************************************************************/
uint32_t get_hex_address (void)
{
    return (address);
}

/***********************************************************************************************************************
 * End of function get_hex_address
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: get_hex_start_address
 * Description  : Returns pointer to the start address
 * Arguments    : None
 * Return Value : uint32_t start address of code
 ***********************************************************************************************************************/
uint32_t get_hex_start_address (void)
{
    return (start_address);
}

/***********************************************************************************************************************
 * End of function get_hex_start_address
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: is_hex_file_error
 * Description  : Returns TRUE if the re was an error reading the hex file
 *                Resets the error flag if set.
 * Arguments    : None
 * Return Value : uint8_t
 ***********************************************************************************************************************/
uint8_t is_hex_file_error (void)
{
    if (hex_file_error)
    {
        hex_file_error = FALSE;
        return (TRUE);
    }
    return (FALSE);
}

/***********************************************************************************************************************
 * End of function is_hex_file_error
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: update_checksum
 * Description  : updates checksum variable calcCheckSum with converted ASCII data nibble
 *                needs to be called twice, for each ASCII byte pair, MS nibble first
 * Arguments    : uint8_t data - data value for current nibble 0x0 - 0xF
 * Return Value : None
 ***********************************************************************************************************************/
static void update_checksum (const uint8_t byte)
{
    hex_checksum += byte;
}

/***********************************************************************************************************************
 * End of function update_checksum
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: calc_checksum
 * Description  : returns checksum calculation - 2's complement of summed bytes
 * Arguments    : void
 * Return Value : uint8_t checksum calculation
 ***********************************************************************************************************************/
static uint8_t calc_checksum (void)
{
    uint8_t chk;
    chk = (uint8_t) hex_checksum;
    chk = (chk ^ (uint8_t) 0xFF);
    chk++;
    return ((uint8_t) chk);
}

/***********************************************************************************************************************
 * End of function calc_checksum
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: reset_checksum
 * Description  : resets checksum
 * Arguments    : void
 * Return Value : void
 ***********************************************************************************************************************/
static void reset_checksum (void)
{
    hex_checksum = 0;
}

/***********************************************************************************************************************
 * End of function reset_checksum
 ***********************************************************************************************************************/


/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : srec.c
* Version      : 1.00
* Device(s)    : R5F565N9AxFB
* Tool-Chain   : CCRX
* H/W Platform : RSK+RX65N
* Description  : Decode S-Record format data into flash
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2016 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "srec.h"
#include "crc.h"
#include "r_cg_macrodriver.h"
#include "r_rsk_utility.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define SREC_DATA_SIZE                 (256)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static void update_checksum (const uint8_t byte);
static uint8_t calc_checksum (void);
static void reset_checksum (void);

static uint8_t srec_buf[SREC_DATA_SIZE];
static srec_type_t record_type = SREC_NONE;
static uint8_t file_error = FALSE;
static char * perror_text = NULL;
static uint16_t checksum = 0;
static uint32_t address = 0;
static uint32_t start_address = 0;
static uint8_t num_data_bytes = 0;
static uint8_t ignore_flag = FALSE;

/***********************************************************************************************************************
* Function Name: is_file_srec
* Description  : This function expects to be passed the first byte of a file and determines whether or not it is an
*                SREC file, signified by a capital 'S'
* Arguments    : char byte
* Return Value : uint8_t TRUE  - SREC format
*                        FALSE - not SREC format
***********************************************************************************************************************/
uint8_t is_file_srec (char byte)
{
    if ('S' == byte)
    {
        return (TRUE);
    }
    return (FALSE);
}

/***********************************************************************************************************************
* End of function is_file_srec
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_srec_type
* Description  : This function decides what to do with a complete SREC line just imported.
* Arguments    : none
* Return Value : uint8_t RECORD_DONE   - import completed
*                        RECORD_FLASH  - update flash with current flash buffer contents
*                        RECORD_NEXT   - no action - not a code line, or FLASH not required
*                        RECORD_HEADER - current line is a header line, contains file info generally
***********************************************************************************************************************/
uint8_t get_srec_type (void)
{
    switch (record_type)
    {

        /* Header line received */
        case SREC_HEADER :
            return (RECORD_HEADER);
        break;

            /* Data line received */
        case SREC_16BIT_ADDR :
        case SREC_24BIT_ADDR :
        case SREC_32BIT_ADDR :

            /* Allow data received to be programmed to flash if in a valid area */
            if (FALSE == ignore_flag)
            {

                /* Flash program data */
                return (RECORD_FLASH);
            }
        break;

            /* A start record block generally indicates the end of the file as well as the
             * actual entry point to the application code */
        case SREC_RECORD_START32 :
        case SREC_RECORD_START24 :
        case SREC_RECORD_START16 :

            return (RECORD_FINISHED);
        break;

        default :
        break;
    }
    return (RECORD_NEXT);
}

/***********************************************************************************************************************
* End of function get_srec_type
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: import_srec_file
* Description  : This function decodes the SREC data byte by byte, obtaining address and placing data
*                in a buffer.
*                For a useful description of the structure of an SREC file, see
*                http://en.wikipedia.org/wiki/SREC_(file_format)
* Arguments    : uint8_t byte - pointer to the current byte
* Return Value : uint8_t FALSE - line incomplete. TRUE line complete
***********************************************************************************************************************/
uint8_t import_srec_file (const uint8_t byte)
{
    static uint8_t * pdata_ptr;
    static uint8_t step = 0;
    static uint8_t loop_ctr = 0;
    static uint8_t byte_count = 0;
    static uint8_t read_checksum = 0;
    static uint8_t calculated_checksum = 0;
    uint8_t data;

    switch (step)
    {

        /* Start of line 'S' */
        case 0 :

            /* Set pointer to start of buffer to contain actual decoded bytes from record */
            pdata_ptr = srec_buf;

            /* Reset flag that prevents writes to non-application areas */
            ignore_flag = FALSE;

            /* Clear buffer */
            memset(srec_buf, '\0', SREC_DATA_SIZE);
            if ('S' == byte)
            {
                step = 1;
            }
        break;

            /* Record type */
        case 1 :

            /* Record type of Srec line is determined by a number here, from 0 to 9 */

            /* Record   Description   Address Bytes   Data Sequence */
            /* S0      Block header  2               Yes           */
            /* S1      Data sequence 2               Yes           */
            /* S2      Data sequence 3               Yes           */
            /* S3      Data sequence 4               Yes           */
            /* S5      Record count  2               No            */
            /* S7      End of block  4               No            */
            /* S8      End of block  3               No            */
            /* S9      End of block  2               No            */

            record_type = SREC_NONE;
            if ('0' == byte)
            {
                record_type = SREC_HEADER;
            }
            if ('1' == byte)
            {
                record_type = SREC_16BIT_ADDR;
            }
            if ('2' == byte)
            {
                record_type = SREC_24BIT_ADDR;
            }
            if ('3' == byte)
            {
                record_type = SREC_32BIT_ADDR;
            }
            if ('5' == byte)
            {
                record_type = SREC_RECORD_COUNT;
            }
            if ('7' == byte)
            {
                record_type = SREC_RECORD_START32;
            }
            if ('8' == byte)
            {
                record_type = SREC_RECORD_START24;
            }
            if ('9' == byte)
            {
                record_type = SREC_RECORD_START16;
            }

            /* Invalid record type */
            if (SREC_NONE == record_type)
            {
                file_error = TRUE;
                perror_text = (char *) &"Invalid record type";
                return (FALSE);
            }

            /* Number of bytes for the 'Number of bytes' record */
            loop_ctr = 2;

            /* Reset the 'Number of bytes' record before getting data */
            byte_count = 0;

            /* Calculated checksum is over count, address and data fields */
            reset_checksum();

            step = 2;
        break;

            /* Number of Bytes in record. 1 byte in ASCII hex - 2 loops */
        case 2 :
            loop_ctr--;

            /* Move value in byte_count up one nibble */
            byte_count = (uint8_t) (byte_count << 4);

            /* Extract real number from ASCII hex data */
            data = get_nibble_from_ascii(byte);

            /* Value in data is valid if between 0 and 15 inclusive */
            if (data < 16)
            {
                byte_count = (uint8_t) (byte_count + data);
            }
            else
            {
                file_error = TRUE;
                perror_text = (char *) &"Invalid ascii character";
                return (FALSE);
            }

            /* Check for complete number of bytes value */
            if (!loop_ctr)
            {

                /* Update the running counter for the checksum calculation */
                update_checksum(byte_count);

                /* Next group of bytes will contain the start address of the block */
                step = 3;

                /* Ensure that the address variable is reset before getting address data */
                address = 0;

                /* There are a varying number of bytes depending upon the address size */
                /* 16 bit addressing : 2 bytes */
                if ((((SREC_HEADER == record_type) || (SREC_16BIT_ADDR == record_type))
                        || (SREC_RECORD_COUNT == record_type)) || (SREC_RECORD_START16 == record_type))
                {
                    loop_ctr = 4;
                }

                /* 24 bit addressing : 3 bytes */
                if ((SREC_24BIT_ADDR == record_type) || (SREC_RECORD_START24 == record_type))
                {
                    loop_ctr = 6;
                }

                /* 32 bit addressing : 4 bytes */
                if ((SREC_32BIT_ADDR == record_type) || (SREC_RECORD_START32 == record_type))
                {
                    loop_ctr = 8;
                }

                /* Determine the number of data bytes to be extracted */
                num_data_bytes = (uint8_t) ((byte_count - (loop_ctr >> 1)) - 1);

                /* Because srec data is in hex ascii i.e. 2 bytes per actual byte, */
                /* double the total number of bytes to be read */
                byte_count = (uint8_t) (byte_count << 1);
            }
        break;

            /* Address in ASCII hex, number of records dependent upon address length */
        case 3 :

            /* Dec counter for number of bytes in address block */
            loop_ctr--;

            /* Dec counter for total number of bytes */
            byte_count--;

            /* Move current address bytes up one nibble to accommodate new byte */
            address <<= 4;

            /* Running out of bytes means bad data */
            if (!byte_count)
            {
                file_error = TRUE;
            }

            /* Extract real number from ASCII hex data */
            data = get_nibble_from_ascii(byte);

            /* If data is valid then put it in the lowest nibble of the address total */
            if (data < 16)
            {
                address += data;
            }
            else
            {
                file_error = TRUE;
                perror_text = (char *) &"Invalid ASCII character";
                return (FALSE);
            }

            /* Update the running counter for the checksum calculation if a */
            /* whole byte is received                                       */
            if (0 == (byte_count & 0x01))
            {
                update_checksum((uint8_t) address);
            }

            /* Check if address block completely received */
            if (!loop_ctr)
            {

                /* If not a data record then skip to checksum stage */
                if (SREC_RECORD_COUNT == record_type)
                {
                    read_checksum = 0;
                    loop_ctr = 2;
                    step = 5;
                }
                else
                {

                    /* Valid data record */

                    /* Ignore (don't program) data that is not within application code space */
                    if (((address < APP_START) || (address > APP_END)) || ((address + num_data_bytes) > APP_END))
                    {
                        ignore_flag = TRUE;
                    }

                    /* Start records hold the start address of the program */
                    /* This implementation is defined for programming the internal flash,
                     * Empirically this has been seen to be 0x000000 in the RX tool chain.
                     * With reference to the Wikipedia S-Record entry:
                     *
                     * "If a SREC file is only used to program a memory device and the
                     * execution location is ignored, then an address of zero could be used."
                     *
                     * So here we process the data, updating the Start_address, but choose
                     * to skip the checking of the ignore_flag. The actual start address
                     * for this flashing application is read from the vector table. */

                    if (((SREC_RECORD_START16 == record_type) || (SREC_RECORD_START24 == record_type))
                            || (SREC_RECORD_START32 == record_type))
                    {

                        /* Hold start address */
                        start_address = address;

                        /* No data expected - straight to checksum */
                        step = 5;
                        loop_ctr = 2;

#ifdef CHECK_START_ADDRESS  /* See comment above */
                        /* If the start address is outside of the application space then there is an error */
                        if (TRUE == ignore_flag)
                        {

                            /* Run out of bytes early */
                            file_error = TRUE;
                            perror_text = (char *)&"Invalid start address";
                            return (FALSE);
                        }
#endif  /* CHECK_START_ADDRESS */

                    }
                    else
                    {

                        /* Move to next step - reading in data */
                        step = 4;
                        loop_ctr = 0;
                        (*pdata_ptr) = 0;
                    }
                }
            }
        break;

            /* Data in ASCII hex */
        case 4 :

            /* Decrement counter for total number of bytes */
            byte_count--;

            /* Move data byte up one nibble to accommodate new byte */
            (*pdata_ptr) = (uint8_t) ((*pdata_ptr) << 4);

            if (!byte_count)
            {

                /* Run out of bytes early */
                file_error = TRUE;
                perror_text = (char *) &"Incorrect number of bytes";
                return (FALSE);
            }

            /* Extract real number from ASCII hex data */
            data = get_nibble_from_ascii(byte);

            /* If data is valid then add it to the buffer */
            /* containing the actual bytes to be flashed  */
            if (data < 16)
            {
                (*pdata_ptr) = (uint8_t) ((*pdata_ptr) + data);

                /* If byte_count is even then a whole byte has been received */
                if (0 == (byte_count & 0x01))
                {

                    /* Update the running counter for the checksum calculation */
                    update_checksum(*pdata_ptr);

                    /* Increment buffer pointer */
                    pdata_ptr++;

                    /* Check bounds of buffer */
                    if (pdata_ptr > (uint8_t *) (srec_buf + SREC_DATA_SIZE))
                    {
                        pdata_ptr = srec_buf;
                        file_error = TRUE;
                        perror_text = (char *) &"Overrun buffer";
                        return (FALSE);

                    }
                }

                /* All data received when only 1 byte left to go */
                /* so prepare to receive checksum */
                if (byte_count <= 2)
                {
                    step = 5;
                    read_checksum = 0;
                    loop_ctr = 2;
                }
            }
            else
            {

                /* Received data is no good */
                file_error = TRUE;
                perror_text = (char *) &"Invalid ASCII character";
                return (FALSE);
            }
        break;

            /* Verify checksum */
        case 5 :
            loop_ctr--;

            if (get_nibble_from_ascii(byte) > 15)
            {
                file_error = TRUE;
                perror_text = (char *) &"Invalid ASCII character";
                return (FALSE);
            }

            /* Update checksum variable */
            read_checksum = (uint8_t) (read_checksum << 4);
            read_checksum = (uint8_t) (read_checksum + get_nibble_from_ascii(byte));

            /* Complete checksum now received */
            if (!loop_ctr)
            {

                /* The checksum is the LSB of the one's complement of the sum of data values */
                calculated_checksum = calc_checksum();
                if (read_checksum != calculated_checksum)
                {
                    file_error = TRUE;
                    perror_text = (char *) &"Incorrect Checksum";
                    return (FALSE);
                }
                step = 6;
            }
        break;

            /* Carriage return */
        case 6 :

            /* SREC line finished */
            /* Reset various parameters ready for next line */
            step = 0;
            loop_ctr = 0;
            byte_count = 0;
            pdata_ptr = srec_buf;

            /* Finished */
            return (TRUE);
        break;

        default :
            step = 0;
            loop_ctr = 0;
            byte_count = 0;
            pdata_ptr = srec_buf;
            file_error = TRUE;
    }

    if (TRUE == file_error)
    {
        return (FALSE);
    }
    return (UNDECIDED);
}

/***********************************************************************************************************************
* End of function import_srec_file
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_srec_error_text
* Description  : returns error text from srec decoding
* Arguments    : None
* Return Value : uint8_t TRUE or FALSE
***********************************************************************************************************************/
char *get_srec_error_text (void)
{
    return (perror_text);
}

/***********************************************************************************************************************
* End of function get_srec_error_text
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_srec_data
* Description  : Returns pointer to the decoded data in buffer srec_buf
* Arguments    : None
* Return Value : uint8_t *
***********************************************************************************************************************/
char *get_srec_data (void)
{
    return ((char *) &srec_buf);
}

/***********************************************************************************************************************
* End of function get_srec_data
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_srec_num_data_bytes
* Description  : Returns pointer to the decoded data in buffer srec_buf
* Arguments    : None
* Return Value : uint8_t *
***********************************************************************************************************************/
uint8_t get_srec_num_data_bytes (void)
{
    return (num_data_bytes);
}

/***********************************************************************************************************************
* End of function get_srec_num_data_bytes
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_srec_address
* Description  : Returns pointer to the decoded data in buffer srec_buf
* Arguments    : None
* Return Value : uint8_t *
***********************************************************************************************************************/
uint32_t get_srec_address (void)
{
    return (address);
}

/***********************************************************************************************************************
* End of function get_srec_address
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_srec_start_address
* Description  : Returns pointer to the start address
* Arguments    : None
* Return Value : uint8_t *
***********************************************************************************************************************/
uint32_t get_srec_start_address (void)
{
    return (start_address);
}

/***********************************************************************************************************************
* End of function get_srec_start_address
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: is_srec_file_error
* Description  : Returns TRUE if there was an error reading the srec file
* Arguments    : None
* Return Value : uint8_t
***********************************************************************************************************************/
uint8_t is_srec_file_error (void)
{
    if (file_error)
    {
        file_error = FALSE;
        return (TRUE);
    }
    return (FALSE);
}

/***********************************************************************************************************************
* End of function is_srec_file_error
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
    checksum += byte;
}

/***********************************************************************************************************************
* End of function update_checksum
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: calc_checksum
* Description  : returns checksum calculation - 1's complement of summed bytes
* Arguments    : void
* Return Value : uint8_t checksum calculation
***********************************************************************************************************************/
static uint8_t calc_checksum (void)
{
    uint8_t chk;
    chk = (uint8_t) checksum;
    chk = (uint8_t) ~chk;
    return (chk);
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
    checksum = 0;
}

/***********************************************************************************************************************
* End of function reset_checksum
***********************************************************************************************************************/

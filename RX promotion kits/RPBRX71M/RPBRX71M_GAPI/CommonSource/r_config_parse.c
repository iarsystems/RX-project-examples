/******************************************************************************
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
 *******************************************************************************
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    */
/******************************************************************************
 * File Name    : r_config_parse.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : RXC
 * OS           : None
 * H/W Platform : RPB
 * Description  : Loads a config file from the file system and provides
 *                a function to retrieve entries.
 *                Config files have a similar format to Windows INI files.
 *                Entries have a 'section' in the format [name]
 *                r_get_config_entry will locate the specified section and
 *                return the following text.
 *******************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 14.04.2016 1.00 First Release
 ******************************************************************************/

/******************************************************************************
 WARNING!  IN ACCORDANCE WITH THE USER LICENCE THIS CODE MUST NOT BE CONVEYED
 OR REDISTRIBUTED IN COMBINATION WITH ANY SOFTWARE LICENSED UNDER TERMS THE
 SAME AS OR SIMILAR TO THE GNU GENERAL PUBLIC LICENCE
 ******************************************************************************/

/******************************************************************************
 Includes   <System Includes> , "Project Includes"
 ******************************************************************************/
#include <string.h>
#include "Global.h"
#include "r_tfat_lib.h"
#include "r_config_parse.h"

/******************************************************************************
 Macro Definitions
 ******************************************************************************/
#define CONFIG_FILE_NAME ("demo.txt")   /* File name of the configuration file */
#define CONFIG_FILE_MAX (200)           /* Defiles the maximum size of the configuration file */

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/
static uint8_t config_file_store[CONFIG_FILE_MAX];

/******************************************************************************
 Function Prototypes
 ******************************************************************************/

/******************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 Public Functions
 ******************************************************************************/
static char* config_locate (const char* p_inbuffer, const char* p_section, size_t* p_char_count);
static void config_extract (const char* string_buffer, char* string_loc);

/*****************************************************************************
 Function Name: r_config_load_from_filesystem
 Description:   Mounts and locates 'CONFIG_FILE_NAME' in the file system,
 Verifies file is not larger than buffer_size and then copies
 file contents in to global buffer available to the search
 functions.
 Parameters:    None
 Return value:  FILE_LARGER_THAN_BUFF if file size is > buffer_size or
 Number of bytes copied in to buffer.
 Zero if ini file not found
 *****************************************************************************/
int16_t r_config_load_from_filesystem (void)
{
    FATFS file_system_object;
    FRESULT fresult;
    FILINFO finfo;
    FIL file_object;
    int16_t retval = 0;

    memset(config_file_store, 0x00, sizeof(config_file_store)); /* Clear the config file buffer */

    fresult = R_tfat_f_mount(0, &file_system_object);

    if (TFAT_FR_OK == fresult)
    {
        /* Check we have sufficient buffer in which to load the file */
        fresult = R_tfat_f_stat(CONFIG_FILE_NAME, &finfo);
        if (finfo.fsize > (sizeof(config_file_store)))
        {
            retval = FILE_LARGER_THAN_BUFF;
        }
        else
        {
            fresult = R_tfat_f_open(&file_object, CONFIG_FILE_NAME, TFAT_FA_READ);
            if (TFAT_FR_OK == fresult)
            {
                fresult = R_tfat_f_read(&file_object, config_file_store, (sizeof(config_file_store)),
                        (uint16_t*) &retval);
            }
        }
    }

    return (retval);
}
/*****************************************************************************
 End of function  r_config_load_from_filesystem
 ******************************************************************************/

/*****************************************************************************
 Function Name: r_config_get_entry
 Description:   Locates a [section] in a buffer and copies out the associated
 entry (key).
 Parameters:    p_ini_file - pointer the configuration contents,
 p_section  - section to locate within the p_section contents
 string_buffer - Buffer in which the key is copied in to.
 max_chars - the number of chars available in string_buffer
 Return value:  0 - Success,
 PINI_INVALID - Specified p_ini_file is invalid,
 PSECT_NOTFOUND - Unable to locate p_section,
 PSECT_TOOBIG - p_section entry exceeds TXT_MAX_CHARS limit.
 *****************************************************************************/
int8_t r_config_get_entry (const char* p_section, char* string_buffer, uint16_t max_chars)
{
    int8_t retval = 0;
    char* pstring_loc;
    size_t char_count;

    string_buffer[0] = NULL; /* Annul any prev. entry */

    if (NULL == config_file_store)
    {
        retval = PINI_INVALID; /* Signal there is an error */
    }
    else
    {
        pstring_loc = config_locate((const char*) config_file_store, p_section, &char_count);

        if (NULL == pstring_loc)
        {
            retval = PSECT_NOTFOUND; /* Signal there is an error */
        }
        else if (char_count > max_chars)
        {
            retval = PSECT_TOOBIG; /* Signal there is an error */
        }
        else
        {
            /* Copy out the string */
            config_extract(pstring_loc, string_buffer);
        }
    }
    return (retval);
}
/*****************************************************************************
 End of function r_config_get_entry
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: config_locate
 Description:   Locates an .ini style [section] in an array and returns a pointer
 to the section result.
 Parameters:    char const* p_inbuffer - buffer in which to locate the section,
 char const* p_section - Pointer to the section name to find,
 size_t* char_count - set to the number of characters in the entry
 found.
 Return value:  NULL - String Not Found
 <value> - pointer to the section result.
 *****************************************************************************/
static char* config_locate (const char* p_inbuffer, const char* p_section, size_t* p_char_count)
{
    char* pstring_location = NULL;
    char* pterminator_location = NULL;

    pstring_location = strstr(p_inbuffer, p_section);

    if (0 != pstring_location)
    {
        pstring_location += (strlen(p_section)); /* Point to the section result */

        /* Look for a \r (return) which indicate the end of the string wanted. */
        pterminator_location = strstr(pstring_location, "\r") /* <return>- string_location*/;

        /* If \r terminator not found, look for 0x00, which indicates the end of the file */
        if (NULL == pterminator_location)
        {
            pterminator_location = strchr(pstring_location, 0x00);
        }

        *p_char_count = (size_t) (pterminator_location - pstring_location);
    }

    return (pstring_location);
}
/*****************************************************************************
 End of function  config_locate
 ******************************************************************************/

/*****************************************************************************
 Function Name: config_extract
 Description:   Copies out a key to a string. Key termator is '\r'
 Parameters:    string_buffer - Source Buffer
 string_loc - Destination for extracted key
 Return value:  None
 *****************************************************************************/
static void config_extract (const char* string_buffer, char* string_loc)
{
    size_t character_count;
    char* pterminator_location = NULL;
    pterminator_location = strstr(string_buffer, "\r") /* Find the end of the key  */;

    /* If \r terminator not found, look for 0x00, which indicates the end of the file */
    if (NULL == pterminator_location)
    {
        pterminator_location = strchr(string_buffer, 0x00);
    }
    character_count = (size_t) (pterminator_location - string_buffer);

    memcpy(string_loc, string_buffer, character_count);

    *(string_loc + character_count) = NULL; /* Add a null terminator */
}

/*****************************************************************************
 End of function  config_extract
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/

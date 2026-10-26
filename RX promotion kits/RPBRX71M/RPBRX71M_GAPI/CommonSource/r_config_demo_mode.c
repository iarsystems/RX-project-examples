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
 * File Name    : r_config_demo_mode.c
 * Version      : 1.00
 * Device(s)    : Renesas
 * Tool-Chain   : RXC
 * OS           : None
 * H/W Platform : RPB
 * Description  : Reads a file from the file system to determine which
 *                demo mode to operate in.
 *                Allows the writing of a file on the file system to set the
 *                demo mode.
 *
 *                Demo Mode File Content Format:
 *                1 ASCII Character -
 *                  G - GAPI Demonstration
 *                  B - Badge Demonstration
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
#include "r_config_demo_mode.h"

/******************************************************************************
 Macro Definitions
 ******************************************************************************/
#define MODE_FILE_NAME ("mode.txt")     /* File name of the demo mode file */
#define MODE_FILE_MAX (5)           /* Defiles the maximum size of the demo mode file */

/******************************************************************************
 Enumerated Types
 ******************************************************************************/

/******************************************************************************
 Typedef definitions
 ******************************************************************************/

/******************************************************************************
 Private global variables and functions
 ******************************************************************************/

/******************************************************************************
 Function Prototypes
 ******************************************************************************/

/******************************************************************************
 Constant Data
 ******************************************************************************/

/******************************************************************************
 Public Functions
 ******************************************************************************/

/*****************************************************************************
 Function Name: r_config_get_demo_mode
 Description:   Mounts and locates 'MODE_FILE_NAME' in the file system,
 Opens the file defined in MODE_FILE_NAME.
 Reads one byte from file.
 Returns the read byte or 0x00 in error conditions.
 Parameters:    None
 Return value:  Returns the read byte or NULL in error conditions.
 *****************************************************************************/
char r_config_get_demo_mode (void)
{
    FATFS file_system_object;
    FRESULT fresult;
    FIL file_object;
    int16_t readcnt = 0;
    char read_char = NULL;

    fresult = R_tfat_f_mount(0, &file_system_object);

    if (TFAT_FR_OK == fresult)
    {
        /* Check we have sufficient buffer in which to load the file */
        fresult = R_tfat_f_open(&file_object, MODE_FILE_NAME, TFAT_FA_READ);
        if (TFAT_FR_OK == fresult)
        {
            fresult = R_tfat_f_read(&file_object, (void*) &read_char, (sizeof(read_char)), (uint16_t*) &readcnt);

            /* Check the correct number of bytes have been read */
            if ((sizeof(read_char)) != readcnt)
            {
                /* If we have not read the correct number of bytes set return to error */
                read_char = NULL;
            }

        }
    }

    return (read_char);
}
/*****************************************************************************
 End of function  r_config_get_demo_mode
 ******************************************************************************/

/*****************************************************************************
 Function Name: r_config_set_demo_mode
 Description:   Mounts and locates 'MODE_FILE_NAME' in the file system,
 Opens the file defined in MODE_FILE_NAME.
 Writes the specified byte to the file.
 Returns the read byte or 0x00 in error conditions.
 Parameters:    char_to_write - Character to write to the config file
 Return value:  0 - Write Successful
 Other - Write failed
 *****************************************************************************/
uint8_t r_config_set_demo_mode (char char_to_write)
{
    FATFS file_system_object;
    FRESULT fresult;
    FIL file_object;
    uint16_t writecnt = 0;
    uint8_t retval = 1;

    fresult = R_tfat_f_mount(0, &file_system_object);

    if (TFAT_FR_OK == fresult)
    {
        /* Open the file with the attribute: TFAT_FA_CREATE_ALWAYS so that the file is overwritten
         * or created is it does not exist already.
         */
        fresult = R_tfat_f_open(&file_object, MODE_FILE_NAME, (TFAT_FA_WRITE | TFAT_FA_OPEN_ALWAYS));
        if (TFAT_FR_OK == fresult)
        {
            fresult = R_tfat_f_write(&file_object, (void*) &char_to_write, sizeof(char_to_write), &writecnt);
            R_tfat_f_close(&file_object);

            /* Check we have successfully written enough characters */
            if ((sizeof(char_to_write)) == writecnt)
            {
                retval = 0;
            }
        }
    }

    return (retval);
}
/*****************************************************************************
 End of function  r_config_set_demo_mode
 ******************************************************************************/

/******************************************************************************
 Private Functions
 ******************************************************************************/

/******************************************************************************
 End  Of File
 ******************************************************************************/

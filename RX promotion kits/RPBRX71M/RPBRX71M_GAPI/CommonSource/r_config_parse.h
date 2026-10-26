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
 * File Name    : r_config_parse.h
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
#ifndef INIPARSE_H_INCLUDED
    #define INIPARSE_H_INCLUDED

/******************************************************************************
 Macro definitions
 ******************************************************************************/
    #define FILE_LARGER_THAN_BUFF ((int16_t)(-1))

    #define PINI_INVALID     (-1)
    #define PSECT_NOTFOUND   (-2)
    #define PSECT_TOOBIG     (-3)

/******************************************************************************
 Typedef definitions
 ******************************************************************************/
/******************************************************************************
 Function Prototypes
 ******************************************************************************/
int8_t r_config_get_entry (const char* p_section, char* string_buffer, uint16_t max_chars);
int16_t r_config_load_from_filesystem (void);
#endif
/* INIPARSE_H_INCLUDED */

/******************************************************************************
 End  Of File
 ******************************************************************************/

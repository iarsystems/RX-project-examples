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
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : bootflags.c
* Version       : 1.00
* Device        : R5F51115AxFM
* Tool Chain    : CCRX
* H/W Platform  : RPBRX111
* Description   : Functions to get and set boot mode flag
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 26.10.2011 1.00     First Release
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include "bootflags.h"

/* boot flag in memory */
#define BOOTFLAG_MEMORY_ADDRESS 0x1000

/******************************************************************************
* Function Name: get_boot_flag
* Description  : returns boot flag from memory
* Arguments    : none
* Return Value : bootflag
******************************************************************************/
bootflags_t get_boot_flag (void)
{
    bootflags_t * p_flag = (bootflags_t *)BOOTFLAG_MEMORY_ADDRESS;
    return *p_flag;
}
/******************************************************************************
End of function get_boot_flag
******************************************************************************/

/******************************************************************************
* Function Name: set_boot_flag
* Description  : sets the boot flag
* Arguments    : new boot flag
* Return Value : none
******************************************************************************/
void set_boot_flag (bootflags_t const newflag)
{   
    bootflags_t * p_flag = (bootflags_t *)BOOTFLAG_MEMORY_ADDRESS;
    *p_flag = newflag;
}
/******************************************************************************
End of function set_boot_flag
******************************************************************************/

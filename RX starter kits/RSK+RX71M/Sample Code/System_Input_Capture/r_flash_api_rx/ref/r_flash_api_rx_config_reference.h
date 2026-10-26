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
 * File Name    : r_flash_api_rx_config.h
 * Description  : Configures the FLASH API module.
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 12.04.2014 1.00    First Release
***********************************************************************************************************************/
#ifndef FLASH_CONFIG_HEADER_FILE
#define FLASH_CONFIG_HEADER_FILE

/***********************************************************************************************************************
 Configuration Options
 ***********************************************************************************************************************/
/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
// Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
// Setting to 1 includes parameter checking; 0 compiles out parameter checking
#define FLASH_CFG_PARAM_CHECKING_ENABLE		(BSP_CFG_PARAM_CHECKING_ENABLE)


/* SPECIFY WHETHER TO CONFIGURE API for CODE FLASH OPERATION */
// Setting to 1 configures the API for Code Flash operations like Erase and Program
// Setting to 0 disables Code Flash Support.
// Irrespective of this define, Data Flash operations are always supported
#define FLASH_CFG_CODE_FLASH_ENABLE	(1)

/* SPECIFY WHETHER TO CONFIGURE API for CODE FLASH BGO OPERATION */
// Setting to 1 configures the API for Code Flash BGO operation\
   In this case, ensure that the MCU has more than 2 MB of Code Flash\
   and the flash API does not reside in the same 2 MB block as the area that it is programming.\
   For example if the Flash API is placed in the first 2 MB block, then all Code Flash operations\
   supported by the FLASH API must be performed only on the area over 2 MB.\
   The FLASH API will NOT be copied into RAM
// Setting to 0 disables Code Flash BGO operation\
// In this case, the FLASH API will be copied into RAM for execution and will return\
   only after the operation is completed.
#define FLASH_CFG_CODE_FLASH_BGO (1)


#endif /* FLASH_CONFIG_HEADER_FILE */


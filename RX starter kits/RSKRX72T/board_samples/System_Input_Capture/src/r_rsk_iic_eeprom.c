/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_rsk_iic_eeprom.c
* Version      : 1.00
* Device(s)    : R5F572TKCxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX72T
* Description  : This file contains functions for accessing eeprom via iic.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.11.2018 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* For memcpy() */
#include <string.h>

/* Contains string functions used in this file */
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"

#include "Config_RIIC0.h"
#include "r_rsk_iic_eeprom.h"
#include "r_rsk_nvm.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define EEPROM_DEVICE_ADDRESS (0xa6u)
#define MAX_WRITE_BYTES       (17)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/* API Functions */
/***********************************************************************************************************************
* Function Name: R_IIC_EepromRead
* Description  : Read data from the IIC EEPROM
* Arguments    : uint16_t addr    : address of eeprom data (0-based)
*                uint8_t * data   : destination buffer to place retrieved data
*                uint16_t no_bytes: number of bytes to read
* Return value : TRUE if READ ok, otherwise FALSE
***********************************************************************************************************************/
uint8_t R_IIC_EepromRead (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    MD_STATUS     ret;

    /* R1EX24002 addressing is achieved with A2-A0(Device bus connection) in the device address, */
    /* Memory address is bits a7-a0 */
    const uint16_t device_addr = (uint16_t)((EEPROM_DEVICE_ADDRESS | (uint8_t)((addr & 0x0f00u) >> 7u)) >> 1);

    /* Casting the size to a uint8_t type is valid because lower 8bits are required. */
    uint8_t low_addr = (uint8_t)(addr & 0x00ffu);

    /* Send write to configure memory to be read  */
    ret = R_Config_RIIC0_Master_Send(device_addr, &low_addr, (uint16_t)sizeof(uint8_t));

    if (MD_OK != ret)
    {
        return (FALSE);
    }

    /* Wait for transmission to complete with a timeout*/
    if (FALSE == R_IIC0_MasterWaitTxComplete())
    {
        return (FALSE);
    }

    ret = R_Config_RIIC0_Master_Receive(device_addr, data, no_bytes);

    if (MD_OK != ret)
    {
        return (FALSE);
    }

    /* Wait for reception to complete with a timeout*/
    if (FALSE == R_IIC0_MasterWaitRxComplete())
    {
        return (FALSE);
    }

    return (TRUE);
}

/***********************************************************************************************************************
* End of function R_IIC_EepromRead
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_IIC_EepromWrite
* Description  : Write data to the eeprom.
* Arguments    : uint16_t addr    : address of eeprom data (0-based)
*                uint8_t * data   : buffer containing data to write
*                uint16_t no_bytes: number of bytes to write
* Return value : TRUE if write ok, otherwise FALSE
***********************************************************************************************************************/
uint8_t R_IIC_EepromWrite (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    /* Requires buffer in static memory as write is done in interrupts */
    static uint8_t iic_write_buf[MAX_WRITE_BYTES];
    MD_STATUS      ret;

    /* R1EX24002 addressing is achieved with A2-A0(Device bus connection) in the device address, */
    /* Memory address is bits a7-a0 */
    const uint16_t  device_addr = (uint16_t)((EEPROM_DEVICE_ADDRESS | (uint8_t)((addr & 0x0f00u) >> 7u)) >> 1);

    /* Form static write buffer than contains address and data */
    if (no_bytes > (MAX_WRITE_BYTES - sizeof(uint8_t)))
    {
        return (FALSE);
    }

    /* Casting the size to a uint8_t type is valid because lower 8bits are required. */
    iic_write_buf[0] = (uint8_t)(addr & 0x00ff);

    /* Casting to ensure use of correct data type. */
    memcpy(&iic_write_buf[1], data, (uint32_t)no_bytes);

    /* Send write of address and data  */
    ret = R_Config_RIIC0_Master_Send(device_addr,
                              iic_write_buf,
                              (no_bytes + sizeof(uint8_t)));

    if (MD_OK != ret)
    {
        return (FALSE);
    }

    /* Wait for transmission to complete with a 1 s timeout*/
    if (FALSE == R_IIC0_MasterWaitTxComplete())
    {
        return (FALSE);
    }

    return (TRUE);
}

/***********************************************************************************************************************
* End of function R_IIC_EepromWrite
***********************************************************************************************************************/


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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_rsk_nvm.c
* Version      : 1.00
* Device(s)    : R5F56609HxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX660
* Description  : This file contains functions for abstracting NVM from underlying hardware.
* Creation Date: 2022-01-18
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>
/* Contains string functions used in this file */
#include "r_flash_rx_if.h"

#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_rtc.h"
/* Includes flash definitions and function prototypes */
#include "r_rsk_flash_data.h"

/* Includes IIC EEPROM definitions and function prototypes */
#include "r_rsk_iic_eeprom.h"

/* Header file contains declarations of functions defined in this file */
#include "r_rsk_nvm.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    NVM_INVALID,
    NVM_IIC,
    NVM_FLASH
} e_nvm_type_t;

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static e_nvm_type_t s_nvm_type         = NVM_INVALID;
static uint16_t     s_nvm_address      = 0;
static uint16_t     s_nvm_record_count = 0;
static uint16_t     s_nvm_size_bytes;

static uint8_t nvm_read (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes);
static uint8_t nvm_write (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes);

/***********************************************************************************************************************
* Function Name: R_NVM_Init
* Description  : Initializes the NVM storage medium
* Arguments    : None
* Return Value : FALSE if failed, TRUE if ok
***********************************************************************************************************************/
uint8_t R_NVM_Init(void)
{
    uint16_t sig;

    /* Try to read from the IIC eeprom */
    if (TRUE == R_IIC_EepromRead(0u, (uint8_t *)&sig, sizeof(uint16_t)))
    {
        /* Found IIC EEPROM */
        s_nvm_type       = NVM_IIC;
        s_nvm_size_bytes = IIC_EEPROM_SIZE_BYTES;
    }
    else
    {
        if (FLASH_SUCCESS == R_FLASH_Open())
        {
            /* Found internal FLASH */
            s_nvm_type       = NVM_FLASH;
            s_nvm_size_bytes = (BSP_DATA_FLASH_SIZE_BYTES / 2); /* (Half of Data flash area */
        }
        else
        {
            /* No valid NVM has been found */
            s_nvm_type = NVM_INVALID;
        }
    }

    /* Find the last signature in the NVM */
    if (FALSE == nvm_read(s_nvm_address, (uint8_t *)&sig, sizeof(uint16_t)))
    {
        return (FALSE);
    }

    while (NVM_SIGNATURE == sig)
    {
        /* Advance 1 NVM record */
        s_nvm_address += (uint16_t)sizeof(st_nvm_record_t);
        s_nvm_record_count++;

        /* Check end of Flash address */
        if (s_nvm_address >= s_nvm_size_bytes)
        {
            /* We are full so don't allow any more writes */
            return (FALSE);
        }

        /* Read next signature */
        if (FALSE == nvm_read(s_nvm_address, (uint8_t *)&sig, sizeof(uint16_t)))
        {
            return (FALSE);
        }
    }

    return (TRUE);
}

/***********************************************************************************************************************
End of function R_NVM_Init
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_NVM_Store
* Description   : Stores the rtc, temperature and adc value in a 16 byte record
*                Taking account of addressing which is managed internally in this module
* Arguments     : float   temp : temperature in deg C
*                 uint16_t adc : adc value
* Return Value : FALSE if failed, TRUE if ok
***********************************************************************************************************************/
uint8_t R_NVM_Store(rtc_calendarcounter_value_t * const pRtc, const float temp, const uint16_t adc)
{
    uint8_t      store_ok = FALSE;

    /* Copy passed data into NVM_RECORD structure */
    st_nvm_record_t newrec;


    /* Zero the memory first */
    memset(&newrec, 0, sizeof(st_nvm_record_t));

    newrec.signature = NVM_SIGNATURE;

    /* RTC data first */
    newrec.log_time.rhrcnt  = pRtc->rhrcnt;
    newrec.log_time.rmincnt = pRtc->rmincnt;
    newrec.log_time.rseccnt = pRtc->rseccnt;
    newrec.log_time.rdaycnt = pRtc->rdaycnt;
    newrec.log_time.rwkcnt  = pRtc->rwkcnt;
    newrec.log_time.rmoncnt = pRtc->rmoncnt;
    newrec.log_time.ryrcnt  = pRtc->ryrcnt;

    /* Temperature and adc */
    newrec.temperature = temp;
    newrec.adc_value   = adc;

    /* Write to NVM */
    store_ok = nvm_write(s_nvm_address, (uint8_t *)&newrec, sizeof(st_nvm_record_t));

    /* Check the result and increment the address and record count accordingly */
    if (TRUE == store_ok)
    {
        /* Casting to ensure use of correct data type. */
        s_nvm_address += (uint16_t)sizeof(st_nvm_record_t);
        s_nvm_record_count++;

        /* Check end of NVM address */
        if (s_nvm_address >= s_nvm_size_bytes)
        {
            /* We are full, return failure to prevent further access */
            store_ok = FALSE;
        }
    }
    else
    {
        /* Assume all bets are off for write from this point */
        s_nvm_address      = 0u;
        s_nvm_record_count = 0u;
    }


    return (store_ok);
}

/***********************************************************************************************************************
End of function R_NVM_Store
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_NVM_GetRecordCount
* Description  : Returns the number of records currently in NVM
* Arguments    : none
* Return Value : Number of records
***********************************************************************************************************************/
uint16_t R_NVM_GetRecordCount(void)
{
    return (s_nvm_record_count);
}

/***********************************************************************************************************************
End of function R_NVM_GetRecordCount
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_NVM_GetRecord
* Description  : Copies the NVM record requested into the passed storage
* Arguments    : uint16_t record_no : index of the record requested (0-based)
*                PNVM_RECORD p_nvm_rec: pointer to memory allocated for NVM_RECORD struct
* Return Value : FALSE if failed, TRUE if ok
***********************************************************************************************************************/
uint8_t R_NVM_GetRecord(const uint16_t record_no, st_nvm_record_t * const p_nvm_rec)
{
    /* Casting to ensure use of correct data type. */
    return (nvm_read((uint16_t)(record_no * sizeof(st_nvm_record_t)), (uint8_t *)p_nvm_rec, sizeof(st_nvm_record_t)));
}

/***********************************************************************************************************************
End of function R_NVM_GetRecord
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_NVM_Erase
* Description  : Erases the NVM storage medium
* Arguments    : None
* Return Value : FALSE if failed, TRUE if ok
***********************************************************************************************************************/
uint8_t R_NVM_Erase(void)
{
    st_nvm_record_t newrec;    /* Dummy Record used in the EEPROM erasure */
    uint8_t         erase_ok = TRUE;

    if (NVM_FLASH == s_nvm_type)
    {
        /* Erase all of data flash */
        erase_ok = R_FLASH_Erase(FLASH_DF_BLOCK_0, FLASH_NUM_BLOCKS_DF);
    }
    else
    if (NVM_IIC == s_nvm_type)
    {
        /* Zero the record first */
        memset(&newrec, 0, sizeof(st_nvm_record_t));

        /* For IIC eeprom loop to zero entire EEPROM */
        for (s_nvm_address = 0; ((s_nvm_address < s_nvm_size_bytes) || (FALSE == erase_ok)); 
            s_nvm_address += sizeof(st_nvm_record_t))
        {
            /* Casting to ensure use of correct data type. */
            erase_ok = nvm_write(s_nvm_address, (uint8_t *)&newrec, sizeof(st_nvm_record_t));
        }
    }
    else
    {
        erase_ok = FALSE;
    }

    /* Reset variables */
    s_nvm_address      = 0u;
    s_nvm_record_count = 0u;

    return (erase_ok);
}

/***********************************************************************************************************************
End of function R_NVM_Erase
***********************************************************************************************************************/



/**********************************************************************************************************************
* Function Name: R_NVM_SleepRecover
* Description  : Performs initialization actions required after processor
*                recovers from sleep mode
* Arguments    : None
* Return value : TRUE if recover ok, otherwise FALSE
***********************************************************************************************************************/
uint8_t R_NVM_SleepRecover(void)
{
    uint8_t return_val = FALSE;

    if (NVM_FLASH == s_nvm_type)
    {
        return_val = R_FLASH_DataOpen();
    }
    else
    if (NVM_IIC == s_nvm_type)
    {
        /* No action required for IIC */
    }
    else
    {
        /* No flash type declared */
    }
    return (return_val);
}

/**********************************************************************************************************************
* End of function R_NVM_SleepRecover
***********************************************************************************************************************/




/* Local functions */
/**********************************************************************************************************************
* Function Name: nvm_read
* Description  : Read data from the NVM
* Arguments    : uint16_t addr    : address of NVM data (0-based)
*                uint8_t * data   : destination buffer to place retrieved data
*                uint16_t no_bytes: number of bytes to read
* Return value : TRUE if READ ok, otherwise FALSE
***********************************************************************************************************************/
static uint8_t nvm_read(const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    uint8_t read_ok;

    if (NVM_FLASH == s_nvm_type)
    {
        read_ok = R_FLASH_DataRead(addr, data, no_bytes);
    }
    else
    if (NVM_IIC == s_nvm_type)
    {
        read_ok = R_IIC_EepromRead(addr, data, no_bytes);
    }
    else
    {
        read_ok = FALSE;
    }

    return (read_ok);
}

/**********************************************************************************************************************
* End of function nvm_read
***********************************************************************************************************************/

/**********************************************************************************************************************
* Function Name: nvm_write
* Description  : Write data to the NVM.
* Arguments    : uint16_t addr    : address of NVM data (0-based)
*                uint8_t * data   : buffer containing data to write
*                uint16_t no_bytes: number of bytes to write
* Return value : TRUE if READ ok, otherwise FALSE
******************************************************************************/
static uint8_t nvm_write(const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    uint8_t write_ok;

    if (NVM_FLASH == s_nvm_type)
    {
        flash_err_t flash_write_ok;

        /* Write to the data flash memory area */
        flash_write_ok = R_FLASH_Write((uint32_t)data, (uint32_t)(FLASH_DF_BLOCK_0 + addr), (uint32_t)no_bytes);

        if (FLASH_SUCCESS == flash_write_ok)
        {
            write_ok = TRUE;
        }
    } else
    if (NVM_IIC == s_nvm_type)
    {
        write_ok = R_IIC_EepromWrite(addr, data, no_bytes);
    }
    else
    {
        write_ok = FALSE;
    }

    return (write_ok);
}

/**********************************************************************************************************************
* End of function nvm_write
***********************************************************************************************************************/


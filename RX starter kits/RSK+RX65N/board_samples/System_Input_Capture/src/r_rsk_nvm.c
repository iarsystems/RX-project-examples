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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_rsk_nvm.c
* Version      : 1.00
* Device(s)    : R5F565N9AxFB
* Tool-Chain   : CCRX
* H/W Platform : RSK+RX65N
* Description  : This file contains functions for abstracting NVM from underlying hardware.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.06.2016 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>
/* Contains string functions used in this file */
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_rtc.h"

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
} nvm_type_t;

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
 
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static nvm_type_t nvm_type = NVM_INVALID;
static uint16_t nvm_address = 0;
static uint16_t nvm_record_count = 0;
static uint16_t nvm_size_bytes;

static uint8_t nvm_read (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes);
static uint8_t nvm_write (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes);

/***********************************************************************************************************************
* Function Name: R_NVM_Init
* Description  : Initializes the NVM storage medium, if I2C eeprom not found use flash data
* Arguments    : None
* Return Value : FALSE if failed, TRUE if ok
***********************************************************************************************************************/
uint8_t R_NVM_Init (void)
{
    uint16_t sig;

    /* Try to read from the IIC eeprom */
    if (TRUE == R_IIC_EepromRead(0u, (uint8_t *)&sig, sizeof(uint16_t)))
    {
        /* Found IIC EEPROM */
        nvm_type = NVM_IIC;
        nvm_size_bytes = IIC_EEPROM_SIZE_BYTES;
    }
    else
    {
        /* No valid NVM has been found */
        nvm_type = NVM_INVALID;
    }

    /* Find the last signature in the NVM */
    if (FALSE == nvm_read(nvm_address, (uint8_t *)&sig, sizeof(uint16_t)))
    {
        return (FALSE);
    }

    while (NVM_SIGNATURE == sig)
    {
        /* Advance 1 NVM record */
        nvm_address += (uint16_t)sizeof(nvm_record_t);
        nvm_record_count++;

        /* Check end of Flash address */
        if (nvm_address >= nvm_size_bytes)
        {
            /* We are full so don't allow any more writes */
            return (FALSE);
        }

        /* Read next signature */
        if (FALSE == nvm_read(nvm_address, (uint8_t *)&sig, sizeof(uint16_t)))
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
* Arguments     : rtc_calendarcounter_value_t * pRtc : pointer to RTC data structure
*                 double temp : temperature in deg C
*                 uint16_t adc : adc value
* Return Value : FALSE if failed, TRUE if ok
***********************************************************************************************************************/
uint8_t R_NVM_Store (rtc_calendarcounter_value_t * const pRtc, const double temp, const uint16_t adc)
{
    uint8_t      store_ok = FALSE;

    /* Copy passed data into NVM_RECORD structure */
    nvm_record_t newrec;

    /* Zero the memory first */
    memset(&newrec, 0, sizeof(nvm_record_t));

    newrec.signature = NVM_SIGNATURE;

    /* RTC data first */
    newrec.log_time.rhrcnt = pRtc->rhrcnt;
    newrec.log_time.rmincnt = pRtc->rmincnt;
    newrec.log_time.rseccnt = pRtc->rseccnt;
    newrec.log_time.rdaycnt = pRtc->rdaycnt;
    newrec.log_time.rwkcnt = pRtc->rwkcnt;
    newrec.log_time.rmoncnt = pRtc->rmoncnt;
    newrec.log_time.ryrcnt = pRtc->ryrcnt;

    /* Temperature and adc */
    newrec.temperature = temp;
    newrec.adc_value = adc;

    /* Write to NVM */
    store_ok = nvm_write(nvm_address, (uint8_t *)&newrec, sizeof(nvm_record_t));

    /* Check the result and increment the address and record count accordingly */
    if (TRUE == store_ok)
    {
        nvm_address += (uint16_t)sizeof(nvm_record_t);
        nvm_record_count++;

        /* Check end of NVM address */
        if (nvm_address >= nvm_size_bytes)
        {
            /* We are full, return failure to prevent further access */
            store_ok = FALSE;
        }
    }
    else
    {
       /* Assume all bets are off for write from this point */
        nvm_address = 0u;
        nvm_record_count = 0u;
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
* Return Value : Number of 32 byte records
***********************************************************************************************************************/
uint16_t R_NVM_GetRecordCount (void)
{
    return (nvm_record_count);
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
uint8_t R_NVM_GetRecord (const uint16_t record_no, nvm_record_t * const p_nvm_rec)
{
    return (nvm_read((uint16_t)(record_no * sizeof(nvm_record_t)),
                    (uint8_t *)p_nvm_rec,
                    sizeof(nvm_record_t)));
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
uint8_t R_NVM_Erase (void)
{
    uint8_t erase_ok = TRUE;
    nvm_record_t newrec;    /* Dummy Record used in the EEPROM erasure */

    if (NVM_IIC == nvm_type)
    {
        /* Zero the record first */
        memset(&newrec, 0, sizeof(nvm_record_t));

         /* For IIC eeprom loop to zero entire EEPROM */
        for (nvm_address = 0; ( (nvm_address < nvm_size_bytes) || (FALSE == erase_ok) ); nvm_address+=sizeof(nvm_record_t))
        {
            erase_ok = nvm_write(nvm_address, (uint8_t *)&newrec, sizeof(nvm_record_t));
        }
    }
    else
    {
        erase_ok = FALSE;
    }

    /* Reset variables */
    nvm_address = 0u;
    nvm_record_count = 0u;

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
uint8_t R_NVM_SleepRecover (void)
{
    uint8_t return_val = FALSE;

    if (NVM_IIC == nvm_type)
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
static uint8_t nvm_read (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    uint8_t read_ok;

    if (NVM_IIC == nvm_type)
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
static uint8_t nvm_write (const uint16_t addr, uint8_t * const data, const uint16_t no_bytes)
{
    uint8_t write_ok;

    if (NVM_IIC == nvm_type)
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





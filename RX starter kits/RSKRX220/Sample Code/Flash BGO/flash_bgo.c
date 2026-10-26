/*******************************************************************************
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
*******************************************************************************/
/* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : flash_bgo.c
* Version       : 1.00
* Device        : R5F52206
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines flash and ADC functions used in this sample.
******************************************************************************/
/******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* System Includes
******************************************************************************/

/* Provides standard string function definitions used in this file */
#include <string.h>

/******************************************************************************
* Project Includes
******************************************************************************/

/* Defines RX220 port registers */
#include <iorx220.h>

/* Defines macros relating to the RX220 user LEDs and switches */
#include "rskrx220def.h"

/* Defines switch functions and variables used in this file */
#include "switch.h"

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Declares prototypes for functions defined in this file */
#include "flash_bgo.h"

/* Defines flash API functions used in this file */
#include "r_Flash_API_RX200.h"

/* Defines options for customization of the flash API */
#include "r_Flash_API_RX200_UserConfig.h"

/******************************************************************************
* Global Variables
******************************************************************************/

/* Reserve an area for the DTC vector table */
#pragma location = 0x00003000
/* Declaration of the DTC vector table */
uint32_t dtc_vector_table[256];

/* Reserve an area for the reserved flash area */
#pragma location = 0x00100000
/* Declaration of the reserved flash area */
uint16_t gFlashWriteBuffer[256];

/* Reserve 16 bytes for the ADC trigger transfer data area */
uint32_t gDTC_S12AD_Transfer_Data[3];

/* Imported global variable, list of block addresses array */
extern const uint32_t block_addresses[260];

/* Variable to store ADC result transfered using DTC */
uint16_t g_store[256];

/* Declare number of transfers container variable */
const uint8_t gNum_Transfers = 0x00;

/* Declare a pointer to the DTC vector table */
uint32_t * gpVector_Table_Pointer = dtc_vector_table;

/* DTC transfer count temporary storage variable */
uint16_t gDTC_Transfer_Count = 0x0000;

/* Declare the current address of FLASH */
uint16_t gp_Address = 0x0;

/* Flash write address location global variable */
uint32_t gFlashWriteAddr;

/* Declare variable for clearing the debug LCD */
bool g_clear_lcd = false;

/* Declare a flag to indicate flash write operation statuses */
bool g_write_status = false;

/* Declare a flag to indicate ADC status */
bool g_adc_status = true;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/

/* ADC initialisation function prototype declaration */
static void init_adc (void);

/* DTC initialisation function prototype declaration */
static void init_dtc (void);

/*******************************************************************************
* Function Name: init_flash_bgo
* Description  : This function initialises the MCU flash area, allowing it to be
*                read and written to by user code. The function then calls the
*                init_dtc function to configure the DTC and ADC units used in
*                the project. Finally the function erases the contents of the
*                data flash.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_flash_bgo (void)
{
    /* Enable MCU access to the data flash area */
    R_FlashDataAreaAccess(FCU_ACCESS_MASK, FCU_ACCESS_MASK);

    /* Initialise the DTC unit */
    init_dtc();

    /* Initialise the ADC */
    init_adc();



    /* Clear the contents of the flash write buffer array */
    memset(g_store, 0x00, sizeof(g_store));

    /* Erase flash memory before writing to it */
    erase_flash_data();
}
/*******************************************************************************
* End of function init_flash_bgo
*******************************************************************************/

/*******************************************************************************
* Function Name: init_dtc
* Description  : This function initialises the DTC unit to perform 256 repeat
*                transfers of S12AD A/D conversions stored in g_store buffer.
*                The function first clears the g_store buffer and configures
*                the DTC transfers, triggered after completion of each A/D
*                conversion. The S12AD module is then configured in continuous
*                scan mode with interrupt requests enabled.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void init_dtc (void)
{
    /* Clear the contents of the 'gDTC_Destination' array */
    if (memset(g_store, 0u, sizeof(g_store)) == NULL)
    {
        /* Halt in while loop when errors detected */
        while (1)
        {
            /* Halt here as sample operation cannot continue */
        }
    }

    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the DTC module clock stop mode */
    MSTP_DTC = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Ensure DTC is disabled */
    DTC.DTCST.BIT.DTCST = 0x0;

    /* Reset the transfer skip flag */
    DTC.DTCCR.BIT.RRS = 0x0;

    /* Use short address mode */
    DTC.DTCADMOD.BIT.SHORT = 0x1u;

    /* Set the base address for calculating the DTC vector table address */
    DTC.DTCVBR = dtc_vector_table;

    /* Write to the MRA and SAR registers:
       Source address is fixed
       Select repeat transfer mode
       16-bit word transfer
        */
    gDTC_S12AD_Transfer_Data[0] = (0x50u << 24u) | ((uint32_t)DTC_SOURCE);

    /* Write to the MRB and DAR registers
       Transfer destination is the repeat area
       Destination address is incremented after each data transfer
       Chain transfers are disabled
       */
    gDTC_S12AD_Transfer_Data[1] = (0x08u << 24u) | ((uint32_t)g_store);

    /* Write to the CRA and CRB registers */
    gDTC_S12AD_Transfer_Data[2] = 0x00000000;

    /* Point to the S12AD vector in the DTC vector table */
    gpVector_Table_Pointer += VECT_S12AD_S12ADI0;

    /* Store the data start address */
    *gpVector_Table_Pointer = (uint32_t)gDTC_S12AD_Transfer_Data;

    /* Enable DTC trigger on ADC conversion end */
    DTCE(S12AD, S12ADI0) = 0x1u;

    /* Set the transfer skip flag */
    DTC.DTCCR.BIT.RRS = 0x1u;

    /* Enable DTC */
    DTC.DTCST.BIT.DTCST = 0x1u;
}
/*******************************************************************************
* End of function init_dtc
*******************************************************************************/

/*******************************************************************************
* Function Name: init_adc
* Description  : This function configures the 12-bit ADC in continuous scan
*                mode with interrupt requests enabled
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void init_adc (void)
{
    /* Use the AN000 (Potentiometer) pin as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01u;

    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0x0;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* ADC Continuous scan mode */
    S12AD.ADCSR.BIT.ADCS = 0x2u;

    /* Enable scan end interrupts */
    S12AD.ADCSR.BIT.ADIE = 0x1u;

    /* Enable automatic clearing of ADDR0 register after it's been read */
    S12AD.ADCER.BIT.ACE = 0x1u;

    /* Selects AN000 */
    S12AD.ADANSA.WORD = 0x0001u;

    /* Set S12AD interrupt level to 7 */
    IPR(S12AD, S12ADI0) = 0x7u;

    /* Enable S12AD interrupt requests */
    IEN(S12AD, S12ADI0) = 0x1u;
}
/*******************************************************************************
* End of function init_adc
*******************************************************************************/

/*******************************************************************************
* Function Name: erase_flash_data
* Description  : This function enters a for loop, and erases a block of data
*                flash memory each iteration until all blocks have been erased.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void erase_flash_data (void)
{
    /* Declare flash API error flag */
    uint8_t ret = 0;

    /* Declare loop count and block count variables */
    uint8_t  loop_counter = 0;
    uint16_t block_count = BLOCK_DB0;

    /* Declare an address variable */
    uint32_t address = 0x00000000;

    /* Initialise a for loop to erase each of the data flash blocks */
    for (loop_counter = 0; loop_counter < DF_NUM_BLOCKS; loop_counter++)
    {
        /* Fetch beginning address of DF block */
        address = block_addresses[BLOCK_DB0 + loop_counter];

        /* Erase data flash block */
        ret |=  R_FlashErase(block_count);

        /* Halt here if erase was unsuccessful */
        while (FLASH_SUCCESS != R_FlashGetStatus())
        {
            /* Halt here as sample operation cannot continue */
        }

        /* Check Blank Checking */
        ret |= R_FlashDataAreaBlankCheck(address, BLANK_CHECK_ENTIRE_BLOCK);

        /* Halt here if check was unsuccessful */
        while (FLASH_SUCCESS != R_FlashGetStatus())
        {
            /* Halt here as sample operation cannot continue */
        }

        /* Increment block counter */
        block_count++;
    }

    /* Halt in while loop when flash API errors detected */
    while (ret)
    {
        /* Halt here as sample operation cannot continue */
    }
}
/*******************************************************************************
* End of function erase_flash_data
*******************************************************************************/

/*******************************************************************************
* Function Name: write_flash_data
* Description  : This function writes the contents of g_store to the
*                data flash, at the location pointed by gFlashWriteBuffer. If
*                the number of bytes to write to flash is not a multiple of 8,
*                the data is padded with null bytes (0x00) to make up to the
*                nearest multiple of 8.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void write_flash_data (void)
{
    /* Declare flash API error flag */
    uint8_t status = 0x00;

    /* Declare the number of bytes variable, and initialise with the number of
       bytes the variable gFlashWriteBuffer contains */
    const uint16_t num_bytes = (uint16_t)(sizeof(g_store));

    /* Check if flash is busy */
    while (FLASH_BUSY == R_FlashGetStatus())
    {
        /* Do nothing until flash is free */
    }

    /* Write contents of write buffer to data flash */
    status |=   R_FlashWrite((uint32_t)gFlashWriteBuffer, (uint32_t)g_store,
                              num_bytes);

    /* Halt in while loop when flash API errors detected */
    while (status)
    {
        /* Halt here as sample operation cannot continue */
    }

    /* Halt here if check was unsuccessful */
    while (R_FlashGetStatus() != FLASH_SUCCESS)
    {
        /* Halt here as sample operation cannot continue */
    }

    /* Only do the check if the ADC is stopped */
    if (false == g_adc_status)
    {
        /* Compare memory locations to verify written data */
        if (memcmp(gFlashWriteBuffer, g_store, (size_t)num_bytes))
        {
            /* Display instructions on the debug LCD */
            Display_LCD(LCD_LINE2, "NO MATCH");
        }
        else
        {
            /* Display instructions on the debug LCD */
            Display_LCD(LCD_LINE2, " MATCH  ");
        }

        /* Create a short delay display delay */
        DisplayDelay(LCD_MSG_DELAY);
    }
}
/*******************************************************************************
* End of function write_flash_data
*******************************************************************************/

/*******************************************************************************
* Function Name: FlashError
* Description  : This is a callback function used to display a message on the
*                LCD, indicating a flash access error. A delay is added to
*                allow the message to be read.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void FlashError (void)
{
    /* Display instructions on the debug LCD */
    Display_LCD(LCD_LINE2, "FlashERR");

    /* Create a short delay display delay */
    DisplayDelay(LCD_MSG_DELAY);
}
/*******************************************************************************
* End of function FlashError
*******************************************************************************/

/*******************************************************************************
* Function Name: FlashWriteDone
* Description  : This is a user-defined callback function used to inidicate the
*                status of a flash write operation. Turns on LED1 to inidicate
*                that a flash write operation completed.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void FlashWriteDone (void)
{
    /* Turn on LED1 */
    LED1 = LED_ON;

    /* Set flag to true */
    g_clear_lcd = true;
}
/*******************************************************************************
* End of function FlashWriteDone
*******************************************************************************/

/*******************************************************************************
* Function Name: FlashEraseDone
* Description  : This is a user-defined callback function used to indicate the
*                status of a flash erase operation. Turns on LED2 to indicate a
*                flash erase operation completed.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void FlashEraseDone (void)
{
    /* Turn on LED2 */
    LED2 = LED_ON;

    /* Set flag to true */
    g_clear_lcd = true;
}
/*******************************************************************************
* End of function FlashEraseDone
*******************************************************************************/

/*******************************************************************************
* Function Name: FlashBlankCheckDone
* Description  : This is a user-defined callback function used to inidicate the
*                status of a flash blank check operation. Turns on LED0 to
*                indicate that the flash is not blank, it is turned off to
*                indicate flash contains data.
* Arguments    : (uint8_t) result - Sent by the FlashReadyInterrupt function in
*                                   file r_Flash_API_RX600.c
* Return Value : none
*******************************************************************************/
void FlashBlankCheckDone (uint8_t result)
{
    /* Check the result returned by the API */
    if (!result)
    {
        /* Turn off LED0 */
        LED0 = LED_OFF;
    }
    else
    {
        /* Turn on LED0 */
        LED0 = LED_ON;
    }
}
/*******************************************************************************
* End of function FlashBlankCheckDone
*******************************************************************************/


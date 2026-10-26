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
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines flash and ADC functions used in this sample.
******************************************************************************/
/******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* System Includes
******************************************************************************/
/* Provides standard string function definitions used in this file */
#include <string.h>

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines MCU port registers */
#include "iorx63n.h"
/* Defines macros relating to the RX63N user LEDs and switches */
#include "rskrx63ndef.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Declares prototypes for functions defined in this file */
#include "flash_bgo.h"
/* Defines flash API functions used in this file */
#include "r_Flash_API_RX600.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Define the data source for the DTC transfer as the ADC result register */ 
#define DTC_SOURCE &S12AD.ADDR0

/******************************************************************************
* Global Variables
******************************************************************************/

/* Reserve an area for the DTC vector table */
#pragma location = 0x00003000u
/* Declaration of the DTC vector table */
uint32_t dtc_vector_table[256u];

/* Reserve an area for the reserved flash area */
#pragma location = 0x00100000u
/* Declaration of the reserved flash area */
uint16_t gFlashWriteBuffer[256u];

/* Reserve 16 bytes for the ADC trigger transfer data area */
uint32_t gDTC_S12AD_Transfer_Data[3u];
/* Variable to store ADC result transfered using DTC */
uint16_t usStore[256u];
/* Declare number of transfers container variable */
const uint8_t gNum_Transfers = 0x00u;
/* Declare a pointer to the DTC vector table */
uint32_t * gpVector_Table_Pointer = dtc_vector_table;
/* DTC transfer count temporary storage variable */
uint16_t gDTC_Transfer_Count = 0x0000u;
/* Declare the current address of FLASH */
uint16_t gp_Address = 0x0u;
/* Flash write address location global variable */
uint32_t gFlashWriteAddr;
/* Total number of data flash blocks */
uint16_t gNumFlashBlocks = sizeof(g_flash_BlockAddresses)/4u;
/* Declare variable for clearing the debug LCD */
bool gClear_LCD = false;
/* Declare a flag to indicate flash write operation statuses */
bool gWrite_Status = false;
/* Declare a flag to indicate ADC status */
bool gADC_Status = true;
/* Switch press flag global variable (imported) */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* ADC initialisation function prototype declaration */
void Init_FlashBGO(void);
/* ADC initialisation function prototype declaration */
void InitADC_FlashBGO(void);
/* DTC initialisation function prototype declaration */
void InitDTC_FlashBGO(void);
/* Switch SW1 press callback function */
void SW1PressCB(void);
/* Switch SW2 press callback function */
void SW2PressCB(void);
/* Switch SW3 press callback function */
void SW3PressCB(void);

/*******************************************************************************
* Outline      : Init_FlashBGO
* Description  : This function initialises the MCU flash area, allowing it to be
*                 read and written to by user code. The function then calls the
*                 Init_DTC function to configure the DTC and ADC units used in
*                 the project. Finally the function erases the contents of the
*                 data flash.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_FlashBGO(void)
{    
    /* Enable MCU access to the data flash area */
    R_FlashDataAreaAccess(0xFFFFu, 0xFFFFu);
    
    /* Initialise the ADC unit */
    InitADC_FlashBGO();
    
    /* Initialise the DTC unit */
    InitDTC_FlashBGO();
    
    /* Clear the contents of the flash write buffer array */
    memset(usStore, 0x00, sizeof(usStore));
        
    /* Erase flash memory before writing to it */
    Erase_FlashBGO();
    
    /* Turn off LED2 */
    LED2 = LED_OFF;
}
/*******************************************************************************
* End of function Init_FlashBGO
*******************************************************************************/

/*******************************************************************************
* Outline      : InitDTC_FlashBGO
* Description  : This function initialises the DTC unit to perform 256 repeat 
*                 transfers of S12AD A/D conversions stored in usStore buffer. 
*                 The function first clears the usStore buffer and configures the 
*                 DTC transfers, triggered after completion of each A/D 
*                 conversion. 
* Argument     : none
* Return value : none
*******************************************************************************/
void InitDTC_FlashBGO(void)
{
    /* Clear the contents of the 'gDTC_Destination' array */
    if(memset(usStore, 0u, sizeof(usStore)) == NULL)
    {
        /* Halt in while loop when errors detected */
        while(1);
    }
        
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;
    
    /* Cancel the DTC module clock stop mode */
    MSTP_DTC = 0u;
    
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;        
    
    /* Ensure DTC is disabled */
    DTC.DTCST.BIT.DTCST = 0x0u;
    
    /* Reset the transfer skip flag */
    DTC.DTCCR.BIT.RRS = 0u;
    
    /* Use short address mode */
    DTC.DTCADMOD.BIT.SHORT = 0x1u;
    
    /* Set the base address for calculating the DTC vector table address */
    DTC.DTCVBR = (uint32_t*)dtc_vector_table;
    
    /* Write to the MRA and SAR registers:
       Source address is fixed
       Select repeat transfer mode
       16-bit word transfer
        */
    gDTC_S12AD_Transfer_Data[0] = ((uint32_t)0x50 << 24u) | \
                                                      ((uint32_t)DTC_SOURCE);
    
    /* Write to the MRB and DAR registers
       Transfer destination is the repeat area
       Destination address is incremented after each data transfer
       Chain transfers are disabled
       */
    gDTC_S12AD_Transfer_Data[1] = ((uint32_t)0x08 << 24u) | \
                                                 ((uint32_t)usStore);    
    
    /* Write to the CRA and CRB registers */
    gDTC_S12AD_Transfer_Data[2] = ((uint32_t)gNum_Transfers << 16u) \
                                                        & 0x00000000;
    
    /* Point to the S12AD vector in the DTC vector table */
    gpVector_Table_Pointer += VECT_S12AD_S12ADI0;
    
    /* Store the data start address */
    *gpVector_Table_Pointer = (uint32_t)gDTC_S12AD_Transfer_Data;
    
    /* Enable DTC trigger on ADC conversion end */
    DTCE(S12AD,S12ADI0) = 1u;
        
    /* Set the transfer skip flag */
    DTC.DTCCR.BIT.RRS = 1u;
            
    /* Enable DTC */
    DTC.DTCST.BIT.DTCST = 0x1u;
}
/*******************************************************************************
* End of function InitDTC_FlashBGO
*******************************************************************************/

/*******************************************************************************
* Outline      : InitADC_FlashBGO
* Description  : This function initialises the ADC unit to perform repeat
*                conversions of the potentiometer voltage.
* Argument     : none
* Return value : none
*******************************************************************************/
void InitADC_FlashBGO(void)
{    
    /* Use the AN000 (Potentiometer) pin as an I/O for peripheral functions */
    PORT4.PMR.BYTE = 0x01u;
    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503u;
    
    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0u;
        
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500u;        
            
    /* ADC clock = PCLK/8
       Continuous scan mode
       Enable scan end interrupts */
    S12AD.ADCSR.BYTE = 0x50u;    
                                    
    /* Enable automatic clearing of ADDR0 register after it's been read */
    S12AD.ADCER.BIT.ACE = 0x1u;
    
    /* Selects AN000 */
    S12AD.ADANS0.WORD = 0x0001u;
    
    /* Set S12AD interrupt level to 7 */
    IPR(S12AD, S12ADI0) = 0x7u;
    
    /* Enable S12AD interrupt requests */
    IEN(S12AD, S12ADI0) = 0x1u;
            
    /* Start ADC conversions */
    S12AD.ADCSR.BIT.ADST = 1u;
}
/*******************************************************************************
* End of function InitADC_FlashBGO
*******************************************************************************/

/*******************************************************************************
* Outline      : Erase_FlashBGO
* Description  : This function enters a for loop, and erases a block of data
*                 flash memory each iteration until all blocks have been erased.
* Argument     : none
* Return value : none
*******************************************************************************/
void Erase_FlashBGO(void)
{
    /* Turn on LED2 to indicate flash erase operation */
    LED2 = LED_ON;
    
    /* Declare flash API error flag */
    uint8_t ret = 0u; 
        
    /* Declare current data flash block variable */
    uint8_t current_block;
    
    /* Initialise a for loop to erase each of the data flash blocks */
    for(current_block = BLOCK_DB0; current_block < gNumFlashBlocks;
        current_block++)
    {
        /* Fetch beginning address of DF block */  
        uint32_t address = g_flash_BlockAddresses[current_block];
    
        /* Erase data flash block */
        ret |=     R_FlashErase
                (
                    current_block
                 );        
        
        /* Halt here if erase was unsuccessful */
        while(R_FlashGetStatus() != FLASH_SUCCESS);
                        
         /* Check Blank Checking */
         ret |= R_FlashDataAreaBlankCheck
                (
                    address,
                    BLANK_CHECK_ENTIRE_BLOCK
                );
    
        /* Halt here if check was unsuccessful */        
        while(R_FlashGetStatus() != FLASH_SUCCESS);
    }
          
    /* Halt in while loop when flash API errors detected */
    while(ret);
}    
/*******************************************************************************
* End of function Erase_FlashBGO
*******************************************************************************/

/*******************************************************************************
* Outline      : Write_FlashBGO
* Description  : This function writes the contents of usStore to the
*                 data flash, at the location pointed by gFlashWriteBuffer. If 
*                 the number of bytes to write to flash is not a multiple of 8, 
*                 the data is padded with null bytes (0x00) to make up to the 
*                 nearest multiple of 8.
* Argument     : none
* Return value : none
*******************************************************************************/
void Write_FlashBGO(void)
{
    /* Turn on LED1 to indicate flash write operation */
    LED1 = LED_ON;
    
    /* Declare flash API error flag */
    uint8_t status = 0x00u; 
        
    /* Declare the number of bytes variable, and initialise with the number of
       bytes the variable gFlashWriteBuffer contains */
    uint16_t num_bytes = (uint16_t)(sizeof(usStore));

    /* Check if flash is busy */    
    while(R_FlashGetStatus() == FLASH_BUSY);
    
    /* Write contents of write buffer to data flash */
    status |=     R_FlashWrite
                (
                    (uint32_t)gFlashWriteBuffer,
                    (uint32_t)usStore,
                    num_bytes
                );

    /* Halt in while loop when flash API errors detected */
    while(status);
    
    /* Halt here if check was unsuccessful */
    while(R_FlashGetStatus() != FLASH_SUCCESS);
    
    /* Only do the check if the ADC is stopped */
    if(gADC_Status == false)
    {    
        /* Compare memory locations to verify written data */    
        if(memcmp(gFlashWriteBuffer, usStore, (size_t)num_bytes))
        {
            /* Display flash data mismatch on the debug LCD */    
            Display_LCD(LCD_LINE1, "F.Data  ");
            Display_LCD(LCD_LINE2, "Mismatch");
        }
        else
        {
            /* Display flash data match on the debug LCD */    
            Display_LCD(LCD_LINE1, "F.Data  ");
            Display_LCD(LCD_LINE2, "Match   ");
        }
    
        /* Create a short delay display delay */
        Delay_s(2u);
    }
} 
/*******************************************************************************
* End of function Write_FlashBGO
*******************************************************************************/

/*******************************************************************************
* Outline      : SwitchPressHandler_BGO
* Description  : This function is called when a user switch is pressed. The 
*                function detects which switch was pressed, and calls the 
*                appropriate function.
* Argument     : none
* Return value : none
*******************************************************************************/
void SwitchPressHandler_BGO(void)
{
    /* Mask switch flag to ignore switch release bits */
    switch(gSwitchFlag & 0xF0u)
    {
        /* Switch 1 was pressed */
        case SWITCHPRESS_1:
        {
            /* Execute switch 1 press callback function */
            SW1PressCB();            
            
            /* Clear switch press flag */
            gSwitchFlag &= 0x0Fu;
            
            /* Exit switch */
            break;   
        }
        
        /* Switch 2 was pressed */
        case SWITCHPRESS_2:
        {
            /* Execute switch 2 press callback function */
            SW2PressCB();  
            
            /* Clear switch press flag */
            gSwitchFlag &= 0x0Fu;
            
            /* Exit switch */
            break;   
        }
        
        /* Switch 3 was pressed */
        case SWITCHPRESS_3:
        {
            /* Execute switch 3 press callback function */
            SW3PressCB();  
            
            /* Clear switch press flag */
            gSwitchFlag &= 0x0Fu;
            
            /* Exit switch */
            break;   
        }
        
        /* Default case, no switches pressed */
        default:
        {
            /* Exit switch */
            break;   
        }
    }
}
/*******************************************************************************
* End of function SwitchPressHandler_BGO
*******************************************************************************/

/*******************************************************************************
* Outline      : SW1PressCB
* Description  : Switch SW1 callback function. This function erases the data 
*                flash block, and writes calls the Write_FlashBGO function to 
*                write the contents of usStore to data flash. It then indicates
*                whether the operation was a success or failure on the debug 
*                LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
void SW1PressCB(void)
{
    /* Erase flash memory before writing to it */
    Erase_FlashBGO();
            
    /* Turn off LED 2 */
    LED2 = (uint8_t)LED_OFF;
        
    /* Write usStore data to flash memory */
    Write_FlashBGO();
            
    /* Check flash operation status returned by the FLASH interrupt */
    if(gWrite_Status == false)
    {
        /* Display instructions on the debug LCD */    
        Display_LCD(LCD_LINE1, "F.Write ");
        Display_LCD(LCD_LINE2, "Pass    ");
    }
    else
    {
        /* Display instructions on the debug LCD */    
        Display_LCD(LCD_LINE1, "F.Write ");
        Display_LCD(LCD_LINE2, "Fail    ");
    }
            
    /* Create a short delay display delay */
    Delay_s(2u);
            
    /* Display instructions on the debug LCD */    
    Display_LCD(LCD_LINE1, "FlashBGO");
    Display_LCD(LCD_LINE2, "Push SW ");           
            
    /* Turn off LED 1 */
    LED1 = (uint8_t)LED_OFF;
}
/*******************************************************************************
* End of function SW1PressCB
*******************************************************************************/

/*******************************************************************************
* Outline      : SW2PressCB
* Description  : Switch SW2 callback function. Toggle the ADC unit on or off
*                (depending on previous state) and indicate whether the ADC is
*                 running or not on the debug LCD.
*                LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
void SW2PressCB(void)
{
   /* Toggle the ADC conversion start bit to either 
      stop or restart conversions based on last state. */
   S12AD.ADCSR.BIT.ADST = (uint8_t)~(S12AD.ADCSR.BIT.ADST);
            
    /* Check if the ADC start bit is set */
    if(0x0 == S12AD.ADCSR.BIT.ADST)
    {
        /* Display instructions on the debug LCD */    
        Display_LCD(LCD_LINE1, "ADC Unit");
        Display_LCD(LCD_LINE2, "Stop    ");
        
        /* Set the flag to false if ADC is stopped */
        gADC_Status = false;
    }
    else
    {
        /* Display instructions on the debug LCD */    
        Display_LCD(LCD_LINE1, "ADC Unit");
        Display_LCD(LCD_LINE2, "Run     ");
            
        /* Set the flag to true if ADC is running */
        gADC_Status = true;
    }
            
    /* Create a short delay display delay */
    Delay_s(2u);
    
    /* Display instructions on the debug LCD */    
    Display_LCD(LCD_LINE1, "FlashBGO");
    Display_LCD(LCD_LINE2, "Push SW ");  
}
/*******************************************************************************
* End of function SW2PressCB
*******************************************************************************/

/*******************************************************************************
* Outline      : SW3PressCB
* Description  : Switch SW3 callback function. This function erases the flash
*                data block.
*                LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
void SW3PressCB(void)
{
    /* Erase flash memory */
    Erase_FlashBGO();
            
    /* Display instructions on the debug LCD */    
    Display_LCD(LCD_LINE1, "F.Erase ");
    Display_LCD(LCD_LINE2, "Pass    ");
                
    /* Create a short delay display delay */
    Delay_s(2u);
            
    /* Display instructions on the debug LCD */    
    Display_LCD(LCD_LINE1, "FlashBGO");
    Display_LCD(LCD_LINE2, "Push SW ");  
            
    /* Turn off LED 2 */
    LED2 = (uint8_t)LED_OFF;
            
    /* Turn off LED 3 */
    LED3 = (uint8_t)LED_OFF;
}
/*******************************************************************************
* End of function SW3PressCB
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashError
* Description  : This is a callback function used to display a message on the 
*                 LCD, indicating a flash access error. A delay is added to allow 
*                 the message to be read.
* Argument     : none
* Return value : none
*******************************************************************************/
void FlashError(void)
{
    /* Display instructions on the debug LCD */    
    Display_LCD(LCD_LINE2, "FlashERR");
    
    /* Wait in infinite while loop */
    while(1);
}
/*******************************************************************************
* End of function FlashError
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashWriteDone
* Description  : This is a user-defined callback function used to inidicate the 
*                 status of a flash write operation. Turns on LED1 to inidicate 
*                 that a flash write operation completed.
* Argument     : none
* Return value : none
*******************************************************************************/
void FlashWriteDone(void)                    
{    
    /* Turn on LED1 */
    LED1 = LED_ON;
    
    /* Set flag to true */
    gClear_LCD = true;
}
/*******************************************************************************
* End of function FlashWriteDone
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashEraseDone
* Description  : This is a user-defined callback function used to inidicate the 
*                 status of a flash erase operation. Turns on LED2 to indicate a 
*                 flash erase operation completed.
* Argument     : none
* Return value : none
*******************************************************************************/
#if 0
void FlashEraseDone(void)
{
    /* Turn on LED2 */
    LED2 = LED_ON;
    
    /* Set flag to true */
    gClear_LCD = true;
}
#endif
/*******************************************************************************
* End of function FlashEraseDone
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashBlankCheckDone
* Description  : This is a user-defined callback function used to inidicate the 
*                 status of a flash blank check operation. Turns on LED0 to 
*                 indicate that the flash is not blank, it is turned off to 
*                 indicate flash contains data.
* Argument     : uint8_t result - sent by the FlashReadyInterrupt function in 
*                                   file r_Flash_API_RX600.c
* Return value : none
*******************************************************************************/
void FlashBlankCheckDone(uint8_t result)
{
    /* Check the result returned by the API */
    if(!result)
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

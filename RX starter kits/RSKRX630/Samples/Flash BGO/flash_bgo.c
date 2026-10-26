/* Adapted for use with IAR Embedded Workbench */
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
* File Name      : flash_bgo.c 
* Version     : 1.00
* Device     : R5F5630E
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX630
* Description    : Defines flash and ADC functions used in this sample.
******************************************************************************/
/******************************************************************************
* History     : 23 Jan. 2012  Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* System Includes
******************************************************************************/
/* Provides standard string function definitions used in this file */
#include <string.h>

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines RX630 port registers */
#include "iorx630.h"
/* Defines macros relating to the RX630 user LEDs and switches */
#include "rskrx630def.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Declares prototypes for functions defined in this file */
#include "flash_bgo.h"
/* Defines flash API functions used in this file */
#include "r_Flash_API_RX600.h"
/* Defines options for customization of the flash API */
#include "r_Flash_API_RX600_UserConfig.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Define the data source for the DTC transfer as the ADC result register */ 
#define DTC_SOURCE ((uint16_t *)&S12AD.ADDR0)

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
extern const uint32_t block_addresses[86];

/* Variable to store ADC result transfered using DTC */
uint16_t gStore[256];
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
bool gClear_LCD = false;
/* Declare a flag to indicate flash write operation statuses */
bool gWrite_Status = false;
/* Declare a flag to indicate ADC status */
bool gADC_Status = true;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* ADC initialisation function prototype declaration */
void Init_FlashBGO(void);
/* ADC initialisation function prototype declaration */
void Init_S12AD(void);
/* DTC initialisation function prototype declaration */
void Init_DTC(void);

/*******************************************************************************
* Outline      : Init_FlashData
* Description  : This function initialises the MCU flash area, allowing it to be
*         read and written to by user code. The function then calls the
*         Init_DTC function to configure the DTC and ADC units used in
*         the project. Finally the function erases the contents of the
*         data flash.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_FlashBGO(void)
{  
  /* Enable MCU access to the data flash area */
    R_FlashDataAreaAccess(0xFFFF, 0xFFFF);
  
  /* Initialise the DTC unit */
  Init_DTC();
  
  /* Clear the contents of the flash write buffer array */
  memset(gStore, 0x00, sizeof(gStore));
    
  /* Erase flash memory before writing to it */
  Erase_FlashData();
}
/*******************************************************************************
* End of function Init_FlashData
*******************************************************************************/

/*******************************************************************************
* Outline      : Erase_FlashData
* Description  : This function enters a for loop, and erases a block of data
*         flash memory each iteration until all blocks have been erased.
* Argument     : none
* Return value : none
*******************************************************************************/
void Erase_FlashData(void)
{
  /* Declare flash API error flag */
  uint8_t ret = 0;
  
  /* Declare loop count and block count variables */
  uint8_t loop_counter = 0;
  uint8_t block_count = BLOCK_DB0;
  
  /* Declare an address variable */
  uint32_t address = 0x00000000;
  
  /* Initialise a for loop to erase each of the data flash blocks */
  for(loop_counter = 0; loop_counter < DF_NUM_BLOCKS; loop_counter++)
    {
        /* Fetch beginning address of DF block */  
        address = block_addresses[BLOCK_DB0 + loop_counter];
  
    /* Erase data flash block */
        ret |=   R_FlashErase
        (
          block_count
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

    /* Increment block counter */
    block_count++;
  }
      
  /* Halt in while loop when flash API errors detected */
  while(ret);
}  
/*******************************************************************************
* End of function Erase_FlashData
*******************************************************************************/

/*******************************************************************************
* Outline      : Write_FlashData
* Description  : This function writes the contents of gStore to the
*         data flash, at the location pointed by gFlashWriteBuffer. If 
*         the number of bytes to write to flash is not a multiple of 8, 
*         the data is padded with null bytes (0x00) to make up to the 
*         nearest multiple of 8.
* Argument     : none
* Return value : none
*******************************************************************************/
void Write_FlashData(void)
{
  /* Declare flash API error flag */
  uint8_t status = 0x00; 
    
  /* Declare the number of bytes variable, and initialise with the number of
     bytes the variable gFlashWriteBuffer contains */
  uint16_t num_bytes = (uint16_t)(sizeof(gStore));

  /* Check if flash is busy */  
  while(R_FlashGetStatus() == FLASH_BUSY);
  
  /* Write contents of write buffer to data flash */
    status |=   R_FlashWrite
      (
        (uint32_t)gFlashWriteBuffer,
        (uint32_t)gStore,
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
    if(memcmp(gFlashWriteBuffer, gStore, (size_t)num_bytes))
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
    DisplayDelay((uint32_t)0x1FFFF);
  }
} 
/*******************************************************************************
* End of function Write_FlashData
*******************************************************************************/

/*******************************************************************************
* Outline      : Init_DTC
* Description  : This function initialises the DTC unit to perform 256 repeat 
*         transfers of S12AD A/D conversions stored in gStore buffer. 
*         The function first clears the gStore buffer and configures the 
*         DTC transfers, triggered after completion of each A/D 
*         conversion. The S12AD module is then configured in continuous 
*         scan mode with interrupt requests enabled.  
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_DTC(void)
{
  /* Clear the contents of the 'gDTC_Destination' array */
  if(memset(gStore, 0u, sizeof(gStore)) == NULL)
  {
    /* Halt in while loop when errors detected */
    while(1);
  }
  
  /* Use the AN000 (Potentiometer) pin as an I/O for peripheral functions */
  PORT4.PMR.BYTE = 0x01;
  
  /* Protection off */
  SYSTEM.PRCR.WORD = 0xA503;
  
  /* Cancel the S12AD module clock stop mode */
  MSTP_S12AD = 0;
  
  /* Cancel the DTC module clock stop mode */
  MSTP_DTC = 0;
  
  /* Protection on */
  SYSTEM.PRCR.WORD = 0xA500;    
  
  /* Ensure DTC is disabled */
  DTC.DTCST.BIT.DTCST = 0x0;
  
  /* Reset the transfer skip flag */
  DTC.DTCCR.BIT.RRS = 0;
  
  /* Use short address mode */
  DTC.DTCADMOD.BIT.SHORT = 0x1;
  
  /* Set the base address for calculating the DTC vector table address */
  DTC.DTCVBR = dtc_vector_table;
  
  /* Write to the MRA and SAR registers:
     Source address is fixed
     Select repeat transfer mode
     16-bit word transfer
      */
  gDTC_S12AD_Transfer_Data[0] = ((uint32_t)0x50 << 24) | \
                                                    ((uint32_t)DTC_SOURCE);
  
  /* Write to the MRB and DAR registers
     Transfer destination is the repeat area
     Destination address is incremented after each data transfer
     Chain transfers are disabled
     */
  gDTC_S12AD_Transfer_Data[1] = ((uint32_t)0x08 << 24) | \
                                               ((uint32_t)gStore);  
  
  /* Write to the CRA and CRB registers */
  gDTC_S12AD_Transfer_Data[2] = ((uint32_t)gNum_Transfers << 16) \
                              & 0x00000000;
  
  /* Point to the S12AD vector in the DTC vector table */
  gpVector_Table_Pointer += VECT_S12AD_S12ADI0;
  
  /* Store the data start address */
  *gpVector_Table_Pointer = (uint32_t)gDTC_S12AD_Transfer_Data;
  
  /* Enable DTC trigger on ADC conversion end */
  DTCE(S12AD,S12ADI0) = 1;
    
  /* Set the transfer skip flag */
  DTC.DTCCR.BIT.RRS = 1;
  
  /* Configure the 12-bit S12AD unit */
      
  /* ADC clock = PCLK/8
     Continuous scan mode
     Enable scan end interrupts */
  S12AD.ADCSR.BYTE = 0x50;  
                  
  /* Enable automatic clearing of ADDR0 register after it's been read */
  S12AD.ADCER.BIT.ACE = 0x1;
  
  /* Selects AN000 */
  S12AD.ADANS0.WORD = 0x0001;
  
  /* Set S12AD interrupt level to 7 */
  IPR(S12AD, S12ADI0) = 0x7;
  
  /* Enable S12AD interrupt requests */
  IEN(S12AD, S12ADI0) = 0x1;
    
  /* Enable DTC */
  DTC.DTCST.BIT.DTCST = 0x1;
}
/*******************************************************************************
* End of function Init_DTC
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashError
* Description  : This is a callback function used to display a message on the 
*         LCD, indicating a flash access error. A delay is added to allow 
*         the message to be read.
* Argument     : none
* Return value : none
*******************************************************************************/
void FlashError(void)
{
  /* Display instructions on the debug LCD */  
  Display_LCD(LCD_LINE2, "FlashERR");
  
  /* Create a short delay display delay */
  DisplayDelay(0x1FFFF);
}
/*******************************************************************************
* End of function FlashError
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashWriteDone
* Description  : This is a user-defined callback function used to inidicate the 
*         status of a flash write operation. Turns on LED1 to inidicate 
*         that a flash write operation completed.
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
*         status of a flash erase operation. Turns on LED2 to indicate a 
*         flash erase operation completed.
* Argument     : none
* Return value : none
*******************************************************************************/
void FlashEraseDone(void)
{
  /* Turn on LED2 */
  LED2 = LED_ON;
  
  /* Set flag to true */
  gClear_LCD = true;
}
/*******************************************************************************
* End of function FlashEraseDone
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashBlankCheckDone
* Description  : This is a user-defined callback function used to inidicate the 
*         status of a flash blank check operation. Turns on LED0 to 
*         indicate that the flash is not blank, it is turned off to 
*         indicate flash contains data.
* Argument     : uint8_t result - sent by the FlashReadyInterrupt function in 
*                   file r_Flash_API_RX600.c
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

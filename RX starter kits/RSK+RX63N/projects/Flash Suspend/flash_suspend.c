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
* File Name     : flash_suspend.c 
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines flash suspend functions used in this sample.
******************************************************************************/
/******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* System Includes
******************************************************************************/
/* Provides standard string function definitions used in this file */
#include <string.h>
/* Defines machine level functions used in this file */
#include <machine.h>

/******************************************************************************
* Project Includes
******************************************************************************/
/* Defines macros relating to the RX63N user LEDs and switches */
#include "rskrx63ndef.h"
/* Defines switch functions and variables used in this file */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Declares prototypes for functions defined in this file */
#include "flash_suspend.h"
/* Defines flash API functions used in this file */
#include "r_Flash_API_RX600.h"        

/******************************************************************************
* Global Variables
******************************************************************************/
/* Flash ready flag variable */
volatile bool gFlashReady = false;
/* Switch status flag variable (imported) */
extern volatile uint8_t gSwitchFlag;
/* Declare command pointer in dataflash area */
uint8_t * const fcu_cmd = (uint8_t *) FCU_DF_CMD_ADDR;
/* Total number of data flash blocks */
uint16_t gNumFlashBlocks = sizeof(g_flash_BlockAddresses)/4u;
            
/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Suspend flash program/erase operations function prototype */
bool SuspendPE_FlashSuspend(void);
/* Resume flash program/erase operations function prototype */
bool ResumePE_FlashSuspend(void);
/* Switch interrupt callback function prototype */
void SwitchCB_FlashSuspend(void);
/* Dataflash erase function prototype */
void Erase_FlashSuspend(void);

/*******************************************************************************
* Outline      : Init_FlashSuspend
* Description  : This function initialises the MCU flash area, allowing it to be
*                 read and written to by user code. The function then calls the
*                 Erase_FlashSuspend function which erases the entire dataflash
*                 area. The function finally sets switch callback function.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_FlashSuspend(void)
{
    /* Enable MCU access to the data flash area */
    R_FlashDataAreaAccess(0xFFFFu, 0xFFFFu);
    
    /* Erase flash memory before writing to it */
    Erase_FlashSuspend();
    
    /* Configure switch callback function */
    SetSwitchReleaseCallback(SwitchCB_FlashSuspend);
}
/******************************************************************************
* End of function Init_FlashSuspend
******************************************************************************/

/*******************************************************************************
* Outline      : FlashBlankCheckDone
* Description  : This is a callback function (prototype in flash API file
*                 r_Flash_API_RX600). Function is called every time a blank check
*                 is complete, and sets the gFlashReady flag true.
* Argument     : none
* Return value : none
*******************************************************************************/
void FlashBlankCheckDone(uint8_t result)
{
    /* Perform dummy read/write of variable to supress warning C5826 */
    result &= result;

    /* Set flash ready flag true */
    gFlashReady = true;
}
/*******************************************************************************
* End of function FlashBlankCheckDone
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashEraseDone
* Description  : This is a callback function (prototype in flash API file
*                 r_Flash_API_RX600). Function is called every time a flash erase
*                 is complete, and sets the gFlashReady flag true.
* Argument     : none
* Return value : none
*******************************************************************************/
void FlashEraseDone(void)
{
    /* Set flash ready flag true */
    gFlashReady = true;
}
/*******************************************************************************
* End of function FlashEraseDone
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashError
* Description  : This is a callback function (prototype in flash API file
*                 r_Flash_API_RX600). Function is called every time a FCU flash
*                 error occurs. The function waits in an infinite while loop.
* Argument     : none
* Return value : none
*******************************************************************************/
void FlashError(void)
{
    /* FCU error occured, wait in infinite while loop */
    while(1);
}
/*******************************************************************************
* End of function FlashError
*******************************************************************************/

/*******************************************************************************
* Outline      : FlashWriteDone
* Description  : This is a callback function (prototype in flash API file
*                 r_Flash_API_RX600). Function is called every time a flash write
*                 is complete, and sets the gFlashReady flag true.
* Argument     : none
* Return value : none
*******************************************************************************/
void FlashWriteDone(void)
{
    /* Set flash ready flag true */
    gFlashReady = true;
}
/*******************************************************************************
* End of function FlashWriteDone
*******************************************************************************/

/*******************************************************************************
* Outline      : Erase_FlashSuspend
* Description  : This function enters a for loop, and erases a block of data
*                 flash memory each iteration until all blocks have been erased.
* Argument     : none
* Return value : none
*******************************************************************************/
void Erase_FlashSuspend(void)
{
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
* End of function Erase_FlashSuspend
*******************************************************************************/

/*******************************************************************************
* Outline      : SwitchCB_FlashSuspend
* Description  : This is a callback function which is triggered when a user 
*                 releases any of the user switches. The function first checks 
*                 which switch was released, and performs a flash write if SW1
*                 was pressed and a flash erase if SW3 was pressed. The flash
*                 write is suspended for approximately 5 seconds whilst the user
*                 LEDs flash, after which the write is resumed.
* Argument     : none
* Return value : none
*******************************************************************************/
void SwitchCB_FlashSuspend(void)
{    
    /* Check if SW1 was pressed */
    if(gSwitchFlag & SWITCHPRESS_1)
    {
        /* Declare local variable flash write buffer */
        static uint8_t flash_write_buffer[16u];
        
        /* Declare local static write count variable */
        static uint8_t write_count = 0u;
        
        /* Fill the flash write buffer with the write count value */
        for(uint8_t i = 0; i < 16u; i++)
        {
            /* Copy write count into buffer array element */
            flash_write_buffer[i] = write_count;
        }        
    
        /* Start flash write at incrementing 16byte locations */
        R_FlashWrite((uint32_t)(0x00100000 + (16u * write_count)), 
                                (uint32_t) flash_write_buffer, 16u);
 
        /* Increment the write count */
        write_count++;
        
        /* Wait a short time for the flash write to be mid-way through */
        Delay_us(2u);
        
        /* Suspend the flash write, and check it was successful */
        if(SuspendPE_FlashSuspend())
        {        
            /* Flash write has now been suspended, indicate this on debug LCD */
            Display_LCD(LCD_LINE1, "F.Write ");
            Display_LCD(LCD_LINE2, "Suspend ");
        }
        /* Suspend operation failed */
        else
        {
            /* Indicate on debug LCD suspend operation failed */
            Display_LCD(LCD_LINE1, "Suspend ");
            Display_LCD(LCD_LINE2, "Failed  ");
            
            /* Wait in an infinite while loop */
            while(1);
        }
        
        /* Turn off all user LEDs */
        LED0 = LED1 = LED2 = LED3 = 1;
        
        /* Iterate through loop 10 time, toggling the user LEDs with a delay */
        for(uint8_t i = 0u; i < 10u; i++)
        {
            /* Wait before toggle the user LEDs again */
            Delay_us(250000u);
            
            /* Toggle the user LEDs */
            if(LED_ON == LED0)
            {
                /* LEDs are on, so turn them off */
                LED0 = LED1 = LED2 = LED3 = LED_OFF;
            }
            else
            {
                /* LEDs are off, so turn them on */
                LED0 = LED1 = LED2 = LED3 = LED_ON;
            }
        }
        
        /* Resume the flash write operation, and check operation was successful */
        if(ResumePE_FlashSuspend())
        {
            /* Poll the FCU status, and wait until last operation is complete */
             while(!gFlashReady)
        
            /* Update debug LCD to indicate flash write now complete */
            Display_LCD(LCD_LINE1, "F.Write ");
            Display_LCD(LCD_LINE2, "Resumed ");
        }
        /* Resume operation failed */
        else
        {
            /* Indicate on debug LCD resume operation failed */
            Display_LCD(LCD_LINE1, "Resume  ");
            Display_LCD(LCD_LINE2, "Failed  ");
            
            /* Wait in an infinite while loop */
            while(1);    
        }
    }
    /* Check if SW3 was pressed */
    else if(gSwitchFlag & SWITCHPRESS_3)
    {
        /* Erase the first virtual flash block */
        R_FlashErase(BLOCK_DB0);

         /* Poll the FCU status, and wait until last operation is complete */
         while(!gFlashReady);
        
        /* Indicate erase operation complete on debug LCD */
        Display_LCD(LCD_LINE1, "F.Block ");
        Display_LCD(LCD_LINE2, "Erased  ");
    }
        
    /* Wait a short time to allow user to read last LCD message */
    Delay_s(2u);
    
    /* Revert the debug LCD back to its original message */
    Display_LCD(LCD_LINE1, "F. Susp.");
    Display_LCD(LCD_LINE2, "Push SW1");
    
    /* Clear the switch flag */
    gSwitchFlag = 0x00;
}
/*******************************************************************************
* End of function SwitchCB_FlashSuspend
*******************************************************************************/

/*******************************************************************************
* Outline      : SuspendPE_FlashSuspend
* Description  : This function suspends a flash write operation midway through
*                 completion to allow it to be resumed later. The function checks
*                 flash status registers to confirm the suspend was successful or
*                 not, and returns true or false accordingly.
* Argument     : none
* Return value : bool - Returns true if suspend was successful, otherwise false.
*******************************************************************************/
bool SuspendPE_FlashSuspend(void)
{    
    /* Wait for until the suspend ready bit is 1 */
    uint32_t timeout = 0u;
    
    /* Wait until FCU is ready to receive suspend command */
    while(!FLASH.FSTATR0.BIT.SUSRDY)
    {
        /* Increment timeout counter, and check if its above threshold */
        if(timeout++ > FCU_SUSRDY_TIMEOUT)
        {
            /* Timeout occured, break out of while loop */
            return false;
        }
    }

    /* Declare command pointer in dataflash area */
    uint8_t * const fcu_cmd = (uint8_t *) FCU_DF_CMD_ADDR;
    
    /* Send suspend command */
    *fcu_cmd = FCU_PE_SUSPEND;
    
    /* Wait for FRDY bit to set to 1 (suspend was successful) */
    timeout = 0;
    while(!(FLASH.FSTATR0.BYTE & 0x80u))
    {
        /* Increment timeout counter, and check if its above threshold */
        if(timeout++ > FCU_FRDY_TIMEOUT)
        {
            /* Timeout occured, break out of while loop */
            return false;
        }    
    }
    
    /* Suspend operation successful, return true */
    return true;
}
/*******************************************************************************
* End of function SuspendPE_FlashSuspend
*******************************************************************************/

/*******************************************************************************
* Outline      : ResumePE_FlashSuspend
* Description  : This function resumes a suspended flash write operation. The
*                 function checks flash status registers to confirm the resume
*                 was successful or not, and returns true or false accordingly.
* Argument     : none
* Return value : bool - Returns true if resume was successful, otherwise false.
*******************************************************************************/
bool ResumePE_FlashSuspend(void)
{        
    /* Declare command pointer in dataflash area */
    uint8_t * const fcu_cmd = (uint8_t *)FCU_DF_CMD_ADDR;
    
    /* Send suspend command */
    *fcu_cmd = FCU_PE_RESUME;
    
    /* Wait a few nops for command to be received */
    nop();
    nop();
    nop();
    
    /* Check if an illegal operation was detected */
    if(FLASH.FSTATR0.BYTE & 0x70)
    {
        /* Illegal operation detected, return false */
        return false;
    }
    else
    {    
        /* Resume operation successful, return true */
        return true;
    }
}
/*******************************************************************************
* End of function ResumePE_FlashSuspend
*******************************************************************************/

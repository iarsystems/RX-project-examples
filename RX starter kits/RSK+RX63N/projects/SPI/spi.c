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
* File Name     : spi.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines SPI functions used in this file. 
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides standard boolean type definitions. */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines SPI driver functions used in this file */
#include "r_spiflash_driver.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines switch functions used in this file */
#include "switch.h"
/* Defines string functions used in this file */
#include "string.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Write data buffer, global variable */
uint8_t gWriteData_SPI[32u] = "X. RENESAS RSK+RX63N SPI FLASH  ";
/* Read data buffer, global variable */
uint8_t gReadData_SPI[32u] = {0};
/* Switch flag variable, imported global variable */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Static local switch callback function */
static void SwitchCB_SPI(void);

/*******************************************************************************
* Outline       : Init_SPI
* Description   : Initialises the SPI unit, and the SPI Flash memory device.
*                 The function also disables memory protection, and erases the
*                 contents of the first 32KByte data block.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_SPI(void)
{    
    /* Call the SPI Flash initialise function, and check if it fails */
    if( !R_SPIFLASH_Initialise() )
    {
        /* SPI Flash initialisation failed, display failure message on LCD */
        Display_LCD(LCD_LINE1, "SPI Init");
        Display_LCD(LCD_LINE2, "Failure ");
        
        /* Cannot continue with the sample, wait in infinite while loop */
        while(1);
    }
    
    /* Disable flash memory protection (enable write & erase operations) */
    R_SPIFLASH_MemoryProtection(false);
    
    /* Erase the contents of the first 32Kbyte block */
    R_SPIFLASH_EraseBlock(1u);
    
    /* Configure the switch callback function */
    SetSwitchPressCallback(SwitchCB_SPI);
}
/*******************************************************************************
* End of function Init_SPI
*******************************************************************************/

/*******************************************************************************
* Outline       : SwitchCB_SPI
* Description   : This is a switch callback function, which is execute when a
*                 user switch is pressed. If switch SW1 is pressed, the function
*                 writes the contents of gWriteData_SPI to the first location in
*                 first memory block. After each write, the ascii number at start
*                 of the string is incremented. If the write fails, this is 
*                 indicated on the debug LCD. If SW2 is pressed, the function
*                 reads back the data in the write location. If the read data
*                 doesn't match the write data, it displays an error on the LCD.
* Argument      : none
* Return value  : none
*******************************************************************************/
void SwitchCB_SPI(void)
{
    /* Start switch structure, depending on the switch flag variable */
    switch( gSwitchFlag & 0x0Fu )
    {
        /* If switch SW1 has been pushed down */
        case SWITCHHOLD_1:
        {
            /* Declare variable to store ascii write number value */
            static uint8_t ascii_write_num = '0';
            
            /* Declare variable to store address of first block */
            uint32_t address = {0};
            
            /* Fetch the starting address of the first memory block */
            R_SPIFLASH_GetBlockAddress(1u, &address);
            
            /* Insert ascii write number into write data array */
            gWriteData_SPI[0u] = ascii_write_num;
            
            /* Erase the block before writing */
            R_SPIFLASH_EraseBlock(address);
            
            /* Write the contents of gWriteData_SPI to the SPI flash device,
               and check if the operation is successful */
            if( R_SPIFLASH_Write(gWriteData_SPI, address, 32u) )
            {
                /* Declare buffer to hold lcd data string */
                uint8_t lcd_buffer[9] = "Wrote  X\0";
                
                /* Insert ascii write number into LCD buffer string */
                lcd_buffer[7u] = ascii_write_num;
                
                /* Display LCD buffer on debug LCD */
                Display_LCD(LCD_LINE2, lcd_buffer);
                
                /* Increment ascii write number */
                ascii_write_num++;                
                
                /* Check if ascii write number has gone past 9 */
                if(ascii_write_num > '9')
                {
                    /* Set write number back to 0 */
                    ascii_write_num = '0';
                }
            }
            /* Flash write operation was not successful */
            else
            {
                /* Write operation failed, indicate this on the debug LCD */
                Display_LCD(LCD_LINE2, "WR. Fail");
            }
        } break;
        
        /* If switch SW2 has been pushed down */
        case SWITCHHOLD_2:
        {
            /* Declare variable to store address of first block */
            uint32_t address = {0};
            
            /* Fetch the starting address of the first memory block */
            R_SPIFLASH_GetBlockAddress(1u, &address);
            
            /* Set each element in the read data buffer to 0x00 */
            memset(gReadData_SPI, 0x00, 32u);
            
            /* Perform SPI flash read */
            R_SPIFLASH_Read(gReadData_SPI, address, 32u);
            
            /* Perform if gWriteData_SPI matches gReadData_SPI exactly
               (read successful) */
            if( 0u == memcmp(gWriteData_SPI, gReadData_SPI, 32u) )
            {
                /* Declare buffer to hold lcd data string */
                uint8_t lcd_buffer[9] = "Read   X\0";
                
                /* Pull write number from read data, insert into lcd buffer */
                lcd_buffer[7u] = gReadData_SPI[0u];
                
                /* Display LCD buffer on debug LCD */
                Display_LCD(LCD_LINE2, lcd_buffer);
            }
            /* Data mismatch between read and write found */
            else
            {
                /* Read error found, indicate this on debug LCD */
                Display_LCD(LCD_LINE2, "R.  Fail");
            }
        } break;
        
        /* Default branch (perform no operation) */
        default: break;
    }
}
/*******************************************************************************
* End of function SwitchCB_SPI
*******************************************************************************/

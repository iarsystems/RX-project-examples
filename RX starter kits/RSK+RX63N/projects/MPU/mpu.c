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
* File Name     : mpu.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines MPU functions used in this sample. The MPU protection
*                 regions are mapped as follows:
*    
*                  |-------------| 0x0000 0000
*                  | Region 0    |
*                  | Full Access |
*                  |-------------| 0x0000 3000
*                  |-------------| 0x0000 3000
*                  | Region 1    |
*                  | Controlled  |
*                  | Access      |
*                  |-------------| 0x0000 3080
*                  |-------------| 0x0000 3080
*                  | Region 2    |
*                  | Full Access |
*                  |-------------| 0xFFFF FFFF
*
*                 Region 1 is the location of the MEMORY_BLOCK
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Defines standard integer types used in this file */
#include <stdint.h>
/* Defines standard boolean types used in this file */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Provides declarations for functions defined in this file */
#include "mpu.h"
/* Defines MCU registers used in this file */
#include "iorx63n.h"
/* Defines switch functions used in this file */
#include "switch.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Provides definitions for RSK board LEDs and switches */
#include "rskrx63ndef.h"

/*******************************************************************************
* Preprocessor Directives
*******************************************************************************/
/* Define MPU_BLOCK section, starting at 0000 3000 */
#pragma location = 0x00003000

/* Declare 256 byte array memory block to fille MPU_BLOCK section */
volatile uint8_t gMemoryBlock[256u];

/* End of MPU_BLOCK section */

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Import switch flag global variable */
extern volatile uint8_t gSwitchFlag;
/* Supervisor command global variable */
volatile uint8_t gSupervisorCommand = NO_COMMAND;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Supervisor mode function prototype */
void SupervisorMode_MPU(uint8_t);

/******************************************************************************
* Outline       : Init_MPU
* Description   : Function initialises the MPU unit by calling the 
*                 SupervisorMode_MPU function and passing the command
*                 'INITIALISE_MPU'. 
* Argument      : none
* Return value  : none
******************************************************************************/
void Init_MPU(void)
{
    /* Initialise the MPU unit */
    SupervisorMode_MPU(INITIALISE_MPU);
}
/******************************************************************************
* End of function Init_MPU
******************************************************************************/

/******************************************************************************
* Outline       : SupervisorMode_MPU
* Description   : This function parses the command variable input and, if a 
*                 recognised command, initiates the software interrupt and sets
*                 the global variable gSupervisorCommand to the appropriate
*                 value. 
* Argument      : uint8_t command - This input should be passed using the
*                                   commands defined in mpu.h
*
*                 INITIALISE_MPU            : Initialises the MPU unit
*                 ENABLE_MEMORY_PROTECTION  : Enable protection of address
*                                              region gMemoryBlock is located.
*                 DISABLE_MEMORY_PROTECTION : Disable protection of the address
*                                              region gMemoryBlock is located.                                                                                
* Return value  : none
******************************************************************************/
void SupervisorMode_MPU(uint8_t command)
{
    /* Enable software interrupt */
    IPR(ICU, SWINT) = 1u;
    IEN(ICU, SWINT) = 1u;
    IR(ICU, SWINT) = 0;
    
    /* Start switch structure, based on command input variable */
    switch(command)
    {
        /* Initialise command passed */
        case INITIALISE_MPU:
        {
            /* Set supervisor command variable */
            gSupervisorCommand = INITIALISE_MPU;
            
            /* Initiate software interrupt to enter supervisor mode */
            ICU.SWINTR.BIT.SWINT = 1u;
            
            /* Exit switch case */
            break;
        }
        /* Enable memory protection command passed */
        case ENABLE_MEMORY_PROTECTION:
        {
            /* Set supervisor command variable */
            gSupervisorCommand = ENABLE_MEMORY_PROTECTION;  
            
            /* Initiate software interrupt to enter supervisor mode */
            ICU.SWINTR.BIT.SWINT = 1u;
            
            /* Exit switch case */
            break;
        }
        /* Disable memory protection command passed */
        case DISABLE_MEMORY_PROTECTION:
        {
            /* Set supervisor command variable */
            gSupervisorCommand = DISABLE_MEMORY_PROTECTION; 
            
            /* Initiate software interrupt to enter supervisor mode */
            ICU.SWINTR.BIT.SWINT = 1u;
            
            /* Exit switch case */
            break;
        }
        /* Default switch branch */
        default:
        {
            /* No recognised command, set to NO_COMMAND */
            gSupervisorCommand = NO_COMMAND;
            
            /* Exit switch case */
            break;
        }
    }
}    
/******************************************************************************
* End of function SupervisorMode_MPU
******************************************************************************/     

/******************************************************************************
* Outline       : Excep_ICU_SWINT
* Description   : Software interrupt handler function. The software interrupt
*                 is triggered by the function SupervisorMode_MPU. This ISR
*                 checks the gSupervisorCommand variable, and if it matches a 
*                 known command it performs the appropriate action. This ISR
*                 is used to set registers which can only be set when the MCU
*                 is in supervisor mode.
* Argument      : none
* Return value  : none
******************************************************************************/
#pragma vector=VECT_ICU_SWINT
__interrupt void Excep_ICU_SWINT(void)
{    
    /* Start switch structure based on gSupervisorCommand variable */
    switch(gSupervisorCommand)
    {
        /* Initialise command passed */
        case INITIALISE_MPU:
        {
            /* Turn off memory access protection */
            MPU.MPEN.BIT.MPEN = 0u;
            
            /* Disable all protection regions */
            MPU.MPOPI.BIT.INV = 1u;
            
            /* Deny execute/read/write access to entire address range */
            MPU.MPBAC.BIT.UBAC = NO_ACCESS;
                        
            /* Set region 0 start address */
            MPU.RSPAGE0.BIT.RSPN = REGION0_START_ADDR;

            /* Set region 0 end address */
            MPU.REPAGE0.BIT.REPN = REGION0_END_ADDR;
            
            /* Set region 0 to full read/write/execute access */
            MPU.REPAGE0.BIT.UAC = READ_ENABLE|WRITE_ENABLE|EXEC_ENABLE;
            
            /* Set region 1 start address */
            MPU.RSPAGE1.BIT.RSPN = REGION1_START_ADDR;
            
            /* Set region 1 end address */
            MPU.REPAGE1.BIT.REPN = REGION1_END_ADDR;
            
            /* Set region 1 to full read/write/execute access */
            MPU.REPAGE1.BIT.UAC = READ_ENABLE|WRITE_ENABLE|EXEC_ENABLE;
            
            /* Set region 2 start address */
            MPU.RSPAGE2.BIT.RSPN =  REGION2_START_ADDR;
            
            /* Set region 2 end address */
            MPU.REPAGE2.BIT.REPN = REGION2_END_ADDR;
            
            /* Set region 2 to full read/write/execute access */
            MPU.REPAGE2.BIT.UAC = READ_ENABLE|WRITE_ENABLE|EXEC_ENABLE;
            
            /* Set regions 0, 1 & 2 settings as valid */
            MPU.REPAGE0.BIT.V = 1u;
            MPU.REPAGE1.BIT.V = 1u;
            MPU.REPAGE2.BIT.V = 1u;
            
            /* Turn on protection regions */
            MPU.MPEN.BIT.MPEN = 1u;
             
            /* Exit case structure*/
            break;
        }
        /* Enable memory protection command passed */
        case ENABLE_MEMORY_PROTECTION:
        {
            /* Set region 1 access to read only */
            MPU.REPAGE1.BIT.UAC = READ_ENABLE;
            
            /* Exit case structure */
            break;
        }
        /* Enable memory protection command passed */
        case DISABLE_MEMORY_PROTECTION:
        {
            /* Set region 1 access to full read/write/execute */
            MPU.REPAGE1.BIT.UAC = READ_ENABLE|WRITE_ENABLE|EXEC_ENABLE;

            /* Exit case structure */
            break;       
        }
        /* Default switch branch */
        default:
        {
            /* Exit case structure */
            break;
        }
    }
    
    /* Reset Supervisor command variable back to NO_COMMAND */
    gSupervisorCommand = NO_COMMAND;
}
/******************************************************************************
* End of function SupervisorMode_MPU
******************************************************************************/   

/******************************************************************************
* Outline       : SwitchPoll_MPU
* Description   : This function is called repeatedly in the main while(1) loop.
*                 The function polls the switches, and writes to increment
*                 memory locations in the MEMORY_BLOCK section when SW1 is
*                 pressed. When SW2 is pressed, the protection access to the 
*                 MEMORY_BLOCK is toggled between full read/write/execute and
*                 read only.
* Argument      : none
* Return value  : none
******************************************************************************/
void SwitchPoll_MPU(void)
{
    /* Check which switch was pressed */
    switch(gSwitchFlag & 0x0F)
    {
        /* Switch SW1 was pressed */
        case SWITCHHOLD_1:
        {            
            /* Declare data write pointer to start at beginning of the
               MEMORY_BLOCK section */
            static volatile uint8_t * write_ptr = gMemoryBlock;
            
            /* Declare write data variable */
            static volatile uint8_t write_data = 0x11u;
            
            /* Write data to the pointer location */
            *write_ptr = write_data;
            
            /* Verify the correct data was written */
            if(write_data == *write_ptr)
            {   
                /* Increment the write pointer and write data */
                write_ptr++;
                write_data++;
                                
                /* Declare temporary lcd buffer */
                uint8_t lcd_buffer[9u] = "xxxxxxxx";
                
                /* Convert address of write point into hex string, and insert
                   into the lcd buffer */
                u32ToString(lcd_buffer, 0, (uint32_t)write_ptr);
                
                /* Display write pass and address of successful write
                   on debug LCD */
                Display_LCD(LCD_LINE1, "WR PASS ");
                Display_LCD(LCD_LINE2, lcd_buffer);
                
                /* Add delay to allow read to read previous message */
                Delay_s(2u);
            }
            /* Data was not written correctly */
            else
            {
                /* Wait in an infinite while loop */
                while(1);        
            }
        
            /* Exit from switch case */
            break;
        }
        
        /* Switch SW2 was pressed */
        case SWITCHHOLD_2:
        {
            /* Initialise memory protection flag variable */
            static bool protection_active = false;
            
            /* Check to see if memory protection is active */
            if(protection_active)
            {
                /* Disable MPU protection */
                SupervisorMode_MPU(DISABLE_MEMORY_PROTECTION);
                
                /* Set protection active flag false */
                protection_active = false;
                
                /* Turn LED1 off */
                LED1 = LED_OFF;
                
                /* Display MPU disabled on debug LCD */
                Display_LCD(LCD_LINE1, "PROTECT ");
                Display_LCD(LCD_LINE2, "DISABLED");
                
                /* Add delay to allow user to read previous message */
                Delay_s(2u);
            }
            /* Memory protection is enabled */
            else
            {
                /* Enable MPU protection */
                SupervisorMode_MPU(ENABLE_MEMORY_PROTECTION);
                
                /* Set protection active flag true */
                protection_active = true;
                
                /* Turn LED1 on */
                LED1 = LED_ON;
                
                /* Display MPU enabled on debug LCD */
                Display_LCD(LCD_LINE1, "PROTECT ");
                Display_LCD(LCD_LINE2, "ENABLED ");
                
                /* Add delay to allow user to read previous message */
                Delay_s(2u);
            }
            
            /* Exit from switch case */
            break;
        }
        
        /* Default switch case */
        default:
        
            /* Perform no action */
        
        /* Exit from switch case */
        break;
    }
    
    /* Clear switch flag */
    gSwitchFlag = 0x00u;
    
    /* Set screen back to original display */
    Display_LCD(LCD_LINE1, " MPU    ");
    Display_LCD(LCD_LINE2, "Push SW "); 
}
/******************************************************************************
* End of function SwitchPoll_MPU
******************************************************************************/

/******************************************************************************
* Outline       : Excep_AccessViolation
* Description   : Access violation exception handler. This function is called
*                 when an access violation occurs. An access violation is 
*                 triggered when the MCU attempts to read, write or execute 
*                 code from a protected address region.
* Argument      : none
* Return value  : none
******************************************************************************/
//This fixed vector handler name is defined in nmivec.s, local to this project.
__interrupt void __violation_handler(void)
{       
    /* Turn on LED2 */
    LED2 = LED_ON;
    
    /* Indicate access exception on the debug LCD */
    Display_LCD(LCD_LINE1, "ACCESS  ");
    Display_LCD(LCD_LINE2, "EXCEP.  ");
    
    /* Add delay to allow user to read previous message */
    Delay_s(1u);    
    
    /* Declare temporary lcd buffer variable */
    uint8_t buffer[9u];
   
    /* Fetch address of data access violation and convert into string */
    u32ToString(buffer, 0, MPU.MPDEA.BIT.DEA);

    /* Display write fail and the address of failed write on debud LCD */
    Display_LCD(LCD_LINE1, "WR FAIL ");
    Display_LCD(LCD_LINE2, buffer);
    
    /* Wait in infinite while loop */
    while(1);
}
/******************************************************************************
* End of function Excep_AccessViolation
******************************************************************************/

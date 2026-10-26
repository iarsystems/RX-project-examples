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
/*******************************************************************************
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : iic_eeprom_slave_app.c 
* Version      : 1.00
* Device       : R5F562GAADFP
* Tool Chain   : RX Family C Compiler 1.2.1.0
* H/W Platform : RSKRX62G
* Description  : Defines a program sequence that reads and writes to an EEPROM
*                IIC, using functions from iic_eeprom_slave.c - refer to this
*                file for more detailed information.
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines string functions used in this file */
#include <string.h>
/* Defines LCD functions used in this file */
#include "lcd.h"
/* IO port RPDL function definitions */
#include "r_pdl_io_port.h"
/* IIC RPDL function definitions */
#include "r_pdl_iic.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Provides declarations of functions defined in this file */
#include "iic_eeprom_slave.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* IIC slave status flag variable */
extern uint8_t gIIC_Slave_Status;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Bus reply function prototype declaration */
void BusReply_EEPROM(void);
/* Bus activity indicator function prototype declaration */
void BusActivity_IIC(bool);

/*******************************************************************************
* Outline      : SlaveSequence_EEPROM
* Description  : This function first intialises the IIC unit for slave mode, and 
*                then waits in a polling loop for IIC activity. Once a complete
*                IIC data transaction is complete, the Bus Reply function
*                identifies whether a valid read or write request was received,
*                and sets the IIC unit to respond suitably. 
* Argument     : none
* Return value : none
*******************************************************************************/
void SlaveSequence_EEPROM(void)
{
    /* Initialise the IIC unit for slave mode */
    Init_EEPROM_Slave();
        
    /* Enter infinite while loop */
    while(1)
    {   
        /* Wait in polling loop for IIC data transaction */
        BusMonitor_EEPROM_Slave();      
        
        /* Identify the IIC master request, and configure a respond */
        BusReply_EEPROM();
    }
    
}
/*******************************************************************************
* End of function SlaveSequence_EEPROM
*******************************************************************************/

/*******************************************************************************
* Outline      : BusRepy_EEPROM
* Description  : This function checks gIIC_Slave_Status flag to see if a valid
*                operation request was received from a master. If a read or
*                write is requested, the function calls the appropriate function
*                and displays the operation on the debug LCD. LED1 is held on
*                for the duration of the IIC response. If the slave status flag
*                reports a transaction failure, this is display on the debug
*                LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
void BusReply_EEPROM(void)
{
    /* Check the slave status flag variable */
    switch(gIIC_Slave_Status)
    {
        /* IIC bus is idle */
        case IIC_SLAVE_IDLE:
                
        /* Do nothing */
        
        break;
        
        /* EEPROM read operation */
        case IIC_SLAVE_EEPROM_READ:

            /* Set LED1 on to indicate IIC bus activity */
            BusActivity_IIC(true);
            
            /* Configure IIC unit to reply with read data */
            Read_EEPROM_Slave();
            
            /* Display memory read on debug LCD */
            DisplayLCD(LCD_LINE2, "Read  M.");
            
            /* Set LED1 off to indicate IIC bus activity ended */
            BusActivity_IIC(false);
            
        break;
        
        /* EEPROM write operation */
        case IIC_SLAVE_EEPROM_WRITE:
        
            /* Set LED1 on to indicate IIC bus activity */
            BusActivity_IIC(true);
            
            /* Configure IIC unit to reply with read data */
            Write_EEPROM_Slave();
            
            if(gIIC_Slave_Status == IIC_SLAVE_EEPROM_WRITE_OVERFLOW)
            {
                /* Display memory write failure on debug LCD */
                DisplayLCD(LCD_LINE2, "Error W.");
            }
            else 
            {
                /* Display memory write on debug LCD */
                DisplayLCD(LCD_LINE2, "Write M.");
            }
            
            /* Set LED1 off to indicate IIC bus activity ended */
            BusActivity_IIC(false);
            
        break;
        
        /* IIC communication failure */
        case IIC_SLAVE_FAILURE:
        
            /* Display IIC data transaction failure */
            DisplayLCD(LCD_LINE2, "IIC Fail");
            
        break;
                        
        /* Unknown status */
        default:
        
            /* Do nothing */
            
        break;
    }
        
    /* Clear the slave status flag variable */
    gIIC_Slave_Status = 0x00;
}   
/*******************************************************************************
* End of function BusReply_EEPROM
*******************************************************************************/

/*******************************************************************************
* Outline      : BusActivity_IIC
* Description  : Simple function that toggles LED1 on and off, based on a
*                boolean input value. Intended to be used as an indicator of IIC
*                bus acitivity.
* Argument     : bool control - True turns LED1 on, false turns LED1 off.
* Return value : none
*******************************************************************************/
void BusActivity_IIC(bool control)
{
    /* Declare error flag */
    bool err = true;
    
    /* Invert control value, as LEDs operate active low */
    control = (bool)!control;
    
    /* Turn LED1 on or off, depending on control argument */
    err &=  R_IO_PORT_Write
            (
                PDL_IO_PORT_7_2,
                (uint8_t)control
            );
    
    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function BusActivity_IIC
*******************************************************************************/

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
* File Name     : iic_eeprom_slave_app.c 
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines a program sequence that reads and writes to an EEPROM
*                 IIC, using functions from iic_eeprom_slave.c - refer to this
*                 file for more detailed information.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/
/* Following header file provides standard integer type definitions. */
#include <stdint.h>
/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines RX63N LEDs and switches */
#include "rskrx63ndef.h"
/* Defines RX63N port registers */
#include "iorx63n.h"
/* Defines LCD functions used in thuis file */
#include "lcd.h"
/* Switch handler function definitions */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "iic_eeprom_slave.h"
/* Defines IIC slave sequences used in this file */
#include "iic_eeprom_slave_app.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Reserve an area for the simulated EEPROM data */
#pragma location = SIM_EEPROM_LOCATION
/* Declare the simulated EEPROM data table */
uint8_t gSIM_EEPROM_Memory[256u];
/* Global bytes received number container variable */
uint8_t g_IIC_NumBytesReceived = 0x00u;
/* Current simulated memory read/write address */
uint8_t g_SIM_EEPROM_Pointer = 0x00u;
/* Variable used to store the first two byte sent by the master. The first 
contains the slave address, the second contains the memory to be accessed */
uint8_t g_Master_Control[2] = {0xffu, 0xffu};

/*******************************************************************************
* Outline       : SlaveSequence_EEPROM
* Description   : This function first intialises the IIC unit for slave mode, and 
*                 then waits in a polling loop for IIC activity. The function 
*                 checks for a start bit detection followed by an address match. 
*                 Following an address match the function enters receive mode and 
*                 stores the received data to buffer array 'gSIM_EEPROM_Memory'; 
*                 otherwise the slave enters transmit mode and sends data to the 
*                 master until it detects a NACK.
* Argument      : none
* Return value  : none
*******************************************************************************/
void SlaveSequence_EEPROM(void)
{
    /* Declare a dummy variable */
    volatile uint8_t dummy_read;
    
    /* Initialise the IIC unit for slave mode */
    Init_EEPROM_Slave();
        
    /* Enter infinite while loop */
    while(1)
    {        
        /* Wait for detection of the START condition */
        while((RIIC0.ICSR2.BIT.START == 0));
        
        /* Wait for address match detection */
        while(RIIC0.ICSR1.BIT.AAS0 == 0);        
        
        /* Clear the START detection flag */
        RIIC0.ICSR2.BIT.START = 0;
                
        /* While in receive mode */
        while(RIIC0.ICCR2.BIT.TRS == 0)
        {
            /* Check if the STOP condition has been detected */
            if(RIIC0.ICSR2.BIT.STOP == 1)
            {
                /* Check if the receive buffer contains data */
                if(RIIC0.ICSR2.BIT.RDRF == 1)
                {
                    /* Store the received data */
                    gSIM_EEPROM_Memory[g_SIM_EEPROM_Pointer++] = RIIC0.ICDRR;
                }
                
                /* Clear the number of bytes received counter */
                g_IIC_NumBytesReceived = 0;
                
                /* Clear the STOP detection flag */
                RIIC0.ICSR2.BIT.STOP = 0;
                    
                /* Inform user that a write was successful */
                Display_LCD(LCD_LINE2, "Write M.");
                            
                /* Exit receive mode */
                break;
            }
            else
            {
                /* Check if data has been received */
                if(RIIC0.ICSR2.BIT.RDRF == 1u)
                {    
                    /* The first 2 data bytes contain the slave 
                       address and memory location to be accessed */
                    if(g_IIC_NumBytesReceived < 2u)
                    {            
                        /* Store the received data */
                        g_Master_Control[g_IIC_NumBytesReceived] = RIIC0.ICDRR;
                        
                        /* When the second data is received after a START condition */
                        if(g_IIC_NumBytesReceived == 1)
                        {
                            /* The second control data sent by the master contains 
                               the memory location to be written to or read from */
                            g_SIM_EEPROM_Pointer = g_Master_Control[1];
                        }
                    }
                    else
                    {                        
                        /* Store the received data */
                        gSIM_EEPROM_Memory[g_SIM_EEPROM_Pointer++] = RIIC0.ICDRR;
                    }
                    
                    /* Increment the number of bytes received counter */
                    g_IIC_NumBytesReceived++;
                }
            }                        
        }
        
        /* On detection of the R/W bit set to 1, 
           the RX63N enters slave transmit mode */            
        if(RIIC0.ICCR2.BIT.TRS == 1)
        {        
            /* Set the g_SIM_EEPROM_Pointer to the requested memory location */
            g_SIM_EEPROM_Pointer = g_Master_Control[1];
            
            /* Transmit data as long as the master does not issue a STOP */
            while(RIIC0.ICSR2.BIT.STOP == 0)
            {
                /* Check if a NACK hasn't been detected */
                if(RIIC0.ICSR2.BIT.NACKF == 0)
                {    
                    /* Check if the transmit buffer is empty */
                    if(RIIC0.ICSR2.BIT.TDRE == 1)
                    {
                        /* Transmit data until a STOP bit is detected.
                           Increment the memory location address pointer */ 
                        RIIC0.ICDRT = gSIM_EEPROM_Memory[g_SIM_EEPROM_Pointer++];
                    }
                            
                    /* Wait whilst data is being transmitted */
                    while(RIIC0.ICSR2.BIT.TEND == 0)
                    {
                        /* A NACK from the master indicates 
                           end of slave transimission */
                        if(RIIC0.ICMR3.BIT.ACKBR == 1)
                        {                            
                            /* Exit the the data transmission wait */
                            break;
                        }
                    }
                }
                /* NACK has been detected */
                else
                {
                    /* A dummy read is required to release the 
                       SCL line after detection of a NACK */
                    dummy_read = RIIC0.ICDRR;
                    
                    /* Inform user that a read was successful */
                    Display_LCD(LCD_LINE2, "Read  M.");                    
                }
            }
            
            /* Clear the NACKF flag */
            RIIC0.ICSR2.BIT.NACKF = 0;
            
            /* Clear the STOP detection flag */
            RIIC0.ICSR2.BIT.STOP = 0;
            
            /* Clear the number of recieved bytes to zero */
            g_IIC_NumBytesReceived = 0;
        }
    }    
}
/*******************************************************************************
* End of function SlaveSequence_EEPROM
*******************************************************************************/

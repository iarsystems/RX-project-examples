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
* File Name    : iic_eeprom_slave.c
* Version      : 1.00
* Device       : R5F562GAADFP
* Tool Chain   : RX Family C Compiler 1.2.1.0
* H/W Platform : RSKRX62G
* Description  : Defines IIC functions used in this sample. The functions
*                defined operate on the following Renesas IIC EEPROM devices:
*                  R1EX24xxx series, 16Kbit EEPROM, 400KHz
*******************************************************************************/
/*******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* IIC RPDL function definitions */
#include "r_pdl_iic.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines string functions used in this file */
#include <string.h>
/* Provides declarations of functions defined in this file */
#include "iic_eeprom_slave.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Reserve an area for the simulated EEPROM data */
#pragma location=SIM_EEPROM_LOCATION
/* Declare the simulated EEPROM data table */
uint8_t gSIM_EEPROM_Memory[256];

/* Global bytes received number container variable */
uint16_t gIIC_NumBytesReceived = 0x00;
/* IIC received data buffer array */
uint8_t gIIC_ReceiveBuffer[SIM_EEPROM_MAX_WRITE + 1u];
/* Current simulated memory read/write address */
uint8_t gSIM_EEPROM_Pointer = 0x00;
/* IIC slave status flag variable */
uint8_t gIIC_Slave_Status = 0x00;

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Transfer status check function prototype declaration */
void CheckStatus_EEPROM_Slave(void);

/*******************************************************************************
* Outline      : Init_EEPROM_Slave
* Description  : This function configures the IIC unit to operate in bus slave
*                mode. See the iic_eeprom_slave.h header file for configuration
*                options.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_EEPROM_Slave(void)
{
    /* Declare error flag */
    bool err = true;

    /* Configure the IIC unit to operate as bus slave */
    err &=  R_IIC_Create
            (
                IIC_SLAVE_CHANNEL,
                PDL_IIC_MODE_IIC|PDL_IIC_INT_PCLK_DIV_32,
                PDL_IIC_SLAVE_0_ENABLE_7,
                0x00A0,
                PDL_NO_DATA,
                PDL_NO_DATA,
                IIC_SLAVE_TRANSFER_DATA_RATE,
                (SIM_EEPROM_SCL_RISE_TIME << 16) | SIM_EEPROM_SCL_FALL_TIME
            );

    /* Clear the IIC receive buffer */
    memset(gIIC_ReceiveBuffer, 0x00, 17);

    /* Clear the contents of the simulated EEPROM memory */
    memset(gSIM_EEPROM_Memory, 0x00, 256u);

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function Init_EEPROM_slave
*******************************************************************************/

/*******************************************************************************
* Outline      : BusMonitor_EEPROM_Slave
* Description  : This function polls the IIC bus as a slave, and waits for a
*                transmission from a master device. When a complete transmission
*                from a master has been received, the data is stored in the
*                receive buffer and the CheckStatus_EEPROM_Slave function is
*                called in order to determine if a read or write operation was
*                requested from a master.
* Argument     : none
* Return value : none
* Note         : This function will not exit untill an IIC transaction is
*                completed on the IIC bus.
*******************************************************************************/
void BusMonitor_EEPROM_Slave(void)
{
    /* Declare error flag */
    bool err = true;

    /* Clear the IIC receive buffer */
    memset(gIIC_ReceiveBuffer, 0x00, 17);

    /* Enter the slave monitor poll loop */
    err &=  R_IIC_SlaveMonitor
            (
                IIC_SLAVE_CHANNEL,
                PDL_NO_DATA,
                gIIC_ReceiveBuffer,
                SIM_EEPROM_MAX_WRITE + 1u,
                PDL_NO_FUNC,
                0
            );

    /* An IIC transaction has completed, check the status of transfer */
    CheckStatus_EEPROM_Slave();

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function BusMonitor_EEPROM_Slave
*******************************************************************************/

/*******************************************************************************
* Outline      : Read_EEPROM_Slave
* Description  : This function is called as a response to a EEPROM read request
*                a master device over the IIC bus. The function prepares data to
*                be sent to the master device, reading from the current EEPROM
*                pointer location. The read operation will report a nack if the
*                master device attempts to read past the maximum EEPROM address.
* Argument     : none
* Return value : none
* Note         : This function should be called as soon after a read request is
*                detected as possible, to prevent the master device from timing
*                out waiting for a slave response.
*******************************************************************************/
void Read_EEPROM_Slave(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare temporary number of bytes container variable */
    uint16_t num_bytes = 0x0000;

    /* Set the number of bytes readable to the maximum number before the last
       simulated EEPROM memory location is reached */
    num_bytes = (uint16_t)(SIM_EEPROM_MAX_ADDRESS - gSIM_EEPROM_Pointer);

    /* Configure the IIC unit to feed the data from the EEPROM pointer location
       to the IIC master device. */
    err &=  R_IIC_SlaveSend
            (
                IIC_SLAVE_CHANNEL,
                &gSIM_EEPROM_Memory[gSIM_EEPROM_Pointer],
                num_bytes
            );

    /* Reset global status flag */
    gIIC_Slave_Status = IIC_SLAVE_IDLE;

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function Read_EEPROM_Slave
*******************************************************************************/

/*******************************************************************************
* Outline      : Write_EEPROM_Slave
* Description  : This function is called as a response to a EEPROM write request
*                from a master device over the IIC bus. The function sets the
*                simulated EEPROM memory pointer to the value of the first
*                received byte, then fills the simulated memory with the rtest
*                received data. If the impending simulated EEPROM write would
*                overflow the maximum memory capacity, none of the write data is
*                commited memory.
* Argument     : none
* Return value : none
* Note         : This function should be called as soon after a read request is
*                detected as possible, to prevent the master device from timing
*                out waiting for a slave response.
*******************************************************************************/
void Write_EEPROM_Slave(void)
{
    /* Set simulated EEPROM memory pointer */
    gSIM_EEPROM_Pointer = gIIC_ReceiveBuffer[0];

    /* Decrement number of bytes received, to remove address byte */
    gIIC_NumBytesReceived--;

    /* Check if impending write will exceed maximum simulated memory storage */
    if((gIIC_NumBytesReceived + gSIM_EEPROM_Pointer) > SIM_EEPROM_MAX_ADDRESS)
    {
        /* Write would overflow memory capacity, set status flag */
        gIIC_Slave_Status = IIC_SLAVE_EEPROM_WRITE_OVERFLOW;
    }
    else
    {
        /* Declare temporary byte counter variable */
        uint8_t byte = 1u;

        /* Write request valid */
        while(gIIC_NumBytesReceived)
        {
            /* Copy contents of the receive buffer to simulated memory */
            gSIM_EEPROM_Memory[gSIM_EEPROM_Pointer] = gIIC_ReceiveBuffer[byte];

            /* Increment the byte counter */
            byte++;

            /* Increment the simulated EEPROM pointer to the next location */
            gSIM_EEPROM_Pointer++;

            /* Decrement the number of remaining bytes */
            gIIC_NumBytesReceived--;
        }

        /* Reset global status flag */
        gIIC_Slave_Status = IIC_SLAVE_IDLE;
    }

}
/*******************************************************************************
* End of function Write_EEPROM_Slave
*******************************************************************************/

/*******************************************************************************
* Outline      : CheckStatus_EEPROM_Slave
* Description  : This function checks the status of the last received IIC data,
*                determines whether a read or write was requested and sets the
*                global status flag.
* Argument     : none
* Return value : none
* Note         : This function should be called as soon after a read request is
*                detected as possible, to prevent the master device from timing
*                out waiting for a slave response.
*******************************************************************************/
void CheckStatus_EEPROM_Slave(void)
{
    /* Declare error flag */
    bool err = true;

    /* Declare status flags container variable */
    uint32_t status_flags = 0u;

    /* Fetch the last IIC transmission status */
    err &=  R_IIC_GetStatus
            (
                IIC_SLAVE_CHANNEL,
                &status_flags,
                PDL_NO_PTR,
                &gIIC_NumBytesReceived
            );

    /* Check if correct slave address received */
    if((status_flags & 0x0007) == 0x0001)
    {
        /* Check if number of bytes received was zero */
        if(!gIIC_NumBytesReceived)
        {
            /* No data received, set global status flag as failure */
            gIIC_Slave_Status = IIC_SLAVE_FAILURE;
        }

        /* Check the transmission active and R/W bits */
        if(status_flags & 0x00C0)
        {
            /* EEPROM read requested, set global status flag */
            gIIC_Slave_Status = IIC_SLAVE_EEPROM_READ;
        }
        else
        {
            /* EEPROM write requested, set global status flag */
            gIIC_Slave_Status = IIC_SLAVE_EEPROM_WRITE;
        }
    }

    else
    {
        /* Wrong address, set global status flag as failure */
        gIIC_Slave_Status = IIC_SLAVE_FAILURE;
    }

    /* Halt in while loop when RPDL errors detected */
    while(!err);
}
/*******************************************************************************
* End of function CheckStatus_EEPROM_Slave
*******************************************************************************/

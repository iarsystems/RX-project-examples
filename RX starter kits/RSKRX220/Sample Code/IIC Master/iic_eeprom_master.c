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
* File Name     : iic_eeprom_master.c
* Version       : 1.00
* Device        : R5F52206
* Tool-Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX220
* Description   : Defines IIC functions used in this sample. The functions
*                 defined are to operate the IIC EEPROM with 8 bit memory
*                 address (A7-A0).
*******************************************************************************/
/*******************************************************************************
* History       : 14.12.2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/* Defines standard boolean variable types used in this file */
#include <stdbool.h>

/*******************************************************************************
User Includes (Project Level Includes)
*******************************************************************************/

/* Defines RX220 LEDs and switches */
#include "rskrx220def.h"

/* Defines RX220 port registers */
#include <iorx220.h>

/* Provides declarations of functions defined in this file */
#include "iic_eeprom_master.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/* Declare a count variable */
uint8_t receive_data_count = 0;

/* Declare a global variable to store the total number of bytes to be read */
uint8_t g_num_bytes = 0;

/* *Declare a receive end flag and initialise to false */
volatile bool gEnd_RX = false;

/* Define read buffer array */
extern uint8_t read_buffer[9];


/*******************************************************************************
* Function Name: init_eeprom_master
* Description  : This function configures the IIC unit to operate in bus master
*                mode. See the 'iic_eeprom_master.h' header file for
*                configuration options.
* Arguments    : none
* Return Value : none
*******************************************************************************/
void init_eeprom_master (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = PROTECTOFF;

    /* Cancel the RIIC0 module clock stop mode */
    MSTP_RIIC0 = 0x0;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0x0;

    /* Set SCL0 and SDA0 port pin directions to inputs */
    PORT1.PDR.BYTE &= 0xF3u;

    /* Set SCL0 and SDA0 port pin for peripheral use */
    PORT1.PMR.BYTE |= 0x0Cu;

    /* Protection on */
    SYSTEM.PRCR.WORD = PROTECTON;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00u;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTOFF;

    /* Select the pin function as SCL0 */
    MPC.P12PFS.BIT.PSEL = 0xFu;

    /* Set pin 3 on Port1 as a normal pin */
    MPC.P13PFS.BIT.ISEL = 0x0;

    /* Select the pin function as SDA0 */
    MPC.P13PFS.BIT.PSEL = 0xFu;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = PFSPROTECTON;

    /* Disable the RIIC0  */
    RIIC0.ICCR1.BIT.ICE = 0x0;

    /* Issue an internal RIIC0 reset */
    RIIC0.ICCR1.BIT.IICRST = 0x1u;

    /* Cancel the internal reset */
    RIIC0.ICCR1.BIT.IICRST = 0x0;

    /* Set the transfer clock to PCLK/4=5MHz */
    RIIC0.ICMR1.BIT.CKS = 0x0;

    /* Using 50% duty */
    /* Set the transfer bit rate to 100kbps. */
    RIIC0.ICBRL.BIT.BRL = IICBUSRATE;

    /* Set the transfer rate to 100kbps */
    RIIC0.ICBRH.BIT.BRH = IICBUSRATE;

    /* Configure the bus mode register 2:
       Timeout detection uses a 16-bit timer
       Count is disable for both high and low level on the SCL0 line
       No SDA output delays used
       Output delay clock source set to internal IIC reference clock */
    RIIC0.ICMR2.BYTE = 0x00;

    /* Configure the bus mode register 2:
       Single stage noise filter used.
       Select IIC bus function */
    RIIC0.ICMR2.BYTE = 0x00;

    /* Hold the clock line low after each received byte in receive mode */
    RIIC0.ICMR3.BIT.WAIT = 0x1u;

    /* Configure the IIC bus function:
       Disable the timeout function
       Disable master arbitration-lost detection
       Disable slave arbitration-lost detection
       No suspension of transfer operation during NACK reception
       No digital noise filtering used
       Disable SCL synchronous circuit
       Disable the fast-mode */
    RIIC0.ICFER.BYTE = 0x00;

    /* Enable receive end interrupts */
    RIIC0.ICIER.BYTE = 0x20u;

    /* Set RIIC0 RXI0 interrupt priority level to 7 */
    IPR(RIIC0,RXI0) = 0x7u;

    /* Enable RIIC0 RXI0 interrupts */
    IEN(RIIC0,RXI0) = 0x1u;

    /* Clear RIIC0 RXI0 interrupt flag */
    IR(RIIC0,RXI0) = 0x0;

    /* Enable writing to the MST and TRS bits in ICCR2 */
    RIIC0.ICMR1.BIT.MTWP = 0x1u;

    /* Enable RIIC bus interface */
    RIIC0.ICCR1.BIT.ICE = 0x1u;
}
/*******************************************************************************
* End of function init_eeprom_master
*******************************************************************************/

/*******************************************************************************
* Function Name: write_eeprom_master
* Description  : This function writes to the EEPROM device. This function will
*                either perform a page or a byte write, depending on the amount
*                of data to write.
* Arguments    : (uint16_t)address       - The start address of the EEPROM write
*                (uint8_t *)write_data   - A pointer to the array of data to
*                                         the EEPROM.
*                (uint8_t)bytes          - The number of bytes to write.
* Return Value : none
* Note         : 1. The Init_EEPROM function must be called prior to calling
*                   this function.
*                3. If any of the write data would exceed the device's address
*                   range, the operation will not occur.
*                4. If the input parameters are not valid, the EEPROM write will
*                   not happen.
*******************************************************************************/
bool write_eeprom_master (const uint8_t address, uint8_t * write_data,
                                                            const uint8_t bytes)
{
    /* Declare a status variable */
    bool    status = false;

    /* Declare variable to indicate
    status of slave address transmission */
    bool    SlaveDevAddrSent = false;

    /* Declare a count variable */
    uint8_t i = 0;

    /* Check if the IIC bus is busy */
    while (0x1 == RIIC0.ICCR2.BIT.BBSY)
    {
        /* Wait while bus is busy */
    }

    /* Issue a start condition */
    RIIC0.ICCR2.BIT.ST = 0x1u;

    /* Verifies issue of a start condition */
    while ((0x0 == RIIC0.ICCR2.BIT.BBSY) && (0x0 == RIIC0.ICSR2.BIT.START)
           && (0x1 == RIIC0.ICCR2.BIT.ST))
    {
        /* Wait until start condition is issued */
    }

    /* If no acknowledgement received or all data have been sent,
       stop transmission */
    while ((0x0 == RIIC0.ICSR2.BIT.NACKF) && (0 != *write_data) &&
          (i < (bytes + 1)) && (false == status))
    {
        /* Sent the slave address and set the slave device memory to 0 */
        if (false == SlaveDevAddrSent)
        {
            /* check if transmit buffer is empty */
            if (0x1 == RIIC0.ICSR2.BIT.TDRE)
            {
                /* Specify device write */
                RIIC0.ICDRT = 0xA0u;
            }

            /* Wait whilst data is being transmitted */
            while (0x0 == RIIC0.ICSR2.BIT.TEND)
            {
                /* Do nothing until data transmission is finished */
            }

            /* wait for slave acknowledgement */
            while (0x1 == RIIC0.ICMR3.BIT.ACKBR)
            {
                /* Do nothing until slave acknowledges */
            }

            /* Check if transmit buffer is empty */
            if (0x1 == RIIC0.ICSR2.BIT.TDRE)
            {
                /* Set the slave's internal memory to 0 */
                RIIC0.ICDRT = address;
            }

            /* Wait whilst data is being transmitted  */
            while (0x0 == RIIC0.ICSR2.BIT.TEND)
            {
                /* Do nothing until data transmission is finished */
            }

            /* wait for slave acknowledgement */
            while (0x1 == RIIC0.ICMR3.BIT.ACKBR)
            {
                /* Do nothing until slave acknowledges */
            }

            /* Set flag to ensure device address is only sent once */
            SlaveDevAddrSent = true;
        }
        else
        {
            /* check if transmit buffer is empty */
            if (0x1 == RIIC0.ICSR2.BIT.TDRE)
            {
                /* Specify device write */
                RIIC0.ICDRT = *write_data++;
            }

            /* Ensure transmission has ended */
            while (0x0 == RIIC0.ICSR2.BIT.TEND)
            {
                /* Do nothing until transmission has ended */
            }

            /* wait for slave acknowledgement */
            while (0x1 == RIIC0.ICMR3.BIT.ACKBR)
            {
                /* Do nothing until slave acknowledges */
            }

            /* Increment counter */
            i++;

            /* Check if the desired number of bytes sent has been reached */
            if (i == (bytes - 1))
            {
                /* Set the status to true if all data have been sent */
                status = true;
            }
        }
    }

    /* Check if the slave did not acknowledge any byte sent to it */
    if (0x1 == RIIC0.ICSR2.BIT.NACKF)
    {
        /* Set the status to true if all data have been sent */
        status = false;
    }

    /* Clear the stop condition flag */
    RIIC0.ICSR2.BIT.STOP = 0x0;

    /* Issue a stop condition */
    RIIC0.ICCR2.BIT.SP = 0x1u;

    /* Ensure the stop condition has been issued */
    while (0x0 == RIIC0.ICSR2.BIT.STOP)
    {
        /* Do nothing until stop condition has been issued */
    }

    /* Clear the NACKF flag */
    RIIC0.ICSR2.BIT.NACKF = 0x0;

    /* Clear the STOP flag */
    RIIC0.ICSR2.BIT.STOP = 0x0;

    /* Return the status */
    return status;
}

/*******************************************************************************
* End of function write_eeprom_master
*******************************************************************************/

/*******************************************************************************
* Function Name: read_eeprom_master
* Description  : This function reads the data at the location of the address
*                passed, and copies the data to the read_buffer array. The
*                function will read the entire address range in one call, as
*                long as the read operation does not request data outside of
*                the EEPROM device's maximum address range. The function first
*                performs a dummy write to set the EEPROM's pointer, then
*                performs a sequential read until all the data has been read.
* Arguments    : (const uint8_t)address       - Address to start the read from.
*                (const uint8_t)num_bytes     - The number of bytes to read.
* Return Value : none
* Note         : 1. If invalid input parameters are used the read operation
*                   will not happen.
*******************************************************************************/
bool read_eeprom_master (const uint8_t address, const uint8_t num_bytes)
{
    /* Declare a status variable */
    bool status = 0;

    /* Declare a dummy variable */
    volatile uint8_t dummy_read;

    /* Reset the data received counter */
    receive_data_count = 0;

    /* Copy the number of bytes to be receive from the slave */
    g_num_bytes = num_bytes;

    /* Set flag to false before start of data reception */
    gEnd_RX = false;

    /* Disable the RIIC0 */
    RIIC0.ICCR1.BIT.ICE = 0x0;

    /* Issue an internal RIIC0 reset */
    RIIC0.ICCR1.BIT.IICRST = 0x1u;

    /* Cancel the internal reset */
    RIIC0.ICCR1.BIT.IICRST = 0x0;

    /* Set the transfer clock to PCLK/1=20MHz */
    RIIC0.ICMR1.BIT.CKS = 0x0;

    /* Using 50% duty */
    /* Set the transfer bit rate to 100kbps */
    RIIC0.ICBRL.BIT.BRL = IICBUSRATE;

    /* Set the transfer rate to 100kbps */
    RIIC0.ICBRH.BIT.BRH = IICBUSRATE;

    /* Hold the clock line low after each received byte in receive mode */
    RIIC0.ICMR3.BIT.WAIT = 0x0;

    /* Enable transmit and receive end interrupts */
    RIIC0.ICIER.BYTE = 0xA0u;

    /* Enable RIIC bus interface */
    RIIC0.ICCR1.BIT.ICE = 0x1u;

    /* Enable writing to ACKBT bit */
    RIIC0.ICMR3.BIT.ACKWP = 0x1u;

    /* Write 0 as acknowledgement bit */
    RIIC0.ICMR3.BIT.ACKBT = 0x0;

    /* Check if the IIC bus is busy */
    while (0x1 == RIIC0.ICCR2.BIT.BBSY)
    {
        /* Do nothing until bus is free */
    }

    /* Issue a start condition */
    RIIC0.ICCR2.BIT.ST = 0x1u;

    /* Confirm issue of start condition */
    while ((0x0 == RIIC0.ICCR2.BIT.BBSY) && (0x0 == RIIC0.ICSR2.BIT.START)
           && (0x1 == RIIC0.ICCR2.BIT.ST))
    {
        /* Do nothing until start condition is issued */
    }

    /* check if transmit buffer is empty */
    if (0x1 == RIIC0.ICSR2.BIT.TDRE)
    {
        /* Specify device write */
        RIIC0.ICDRT = EEPROM_DEVICE_ADDRESS;
    }

    /* wait for slave acknowledgement */
    while (0x1 == RIIC0.ICMR3.BIT.ACKBR)
    {
        /* Do nothing until slave acknowledges */
    }

    /* Ensure the transmit buffer is empty */
    while (0x0 == RIIC0.ICSR2.BIT.TEND)
    {
        /* Do nothing while data remains in the buffer */
    }

    /* Check if transmit buffer is empty */
    if (0x1 == RIIC0.ICSR2.BIT.TDRE)
    {
        /* Set the slave's internal memory to 0 */
        RIIC0.ICDRT = address;
    }

    /* wait for slave acknowledgement */
    while (0x1 == RIIC0.ICMR3.BIT.ACKBR)
    {
        /* Do nothing until slave acknowledges */
    }

    /* Ensure the transmit buffer is empty */
    while (0x0 == RIIC0.ICSR2.BIT.TEND)
    {
        /* Do nothing while data remains in the buffer */
    }

    /* Issue a restart condition */
    RIIC0.ICCR2.BIT.RS = 0x1u;

    /* Declare a count variable */
    volatile uint8_t j = 0xffu;

    /* Add a small delay */
    while (j--)
    {
        /* Do nothing until delay is complete */
    }

    /* Check if the IIC bus is occupied */
    while (0x0 == RIIC0.ICCR2.BIT.BBSY)
    {
    }

    /* Check if transmit buffer is empty */
    if (0x1 == RIIC0.ICSR2.BIT.TDRE)
    {
        /* Set the 'W' bit to indicate a read from device */
        RIIC0.ICDRT = (uint8_t)(EEPROM_DEVICE_ADDRESS | 0x01u);
    }

    /* Wait for slave acknowledgement */
    while (0x1 == RIIC0.ICMR3.BIT.ACKBR)
    {
        /* Do nothing until slave acknowledges */
    }

    /* Wait for the dummy byte's reception */
    while (0x1 == RIIC0.ICSR2.BIT.RDRF)
    {
        /* Do nothing until dummy bytes has been received */
    }

    /* Wait until all expected data has been
       received or until a NACKF has been detected */
    while ((false == gEnd_RX) && (0x0 == RIIC0.ICSR2.BIT.NACKF))
    {
        /* Wait for the data to be received */
    }

    /* Clear the wait bit if all expected data were correctly received */
    if (0x1 != RIIC0.ICSR2.BIT.NACKF)
    {
        /* Clear the wait bit */
        RIIC0.ICMR3.BIT.WAIT = 0x0;
    }

    /* Reception was terminated due to NACK reception */
    else
    {
        /* Dummy read */
        dummy_read = RIIC0.ICDRR;
    }

    /* Ensure the stop condition has been issued */
    while (0x0 == RIIC0.ICSR2.BIT.STOP)
    {
        /* Do nothing until stop condition has been issued */
    }

    /* Clear the NACKF flag */
    RIIC0.ICSR2.BIT.NACKF = 0x0;

    /* Clear the STOP flag */
    RIIC0.ICSR2.BIT.STOP = 0x0;

    /* Disable writing to ACKBT bit */
    RIIC0.ICMR3.BIT.ACKWP = 0x0;

    /* Disable receive end interrupts */
    RIIC0.ICIER.BIT.RIE = 0x0;

    /* Declare a transfer delay variable */
    volatile uint16_t transfer_delay = 0x0;

    /* Add a delay */
    for (transfer_delay = 0xffffu; transfer_delay; transfer_delay--)
    {
        /* Wait */
    }

    /* Read the status returned from the interrupt routine */
    status = gEnd_RX;

    /* Return the status */
    return status;
}

/*******************************************************************************
* End of function read_eeprom_master
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_RIIC0_RXI0_isr
* Description  : RIIC0 interrupt function. The start of data reception starts
*                with a dummy read followed by reception of expected number of
*                receive bytes. The acknowledgement bit is set to NACK after
*                reception of the next to last byte to be received. Upon
*                receiving the final byte a stop signal is issued to end
*                reception.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#pragma vector=VECT_RIIC0_RXI0
__interrupt  void Excep_RIIC0_RXI0_isr(void)
{
    /* Declare a dummy variable */
    volatile uint8_t dummy_read;

    /* The first byte received is the dummy byte */
    if (0x0 == receive_data_count)
    {
        /* Dummy read */
        dummy_read = RIIC0.ICDRR;
    }
    else
    {
        /* Store the data */
        read_buffer[receive_data_count - 1] = RIIC0.ICDRR;

        /* Check if the received byte is the next to last */
        if (receive_data_count == (g_num_bytes - 1))
        {
            /* Write 1 as acknowledgement bit to terminate
               read sequence after reception of last byte */
            RIIC0.ICMR3.BIT.ACKBT = 0x1u;
        }

        /* Check if the received number of bytes has been reached */
        if (receive_data_count == (g_num_bytes))
        {
            /* Prepare for receiving final data */
            RIIC0.ICMR3.BIT.WAIT = 0x1u;

            /* Issue a stop condition */
            RIIC0.ICCR2.BIT.SP = 0x1u;

            /* Set flag to indicate end of data reception */
            gEnd_RX = true;
        }
    }

    /* Increment the counter */
    receive_data_count++;
}
/*******************************************************************************
* End of function Excep_RIIC0_RXI0_isr
*******************************************************************************/


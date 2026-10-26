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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/*******************************************************************************
* File Name     : iic_master.c
* Version       : 1.00
* Device        : R5F563TE
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX63T144
* Description   : Defines IIC functions used in this sample. The functions
*                 defined are to operate the IIC EEPROM with 8 bit memory
*                 address (A7-A0).
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
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
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* LCD controlling function prototypes & macro definitions */
#include "lcd.h"

/* Provides declarations of functions defined application file */
#include "iic_eeprom_master_app.h"

/* Provides declarations of functions defined in this file */
#include "iic_eeprom_master.h"

/*******************************************************************************
* Local Variables
*******************************************************************************/
/* Declare a count variable */
static uint8_t receive_data_count = 0;

/* Declare a global variable to store the total number of bytes to be read */
static uint8_t num_bytes_to_read = 0;

/* Declare a receive end flag and initialise to false */
static volatile bool rx_end_flag = false;

/*******************************************************************************
* Function Name: Init_EEPROM_Master
* Description  : This function configures the IIC unit to operate in bus master
*                mode. See the 'iic_eeprom_master.h' header file for
*                configuration options.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_EEPROM_Master (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the RIIC1 module clock stop mode */
    MSTP_RIIC1 = 0;

    /* Cancel the S12AD module clock stop mode */
    MSTP_S12AD = 0;

    /* Set SCL1 and SDA1 port 2 pin directions to inputs */
    PORT2.PDR.BIT.B5 = 0x0;
    PORT2.PDR.BIT.B6 = 0x0;

    /* Set SCL1 and SDA1 port pin for peripheral use */
    PORT2.PMR.BIT.B5 = 0x1;
    PORT2.PMR.BIT.B6 = 0x1;

    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;

    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;

    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40;

    /* Select the pin function as SCL1 */
    MPC.P25PFS.BIT.PSEL = 0xF;

    /* Select the pin function as SDA1 */
    MPC.P26PFS.BIT.PSEL = 0xF;

    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80;

    /* Disable the RIIC1  */
    RIIC1.ICCR1.BIT.ICE = 0;

    /* Issue an internal RIIC1 reset */
    RIIC1.ICCR1.BIT.IICRST = 0x1;

    /* Cancel the internal reset */
    RIIC1.ICCR1.BIT.IICRST = 0x0;

    /* Set the transfer clock to PCLK/8=6MHz */
    RIIC1.ICMR1.BIT.CKS = 0x3;

    /* Set the transfer bit rate to 100kbps, 50% duty cycle
       PCLK = 48MHz, ratio = 1/8, Values used in table 27.5 in Hardware
       Manual */
    RIIC1.ICBRL.BIT.BRL = 0xF8;
    RIIC1.ICBRH.BIT.BRH = 0xFD;

    /* Configure the bus mode register 2:
       Timeout detection uses a 16-bit timer
       Count is disable for both high and low level on the SCL0 line
       No SDA output delays used
       Output delay clock source set to internal IIC reference clock
       Single stage noise filter used.
       Select IIC bus function */
    RIIC1.ICMR2.BYTE = 0x00;

    /* Hold the clock line low after each received byte in receive mode */
    RIIC1.ICMR3.BIT.WAIT = 0x1;

    /* Configure the IIC bus function:
       Disable the timeout function
       Disbale master arbitration-lost detection
       Disable slave arbitration-lost detection
       No suspension of transfer operation during NACK reception
       No digital noise filtering used
       Disable SCL synchronous circuit
       Disable the fast-mode */
    RIIC1.ICFER.BYTE = 0x00;

    /* Enable receive end interrupts */
    RIIC1.ICIER.BYTE = 0x20;

    /* Set RIIC1 RXI0 interrupt priority level to 7 */
    IPR(RIIC1,RXI1) = 0x7;

    /* Enable RIIC1 RXI0 interrupts */
    IEN(RIIC1,RXI1) = 0x1;

    /* Clear RIIC1 RXI0 interrupt flag */
    IR(RIIC1,RXI1) = 0x0;

    /* Enable writing to the MST and TRS bits in ICCR2 */
    RIIC1.ICMR1.BIT.MTWP = 0x1;

    /* Enable RIIC bus interface */
    RIIC1.ICCR1.BIT.ICE = 0x1;
}
/*******************************************************************************
* End of function Init_EEPROM_Master
*******************************************************************************/

/*******************************************************************************
* Function Name: Write_EEPROM_Master
* Description  : This function writes to the EEPROM device. This function will
*                either perform a page or a byte write, depending on the amount
*                of data to write.
* Argument     : uint16_t address       - The start address of the EEPROM write.
*                uint8_t * write_data   - A pointer to the array of data to
*                                         the EEPROM.
*                uint8_t bytes          - The number of bytes to write.
* Return value : none
* Note         : 1. The Init_EEPROM function must be called prior to calling
*                   this function.
*                3. If any of the write data would exceed the device's address
*                   range, the operation will not occur.
*                4. If the input parameters are not valid, the EEPROM write will
*                   not happen.
*******************************************************************************/
bool Write_EEPROM_Master (const uint8_t address, char * write_data,
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
    while (0x1 == RIIC1.ICCR2.BIT.BBSY)
    {
        /* Wait */
    }

    /* Issue a start condition */
    RIIC1.ICCR2.BIT.ST = 0x1;

    /* Verifies issue of a start condition */
    while ((0x0 == RIIC1.ICCR2.BIT.BBSY) &&
           (0x0 == RIIC1.ICSR2.BIT.START) &&
           (0x1 == RIIC1.ICCR2.BIT.ST))
    {
        /* Wait */
    }

    /* If no acknowledgement received or all data have been sent,
       stop transmission */
    while ((0x0 == RIIC1.ICSR2.BIT.NACKF) &&
           (0x0 != *write_data) &&
           (i < (bytes + 1)) &&
           (false == status))
    {
        /* Send the slave address and set the slave device memory to 0 */
        if (false == SlaveDevAddrSent)
        {
            /* Check if transmit buffer is empty */
            if (0x1 == RIIC1.ICSR2.BIT.TDRE)
            {
                /* Specify device write */
                RIIC1.ICDRT = 0xA0;
            }

            /* Wait whilst data is being transmitted */
            while (0x0 == RIIC1.ICSR2.BIT.TEND)
            {
                /* Wait */
            }

            /* Wait for slave acknowledgement */
            while (0x1 == RIIC1.ICMR3.BIT.ACKBR)
            {
                /* Wait */
            }

            /* Check if transmit buffer is empty */
            if (0x1 == RIIC1.ICSR2.BIT.TDRE)
            {
                /* Set the slave's internal memory to 0 */
                RIIC1.ICDRT = address;
            }
            /* Wait whilst data is being transmitted  */
            while (0x0 == RIIC1.ICSR2.BIT.TEND)
            {
                /* Wait */
            }

            /* Wait for slave acknowledgement */
            while (0x1 == RIIC1.ICMR3.BIT.ACKBR)
            {
                /* Wait */
            }

            /* Set flag to ensure device address is only sent once */
            SlaveDevAddrSent = true;
        }
        else
        {
            /* check if transmit buffer is empty */
            if (0x1 == RIIC1.ICSR2.BIT.TDRE)
            {
                /* Specify device write */
                /* Cast required because register is defined as uint8_t */
                RIIC1.ICDRT = (uint8_t)*write_data++;
            }

            /* Ensure transmission has ended */
            while (0x0 == RIIC1.ICSR2.BIT.TEND)
            {
                /* Wait */
            }

            /* Wait for slave acknowledgement */
            while (0x1 == RIIC1.ICMR3.BIT.ACKBR)
            {
                /* Wait */
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
    if (0x1 == RIIC1.ICSR2.BIT.NACKF)
    {
        /* Set the status to true if all data have been sent */
        status = false;
    }

    /* Clear the stop condition flag */
    RIIC1.ICSR2.BIT.STOP = 0x0;

    /* Issue a stop condition */
    RIIC1.ICCR2.BIT.SP = 0x1;

    /* Ensure the stop condition has been issued */
    while (0x0 == RIIC1.ICSR2.BIT.STOP)
    {
        /* Wait */
    }

    /* Clear the NACKF flag */
    RIIC1.ICSR2.BIT.NACKF = 0x0;

    /* Clear the STOP flag */
    RIIC1.ICSR2.BIT.STOP = 0x0;

    /* Return the status */
    return status;
}
/*******************************************************************************
* End of function Write_EEPROM_Master
*******************************************************************************/

/*******************************************************************************
* Function Name: Read_EEPROM_Master
* Description  : This function reads the data at the location of the address
*                passed, and copies the data to the output array passed. The
*                function will read the entire address range in one call, as
*                long as the read operation does not request data outside of
*                the EEPROM device's maximum address range. The function first
*                performs a dummy write to set the EEPROM's pointer, then
*                performs a sequential read until all the data has been read.
* Argument     : uint8_t address       - Address to start the read from.
*                uint8_t num_bytes     - The number of bytes to read.
* Return value : none
* Note         : 1. If invalid input parameters are used the read operation
*                   will not happen.
*******************************************************************************/
bool Read_EEPROM_Master (const uint8_t address, const uint8_t num_bytes)
{
    /* Declare a status variable */
    bool                status = 0;

    /* Declare a dummy variable */
    volatile uint8_t    dummy_read = 0;

    /* Declare a count variable */
    volatile uint8_t    j = 0xff;

    /* Reset the data received counter */
    receive_data_count = 0;

    /* Copy the number of bytes to be receive from the slave */
    num_bytes_to_read = num_bytes;

    /* Set flag to false before start of data reception */
    rx_end_flag = false;

    /* Disable the RIIC1 */
    RIIC1.ICCR1.BIT.ICE = 0;

    /* Issue an internal RIIC1 reset */
    RIIC1.ICCR1.BIT.IICRST = 0x1;

    /* Cancel the internal reset */
    RIIC1.ICCR1.BIT.IICRST = 0x0;

    /* Set the transfer clock to PCLK/8=6MHz */
    RIIC1.ICMR1.BIT.CKS = 0x3;

    /* Set the transfer bit rate to 100kbps, 50% duty cycle
       PCLK = 48MHz, ratio = 1/8, Values used in table 27.5 in Hardware
       Manual */
    RIIC1.ICBRL.BIT.BRL = 0xF8;
    RIIC1.ICBRH.BIT.BRH = 0xFD;

    /* Hold the clock line low after each received byte in receive mode */
    RIIC1.ICMR3.BIT.WAIT = 0x0;

    /* Enable transmit and receive end interrupts */
    RIIC1.ICIER.BYTE = 0xA0;

    /* Enable RIIC bus interface */
    RIIC1.ICCR1.BIT.ICE = 0x1;

    /* Enable writing to ACKBT bit */
    RIIC1.ICMR3.BIT.ACKWP = 0x1;

    /* Write 0 as acknowledgement bit */
    RIIC1.ICMR3.BIT.ACKBT = 0x0;

    /* Check if the IIC bus is busy */
    while (0x1 == RIIC1.ICCR2.BIT.BBSY)
    {
        /* Wait */
    }

    /* Issue a start condition */
    RIIC1.ICCR2.BIT.ST = 0x1;

    /* Confirm issue of start condition */
    while ((0x0 == RIIC1.ICCR2.BIT.BBSY) &&
           (0x0 == RIIC1.ICSR2.BIT.START) &&
           (0x1 == RIIC1.ICCR2.BIT.ST))
    {
        /* Wait */
    }

    /* check if transmit buffer is empty */
    if (0x1 == RIIC1.ICSR2.BIT.TDRE)
    {
        /* Specify device write */
        RIIC1.ICDRT = 0xA0;
    }

    /* Wait for slave acknowledgement */
    while (0x1 == RIIC1.ICMR3.BIT.ACKBR)
    {
        /* Wait */
    }

    /* Ensure the transmit buffer is empty */
    while (0x0 == RIIC1.ICSR2.BIT.TEND)
    {
        /* Wait */
    }

    /* Check if transmit buffer is empty */
    if (0x1 == RIIC1.ICSR2.BIT.TDRE)
    {
        /* Set the slave's internal memory to 0 */
        RIIC1.ICDRT = address;
    }

    /* Wait for slave acknowledgement */
    while (0x1 == RIIC1.ICMR3.BIT.ACKBR)
    {
        /* Wait */
    }

    /* Ensure the transmit buffer is empty */
    while (0x0 == RIIC1.ICSR2.BIT.TEND)
    {
        /* Wait */
    }

    /* Issue a restart condition */
    RIIC1.ICCR2.BIT.RS = 0x1;

    /* Add a small delay */
    while (j--)
    {
        /* Wait */
    }

    /* Check if the IIC bus is occupied */
    while (0x0 == RIIC1.ICCR2.BIT.BBSY)
    {
        /* Wait */
    }

    /* Check if transmit buffer is empty */
    if (0x1 == RIIC1.ICSR2.BIT.TDRE)
    {
        /* Set the 'W' bit to indicate a read from device */
        RIIC1.ICDRT = (uint8_t)(EEPROM_DEVICE_ADDRESS | 0x01u);
    }

    /* Wait for slave acknowledgement */
    while (0x01 == RIIC1.ICMR3.BIT.ACKBR)
    {
        /* Wait */
    }

    /* Wait for the dummy byte's reception */
    while (0x01 == RIIC1.ICSR2.BIT.RDRF)
    {
        /* Wait */
    }

    /* Wait until all expected data has been
       received or until a NACKF has been detected */
    while ((false == rx_end_flag) && (0x01 == RIIC1.ICSR2.BIT.NACKF))
    {
        /* Wait for the data to be received */
    }

    /* Clear the Wait bit if all expected data were correctly received */
    if (0x1 != RIIC1.ICSR2.BIT.NACKF)
    {
        /* Clear the Wait bit */
        RIIC1.ICMR3.BIT.WAIT = 0x0;
    }
    else
    {
        /* Reception was terminated due to NACK reception */
        /* Dummy read */
        dummy_read = RIIC1.ICDRR;
    }

    /* Ensure the stop condition has been issued */
    while (0x0 == RIIC1.ICSR2.BIT.STOP)
    {
        /* Wait */
    }

    /* Clear the NACKF flag */
    RIIC1.ICSR2.BIT.NACKF = 0x0;

    /* Clear the STOP flag */
    RIIC1.ICSR2.BIT.STOP = 0x0;

    /* Disable writing to ACKBT bit */
    RIIC1.ICMR3.BIT.ACKWP = 0x0;

    /* Disable receive end interrupts */
    RIIC1.ICIER.BIT.RIE = 0x0;

    /* Add a delay */
    for (volatile uint16_t delay = 0xffff; delay > 0; delay--)
    {
        /* Wait */
    }

    /* Read the status returned from the interrupt routine */
    status = rx_end_flag;

    /* Return the status */
    return status;
}
/*******************************************************************************
* End of function Read_EEPROM_Master
*******************************************************************************/

/*******************************************************************************
* Function Name: Excep_RIIC1_RXI0
* Description  : RIIC1 interrupt function. The start of data reception starts
*                with a dummy read followed by reception of expected number of
*                receive bytes. The acknowledgement bit is set to NACK after
*                reception of the next to last byte to be received. Upon
*                receiving the final byte a stop signal is issued to end
*                reception.
* Argument     : none
* Return value : none
*******************************************************************************/
#pragma vector=VECT_RIIC1_RXI1
__interrupt  void Excep_RIIC1_RXI1(void)
{
    /* Declare a dummy variable */
    volatile uint8_t dummy_read = 0;

    /* The first byte received is the dummy byte */
    if (0 == receive_data_count)
    {
        /* Dummy read */
        dummy_read = RIIC1.ICDRR;
    }
    else
    {
        /* Store the data */
        /* Cast required because register is defined as uint8_t */
        g_read_buffer[receive_data_count - 1] = (char)RIIC1.ICDRR;

        /* Check if the received byte is the next to last */
        if (receive_data_count == (num_bytes_to_read - 1))
        {
            /* Write 1 as acknowledgement bit to terminate
               read sequence after reception of last byte */
            RIIC1.ICMR3.BIT.ACKBT = 0x1;
        }

        /* Check if the received number of bytes has been reached */
        if (receive_data_count == num_bytes_to_read)
        {
            /* Prepare for receiving final data */
            RIIC1.ICMR3.BIT.WAIT = 0x1;

            /* Issue a stop condition */
            RIIC1.ICCR2.BIT.SP = 0x1;

            /* Set flag to indicate end of data reception */
            rx_end_flag = true;
        }
    }

    /* Increment the counter */
    receive_data_count++;
}
/*******************************************************************************
* End of function Excep_RIIC1_RXI1
*******************************************************************************/


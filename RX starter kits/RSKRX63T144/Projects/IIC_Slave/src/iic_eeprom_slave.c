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
* File Name    : iic_eeprom_slave.c
* Version      : 1.00
* Device       : R5F563TE
* Tool Chain   : RX Family C Compiler 1.2.1.0
* H/W Platform : RSKRX63T
* Description  : Defines IIC functions used in this sample. The functions
*                defined operate on the following Renesas IIC EEPROM devices:
*                  R1EX24xxx series, 16Kbit EEPROM
*******************************************************************************/
/*******************************************************************************
* History       : 05.12.2013  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
System Includes
*******************************************************************************/
/* Defines standard boolean variable types used in this file */
#include <stdbool.h>

/* Following header file provides standard integer type definitions. */
#include <stdint.h>

/* Following header file provides string type definitions. */
#include <string.h>

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines R5F563TE port registers */
#include <iorx63t.h>

/* Defines LCD functions used in this file */
#include "lcd.h"

/* Provides declarations of functions defined in this file */
#include "iic_eeprom_slave.h"

/*******************************************************************************
* Local Variables
*******************************************************************************/
/* Reserve an area for the simulated EEPROM data */
#pragma location = SIM_EEPROM_LOCATION

/* Declare the simulated EEPROM data table */
__root uint8_t g_sim_eeprom_memory[256];

/*******************************************************************************
* Function Name: Init_EEPROM_Slave
* Description  : This function configures the IIC unit to operate in bus slave
*                mode. See the iic_eeprom_slave.h header file for configuration
*                options.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_EEPROM_Slave (void)
{
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;

    /* Cancel the RIIC0 module clock stop mode */
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

    /* Low-level clock period for Data setup time > 250ns (Slave mode)
       See Hardware Manual Section 27.2.12. 2 * 6MHz clock period is 333 ns*/
    RIIC1.ICBRL.BIT.BRL = 0x72;

    /* Set the address mode to 7-bits */
    RIIC1.SARU0.BIT.FS = 0;

    /* Set the slave address */
    RIIC1.SARL0.BYTE = SIM_EEPROM_DEVICE_ADDRESS;

    /* Configure the bus mode register 2:
       Timeout detection uses a 16-bit timer
       Count is disabled for both high and low level on the SCL0 line
       No SDA output delays used
       Output delay clock source set to internal IIC reference clock
       Single stage noise filter used.
       Select IIC bus function */
    RIIC1.ICMR2.BYTE = 0x00;

    /* Configure the IIC bus function:
       Disable the timeout function
       Disbale master arbitration-lost detection
       Disable slave arbitration-lost detection
       Enable suspension of transfer operation during NACK reception
       No digital noise filtering used
       Disable SCL synchronous circuit
       Disable the fast-mode */
    RIIC1.ICFER.BYTE = 0x10;

    /* Enable RIIC bus interface */
    RIIC1.ICCR1.BIT.ICE = 0x1;

    /* Clear the contents of the simulated EEPROM memory */
    memset(g_sim_eeprom_memory, 0x00, 256u);
}
/*******************************************************************************
* End of function Init_EEPROM_Slave
*******************************************************************************/

/*******************************************************************************
* Function Name: SlaveSequence_EEPROM
* Description  : This function first intialises the IIC unit for slave mode, and
*                then waits in a polling loop for IIC activity. The function
*                checks for a start bit detection followed by an address match.
*                Following an address match the function enters receive mode and
*                stores the received data to buffer array 'g_sim_eeprom_memory';
*                otherwise the slave enters transmit mode and sends data to the
*                master until it detects a NACK.
* Argument     : none
* Return value : none
*******************************************************************************/
void SlaveSequence_EEPROM (void)
{
    /* Bytes received number container variable */
    uint8_t             iic_num_bytes_received = 0x00;

    /* Current simulated memory read/write address */
    uint8_t             sim_eeprom_pointer = 0x00;

    /* Variable used to store the first two byte sent by the master. The first
    contains the slave address, the second contains the memory to be accessed */
    uint8_t             master_control[2] = {0xff, 0xff};

    /* Declare a dummy variable */
    volatile uint8_t    dummy_read;

    /* Variable used to flag the NACK received condition */
    bool                nack_received = false;

    /* Wait for detection of the START condition */
    while (0x0 == RIIC1.ICSR2.BIT.START)
    {
        /* Wait */
    }

    /* Wait for address match detection */
    while (0x0 == RIIC1.ICSR1.BIT.AAS0)
    {
        /* Wait */
    }

    /* Clear the START detection flag */
    RIIC1.ICSR2.BIT.START = 0;

    /* While in receive mode */
    while (0x0 == RIIC1.ICCR2.BIT.TRS)
    {
        /* Check if the STOP condition has been detected */
        if (0x1 == RIIC1.ICSR2.BIT.STOP)
        {
            /* Check if the receive buffer contains data */
            if (0x1 == RIIC1.ICSR2.BIT.RDRF)
            {
                /* Store the received data */
                g_sim_eeprom_memory[sim_eeprom_pointer++] = RIIC1.ICDRR;
            }

            /* Clear the number of bytes received counter */
            iic_num_bytes_received = 0;

            /* Clear the STOP detection flag */
            RIIC1.ICSR2.BIT.STOP = 0;

            /* Inform user that a write was successful */
            Display_LCD(LCD_LINE2, "Write M.");

            /* Exit receive mode */
        }
        else
        {
            /* Check if data has been received */
            if (0x1 == RIIC1.ICSR2.BIT.RDRF)
            {
                /* The first 2 data bytes contain the slave
                   address and memory location to be accessed */
                if (iic_num_bytes_received < 2)
                {
                    /* Store the received data */
                    master_control[iic_num_bytes_received] = RIIC1.ICDRR;

                    /* When the 2nd data is received after a START condition */
                    if (1 == iic_num_bytes_received)
                    {
                        /* The second control data sent by the master contains
                           the memory location to be written to or read from */
                        sim_eeprom_pointer = master_control[1];
                    }
                }
                else
                {
                    /* Store the received data */
                    g_sim_eeprom_memory[sim_eeprom_pointer++] = RIIC1.ICDRR;
                }

                /* Increment the number of bytes received counter */
                iic_num_bytes_received++;
            }
        }
    }

    /* On detection of the R/W bit set to 1,
       the R5F563TE enters slave transmit mode */
    if (0x1 == RIIC1.ICCR2.BIT.TRS)
    {
        /* Set the sim_eeprom_pointer to the requested memory location */
        sim_eeprom_pointer = master_control[1];

        /* Transmit data as long as the master does not issue a STOP */
        while (0x0 == RIIC1.ICSR2.BIT.STOP)
        {
            /* Check if a NACK hasn't been detected */
            if (0x0 == RIIC1.ICSR2.BIT.NACKF)
            {
                /* Check if the transmit buffer is empty */
                if (0x1 == RIIC1.ICSR2.BIT.TDRE)
                {
                    /* Transmit data until a STOP bit is detected.
                       Increment the memory location address pointer */
                    RIIC1.ICDRT = g_sim_eeprom_memory[sim_eeprom_pointer++];
                }

                /* Wait whilst data is being transmitted */
                while ((0x0 == RIIC1.ICSR2.BIT.TEND) &&
                       (false == nack_received))
                {
                    /* A NACK from the master indicates
                       end of slave transimission */
                    if (0x1 == RIIC1.ICMR3.BIT.ACKBR)
                    {
                        /* Exit the the data transmission wait */
                        nack_received = true;
                    }
                }
            }
            /* NACK has been detected */
            else
            {
                /* A dummy read is required to release the
                   SCL line after detection of a NACK */
                dummy_read = RIIC1.ICDRR;

                /* Inform user that a read was successful */
                Display_LCD(LCD_LINE2, "Read  M.");
            }
        }

        /* Clear the NACKF flag */
        RIIC1.ICSR2.BIT.NACKF = 0;

        /* Clear the STOP detection flag */
        RIIC1.ICSR2.BIT.STOP = 0;

        /* Clear the number of recieved bytes to zero */
        iic_num_bytes_received = 0;
    }
}
/*******************************************************************************
* End of function SlaveSequence_EEPROM
*******************************************************************************/



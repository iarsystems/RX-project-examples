/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_riic_user.c
* Version      : Applilet4 for RX111 V1.00.00.05 [06 Jun 2013]
* Device(s)    : R5F51115AxFM
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for RIIC module.
* Creation Date: 2013-08-26
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_riic.h"
/* Start user code for include. Do not edit comment generated here */
#include <string.h>
#include "lcd.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern uint8_t    g_riic0_mode_flag;               /* RIIC0 master transmit receive flag */
extern uint8_t    g_riic0_state;                   /* RIIC0 master state */
extern uint16_t   g_riic0_slave_address;           /* RIIC0 slave address */
extern uint8_t *  gp_riic0_tx_address;             /* RIIC0 transmit buffer address */
extern uint16_t   g_riic0_tx_count;                /* RIIC0 transmit data number */
extern uint8_t *  gp_riic0_rx_address;             /* RIIC0 receive buffer address */
extern uint16_t   g_riic0_rx_count;                /* RIIC0 receive data number */
extern uint16_t   g_riic0_rx_length;               /* RIIC0 receive data length */
extern uint8_t    g_riic0_dummy_read_count;        /* RIIC0 count for dummy read */
/* Start user code for global. Do not edit comment generated here */

/* The functions in this file are generated using "Application Leading Tool (Applilet)" for e2 studio.
 * Warnings exist for interrupt handlers, variables and callback functions. */

/* Declare the simulated EEPROM data table */
uint8_t g_sim_eeprom_memory[256];

/* End user code. Do not edit comment generated here */



/* Start user code for adding. Do not edit comment generated here */
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
    /* Clear the contents of the simulated EEPROM memory */
    memset(g_sim_eeprom_memory, 0xFF, 256u);

    /* set the reporting status flag to reset state */
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
    uint8_t          iic_num_bytes_received = 0x00;

    /* Current simulated memory read/write address */
    uint8_t          sim_eeprom_pointer = 0x00;

    /* Variable used to store the first two byte sent by the master. The first
    contains the slave address, the second contains the memory to be accessed */
    uint8_t          master_control[2] = {0xff, 0xff};

    /* Declare a dummy variable */
    volatile uint8_t dummy_read;

    /* Variable used to flag the NACK received condition */
    uint8_t          nack_received = 0;

    /* Wait for detection of the START condition */
    while (0x0 == RIIC0.ICSR2.BIT.START)
    {
        /* Wait */
    }

    /* Wait for address match detection */
    while (0x0 == RIIC0.ICSR1.BIT.AAS0)
    {
        /* Wait */
    }

    /* Clear the START detection flag */
    RIIC0.ICSR2.BIT.START = 0;

    /* While in receive mode */
    while (0x0 == RIIC0.ICCR2.BIT.TRS)
    {
        /* Check if the STOP condition has been detected */
        if (0x1 == RIIC0.ICSR2.BIT.STOP)
        {
            /* Check if the receive buffer contains data */
            if (0x1 == RIIC0.ICSR2.BIT.RDRF)
            {
                /* Store the received data */
                g_sim_eeprom_memory[sim_eeprom_pointer++] = RIIC0.ICDRR;
            }

            /* Clear the number of bytes received counter */
            iic_num_bytes_received = 0;

            /* Clear the STOP detection flag */
            RIIC0.ICSR2.BIT.STOP = 0;

            /* Inform user that a write was successful */
            Display_LCD(3, (uint8_t *)"Write OK             ");

            /* Exit receive mode */
        }
        else
        {
            /* Check if data has been received */
            if (0x1 == RIIC0.ICSR2.BIT.RDRF)
            {
                /* The first 2 data bytes contain the slave
                   address and memory location to be accessed */
                if (iic_num_bytes_received < 2)
                {
                    /* Store the received data */
                    master_control[iic_num_bytes_received] = RIIC0.ICDRR;

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
                    g_sim_eeprom_memory[sim_eeprom_pointer++] = RIIC0.ICDRR;
                }

                /* Increment the number of bytes received counter */
                iic_num_bytes_received++;
            }
        }
    }

    /* On detection of the R/W bit set to 1,
       the R5F51115AxFM enters slave transmit mode */
    if (0x1 == RIIC0.ICCR2.BIT.TRS)
    {
        /* Set the sim_eeprom_pointer to the requested memory location */
        sim_eeprom_pointer = master_control[1];

        /* Transmit data as long as the master does not issue a STOP */
        while (0x0 == RIIC0.ICSR2.BIT.STOP)
        {
            /* Check if a NACK hasn't been detected */
            if (0x0 == RIIC0.ICSR2.BIT.NACKF)
            {
                /* Check if the transmit buffer is empty */
                if (0x1 == RIIC0.ICSR2.BIT.TDRE)
                {
                    /* Transmit data until a STOP bit is detected.
                       Increment the memory location address pointer */
                    RIIC0.ICDRT = g_sim_eeprom_memory[sim_eeprom_pointer++];
                }

                /* Wait whilst data is being transmitted */
                while ((0x0 == RIIC0.ICSR2.BIT.TEND) &&
                       (0x0 == nack_received))
                {
                    /* A NACK from the master indicates
                       end of slave transmission */
                    if (0x1 == RIIC0.ICMR3.BIT.ACKBR)
                    {
                        /* Exit the the data transmission wait */
                        nack_received = 0x0;
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
                Display_LCD(3, (uint8_t *)"read OK              ");
            }
        }

        /* Clear the NACKF flag */
        RIIC0.ICSR2.BIT.NACKF = 0;

        /* Clear the STOP detection flag */
        RIIC0.ICSR2.BIT.STOP = 0;

        /* Clear the number of received bytes to zero */
        iic_num_bytes_received = 0;
    }
}
/*******************************************************************************
* End of function SlaveSequence_EEPROM
*******************************************************************************/
/* End user code. Do not edit comment generated here */

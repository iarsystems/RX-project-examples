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
* Creation Date: 2013-08-19
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
#include <stdlib.h>
#include "switch.h"
#include "r_cg_port.h"
#include "r_cg_mtu2.h"
#include "r_cg_icu.h"
#include "lcd.h"
#include "r_cg_userdefine.h"
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
 * Warnings exist for interrupt handlers. */

static MD_STATUS write_eeprom_master(const uint16_t address,
                                     uint8_t * const write_data,
                                     const uint16_t bytes);
static MD_STATUS read_eeprom_master(const uint16_t EEPROM_address,
                                    uint8_t * const destination,
                                    const uint16_t num_bytes);

volatile uint8_t I2C_comm_flag = 0;
volatile uint8_t I2C_error_flag = 0;
/* Define read buffer array */
uint8_t g_read_buffer[] = "xxxxxxxxx";
uint16_t g_eeprom_address;
uint8_t  g_write_count_bcd[2] = {'0', '0'};

/* End user code. Do not edit comment generated here */


/***********************************************************************************************************************
* Function Name: r_riic0_transmit_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_RIIC0_TXI0
#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_TXI0
__fast_interrupt static void r_riic0_transmit_interrupt(void)
#else
__interrupt static void r_riic0_transmit_interrupt(void)
#endif
{
    volatile uint8_t dummy;

    if (g_riic0_mode_flag == _0D_IIC_MASTER_TRANSMIT)
    {
        if (g_riic0_state == _01_IIC_MASTER_SENDS_ADR_7_W)
        {
            RIIC0.ICDRT = (uint8_t)(g_riic0_slave_address << 1U);
            g_riic0_state = _05_IIC_MASTER_SENDS_DATA;
        }
        else if (g_riic0_state == _02_IIC_MASTER_SENDS_ADR_10A_W)
        {
            RIIC0.ICDRT = (uint8_t)(0x00F0U | (g_riic0_slave_address & 0x0300U));
            g_riic0_state = _04_IIC_MASTER_SENDS_ADR_10B;
        }
        else if (g_riic0_state == _04_IIC_MASTER_SENDS_ADR_10B)
        {
            RIIC0.ICDRT = (uint8_t)(g_riic0_slave_address & 0x00FFU);
            g_riic0_state = _05_IIC_MASTER_SENDS_DATA;
        }
        else if (g_riic0_state == _05_IIC_MASTER_SENDS_DATA)
        {
            if (g_riic0_tx_count > 0U)
            {
                RIIC0.ICDRT = *gp_riic0_tx_address;
                gp_riic0_tx_address++;
                g_riic0_tx_count--;
            } 
            else
            {
                g_riic0_state = _06_IIC_MASTER_SENDS_END;
            }
        }
    }
    else if (g_riic0_mode_flag == _0C_IIC_MASTER_RECEIVE)
    {
        if (g_riic0_state == _00_IIC_MASTER_SENDS_ADR_7_R)
        {
            RIIC0.ICDRT = (uint8_t)((g_riic0_slave_address << 1U)  | 0x0001U);
            g_riic0_state = _08_IIC_MASTER_RECEIVES_START;
        }
        else if (g_riic0_state == _02_IIC_MASTER_SENDS_ADR_10A_W)
        {
            RIIC0.ICDRT = (uint8_t)(0x00F0U | (g_riic0_slave_address & 0x0300U));
            g_riic0_state = _04_IIC_MASTER_SENDS_ADR_10B;
        }
        else if (g_riic0_state == _04_IIC_MASTER_SENDS_ADR_10B)
        {
            RIIC0.ICDRT = (uint8_t)(g_riic0_slave_address & 0x00FFU);
            g_riic0_state = _03_IIC_MASTER_SENDS_ADR_10A_R;
        }
        else if (g_riic0_state == _03_IIC_MASTER_SENDS_ADR_10A_R)
        {
            RIIC0.ICDRT = (uint8_t)(0x00F0U | (g_riic0_slave_address & 0x0300U));
            g_riic0_state = _05_IIC_MASTER_SENDS_DATA;
        }
        else if (g_riic0_state == _05_IIC_MASTER_SENDS_DATA)
        {
            if (g_riic0_tx_count > 0U)
            {
                RIIC0.ICDRT = *gp_riic0_tx_address;
                gp_riic0_tx_address++;
                g_riic0_tx_count--;
            } 
            else
            {
                g_riic0_state = _06_IIC_MASTER_SENDS_END;
            }
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_riic0_transmitend_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_RIIC0_TEI0
#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_TEI0
__fast_interrupt static void r_riic0_transmitend_interrupt(void)
#else
__interrupt static void r_riic0_transmitend_interrupt(void)
#endif
{
    volatile uint8_t dummy;

    if (g_riic0_state == _06_IIC_MASTER_SENDS_END)
    {
        RIIC0.ICSR2.BIT.STOP = 0U;
        RIIC0.ICCR2.BIT.SP = 1U;

        while(0U == RIIC0.ICSR2.BIT.STOP);

        RIIC0.ICSR2.BIT.NACKF = 0U;
        RIIC0.ICSR2.BIT.STOP = 0U;

        g_riic0_state = _07_IIC_MASTER_SENDS_STOP;
    }
}
/***********************************************************************************************************************
* Function Name: r_riic0_receive_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_RIIC0_RXI0
#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_RXI0
__fast_interrupt static void r_riic0_receive_interrupt(void)
#else
__interrupt static void r_riic0_receive_interrupt(void)
#endif
{
    volatile uint8_t dummy;
    if (g_riic0_state == _08_IIC_MASTER_RECEIVES_START)
    {
        /* Dummy read to release SCL */
        dummy = RIIC0.ICDRR;  

        g_riic0_state = _09_IIC_MASTER_RECEIVES_DATA;
    }
    else if (g_riic0_state == _09_IIC_MASTER_RECEIVES_DATA)
    {
        if (g_riic0_rx_count < g_riic0_rx_length)
        {
            if (g_riic0_rx_count == g_riic0_rx_length - 3)
            {
                RIIC0.ICMR3.BIT.WAIT = 1U;

                *gp_riic0_rx_address = RIIC0.ICDRR;
                gp_riic0_rx_address++;
                g_riic0_rx_count++;
            }
            else if (g_riic0_rx_count == g_riic0_rx_length - 2)
            {
                RIIC0.ICMR3.BIT.RDRFS = 1U;
                RIIC0.ICMR3.BIT.ACKBT = 1U;

                *gp_riic0_rx_address = RIIC0.ICDRR;
                gp_riic0_rx_address++;
                g_riic0_rx_count++;

                g_riic0_state = _0A_IIC_MASTER_RECEIVES_STOPPING;
            }
            else
            {
                *gp_riic0_rx_address = RIIC0.ICDRR;
                gp_riic0_rx_address++;
                g_riic0_rx_count++;
            }
        }
    }
    else if (g_riic0_state == _0A_IIC_MASTER_RECEIVES_STOPPING)
    {
        RIIC0.ICSR2.BIT.STOP = 0U;
        RIIC0.ICCR2.BIT.SP = 1U;

        *gp_riic0_rx_address = RIIC0.ICDRR;
        gp_riic0_rx_address++;
        g_riic0_rx_count++;

        RIIC0.ICMR3.BIT.ACKWP = 1U;
        RIIC0.ICMR3.BIT.ACKBT = 1U;
        RIIC0.ICMR3.BIT.WAIT = 0U;

        g_riic0_state = _0B_IIC_MASTER_RECEIVES_STOP;
    }
}
/***********************************************************************************************************************
* Function Name: r_riic0_error_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector = VECT_RIIC0_EEI0
#if FAST_INTERRUPT_VECTOR == VECT_RIIC0_EEI0
__fast_interrupt static void r_riic0_error_interrupt(void)
#else
__interrupt static void r_riic0_error_interrupt(void)
#endif
{
    MD_STATUS status = MD_OK;
    volatile uint8_t dummy;

    if ((1U == RIIC0.ICIER.BIT.ALIE) && (1U == RIIC0.ICSR2.BIT.AL))
    {
        status = MD_ERROR1;

        r_riic0_callback_receiveerror(status);
    }
    else if ((1U == RIIC0.ICIER.BIT.TMOIE) && (1U == RIIC0.ICSR2.BIT.TMOF))
    {
        status = MD_ERROR2;

        r_riic0_callback_receiveerror(status);
    }
    else if ((1U == RIIC0.ICIER.BIT.NAKIE) && (1U == RIIC0.ICSR2.BIT.NACKF))
    {
        status = MD_ERROR3;


        /* Dummy read to release SCL */
        dummy = RIIC0.ICDRR;

        while(0U == RIIC0.ICSR2.BIT.STOP);

        RIIC0.ICSR2.BIT.NACKF = 0U;
        RIIC0.ICSR2.BIT.STOP = 0U;

        r_riic0_callback_receiveerror(status);
    }
    else if (g_riic0_mode_flag == _0D_IIC_MASTER_TRANSMIT)
    {
        if ((g_riic0_state == _01_IIC_MASTER_SENDS_ADR_7_W) || (g_riic0_state == _02_IIC_MASTER_SENDS_ADR_10A_W))
        {
            RIIC0.ICSR2.BIT.START = 0U;
            RIIC0.ICIER.BIT.STIE = 0U;
        }
        else if (g_riic0_state == _07_IIC_MASTER_SENDS_STOP)
        {
            RIIC0.ICSR2.BIT.NACKF = 0U;
            RIIC0.ICSR2.BIT.STOP = 0U;

            r_riic0_callback_transmitend();
        }
    }
    else if (g_riic0_mode_flag == _0C_IIC_MASTER_RECEIVE)
    {
        if ((g_riic0_state == _00_IIC_MASTER_SENDS_ADR_7_R) || (g_riic0_state == _02_IIC_MASTER_SENDS_ADR_10A_W))
        {
            RIIC0.ICSR2.BIT.START = 0U;
            RIIC0.ICIER.BIT.STIE = 0U;
        }
        else if (g_riic0_state == _0B_IIC_MASTER_RECEIVES_STOP)
        {
            RIIC0.ICMR3.BIT.RDRFS = 0U;
            RIIC0.ICMR3.BIT.ACKBT = 0U;
            RIIC0.ICSR2.BIT.NACKF = 0U;
            RIIC0.ICSR2.BIT.STOP = 0U;

            r_riic0_callback_receiveend();
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_riic0_callback_transmitend
* Description  : This function is a callback function when I2C bus interface completed data transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_riic0_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */
    I2C_comm_flag = 0;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_riic0_callback_receiveend
* Description  : This function is a callback function when I2C bus interface completed data reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_riic0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    I2C_comm_flag = 0;
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_riic0_callback_receiveerror
* Description  : This function is a callback function when I2C bus interface encounters error while receiving.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_riic0_callback_receiveerror(MD_STATUS status)
{
    /* Start user code. Do not edit comment generated here */
    if (MD_OK != status)
    {
        I2C_comm_flag = 0;
        I2C_error_flag = 1;
    }
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/*******************************************************************************
	Note: If the above code is regenerated using Applilet4, then remove  or
	comment out the following line in the function: r_riic0_error_interrupt(void);

	while(0U == RIIC0.ICSR2.BIT.STOP); // comment this line out or delete it.

*******************************************************************************/


/*******************************************************************************
* Function Name: Init_EEPROM_Master
* Description  : This function configures the IIC unit to operate in bus master
*                mode.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_EEPROM_Master (void)
{
    R_RIIC0_Start();
}
/*******************************************************************************
* End of function Init_EEPROM_Master
*******************************************************************************/

/*******************************************************************************
* Function Name: MasterSequence_EEPROM
* Description  : This function performs an EEPROM
*                read when switch SW2 is pressed, and an EEPROM write when
*                switch SW3 is pressed. The written data is "XX RX111",
*                where XX is an incrementing data identifier number. The read
*                operation checks the read data, and if it matches the written
*                data it reports success on the debug LCD, along with the
*                identifier belonging to that data entry. The last data byte in
*                the write_buffer array is used to indicate the end of
*                transmission. It is not transmitted to the slave device and
*                omitted from the comparison between the write_buffer and
*                g_read_buffer data arrays.
* Argument     : none
* Return value : none
*******************************************************************************/
void MasterSequence_EEPROM (void)
{
    /* Declare a status variable */
    MD_STATUS       status = 0;

    /* Declare write buffer array, initialised with constant data written to the
    EEPROM each time. XX is replaced with the data identifier before writing
    and compared to the g_read_buffer on master read.  Therefore it needs static
    storage class specifier to ensure persistence through successive calls to
    this function */
    static uint8_t  write_buffer[] = "XX RX111";

    /* Declare the data identifier number array, stored as BCD ASCII values with
    range 0 - 99.  This is incremented on each write, so needs the static
    storage class specifier to ensure persistence through successive calls to
    this function */

    /* Define the read and write address variables, initialised to 0 */
    const uint16_t write_address = 0x00;
    const uint16_t read_address = 0x00;

    /* Check if switch SW1 has been pressed */
    if (g_switch_flag & SWITCHHOLD_1)
    {    
        if (g_eeprom_address == INTERNAL_I2C_EEPROM_ADDRESS)
        {
            g_eeprom_address = EXTERNAL_I2C_EEPROM_ADDRESS;  
        }
        else
        {
            g_eeprom_address = INTERNAL_I2C_EEPROM_ADDRESS;
        }
        DisplayClearLine(6);
        DisplayClearLine(7);
        g_write_count_bcd[0] = '0';
        g_write_count_bcd[1] = '0';
        
        while (!g_switch_standby_ready)
        {
            /* Wait until key is relesed */
        }
        
        /* Clear the switch press flag */
        g_switch_flag = 0x00;

    }
    /* Check if switch SW2 has been pressed */
    else if (g_switch_flag & SWITCHHOLD_2)
    {
        /* Store write count in first two bytes of write buffer */
        write_buffer[0] = g_write_count_bcd[0];
        write_buffer[1] = g_write_count_bcd[1];

        /* Perform write */
        status = write_eeprom_master(write_address, write_buffer,
                                     sizeof(write_buffer));

        /* report error */
        if (MD_OK == status)
        {
            /* Initialise temporary LCD buffer string */
            uint8_t lcd_buffer[] = "Wrote  0";

            /* Insert the write count into the first two characters of the
               write buffer */
            lcd_buffer[6] = g_write_count_bcd[0];
            lcd_buffer[7] = g_write_count_bcd[1];

            /* Display contents of lcd buffer to debug LCD */
            DisplayClearLine(6);
            DisplayCenter(6, lcd_buffer);

            /* Increment write counter */
            g_write_count_bcd[1]++;

            /* Ensure write count stays within valid ASCII decimal values */
            if (g_write_count_bcd[1] > '9')
            {
                /* Increment tens decimal digit, and set units back to 0 */
                g_write_count_bcd[0]++;
                g_write_count_bcd[1] = '0';

                /* If write count exceeds 99, reset back to 0 */
                if (g_write_count_bcd[0] > '9')
                {
                    /* Set tens digit back to 0 */
                    g_write_count_bcd[0] = '0';
                }
            }
        }

        /* report write error */
        else
        {
        	DisplayClearLine(6);
        	DisplayCenter(6, (uint8_t *)"Write error");
        }

        /* reset error flag */
        I2C_error_flag = 0;

        /* Clear the switch press flag */
        g_switch_flag = 0x00;
    }

    /* Check if switch SW3 has been pressed */
    else if (g_switch_flag & SWITCHHOLD_3)
    {
        /* Clear the contents of the g_read_buffer array */
        memset(g_read_buffer, 0xFFu, sizeof(g_read_buffer));

        /* Turn on LED2 to indicate IIC bus read activity */
        LED2_On();

        /* Read from the EEPROM device */
        status = read_eeprom_master(read_address, g_read_buffer, sizeof(write_buffer));

        /* Write operation was successful */
        if (MD_OK == status)
        {
            /* Check if data read matches data written */
            if (memcmp((g_read_buffer), (write_buffer), sizeof(write_buffer)))
            {
                /* Mismatch between data sent and received, display error on
                            debug lcd */
            	DisplayClearLine(7);
            	DisplayCenter(7, (uint8_t *)"Read mismatch error");
            }
            /* Read data matches written data */
            else
            {
                /* Declare lcd buffer string array */
                uint8_t lcd_buffer[] = "Read  XX";

                /* Replace the initialised 'XX' with the count value of the
                           read string */
                lcd_buffer[6] = g_read_buffer[0];
                lcd_buffer[7] = g_read_buffer[1];

                /* Write the contents of the lcd buffer to the debug lcd */
                DisplayClearLine(7);
                DisplayCenter(7, lcd_buffer);
            }
        }
        /* Write operation failure */
        else
        {
            /* Display write error on debug LCD */
            DisplayClearLine(6);
            DisplayCenter(7, (uint8_t *)"Read Error");
        }

        /* reset error flag */
        I2C_error_flag = 0;

        /* Clear the switch press flag */
        g_switch_flag = 0x00;
    }
    else
    {
        /* Do nothing */
    }
}
/*******************************************************************************
* End of function MasterSequence_EEPROM
*******************************************************************************/

/*******************************************************************************
* Function Name: write_eeprom_master
* Description  : This function writes data to an address in the EEPROM device.
*
* Argument     : uint16_t address       - The start address of the EEPROM write.
*                uint8_t * write_data   - A pointer to the array of data to
*                                         the EEPROM.
*                uint16_t bytes         - The number of bytes to write.
* Return value : MD_STATUS
*******************************************************************************/
static MD_STATUS write_eeprom_master (const uint16_t address,
                                      uint8_t * const write_data,
                                      const uint16_t bytes)
{
    /* The EEPROM address needs to be sent with the data in 1 single
     * communication, as the first byte. Therefore allocate a temporary
     * memory area to hold the address + data as one block */
    uint8_t * const dataBuffer = malloc((size_t)(bytes + 1));

    uint16_t        loop = 0;
    uint8_t         timed_Out = 0;
    uint16_t		tmp_bytes;

    /* Declare a status variable */
    MD_STATUS       status;

    uint16_t shift_address = (uint16_t)(g_eeprom_address >> 1);

    /* See if the malloc succeeded if not then lock up */
    if (0 == dataBuffer)
    {
        while (1)
        {
            /* Stay here forever */
        }
    }

    /* set up temporary memory area */
    *dataBuffer = (uint8_t)(address & 0x00FF);

    /* copy the data into the temp block */
    for (loop = 0; loop < bytes; loop++)
    {
        *(dataBuffer + loop + 1) = *(write_data + loop);
    }

    /* Turn LED1 on to indicate IIC bus write activity */
    LED1_On();

    /* start timeout measurement for 100ms */
    I2C_timer_start();

    /* ensure I2C send loop is entered */
    status = MD_ERROR1;

    /* Keep trying to communicate if bus is busy */
    while ((MD_ERROR1 == status) && (0 == I2C_timeout()))
    {
    	tmp_bytes = (uint16_t)(bytes + 1);

        /* send EEPROM address and data to EEPROM */
        status = R_RIIC0_Master_Send(shift_address, dataBuffer,
                                     (tmp_bytes));

        /* proceed if initial send command successful */
        if (status == MD_OK)
        {
            /* Wait for transmission to complete */
            while ((g_riic0_state != _07_IIC_MASTER_SENDS_STOP) && (0 == I2C_timeout()))
            {
                /* Wait */
            }
        }
    }

    /* record as timed out if the 100ms timer has expired */
    if (1 == I2C_timeout())
    {
        timed_Out = 1;
    }

    /* wait for at least enough time for LED to have blinked recognisably */
    while (0 == I2C_timeout())
    {
        /* Wait */
    }

    /* Turn LED1 off to indicate IIC bus write activity complete */
    LED1_Off();

    /* report if an error occurred */
    if ((1 == I2C_error_flag) || (1 == timed_Out) || (status != MD_OK))
    {
        I2C_error_flag = 0;
        /* allow error numbers to be returned but if timeout then set as MD_ERROR3 */
        if (MD_OK == status)
        {
            status = MD_ERROR3;
        }
    }

    /* release temporary memory area */
    free(dataBuffer);

    return (status);
}
/*******************************************************************************
* End of function write_eeprom_master
*******************************************************************************/

/*******************************************************************************
* Function Name: read_eeprom_master
* Description  : This function reads the data at the location of the address
*                passed, and copies the data to the output array passed. The
*                function will read the entire address range in one call, as
*                long as the read operation does not request data outside of
*                the EEPROM device's maximum address range. The function first
*                performs a dummy write to set the EEPROM's pointer, then
*                performs a sequential read until all the data has been read.
* Argument     : uint8_t address       - Address to start the read from.
*                uint8_t * destination - destination buffer for read
*                uint8_t num_bytes     - The number of bytes to read.
* Return value : none
* Note         : 1. If invalid input parameters are used the read operation
*                   will not happen.
*******************************************************************************/
static MD_STATUS read_eeprom_master (const uint16_t EEPROM_address,
                                     uint8_t * const destination,
                                     const uint16_t num_bytes)
{
    /* Declare a status variable */
    MD_STATUS   status;

    /* indication of I2C taking too long */
    uint8_t     timed_Out = 0;

    uint16_t 	shift_address =  (uint16_t)(g_eeprom_address >> 1);

    /* Turn LED2 on to indicate IIC bus read activity started */
    LED2_On();

    /* start timeout measurement for 100ms */
    I2C_timer_start();

    /* set I2C busy flag */
    I2C_comm_flag = 1;

    /* set status to ensure the send is performed */
    status = MD_ERROR1;

    /* If the I2C bus is busy, retry until timeout */
    while ((MD_ERROR1 == status) && (0 == I2C_timeout()))
    {
        /* Perform dummy write to set address */
        status = R_RIIC0_Master_Send(shift_address, (uint8_t *)(&EEPROM_address), 1);
    }

    /* wait for dummy write to complete */
    while ((g_riic0_state != _07_IIC_MASTER_SENDS_STOP) &&
           (0 == I2C_timeout()) && (MD_OK == status))
    {
        /* Wait */
    }

    if (MD_OK == status)
    {
        /* perform read incrementing from address sent in dummy write */
        status = R_RIIC0_Master_Receive(shift_address, destination, (uint16_t)num_bytes);

        /* wait for read to complete */
        while ((g_riic0_state != _0B_IIC_MASTER_RECEIVES_STOP) &&
               (0 == I2C_timeout()) && (MD_OK == status))
        {
            /* Wait */
        }
    }

    /* record as timed out if the 100ms timer has expired */
    if (1 == I2C_timeout())
    {
        timed_Out = 1;
    }

    /* wait for at least enough time for LED to have blinked recognisably */
    while (0 == I2C_timeout())
    {
        /* Wait */
    }

    /* Turn LED2 off to indicate IIC bus read activity completed */
    LED2_Off();

    /* report if an error occurred */
    if ((1 == I2C_error_flag) || (1 == timed_Out) || (status != MD_OK))
    {
        I2C_error_flag = 0;
        /* allow error numbers to be returned but if timeout then set as MD_ERROR3 */
        if (MD_OK == status)
        {
            status = MD_ERROR3;
        }
    }

    return (status);
}
/*******************************************************************************
* End of function read_eeprom_master
*******************************************************************************/
/* End user code. Do not edit comment generated here */

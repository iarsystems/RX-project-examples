/*Adapted for IAR Embedded Workbench*/
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
* Copyright (C) 2013, 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_riic_user.c
* Version      : Code Generator for RX64M V1.00.01.01 [09 May 2014]
* Device(s)    : R5F571MLCxFC
* Tool-Chain   : IAR Embedded Workbench
* Description  : This file implements device driver for RIIC module.
* Creation Date: 03/07/2014
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

/* Some of the code in this file is generated using "Code Generator" for e2 studio.
 * Warnings exist in this module. */

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern uint8_t    g_riic2_mode_flag;               /* RIIC2 master transmit receive flag */
extern uint8_t    g_riic2_state;                   /* RIIC2 master state */
extern uint16_t   g_riic2_slave_address;           /* RIIC2 slave address */
extern uint8_t *  gp_riic2_tx_address;             /* RIIC2 transmit buffer address */
extern uint16_t   g_riic2_tx_count;                /* RIIC2 transmit data number */
extern uint8_t *  gp_riic2_rx_address;             /* RIIC2 receive buffer address */
extern uint16_t   g_riic2_rx_count;                /* RIIC2 receive data number */
extern uint16_t   g_riic2_rx_length;               /* RIIC2 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */



/***********************************************************************************************************************
* Function Name: r_riic2_transmit_interrupt
* Description  : This function is TXI2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector=VECT(RIIC2,TXI2)
__interrupt static void r_riic2_transmit_interrupt(void)
{
    if (g_riic2_mode_flag == _0D_IIC_MASTER_TRANSMIT)
    {
        if (g_riic2_state == _01_IIC_MASTER_SENDS_ADR_7_W)
        {
            RIIC2.ICDRT = (uint8_t)(g_riic2_slave_address << 1U);
            g_riic2_state = _05_IIC_MASTER_SENDS_DATA;
        }
        else if (g_riic2_state == _02_IIC_MASTER_SENDS_ADR_10A_W)
        {
            RIIC2.ICDRT = (uint8_t)(((g_riic2_slave_address & 0x0300U) >> 7U) | 0x00F0U);
            g_riic2_state = _04_IIC_MASTER_SENDS_ADR_10B;
        }
        else if (g_riic2_state == _04_IIC_MASTER_SENDS_ADR_10B)
        {
            RIIC2.ICDRT = (uint8_t)(g_riic2_slave_address & 0x00FFU);
            g_riic2_state = _05_IIC_MASTER_SENDS_DATA;
        }
        else if (g_riic2_state == _05_IIC_MASTER_SENDS_DATA)
        {
            if (g_riic2_tx_count > 0U)
            {
                RIIC2.ICDRT = *gp_riic2_tx_address;
                gp_riic2_tx_address++;
                g_riic2_tx_count--;
            } 
            else
            {
                g_riic2_state = _06_IIC_MASTER_SENDS_END;
            }
        }
    }
    else if (g_riic2_mode_flag == _0C_IIC_MASTER_RECEIVE)
    {
        if (g_riic2_state == _00_IIC_MASTER_SENDS_ADR_7_R)
        {
            RIIC2.ICDRT = (uint8_t)((g_riic2_slave_address << 1U) | 0x0001U);
            g_riic2_state = _08_IIC_MASTER_RECEIVES_START;
        }
        else if (g_riic2_state == _02_IIC_MASTER_SENDS_ADR_10A_W)
        {
            RIIC2.ICDRT = (uint8_t)(((g_riic2_slave_address & 0x0300U) >> 7U) | 0x00F0U);
            g_riic2_state = _04_IIC_MASTER_SENDS_ADR_10B;
        }
        else if (g_riic2_state == _04_IIC_MASTER_SENDS_ADR_10B)
        {
            RIIC2.ICDRT = (uint8_t)(g_riic2_slave_address & 0x00FFU);
            g_riic2_state = _0E_IIC_MASTER_RECEIVES_RESTART;
        }
        else if (g_riic2_state == _0E_IIC_MASTER_RECEIVES_RESTART)
        {
            RIIC2.ICSR2.BIT.START = 0U;
            RIIC2.ICIER.BIT.STIE = 1U;
            RIIC2.ICCR2.BIT.RS = 1U;    /* Set restart condition flag */
        }
        else if (g_riic2_state == _03_IIC_MASTER_SENDS_ADR_10A_R)
        {
            RIIC2.ICDRT = (uint8_t)(((g_riic2_slave_address & 0x0300U) >> 7U) | 0x00F1U);
            g_riic2_state = _08_IIC_MASTER_RECEIVES_START;
        }
    }
}
  
/***********************************************************************************************************************
* Function Name: r_riic2_transmitend_interrupt
* Description  : This function is TEI2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_riic2_transmitend_interrupt(void)
{
    if (g_riic2_state == _06_IIC_MASTER_SENDS_END)
    {
        RIIC2.ICSR2.BIT.STOP = 0U;
        RIIC2.ICCR2.BIT.SP = 1U;

        g_riic2_state = _07_IIC_MASTER_SENDS_STOP;
    }
}
/***********************************************************************************************************************
* Function Name: r_riic2_receive_interrupt
* Description  : This function is RXI2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
#pragma vector=VECT(RIIC2,RXI2)
__interrupt static void r_riic2_receive_interrupt(void)
{
    volatile uint8_t dummy;

    if (g_riic2_state == _08_IIC_MASTER_RECEIVES_START)
    {
        /* Dummy read to release SCL */
        dummy = RIIC2.ICDRR;  

        g_riic2_state = _09_IIC_MASTER_RECEIVES_DATA;
    }
    else if (g_riic2_state == _09_IIC_MASTER_RECEIVES_DATA)
    {
        if (g_riic2_rx_count < g_riic2_rx_length)
        {
            if (g_riic2_rx_count == g_riic2_rx_length - 3)
            {
                RIIC2.ICMR3.BIT.WAIT = 1U;

                *gp_riic2_rx_address = RIIC2.ICDRR;
                gp_riic2_rx_address++;
                g_riic2_rx_count++;
            }
            else if (g_riic2_rx_count == g_riic2_rx_length - 2)
            {
                RIIC2.ICMR3.BIT.RDRFS = 1U;
                RIIC2.ICMR3.BIT.ACKWP = 1U;
                RIIC2.ICMR3.BIT.ACKBT = 1U;

                *gp_riic2_rx_address = RIIC2.ICDRR;
                gp_riic2_rx_address++;
                g_riic2_rx_count++;

                g_riic2_state = _0A_IIC_MASTER_RECEIVES_STOPPING;
            }
            else
            {
                *gp_riic2_rx_address = RIIC2.ICDRR;
                gp_riic2_rx_address++;
                g_riic2_rx_count++;
            }
        }
    }
    else if (g_riic2_state == _0A_IIC_MASTER_RECEIVES_STOPPING)
    {
        RIIC2.ICSR2.BIT.STOP = 0U;
        RIIC2.ICCR2.BIT.SP = 1U;

        *gp_riic2_rx_address = RIIC2.ICDRR;
        gp_riic2_rx_address++;
        g_riic2_rx_count++;

        RIIC2.ICMR3.BIT.ACKWP = 1U;
        RIIC2.ICMR3.BIT.ACKBT = 1U;
        RIIC2.ICMR3.BIT.WAIT = 0U;

        g_riic2_state = _0B_IIC_MASTER_RECEIVES_STOP;
    }
}
/***********************************************************************************************************************
* Function Name: r_riic2_error_interrupt
* Description  : This function is EEI2 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_riic2_error_interrupt(void)
{
    volatile uint8_t dummy;

    if ((1U == RIIC2.ICIER.BIT.ALIE) && (1U == RIIC2.ICSR2.BIT.AL))
    {
        r_riic2_callback_receiveerror(MD_ERROR1);
    }
    else if ((1U == RIIC2.ICIER.BIT.TMOIE) && (1U == RIIC2.ICSR2.BIT.TMOF))
    {
        r_riic2_callback_receiveerror(MD_ERROR2);
    }
    else if ((1U == RIIC2.ICIER.BIT.NAKIE) && (1U == RIIC2.ICSR2.BIT.NACKF))
    {
        /* Dummy read to release SCL */
        dummy = RIIC2.ICDRR;

        RIIC2.ICSR2.BIT.NACKF = 0U;

        r_riic2_callback_receiveerror(MD_ERROR3);
    }
    else if (g_riic2_mode_flag == _0D_IIC_MASTER_TRANSMIT)
    {
        if ((g_riic2_state == _01_IIC_MASTER_SENDS_ADR_7_W) || (g_riic2_state == _02_IIC_MASTER_SENDS_ADR_10A_W))
        {
            RIIC2.ICSR2.BIT.START = 0U;
            RIIC2.ICIER.BIT.STIE = 0U;
        }
        else if (g_riic2_state == _07_IIC_MASTER_SENDS_STOP)
        {
            RIIC2.ICSR2.BIT.NACKF = 0U;
            RIIC2.ICSR2.BIT.STOP = 0U;

            r_riic2_callback_transmitend();
        }
    }
    else if (g_riic2_mode_flag == _0C_IIC_MASTER_RECEIVE)
    {
        if ((g_riic2_state == _00_IIC_MASTER_SENDS_ADR_7_R) || (g_riic2_state == _02_IIC_MASTER_SENDS_ADR_10A_W))
        {
            RIIC2.ICSR2.BIT.START = 0U;
            RIIC2.ICIER.BIT.STIE = 0U;
        }
        else if (g_riic2_state == _0E_IIC_MASTER_RECEIVES_RESTART)
        {
            RIIC2.ICSR2.BIT.START = 0U;
            RIIC2.ICIER.BIT.STIE = 0U;
            g_riic2_state = _03_IIC_MASTER_SENDS_ADR_10A_R;
        }
        else if (g_riic2_state == _0B_IIC_MASTER_RECEIVES_STOP)
        {
            RIIC2.ICMR3.BIT.RDRFS = 0U;
            RIIC2.ICMR3.BIT.ACKWP = 1U;
            RIIC2.ICMR3.BIT.ACKBT = 0U;
            RIIC2.ICSR2.BIT.NACKF = 0U;
            RIIC2.ICSR2.BIT.STOP = 0U;

            r_riic2_callback_receiveend();
        }
    }
}
/***********************************************************************************************************************
* Function Name: r_riic2_callback_transmitend
* Description  : This function is a callback function when I2C bus interface completed data transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_riic2_callback_transmitend(void)
{
    /* Start user code. Do not edit comment generated here */

    /* Set the g_tx_complete flag */
    g_tx_complete = 1;

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_riic2_callback_receiveend
* Description  : This function is a callback function when I2C bus interface completed data reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_riic2_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */

    /* Set the g_rx_complete flag */
    g_rx_complete = 1;

    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_riic2_callback_receiveerror
* Description  : This function is a callback function when I2C bus interface encounters error while receiving.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_riic2_callback_receiveerror(MD_STATUS status)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

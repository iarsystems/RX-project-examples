/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_riic_user.c
* Version      : Applilet4 for RX113 V1.01.00.02 [31 Jul 2014]
* Device(s)    : R5F51138AxFP
* Tool-Chain   : IAR Systems ICCRX
* Description  : This file implements device driver for RIIC module.
* Creation Date: 2015-01-20
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
/* Start user code for global. Do not edit comment generated here */
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

    if (g_riic0_mode_flag == _0D_IIC_MASTER_TRANSMIT)
    {
        if (g_riic0_state == _01_IIC_MASTER_SENDS_ADR_7_W)
        {
            RIIC0.TMOCNTL.BYTE = 0x00U;
            RIIC0.TMOCNTU.BYTE = 0x00U;
            RIIC0.ICDRT = (uint8_t)(g_riic0_slave_address << 1U);
            g_riic0_state = _05_IIC_MASTER_SENDS_DATA;
        }
        else if (g_riic0_state == _02_IIC_MASTER_SENDS_ADR_10A_W)
        {
            RIIC0.ICDRT = (uint8_t)(((g_riic0_slave_address & 0x0300U) >> 7U) | 0x00F0U);
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
                RIIC0.TMOCNTL.BYTE = 0x00U;
                RIIC0.TMOCNTU.BYTE = 0x00U;
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
            RIIC0.TMOCNTL.BYTE = 0x00U;
            RIIC0.TMOCNTU.BYTE = 0x00U;
            RIIC0.ICDRT = (uint8_t)((g_riic0_slave_address << 1U)  | 0x0001U);
            g_riic0_state = _08_IIC_MASTER_RECEIVES_START;
        }
        else if (g_riic0_state == _02_IIC_MASTER_SENDS_ADR_10A_W)
        {
            RIIC0.ICDRT = (uint8_t)(((g_riic0_slave_address & 0x0300U) >> 7U) | 0x00F0U);
            g_riic0_state = _04_IIC_MASTER_SENDS_ADR_10B;
        }
        else if (g_riic0_state == _04_IIC_MASTER_SENDS_ADR_10B)
        {
            RIIC0.ICDRT = (uint8_t)(g_riic0_slave_address & 0x00FFU);
            g_riic0_state = _0E_IIC_MASTER_RECEIVES_RESTART;
        }
        else if (g_riic0_state == _0E_IIC_MASTER_RECEIVES_RESTART)
        {
            RIIC0.ICIER.BIT.STIE = 1U;

            /* Set restart condition flag */
            RIIC0.ICCR2.BIT.RS = 1U;
        }
        else if (g_riic0_state == _03_IIC_MASTER_SENDS_ADR_10A_R)
        {
            RIIC0.ICDRT = (uint8_t)(((g_riic0_slave_address & 0x0300U) >> 7U) | 0x00F1U);
            g_riic0_state = _08_IIC_MASTER_RECEIVES_START;
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

    if (g_riic0_state == _06_IIC_MASTER_SENDS_END)
    {
        RIIC0.TMOCNTL.BYTE = 0x00U;
        RIIC0.TMOCNTU.BYTE = 0x00U;
        RIIC0.ICSR2.BIT.STOP = 0U;
        RIIC0.ICCR2.BIT.SP = 1U;

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
        RIIC0.TMOCNTL.BYTE = 0x00U;
        RIIC0.TMOCNTU.BYTE = 0x00U;
        /* Dummy read to release SCL0 */
        dummy = RIIC0.ICDRR;  

        g_riic0_state = _09_IIC_MASTER_RECEIVES_DATA;
    }
    else if (g_riic0_state == _09_IIC_MASTER_RECEIVES_DATA)
    {
        if (g_riic0_rx_count < g_riic0_rx_length)
        {
            RIIC0.TMOCNTL.BYTE = 0x00U;
            RIIC0.TMOCNTU.BYTE = 0x00U;
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
                RIIC0.ICMR3.BIT.ACKWP = 1U;
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
        RIIC0.TMOCNTL.BYTE = 0x00U;
        RIIC0.TMOCNTU.BYTE = 0x00U;
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
    volatile uint8_t dummy;

    if ((1U == RIIC0.ICIER.BIT.ALIE) && (1U == RIIC0.ICSR2.BIT.AL))
    {
        r_riic0_callback_receiveerror(MD_ERROR1);
    }
    else if ((1U == RIIC0.ICIER.BIT.TMOIE) && (1U == RIIC0.ICSR2.BIT.TMOF))
    {
        r_riic0_callback_receiveerror(MD_ERROR2);
    }
    else if ((1U == RIIC0.ICIER.BIT.NAKIE) && (1U == RIIC0.ICSR2.BIT.NACKF))
    {
        RIIC0.TMOCNTL.BYTE = 0x00U;
        RIIC0.TMOCNTU.BYTE = 0x00U;
        /* Dummy read to release SCL0 */
        dummy = RIIC0.ICDRR;

        RIIC0.TMOCNTL.BYTE = 0x00U;
        RIIC0.TMOCNTU.BYTE = 0x00U;
        RIIC0.ICSR2.BIT.NACKF = 0U;

        r_riic0_callback_receiveerror(MD_ERROR3);
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
            RIIC0.TMOCNTL.BYTE = 0x00U;
            RIIC0.TMOCNTU.BYTE = 0x00U;
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
        else if (g_riic0_state == _0E_IIC_MASTER_RECEIVES_RESTART)
        {
            RIIC0.ICSR2.BIT.START = 0U;
            RIIC0.ICIER.BIT.STIE = 0U;
            g_riic0_state = _03_IIC_MASTER_SENDS_ADR_10A_R;
        }
        else if (g_riic0_state == _0B_IIC_MASTER_RECEIVES_STOP)
        {
            RIIC0.TMOCNTL.BYTE = 0x00U;
            RIIC0.TMOCNTU.BYTE = 0x00U;
            RIIC0.ICMR3.BIT.RDRFS = 0U;
            RIIC0.ICMR3.BIT.ACKWP = 1U;
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

	/* Set the g_tx_complete flag */
    g_tx_complete = 1;

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

	/* Set the g_rx_complete flag */
    g_rx_complete = 1;

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
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

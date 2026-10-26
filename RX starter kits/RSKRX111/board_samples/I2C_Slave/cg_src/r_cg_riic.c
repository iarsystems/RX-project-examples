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
* File Name    : r_cg_riic.c
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
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
uint8_t    g_riic0_mode_flag;               /* RIIC0 master transmit receive flag */
uint8_t    g_riic0_state;                   /* RIIC0 state */
uint16_t   g_riic0_slave_address;           /* RIIC0 slave address */
uint8_t *  gp_riic0_tx_address;             /* RIIC0 transmit buffer address */
uint16_t   g_riic0_tx_count;                /* RIIC0 transmit data number */
uint8_t *  gp_riic0_rx_address;             /* RIIC0 receive buffer address */
uint16_t   g_riic0_rx_count;                /* RIIC0 receive data number */
uint16_t   g_riic0_rx_length;               /* RIIC0 receive data length */
uint8_t    g_riic0_dummy_read_count;        /* RIIC0 count for dummy read */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_RIIC0_Create
* Description  : This function initializes the I2C Bus Interface.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RIIC0_Create(void)
{
    SYSTEM.MSTPCRB.BIT.MSTPB21 = 0U; /* Cancel RIIC0 module stop state */
    RIIC0.ICCR1.BIT.ICE = 0U;        /* SCL and SDA pins in inactive state */
    RIIC0.ICCR1.BIT.IICRST = 1U;     /* RIIC reset */
    RIIC0.ICCR1.BIT.ICE = 1U;        /* Internal reset */

    /* Set interrupt priority */
    IPR(RIIC0,TXI0) |= _0F_IIC_PRIORITY_LEVEL15; /* Set TXI priority */
    IPR(RIIC0,TEI0) |= _0F_IIC_PRIORITY_LEVEL15; /* Set TEI priority */
    IPR(RIIC0,RXI0) |= _0F_IIC_PRIORITY_LEVEL15; /* Set RXI priority */
    IPR(RIIC0,EEI0) |= _0F_IIC_PRIORITY_LEVEL15; /* Set EEI priority */

    /* Set SARLy and SARUy */
    RIIC0.SARU0.BYTE |= _00_IIC_SARU0_7_BIT;
    RIIC0.SARL0.BYTE |= 0x60U;

    /* Set ICSER */
    RIIC0.ICSER.BYTE |= _01_IIC_SLAVE_0_ENABLE | _00_IIC_SLAVE_1_DISABLE | _00_IIC_SLAVE_2_DISABLE | 
                        _08_IIC_GENERAL_CALL_ADDRESS_ENABLE | _00_IIC_DEVICEID_DETECT_DISABLE | 
                        _00_IIC_HOST_ADDRESS_DETECT_DISABLE;

    /* Set transfer bit rate */
    RIIC0.ICMR1.BYTE |= _00_IIC_PCLK_DIV_1;
    RIIC0.ICBRL.BYTE = _EE_IIC_SCL_LOW_LEVEL_PERIOD;
    RIIC0.ICBRH.BYTE = _EE_IIC_SCL_HIGH_LEVEL_PERIOD;

    /* Set ICMR2 and ICMR3 */
    RIIC0.ICMR2.BYTE |= _00_IIC_NO_OUTPUT_DELAY;
    RIIC0.ICMR3.BYTE |= _00_IIC_IICBUS;

    /* Set ICFER */
    RIIC0.ICFER.BYTE |= _00_IIC_TIMEOUT_FUNCTION_DISABLE | _00_IIC_MASTER_ARBITRATION_DISABLE | 
                        _00_IIC_NACK_ARBITRATION_DISABLE | _00_IIC_SLAVE_ARBITRATION_DISABLE | 
                        _10_IIC_NACK_SUSPENSION_DISABLE | _00_IIC_NOISE_FILTER_UNUSED;

    /* Set ICIER */
    RIIC0.ICIER.BYTE |= _00_IIC_TIMEOUT_INTERRUPT_DISABLE | _00_IIC_ARBITRATION_LOST_INTERRUPT_DISABLE | 
                        _00_IIC_START_CONDITION_INTERRUPT_DISABLE | _00_IIC_STOP_CONDITION_INTERRUPT_DISABLE | 
                        _00_IIC_NACK_INTERRUPT_DISABLE | _00_IIC_RECEIVE_DATA_INTERRUPT_DISABLE | 
                        _00_IIC_TRANSMIT_END_INTERRUPT_DISABLE | _00_IIC_TRANSMIT_EMPTY_INTERRUPT_DISABLE;

    /* Cancel internal reset */
    RIIC0.ICCR1.BIT.IICRST = 0;

    /* Set SCL pin */
    MPC.PB0PFS.BYTE = 0x0FU;
    PORTB.PDR.BYTE |= 0x01U;
    PORTB.PMR.BYTE |= 0x01U;

    /* Set SDA pin */
    MPC.PA6PFS.BYTE = 0x0FU;
    PORTA.PDR.BYTE &= 0xBFU;
    PORTA.PMR.BYTE |= 0x40U;
}

/***********************************************************************************************************************
* Function Name: R_RIIC0_Start
* Description  : This function Starts the I2C Bus Interface.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RIIC0_Start(void)
{
    /* Enable I2C interrupt */
    IEN(RIIC0,TXI0) = 1U;
    IEN(RIIC0,TEI0) = 1U;
    IEN(RIIC0,RXI0) = 1U;
    IEN(RIIC0,EEI0) = 1U;

    /* Clear interrupt flag */
    IR(RIIC0,TXI0) = 0U;
    IR(RIIC0,TEI0) = 0U;
    IR(RIIC0,RXI0) = 0U;
    IR(RIIC0,EEI0) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_RIIC0_Stop
* Description  : This function Stops the I2C Bus Interface.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RIIC0_Stop(void)
{
    /* Clear interrupt flag */
    IR(RIIC0,TXI0) = 0U;
    IR(RIIC0,TEI0) = 0U;
    IR(RIIC0,RXI0) = 0U;
    IR(RIIC0,EEI0) = 0U;

    /* Disable I2C interrupt */
    IEN(RIIC0,TXI0) = 0U;
    IEN(RIIC0,TEI0) = 0U;
    IEN(RIIC0,RXI0) = 0U;
    IEN(RIIC0,EEI0) = 0U;
}


/***********************************************************************************************************************
* Function Name: R_RIIC0_Slave_Send
* Description  : This function write data to a master device.
* Arguments    : txbuf -
*                    transmit buffer pointer
*                txnum -
*                    transmit data length
* Return Value : None
***********************************************************************************************************************/
void R_RIIC0_Slave_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    g_riic0_tx_count = tx_num;
    gp_riic0_tx_address = tx_buf;
    RIIC0.ICMR3.BYTE |= 0x70U;
    RIIC0.ICMR3.BIT.ACKBT = 0U;

    g_riic0_mode_flag = _17_IIC_SLAVE_TRANSMIT;
    g_riic0_state = _15_IIC_SLAVE_WAIT_START_CONDITION;
}

/***********************************************************************************************************************
* Function Name: R_RIIC0_Slave_Receive
* Description  : This function read data from a master device.
* Arguments    : rxbuf -
*                    receive buffer pointer
*                rxnum -
*                    receive data length
* Return Value : None
***********************************************************************************************************************/
void R_RIIC0_Slave_Receive(uint8_t * const rx_buf, uint16_t rx_num)
{
    g_riic0_rx_length = rx_num;
    g_riic0_rx_count = 0U;
    gp_riic0_rx_address = rx_buf;
    RIIC0.ICMR3.BYTE |= 0x70U;
    RIIC0.ICMR3.BIT.ACKBT = 0U;
    g_riic0_dummy_read_count = 0U;

    g_riic0_mode_flag = _16_IIC_SLAVE_RECEIVE;
    g_riic0_state = _15_IIC_SLAVE_WAIT_START_CONDITION;
}

/***********************************************************************************************************************
* Function Name: R_RIIC0_StartCondition
* Description  : This function generate I2C start condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RIIC0_StartCondition(void)
{
    RIIC0.ICCR2.BIT.ST = 1U;    /* Set start condition flag */
}
/***********************************************************************************************************************
* Function Name: R_RIIC0_StopCondition
* Description  : This function generate I2C stop condition.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RIIC0_StopCondition(void)
{
    RIIC0.ICCR2.BIT.SP = 1U;    /* Set stop condition flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

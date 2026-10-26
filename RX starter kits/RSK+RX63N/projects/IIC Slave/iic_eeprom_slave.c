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
* File Name     : iic_eeprom_slave.c 
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines IIC functions used in this sample. The functions 
*                  defined operate on the following Renesas IIC EEPROM devices:
*                   R1EX24xxx series, 16Kbit EEPROM, 400KHz
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
/* Provides declarations of functions defined in this file */
#include "iic_eeprom_slave.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare the simulated EEPROM data table */
extern uint8_t gSIM_EEPROM_Memory[256];

/*******************************************************************************
* Outline      : Init_EEPROM_Slave
* Description  : This function configures the IIC unit to operate in bus slave
*                 mode. See the iic_eeprom_slave.h header file for configuration
*                 options.
* Argument     : none
* Return value : none
*******************************************************************************/
void Init_EEPROM_Slave(void)
{    
    /* Protection off */
    SYSTEM.PRCR.WORD = 0xA503;
    /* Cancel the RIIC0 module clock stop mode */
    MSTP_RIIC0 = 0;    
    /* Set SCL0 and SDA0 port pin directions to inputs */
    PORT1.PDR.BYTE &= 0xF3;
    /* Set SCL0 and SDA0 port pin for peripheral use */
    PORT1.PMR.BYTE |= 0x0C;        
    /* Protection on */
    SYSTEM.PRCR.WORD = 0xA500;
    
    /* Enable write to PFSWE bit */
    MPC.PWPR.BYTE = 0x00;
    /* Disable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x40;
    /* Select the pin function as SCL0 */
    MPC.P12PFS.BIT.PSEL = 0xF;
    /* Set pin 3 on Port1 as a normal pin */
    MPC.P13PFS.BIT.ISEL = 0;
    /* Select the pin function as SDA0 */
    MPC.P13PFS.BIT.PSEL = 0xF;
    /* Enable write protection to PFS registers */
    MPC.PWPR.BYTE = 0x80;

    /* Disable the RIIC0  */
    RIIC0.ICCR1.BIT.ICE = 0;
    /* Issue an internal RIIC0 reset */
    RIIC0.ICCR1.BIT.IICRST = 0x1;
    /* Cancel the internal reset */
    RIIC0.ICCR1.BIT.IICRST = 0x0;

    /* Set the transfer clock to PCLK/8=6MHz */
    RIIC0.ICMR1.BIT.CKS = 0x3;                    
    /* Using 50% duty */
    /* Set the transfer bit rate to 100kbps */
    RIIC0.ICBRL.BYTE = 0x7F;
    /* Set the transfer rate to 100kbps */
    RIIC0.ICBRH.BYTE = 0x7F;
    
    /* Set the address mode to 7-bits */
    RIIC0.SARU0.BIT.FS = 0;
    
    /* Set the slave address */
    RIIC0.SARL0.BYTE = SIM_EEPROM_DEVICE_ADDRESS;
        
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
    
    /* Configure the IIC bus function:
       Disable the timeout function
       Disbale master arbitration-lost detection
       Disable slave arbitration-lost detection
       Enable suspension of transfer operation during NACK reception
       No digital noise filtering used 
       Disable SCL synchronous circuit
       Disable the fast-mode */            
    RIIC0.ICFER.BYTE = 0x10;    
    
    /* Enable RIIC bus interface */
    RIIC0.ICCR1.BIT.ICE = 0x1;        
    
    /* Clear the contents of the simulated EEPROM memory */
    memset(gSIM_EEPROM_Memory, 0x00, 256u);
}
/*******************************************************************************
* End of function Init_EEPROM_slave
*******************************************************************************/

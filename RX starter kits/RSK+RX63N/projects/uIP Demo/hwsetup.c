/*******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. 

* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : hwsetup.c
* Version       : 1.0
* Device(s)     : R5F563NB
* Tool-Chain    : Renesas RX Standard Toolchain 1.1.0.0
* OS            : None
* H/W Platform  : RSK+RX63N
* Description   : Sets up the hardware after reset
*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 01.10.2011     1.00        First release
*         : 16.05.2012     1.01        Modified for Ethernet demo 
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
/* Defines standard type structures */
#include <stdint.h>
/* Defines boolean type structures */
#include <stdbool.h>
/* Defines Special Flag Registers (SFR)s for RX63N */
#include "iorx63n.h"
/* Defines hardware plaform */
#include "platform.h"

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* MCU I/O port configuration function delcaration */
static void output_ports_configure(void);

/* Ethernet port configuration function delcaration */
static void ethernet_ports_configure(void);

/* Interrupt configuration function delcaration */
static void interrupts_configure(void);

/* MCU peripheral module configuration function declaration */
static void peripheral_modules_enable(void);

/******************************************************************************
* Function name: hardware_setup
* Description  : Contains setup functions called at device restart
* Arguments    : none
* Return value : none
******************************************************************************/
void HardwareSetup(void)
{
    output_ports_configure();
    ethernet_ports_configure();
    peripheral_modules_enable();
    interrupts_configure();
} 

/******************************************************************************
* Function name: output_ports_configure
* Description  : Configures the port and pin direction settings, and sets the
*                pin outputs to a safe level. Peripheral pin configurations 
*                are done in each peripheral initialization functions.  
* Arguments    : none
* Return value : none
******************************************************************************/
static void output_ports_configure(void)
{
    /* Port 0 */
    PORT0.PODR.BYTE = 0x28;    /* LED0 and LED1 are set high */
    PORT0.PMR.BYTE  = 0x00;    /* GPIO */
    PORT0.PDR.BYTE  = 0x28;    /* LED0 and LED1 are outputs */

    /* Port 1 */
    PORT1.PODR.BYTE = 0x03;    /* LED2 and LED3 are set high */
    PORT1.PMR.BYTE  = 0x00;    /* GPIO */
    PORT1.PDR.BYTE  = 0x03;    /* LED2 and LED3 are outputs */

    /* Port 2 */
    PORT2.PODR.BYTE = 0x00;    /* */
    PORT2.PMR.BYTE  = 0x00;    /* GPIO */
    PORT2.PDR.BYTE  = 0x00;    /* */
    
    /* Port 3 */
    PORT3.PODR.BYTE = 0x00;    /* */
    PORT3.PMR.BYTE  = 0x00;    /* GPIO */
    PORT3.PDR.BYTE  = 0x00;    /* */
    
    /* Port 4 */
    PORT4.PODR.BYTE = 0x00;    /* */
    PORT4.PMR.BYTE  = 0x00;    /* GPIO */
    PORT4.PDR.BYTE  = 0x00;    /* */

    /* Port 5 */
    PORT5.PODR.BYTE = 0x00;    /* */
    PORT5.PMR.BYTE  = 0x00;    /* GPIO */
    PORT5.PDR.BYTE  = 0x00;    /* */
    
    /* Port 6 */
    PORT6.PODR.BYTE = 0x00;    /* */
    PORT6.PMR.BYTE  = 0x00;    /* GPIO */
    PORT6.PDR.BYTE  = 0x00;    /* */
    
    /* Port 7 */
    PORT7.PODR.BYTE = 0x00;    /* */
    PORT7.PMR.BYTE  = 0x00;    /* GPIO */
    PORT7.PDR.BYTE  = 0x00;    /* */
    
    /* Port 8 */
    PORT8.PODR.BYTE = 0x00;    /* */
    PORT8.PMR.BYTE  = 0x00;    /* GPIO */
    PORT8.PDR.BYTE  = 0x00;    /* */
    
    /* Port 9 */
    PORT9.PODR.BYTE = 0x00;    /* */
    PORT9.PMR.BYTE  = 0x00;    /* GPIO */
    PORT9.PDR.BYTE  = 0x00;    /* */

    /* Port A */
    PORTA.PODR.BYTE = 0x00;    /* */
    PORTA.PMR.BYTE  = 0x00;    /* GPIO */
    PORTA.PDR.BYTE  = 0x00;    /* */
    
    /* Port B */
    PORTB.PODR.BYTE = 0x00;    /* */
    PORTB.PMR.BYTE  = 0x00;    /* GPIO */
    PORTB.PDR.BYTE  = 0x00;    /* */
    
    /* Port C */
    PORTC.PODR.BYTE = 0x00;    /* */
    PORTC.PMR.BYTE  = 0x00;    /* GPIO */
    PORTC.PDR.BYTE  = 0x00;    /* */
    
    /* Port D */
    PORTD.PODR.BYTE = 0x00;    /* */
    PORTD.PMR.BYTE  = 0x00;    /* GPIO */
    PORTD.PDR.BYTE  = 0x0F;    /* */
    
    /* Port E */
    PORTE.PODR.BYTE = 0x00;    /* */
    PORTE.PMR.BYTE  = 0x00;    /* GPIO */
    PORTE.PDR.BYTE  = 0x00;    /* */
    
    /* Port F */
    PORTF.PODR.BYTE = 0x00;    /* */
    PORTF.PMR.BYTE  = 0x00;    /* GPIO */
    PORTF.PDR.BYTE  = 0x00;    /* */
    
    /* Port J */
    PORTJ.PODR.BYTE = 0x00;    /* */
    PORTJ.PMR.BYTE  = 0x00;    /* GPIO */
    PORTJ.PDR.BYTE  = 0x00;    /* */
}

/******************************************************************************
* Function name: ethernet_ports_configure
* Description  : Configures the port and pin direction settings, and sets the
*                pin outputs to a safe level. Peripheral pin configurations 
*                are done in each peripheral initialization functions.  
* Arguments    : none
* Return value : none
******************************************************************************/
static void ethernet_ports_configure(void)
{
    /* Unlock protection register */
    MPC.PWPR.BIT.B0WI = 0;
    /* Unlock MPC registers */
    MPC.PWPR.BIT.PFSWE = 1;         

#if ETH_MODE_SEL == ETH_MII_MODE

    /* Set PHY mode to MII */
    MPC.PFENET.BIT.PHYMODE = 1;

    /* Port 7 - Ethernet signals */
    MPC.P71PFS.BYTE = 0x11;     /* P71 - MDIO   input/output    */
    MPC.P72PFS.BYTE = 0x11;     /* P72 - MDC    output          */
    MPC.P74PFS.BYTE = 0x11;     /* P74 - RXD1   input           */
    MPC.P75PFS.BYTE = 0x11;     /* P75 - RXD0   input           */
    MPC.P76PFS.BYTE = 0x11;     /* P76 - RXCLK  input           */
    MPC.P77PFS.BYTE = 0x11;     /* P77 - RXER   input           */
    PORT7.PMR.BYTE = 0xF6;      /* Pins above assigned to Ether */
    PORT7.PDR.BYTE = 0x06;      /* P71-2 are output             */

    /* Port 8 - Ethernet signals */
    MPC.P80PFS.BYTE = 0x11;     /* P80 - TXEN   output          */
    MPC.P81PFS.BYTE = 0x11;     /* P81 - TXD0   output          */
    MPC.P82PFS.BYTE = 0x11;     /* P82 - TXD1   output          */
    MPC.P83PFS.BYTE = 0x11;     /* P83 - CRS    input           */
    PORT8.PMR.BYTE = 0x0F;      /* Pins above assigned to Ether */
    PORT8.PDR.BYTE = 0x07;      /* P80-2 are output             */
    
    /* Port C - Ethernet signals */
    MPC.PC0PFS.BYTE = 0x11;     /* PC0 - RXD3   input           */
    MPC.PC1PFS.BYTE = 0x11;     /* PC1 - RXD2   input           */
    MPC.PC2PFS.BYTE = 0x11;     /* PC2 - RXDV   input           */
    MPC.PC3PFS.BYTE = 0x11;     /* PC3 - TXER   output          */
    MPC.PC4PFS.BYTE = 0x11;     /* PC4 - TXCLK  input           */
    MPC.PC5PFS.BYTE = 0x11;     /* PC5 - TXD2   output          */
    MPC.PC6PFS.BYTE = 0x11;     /* PC6 - TXD3   output          */
    MPC.PC7PFS.BYTE = 0x11;     /* PC7 - COL    input           */
    PORTC.PMR.BYTE = 0xFF;      /* Pins above assigned to Ether */
    PORTC.PDR.BYTE = 0x68;      /* PC3, PC5-6 are output        */

#endif 
 
#if ETH_MODE_SEL == ETH_RMII_MODE

    /* Set PHY mode to RMII */
    MPC.PFENET.BIT.PHYMODE = 0;

    /* Port 7 - Ethernet signals */
    MPC.P71PFS.BYTE = 0x12;     /* P71 - MDIO   input/output    */
    MPC.P72PFS.BYTE = 0x12;     /* P72 - MDC    output          */
    MPC.P74PFS.BYTE = 0x12;     /* P74 - RXD1   input           */
    MPC.P75PFS.BYTE = 0x12;     /* P75 - RXD0   input           */
    MPC.P76PFS.BYTE = 0x12;     /* P76 - RXCLK  input           */
    MPC.P77PFS.BYTE = 0x12;     /* P77 - RXER   input           */
    PORT7.PMR.BYTE = 0xF6;      /* Pins above assigned to Ether */
    PORT7.PDR.BYTE = 0x06;      /* P71-2 are output             */

    /* Port 8 - Ethernet signals */
    MPC.P80PFS.BYTE = 0x12;     /* P80 - TXEN   output          */
    MPC.P81PFS.BYTE = 0x12;     /* P81 - TXD0   output          */
    MPC.P82PFS.BYTE = 0x12;     /* P82 - TXD1   output          */
    MPC.P83PFS.BYTE = 0x12;     /* P83 - CRS    input           */
    PORT8.PMR.BYTE = 0x0F;      /* Pins above assigned to Ether */
    PORT8.PDR.BYTE = 0x07;      /* P80-2 are output             */

#endif 

    /* Lock MPC registers */
    MPC.PWPR.BIT.PFSWE = 0;         
    /* Lock protection register */
    MPC.PWPR.BIT.B0WI = 1;
}

/******************************************************************************
* Function name: interrupts_configure
* Description  : Configures interrupts used
* Arguments    : none
* Return value : none
******************************************************************************/
static void interrupts_configure(void)
{
    /* Add code here to setup additional interrupts */
}

/******************************************************************************
* Function name: peripheral_modules_enable
* Description  : Enables and configures peripheral devices on the MCU
* Arguments    : none
* Return value : none
******************************************************************************/
static void peripheral_modules_enable(void)
{
    /* Add code here to enable peripherals used by the application */
    
    /* Protect off */
    SYSTEM.PRCR.WORD = 0xA50B;
    
    MSTP(EDMAC) = 0;            /* Power up ethernet block */
    MSTP(CMT0)  = 0;            /* Power up compare timer 0 */

    /* Protect on */
    SYSTEM.PRCR.WORD = 0xA500;

}



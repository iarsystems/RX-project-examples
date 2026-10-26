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
* File Name     : ether.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : Defines high-level ethernet functions used in this sample.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* System Includes
*******************************************************************************/
/* Defines standard integer types used in this file */
#include <stdint.h>
/* Defines standard boolean types used in this file */
#include <stdbool.h>

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines ethernet functions used in this file */
#include "r_ether.h"
/* Defines MCU registers in this file */
#include "iorx63n.h"
/* Declares prototypes for functions defined in this file */
#include "ether.h"
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines switch functions used in this file */
#include "switch.h"

/******************************************************************************
* Typedef definitions
******************************************************************************/
/* Declare start if section ETH_BUFF */
#pragma location = 0x00068B0

/* Declare structure to hold received ethernet frames */
typedef struct
{
    uint8_t frame[BUFSIZE];
    int32_t len;
    uint8_t wk[12];
} USER_BUFFER;
USER_BUFFER recv[10];

/* Declare end of section ETH_BUFF */

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Declare the ARP frame contents to send */
static uint8_t gARPFrame_Ether[] = 
{
    0xffu,0xffu,0xffu,0xffu,0xffu,0xffu,/* Destination MAC address             */
    0x00u,0x01u,0x02u,0x03u,0x04u,0x05u,/* Source MAC address:
                                           00:01:02:03:04:05                 */
    0x08u,0x06u,                        /* Type (ARP)                         */
    0x00u,0x01u,                        /* H/W type = Ethernet                 */
    0x08u,0x00u,                        /* Protocol type = IP                 */
    0x06u,0x04u,                        /* HW/protocol address length         */
    0x00u,0x01u,                        /* OPCODE = request                     */
    0x00u,0x01u,0x02u,0x03u,0x04u,0x05u,/* Source MAC address:
                                           00:01:02:03:04:05                 */
    0xc0u,0xa8u,0x00u,0x03u,            /* Source IP address: 192.168.0.3    */
    0x00u,0x00u,0x00u,0x00u,0x00u,0x00u,/* Inquiry MAC address                 */
    0xc0u,0xa8u,0x00u,0x05u,            /* Inquiry IP address: 192.168.0.5     */
};

/* Declare the RSK device MAC address */
static uint8_t gMACAddress_Ether[6u] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};

/* Switch press flag global variable (imported) */
extern volatile uint8_t gSwitchFlag;


/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* Switch callback function prototype */
void SwitchCB_Ether(void);

/*******************************************************************************
* Outline       : SwitchCB_Ether
* Description   : This callback function is executed when any of the user
*                 switches are pressed. If SW1 is pressed the RSK sends frames
*                 using the SendFrames_Ether function. If SW2 is pressed, the
*                 RSK prepares to receive frames.
* Argument      : none
* Return value  : none
*******************************************************************************/
void SwitchCB_Ether(void)
{
    /* Start switch case depending on switchflag */
    switch(gSwitchFlag & 0x0F)
    {
        /* Switch SW1 was pressed */
        case SWITCHHOLD_1:
        {
            /* Indicate frame sending has started on debug LCD */
            Display_LCD(LCD_LINE2, "TX START");
            
            /* Send 10 ARP test frames, check if operation was successful */
            if( SendFrames_Ether() )
            {
                /* Transmission successful, indicate this on debug LCD */
                Display_LCD(LCD_LINE2, "TX PASS ");
                
                /* Wait 2 seconds to allow user to read message */
                Delay_s(2u);
            }
            /* Frame sending unsuccessful */
            else
            {
                /* Transmission unsuccessful, indicate this on debug LCD */
                Display_LCD(LCD_LINE2, "TX FAIL ");
                
                /* Wait 2 seconds to allow user to read message */
                Delay_s(2u);
            }
        } break;
        
        /* Switch SW2 was pressed */
        case SWITCHHOLD_2:        
        {
            /* Indicate frame reception has started on debug LCD */
            Display_LCD(LCD_LINE2, "RX START");  
            
            /*Receive 10 ARP test frames, check if operation was successful */
            if( ReceiveFrames_Ether() )
            {
                /* Transmission successful, indicate this on debug LCD */
                Display_LCD(LCD_LINE2, "RX PASS ");
                
                /* Wait 2 seconds to allow user to read message */
                Delay_s(2u);
            }
            /* Frame sending unsuccessful */
            else
            {
                /* Transmission unsuccessful, indicate this on debug LCD */
                Display_LCD(LCD_LINE2, "RX FAIL ");
                
                /* Wait 2 seconds to allow user to read message */
                Delay_s(2u);
            }
        } break;
        
        /* Default branch */
        default: break;
    }
    
    /* Clear debug LCD second line */
    Display_LCD(LCD_LINE2, "        ");
}
/*******************************************************************************
* End of function SwitchCB_Ether
*******************************************************************************/

/*******************************************************************************
* Outline       : Init_Ether
* Description   : This function initialises the port pins used to perform
*                 ethernet operations. The function also configures the switch
*                 press callback function.
* Argument      : none
* Return value  : none
*******************************************************************************/
void Init_Ether(void)
{
    /* Disable protection */   
    SYSTEM.PRCR.WORD = 0xA502;
    
    /* Module standby clear */
    SYSTEM.MSTPCRB.BIT.MSTPB15 = 0;

    /* Enable protection */
    SYSTEM.PRCR.WORD = 0xA500;
    
       /*
    Pin Functions : Port
    --------------------
    ET_TX_CLK     : PC4
    ET_RX_CLK     : P76
    ET_TX_EN      : P80
    ET_ETXD3      : PC6
    ET_ETXD2      : PC5
    ET_ETXD1      : P82
    ET_ETXD0      : P81
    ET_TX_ER      : PC3
    ET_RX_DV      : PC2
    ET_ERXD3      : PC0
    ET_ERXD2      : PC1
    ET_ERXD1      : P74
    ET_ERXD0      : P75
    ET_RX_ER      : P77
    ET_CRS        : P83
    ET_COL        : PC7
    ET_MDC        : P72
    ET_MDIO       : P71
    ET_LINKSTA    : P54(no use)
    ET_EXOUT      : NC
    ET_WOL        : NC
    */

    /* Clear PDR and PMR */
    PORT7.PDR.BIT.B1 = 0;
    PORT7.PDR.BIT.B2 = 0;
    PORT7.PDR.BIT.B4 = 0;
    PORT7.PDR.BIT.B5 = 0;
    PORT7.PDR.BIT.B6 = 0;
    PORT7.PDR.BIT.B7 = 0;
    PORT8.PDR.BIT.B0 = 0;
    PORT8.PDR.BIT.B1 = 0;
    PORT8.PDR.BIT.B2 = 0;
    PORT8.PDR.BIT.B3 = 0;
    PORTC.PDR.BYTE = 0x00;
    PORT7.PMR.BIT.B1 = 0;
    PORT7.PMR.BIT.B2 = 0;
    PORT7.PMR.BIT.B4 = 0;
    PORT7.PMR.BIT.B5 = 0;
    PORT7.PMR.BIT.B6 = 0;
    PORT7.PMR.BIT.B7 = 0;
    PORT8.PMR.BIT.B0 = 0;
    PORT8.PMR.BIT.B1 = 0;
    PORT8.PMR.BIT.B2 = 0;
    PORT8.PMR.BIT.B3 = 0;
    PORTC.PMR.BYTE = 0x00;
    
    /* Disable PFS protection */
    MPC.PWPR.BYTE = 0x00;
    MPC.PWPR.BYTE = 0x40;        
    
    /* Select pin function */
    MPC.P71PFS.BYTE = 0x11;
    MPC.P72PFS.BYTE = 0x11;
    MPC.P74PFS.BYTE = 0x11;
    MPC.P75PFS.BYTE = 0x11;
    MPC.P76PFS.BYTE = 0x11;
    MPC.P77PFS.BYTE = 0x11;
    MPC.P80PFS.BYTE = 0x11;
    MPC.P81PFS.BYTE = 0x11;
    MPC.P82PFS.BYTE = 0x11;
    MPC.P83PFS.BYTE = 0x11;
    MPC.PC0PFS.BYTE = 0x11;
    MPC.PC1PFS.BYTE = 0x11;
    MPC.PC2PFS.BYTE = 0x11;
    MPC.PC3PFS.BYTE = 0x11;
    MPC.PC4PFS.BYTE = 0x11;
    MPC.PC5PFS.BYTE = 0x11;
    MPC.PC6PFS.BYTE = 0x11;
    MPC.PC7PFS.BYTE = 0x11;

    /* Enable PFS protection */
    MPC.PWPR.BYTE = 0x80;

    /* Set ethernet to MII mode */
    MPC.PFENET.BIT.PHYMODE = 1;

    /* Set the ethernet pin port mode */
    PORT7.PMR.BIT.B1 = 1;
    PORT7.PMR.BIT.B2 = 1;
    PORT7.PMR.BIT.B4 = 1;
    PORT7.PMR.BIT.B5 = 1;
    PORT7.PMR.BIT.B6 = 1;
    PORT7.PMR.BIT.B7 = 1;
    PORT8.PMR.BIT.B0 = 1;
    PORT8.PMR.BIT.B1 = 1;
    PORT8.PMR.BIT.B2 = 1;
    PORT8.PMR.BIT.B3 = 1;
    PORTC.PMR.BYTE = 0xFF;
    
    SetSwitchPressCallback(SwitchCB_Ether);
}
/*******************************************************************************
* End of function Init_Ether
*******************************************************************************/

/*******************************************************************************
* Outline       : SendFrames_Ether
* Description   : This function function iterates through and sends 10 test ARP
*                 frames.
* Argument      : none
* Return value  : bool  true:  Sending ARP frames successful
*                       false: Sending ARP frames unsucessful
*******************************************************************************/
bool SendFrames_Ether(void)
{
    /* Declare return value container variable */
    int32_t ret = 0;

    /* Open ethernet connection */
    ret = R_Ether_Open(0u, gMACAddress_Ether);
    
    /* Wait in for loop to open ethernet connection */
    for(uint32_t wait = 0; wait < ETH_OPEN_WAIT; wait++);

    /* Check the ethernet device was opened successfully */
    if( ret == R_ETHER_OK )
    {
        /* Iterate through and send 10 ARP frames */
        for(uint16_t i = 0; i < 10; i++)
        {
            /* Start frame transmission */
            ret = R_Ether_Write( 
                                  0,
                                  gARPFrame_Ether,
                                  sizeof(gARPFrame_Ether)
                                );
            
            /* Check if transmission failed */
            if( ret != R_ETHER_OK )
            {
                /* Break from function, and return false (failure) */
                return false;
            }
        }
    }
    
    /* Wait until transmission is complete */
    while(EDMAC.EDTRR.BIT.TR != 0);
    
    /* End ethernet reception & transmission */
    R_Ether_Close(0);
    
    /* Send operation successful, return true */
    return true;
}
/*******************************************************************************
* End of function SendFrames_Ether
*******************************************************************************/

/*******************************************************************************
* Outline       : ReceiveFrames_Ether
* Description   : This function polls for new frames until it receives 10 ARP
*                 frames (irrespective of IP address or MAC address). If the
*                 function does not receive 10 ARP frames before it times out,
*                 it will return false.
* Argument      : none
* Return value  : bool  true:  Receiving ARP frames successful
*                       false: Receiving ARP frames unsucessful
*******************************************************************************/
bool ReceiveFrames_Ether(void)
{
    /* Declare return value variable */
    int32_t ret = 0u;

    /* Ethernet initial setting */
    ret = R_Ether_Open(0, gMACAddress_Ether);
    
    /* Wait a short duration for ethernet channel to open */
    for(uint32_t wait = 0; wait < 0x1000000;wait++);

    /* Declare timeout value */
    uint32_t timeout = 0x007FFFFF;

    /* Check if R_Ether_Open was successful */
    if( ret == R_ETHER_OK )
    {
        /* Declare received frames counter */
        uint8_t rx_count = 0;
        
        /* Iterate through and check for ethernet ARP frames each time */
        while( (--timeout) && (rx_count < 10) )
        {
            /* Call the R_Ether_Read function to check for new packets, and
               store packet length (zero length indicates no new packet) */
            recv[rx_count].len = R_Ether_Read(0, recv[rx_count].frame);
            
            /* Check if the received packet is an ARP request (0x08, 0x06)*/
            if( (recv[rx_count].frame[12u] == 0x08u) && \
                (recv[rx_count].frame[13u] == 0x06u) )
            {
                /* ARP request received, increment the rx_counter variable */
                rx_count++;
            }
        }
    }
    
    /* Close ethernet connection */
    R_Ether_Close(0);
    
    /* Check if a timeout occured whilst receiving frames, or if the R_Ether_Open
       operation failed. */
    if( (timeout == 0) || (ret != R_ETHER_OK) )
    {
        /* Either timed out waiting for frames, or failed to open ethernet connection,
           return false */
        return false;
    }
    /* No failure detected */
    else
    {
        /* No failure detected, return true */
        return true;
    }
}
/*******************************************************************************
* End of function ReceiveFrames_Ether
*******************************************************************************/

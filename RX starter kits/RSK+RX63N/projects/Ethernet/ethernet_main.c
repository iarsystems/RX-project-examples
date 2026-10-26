/* Adapted for use with IAR Embedded Workbench */
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
* File Name     : main.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : This sample demonstrates use of the onboard Ethernet
*                 controller by sending and receiving ARP frames
* Operation     : 1. Compile and download the sample code to the target.
*
*                  2. Connect the target to an Ethernet network and use a network
*                     analysis tool (such as Wireshark) to view ARP frames on the
*                     network.
*
*                  3. Press SW1. TX START will appear on the debug LCD and 10 ARP
*                     frames will be broadcast onto the network. TX PASS will
*                     appear indicating successful transmission.
*
*                  4. Press SW2. The target will attempt to receive ARP frames,
*                     if frames §are received RX PASS will appear on the debug LCD,
*                     if after a few seconds no frames are received, RX FAIL will
*                     appear.
*
*         Note  : In order to generate ARP frames, set a machine on the network
*               the target is attached to with IP address 192.168.0.5 and this
*               machine will automatically generate response to the target's query.        
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines ethernet functions used in this file */
#include "ether.h"

/******************************************************************************
* Outline       : main
* Description   : The main program function. 
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{            
    /* Initialises the debug LCD */
    Init_LCD();
    
    /* Display sample name on the debug LCD */
    Display_LCD(LCD_LINE1, "Ethernet");
    
    /* Initialises the ethernet sample */
    Init_Ether();
    
    /* Infinite while loop */
    while(1);   
}
/******************************************************************************
* End of function main
******************************************************************************/

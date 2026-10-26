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
/*******************************************************************************
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    */
/*******************************************************************************
* File Name     : main.c
* Version       : 1.00
* Device        : R5F562GAADFP
* Tool Chain    : RX Family C Compiler 1.2.1.0
* H/W Platform  : RSKRX62G
* Description   : Empty application project.
*
* Description   : This sample demonstrates usage of the LIN master interface.
*                 The demonstration can be configured to be performed between
*                 an RSKRX62G (master) and another device capable of LIN slave
*                 operations (called open communication for this sample) or
*                 simply through a loopback mode.
*
*                 LOOPBACK MODE
*                 The loopback mode disconnects the LIN Tx and Rx pins from the
*                 LIN bus and internally connects the two pins together.
*
*                 OPEN COMMUNICATION MODE
*                 When using another device as the slave, ensure that it is compliant
*                 with LIN Specification Package Revisions 2.0, and 2.1. Although
*                 the RSKRX62G is also compliant with Revision 1.3, for the demonstration
*                 it is configured to only comply with Revisions 2.0 and 2.1. The slave
*                 ID should be set to 0x11. The RSKRX62G expects to receive 8 data bytes from
*                 the slave.
*
* Operation     :
*                 LOOPBACK MODE
*                 1. The transmission method used is frame separate mode.
*                    This mode allows for transmission of the LIN header which
*                    a slave device responds to with data bytes. Ensure the following
*                    definition is not commented in the header file lin_master.h:
*                    #define LIN_LOOPBACK_MODE
*
*                 2. Build this application and download it to the RX62G board.
*
*                 3. Press "Reset Go" icon available on debug tool bar.
*
*                 4. Pressing SW3 will carry out a 10-bit A/D conversion whose
*                    value is inverted and internally transmitted from the Tx
*                    pin to the Rx pin. The displayed value will be displayed
*                    as follows:
*                        =H'xxxx
*                    where xxxx denotes the 10-bit A/D conversion value.
*                    The A/D value will be shown on the on the debug LCD.
*
*                 5. Adjust the potentiometer and press SW3 to view the A/D
*                    value corresponding to the potentiometer's new position.
*
*                 6. Repeat step 5.
*                    The received data can be watched in the variable gRx_Buffer.
*                    Right click the variable, and select 'Instant Watch' and
*                    click Add on the following dialog.  Right click in the
*                    watch window and select Enable Auto-Update.
*
*                 OPEN COMMUNICATION MODE
*                 1. The transmission method used is frame separate mode.
*                    This mode allows for transmission of the LIN header which
*                    the slave device responds to with data bytes. When
*                    using a slave device, configure it to transmit 8 data bytes
*                    upon reception of a LIN header. Comment the instruction:
*                    #define LIN_LOOPBACK_MODE
*                    in the header file lin_master.h.
*
*                 2. Make the following connections between master RSKRX62G board
*                    and slave device.
*                    ----------------------------------------------------
*                    RSKRX62G   MASTER                  SLAVE device
*                    ----------------------------------------------------
*                    a. LIN pin no.1 (VBAT)     -->     LIN pin no.1 (VBAT)
*                    b. LIN pin no.2 (LIN)      -->     LIN pin no.2 (LIN)
*                    c. LIN pin no.3 (GROUND)   -->     LIN pin no.3 (GROUND)
*
*                 3. Download LIN Master application to the RSKRX62G by selecting
*                    the 'Program Flash' option in the 'emulator setting'.
*
*                 4. Build this application and download it to the RX62G board.
*
*                 5. Press "Reset Go" icon available on debug tool bar.
*
*                 6. Reset the slave device.
*
*                 7. In slave operations, pressing switch SW3 will transmit a
*                    header to the slave. The slave data will be shown on the
*                    on the debug LCD.
*
*                 8. Press SW3 to transmit a header and receive another 8 bytes
*                    of data.
*
*                 9. Repeat step 8.
*                    The received data can be watched in the variable
*                    gRx_Buffer. Right click the variable, and select
*                    'Instant Watch' and click Add on the following
*                    dialog.  Right click in the watch window and select
*                    Enable Auto-Update.
*
* NOTE          : 1. Usually there is no need to connect power supply to the
*                    LIN slave device as it will draw power from RSKRX62G master
*                    board via the LIN connector.
******************************************************************************/
/******************************************************************************
* History       : 06.09.2012 Ver. 1.00 First Release
******************************************************************************/

/******************************************************************************
* Project Includes
******************************************************************************/
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* CGC RPDL function definitions */
#include "r_pdl_cgc.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Provides user-modifiable definitions */
#include "r_pdl_user_definitions.h"
/* LCD function definitions */
#include "lcd.h"
/* LIN function definitions */
#include "lin_master.h"

#include "hwsetup.h"
#include "intrinsics.h"

/******************************************************************************
* Outline       : main
* Description   : The main program function intialises the debug LCD display
*                 and informs the user of the LIN mode to be performed. The
*                 LIN module is initialised and ready for LIN operations.
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{
    /* Do the hardware setup */
    HardwareSetup();

    /* Enable interrupts */
    __enable_interrupt();

    /* Initialise the debug LCD */
    InitialiseLCD();

#ifndef LIN_LOOPBACK_MODE
    /* Display instructions on the debug LCD */
    DisplayLCD(LCD_LINE1, "LIN Mstr");
#else
    /* Display instructions on the debug LCD */
    DisplayLCD(LCD_LINE1, "LIN LOOP");
#endif
    DisplayLCD(LCD_LINE2, "Push SW3");

    /* Initialise the ADC unit */
    Init_LIN();

    /* Infinite while loop */
    while(1);
}
/******************************************************************************
* End of function main
******************************************************************************/

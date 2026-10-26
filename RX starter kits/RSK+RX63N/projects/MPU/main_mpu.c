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
* File Name     : main_mpu.c
* Version       : 1.00
* Device        : R5F563NB
* Tool-Chain    : Renesas RX Standard 1.2.0.0
* H/W Platform  : RSK+RX63N
* Description   : This sample demonstrates the MPU unit by writing to a
*                 memory area which can be protected and unprotected.
* Operation     : 1. Compile and download the sample code to the target. 
*                    Click the 'Resume' icon on the Renesas Debug toolbar to
*                    run to the beginning of the main() function, and click
*                    'Resume' again to run the software.
*
*                 2. Open the Memory tab (Window->Show View->Memory).
*                    Right click and select 'Add Memory Monitor'. Set the address
*                    to 0x00003000. Enable the 'Real-time Refresh' option in
*                    the toolbar.
*
*                  3. Press switch SW1, and observe the contents of the memory
*                    viewer change. The debug LCD will also indicate that a
*                    successful write occured, and give the address of the
*                    data write.
*
*                 4. Repress SW1 to perform successive writes. The write
*                    location will increment each time.
*
*                  5. Press switch SW2 to enable protection on the memory block
*                    area, the debug LCD will indicate that this has been 
*                    enabled by showing a message briefly.
*
*                  6. Press switch SW1 again to attempt another write. This
*                    memory location is now protected so the write will fail.
*                    This can be observed as no change occurs in the memory
*                    viewer. The debug LCD will indicate a access violation
*                    has occured, then show the address of the attempted
*                    data write. 
*
*                 7. Click 'Reset Go' to repeat the sample.
* Note          : Memory protection is only valid when the MCU is in user mode.
*                 When processing an interrupt, the MCU is in supervisor mode
*                 the MPU unit has no bearing on its access rights.
*******************************************************************************/
/*******************************************************************************
* History       : 13 Aug. 2012  Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* User Includes (Project Level Includes)
*******************************************************************************/
/* Defines LCD functions used in this file */
#include "lcd.h"
/* Defines MPU functions used in this file */
#include "mpu.h"

/******************************************************************************
* Outline       : main
* Description   : The main program function. This function initialises the
*                 debug LCD, then displays the sample name and instructions. The
*                 function then calls the Init_MPU function to initialise the
*                 MPU unit, then waits in an infinite while loop and repeatedly
*                 calls the SwitchPoll_MPU function which polls the user
*                 switches.
* Argument      : none
* Return value  : none
******************************************************************************/
void main(void)
{          
    /* Initialise the debug LCD */
    Init_LCD();
    
    /* Display sample name and instructions on the debug LCD */
    Display_LCD(LCD_LINE1, " MPU    ");
    Display_LCD(LCD_LINE2, "Push SW ");
    
    /* Initialise the MPU unit */
    Init_MPU();
    
    /* Infinite while loop */
    while(1)
    {
        /* Poll the user switches */
        SwitchPoll_MPU();
    }
}
/******************************************************************************
* End of function main
******************************************************************************/

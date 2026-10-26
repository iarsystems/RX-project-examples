/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : main.c
* Version      : 1.00
* Device(s)    : R5F56609HxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX660
* Description  : This file implements main function.
* Creation Date: 2022-01-18
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

#include "r_smc_entry.h"

#include "r_okaya_lcd.h"
#include "r_rsk_async.h"
#include "rskrx660def.h"

#include "Config_SCI10.h"
#include "Config_CMT2.h"

#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

void main (void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    /* Initialize the debug LCD */
    R_LCD_Init();

    /* Displays the application name on the debug LCD.
       Casting for use as characters. */
    R_LCD_Display(0, (uint8_t *)" Asynchronous ");

    /* Casting for use as characters. */
    R_LCD_Display(1, (uint8_t *)" Serial ");

    /* Casting for use as characters. */
    R_LCD_Display(2, (uint8_t *)" Comms ");

    /* Enable uart reception and transmission */
    R_ASYNC_Init();

    /* Start CMT2 timer to schedule RS232 transmissions via interrupt */
    R_Config_CMT2_Start();

    /* Turn on LED0 to indicate serial transmission active. */
    LED0 = LED_ON;

    /* Turn off LED1. */
    LED1 = LED_OFF;

    while (1U)
    {
        /* Declare and initialize a variable used for counting from 0-9 */
        static uint8_t s_count = 0;

        /* Declare serial data output buffer */
        static uint8_t s_serial_data_out[] = "0123456789\r\n";

        /* Check the received character */
        if (TRUE == g_tx_flag)
        {
            g_tx_flag = FALSE;

            /* Transmit the data */
            R_SCI10_AsyncTransmit(&s_serial_data_out[s_count], 1);
            s_count += 1;

            /* Reset the s_count after transmitting all data in the buffer */
            if ((sizeof(s_serial_data_out)) == s_count)
            {
                /* Reset the s_count to 0 */
                s_count = 0;
            }
        }
    }
}
/***********************************************************************************************************************
* End of function main
***********************************************************************************************************************/

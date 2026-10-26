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
* File Name    : commands.c
* Version      : 1.0
* Device(s)    : R5F51115AxFM
* Tool-Chain   : CCRX
* Description  : C file for command processing module.
* Creation Date: 25/07/2013
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_lpc.h"
#include "r_cg_rtc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "lowlvl_debug_console.h"
#include "bootflags.h"

/***********************************************************************************************************************
* Function Name: send_resp
* Description  : Send command response sequence according to the passed parameters.
* Arguments    : uint8_t: cmd : command byte, needs to be in 0th position in response
*                uint8_t * rsp: pointer to array of bytes containing desired response
*                uint8_t rsp_siz: number of bytes in desired response
*                uint8_t buf_siz
*
* Return Value : None
***********************************************************************************************************************/
static void send_resp (const uint8_t cmd, const uint8_t * const rsp,  const uint8_t rsp_siz)
{
    /* send the command byte and the first CR,LF */
    charput(cmd);
    charput(CR);
    charput(LF);

    /* If there are response bytes, copy them in too plus the terminating CR,LF */
    if (0 != rsp_siz)
    {
        for (int i = 0; i < rsp_siz; i++)
        {
            charput(rsp[i]);
        }
        charput(CR);
        charput(LF);
    }

}
/*******************************************************************************
* End of function send_resp
*******************************************************************************/

/***********************************************************************************************************************
* Function Name: translate_mode
* Description  : Converts operating mode specified in API to enumerated type used by Applilet.
* Arguments    : uint8_t: api_par : API parameter for operating mode
* Return Value : enumerated value used by Appliet
***********************************************************************************************************************/
static operating_mode_t translate_mode (const uint8_t api_par)
{
    operating_mode_t ret_mode;

    switch (api_par)
    {
    case OPMODE_HIGH_SPEED:
        ret_mode = HIGH_SPEED;
        break;

    case OPMODE_MED_SPEED:
        ret_mode = MIDDLE_SPEED;
        break;

    case OPMODE_LOW_SPEED:
        ret_mode = LOW_SPEED;
        break;

    default:
        ret_mode = LOW_SPEED;
        break;
    }

    return ret_mode;

}
/*******************************************************************************
* End of function send_resp
*******************************************************************************/

/***********************************************************************************************************************
* Function Name: cmd_process
* Description  : This function waits for commands over the debug console and acts on them.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void cmd_process (void)
{
    uint8_t                     cmd;
    operating_mode_t            new_mode;
    uint8_t                     rsp_buffer[12];
    rtc_calendarcounter_value_t rtc;

    /* retrieve first command byte */
    if (1 == charget(&cmd))
    {
        switch (cmd)
        {
        case CMD_DISCONNECT:
            send_resp(cmd, NULL, 0);
            break;

        case CMD_GET_TIME:
            R_RTC_Get_CalendarCounterValue(&rtc);
            rsp_buffer[0] = rtc.rseccnt;
            rsp_buffer[1] = rtc.rmincnt;
            rsp_buffer[2] = rtc.rhrcnt;
            rsp_buffer[3] = rtc.rdaycnt;
            rsp_buffer[4] = rtc.rwkcnt;
            rsp_buffer[5] = rtc.rmoncnt;
            rsp_buffer[6] = (uint8_t)rtc.ryrcnt;
            rsp_buffer[7] = 0;
            send_resp(cmd, rsp_buffer, 8);
            break;

        case CMD_SET_TIME:
            rtc.rseccnt = charwait();
            rtc.rmincnt = charwait();;
            rtc.rhrcnt = charwait();
            rtc.rdaycnt = charwait();
            rtc.rwkcnt = charwait();
            rtc.rmoncnt = charwait();
            rtc.ryrcnt = (uint16_t)charwait();
            R_RTC_Set_CalendarCounterValue(rtc);
            R_RTC_Start();
            send_resp(cmd, NULL, 0);
            break;

        case CMD_SET_OP_MODE:
            new_mode = translate_mode(charwait());

            /* Check for LOW_SPEED as this will require a reset first to disable JTAG */
            if (LOW_SPEED == new_mode)
            {
                set_boot_flag(BOOT_SUBCLK);
            }
            else if (MIDDLE_SPEED == new_mode)
            {
                set_boot_flag(BOOT_MIDCLK);
                Select_Operating_Mode(new_mode);
            }
            else
            {
                set_boot_flag(BOOT_UNDEFINED);
                Select_Operating_Mode(new_mode);
            }
            send_resp(cmd, NULL, 0);
            break;

        case CMD_GET_VERSION:
            rsp_buffer[0] = '1';
            rsp_buffer[1] = '.';
            rsp_buffer[2] = '9';
            send_resp(cmd, rsp_buffer, 3);
            break;

        case CMD_GET_USB_STATE:
            /* Time out - not supported */
            break;

        default:
            /* Just echo */
            send_resp(cmd, NULL, 0);
            break;

        }
    }


}
/*******************************************************************************
* End of function cmd_process
*******************************************************************************/


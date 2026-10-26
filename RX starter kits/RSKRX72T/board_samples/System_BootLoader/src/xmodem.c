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
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : xmodem.c
* Version      : 1.00
* Device(s)    : R5F572TKCxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX72T
* Description  : Simple x-modem file transfer
*                This implementation is entirely based on 'XMODEM PROTOCOL OVERVIEW', 1/1/82, Ward Christensen.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.11.2018 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
#include <string.h>
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_SCI11.h"
#include "Config_TMR0_TMR1.h"
#include "xmodem.h"
#include "r_rsk_utility.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables (to be accessed by other files)
***********************************************************************************************************************/
uint8_t g_xmodem_frame[XMODEM_FRAME_SIZE]; /* Global buffer that contains a single XModem frame */
uint8_t g_xmodem_frame_valid = FALSE; /* Flag to indicate an XModem frame has been received */
uint8_t g_xmodem_status = FALSE; /* Flag to show an x-modem frame is currently being received */
uint8_t g_xmodem_ack_pending = FALSE; /* Flag to show the Host is awaiting an ACK */

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: xmodem_frame_rx
* Description  : This function is called at the end of a transfer when no more data is expected.
*                It sends an ack (if one is pending), then calls the frame RX, this will handle the EOT
*                XModem end of text handshaking
*
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t xmodem_frame_rx (void)
{
    uint8_t rtn;

    xmodem_send_ack(); /* If An ACK was pending from a previous frame, send it here */
    rtn = xmodem_rx_frame(g_xmodem_frame); /* Receive the frame */

    return (rtn);
}

/***********************************************************************************************************************
* End of function xmodem_frame_rx
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: xmodem_send_nak
* Description  : Sends the NAK character to the host.
*
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void xmodem_send_nak (void)
{
    uint8_t send_char = XMODEM_NAK;

    R_SCI11_SerialTxChar (send_char);
}

/***********************************************************************************************************************
* End of function xmodem_send_nak
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: xmodem_send_ack
* Description  : Sends the ACK character to the host, if one is pending.
*               This function can be called from multiple places in the code to trigger the next frame,
*               so it checks the 'g_xmodem_ack_pending' and only sends the character on to the line
*               if it is expected.
*
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void xmodem_send_ack (void)
{
    uint8_t send_char = XMODEM_ACK;

    /* Check to see if there is an ACK pending back to the host */
    if (TRUE == g_xmodem_ack_pending)
    {
        R_SCI11_SerialTxChar (send_char);
        g_xmodem_ack_pending = FALSE; /* ACK sent so invalidate the flag */
    }

}

/***********************************************************************************************************************
* End of function xmodem_send_ack
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: xmodem_check_frame
* Description  : Checks an Xmodem for SOH and checksum.
*
* Arguments    : uint8_t* pointer to the start of the buffer to check
* Return Value : TRUE - Frame successfully verified.
*               FALSE - Error in Frame format
***********************************************************************************************************************/
uint8_t xmodem_check_frame (uint8_t* const p_xmodem_buff)
{
    uint8_t head_byte = *p_xmodem_buff;
    uint8_t frame_csum = *(p_xmodem_buff + 131);
    uint8_t csum_calc = 0;
    uint8_t counter;
    uint8_t ret = FALSE;

    if (XMODEM_SOH == head_byte)
    {
        /* Sum all the data bytes for a local checksum calculation */
        for (counter = 3; counter < 131; counter++)
        {
            csum_calc += (*(p_xmodem_buff + counter));
        }

        /* Compare the locally generated checksum with the frame checksum */
        if (csum_calc == frame_csum)
        {
            /* If they match the frame is validated. */
            ret = TRUE;

            /* Indicate the host is now expecting this valid packet to be acknowledged */
            g_xmodem_ack_pending = TRUE;

            /* Note: the protocol supports a further 1's compliment checksum.
             * It was felt it was not needed to check this or the frame number
             * due to the robust transfer medium (USB to serial). */
        }
    }

    return (ret);
}

/***********************************************************************************************************************
* End of function xmodem_check_frame
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: xmodem_rx_frame
* Description  : Receives an Xmodem frame or aborts if a time-out occurs.
*
* Arguments    : uint8_t* pointer to the start of the buffer in to which the frame is placed
* Return Value : TRUE - Frame successfully received,
*               FALSE - Frame timeout hit before all bytes received
***********************************************************************************************************************/
uint8_t xmodem_rx_frame (uint8_t* p_xmodem_buff)
{
    uint16_t byte_counter = 0;
    uint8_t retry_counter = XMODEM_TIMEOUT;
    uint8_t by_return = TRUE;
    uint8_t end_of_text = FALSE; /* Used to indicate the X-Modem protocol signalled End OF Text */
    uint8_t rx_char;
    uint8_t* p_plocal_buff;

    p_plocal_buff = p_xmodem_buff; /* Set the local buffer pointer to the passed pointer */

    r_tmr_resetstart_1s_counter(); /* Reset the 1s count - used for the serial RX timeout */

    xmodem_send_ack(); /* If An ACK was pending from a previous frame, send it here */
    while (((byte_counter < XMODEM_FRAME_SIZE) && (retry_counter > 1)) && (FALSE == end_of_text))
    {
        for (byte_counter = 0;
                ((byte_counter < XMODEM_FRAME_SIZE) && (XMODEM_TIMEOUT > r_tmr_get_1s_counter()))
                        && (FALSE == end_of_text);)
        {

            blink_led0();       /* Blink LED during X-Modem cycles */

            /* Check to see if a serial byte has been received */
            if (R_SCI11_SerialRxCheck())
            {

                R_SCI11_SerialRxGetChar(&rx_char); /* Place the byte in to the local var */

                /* Check to see there is an EOT condition */
                if ((0 == byte_counter) && (XMODEM_EOT == rx_char))
                {
                    /* EOT Condition */
                    end_of_text = TRUE;

                    /* Indicate the host is now expecting an ack for the EOT */
                    g_xmodem_ack_pending = TRUE;

                    xmodem_send_ack();
                }
                else
                {
                    /* Normal frame transfer */
                    (*p_plocal_buff++) = rx_char; /* Place the byte in to the receive buffer */
                    r_tmr_resetstart_1s_counter(); /* Reset the timeout counter */
                    byte_counter++; /* Increment the byte counter */
                }
            }
        }

        /* Check for a time out condition */
        if (r_tmr_get_1s_counter() > byte_counter)
        {
            /* There was some sort of time out condition */
            by_return = FALSE;
        }
        else
        {
            /* There was no time out condition, so validate the checksum */
            by_return = xmodem_check_frame(p_xmodem_buff); /* Check the frame format and checksum */
        }

        /* If any of the checks above results in a return of FALSE, there is an error
         * so retry the frame */
        if (FALSE == by_return)
        {
            p_plocal_buff = p_xmodem_buff; /* Set the local buffer pointer to the passed pointer */
            r_tmr_resetstart_1s_counter(); /* Reset the 1s count - used for the serial RX timeout */
            retry_counter--; /* Decrement the retry counter */
            byte_counter = 0; /* Reset the loop counter to the beginning */
            xmodem_send_nak(); /* Inform the host we didn't get the frame */
        }
    }

    return (by_return);
}

/***********************************************************************************************************************
* End of function xmodem_rx_frame
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: xmodem_cancel
* Description  : Cancels an XModem transfer in progress
*
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void xmodem_cancel (void)
{
    char abort_string[3];

    abort_string[0] = XMODEM_CAN;
    abort_string[1] = XMODEM_CAN;
    abort_string[2] = 0x00;

    /* Send a double CAN - some implementations require just one, some need two */
    /* and some do not even support cancel. */
    R_SCI11_SerialTxString(abort_string);

}

/***********************************************************************************************************************
* End of function xmodem_cancel
***********************************************************************************************************************/


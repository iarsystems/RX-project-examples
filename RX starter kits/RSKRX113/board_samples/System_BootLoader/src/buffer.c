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
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * File Name    : buffer.c
 * Version      : 1.00
 * Device(s)    : R5F51138AxFP
 * Tool-Chain   : IAR Systems ICCRX
 * Description  : Code to operate circular input buffer
 * Creation Date: 17/02/2014
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Pragma directive
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/
#include <string.h>
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "r_cg_sci.h"
#include "buffer.h"
#include "xmodem.h"

/***********************************************************************************************************************
 Global variables and functions
 ***********************************************************************************************************************/
#define BUFFER_SIZE                 (1542)

/* Buffer utilisation where X-Modem protocol is used to pause incoming data */
#define BUFFER_HIGH_TIDE (1286U)

/* Buffer utilisation where X-Modem protocol is used to resume incoming data */
#define BUFFER_LOW_TIDE  (1158u)

extern uint8_t g_xmodem_frame[]; /* Global buffer that contains a single XModem frame */

uint8_t g_rx_buffer[BUFFER_SIZE];
uint8_t * g_prx_buf_head = g_rx_buffer;
uint8_t * g_prx_buf_tail = g_rx_buffer;
uint16_t g_buf_usage = 0;
uint8_t g_buf_on;
uint8_t g_byte_rxd = FALSE;

/* function pointer for testing if a byte has been received from data source */
uint8_t (*testByteReceived) (void) = NULL;

/* function pointer used to get received byte from data source */
void (*getByte) (uint8_t *) = NULL;

/* function pointer used to send byte to data source */
void (*sendByte) (uint8_t) = NULL;

/***********************************************************************************************************************
 * Function Name: init_buffer
 * Description  : This function initialises the circular buffer, head/tail pointers and the function pointers
 *                which point to the data source to receive the byte into the buffer
 * Arguments    : None
 * Return Value : none
 ***********************************************************************************************************************/
void init_buffer (void)
{

    /* clear buffer */
    memset(g_rx_buffer, '\0', BUFFER_SIZE);

    /* reset buffer head and tail pointers */
    g_prx_buf_tail = g_rx_buffer;
    g_prx_buf_head = g_rx_buffer;

    /* reset buffer capacity / control variables */
    g_buf_usage = 0;
    g_buf_on = TRUE;

    /* set function pointers to appropriate data source */
    /* change these to an appropriate function to change data source */

    /* testByteReceived expects the pointed to function to
     * return TRUE or FALSE if a data byte is available or not */
    testByteReceived = R_SCI1_SerialRxCheck;

    /* getByte places the received byte at the uint8_t pointer parameter */
    getByte = R_SCI1_SerialRxGetChar;

    /* sendByte expects pointed to function to be used to send single byte control
     * signals. Would need further modification for non-UART sources or hardware flow control */
    sendByte = R_SCI1_SerialTxChar;
}

/***********************************************************************************************************************
 * End of function init_buffer
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: get_buffer_char
 * Description  : returns the data from the current position in the buffer
 * Arguments    : None
 * Return Value : uint8_t data from current buffer position
 ***********************************************************************************************************************/
uint8_t get_buffer_char (void)
{
    return (*g_prx_buf_tail);
}

/***********************************************************************************************************************
 * End of function get_buffer_char
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: next_buffer_char
 * Description  : This function updates the last-read (or tail) pointer in the buffer assuming unread data exists.
 *                Also provides for buffer capacity management.
 * Arguments    : None
 * Return Value : none
 ***********************************************************************************************************************/
void next_buffer_char (void)
{

    /* if the buffer has unread data in it, update the tail pointer */
    if (g_prx_buf_tail != g_prx_buf_head)
    {
        g_prx_buf_tail++;
        if (g_prx_buf_tail >= ((uint8_t *) (g_rx_buffer + BUFFER_SIZE)))
        {

            /* hit end of buffer, move tail pointer to start of buffer */
            g_prx_buf_tail = g_rx_buffer;
        }

        /* decrement measure of how full the buffer is */
        g_buf_usage--;

        /* if buffer has been full, and now reaches the low tide value,
         * arrange for ACK signal to be sent to re-enable
         * reception from data source */
        if ((g_buf_usage < BUFFER_LOW_TIDE) && (FALSE == g_buf_on))
        {
            /* OK to continue transmission */
        }
    }
}

/***********************************************************************************************************************
 * End of function next_buffer_char
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: poll_buffer_for_new_data
 * Description  : This function tests for a frame received and updates buffer head pointer if so
 *                Also pauses data reception if buffer is becoming too full.
 * Arguments    : None
 * Return Value : TRUE  - byte received
 *                FALSE - no byte received
 ***********************************************************************************************************************/
uint8_t poll_buffer_for_new_data (void)
{
    uint16_t counter;
    uint8_t ret = FALSE;

    /* If the head and tail are not equal then the buffer contains unprocessed data */
    if (g_prx_buf_head != g_prx_buf_tail)
    {
        ret = TRUE;
    }
    else
    {

        /* check if X-Modem frame has been received */
        if (xmodem_frame_rx()) /* check if X-Modem frame has been received */
        {
            /* Copy the data from the X-Modem frame in to the Main Buffer */
            /* NB: it is critical that the buffer size and high time levels
             * are set such that there is always space for a full frame to be copied in
             * or data will start to get overwritten.
             */
            for (counter = 0; counter < (XMODEM_FRAME_SIZE - 4); counter++)
            {
                (*g_prx_buf_head) = (g_xmodem_frame[counter + 3]); /* Copy a byte from the XModem buffer to the main buffer
                 +3 offset points to data after Header, Checksum & inverted checksum */

                g_prx_buf_head++; /* Increment the pointer to the main buffer */

                /* test for overflow of buffer */
                if (g_prx_buf_head >= ((uint8_t *) (g_rx_buffer + BUFFER_SIZE)))
                {

                    /* hit end of buffer, move head pointer to start of buffer */
                    g_prx_buf_head = g_rx_buffer;
                }

                /* increment measure of how full the buffer is */
                g_buf_usage += (XMODEM_FRAME_SIZE - 4);
            }

            /* if buffer is approaching full, hold off from sending ACK
             * to stall the data being sent from the host */
            if ((g_buf_usage > BUFFER_HIGH_TIDE) && (TRUE == g_buf_on))
            {

                /* Frame accepted but there is no spare capacity in the buffer to receive
                 * another frame. Hold of sending the ACK to allow time to process the
                 * buffer contents
                 */
            }
            else
            {
                /* Frame accepted and buffer has spare capacity */
                /* Nothing to do here with XModem - next receive will signal ACK to host */
            }
        }
    }

    /* If the head and tail are not equal then the buffer contains unprocessed data */
    if (g_prx_buf_head != g_prx_buf_tail)
    {
        ret = TRUE;
    }

    return (ret);
}

/***********************************************************************************************************************
 * End of function poll_buffer_for_new_data
 ***********************************************************************************************************************/


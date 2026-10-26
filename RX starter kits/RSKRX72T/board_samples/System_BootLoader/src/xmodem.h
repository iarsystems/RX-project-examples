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
* File Name    : xmodem.h
* Version      : 1.00
* Device(s)    : R5F572TKCxFB
* Tool-Chain   : CCRX
* H/W Platform : RSKRX72T
* Description  : Contains functions for cmodem functions
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 30.11.2018 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef XMODEM_H_
#define XMODEM_H_

#define XMODEM_FRAME_SIZE   (132U)      /* Full XModem frame size */
#define XMODEM_TIMEOUT      (10U)       /* Number of seconds for XModem timeout */
#define XMODEM_RETRY_COUNT  (10U)       /* Number of frame retries before an abort */

/* XModem protocol definitions */
#define XMODEM_SOH          (0x01)
#define XMODEM_EOT          (0x04)
#define XMODEM_ACK          (0x06)
#define XMODEM_NAK          (0x15)
#define XMODEM_CAN          (0x18)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
uint8_t xmodem_frame_rx (void);
void xmodem_send_nak (void);
void xmodem_send_ack (void);
uint8_t xmodem_check_frame (uint8_t* const p_xmodem_buff);
uint8_t xmodem_rx_frame (uint8_t* p_xmodem_buff);
void xmodem_cancel (void);

#endif /* XMODEM_H_ */


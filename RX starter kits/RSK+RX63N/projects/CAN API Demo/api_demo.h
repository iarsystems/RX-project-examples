/* Adapted for use with IAR Embedded Workbench */
/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : api_demo.h
* Version      : 1.00
* Description  : Virtual EEPROM implementation using MCU's data flash memory
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.12.2011 2.00    From RSK630 project.
***********************************************************************************************************************/

#ifndef _API_DEMO_H
#define _API_DEMO_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_can_api.h"
#include "iorx63n.h"
#include "led.h"
#include "config_r_can_rapi.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
/* Application error codes */
enum app_err_enum {    APP_NO_ERR       = 0x00,
                       APP_ERR_CAN_PERIPH   = 0x01,
             APP_ERR_CAN_INIT   = 0x02,
             APP_TABLE_ERR     = 0x04,
                       APP_STATE_ERR     = 0x08
             /* Add your application errors/alarms here. */
                  };

/* CAN channel numbers */          
enum CAN_channel_num {  CH_0 = 0,
            CH_1 = 1,
            CH_2 = 2
           };
            
/******************************************************************************
Macro definitions
******************************************************************************/
/* TEST CAN ID */
#define   TX_ID_DEFAULT     0x001
#define   RX_ID_DEFAULT     0x001
#define   REMOTE_TEST_ID    0x050

/* Mailboxes used for demo. Keep mailboxes 4 apart if you want masks 
independent - not affecting neighboring mailboxes. */
#define   CANBOX_TX        0x01  //Mailbox #1
#define   CANBOX_RX         0x04  //Mailbox #4
/* Added to show how to use multiple mailboxes. */
#define   CANBOX_REMOTE_RX  0x08  //Mailbox #8
#define   CANBOX_REMOTE_TX  0x0C  //Mailbox #12

/******************************************************************************
Global variables and functions imported (externs)
******************************************************************************/
/* Data */
extern uint32_t     CAN1_frame_received_flag;
extern uint32_t    CAN1_rx_remote_frame_flag;
/* Functions */
extern void Delay(uint32_t);

/******************************************************************************
Constant definitions
*****************************************************************************/
/******************************************************************************
Global variables and functions exported
******************************************************************************/
/* Data */
extern can_frame_t    tx_dataframe, rx_dataframe, remote_frame;
#if (USE_CAN_POLL == 0)
extern uint32_t        CAN1_tx_sentdata_flag;
extern uint32_t        CAN1_tx_err_sentdata_flag;
extern uint32_t        CAN1_rx_newdata_flag;
extern uint32_t        CAN1_rx_test_newdata_flag;
#endif //USE_CAN_POLL
extern enum app_err_enum  app_err_nr;

/* Functions */
uint32_t ResetAllErrors(void);
void api_demo(void);

#endif //API_DEMO_H
/* eof */

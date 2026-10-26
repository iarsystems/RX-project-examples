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
/*******************************************************************************
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : api_demo.h
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Includes user defines and function prototypes used in 
*            : api_demo.c file. 
******************************************************************************/
/******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Removed RPDL)
******************************************************************************/

#ifndef API_DEMO_H
#define API_DEMO_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_can_api.h"
#include "stdint.h"
#include <iorx62t.h>
#include "rskrx62tdef.h"

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
          

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* TEST CAN ID */
#define   TX_ID_DEFAULT     0x001
#define   RX_ID_DEFAULT     0x001
#define   REMOTE_TEST_ID    0x050

/* Mailboxes used for demo. Keep mailboxes 4 apart if you want masks 
independent - not affecting neighboring mailboxes. */
/* Mailbox #1 */
#define   CANBOX_TX        0x01  
/* Mailbox #4 */
#define   CANBOX_RX         0x04  
/* Added to show how to use multiple mailboxes. */
/* Mailbox #8 */
#define   CANBOX_REMOTE_RX  0x08  
/* Mailbox #12 */
#define   CANBOX_REMOTE_TX  0x0C  

/*******************************************************************************
Global variables and functions imported (externs)
*******************************************************************************/
/* Variable prototypes */
extern uint32_t     can0_frame_received_flag;
extern uint32_t    can0_rx_remote_frame_flag;

/* Functions prototypes */
extern void Delay(uint32_t);

/*******************************************************************************
Global variables and functions exported
*******************************************************************************/
/* Structure prototypes */
extern can_std_frame_t    tx_dataframe, rx_dataframe, remote_frame;

/* USE_CAN_POLL */
#ifndef USE_CAN_POLL
extern uint32_t        can0_tx_sentdata_flag;
extern uint32_t        can0_tx_err_sentdata_flag;
extern uint32_t        can0_rx_newdata_flag;
extern uint32_t        can0_rx_test_newdata_flag;
#endif 

extern enum app_err_enum  app_err_nr;

/* Function prototypes */
extern uint32_t ResetAllErrors(void);

/* API_DEMO_H */
#endif 


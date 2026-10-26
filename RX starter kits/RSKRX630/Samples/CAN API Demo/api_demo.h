/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER
* Please refer to http://www.renesas.com/disclaimer
******************************************************************************
  Copyright (C) 2010, 2011. Renesas Electronics Corp., All Rights Reserved.
*******************************************************************************
* File Name    : API_DEMO.H
* Version      : 1.00
* Description  : 
******************************************************************************
* History
* Mar 22 '10  RTA-CSS    For RX62N with new CAN API.
* June 09 2011 REA    Port to RSKRX630
******************************************************************************/

#ifndef API_DEMO_H
#define API_DEMO_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_can_api.h"
#include "stdint.h"
#include "iorx630.h"
#include "rskrx630def.h"
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
extern can_std_frame_t    tx_dataframe, rx_dataframe, remote_frame;
#ifndef USE_CAN_POLL
extern uint32_t        CAN1_tx_sentdata_flag;
extern uint32_t        CAN1_tx_err_sentdata_flag;
extern uint32_t        CAN1_rx_newdata_flag;
extern uint32_t        CAN1_rx_test_newdata_flag;
#endif //USE_CAN_POLL
extern enum app_err_enum  app_err_nr;
/* Functions */
extern uint32_t ResetAllErrors(void);

#endif //API_DEMO_H
/* eof */

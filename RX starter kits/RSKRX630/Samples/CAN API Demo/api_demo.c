/* Adapted for use with IAR Embedded Workbench */
/******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corp. and is only 
* intended for use with Renesas products. No other uses are authorized.

* This software is owned by Renesas Electronics Corp. and is protected under 
* all applicable laws, including copyright laws.

* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES 
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, 
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY 
* DISCLAIMED.

* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORP. NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES 
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS 
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

* Renesas reserves the right, without notice, to make changes to this 
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and 
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************
* Copyright (C) 2010,2011. Renesas Electronics Corp., All Rights Reserved.
******************************************************************************
* File Name    API_DEMO.C
* Version    1.02
* Tool-Chain  RX Standard Toolchain 1.1.0.0
* Platform    ROK505630C011BR
* Description  Demo application program

                CAN Baudrate: 500 kbps.

                Demonstration of CAN receive and transmit using the CAN API.
                
                The demo can be run in three ways:
                1) Program two boards and connect them together over the CAN 
                    bus.
                2) With CANPORT_TEST_1_INT_LOOPBACK used in the R_CAN_PortSet
                    API you can communicate internally, no external bus needed!
                3) Use a CAN bus monitor, e.g. SysTec's low-cost monitor 
                    3204000, to send and receive frames to/from the demo. 
                    Remote frames can also be demonstrated if CAN interrupts 
                    are enabled. See last paragraph below.

                OPERATION:
                The demo transmits and receives frames with CAN-ID 1 by 
                default. The default demo CAN-ID value is set in api_demo.h by 
                g_tx_id_default.
                
                The software starts up by immediately sending ten test frames. 
                This has two purposes, to check the link and to demonstrate 
                how messages are sent back-to-back as quickly as possible.
                
                Press SW1 to send one CAN frame. The frame will be received 
                and displayed by the other RSK as long as that board's receive
                ID (RxID) matches the sending boards transmit ID (TxID).
                
                Press SW2 to display the current demo TxID on the LCD. To inc-
                rement the TxID hold SW2 down and press SW3. The actual send 
                command is invoked by the Sw1Func function.

                Press SW3 to display current demo RxID on the LCD. To change 
                RxID hold SW3 down and press SW2.

                By default, polled CAN is used. To use the CAN interrupts for 
                faster processing, uncomment USE_CAN_POLL in file 
                config_r_can_rapi.h.

                REMOTE frames:
                Besides demonstrating Tx and Rx of Standard CAN frames, the 
                demo will also send remote frame responses for remote frame 
                requests received by the mailbox at CAN-ID 50h (defined by
                REMOTE_TEST_ID in api_demo.h)
                . Remote frames demo is only done in 
                interrupt mode; with USE_CAN_POLL commented in the CAN API 
                config file. Remote requests are not sent by this demo as it 
                is, and so must come from an outside source, e.g. the CAN 
                monitor mentioned above. The external CAN source must be set 
                to send remote frame requests to CAN-ID 50h.
                
* Limitations  -
******************************************************************************
* History
* Mar 22 '10  RTA-CSS    For RX62N with new CAN API on 62N Eval Board.
* Apr 15 '10  RTA-CSS    Improvements when using R_CAN_PortSet test modes.
* Jul 13 '10  RTA-CSS    For RDK.
* June 08 2011        For generic RX600 family RSK board
* Dec 10, 2011  REA          From RSK630 project.
*******************************************************************************/

/******************************************************************************
Includes  <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
#include "hwsetup.h"
#include "api_demo.h"
#include "r_can_api.h"
#include "config_r_can_rapi.h"
#include "lcd.h"
#include "stdint.h"
#include "switches.h"
#include "rskrx630def.h"

#include "intrinsics.h"

#include "hwsetup.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Macro definitions
******************************************************************************/
#define LED_DELAY        0x00080000
#define NR_STARTUP_TEST_FRAMES  10

#define MAX_CHANNELS 3  /* RX63x */

/******************************************************************************
Global variables and functions imported (externs)
******************************************************************************/
/* Data */
/* Functions */
/******************************************************************************
Constant definitions
*****************************************************************************/
/******************************************************************************
Global variables and functions exported
******************************************************************************/
/* Demo data */
can_std_frame_t    tx_dataframe, rx_dataframe, remote_frame;

/* the CAN peripheral channel used in this demo */
uint32_t g_can_channel;

/* Demo flags. */
#ifndef USE_CAN_POLL
uint32_t      CAN1_tx_sentdata_flag = 0;
uint32_t      CAN1_tx_remote_sentdata_flag = 0;
uint32_t      CAN1_rx_newdata_flag = 0;
uint32_t      CAN1_rx_test_newdata_flag = 0;
uint32_t      CAN1_rx_remote_frame_flag = 0;
#endif //USE_CAN_POLL
enum app_err_enum  app_err_nr;
/* Functions */

/******************************************************************************
Global variables and functions private to the file
******************************************************************************/
/* Data */
/* Errors. Peripheral and bus errors. Space for each channel. */
static uint32_t    error_bus_status[MAX_CHANNELS];
static uint32_t     error_bus_status_prev[MAX_CHANNELS];
static uint32_t    can_state[MAX_CHANNELS];
static uint32_t    nr_times_reached_busoff[MAX_CHANNELS];

/* TEST CAN ID */
uint32_t g_tx_id_default;
uint32_t g_rx_id_default;

/* Functions */
static uint32_t InitCanApp(void);
static void CheckCANerrors(void);
static void HandleCanBusState(uint8_t ch_nr);
static void can_channel_start(uint32_t ch_nr);
static void TestLEDs(uint32_t nr_led_flashes);

#ifdef USE_CAN_POLL
static void CanPollDemo(void);
#else 
static void CanIntDemo(void);
#endif //USE_CAN_POLL


/**********************************************************************************
User Program Code
***********************************************************************************/
/*****************************************************************************
Name    : main
Parameters : none
Returns   : nothing
Description: Main Can API demo program
*****************************************************************************/
void main(void)
{
  
    HardwareSetup();
  
    __enable_interrupt();
  
    uint32_t i, api_status = R_CAN_OK;
    
    g_can_channel = CH_1; /* using channel 1 for this demo */
        
    /* Setup LCD. */
    InitialiseDisplay();
    DisplayString(LCD_LINE1, CPU_NAME);
    DisplayString(LCD_LINE2, "TxRxDemo");

    
    /* Blink LEDs. */
    TestLEDs(20);

    /* Set starting mailbox IDs for the demo*/
    if (FRAME_ID_MODE == STD_ID_MODE)
    {
        g_tx_id_default = 0x001;
        g_rx_id_default = 0x001;    
    }
    else
    {
        g_tx_id_default = 0x000A0001;
        g_rx_id_default = 0x000A0001;    
    }
    
    /* RX 63x family has a Protect Register (PRCR) that locks changes to certain registers. */
    /* Unlock the Protection register and then take can module out of stop state */
    can_channel_start(g_can_channel); 
    
    /* Init CAN. */
    api_status = R_CAN_Create(g_can_channel);
    
    if (api_status != R_CAN_OK)
    {   /* An error at this stage is fatal to demo, so stop here. */
        DisplayString(LCD_LINE1,"API err");
        DisplayString(LCD_LINE2,"        ");
        LcdShow2DigHex(api_status, 1*16 + 4);
        
        while(1)
        {
            /* Wait here and leave error displayed. */
        } 
    }
                        
    /*********************************
    * Pick ONE R_CAN_PortSet call below!  *
    **********************************/
    /* Normal CAN bus usage. */
    R_CAN_PortSet(g_can_channel, ENABLE);
    /* Test modes. With Internal loopback mode you only need one board! */
//  R_CAN_PortSet(g_can_channel, CANPORT_TEST_1_INT_LOOPBACK);
//  R_CAN_PortSet(g_can_channel, CANPORT_TEST_0_EXT_LOOPBACK);
//  R_CAN_PortSet(g_can_channel, CANPORT_TEST_LISTEN_ONLY);
    
    /* Init CAN mailboxes. */
    api_status |= InitCanApp();
    
    /* Is all OK after all CAN initialization? */
    if (api_status != R_CAN_OK)
    {
        api_status = R_CAN_OK;
        app_err_nr = APP_ERR_CAN_INIT;
    }
    
    /* Interrupt Enable flag is set by default. */
    
    /* Send several test messages back to back. */
    if (FRAME_ID_MODE == STD_ID_MODE )
    {
        api_status |= R_CAN_TxSet(g_can_channel, CANBOX_TX, &tx_dataframe, DATA_FRAME);   
    }
    else
    {
        api_status |= R_CAN_TxSetXid(g_can_channel, CANBOX_TX, &tx_dataframe, DATA_FRAME);
    }    
 
    #ifdef USE_CAN_POLL
    while (R_CAN_TxCheck(g_can_channel, CANBOX_TX))
    {
    }

    for (i = 0; i < NR_STARTUP_TEST_FRAMES; i++)
    {
        api_status |= R_CAN_Tx(g_can_channel, CANBOX_TX);
        while (R_CAN_TxCheck(g_can_channel, CANBOX_TX))
        {
        }
    }
    #else  //Using CAN interrupts.
    while (CAN1_tx_sentdata_flag == 0)
    {
    }

    CAN1_tx_sentdata_flag = 0;
    
    for (i = 0; i < NR_STARTUP_TEST_FRAMES; i++)
    {
        api_status |= R_CAN_Tx(g_can_channel, CANBOX_TX);
        while (CAN1_tx_sentdata_flag == 0)
        {
        }

        CAN1_tx_sentdata_flag = 0;
    }
    #endif
    
    /*  M A I N  L O O P  * * * * * * * * * * * * * * * * * * * * * * * * * */  
    while(1)
    {
        /* User pressing switch(es)? */
        ReadSwitches();

        /* Check for CAN errors. */
         CheckCANerrors();
    
        if (can_state[0] != R_CAN_STATUS_BUSOFF)
        {
            #ifdef USE_CAN_POLL
            CanPollDemo();
            #else
            CanIntDemo();
            #endif //USE_CAN_POLL
        }
        else
        /* Bus Off. */
        {
            DisplayString(LCD_LINE1, "App in  ");
            DisplayString(LCD_LINE2, "Bus Off ");

            /* HandleCanBusState() will restart app. */
            LCD_FLASH
        }
        /* Reset receive/transmit indication. */
        LED2 = LED_OFF;
        LED3 = LED_OFF;
    }
}/* end main() */

#ifdef USE_CAN_POLL
/*****************************************************************************
Name:      CanPollDemo
Parameters:    -
Returns:    -
Description:  POLLED CAN demo version
*****************************************************************************/
static void CanPollDemo(void)
{
    uint32_t  api_status = R_CAN_OK;

    /*** TRANSMITTED any frames? */
    api_status = R_CAN_TxCheck(g_can_channel, CANBOX_TX);
    if (api_status == R_CAN_OK)
    {
        LED2 = LED_ON;
        DisplayString(LCD_LINE1, "TxChk OK");
        LcdShow4DigHex(tx_dataframe.data[0], tx_dataframe.data[1], 16);
        LcdShow4DigHex(tx_dataframe.data[2], tx_dataframe.data[3], 20); LCD_FLASH
    }
    /* Since we are always polling for transmits, api_status for R_CAN_TxCheck 
      will most often be other than R_CAN_OK, don't show that in this demo.*/

    /*** RECEIVED any frames? */
    api_status = R_CAN_RxPoll(g_can_channel, CANBOX_RX);
    if (api_status == R_CAN_OK)
    {
        LED1 = LED_ON;
        DisplayString(LCD_LINE1, "RxPollOK"); LCD_FLASH
        DisplayString(LCD_LINE1, "RxRead: ");

        /* Read CAN data and show. */
        api_status = R_CAN_RxRead(g_can_channel, CANBOX_RX, &rx_dataframe);
        LcdShow4DigHex(rx_dataframe.data[0], rx_dataframe.data[1], 16);
        LcdShow4DigHex(rx_dataframe.data[2], rx_dataframe.data[3], 20); LCD_FLASH
        if (api_status == R_CAN_MSGLOST)
        {
            DisplayString(LCD_LINE2, "MSGLOST"); LCD_FLASH
        }
        LED1 = LED_OFF;
    }
}/* end CanPollDemo() */

#else

/*****************************************************************************
Name:      CanIntDemo
Parameters:    -
Returns:    -
Description:  INTERRUPT driven CAN demo version
*****************************************************************************/
static void CanIntDemo(void)
{
    uint32_t  api_status = R_CAN_OK;

    /************************************************************************
    * Using CAN INTERRUPTS.                          *
    * See also r_can_api.c for the ISR example.                *
    *************************************************************************/
    /*** TRANSMITTED any frames? Only need to check if flag is set by CAN Tx ISR. */
    if (CAN1_tx_sentdata_flag)
    {
        CAN1_tx_sentdata_flag = 0;
        LED2 = LED_ON;
        /* Show CAN frame was sent. */
        DisplayString(LCD_LINE1, "Tx OK   ");
        LcdShow4DigHex(tx_dataframe.data[0], tx_dataframe.data[1], 16);
        LcdShow4DigHex(tx_dataframe.data[2], tx_dataframe.data[3], 20); LCD_FLASH
    }
    if (CAN1_tx_remote_sentdata_flag)
    {
        CAN1_tx_remote_sentdata_flag = 0;
        DisplayString(LCD_LINE1, "TxRemote"); LCD_FLASH
    }          

    /*** RECEIVED any frames? Only need to check if flag is set by CAN Rx ISR.
     Will only receive own frames in CAN port test modes 0 and 1. */
    if (CAN1_rx_newdata_flag)
    {
        CAN1_rx_newdata_flag = 0;
        LED0 = LED_ON;
        DisplayString(LCD_LINE1, "Rx OK   "); LCD_FLASH
        DisplayString(LCD_LINE1, "Rx Read:");

        /* Read CAN data and show. */
        api_status = R_CAN_RxRead(g_can_channel, CANBOX_RX, &rx_dataframe);
        LcdShow4DigHex(rx_dataframe.data[0], rx_dataframe.data[1], 16);
        LcdShow4DigHex(rx_dataframe.data[2], rx_dataframe.data[3], 20); LCD_FLASH
        if (api_status == R_CAN_MSGLOST)
        {
            //DisplayString(LCD_LINE2, "MSGLOST"); LCD_FLASH
        }
        LED0 = LED_OFF;
    }
    if (CAN1_rx_test_newdata_flag)
    {
        CAN1_rx_test_newdata_flag = 0;
        LED0 = LED_ON;
        DisplayString(LCD_LINE1, "Rx Test"); LCD_FLASH
    }
    /* Set up remote reply if remote request came in. */
    if (CAN1_rx_remote_frame_flag == 1)
    {
        CAN1_rx_remote_frame_flag = 0;
        remote_frame.data[0]++;
        
        if (FRAME_ID_MODE == STD_ID_MODE )
        {
            R_CAN_TxSet(g_can_channel, CANBOX_REMOTE_TX, &remote_frame, DATA_FRAME);   
        }
        else
        {
            R_CAN_TxSetXid(g_can_channel, CANBOX_REMOTE_TX, &remote_frame, DATA_FRAME);
        }           
    }
    LED0 = LED_OFF;
}/* end CanIntDemo() */
#endif //USE_CAN_POLL

/*****************************************************************************
Name:      InitCanApp
Parameters:    -
Returns:    -
Description:  Initialize CAN demo application
*****************************************************************************/
static uint32_t InitCanApp(void)
{  
    uint32_t  api_status = R_CAN_OK;
    uint8_t   i;

    can_state[0] = R_CAN_STATUS_ERROR_ACTIVE;
    
    for (i = 0; i < MAX_CHANNELS; i++) /* Initialize status for all channels. */
    {
        error_bus_status[i] = R_CAN_STATUS_ERROR_ACTIVE;
        error_bus_status_prev[i] = R_CAN_STATUS_ERROR_ACTIVE;            
    }
    
    /* Configure mailboxes in Halt mode. */
    api_status |= R_CAN_Control(g_can_channel, HALT_CANMODE);

    /********  Init demo to recieve data  ********/  
    /* Use API to set one CAN mailbox for demo receive. */
    /* Standard id. Choose value 0-0x07FF (2047). */
    if (FRAME_ID_MODE == STD_ID_MODE)
    { 
        api_status |= R_CAN_RxSet(g_can_channel, CANBOX_RX, g_rx_id_default, DATA_FRAME);
        
        /* Mask for receive box. Write to mask only in Halt mode. */
        /* 0x7FF = no mask. 0x7FD = mask bit 1, for example; If receive ID is set to 1, both
           ID 1 and 3 should be received. */
        R_CAN_RxSetMask( g_can_channel, CANBOX_RX, 0x7FF);        
    }
    else
    {
        api_status |= R_CAN_RxSetXid(g_can_channel, CANBOX_RX, g_rx_id_default, DATA_FRAME);
        
        /* Mask for receive box. Write to mask only in Halt mode. */
        /* 0x1FFFFFFF = no mask. 0x1FFFFFFD = mask bit 1, for example; If receive ID is set to 1, both
           ID 1 and 3 should be received. */
        R_CAN_RxSetMask( g_can_channel, CANBOX_RX, 0x1FFFFFFD);                
    }    
    
    /********  Init. demo Tx dataframe RAM structure  ********/  
    /* Standard id. Choose value 0-0x07FF (2047). */
    tx_dataframe.id        =  g_tx_id_default;
    tx_dataframe.dlc    =  8;
    tx_dataframe.data[0]  =  0x00;
    tx_dataframe.data[1]  =  0x11;
    tx_dataframe.data[2]  =  0x22;
    tx_dataframe.data[3]  =  0x33;
    tx_dataframe.data[4]  =  0x44;
    tx_dataframe.data[5]  =  0x55;
    tx_dataframe.data[6]  =  0x66;
    tx_dataframe.data[7]  =  0x77;


    /* API to send will be set up in SW1Func() in file switches.c. */

    api_status |= R_CAN_Control(g_can_channel, OPERATE_CANMODE);

    /*************** Init. remote dataframe response **********************/
    remote_frame.id = REMOTE_TEST_ID;

    /* Length is specified by the remote request. */

    /* Some data.. */
    for (i = 0; i < 8; i++)
    {
        remote_frame.data[i] = i;
    }

    /* Prepare mailbox for Tx. */
    if (FRAME_ID_MODE == STD_ID_MODE)
    {
        R_CAN_RxSet(g_can_channel, CANBOX_REMOTE_RX, REMOTE_TEST_ID, REMOTE_FRAME);
    }
    else
    {
        R_CAN_RxSetXid(g_can_channel, CANBOX_REMOTE_RX, REMOTE_TEST_ID, REMOTE_FRAME);        
    }  

    /***********************************************************************/

    /* Set frame buffer id so LCD shows correct receive ID from start. */
     rx_dataframe.id = g_rx_id_default;

    return api_status;

} /* end InitCanApp */

/*****************************************************************************
Name:      CheckCANerrors
Parameters:    -
Returns:    -
Description:  Check for all possible errors, in app and peripheral. Add 
                checking for your app here.
*****************************************************************************/
static void CheckCANerrors(void)
{
    /* Error passive or more? */
    HandleCanBusState(g_can_channel);

    if (app_err_nr)
    {
        /* Show error to user */
        /* RESET ERRORs with SW1. */
        LED3 = LED_ON;
        DisplayString(LCD_LINE1,"App err");
        LcdShow2DigHex(app_err_nr, 1*16 + 4);
        LCD_FLASH
        LED3 = LED_OFF;
    }
}/* end CheckCANerrors() */

/*****************************************************************************
Name:        HandleCanBusState
Parameters:      Bus number, 0 or 1.
Returns:      -
Description:    Check CAN peripheral bus state.
*****************************************************************************/
static void HandleCanBusState(uint8_t  ch_nr)
{
    can_std_frame_t err_tx_dataframe;

    /* Has the status register reached error passive or more? */
    if (ch_nr < MAX_CHANNELS)
    {
        error_bus_status[ch_nr] = R_CAN_CheckErr(ch_nr);
    }
    else
    {
        return;
    }

    /* Tell user if CAN bus status changed.
       All Status bits are read only. */
    if (error_bus_status[ch_nr] != error_bus_status_prev[ch_nr])
    {
        switch (error_bus_status[ch_nr])
        {
            /* Error Active. */
            case R_CAN_STATUS_ERROR_ACTIVE:
            /* Only report if there was a previous error. */
                if (error_bus_status_prev[ch_nr] > R_CAN_STATUS_ERROR_ACTIVE)
                {
                    if (ch_nr == 0)
                    {
                        DisplayString(LCD_LINE1, "Bus0: OK");
                    }
                    else
                    {
                        DisplayString(LCD_LINE1, "Bus1: OK");
                    }
                    /* Show user */
                    Delay(0x400000);
                }
                /* Restart if returned from Bus Off. */
                if (error_bus_status_prev[ch_nr] == R_CAN_STATUS_BUSOFF)
                {
                    /* Restart CAN */
                    if (R_CAN_Create(ch_nr) != R_CAN_OK)
          {
                        app_err_nr |= APP_ERR_CAN_PERIPH;
          }

                    /* Restart CAN demos even if only one channel failed. */
                    InitCanApp();
                }
                break;  

            /* Error Passive. */
            case R_CAN_STATUS_ERROR_PASSIVE:
            /* Continue into Busoff case to display. */

            /* Bus Off. */
            case R_CAN_STATUS_BUSOFF:
            default:
                //demo_state[ch_nr] = CAN_BUS_OFF;
                if (ch_nr == 0)
                {
                    DisplayString(LCD_LINE1,"bus0:  ");
                }
                else if (ch_nr == 1)
                {
                    DisplayString(LCD_LINE2,"bus1:  ");
                }
                else if (ch_nr == 2)
                {
                    DisplayString(LCD_LINE2,"bus2:  ");
                }                 
                else
                {
                }

                /* Show user */
                LcdShow2DigHex((uint8_t)error_bus_status[ch_nr], ch_nr*16 + 6);
                Delay(0x400000);
                nr_times_reached_busoff[ch_nr]++;
            break;
        }
        error_bus_status_prev[ch_nr] = error_bus_status[ch_nr];

        /* Transmit CAN bus status change */
        err_tx_dataframe.id = 0x700 + ch_nr;
        err_tx_dataframe.dlc =  1;
        err_tx_dataframe.data[0] = error_bus_status[ch_nr];

        /* Send Error state on both channels. Maybe at least one is up. 
        Warning: If CAN1 and CAN1 are connected to eachother, they will try to
        send practically simultaneously. Let this be a lesson; sending the same 
        ID from two nodes onto the same bus at the same time is very hazardous
        as the arbitration cannot take place. Only use both lines below if 
        CAN1 and CAN1 are on different buses. */
        if (FRAME_ID_MODE == STD_ID_MODE )
        {
            R_CAN_TxSet(g_can_channel, CANBOX_TX, &err_tx_dataframe, DATA_FRAME);   
        }
        else
        {
            R_CAN_TxSetXid(g_can_channel, CANBOX_TX, &err_tx_dataframe, DATA_FRAME);
        } 

        //R_CAN_TxSet(1, TX_ID_DEFAULT, &err_tx_dataframe, DATA_FRAME);
    }
}/* end HandleCanBusState() */

/*******************************************************************************
Function Name:  ResetAllErrors
Description:  Reset all types of errors, application and CAN peripeheral errors.
Parameters:     -
Return value:  CAN API code
*******************************************************************************/
uint32_t ResetAllErrors(void)
{    
    uint32_t status = 0;

    /* Reset errors */
    app_err_nr = APP_NO_ERR;

    error_bus_status[0] = 0;
    error_bus_status[1] = 0;

    /* You can chooose to not reset error_bus_status_prev; if there was an error, 
    keep info to signal recovery */
    error_bus_status_prev[0] = 0; 
    error_bus_status_prev[1] = 0;

    nr_times_reached_busoff[0] = 0;
    nr_times_reached_busoff[1] = 0;

    /* Reset Error Judge Factor and Error Code registers */
    CAN1.EIFR.BYTE /*= CAN1.EIFR.BYTE*/ = 0;

    /* Reset CAN1 Error Code Store Register (ECSR). */
    CAN1.ECSR.BYTE = 0;

    /* Reset CAN1 Error Counters. */
    CAN1.RECR = 0;
    CAN1.TECR = 0;

    return status;
}/* end ResetAllErrors() */


/******************************************************************************
Function Name:    can_channel_start
Parameters:      Channel nr.
Return value:    none
Description:    Unlocks the protect (PRCR) register bit for access to the 
                    peripheral modules' registers so that the CAN channel can 
                    be taken out of module stop state, then re-locks the PRCR.
                    In an multitasking or reentrant situation, this function 
                    should, itself, be protected with a semaphore type lock.
******************************************************************************/
static void can_channel_start(uint32_t ch_nr)
{
    /* First unlock the protect register. */         
    /* Enable writing to PRCR bits while simultaneously enabling PRC1. */
    SYSTEM.PRCR.WORD = 0xA502;
    
    R_CAN_ModuleStopStateCancel(ch_nr);
    
    /* Re-lock the protect register. */         
    /* Enable writing to PRCR bits while simultaneously disabling PRC1. */
    SYSTEM.PRCR.WORD = 0xA500;     
    
} /* End can_channel_start(). */  

     
/******************************************************************************
Function Name:    TestLEDs
Parameters:      -
Return value:    -
Description:    Blink the LEDs
******************************************************************************/
void TestLEDs(uint32_t nr_led_flashes)
{
    uint32_t i = LED_DELAY;
    uint32_t j;

    LED0 = LED_ON;
    LED1 = LED_ON;
    LED2 = LED_ON;
    LED3 = LED_ON;

    for (j = 0; j < nr_led_flashes; j++)
    {
        while(i--)
        {
        }

        i = LED_DELAY;
        
        LED0 ^= 1;
        LED1 ^= 1;
        LED2 ^= 1;
        LED3 ^= 1;
    }
    ALL_LEDS_OFF
} /* end TestLEDs() */

/*******************************************************************************
Function Name:  Delay
Description:  Demo delay
Parameters:     32-bit delay count value
Return value:  -
*******************************************************************************/
#pragma inline=never
void Delay(uint32_t n)
{
    uint32_t i;

    for(i = 0; i < n; i++)
  {
        nop();
  }
}/* end Delay() */


/*******************************************************************************


CAN INTERRRUPTS 
Interrupts are duplicated for each CAN channel used except for the Error 
interrupt which handles all channels in a group. 
Vectors are set according to the channel.
*******************************************************************************/
#ifndef USE_CAN_POLL
/*****************************************************************************
Name:        CAN ISRs
Parameters:    -
Returns:      -
Description:    CAN interrupt routine examples.
*****************************************************************************/
/*****************************************************************************
Name:      CAN1_TXM0_ISR
Parameters:    -
Returns:    -
Description:  CAN1 Transmit interrupt.
                Check which mailbox transmitted data and process it.
*****************************************************************************/
#pragma vector=VECT_CAN1_TXM1
__interrupt void CAN1_TXM1_ISR(void)
{ 
    //Enable nested interrupts
    __enable_interrupt();
  
    uint32_t api_status = R_CAN_OK;

    api_status = R_CAN_TxCheck(CH_1, CANBOX_TX);
    if (api_status == R_CAN_OK)
        CAN1_tx_sentdata_flag = 1;
    api_status = R_CAN_TxCheck(CH_1, CANBOX_REMOTE_TX);
    if (api_status == R_CAN_OK)
        CAN1_tx_remote_sentdata_flag = 1;

    /* Use mailbox search reg. Should be faster than above if a lot of mailboxes to check. 
    Not verified. */
}/* end CAN1_TXM0_ISR() */

/*****************************************************************************
Name:      CAN1_RXM0_ISR
Parameters:    -
Returns:    -
Description:  CAN1 Receive interrupt.
                Check which mailbox received data and process it.
*****************************************************************************/
#pragma vector=VECT_CAN1_RXM1
__interrupt void CAN1_RXM1_ISR(void)
{
    //Enable nested interrupts
    __enable_interrupt();
    
    /* Use CAN API. */
    uint32_t api_status = R_CAN_OK;

    api_status = R_CAN_RxPoll(CH_1, CANBOX_RX);
    if (api_status == R_CAN_OK)
    {
        CAN1_rx_newdata_flag = 1;
    }

    api_status = R_CAN_RxPoll(CH_1, CANBOX_REMOTE_RX);
    if (api_status == R_CAN_OK)
    {
        /* REMOTE_FRAME FRAME REQUEST RECEIVED */
        /* Do not set BP on the next line to check for Remote frame. By the time you 
        continue, the recsucc flag will already have changed to be a trmsucc flag in 
        the CAN status reg. */

        /* Reset of the receive/transmit flag in the MCTL register will be done by 
        set_remote_reply_std_CAN1(). */

        /* Tell application. */
        CAN1_rx_remote_frame_flag = 1;

        //remote_frame.dlc = CAN1.MB[CANBOX_REMOTE_RX].DLC.BIT.DLC;
        remote_frame.dlc = (uint8_t)(CAN1.MB[CANBOX_REMOTE_RX].DLC);    

        /* Reset NEWDATA flag since we won't be reading the mailbox. */
        CAN1.MCTL[CANBOX_REMOTE_RX].BIT.RX.NEWDATA = 0;
    }

    /* Use mailbox search reg. Should be faster if a lot of mailboxes to check. 
    Not verified. */
}/* end CAN1_RXM0_ISR() */

/*****************************************************************************
Name:      CAN_ERS_ISR
Parameters:    -
Returns:    -
Description:  CAN Group Error interrupt.
                Check which CAN channel is source of interrupt
*****************************************************************************/
#pragma vector=VECT_ICU_GROUP0
__interrupt void CAN_ERS_ISR(void)
{
    //Enable nested interrupts
    __enable_interrupt();  

    /* Error interrupt can have multiple sources. Check interrupt flags to id source. */

    if (IS(CAN0, ERS0))
    {
        LED3 = LED_ON;    /*TODO: additional error handling/cause identification */ 
    }
    if (IS(CAN1, ERS1))
    {
        LED3 = LED_ON;    /*TODO: additional error handling/cause identification */  
        CLR(CAN1, ERS1) = 1;  /* clear interrupts */      
    }
    if (IS(CAN2, ERS2))
    {
        LED3 = LED_ON;    /*TODO: additional error handling/cause identification */    
    }

    nop();
}/* end CAN1_ERS0_ISR() */
#endif //USE_CAN_POLL

/* eof */

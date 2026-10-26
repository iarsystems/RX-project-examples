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
/* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.   */
/***********************************************************************************************************************
* File Name    : api_demo.c
* Version      : 1.00
* Description  : Demo application program

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
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 10.12.2011 2.00    From RSK630 project.
***********************************************************************************************************************/

/******************************************************************************
Includes    <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
#include "iorx63t_clr.h"
#include "r_can_api.h"
#include "config_r_can_rapi.h"
#include "led.h"
#include "lcd.h"
#include "switches.h"
#include "api_demo.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Macro definitions
******************************************************************************/
#define LED_DELAY                0x00080000
#define NR_STARTUP_TEST_FRAMES    10

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
can_frame_t        tx_dataframe, rx_dataframe, remote_frame;

/* the CAN peripheral channel used in this demo */
uint32_t g_can_channel;

/* Demo flags. */
#if (USE_CAN_POLL == 0)
uint32_t            CAN1_tx_sentdata_flag = 0;
uint32_t            CAN1_tx_remote_sentdata_flag = 0;
uint32_t            CAN1_rx_newdata_flag = 0;
uint32_t            CAN1_rx_test_newdata_flag = 0;
uint32_t            CAN1_rx_remote_frame_flag = 0;
#endif //USE_CAN_POLL
enum app_err_enum    app_err_nr;
/* Functions */

/******************************************************************************
Global variables and functions private to the file
******************************************************************************/
/* Data */
/* Errors. Peripheral and bus errors. Space for each channel. */
static uint32_t        error_bus_status[MAX_CHANNELS];
static uint32_t     error_bus_status_prev[MAX_CHANNELS];
static uint32_t        can_state[MAX_CHANNELS];
static uint32_t        nr_times_reached_busoff[MAX_CHANNELS];

/* TEST CAN ID */
uint32_t g_tx_id_default;
uint32_t g_rx_id_default;

/* Functions */
static uint32_t InitCanApp(void);
static void CheckCANerrors(void);
static void HandleCanBusState(uint8_t ch_nr);
static void TestLEDs(uint32_t nr_led_flashes);

#if (USE_CAN_POLL == 1)
static void CanPollDemo(void);
#else
static void CanIntDemo(void);
#endif //USE_CAN_POLL


/**********************************************************************************
User Program Code
***********************************************************************************/
/*******************************************************************************
* Function Name : api_demo
* Description   : Main Can API demo program
* Argument      : None
* Return value  : None
*******************************************************************************/
void api_demo(void)
{
    uint32_t i, api_status = R_CAN_OK;

    g_can_channel = CH_1; /* using channel 1 for this demo */

    Display_LCD(LCD_LINE1, "CAN API");
    Display_LCD(LCD_LINE2, "TxRxDemo");

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

    /* Init CAN. */
    api_status = R_CAN_Create(g_can_channel);

    if (api_status != R_CAN_OK)
    {   /* An error at this stage is fatal to demo, so stop here. */
        Display_LCD(LCD_LINE1,"API err");
        Display_LCD(LCD_LINE2,"        ");
        LcdShow2DigHex(api_status, 1*16 + 4);

        while(1)
        {
            /* Wait here and leave error displayed. */
        }
    }

    /*********************************
    * Pick ONE R_CAN_PortSet call below!    *
    **********************************/
    /* Normal CAN bus usage. */
    R_CAN_PortSet(g_can_channel, ENABLE);
    /* Test modes. With Internal loopback mode you only need one board! */
//  R_CAN_PortSet(g_can_channel, CANPORT_TEST_1_INT_LOOPBACK);
//    R_CAN_PortSet(g_can_channel, CANPORT_TEST_0_EXT_LOOPBACK);
//    R_CAN_PortSet(g_can_channel, CANPORT_TEST_LISTEN_ONLY);

    /* Init CAN mailboxes. */
    api_status |= InitCanApp();

    /* Is all OK after all CAN initialization? */
    if (api_status != R_CAN_OK)
    {
        api_status = R_CAN_OK;
        app_err_nr = APP_ERR_CAN_INIT;
    }

    /* Interrupt Enable flag is set by default. */

    /*****************************************************************************/
    /* This is how you send multiple messages back to back. Sending 10 messages. */
    /*****************************************************************************/
    if (FRAME_ID_MODE == STD_ID_MODE )
    {
        api_status |= R_CAN_TxSet(g_can_channel, CANBOX_TX, &tx_dataframe, DATA_FRAME);
    }
    else
    {
        api_status |= R_CAN_TxSetXid(g_can_channel, CANBOX_TX, &tx_dataframe, DATA_FRAME);
    }

    #if (USE_CAN_POLL == 1)
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
    #else    //Using CAN interrupts.
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

    /*    M A I N    L O O P    * * * * * * * * * * * * * * * * * * * * * * * * * */
    while(1)
    {
        /* User pressing switch(es)? */
        ReadSwitches();

        /* Check for CAN errors. */
         CheckCANerrors();

        if (can_state[0] != R_CAN_STATUS_BUSOFF)
        {
            #if (USE_CAN_POLL == 1)
            CanPollDemo();
            #else
            CanIntDemo();
            #endif //USE_CAN_POLL
        }
        else
        /* Bus Off. */
        {
            Display_LCD(LCD_LINE1, "App in    ");
            Display_LCD(LCD_LINE2, "Bus Off ");

            /* HandleCanBusState() will restart app. */
            LCD_FLASH
        }
        /* Reset receive/transmit indication. */
        led_set(LED2, LED_OFF);
        led_set(LED3, LED_OFF);
    }
}/* end main() */
/*******************************************************************************
* End of function api_demo
*******************************************************************************/

#if (USE_CAN_POLL == 1)
/*******************************************************************************
* Function Name : CanPollDemo
* Description   : POLLED CAN demo version
* Argument      : None
* Return value  : None
*******************************************************************************/
static void CanPollDemo(void)
{
    uint32_t    api_status = R_CAN_OK;

    /*** TRANSMITTED any frames? */
    api_status = R_CAN_TxCheck(g_can_channel, CANBOX_TX);
    if (api_status == R_CAN_OK)
    {
        led_set(LED2, LED_ON);
        Display_LCD(LCD_LINE1, "TxChk OK");
        LcdShow4DigHex(tx_dataframe.data[0], tx_dataframe.data[1], 16);
        LcdShow4DigHex(tx_dataframe.data[2], tx_dataframe.data[3], 20); LCD_FLASH
    }
    /* Since we are always polling for transmits, api_status for R_CAN_TxCheck
      will most often be other than R_CAN_OK, don't show that in this demo.*/

    /*** RECEIVED any frames? */
    api_status = R_CAN_RxPoll(g_can_channel, CANBOX_RX);
    if (api_status == R_CAN_OK)
    {
        led_set(LED1, LED_ON);
        Display_LCD(LCD_LINE1, "RxPollOK"); LCD_FLASH
        Display_LCD(LCD_LINE1, "RxRead: ");

        /* Read CAN data and show. */
        api_status = R_CAN_RxRead(g_can_channel, CANBOX_RX, &rx_dataframe);
        LcdShow4DigHex(rx_dataframe.data[0], rx_dataframe.data[1], 16);
        LcdShow4DigHex(rx_dataframe.data[2], rx_dataframe.data[3], 20); LCD_FLASH
        if (api_status == R_CAN_MSGLOST)
        {
            Display_LCD(LCD_LINE2, "MSGLOST"); LCD_FLASH
        }
        led_set(LED1, LED_OFF);
    }
}/* end CanPollDemo() */
/*******************************************************************************
* End of function CanPollDemo
*******************************************************************************/

#else

/*******************************************************************************
* Function Name : CanIntDemo
* Description   : INTERRUPT driven CAN demo version
* Argument      : None
* Return value  : None
*******************************************************************************/
static void CanIntDemo(void)
{
    uint32_t    api_status = R_CAN_OK;

    /************************************************************************
    * Using CAN INTERRUPTS.                                                    *
    * See also r_can_api.c for the ISR example.                                *
    *************************************************************************/
    /* TRANSMITTED any frames? If flag can0_tx_sentdata_flag was set by the
    CAN Tx ISR, the frame was sent successfully. */
    if (CAN1_tx_sentdata_flag)
    {
        CAN1_tx_sentdata_flag = 0;
        led_set(LED2, LED_ON);
        /* Show CAN frame was sent. */
        Display_LCD(LCD_LINE1, "Tx OK   ");
        LcdShow4DigHex(tx_dataframe.data[0], tx_dataframe.data[1], 16);
        LcdShow4DigHex(tx_dataframe.data[2], tx_dataframe.data[3], 20); LCD_FLASH
    }
    if (CAN1_tx_remote_sentdata_flag)
    {
        CAN1_tx_remote_sentdata_flag = 0;
        Display_LCD(LCD_LINE1, "TxRemote"); LCD_FLASH

    }

    /*** RECEIVED any frames? Only need to check if flag is set by CAN Rx ISR.
     Will only receive own frames in CAN port test modes 0 and 1. */
    if (CAN1_rx_newdata_flag)
    {
        CAN1_rx_newdata_flag = 0;
        led_set(LED0, LED_ON);
        Display_LCD(LCD_LINE1, "Rx OK   "); LCD_FLASH
        Display_LCD(LCD_LINE1, "Rx Read:");

        /* Read CAN data and show. */
        api_status = R_CAN_RxRead(g_can_channel, CANBOX_RX, &rx_dataframe);
        LcdShow4DigHex(rx_dataframe.data[0], rx_dataframe.data[1], 16);
        LcdShow4DigHex(rx_dataframe.data[2], rx_dataframe.data[3], 20); LCD_FLASH
        if (api_status == R_CAN_MSGLOST)
        {
            //lcd_display(LCD_LINE2, "MSGLOST"); LCD_FLASH
        }
        led_set(LED0, LED_OFF);
    }
    if (CAN1_rx_test_newdata_flag)
    {
        CAN1_rx_test_newdata_flag = 0;
        led_set(LED0, LED_ON);
        Display_LCD(LCD_LINE1, "Rx Test"); LCD_FLASH
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
    led_set(LED0, LED_OFF);
}/* end CanIntDemo() */
/*******************************************************************************
* End of function CanIntDemo
*******************************************************************************/
#endif //USE_CAN_POLL

/*******************************************************************************
* Function Name : InitCanApp
* Description   : Initialize CAN demo application
* Argument      : None
* Return value  : None
*******************************************************************************/
static uint32_t InitCanApp(void)
{
    uint32_t    api_status = R_CAN_OK;
    uint8_t     i;

    can_state[0] = R_CAN_STATUS_ERROR_ACTIVE;

    for (uint32_t i = 0; i < MAX_CHANNELS; i++) /* Initialize status for all channels. */
    {
        error_bus_status[i] = R_CAN_STATUS_ERROR_ACTIVE;
        error_bus_status_prev[i] = R_CAN_STATUS_ERROR_ACTIVE;
    }

    /* Configure mailboxes in Halt mode. */
    api_status |= R_CAN_Control(g_can_channel, HALT_CANMODE);

    /********    Init demo to recieve data    ********/
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

    /********    Init. demo Tx dataframe RAM structure    ********/
    /* Standard id. Choose value 0-0x07FF (2047). */
    tx_dataframe.id            =    g_tx_id_default;
    tx_dataframe.dlc        =    8;
    tx_dataframe.data[0]    =    0x00;
    tx_dataframe.data[1]    =    0x11;
    tx_dataframe.data[2]    =    0x22;
    tx_dataframe.data[3]    =    0x33;
    tx_dataframe.data[4]    =    0x44;
    tx_dataframe.data[5]    =    0x55;
    tx_dataframe.data[6]    =    0x66;
    tx_dataframe.data[7]    =    0x77;


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
/*******************************************************************************
* End of function InitCanApp
*******************************************************************************/

/*******************************************************************************
* Function Name : CheckCANerrors
* Description   : Check for all possible errors, in app and peripheral. Add
*                 checking for your app here.
* Argument      : None
* Return value  : None
*******************************************************************************/
static void CheckCANerrors(void)
{
    /* Error passive or more? */
    HandleCanBusState(g_can_channel);

    if (app_err_nr)
    {
        /* Show error to user */
        /* RESET ERRORs with SW1. */
        led_set(LED3, LED_ON);
        Display_LCD(LCD_LINE1,"App err");
        LcdShow2DigHex(app_err_nr, 1*16 + 4);
        LCD_FLASH
        led_set(LED3, LED_OFF);
    }
}/* end CheckCANerrors() */
/*******************************************************************************
* End of function CheckCANerrors
*******************************************************************************/

/*******************************************************************************
* Function Name : HandleCanBusState
* Description   : Check CAN peripheral bus state.
* Argument      : uint8_t ch_nr - CAN channel number
* Return value  : None
*******************************************************************************/
static void HandleCanBusState(uint8_t ch_nr)
{
    can_frame_t err_tx_dataframe;

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
                        Display_LCD(LCD_LINE1, "Bus0: OK");
                    }
                    else
                    {
                        Display_LCD(LCD_LINE1, "Bus1: OK");
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
                    Display_LCD(LCD_LINE1,"bus0:    ");
                }
                else if (ch_nr == 1)
                {
                    Display_LCD(LCD_LINE2,"bus1:    ");
                }
                else if (ch_nr == 2)
                {
                    Display_LCD(LCD_LINE2,"bus2:    ");
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
        err_tx_dataframe.dlc =    1;
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
* End of function HandleCanBusState
*******************************************************************************/

/*******************************************************************************
* Function Name : ResetAllErrors
* Description   : Reset all types of errors, application and CAN peripeheral errors.
* Argument      : None
* Return value  : CAN API code
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
/*******************************************************************************
* End of function ResetAllErrors
*******************************************************************************/

/*******************************************************************************
* Function Name : TestLEDs
* Description   : Blink the LEDs
* Argument      : uint32_t nr_led_flashes - number of flashes
* Return value  : None
*******************************************************************************/
void TestLEDs(uint32_t nr_led_flashes)
{
    uint32_t i = LED_DELAY;
    uint32_t j;

    led_set(LED0, LED_ON);
    led_set(LED1, LED_ON);
    led_set(LED2, LED_ON);
    led_set(LED3, LED_ON);

    for (j = 0; j < nr_led_flashes; j++)
    {
        while(i--)
        {
        }

        i = LED_DELAY;

        led_toggle(LED0);
        led_toggle(LED1);
        led_toggle(LED2);
        led_toggle(LED3);
    }

    led_set(LED0, LED_OFF);
    led_set(LED1, LED_OFF);
    led_set(LED2, LED_OFF);
    led_set(LED3, LED_OFF);

} /* end TestLEDs() */
/*******************************************************************************
* End of function TestLEDs
*******************************************************************************/

/*******************************************************************************
* Function Name : Delay
* Description   : Demo delay
* Argument      : 32-bit delay count value
* Return value  : None
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
* End of function Delay
*******************************************************************************/


/*******************************************************************************


CAN INTERRRUPTS
Interrupts are duplicated for each CAN channel used except for the Error
interrupt which handles all channels in a group.
Vectors are set according to the channel.
*******************************************************************************/
#if (USE_CAN_POLL == 0)
/*******************************************************************************
* Function Name : CAN1_TXM0_ISR
* Description   : CAN1 Transmit interrupt.
*                 Check which mailbox transmitted data and process it.
* Argument      : None
* Return value  : None
*******************************************************************************/
#pragma vector=VECT_CAN1_TXM1
__interrupt __nested void CAN1_TXM1_ISR(void)
{
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
/*******************************************************************************
* End of function CAN1_TXM0_ISR
*******************************************************************************/

/*******************************************************************************
* Function Name : CAN1_RXM0_ISR
* Description   : CAN1 Receive interrupt.
*                 Check which mailbox received data and process it.
* Argument      : None
* Return value  : None
*******************************************************************************/
#pragma vector=VECT_CAN1_RXM1
__interrupt __nested void CAN1_RXM1_ISR(void)
{
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
/*******************************************************************************
* End of function CAN1_RXM0_ISR
*******************************************************************************/

/*******************************************************************************
* Function Name : CAN_ERS_ISR
* Description   : CAN Group Error interrupt.
*                 Check which CAN channel is source of interrupt
* Argument      : None
* Return value  : None
*******************************************************************************/
#pragma vector=106
__interrupt  void Excep_ICU_GROUP0(void)
{
    /* Error interrupt can have multiple sources. Check interrupt flags to id source. */

    if (IS(CAN1, ERS1))
    {
        led_set(LED3, LED_ON);        /*TODO: additional error handling/cause identification */
        CLR(CAN1, ERS1) = 1;    /* clear interrupts */
    }

    nop();
}/* end CAN1_ERS0_ISR() */
/*******************************************************************************
* End of function CAN_ERS_ISR
*******************************************************************************/

#endif //USE_CAN_POLL

/* eof */

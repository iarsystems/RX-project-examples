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
* File Name    : api_demo.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Demo application program

        Ensure that R108, R111 and R118 are fitted.

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
        TX_ID_DEFAULT.
        
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
        monitor mentioned above.
        
* Limitations  - 
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Removed RPDL)        
*******************************************************************************/

/*******************************************************************************
Includes  <System Includes> , "Project Includes"
*******************************************************************************/
/*  */
#include <machine.h>
/*  */
#include "hwsetup.h"
#include "api_demo.h"
#include "config_r_can_rapi.h"
#include "lcd.h"
#include "stdint.h"
#include "switches.h"
#include "rskrx62tdef.h"
#include <intrinsics.h>


/******************************************************************************
Macro definitions
******************************************************************************/
/* Defines a delay value used for flashing the user LEDs */
#define LED_DELAY        0x00080000
/* Defines the number of test frames */
#define NR_STARTUP_TEST_FRAMES  10


/******************************************************************************
Global variables and functions exported
******************************************************************************/
/* Data structure instantiation of the Demo data frame */
can_std_frame_t  tx_dataframe, rx_dataframe, remote_frame;

/* Declare and initialise demo flags. */
#ifndef USE_CAN_POLL
uint32_t      can0_tx_sentdata_flag = 0;
uint32_t      can0_tx_remote_sentdata_flag = 0;
uint32_t      can0_rx_newdata_flag = 0;
uint32_t      can0_rx_test_newdata_flag = 0;
uint32_t      can0_rx_remote_frame_flag = 0;
/* USE_CAN_POLL */
#endif 

/* Create an instance of application error codes */
enum app_err_enum  app_err_nr;


/******************************************************************************
Global variables and functions private to the file
******************************************************************************/
/* Declare data variables used to store errors. Prepared for two buses, but 
   only one in RX62T. */
static uint32_t    error_bus_status[2], error_bus_status_prev[2];
static uint32_t    can_state[2];
static uint32_t    nr_times_reached_busoff[2];

/* Function declarations */
static uint32_t InitCanApp(void);
static void CheckCANerrors(void);
static void HandleCanBusState(uint8_t ch_nr);
static void TestLEDs(uint32_t nr_led_flashes);

/* USE_CAN_POLL */
#ifdef USE_CAN_POLL
/* Function declaration */
static void CanPollDemo(void);
/* Use interrupts */
#else 
/* Function declaration */ 
static void CanIntDemo(void);
#endif 


/*******************************************************************************
* Outline     : Main function. Initialises the LCD display and CAN module. 
*          It then performs transmission of test frames to check if it 
*          is connected to another board. Once the connection is  
*          verified, it waits in an infinte loop waiting for commands 
*          initiate by switch presses.
* Description   : none
* Argument    : none
* Return value  : none
*******************************************************************************/
void main(void)
{
  /* Declaration of count varaiable */ 
  uint32_t i;
  /* Declaration of variable used used to keep a record of the api status */
  uint32_t api_status = R_CAN_OK;
  
  /* Configure MCU for 100MHz CPU Clock, ports, peripherals,... */
  HardwareSetup();
  
  __enable_interrupt();
  
  /* Initialise the LCD display. */
  InitialiseDisplay();
  
  /* Display information on the debug LCD */
  DisplayString(LCD_LINE1, "  CAN   ");
  DisplayString(LCD_LINE2, "TxRxDemo");
  
  /* Blink LEDs. */
  TestLEDs(20);

  /* Initialise the CAN module */
  api_status = R_CAN_Create(0);
            
  /* Normal CAN bus usage. */
  R_CAN_PortSet(0, ENABLE);
  
  /* Optional test modes. Ensure to comment the above instruction 
     when using one of the optional test modes  */
     
  /* Internal loopback mode test. Only one board is required. */
  //R_CAN_PortSet(0, CANPORT_TEST_1_INT_LOOPBACK);
  /* External loopback mode test. */
  //R_CAN_PortSet(0, CANPORT_TEST_0_EXT_LOOPBACK);
  /* Reception-only mode test. Only one board is required */
  //R_CAN_PortSet(0, CANPORT_TEST_LISTEN_ONLY);
  
  /* Initialise the CAN mailboxes. */
  api_status |= InitCanApp();
  
  /* Is all OK after all CAN initialisation? */
  if (api_status != R_CAN_OK)
  {
    /* Indicate that no errors detected during initialisation */
    api_status = R_CAN_OK;
    app_err_nr = APP_ERR_CAN_INIT;
  }
  
  /* Interrupt Enable flag is set by default. */
  
  /* Send several test messages back to back. */
  api_status |= R_CAN_TxSet(0, CANBOX_TX, &tx_dataframe, DATA_FRAME);
  
  /* Use polling method if defined */
  #ifdef USE_CAN_POLL
  
    /* Check a mailbox for successful data frame transmission */
    while (R_CAN_TxCheck(0, CANBOX_TX));
    
    /* Send 10 test frame data */
    for (i = 0; i < NR_STARTUP_TEST_FRAMES; i++)
    {
      /* Transmit data from mailbox 1 on channel 0. Store the  
      transmission status, whether successful or unsuccessful */
      api_status |= R_CAN_Tx(0, CANBOX_TX);
      
      /* Check a mailbox for successful data frame transmission */
      while (R_CAN_TxCheck(0, CANBOX_TX))
      {
        /* Do nothing */
      }
    }
  /* Using CAN interrupts */
  #else
    /* Wait for an interrupt generation */
    while(can0_tx_sentdata_flag == 0)
    {
      /* Do nothing */  
    }
    
    /*  Reset flag */
    can0_tx_sentdata_flag = 0;
    
    /* Send 10 test frame data */
    for (i = 0; i < NR_STARTUP_TEST_FRAMES; i++)
    {
      /* Transmit data from mailbox 1 on channel 0. Store the  
      transmission status, whether successful or unsuccessful */
      api_status |= R_CAN_Tx(0, CANBOX_TX);
      
      /* Wait for an interrupt generation */
      while(can0_tx_sentdata_flag == 0)
      {
        /* Do nothing */  
      }
      
      /* Reset flag */
      can0_tx_sentdata_flag = 0;
    }
  #endif
  
  /* Infinte while loop */
  while(1)
  {
    /* User pressing switch(es)? */
    ReadSwitches();

    /* Check for CAN errors. */
     CheckCANerrors();
    
    /* Run the demo if no bus issues encountered */
    if (can_state[0] != R_CAN_STATUS_BUSOFF)
    {
      /* Polling method */
      #ifdef USE_CAN_POLL
        /* Run the demo */
        CanPollDemo();
      /* Interrupt driven */  
      #else
        /* Run the demo */
        CanIntDemo();
      #endif 
    }
     else
    {
      /* Alert user on the LCD display when bus errors have been detected */
      DisplayString(LCD_LINE1, "App in  ");
      DisplayString(LCD_LINE2, "Bus Off ");
      
      /* HandleCanBusState() will restart app. */
      
      /* Clear the LCD display */
      LCD_FLASH
    }
    
    /* Reset receive/transmit indication. */
    RED_LED2 = LED_OFF;
    RED_LED3 = LED_OFF;
  }
}
/*******************************************************************************
* End of function main
*******************************************************************************/  

#ifdef USE_CAN_POLL
/*******************************************************************************
* Outline     : CanPollDemo
* Description   : POLLED CAN demo version
* Argument    : none
* Return value  : none
*******************************************************************************/
static void CanPollDemo(void)
{
  /* Declare and initialise variable for storing the CAN bus status */
  uint32_t api_status = R_CAN_OK;
  
  /*** TRANSMITTED any frames? */
  api_status = R_CAN_TxCheck(0, CANBOX_TX);
  
  /* For a successful transmission of mailbox 1's data */
  if (api_status == R_CAN_OK)
  {
    /* Turn on LED1 */
    RED_LED2 = LED_ON;
    
    /* Inform the user on line 1 of the LCD 
       that the transmission was successful */
    DisplayString(LCD_LINE1, "TxChk OK");
    
    /* Display the sent data frame */
    LcdShow4DigHex(tx_dataframe.data[0], tx_dataframe.data[1], 16);
    LcdShow4DigHex(tx_dataframe.data[2], tx_dataframe.data[3], 20); 
    
    /* Clear the LCD display */
    LCD_FLASH
  }
  
  /* Since we are always polling for transmits, api_status for R_CAN_TxCheck 
  will most often be other than R_CAN_OK, don't show that. */
  
  /*** RECEIVED any frames? */
  api_status = R_CAN_RxPoll(0, CANBOX_RX);
  
  /* For a successful reception data */
  if (api_status == R_CAN_OK)
  {
    /* Turn on LED0 */
    ORG_LED1 = LED_ON;
    
    /* Inform the user that data received after polling  */
    DisplayString(LCD_LINE1, "RxPollOK"); 
    
    /* Clear the LCD display */
    LCD_FLASH
    
    /* Indicate that the received polled data is to be displayed */
    DisplayString(LCD_LINE1, "RxRead: ");
    
    /* Read CAN data and show. */
    api_status = R_CAN_RxRead(0, CANBOX_RX, &rx_dataframe);
    
    /* Displayed the received data on the LCD's line 2 */
    LcdShow4DigHex(rx_dataframe.data[0], rx_dataframe.data[1], 16);
    LcdShow4DigHex(rx_dataframe.data[2], rx_dataframe.data[3], 20); 
    
    /* Clear the LCD display */
    LCD_FLASH
    
    /* On detection of a lost message */
    if (api_status == R_CAN_MSGLOST)
    {
      /* Inform user by writing to line 2 of the LCD display */
      DisplayString(LCD_LINE2, "MSGLOST"); 
      
      /* Clear the LCD's line 1 */
      LCD_FLASH
    }

    /* Turn off LED1 */
    ORG_LED1 = LED_OFF;    
  }
}
/*******************************************************************************
End of function CanPollDemo
*******************************************************************************/  

/* USE_CAN_POLL */ 
#else

/*******************************************************************************
* Outline     : CanIntDemo
* Description   : Interrupt driven CAN demo version
* Argument    : none
* Return value  : none
*******************************************************************************/
static void CanIntDemo(void)
{
  /* Declare and initialise variable for storing the CAN bus status */
  uint32_t api_status = R_CAN_OK;
  
  /************************************************************************
  * Using CAN INTERRUPTS.                          *
  * See also r_can_api.c for the ISR example.                *
  *************************************************************************/
  /*** TRANSMITTED any frames? Only need to check if flag is set by CAN Tx
  ISR. */
  if (can0_tx_sentdata_flag)
  {
    /* Clear flag */
    can0_tx_sentdata_flag = 0;
    
    /* Turn on LED2 */
    RED_LED2 = LED_ON;
    
    /* Show CAN frame was sent. */
    DisplayString(LCD_LINE1, "Tx OK   ");
    LcdShow4DigHex(tx_dataframe.data[0], tx_dataframe.data[1], 16);
    LcdShow4DigHex(tx_dataframe.data[2], tx_dataframe.data[3], 20); 
    
    /* Clear the LCD display */
    LCD_FLASH
  }
  
  /* Check if the flag was set */
  if (can0_tx_remote_sentdata_flag)
  {
    /* Reset flag */
    can0_tx_remote_sentdata_flag = 0;
    
    /* Display information on line 1 of the LCD */
    DisplayString(LCD_LINE1, "TxRemote"); 
    
    /* Clear the LCD display */
    LCD_FLASH
  }          
  
  /*** RECEIVED any frames? Only need to check if flag is set by CAN Rx ISR.
   Will only receive own frames in CAN port test modes 0 and 1. */
  if (can0_rx_newdata_flag)
  {
    /*  Clear flag */
    can0_rx_newdata_flag = 0;
    
    /* Turn on LED0 */
    GRN_LED0 = LED_ON;
    
    /* Display information */
    DisplayString(LCD_LINE1, "Rx OK   "); 
    
    /* Clear the LCD display */
    LCD_FLASH
    
    /* Display information */
    DisplayString(LCD_LINE1, "Rx Read:");
  
    /* Read CAN data and save the read status */
    api_status = R_CAN_RxRead(0, CANBOX_RX, &rx_dataframe);
    
    /* Display the received data */
    LcdShow4DigHex(rx_dataframe.data[0], rx_dataframe.data[1], 16);
    LcdShow4DigHex(rx_dataframe.data[2], rx_dataframe.data[3], 20); 
    
    /* Clear the LCD display */
    LCD_FLASH
    
    /* On detection of a lost message */
    if (api_status == R_CAN_MSGLOST)
    {
      /* Display information on the LCD */
      //DisplayString(LCD_LINE2, "MSGLOST"); 
      
      /* Clear the LCD display */
      //LCD_FLASH
    }

    /* Turn off LED0 */
    GRN_LED0 = LED_OFF;
  }
  
  /* Check if the flag was set */
  if (can0_rx_test_newdata_flag)
  {
    /*  Reset flag */
    can0_rx_test_newdata_flag = 0;
    
    /* Turn on LED0 */
    GRN_LED0 = LED_ON;
    
    /* Inform user of the test being conducted */
    DisplayString(LCD_LINE1, "Rx Test"); 
    
    /* Clear the LCD */
    LCD_FLASH
  }
  
  /* Set up remote reply if remote request came in. */
  if (can0_rx_remote_frame_flag == 1)
  {
    /* Clear the flag */
    can0_rx_remote_frame_flag = 0;
    remote_frame.data[0]++;
    R_CAN_TxSet(0, CANBOX_REMOTE_TX, &remote_frame, DATA_FRAME);
  }

  /* Turn off LED0 */
  GRN_LED0 = LED_OFF;
}
/*******************************************************************************
End of function CanIntDemo
*******************************************************************************/
#endif   

/*******************************************************************************
* Outline     : InitCanApp
* Description   : Initialize CAN demo application
* Argument    : none
* Return value  : none
*******************************************************************************/
static uint32_t InitCanApp(void)
{  
  /* Declare and initialise variable for storing the CAN bus status */
  uint32_t  api_status = R_CAN_OK;
  
  /* Set the CAN state code to 'error active' */
  can_state[0] = R_CAN_STATUS_ERROR_ACTIVE;
  
  /* Configure mailboxes in Halt mode. */
  api_status |= R_CAN_Control(0, HALT_CANMODE);
  
  /********  Init demo to recieve data  ********/  
  /* Use API to set one CAN mailbox for demo receive. */
  /* Standard id. Choose value 0-0x07FF (2047). */
  api_status |= R_CAN_RxSet(0, CANBOX_RX, RX_ID_DEFAULT, DATA_FRAME);
  
  /********  Init. demo Tx dataframe RAM structure  ********/  
  /* Standard id. Choose value 0-0x07FF (2047). */
  tx_dataframe.id    =  TX_ID_DEFAULT;
  tx_dataframe.dlc    =  8;
  tx_dataframe.data[0]  =  0x00;
  tx_dataframe.data[1]  =  0x11;
  tx_dataframe.data[2]  =  0x22;
  tx_dataframe.data[3]  =  0x33;
  tx_dataframe.data[4]  =  0x44;
  tx_dataframe.data[5]  =  0x55;
  tx_dataframe.data[6]  =  0x66;
  tx_dataframe.data[7]  =  0x77;

  /* Mask for receive box. Write to mask only in Halt mode. */
  /* 0x7FF = no mask. 0x7FD = mask bit 1, for example; If receive ID is set to 1, both
  ID 1 and 3 should be received. */
  R_CAN_RxSetMask( 0, CANBOX_RX, 0x7FF);
    
  /* API to send will be set up in SW1Func() in file switches.c. */

  /* Save the current CAN mode status */
  api_status |= R_CAN_Control(0, OPERATE_CANMODE);

  
  /***************  Init. remote dataframe response **********************/
  remote_frame.id = REMOTE_TEST_ID;

  /* Length is specified by the remote request. */
  
  R_CAN_RxSet(0, CANBOX_REMOTE_RX, REMOTE_TEST_ID, REMOTE_FRAME);
  /***********************************************************************/
  
  /* Set frame buffer id so LCD shows correct receive ID from start. */
   rx_dataframe.id = RX_ID_DEFAULT;
  
   return api_status;

} 
/*******************************************************************************
* End of function InitCanApp
*******************************************************************************/  

/*******************************************************************************
* Outline     : CheckCANerrors
* Description   : Check for all possible errors, in app and peripheral. Add 
*          checking for your app here.
* Argument    : none
* Return value  : none
*******************************************************************************/
static void CheckCANerrors(void)
{
  /* Error passive or more? */
   HandleCanBusState(0);
    
  /* Show error to user if errors detected */
  if (app_err_nr)
  {
    /* Turn on LED3 */
    RED_LED3 = LED_ON;
    
    /* RESET ERRORs with SW1. */
    DisplayString(LCD_LINE1,"App err");
    LcdShow2DigHex(app_err_nr, 1*16 + 4);
    
    /* Clear LCD display */
    LCD_FLASH

    /* Turn off LED3 */
    RED_LED3 = LED_OFF;
  }
}
/*******************************************************************************
End of function CheckCANerrors
*******************************************************************************/  

/*******************************************************************************
* Outline     : HandleCanBusState
* Description   : Check CAN peripheral bus state.
* Argument    : Bus number (0 or 1)
* Return value  : none
*******************************************************************************/
static void HandleCanBusState(uint8_t  ch_nr)
{
  /* Create an instance of the data structure can_std_frame_t */
  can_std_frame_t err_tx_dataframe;
  
  /* Has the status register reached error passive or more? */
  if (ch_nr == 0)
  {
    error_bus_status[ch_nr] = R_CAN_CheckErr(0);
    }
  /*else
  {
    error_bus_status[ch_nr] = R_CAN1_CheckErr(1);
    }*/

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
           if (R_CAN_Create(0) != R_CAN_OK)
            app_err_nr |= APP_ERR_CAN_PERIPH;
          
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
          DisplayString(LCD_LINE1,"bus0:   ");
        else
          DisplayString(LCD_LINE2,"bus1:   ");
        
        /* Show user */
         LcdShow2DigHex((uint8_t)error_bus_status[ch_nr], ch_nr*16 + 6);
         Delay(0x400000);
         nr_times_reached_busoff[ch_nr]++;
         break;
    }
    
    /* Save the current bus status */
         error_bus_status_prev[ch_nr] = error_bus_status[ch_nr];

    /* Transmit CAN bus status change */
    err_tx_dataframe.id = 0x700 + ch_nr;
    err_tx_dataframe.dlc =  1;
    err_tx_dataframe.data[0] = error_bus_status[ch_nr];
    
    /* Send Error state on both channels. Maybe at least one is up. 
     Warning: If CAN0 and CAN1 are connected to eachother, they will try to
     send practically simultaneously. Let this be a lesson; sending the same 
     ID from two nodes onto the same bus at the same time is very hazardous
     as the arbitration cannot take place. Only use both lines below if 
     CAN0 and CAN1 are on different buses. */
     R_CAN_TxSet(0, TX_ID_DEFAULT, &err_tx_dataframe, DATA_FRAME);
     //R_CAN_TxSet(1, TX_ID_DEFAULT, &err_tx_dataframe, DATA_FRAME);
   }
}
/*******************************************************************************
End of function HandleCanBusState
*******************************************************************************/  

/*******************************************************************************
* Outline     : ResetAllErrors
* Description   : Reset all types of errors, application and CAN peripeheral 
*          errors.
* Argument    : none
* Return value  : CAN API code
*******************************************************************************/
uint32_t ResetAllErrors(void)
{  
   /* Declare variable */
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
   CAN0.EIFR.BYTE /*= CAN1.EIFR.BYTE*/ = 0;
  
  /* Reset CAN0 Error Code Store Register (ECSR). */
  CAN0.ECSR.BYTE = 0;
  
  /* Reset CAN0 Error Counters. */
  CAN0.RECR = 0;
  CAN0.TECR = 0;

  return status;
}
/*******************************************************************************
End of function ResetAllErrors
*******************************************************************************/  

/*******************************************************************************
* Outline     : TestLEDs
* Description   : Blink the LEDs
* Argument    : uint32_t nr_led_flashes 
* Return value  : none
*******************************************************************************/
void TestLEDs(uint32_t nr_led_flashes)
{
  /* Declare variables used for loop counts */
  uint32_t i = LED_DELAY;
  uint32_t j;
  
  /* Set user LEDs state */
  GRN_LED0 = LED_ON;
  ORG_LED1 = LED_ON;
  RED_LED2 = LED_ON;
  RED_LED3 = LED_ON;
  
  /* Loop until the desired number is reached */
  for (j = 0; j < nr_led_flashes; j++)
  {
    /* Wait until counter reaches zero */
    while(i--)
    {
      /* Do nothing */
    }
    
    /* Reset counter */
    i = LED_DELAY;
    
    /* Toggle user LEDs states */
    GRN_LED0 ^= 1;
    ORG_LED1 ^= 1;
    RED_LED2 ^= 1;
    RED_LED3 ^= 1;
  }
  
  /* Turn off all user LEDs */
  ALL_LEDS_OFF
} 
/*******************************************************************************
* End of function TestLEDs
*******************************************************************************/  

/*******************************************************************************
Function Name:  Delay
Description:  Demo delay
Parameters:     32-bit delay count value
Return value:  -
*******************************************************************************/
#pragma inline=never
void Delay(uint32_t n)
{
  /* Declare a variable to be used for counting */
  uint32_t i;
  
  /* Loop unitl the desired count value is reached */
  for(i = 0; i < n; i++)
  {
     nop();
  }
}
/*******************************************************************************
* End of function Delay
*******************************************************************************/  

/******************************************************************************
            CAN INTERRRUPTS 
*******************************************************************************/
/* Use polling methods */
#ifndef USE_CAN_POLL
/* Use interrupts */
#ifdef CAPI_CFG_CAN0_ISR

/*******************************************************************************
* Outline     : CAN0_TXM0_ISR
* Description   : CAN0 Transmit interrupt.
*          Check which mailbox transmitted data and process it.
* Argument    : none
* Return value  : none
*******************************************************************************/
#pragma vector = VECT_CAN0_TXM0
__interrupt void CAN0_TXM0_ISR(void)
{
  /* Declare variable used for storing CAN status */
  uint32_t api_status = R_CAN_OK;

  /* Check if mailbox 1's data was successfully 
     transmitted and store the status of this check */
  api_status = R_CAN_TxCheck(0, CANBOX_TX);
  
  /* Check if data was sent successfully */
  if (api_status == R_CAN_OK)
  {
    /* Set flag to indicate successful data transmission */
    can0_tx_sentdata_flag = 1;
  }
  
  /* Check if mailbox 12's data was successfully 
     transmitted and store the status of this check */
  api_status = R_CAN_TxCheck(0, CANBOX_REMOTE_TX);
  
  /* Check if data was sent successfully */
  if (api_status == R_CAN_OK)
  {
    /* Set flag to indicate successful transmission */
    can0_tx_remote_sentdata_flag = 1;
  }
  
  /* Use mailbox search reg. Should be faster than above if a lot of mail-
  boxes to check. Not verified. */
}
/******************************************************************************
* End of function CAN0_TXM0_ISR
******************************************************************************/  

/*******************************************************************************
* Outline     : CAN0_RXM0_ISR 
* Description   : CAN0 Receive interrupt.
*          Check which mailbox received data and process it.
* Argument    : none
* Return value  : none
*******************************************************************************/
#pragma vector = VECT_CAN0_RXM0
__interrupt void CAN0_RXM0_ISR(void)
{
  /* Use CAN API. */
  
  /* Declare and initialise the variable used for storing CAN status */
  uint32_t api_status = R_CAN_OK;

  /* Poll channel 0, check mailbox 1, and save the returned status */
  api_status = R_CAN_RxPoll(0, CANBOX_RX);
  
  /* Check if the status indicates successfull reception */
  if (api_status == R_CAN_OK)
  {
    /* Set flag */
    can0_rx_newdata_flag = 1;
  }
  
  /* Poll channel 0, check mailbox 12, and svae the returned status */
  api_status = R_CAN_RxPoll(0, CANBOX_REMOTE_RX);
  
  /* Check if the status indicates successfull reception */
  if (api_status == R_CAN_OK)
  {
    /* REMOTE_FRAME FRAME REQUEST RECEIVED */
    /* Do not set BP on the next line to check for Remote frame. By the time you 
    continue, the recsucc flag will already have changed to be a trmsucc flag in 
    the CAN status reg. */
  
    /* Reset of the receive/transmit flag in the MCTL register will be done by 
    set_remote_reply_std_can0(). */

    /* Tell application. */
    can0_rx_remote_frame_flag = 1;
    
    /* Read the length of data to be transmitted */
    remote_frame.dlc = CAN0.MB[CANBOX_REMOTE_RX].DLC.BIT.DLC;
    
    /* Reset NEWDATA flag since we won't be reading the mailbox. */
    CAN0.MCTL[CANBOX_REMOTE_RX].BIT.RX.NEWDATA = 0;
  }
  
  /* Use mailbox search reg. Should be faster if a lot of mailboxes to check. 
  Not verified. */
}
/*******************************************************************************
* End of function CAN0_RXM0_ISR
*******************************************************************************/

/*******************************************************************************
* Outline     : CAN0_ERS0_ISR
* Description   : CAN0 Error interrupt.
* Argument    : none
* Return value  : none
*******************************************************************************/
#pragma vector = VECT_CAN0_ERS0
__interrupt void CAN0_ERS0_ISR(void)
{
  /* Turn on LED3 */
  RED_LED3 = LED_ON;
  
  nop();
}
/*******************************************************************************
* End of function CAN0_ERS0_ISR
*******************************************************************************/

/* CAPI_CFG_CAN0_ISR */
#endif 
/* USE_CAN_POLL */ 
#endif 


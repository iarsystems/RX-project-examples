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
* Copyright (C) 2010, 2011. Renesas Electronics Corp., All Rights Reserved.
******************************************************************************
File: SWITCHES.C

For CAN API demo.

Revision History
Nov 1 '08   REA    For CAN-D-Kit version 3.00.
Mar 22 '10  REA     For RX62N with new CAN API.
June 07, 2011 REA  Port to RSKRX630 / made more generic 
******************************************************************************/
  
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
/* Following header file provides a structure to access on-chip I/O registers.*/
#include "iorx630.h"
#include "rskrx630def.h" /* contains switch to port mapping */
#include "api_demo.h"
#include "r_can_api.h"
#include "switches.h"
#include "lcd.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
/******************************************************************************
Macro definitions
******************************************************************************/
/**********************************************************************************
User Defines
***********************************************************************************/
/* SWITCHES. Low level = pressed*/
#define     SW1_RELEASED  SW1 /* Switch 1 */
#define     SW2_RELEASED  SW2 /* Switch 2 */
#define     SW3_RELEASED  SW3 /* Switch 3 */

/* logical switch combination byte patterns */
#define     SW1_MASK    0x01//0b00000001  /* Switch 1 mask */
#define     SW2_MASK    0x02//0b00000010  /* Switch 2 mask */
#define     SW3_MASK    0x04//0b00000100  /* Switch 3 mask */
#define     SW23_MASK    0x06//0b00000110  /* Switch 2 & 3 mask */
#define     SW12_MASK    0x03//0b00000011  /* Switch 1 & 2 mask */
#define     SW123_MASK     0x00//0b00000000  /* All Switches mask */
#define     SW_NONE      0x07//0b00000111  /* No Switch */

#define     SW1_PRESSED       0x06//0b00000110  /* Switch 1 pressed only */
#define     SW2_PRESSED    0x05//0b00000101  /* Switch 2 pressed only */
#define     SW3_PRESSED    0x03//0b00000011  /* Switch 3 pressed only */
#define     SW23_PRESSED  0x01//0b00000001  /* Switch 3 & 2 pressed only */



/* Nr times switch-poll must match */
#define     SW_COUNT        2
/* Switch func. table */
#define     SW_TBL_SIZE     6

/******************************************************************************
Global variables and functions imported (externs)
******************************************************************************/
/* Data */
extern uint32_t g_can_channel;

/* Functions */
/******************************************************************************
Constant definitions
*****************************************************************************/
/******************************************************************************
Global variables and functions exported
******************************************************************************/
/* Data */
/* Functions */
/******************************************************************************
Global variables and functions private to the file
******************************************************************************/
/* Data */
/* Switch */
uint8_t last_sw_data;         /* Last switch data */
uint8_t fix_sw_data;          /* Fixed switch data */
uint8_t last_fix_sw_data;     /* Last fixed switch data */
uint8_t sw_count;             /* Switch counter */

/* Functions */
static uint8_t AreSwitchesStable(uint8_t);
static uint8_t SwitchDecode(void);
static void SwNoneFunc(void);
static void Sw1Func(void);
static void Sw2Func(void);
static void Sw3Func(void);
static void Sw23Func(void);
static void Sw32Func(void);

/* An array of constant pointers to the switch functions */
void (* const SwitchFuncTable[SW_TBL_SIZE])(void) = {   SwNoneFunc,
                                                        Sw1Func, 
                                                        Sw2Func, 
                                                        Sw3Func, 
                                                        Sw32Func, 
                                                        Sw23Func   };

/*****************************************************************************
User Program Code
******************************************************************************/

/*****************************************************************************
Name:          ReadSwitches
Parameters:    -
Returns:       -
Description:   Switch function
*****************************************************************************/
void ReadSwitches(void)
{
    uint8_t i, current_sw_pos; 

  /* Build a switch byte pattern */
  current_sw_pos = 0x07;               /* initialize all switch bits to 1 */
  
  if(!SW1_RELEASED)
  {
    current_sw_pos &= SW1_PRESSED;  /* SW1 is pressed. Clear SW1 bit */
  }

  if(!SW2_RELEASED)
  {
    current_sw_pos &= SW2_PRESSED;  /* SW2 is pressed. Clear SW2 bit */
  }

  if(!SW3_RELEASED)
  {
    current_sw_pos &= SW3_PRESSED;  /* SW3 is pressed. Clear SW3 bit */  
  }

    if (AreSwitchesStable(current_sw_pos)) 
  {
        /* Switch pressed, find out which */
    i = SwitchDecode();
        if (i < SW_TBL_SIZE)
    {   /* Switch function table. Call the corresponding switch func */
            SwitchFuncTable[i]();
        }
        else
        {
            app_err_nr |= APP_TABLE_ERR;
        }
  }
}

/*****************************************************************************
Name:          sw_func
Parameters:    Current switch 1-3 positions.
Returns:       switches_stable. 1 = stable change. 0 - no stable change.
Description:   Fix switch function, if ReadSwitches are fix (stable), copy value
*****************************************************************************/
uint8_t AreSwitchesStable(uint8_t current_sw_pos)
{
    uint8_t switches_stable = 0;
    
    /* If switches have changed, time the change. */
    if ((fix_sw_data != current_sw_pos) && (last_sw_data == current_sw_pos))
  {
        sw_count++;
      /* If ReadSwitches are stable, copy value */
        if (sw_count > SW_COUNT)
      {
            fix_sw_data = current_sw_pos;
            sw_count = 0;
            switches_stable = 1;
        }
    }
    else
  {
        sw_count = 0;
    }
    
    last_sw_data = current_sw_pos;
    return switches_stable;
}/* end AreSwitchesStable() */

/*****************************************************************************
Name:          SwitchDecode
Parameters:    -
Returns:       -
Description:   Decodes which switch has been pressed
*****************************************************************************/
uint8_t SwitchDecode(void)
{
    uint8_t switch_func_nr = 0;
    
    switch (fix_sw_data)
  {
        /* No switch */
    case SW_NONE:
            switch_func_nr = 0;
            break;

        /* SW1 */
    case SW1_PRESSED:
            if (last_fix_sw_data == SW_NONE)
                switch_func_nr = 1;
            else
                switch_func_nr = 0;
            break;

        /* SW2 */
    case SW2_PRESSED:
            /* Don't call Switch 2 again after '23' until user has realeased SW2 */
            if (last_fix_sw_data == SW_NONE)
              switch_func_nr = 2;
      else if (last_fix_sw_data == SW23_PRESSED)
                switch_func_nr = 0; /* SwDummyFunc */
            else
                switch_func_nr = 0;
            break;
        /* SW3 */
    case SW3_PRESSED:
            /* Don't call Switch 3 after '32' again until user has realeased SW3 */
            if (last_fix_sw_data == SW_NONE)
              switch_func_nr = 3;
      else if (last_fix_sw_data == SW23_PRESSED)
                switch_func_nr = 0; /* SwDummyFunc */
            else
                switch_func_nr = 0;
            break;
    /* SW1 & SW2 */
    case SW23_PRESSED:
          /* SW1 --> SW2 */
            if (last_fix_sw_data == SW3_PRESSED)
                switch_func_nr = 4;
      /* SW2 --> SW1 */
        else if (last_fix_sw_data == SW2_PRESSED)
                switch_func_nr = 5;
            else
                switch_func_nr = 0;
            break;
        default:
            switch_func_nr = 0;
            break;
    }
    
    last_fix_sw_data = fix_sw_data;
    return switch_func_nr;
}/* end SwitchDecode() */

/*****************************************************************************
Name:          Sw1Func
Parameters:    -
Returns:       -
Description:   Switch 1. Sends a test CAN frame.
         A-D Demo.
*****************************************************************************/
void Sw1Func(void)
{
    /* Inc. 1st databyte for show. */
  tx_dataframe.data[0]++;
    if (FRAME_ID_MODE == STD_ID_MODE )
    {
      R_CAN_TxSet(g_can_channel, CANBOX_TX, &tx_dataframe, DATA_FRAME);   
    }
    else
    {
      R_CAN_TxSetXid(g_can_channel, CANBOX_TX, &tx_dataframe, DATA_FRAME);
    }  

}/* end Sw1Func() */

/*****************************************************************************
Name:          Sw2Func
Parameters:    -
Returns:       -
Description:   Switch 2. Shows test frame CAN TxID. 
*****************************************************************************/
void Sw2Func(void)
{
    uint32_t txid = (tx_dataframe.id & 0x1FFFFFFF);     /* Keep only extended ID bits. */
    
  DisplayString(LCD_LINE1,"Tx ID:  ");
    DisplayString(LCD_LINE2,"        ");

    if (FRAME_ID_MODE == STD_ID_MODE)
    {
      LcdShow4DigHex((uint8_t)(txid >> 8), (uint8_t)txid, 16);         
    }
    else
    {
      /* Showing all 4 bytes of ID variable in order to support extended ID. */
      LcdShow4DigHex((uint8_t)(txid >> 24), (uint8_t)(txid >> 16), 16);
      LcdShow4DigHex((uint8_t)(txid >> 8), (uint8_t)txid, 20);  
    }

    Delay(0x200000);
}/* end Sw2Func() */

/*****************************************************************************
Name:            Sw3Func
Parameters:      -
Returns:         -
Description:     Switch 3. Shows test frame CAN RxID. 
        If you are using a CAN filter mask, the incoming data ID 
        may have overwritten what was set by the user with SW2 & 3.
        Also RESETS any ERRORS.
*****************************************************************************/
void Sw3Func(void)
{
    uint32_t rxid = rx_dataframe.id; 
        
  DisplayString(LCD_LINE1,"Rx ID:  ");
  DisplayString(LCD_LINE2,"        ");
    
    if (FRAME_ID_MODE == STD_ID_MODE)
    {
      LcdShow4DigHex((uint8_t)(rxid >> 8), (uint8_t)rxid, 16);         
    }
    else
    {
      /* Showing all 4 bytes of ID variable in order to support extended ID. */
      LcdShow4DigHex((uint8_t)(rxid >> 24), (uint8_t)(rxid >> 16), 16);
      LcdShow4DigHex((uint8_t)(rxid >> 8), (uint8_t)rxid, 20);  
    }

    Delay(0x200000);    

    //ResetAllErrors();
}/* end Sw3Func() */

/*****************************************************************************
Name:            Sw23Func
Parameters:      -
Returns:         -
Description:     2 then 3 pressed. Increments CAN TX-ID.
*****************************************************************************/
void Sw23Func(void)
{
    /* Inc. transmit ID */
    tx_dataframe.id++;
    if (FRAME_ID_MODE == STD_ID_MODE)
    {
        if (tx_dataframe.id > 0x7FF)
      {
            tx_dataframe.id = 0x01;
        }            
    }
    else
    {   /* Extended or Mixed ID mode. */
        if (tx_dataframe.id > 0x1FFFFFFF)
      {
            tx_dataframe.id = 0x01;
        }            
    }    
    Sw2Func();
}/* end Sw23Func() */

/*****************************************************************************
Name:            Sw32Func
Parameters:      -
Returns:         -
Description:     3 then 2 pressed. Increments CAN RX-ID.
*****************************************************************************/
void Sw32Func(void)
{
    /* When adjusting rx id, stop all transmission 
    abort_trm_can(CANBOX_TX);   */

  /* If InvalData is flagged we are still receiving. Skip and come back. */
    if (CAN1.MCTL[CANBOX_RX].BIT.RX.INVALDATA == 0 )
  {
        /* Stop receiving */
        CAN1.MCTL[CANBOX_RX].BYTE = 0;
    
        rx_dataframe.id++;        /* Increment receive ID */

        if (FRAME_ID_MODE == STD_ID_MODE)
        {
            if (rx_dataframe.id > 0x7FF)
          {
                rx_dataframe.id = 0x01;
            }            
        }
        else
        {   /* Extended or Mixed ID mode. */
            if (rx_dataframe.id > 0x1FFFFFFF)
          {
                rx_dataframe.id = 0x01;
            }            
        } 

        /* Set new ID and start receiving */
        if (FRAME_ID_MODE == STD_ID_MODE)
        {    
            R_CAN_RxSet(g_can_channel, CANBOX_RX, rx_dataframe.id, DATA_FRAME);
        }
        else
        {   /* Extended or Mixed ID mode. */
            R_CAN_RxSetXid(g_can_channel, CANBOX_RX, rx_dataframe.id, DATA_FRAME);            
        } 
        Sw3Func();
    }
}/* end Sw32Func() */

/*****************************************************************************
Name:          SwNoneFunc
Parameters:    -
Returns:       -
Description:   This function will run if no switch is pressed, or there is 
               no desirable switchfunction to run. Does nothing.
*****************************************************************************/
void SwNoneFunc(void)
{
    nop();
}

/* eof */

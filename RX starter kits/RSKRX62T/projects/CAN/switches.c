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
* File Name    : switches.c
* Version     : 2.00
* Device     : R5F562TAADFP
* Tool Chain   : IAR Embedded Workbench
* H/W Platform  : RSKRX62T
* Description   : Provides function definition prototypes used in switches.c 
*******************************************************************************/
/*******************************************************************************
* History     : 15.10.2010 Ver. 1.00 First Release
              : 21.05.2011 Ver. 2.00 Second Release (Removed RPDL)
*******************************************************************************/
 
/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <machine.h>
/* Following header file provides a structure to access on-chip I/O registers.*/
#include <iorx62t.h>
#include "rskrx62tdef.h"
#include "api_demo.h"
#include "r_can_api.h"
#include "switches.h"
#include "lcd.h"

/*******************************************************************************
Typedef definitions
*******************************************************************************/
/*******************************************************************************
Macro definitions
*******************************************************************************/
/*******************************************************************************
User Defines
*******************************************************************************/
/* SWITCHES. SW1 is pE_5, SW2 is pE_4, SW3 is pB_4. */
#define     RSK_RX62T_SW1_RELEASED  PORTE.PORT.BIT.B5 /* Switch 1 */
#define     RSK_RX62T_SW2_RELEASED  PORTE.PORT.BIT.B4 /* Switch 2 */
#define     RSK_RX62T_SW3_RELEASED  PORTB.PORT.BIT.B4 /* Switch 3 */

/* logical switch combination byte patterns */
#define     RSK_RX62T_SW1_MASK    0x01//0b00000001  /* Switch 1 mask */
#define     RSK_RX62T_SW2_MASK    0x02//0b00000010  /* Switch 2 mask */
#define     RSK_RX62T_SW3_MASK    0x04//0b00000100  /* Switch 3 mask */
#define     RSK_RX62T_SW23_MASK    0x06//0b00000110  /* Switch 2 & 3 mask */
#define     RSK_RX62T_SW12_MASK    0x03//0b00000011  /* Switch 1 & 2 mask */
#define     RSK_RX62T_SW123_MASK   0x00//0b00000000  /* All Switches mask */
#define     RSK_RX62T_SW_NONE    0x07//0b00000111  /* No Switch */

#define     RSK_RX62T_SW1        0x06//0b00000110  /* Switch 1 pressed only */
#define     RSK_RX62T_SW2      0x05//0b00000101  /* Switch 2 pressed only */
#define     RSK_RX62T_SW3      0x03//0b00000011  /* Switch 3 pressed only */
#define     RSK_RX62T_SW23      0x01//0b00000001  /* Switch 3 & 2 pressed only */

/* Nr times switch-poll must match */
#define     SW_COUNT        2
/* Switch func. table */
#define     SW_TBL_SIZE     6

/*******************************************************************************
Global variables and functions imported (externs)
*******************************************************************************/
/*******************************************************************************
Global variables and functions private to the file
*******************************************************************************/
/* Last switch data */
uint8_t last_sw_data;     
/* Fixed switch data */
uint8_t fix_sw_data;
/* Last fixed switch data */          
uint8_t last_fix_sw_data;   
/* Switch counter */  
uint8_t sw_count;             


/* Function prototypes */
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

/*******************************************************************************
User Program Code
*******************************************************************************/

/*******************************************************************************
* Name        : ReadSwitches
* Description : Switch function
* Arguments   : none
* Returns     : none
*******************************************************************************/
void ReadSwitches(void)
{
    uint8_t i, current_sw_pos = 0;

  /* Build a switch byte pattern */
  current_sw_pos = 0x07;               /* initialize all switch bits to 1 */
  
  if(!RSK_RX62T_SW1_RELEASED)
  {
    current_sw_pos &= RSK_RX62T_SW1;  /* SW1 is pressed. Clear SW1 bit */
  }

  if(!RSK_RX62T_SW2_RELEASED)
  {
    current_sw_pos &= RSK_RX62T_SW2;  /* SW2 is pressed. Clear SW2 bit */
  }

  if(!RSK_RX62T_SW3_RELEASED)
  {
    current_sw_pos &= RSK_RX62T_SW3;  /* SW3 is pressed. Clear SW3 bit */  
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
/*******************************************************************************
* End of function ReadSwitches
*******************************************************************************/  

/*******************************************************************************
* Outline     : AreSwitchesStable
* Description : Fix switch function, if ReadSwitches are stable, then copy value
* Arguments   : Current switch 1-3 positions.
* Returns     : switches_stable. 1 = stable change. 0 - no stable change.
*******************************************************************************/
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
}
/*******************************************************************************
* End of function AreSwitchesStable
*******************************************************************************/  

/*******************************************************************************
* Name        : SwitchDecode
* Description : Decodes which switch has been pressed
* Arguments   : none
* Returns     : uint8_t switch_func_nr
*******************************************************************************/
uint8_t SwitchDecode(void)
{
    uint8_t switch_func_nr = 0;
    
    switch (fix_sw_data)
  {
        /* No switch */
    case RSK_RX62T_SW_NONE:
            switch_func_nr = 0;
            break;

        /* SW1 */
    case RSK_RX62T_SW1:
            if (last_fix_sw_data == RSK_RX62T_SW_NONE)
                switch_func_nr = 1;
            else
                switch_func_nr = 0;
            break;

        /* SW2 */
    case RSK_RX62T_SW2:
            /* Don't call Switch 2 again after '23' until user has realeased SW2 */
            if (last_fix_sw_data == RSK_RX62T_SW_NONE)
              switch_func_nr = 2;
      else if (last_fix_sw_data == RSK_RX62T_SW23)
                switch_func_nr = 0; /* SwDummyFunc */
            else
                switch_func_nr = 0;
            break;
        /* SW3 */
    case RSK_RX62T_SW3:
            /* Don't call Switch 3 after '32' again until user has realeased SW3 */
            if (last_fix_sw_data == RSK_RX62T_SW_NONE)
              switch_func_nr = 3;
      else if (last_fix_sw_data == RSK_RX62T_SW23)
                switch_func_nr = 0; /* SwDummyFunc */
            else
                switch_func_nr = 0;
            break;
    /* SW1 & SW2 */
    case RSK_RX62T_SW23:
          /* SW1 --> SW2 */
            if (last_fix_sw_data == RSK_RX62T_SW3)
                switch_func_nr = 4;
      /* SW2 --> SW1 */
        else if (last_fix_sw_data == RSK_RX62T_SW2)
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
}
/*******************************************************************************
* End of function SwitchDecode
*******************************************************************************/  

/*******************************************************************************
* Outline        : Sw1Func
* Description    : Switch 1. Sends a test CAN frame.
*             A-D Demo.
* Arguments      : none
* Returns        : none
*******************************************************************************/
void Sw1Func(void)
{
    /* Inc. 1st databyte for show. */
  tx_dataframe.data[0]++;
  
  R_CAN_TxSet(0, CANBOX_TX, &tx_dataframe, DATA_FRAME);
}
/*******************************************************************************
* End of function Sw1Func
*******************************************************************************/  

/*******************************************************************************
* Outline     : Sw2Func
* Description : Switch 2. Shows test frame CAN TxID. 
* Arguments   : none
* Returns     : non 
*******************************************************************************/
void Sw2Func(void)
{
  DisplayString(LCD_LINE1,"Tx ID:  ");
    DisplayString(LCD_LINE2,"        ");

  /* Supposing id is under 0xFF */
  LcdShow4DigHex( 0, (uint8_t)(tx_dataframe.id), 16);

    Delay(0x200000);
}
/*******************************************************************************
* End of function Sw2Func
*******************************************************************************/  

/*******************************************************************************
* Outline     :  Sw3Func
* Description : Switch 3. Shows test frame CAN RxID. 
*        If you are using a CAN filter mask, the incoming data ID 
*        may have overwritten what was set by the user with SW2 & 3.
*        Also RESETS any ERRORS.
* Arguments   : none
* Returns     : none
*******************************************************************************/
void Sw3Func(void)
{
  DisplayString(LCD_LINE1,"Rx ID:  ");

    /* Supposing for now that id is under 0xFF */
  DisplayString(LCD_LINE2,"        ");
    
    LcdShow4DigHex( 0, (uint8_t)(rx_dataframe.id), 16);
    Delay(0x200000);
}
/*******************************************************************************
* End of function Sw3Func
*******************************************************************************/  

/*******************************************************************************
* Outline      : Sw23Func
* Description  : 2 then 3 pressed. Increments CAN TX-ID.
* Arguments    : none
* Returns      : none
*******************************************************************************/
void Sw23Func(void)
{
    /* Inc. transmit ID */
    tx_dataframe.id++;
    if (tx_dataframe.id > 0x0F)
  {
        tx_dataframe.id = 0x01;
    }
    Sw2Func();
}
/*******************************************************************************
* End of function Sw23Func
*******************************************************************************/  

/*******************************************************************************
* Outline     : Sw32Func
* Description : 3 then 2 pressed. Increments CAN RX-ID.
* Arguments   : none
* Returns     : none    
*******************************************************************************/
void Sw32Func(void)
{
    /* When adjusting rx id, stop all transmission 
    abort_trm_can(CANBOX_TX);   */

  /* If InvalData is flagged we are still receiving. Skip and come back. */
    if (CAN0.MCTL[CANBOX_RX].BIT.RX.INVALDATA == 0 )
  {
        /* Stop receiving */
        CAN0.MCTL[CANBOX_RX].BYTE = 0;
    
        rx_dataframe.id++;        /* Increment receive ID */

        if (rx_dataframe.id > 0x0F)
        {
            rx_dataframe.id = 0x01;
    }

        /* Set new ID and start receiving */
    R_CAN_RxSet(0, CANBOX_RX, rx_dataframe.id, DATA_FRAME);
 
        Sw3Func();
    }
}
/*******************************************************************************
* End of function Sw32Func
*******************************************************************************/  

/*******************************************************************************
* Outline     : SwNoneFunc
* Description : This function will run if no switch is pressed, or there is 
*               no desirable switchfunction to run. Does nothing.
* Arguments   : none
* Returns     : none
*******************************************************************************/
void SwNoneFunc(void)
{
    nop();
}
/*******************************************************************************
* End of function SwNoneFunc
*******************************************************************************/  



/*******************************************************************************
* DISCLAIMER

* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products.

* No other uses are authorized.
*
* This software is owned by Renesas Electronics Corp. and is protected under
* all applicable laws, including copyright laws.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
*
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name     : switches.c
* Version       : 1.0
* Device(s)     : R5F563NB
* Tool-Chain    : Renesas RX Standard Toolchain 1.1.0.0
* OS            : None
* H/W Platform  : RSK+RX63N
* Description   : Sets up push button switches and provides ISR code.
*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 01.10.2011     1.00        First release
*         : 12.01.11                   Functions added from RSK630 project.
*******************************************************************************/

/******************************************************************************
Includes <System Includes> , �Project Includes�
******************************************************************************/
/* Defines standard type structures */
#include <stdint.h>
/* Defines boolean type structures */
#include <stdbool.h>
/* Defines Special Flag Registers (SFR)s for RX63N */
#include "iorx63t_clr.h"
#include "switches.h"
#include "api_demo.h"
#include "lcd.h"
#include <machine.h>

/******************************************************************************
Macro definitions
******************************************************************************/
/* Pin configuration defines */
#define SW1_PIDR        (PORT1.PIDR.BIT.B0)
#define SW2_PIDR        (PORT1.PIDR.BIT.B1)
#define SW3_PIDR        (PORTE.PIDR.BIT.B3)

#define SW1_PMR         (PORT1.PMR.BIT.B0)
#define SW2_PMR         (PORT1.PMR.BIT.B1)
#define SW3_PMR         (PORTE.PMR.BIT.B3)

#define SW1_PDR         (PORT1.PDR.BIT.B0)
#define SW2_PDR         (PORT1.PDR.BIT.B1)
#define SW3_PDR         (PORTE.PDR.BIT.B3)

#define SWITCH_IRQ_PRIORITY     (1)

//Added CAN
/* SWITCHES. Low level = pressed*/
/* logical switch combination byte patterns */
#define     SW1_MASK        0x01//0b00000001  /* Switch 1 mask */
#define     SW2_MASK        0x02//0b00000010  /* Switch 2 mask */
#define     SW3_MASK        0x04//0b00000100  /* Switch 3 mask */
#define     SW23_MASK        0x06//0b00000110  /* Switch 2 & 3 mask */
#define     SW12_MASK        0x03//0b00000011  /* Switch 1 & 2 mask */
#define     SW123_MASK         0x00//0b00000000  /* All Switches mask */
#define     SW_NONE            0x07//0b00000111  /* No Switch */

#define     SW1_PRESSED           0x06//0b00000110  /* Switch 1 pressed only */
#define     SW2_PRESSED        0x05//0b00000101  /* Switch 2 pressed only */
#define     SW3_PRESSED        0x03//0b00000011  /* Switch 3 pressed only */
#define     SW23_PRESSED    0x01//0b00000001  /* Switch 3 & 2 pressed only */

/* Nr times switch-poll must match */
#define     SW_COUNT          2
/* Switch func. table */
#define     SW_TBL_SIZE     6

/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
uint32_t g_sw1;
uint32_t g_sw2;

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Data */
static uint8_t last_sw_data;         /* Last switch data */
static uint8_t fix_sw_data;          /* Fixed switch data */
static uint8_t last_fix_sw_data;     /* Last fixed switch data */
static uint8_t sw_count;             /* Switch counter */

/* Functions */
static void switches_setup_sw1(void);
static void switches_setup_sw2(void);
static void switches_setup_sw3(void);

static uint8_t AreSwitchesStable(uint8_t);
static uint8_t SwitchDecode(void);
static void SwNoneFunc(void);
static void Sw1Func(void);
static void Sw2Func(void);
static void Sw3Func(void);
static void Sw23Func(void);
static void Sw32Func(void);

/* An array of constant pointers to the switch functions */
static void (* const SwitchFuncTable[SW_TBL_SIZE])(void) = {   SwNoneFunc,
                                                                Sw1Func,
                                                                Sw2Func,
                                                                Sw3Func,
                                                                Sw32Func,
                                                                Sw23Func   };

/******************************************************************************
Global variables and functions imported (externs)
******************************************************************************/
extern uint32_t g_can_channel;


/******************************************************************************
* Function Name: switches_initialize
* Description  : Sets up globals and initializes push button switches.
* Arguments    : None
* Return Value : None
******************************************************************************/
void switches_initialize(void)
{
    /* Globals are set to not pressed */
    g_sw1 = SWITCH_NOT_PRESSED;
    g_sw2 = SWITCH_NOT_PRESSED;

    /* Call each switch setup function */
    switches_setup_sw1();
    switches_setup_sw2();
    switches_setup_sw3();
}

/******************************************************************************
* Function Name: switches_setup_sw1
* Description  : Sets up the port pin for SW1 and interrupt for switch press
* Arguments    : None
* Return Value : None
******************************************************************************/
void switches_setup_sw1(void)
{
    /* Enable writes to Pin Function Select (PFS) register */
    MPC.PWPR.BIT.B0WI = 0;                  // Enable write to PFSWE
    MPC.PWPR.BIT.PFSWE = 1;                 // Enable write to PFS

    /* Switch 1 is on P10 and used as IRQ0 */
    MPC.P10PFS.BIT.ISEL = 1;

    /* Set as I/O mode. Do not need to set as a peripheral for IRQ2. */
    SW1_PMR = 0;
    SW1_PDR = 0;

    /* Set IRQ0 sense and digital filter */
    ICU.IRQCR[0].BIT.IRQMD = 0;             // Low level
    ICU.IRQFLTC0.BIT.FCLKSEL0 = 3;          // Set digital filter sample to PCLK/64
    ICU.IRQFLTE0.BIT.FLTEN0 = 1;            // Enable digital filter

    /* Clear interrupt before enable it */
    IR(ICU, IRQ0) = 0;

    #if 0
    /* Enable interrupt */
    IPR(ICU, IRQ10) = SWITCH_IRQ_PRIORITY;   // Set interrupt level
    IEN(ICU, IRQ10) = 1;                     // Enable interrupt
    #endif

    /* Disable writes to PFS */
    MPC.PWPR.BIT.PFSWE = 0;                 // Disable write to PFS
    MPC.PWPR.BIT.B0WI = 1;                  // Disable write to PFSWE
}


/******************************************************************************
* Function Name: switches_setup_sw2
* Description  : Sets up the port pin for SW2 and interrupt for switch press
* Arguments    : None
* Return Value : None
******************************************************************************/
void switches_setup_sw2(void)
{
    /* Enable writes to Pin Function Select (PFS) register */
    MPC.PWPR.BIT.B0WI = 0;                  // Enable write to PFSWE
    MPC.PWPR.BIT.PFSWE = 1;                 // Enable write to PFS

    /* Switch 1 is on P11 and used as IRQ1 */
    MPC.P11PFS.BIT.ISEL = 1;

    /* Set as I/O mode. Do not need to set as a peripheral for IRQ8. */
    SW2_PMR = 0;
    SW2_PDR = 0;

    /* Set IRQ1 sense and digital filter */
    ICU.IRQCR[1].BIT.IRQMD = 0;             // Low level
    ICU.IRQFLTC0.BIT.FCLKSEL1 = 3;          // Set digital filter sample to PCLK/64
    ICU.IRQFLTE0.BIT.FLTEN1 = 1;            // Enable digital filter

    /* Clear interrupt before enable it */
    IR(ICU, IRQ1) = 0;

    #if 0
    /* Enable interrupt */
    IPR(ICU, IRQ8) = SWITCH_IRQ_PRIORITY;   // Set interrupt level
    IEN(ICU, IRQ8) = 1;                     // Enable interrupt
    #endif

    /* Disable writes to PFS */
    MPC.PWPR.BIT.PFSWE = 0;                 // Disable write to PFS
    MPC.PWPR.BIT.B0WI = 1;                  // Disable write to PFSWE
}


/******************************************************************************
* Function Name: switches_setup_sw3
* Description  : Sets up the port pin for SW3
* Arguments    : None
* Return Value : None
******************************************************************************/
void switches_setup_sw3(void)
{
    /* Switch 3 is an input pin */
    SW3_PMR = 0;
    SW3_PDR = 0;
}

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

    if (!SW1_PIDR)
    {
        current_sw_pos &= SW1_PRESSED;    /* SW1 is pressed. Clear SW1 bit */
    }

    if (!SW2_PIDR)
    {
        current_sw_pos &= SW2_PRESSED;    /* SW2 is pressed. Clear SW2 bit */
    }

    if (!SW3_PIDR)
    {
        current_sw_pos &= SW3_PRESSED;    /* SW3 is pressed. Clear SW3 bit */
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
static void Sw1Func(void)
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
static void Sw2Func(void)
{
    uint32_t txid = (tx_dataframe.id & 0x1FFFFFFF);     /* Keep only extended ID bits. */

    Display_LCD(LCD_LINE1,"Tx ID:  ");
    Display_LCD(LCD_LINE2,"        ");

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
Name:              Sw3Func
Parameters:        -
Returns:           -
Description:       Switch 3. Shows test frame CAN RxID.
                If you are using a CAN filter mask, the incoming data ID
                may have overwritten what was set by the user with SW2 & 3.
                Also RESETS any ERRORS.
*****************************************************************************/
static void Sw3Func(void)
{
    uint32_t rxid = rx_dataframe.id;

    Display_LCD(LCD_LINE1,"Rx ID:  ");
    Display_LCD(LCD_LINE2,"        ");

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

}/* end Sw3Func() */

/*****************************************************************************
Name:              Sw23Func
Parameters:        -
Returns:           -
Description:       2 then 3 pressed. Increments CAN TX-ID.
*****************************************************************************/
static void Sw23Func(void)
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
Name:              Sw32Func
Parameters:        -
Returns:           -
Description:       3 then 2 pressed. Increments CAN RX-ID.
*****************************************************************************/
static void Sw32Func(void)
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
static void SwNoneFunc(void)
{
    nop();
}
/* file end */

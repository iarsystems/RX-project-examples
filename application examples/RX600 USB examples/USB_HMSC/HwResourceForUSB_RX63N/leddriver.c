/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is  protected
* under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR  A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE  EXPRESSLY
* DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2010(2011) Renesas Electronics Corpration
* and Renesas Solutions Corp. All rights reserved.
*******************************************************************************
* File Name     : leddriver.c
* Version       : 2.00
* Device(s)     : RX-Series
* Tool-Chain    : Renesas RX Standard Toolchain
* OS            : Common to None and uITRON 4.0 Spec
* H/W Platform  : Independent
* Description   : RX RSK LED Lighting processing
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 27.07.2012 2.00    First Release
******************************************************************************/


/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include <iorx63n.h>
#include "hw_resource.h"

#ifdef  USB_LED_ENABLE
/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_rx_rsk"

/******************************************************************************
Private global variables and functions
******************************************************************************/
void        usb_cpu_LedInit(void);
void        usb_cpu_LedReinit(void);
void        usb_cpu_LedSet(uint8_t data);
void        usb_cpu_LedSetBit(uint8_t bit, uint8_t data);

/******************************************************************************
Renesas Abstracted RSK functions
******************************************************************************/

/******************************************************************************
LED function
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_LedInit
Description     : LED Initialize
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_LedInit(void)
{
    /* Data direction register
    b0 B0 Pn0 in/out select bit
    b1 B1 Pn1 in/out select bit
    b2 B2 Pn2 in/out select bit
    b3 B3 Pn3 in/out select bit
    b4 B4 Pn4 in/out select bit
    b5 B5 Pn5 in/out select bit
    b6 B6 Pn6 in/out select bit
    b7 B7 Pn7 in/out select bit
    */
    PORT0.PDR.BIT.B3    = 1;            /* LED0 */
    PORT0.PDR.BIT.B5    = 1;            /* LED1 */
    PORT1.PDR.BIT.B0    = 1;            /* LED2 */
    PORT1.PDR.BIT.B1    = 1;            /* LED3 */

    /* Port output data register (LED off)
    b0 B0 Pn0 output data bit
    b1 B1 Pn1 output data bit
    b2 B2 Pn2 output data bit
    b3 B3 Pn3 output data bit
    b4 B4 Pn4 output data bit
    b5 B5 Pn5 output data bit
    b6 B6 Pn6 output data bit
    b7 B7 Pn7 output data bit
    */
    PORT0.PODR.BIT.B3   = 1;            /* LED0 */
    PORT0.PODR.BIT.B5   = 1;            /* LED1 */
    PORT1.PODR.BIT.B0   = 1;            /* LED2 */
    PORT1.PODR.BIT.B1   = 1;            /* LED3 */
}
/******************************************************************************
End of function usb_cpu_LedInit
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_LedReinit
Description     : LED Initialize
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_LedReinit(void)
{
    usb_cpu_LedSet((uint8_t)0);

    /* Data direction register
    b0 B0 Pn0 in/out select bit
    b1 B1 Pn1 in/out select bit
    b2 B2 Pn2 in/out select bit
    b3 B3 Pn3 in/out select bit
    b4 B4 Pn4 in/out select bit
    b5 B5 Pn5 in/out select bit
    b6 B6 Pn6 in/out select bit
    b7 B7 Pn7 in/out select bit
    */
    PORT0.PDR.BIT.B3    = 1;            /* LED0 */
    PORT0.PDR.BIT.B5    = 1;            /* LED1 */
    PORT1.PDR.BIT.B0    = 1;            /* LED2 */
    PORT1.PDR.BIT.B1    = 1;            /* LED3 */
}
/******************************************************************************
End of function usb_cpu_LedReinit
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_LedSet
Description     : LED Set process
Arguments       : uint8_t data          : LED Set data
Return value    : none
******************************************************************************/
void usb_cpu_LedSet(uint8_t data)
{
    usb_cpu_LedSetBit((uint8_t)0, (data & 0x01));   /* LED0 */
    usb_cpu_LedSetBit((uint8_t)1, (data & 0x02));   /* LED1 */
    usb_cpu_LedSetBit((uint8_t)2, (data & 0x04));   /* LED2 */
    usb_cpu_LedSetBit((uint8_t)3, (data & 0x08));   /* LED3 */
}
/******************************************************************************
End of function usb_cpu_LedSet
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_LedSetBit
Description     : LED Bit Set process
Arguments       : uint8_t bit          : Led Set Position
                : uint8_t data         : Led On/Off
Return value    : none
******************************************************************************/
void usb_cpu_LedSetBit(uint8_t bit, uint8_t data)
{
    uint8_t dat;

    dat = 1;                                    /* OFF */
    if( data != 0 )
    {
        dat = 0;                                /* ON */
    }

    switch( bit )
    {
    /* Data register (LED off)
    b0 B0 Pn0 output data bit
    b1 B1 Pn1 output data bit
    b2 B2 Pn2 output data bit
    b3 B3 Pn3 output data bit
    b4 B4 Pn4 output data bit
    b5 B5 Pn5 output data bit
    b6 B6 Pn6 output data bit
    b7 B7 Pn7 output data bit
    */
    case 0: PORT0.PODR.BIT.B3  = dat;   break;  /* LED0 */
    case 1: PORT0.PODR.BIT.B5  = dat;   break;  /* LED1 */
    case 2: PORT1.PODR.BIT.B0  = dat;   break;  /* LED2 */
    case 3: PORT1.PODR.BIT.B1  = dat;   break;  /* LED3 */
    default:                            break;
    }
}
/******************************************************************************
End of function usb_cpu_LedSetBit
******************************************************************************/
#endif /* USB_LED_ENABLE */

/******************************************************************************
End  Of File
******************************************************************************/

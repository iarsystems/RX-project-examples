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
* File Name     : keydriver.c
* Version       : 2.00
* Device(s)     : RX-Series
* Tool-Chain    : Renesas RX Standard Toolchain
* OS            : Common to None and uITRON 4.0 Spec
* H/W Platform  : Independent
* Description   : RX RSK Switch input processing
******************************************************************************
* History : DD.MM.YYYY Version Description
*        : 27.07.2012 2.00  First Release
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"         /* Type define */
#include "r_usb_cdefusbip.h"        /* USB-FW Library Header */
#include "r_usb_cmacprint.h"        /* Standard IO macro */
#include "r_usb_cextern.h"          /* USB-FW global define */
#include <iorx63n.h>
#include "hw_resource.h"

#ifdef USB_KEY_ENABLE
/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_rx_rsk"

/******************************************************************************
Private global variables and functions
******************************************************************************/
/* global variable prototype definition */
uint16_t    usb_gcstd_KeyTime[2];
uint8_t     usb_gcstd_KeyBuff[4];

/*=== KEY ===================================================================*/
void        usb_cpu_KeyInit(void);
uint16_t    usb_cpu_KeyWait(void);
uint16_t    usb_cpu_GetKeyNo(void);
void        usb_cpu_AcquisitionKeyNo(void);
uint8_t     usb_cpu_KeepKeyNo(void);
uint8_t     usb_cpu_SingleKeyNo(void);

/******************************************************************************
Renesas Abstracted RSK functions
******************************************************************************/

/******************************************************************************
KEY function
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_KeyInit
Description     : Input key port initialize
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_KeyInit(void)
{
    /* Write protect register
    b5-b0 Reserved 0
    b6 PFSWE    PFS register write enable bit
    b7 BOWI     PFSWE bit write disable bit
    */
    MPC.PWPR.BIT.B0WI   = 0u;
    MPC.PWPR.BIT.PFSWE  = 1u;

    /* Port Pin function select register  
    b4-b0 PSEL  Pin function select bit
    b5 Reserved 0
    b6 ISEL     Interrupt input function select bit
    b7 Reserved 0
    */
    MPC.P32PFS.BIT.ISEL = 1;    /* IRQ2 use (SW1) */
    MPC.P00PFS.BIT.ISEL = 1;    /* IRQ8 use (SW2) */

    /* Write protect register
    b5-b0 Reserved 0
    b6 PFSWE    PFS register write enable bit
    b7 BOWI     PFSWE bit write disable bit
    */
    MPC.PWPR.BIT.PFSWE  = 0u;
    MPC.PWPR.BIT.B0WI   = 1u;
}
/******************************************************************************
End of function usb_cpu_KeyInit
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_KeyWait
Description     : wait input key
Arguments       : none
Return value    : uint16_t              : key_no
Note            : Please change for your MCU
******************************************************************************/
uint16_t usb_cpu_KeyWait(void)
{
    uint16_t        key_data;

    USB_PRINTF0("\n--- Please push any main key ---");
    while( 1 )
    {
        /* 1ms wait */
        usb_cpu_DelayXms(1);
        key_data = usb_cpu_GetKeyNo();      /* SW */
        if( key_data != 0 )
        {
            break;
        }
    }
    USB_PRINTF0("\n");
    return key_data;    /* SW no */
}
/******************************************************************************
End of function usb_cpu_KeyWait
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_GetKeyNo
Description     : input key port
Arguments       : none
Return value    : uint16_t              : key_no
******************************************************************************/
uint16_t usb_cpu_GetKeyNo(void)
{
    uint8_t     key_buf, key_buf1;

    key_buf = 0;

    /* Port input register
    b0 B0 Pn0 bit
    b1 B1 Pn1 bit
    b2 B2 Pn2 bit
    b3 B3 Pn3 bit
    b4 B4 Pn4 bit
    b5 B5 Pn5 bit
    b6 B6 Pn6 bit
    b7 B7 Pn7 bit
    */
    if( PORT3.PIDR.BIT.B2 == 0)
    {
        key_buf |= 0x01;    /* SW1 */
    }
    if( PORT0.PIDR.BIT.B0 == 0 )
    {
        key_buf |= 0x02;    /* SW2 */
    }
    if( PORT0.PIDR.BIT.B7 == 0 )
    {
        key_buf |= 0x04;    /* SW3 */
    }

    if( key_buf != 0 )
    {
        do
        {
            key_buf1 = 0;   /* Port read */
            
            /* Port register
            b0 B0 Pn0 bit
            b1 B1 Pn1 bit
            b2 B2 Pn2 bit
            b3 B3 Pn3 bit
            b4 B4 Pn4 bit
            b5 B5 Pn5 bit
            b6 B6 Pn6 bit
            b7 B7 Pn7 bit
            */
            if( PORT3.PIDR.BIT.B2 == 0)
            {
                key_buf1 |= 0x01;   /* SW1 */
            }
            if( PORT0.PIDR.BIT.B0 == 0 )
            {
                key_buf1 |= 0x02;   /* SW2 */
            }
            if( PORT0.PIDR.BIT.B7 == 0 )
            {
                key_buf1 |= 0x04;   /* SW3 */
            }

            usb_cpu_DelayXms(100);  /* 100ms wait */

            key_buf = 0;    /* Port read */

            /* Port register
            b0 B0 Pn0 bit
            b1 B1 Pn1 bit
            b2 B2 Pn2 bit
            b3 B3 Pn3 bit
            b4 B4 Pn4 bit
            b5 B5 Pn5 bit
            b6 B6 Pn6 bit
            b7 B7 Pn7 bit
            */
            if( PORT3.PIDR.BIT.B2 == 0)
            {
                key_buf |= 0x01;    /* SW1 */
            }
            if( PORT0.PIDR.BIT.B0 == 0 )
            {
                key_buf |= 0x02;    /* SW2 */
            }
            if( PORT0.PIDR.BIT.B7 == 0 )
            {
                key_buf |= 0x04;    /* SW3 */
            }
        }
        while( key_buf != 0 );
        return (uint16_t)key_buf1;  /* Port data */
    }
    else
    {
        return (uint16_t)0;
    }
}
/******************************************************************************
End of function usb_cpu_GetKeyNo
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_AcquisitionKeyNo
Description     : input key data
Arguments       : none
Return value    : none
Note            : Please change for your MCU
******************************************************************************/
void usb_cpu_AcquisitionKeyNo(void)
{
    usb_gcstd_KeyTime[0]++;
    switch(usb_gcstd_KeyTime[0])
    {
    case    10:
        usb_gcstd_KeyTime[0] = 0;   /* Initialized counter */
        usb_gcstd_KeyBuff[0] = usb_gcstd_KeyBuff[1];    /* Backup key data & input key data */

        usb_gcstd_KeyBuff[1] = 0;   /* Port read */
    
        /* Port register
        b0 B0 Pn0 bit
        b1 B1 Pn1 bit
        b2 B2 Pn2 bit
        b3 B3 Pn3 bit
        b4 B4 Pn4 bit
        b5 B5 Pn5 bit
        b6 B6 Pn6 bit
        b7 B7 Pn7 bit
        */
        if( PORT3.PIDR.BIT.B2 == 0)
        {
            usb_gcstd_KeyBuff[1] |= 0x01;   /* SW1 */
        }
        if( PORT0.PIDR.BIT.B0 == 0 )
        {
            usb_gcstd_KeyBuff[1] |= 0x02;   /* SW2 */
        }
        if( PORT0.PIDR.BIT.B7 == 0 )
        {
            usb_gcstd_KeyBuff[1] |= 0x04;   /* SW3 */
        }

        usb_gcstd_KeyBuff[2] = usb_gcstd_KeyBuff[3];    /* Backup key code & initialized key code*/
        usb_gcstd_KeyBuff[3] = 0;
        if( usb_gcstd_KeyBuff[1] != 0 )
        {
            /* Now press key */
            if( usb_gcstd_KeyBuff[0] == usb_gcstd_KeyBuff[1] )
            {
                usb_gcstd_KeyTime[1]++; /* To keep pushing the same key */
            }
            else
            {
                usb_gcstd_KeyTime[1]    = 0;    /* Another key was pushed. */
            }
            if( usb_gcstd_KeyTime[1] > 10 )
            {
                usb_gcstd_KeyBuff[3] = usb_gcstd_KeyBuff[0];    /* Encoding of key data */
            }
        }
        break;
    default:
        usb_gcstd_KeyBuff[2] = usb_gcstd_KeyBuff[3];    /* Backup key code & initialized key code*/
        break;
    }
}
/******************************************************************************
End of function usb_cpu_AcquisitionKeyNo
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_KeepKeyNo
Description     : input key port
Arguments       : none
Return value    : uint8_t              : key_no
******************************************************************************/
uint8_t usb_cpu_KeepKeyNo(void)
{
    return  usb_gcstd_KeyBuff[3];
}
/******************************************************************************
End of function usb_cpu_KeepKeyNo
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_SingleKeyNo
Description     : input key port
Arguments       : none
Return value    : uint8_t              : key_no
******************************************************************************/
uint8_t usb_cpu_SingleKeyNo(void)
{
    if( usb_gcstd_KeyBuff[2] == usb_gcstd_KeyBuff[3] )
    {
        return  (uint8_t)0;
    }
    else
    {
        return  usb_gcstd_KeyBuff[3];
    }
}
/******************************************************************************
End of function usb_cpu_SingleKeyNo
******************************************************************************/
#endif /* USB_KEY_ENABLE */

/******************************************************************************
End  Of File
******************************************************************************/

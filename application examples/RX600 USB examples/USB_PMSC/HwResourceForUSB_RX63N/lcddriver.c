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
* File Name     : lcddriver.c
* Version       : 2.00
* Device(s)     : RX-Series
* Tool-Chain    : Renesas RX Standard Toolchain
* OS            : Common to None and uITRON 4.0 Spec
* H/W Platform  : Independent
* Description   : RX RSK LCD Display processing
******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 27.07.2012 2.00    First Release
******************************************************************************/



/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
#include "r_usb_ctypedef.h"         /* Type define */
#include <iorx63n.h>
#include "hw_resource.h"

#ifdef  USB_LCD_ENABLE
/******************************************************************************
Constant macro definitions
******************************************************************************/

/* LCD */
#define USB_RS_PIN                      PORTJ.PODR.BIT.B5   /* XDrive */
#define USB_EN_PIN                      PORTF.PODR.BIT.B5   /* YDrive */
#define USB_DATA_PORT                   PORT8.PODR.BYTE     /* Data */

#define USB_DATA_PORT_MASK              0xF0
#define USB_DATA_PORT_SHIFT             4u

#define USB_SET_BIT_HIGH                1u      /* High bit */
#define USB_SET_BIT_LOW                 0u      /* Low  bit */
#define USB_DATA_WR                     1u      /* Data write */
#define USB_CTRL_WR                     0u      /* Command write */
#define USB_LCD_DISPLAY_ON              0x04    /* Display On */
#define USB_LCD_TWO_LINE                0x08    /* Line count */

#define USB_LCD_LINE2                   8u      /* Line2 offset */

#define USB_LCD_CLEAR                   0x01    /* Clear */
#define USB_LCD_HOME_L1                 0x80    /* Line1 Home */
#define USB_LCD_HOME_L2                 0xC0    /* Line2 Home */
#define USB_LCD_CURSOR_OFF              0x0C    /* Cursor off */


/******************************************************************************
Section    <Section Definition> , "Project Sections"
******************************************************************************/
#pragma section="_rx_rsk"

/******************************************************************************
Private global variables and functions
******************************************************************************/
void        usb_cpu_LcdInit(void);
void        usb_cpu_LcdDisp(uint8_t position, uint8_t *string);
void        usb_cpu_LcdWrite(uint8_t mode, uint8_t value);
void        usb_cpu_LcdNibbleWrite(uint8_t mode, uint8_t value);
void        usb_cpu_LcdWait(uint32_t units);
void        usb_cpu_TargetLcdClear(void);

/******************************************************************************
Renesas Abstracted RSK functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_TargetLcdClear
Description     : Target System Lcd Clear
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_TargetLcdClear(void)
{
}
/******************************************************************************
End of function usb_cpu_TargetLcdClear
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_LcdInit
Description     : LCD initialize
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_LcdInit(void)
{
    /* Data register
    b0 B0 Pn0 output data bit
    b1 B1 Pn1 output data bit
    b2 B2 Pn2 output data bit
    b3 B3 Pn3 output data bit
    b4 B4 Pn4 output data bit
    b5 B5 Pn5 output data bit
    b6 B6 Pn6 output data bit
    b7 B7 Pn7 output data bit
    */
    PORTJ.PODR.BIT.B5   = 0;
    PORTF.PODR.BIT.B5   = 0;

    /* Set LCD pins as outputs. */
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
    PORTJ.PDR.BIT.B5    = 1u;           /* Out LCD-RS */
    PORTF.PDR.BIT.B5    = 1;            /* Out LCD-EN */
    PORT8.PDR.BYTE      |= 0xF0;        /* Out LCD-data D20,D21,D22,D23 */

    /* Power Up Delay for LCD Module */
    /* Data register
    b0 B0 Pn0 output data bit
    b1 B1 Pn1 output data bit
    b2 B2 Pn2 output data bit
    b3 B3 Pn3 output data bit
    b4 B4 Pn4 output data bit
    b5 B5 Pn5 output data bit
    b6 B6 Pn6 output data bit
    b7 B7 Pn7 output data bit
    */
    USB_EN_PIN = USB_SET_BIT_HIGH;
    usb_cpu_LcdWait(7000);

    /* Data register
    b0 B0 Pn0 output data bit
    b1 B1 Pn1 output data bit
    b2 B2 Pn2 output data bit
    b3 B3 Pn3 output data bit
    b4 B4 Pn4 output data bit
    b5 B5 Pn5 output data bit
    b6 B6 Pn6 output data bit
    b7 B7 Pn7 output data bit
    */
    USB_EN_PIN = USB_SET_BIT_LOW;

    /* Display initializes in 8 bit mode
       so send one write (seen as 8 bit) to set to 4 bit mode. */
    /* Function Set */
    usb_cpu_LcdNibbleWrite(USB_CTRL_WR, 0x03);
    usb_cpu_LcdNibbleWrite(USB_CTRL_WR, 0x03);
    usb_cpu_LcdWait(39);
 
    /* Configure display */
    usb_cpu_LcdNibbleWrite(USB_CTRL_WR, 0x03);
    usb_cpu_LcdNibbleWrite(USB_CTRL_WR, 0x02);
    usb_cpu_LcdNibbleWrite(USB_CTRL_WR, (USB_LCD_DISPLAY_ON | USB_LCD_TWO_LINE ));
    usb_cpu_LcdNibbleWrite(USB_CTRL_WR, (USB_LCD_DISPLAY_ON | USB_LCD_TWO_LINE ));
    usb_cpu_LcdWait(39);

    /* Display ON/OFF control */
    usb_cpu_LcdWrite(USB_CTRL_WR, USB_LCD_CURSOR_OFF);
    usb_cpu_LcdWait(39);

    /* Display Clear */
    usb_cpu_LcdWrite(USB_CTRL_WR, USB_LCD_CLEAR);
    usb_cpu_LcdWait(1530);

    /* Entry Mode Set */
    usb_cpu_LcdWrite(USB_CTRL_WR, 0x06);
    usb_cpu_LcdWrite(USB_CTRL_WR, USB_LCD_HOME_L1);
}
/******************************************************************************
End of function usb_cpu_LcdInit
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_LcdDisp
Description     : LCD display string data
Arguments       : uint8_t pos           : Position
                : uint8_t *str          : Write data (string data)
Return value    : none
******************************************************************************/
void usb_cpu_LcdDisp(uint8_t pos, uint8_t *str)
{
    int     i;

    /* Set line position if needed. We don't want to if we don't need
       to because LCD control operations take longer than LCD data
       operations. */
    if( pos < USB_LCD_LINE2 )
    {
        /* Display on Line 1 */
        usb_cpu_LcdWrite(USB_CTRL_WR, (uint8_t)(USB_LCD_HOME_L1 + pos));
    }
    else
    {
        /* Display on Line 2 */
        usb_cpu_LcdWrite(USB_CTRL_WR, (uint8_t)(USB_LCD_HOME_L2 + pos - USB_LCD_LINE2));
    }

    for( i = 0; (i < 8) && (*str != 0); ++i )
    {
        usb_cpu_LcdWrite(USB_DATA_WR, *str++);
    }
}
/******************************************************************************
End of function usb_cpu_LcdDisp
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_LcdWrite
Description     : LCD write data        : Write mode (USB_CTRL_WR/USB_DATA_WR)
Arguments       : uint8_t data          : Write data (8bit data)
Return value    : none
******************************************************************************/
void usb_cpu_LcdWrite(uint8_t mode, uint8_t value)
{
    /* upper nibble first */
    usb_cpu_LcdNibbleWrite(mode, (value & 0xF0) >> 4);
    /* lower nibble second */
    usb_cpu_LcdNibbleWrite(mode, (value & 0x0F));
}
/******************************************************************************
End of function usb_cpu_LcdWrite
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_LcdNibbleWrite
Description     : LCD Nibble data write
Arguments       : uint8_t mode          : Write mode (USB_CTRL_WR/USB_DATA_WR)
                : uint8_t value         : Write data (4bit data)
Return value    : none
******************************************************************************/
void usb_cpu_LcdNibbleWrite(uint8_t mode, uint8_t value)
{
    uint8_t tmp;

    /* Set Register Select pin high for Data */
    if( mode == USB_DATA_WR )
    {
        /* Data register
        b0 B0 Pn0 output data bit
        b1 B1 Pn1 output data bit
        b2 B2 Pn2 output data bit
        b3 B3 Pn3 output data bit
        b4 B4 Pn4 output data bit
        b5 B5 Pn5 output data bit
        b6 B6 Pn6 output data bit
        b7 B7 Pn7 output data bit
        */
        USB_RS_PIN = USB_SET_BIT_HIGH;
    }
    else
    {
        /* Data register
        b0 B0 Pn0 output data bit
        b1 B1 Pn1 output data bit
        b2 B2 Pn2 output data bit
        b3 B3 Pn3 output data bit
        b4 B4 Pn4 output data bit
        b5 B5 Pn5 output data bit
        b6 B6 Pn6 output data bit
        b7 B7 Pn7 output data bit
        */
        USB_RS_PIN = USB_SET_BIT_LOW;
    }
    /* There must be 40ns between RS write and EN write */
    usb_cpu_LcdWait(5);

    /* EN enable chip (HIGH) */
    /* Data register
    b0 B0 Pn0 output data bit
    b1 B1 Pn1 output data bit
    b2 B2 Pn2 output data bit
    b3 B3 Pn3 output data bit
    b4 B4 Pn4 output data bit
    b5 B5 Pn5 output data bit
    b6 B6 Pn6 output data bit
    b7 B7 Pn7 output data bit
    */
    USB_EN_PIN = USB_SET_BIT_HIGH;

    /* Tiny delay */
    usb_cpu_LcdWait(5);

    /* Clear port bits used */
    /* Data register
    b0 B0 Pn0 output data bit
    b1 B1 Pn1 output data bit
    b2 B2 Pn2 output data bit
    b3 B3 Pn3 output data bit
    b4 B4 Pn4 output data bit
    b5 B5 Pn5 output data bit
    b6 B6 Pn6 output data bit
    b7 B7 Pn7 output data bit
    */
    tmp = USB_DATA_PORT;
    tmp &= ~USB_DATA_PORT_MASK;

    /* OR in data */
    tmp |= ((value << USB_DATA_PORT_SHIFT) & USB_DATA_PORT_MASK);

    /* Write data to port */
    /* Data register
    b0 B0 Pn0 output data bit
    b1 B1 Pn1 output data bit
    b2 B2 Pn2 output data bit
    b3 B3 Pn3 output data bit
    b4 B4 Pn4 output data bit
    b5 B5 Pn5 output data bit
    b6 B6 Pn6 output data bit
    b7 B7 Pn7 output data bit
    */
    USB_DATA_PORT = tmp;

    /* write delay while En High */
    usb_cpu_LcdWait(80);

    /* Latch data by dropping EN */
    /* Data register
    b0 B0 Pn0 output data bit
    b1 B1 Pn1 output data bit
    b2 B2 Pn2 output data bit
    b3 B3 Pn3 output data bit
    b4 B4 Pn4 output data bit
    b5 B5 Pn5 output data bit
    b6 B6 Pn6 output data bit
    b7 B7 Pn7 output data bit
    */
    USB_EN_PIN = USB_SET_BIT_LOW;

    /* Data hold delay */
    usb_cpu_LcdWait(80);
    if( mode == USB_CTRL_WR )
    {
        /* Extra delay needed for control writes */
        usb_cpu_LcdWait(0x7FF);
    }
}
/******************************************************************************
End of function usb_cpu_LcdNibbleWrite
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_LcdWait
Description     : Wait process
Arguments       : uint32_t units        : Wait time
Return value    : none
******************************************************************************/
void usb_cpu_LcdWait(uint32_t units)
{
    uint32_t    cnt;

    cnt = units * 15;

    while( cnt-- )
    {
        nop();          /* nop command */
    }
}
/******************************************************************************
End of function usb_cpu_LcdWait
******************************************************************************/
#endif /* USB_LCD_ENABLE */

/******************************************************************************
End  Of File
******************************************************************************/

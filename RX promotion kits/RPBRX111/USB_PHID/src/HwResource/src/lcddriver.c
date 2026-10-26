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
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : lcddriver_rx111.c
* Version       : x.xx
* Description   : This is the LCD driver function code.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : xx.xx.xxxx x.xx    Basic FW mini Ver2.00(ES Version)
*******************************************************************************/

/******************************************************************************
Section definitions
******************************************************************************/
#ifdef  __RX
#endif


/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition */
#include <iorx111.h>                   /* MCU IO definition */
#include "hw_resource.h"


#ifdef  USB_LCD_ENABLE
/*****************************************************************************
Macro definitions
******************************************************************************/
#define LCD_DATA                PORT4.PODR.BYTE
#define LCD_DATA_POS            0x0f
#define LCD_LINE2               8u      /* Line2 offset */
#define LCD_LINE3               16u     /* Line2 offset */
#define LCD_HOME_L1             0x80    /* Line1 home */
#define LCD_HOME_L2             0xC0    /* Line2 home */
#define LCD_CURSOR_OFF          0x0C    /* Cursor off */
#define LCD_CLEAR               0x01    /* Clear display */
#define LCD_ENTRY               0x06    /* Entry mode : I/D=1(inc), SH=0 */
#define LCD_DISPLAY_OFF         0x08    /* Display control : D=0, C=0, B=0 */
#define LCD_DISPLAY_ON          0x0C    /* Display control : D=1(display), C=0, B=0 */
#define LCD_CURSOL_ON           0x0E    /* Display control : D=1, C=1(cursol), B=0 */
#define LCD_BLINK_ON            0x0D    /* Display control : D=1, C=0, B=1(blink) */
#define LCD_FUNCTION            0x28    /* Function set : DL=0(4bit), N=1(2line), F=0(5*8dot) */

#define LCD_E_PORT_FUNCTION     PORTB.PDR.BIT.B1
#define LCD_RS_PORT_FUNCTION    PORTC.PDR.BIT.B5
#define LCD_D4_PORT_FUNCTION    PORT4.PDR.BIT.B0
#define LCD_D5_PORT_FUNCTION    PORT4.PDR.BIT.B1
#define LCD_D6_PORT_FUNCTION    PORT4.PDR.BIT.B2
#define LCD_D7_PORT_FUNCTION    PORT4.PDR.BIT.B3

#define LCD_E_PORT_DATA         PORTB.PODR.BIT.B1
#define LCD_RS_PORT_DATA        PORTC.PODR.BIT.B5
#define LCD_D4_PORT_DATA        PORT4.PODR.BIT.B0
#define LCD_D5_PORT_DATA        PORT4.PODR.BIT.B1
#define LCD_D6_PORT_DATA        PORT4.PODR.BIT.B2
#define LCD_D7_PORT_DATA        PORT4.PODR.BIT.B3


/******************************************************************************
Typedef definitions
******************************************************************************/
/* There is no typedef definition. */


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */

/* functions */
void        usb_cpu_LcdInitial(void);
void        usb_cpu_LcdDisp(uint8_t pos, uint8_t *str);
void        usb_cpu_LcdContDisp(uint8_t pos, uint8_t data);
void        usb_cpu_LcdRSClear(void);
void        usb_cpu_LcdResetFactor(void);


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */

/* functions */
void        usb_cpu_LcdNibbleWrite(uint8_t c);
void        usb_cpu_LcdWrite(uint8_t c);



/******************************************************************************
Renesas Abstracted LCD functions
******************************************************************************/

/******************************************************************************
Function Name   : usb_cpu_LcdInitial
Description     : LCD Initial
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_LcdInitial(void)
{
    /* Initialized port direction register */
    LCD_E_PORT_FUNCTION = PORT_OUT_MODE;    /* DLCDE  (PB1 = out) */
    LCD_RS_PORT_FUNCTION = PORT_OUT_MODE;   /* DLCDRS (PC5 = out) */
    LCD_D4_PORT_FUNCTION = PORT_OUT_MODE;   /* DLCDD4 (P40 = out) */
    LCD_D5_PORT_FUNCTION = PORT_OUT_MODE;   /* DLCDD5 (P41 = out) */
    LCD_D6_PORT_FUNCTION = PORT_OUT_MODE;   /* DLCDD6 (P42 = out) */
    LCD_D7_PORT_FUNCTION = PORT_OUT_MODE;   /* DLCDD7 (P43 = out) */

    /* Initializing software of LCM */
    /* BusyFlag can not be checked before this instruction.(4bit command) */
    usb_cpu_delay_xms(31);
    usb_cpu_LcdNibbleWrite(0x03);
    usb_cpu_LcdNibbleWrite(0x03);
    usb_cpu_delay_1us(10);
    usb_cpu_LcdNibbleWrite(0x03);
    usb_cpu_LcdNibbleWrite(0x02);

    /* BusyFlag can be checked after the following instructions.(8bit command) */
    usb_cpu_LcdWrite(LCD_FUNCTION);
    usb_cpu_delay_1us(40);
    usb_cpu_LcdWrite(LCD_DISPLAY_ON);
    usb_cpu_delay_1us(40);
    usb_cpu_LcdWrite(LCD_CLEAR);
    usb_cpu_delay_xms(2);
    usb_cpu_LcdWrite(LCD_ENTRY);
    usb_cpu_delay_1us(40);
    usb_cpu_LcdWrite(LCD_HOME_L1);
    usb_cpu_delay_xms(2);
}
/******************************************************************************
End of function
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_LcdNibbleWrite
Description     : LCD 4bit data set
Arguments       : uint8_t c                         : Low 4bit data
Return value    : none
******************************************************************************/
void usb_cpu_LcdNibbleWrite(uint8_t c)
{
    LCD_D4_PORT_DATA = PORT_LOW;
    LCD_D5_PORT_DATA = PORT_LOW;
    LCD_D6_PORT_DATA = PORT_LOW;
    LCD_D7_PORT_DATA = PORT_LOW;

    if((c & 0x01) == 0x01)
    {
        LCD_D4_PORT_DATA = PORT_HIGH;
    }
    if((c & 0x02) == 0x02)
    {
        LCD_D5_PORT_DATA = PORT_HIGH;
    }
    if((c & 0x04) == 0x04)
    {
        LCD_D6_PORT_DATA = PORT_HIGH;
    }
    if((c & 0x08) == 0x08)
    {
        LCD_D7_PORT_DATA = PORT_HIGH;
    }

    LCD_E_PORT_DATA = PORT_HIGH;
    usb_cpu_delay_1us(1);
    LCD_E_PORT_DATA = PORT_LOW;
    usb_cpu_delay_1us(520);
}
/******************************************************************************
End of function
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_LcdWrite
Description     : LCD 8bit data set
Arguments       : uint8_t c
Return value    : none
******************************************************************************/
void usb_cpu_LcdWrite(uint8_t c)
{
    usb_cpu_LcdNibbleWrite((c >> 4) & 0x0f);
    usb_cpu_LcdNibbleWrite(c & 0x0f);
}
/******************************************************************************
End of function
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
    int16_t i;

    if(pos < LCD_LINE2)
    {
        usb_cpu_LcdWrite(LCD_HOME_L1 + pos);                /* Display on Line 1 */
    }
    else if(pos < LCD_LINE3)
    {
        usb_cpu_LcdWrite(LCD_HOME_L2 + pos - LCD_LINE2);    /* Display on Line 2 */
    }
    else
    {
        while(1);
    }

    LCD_RS_PORT_DATA = PORT_HIGH;
    for(i = 0; (i < 8) && (*str != 0); ++i)
    {
        usb_cpu_LcdWrite(*str++);
    }
    LCD_RS_PORT_DATA = PORT_LOW;
}
/******************************************************************************
End of function
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_LcdContDisp
Description     : LCD display numeric data
Arguments       : uint8_t pos           : Position
                : uint8_t data          : Write data (string data)
Return value    : none
******************************************************************************/
void usb_cpu_LcdContDisp(uint8_t pos, uint8_t data)
{
    uint8_t buf[3];

    buf[0] = (data / 10) | 0x30;
    buf[1] = (data % 10) | 0x30;
    buf[2] = 0x00;
#ifdef USB_LCD_ENABLE
    usb_cpu_LcdDisp(pos, buf);
#endif
}
/******************************************************************************
End of function
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_LcdRSClear
Description     :
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_LcdRSClear(void)
{
    LCD_RS_PORT_DATA = PORT_LOW;
}
/******************************************************************************
End of function
******************************************************************************/


/******************************************************************************
Function Name   : usb_cpu_LcdResetFactor
Description     :
Arguments       : none
Return value    : none
******************************************************************************/
void usb_cpu_LcdResetFactor(void)
{
    uint32_t    resetflag;

    resetflag = (uint32_t)((SYSTEM.RSTSR0.BYTE << 16) | (SYSTEM.RSTSR1.BYTE << 8) | (SYSTEM.RSTSR2.BYTE));

    usb_cpu_LcdDisp(0, (uint8_t *)"        ");
    if(resetflag & 0x00080000)
    {
        usb_cpu_LcdDisp(0, (uint8_t *)"\\");
        usb_cpu_LcdDisp(8, (uint8_t *)"LVD2RF");
    }
    if(resetflag & 0x00040000)
    {
        usb_cpu_LcdDisp(1, (uint8_t *)"#");
        usb_cpu_LcdDisp(8, (uint8_t *)"LVD1RF");
    }
    if(resetflag & 0x00010000)
    {
        usb_cpu_LcdDisp(2, (uint8_t *)"$");
        usb_cpu_LcdDisp(8, (uint8_t *)"PORF");
    }
    if(resetflag & 0x00000100)
    {
        usb_cpu_LcdDisp(3, (uint8_t *)"%");
        usb_cpu_LcdDisp(8, (uint8_t *)"CWSF");
    }
    if(resetflag & 0x00000004)
    {
        usb_cpu_LcdDisp(4, (uint8_t *)"&");
        usb_cpu_LcdDisp(8, (uint8_t *)"SWRF");
    }
    if(resetflag & 0x00000001)
    {
        usb_cpu_LcdDisp(5, (uint8_t *)"'");
        usb_cpu_LcdDisp(8, (uint8_t *)"IWDTRF");
    }
    if(resetflag)
    {
        while(1);
    }
}
/******************************************************************************
End of function
******************************************************************************/

#endif /* USB_LCD_ENABLE */

/******************************************************************************
End Of File
******************************************************************************/

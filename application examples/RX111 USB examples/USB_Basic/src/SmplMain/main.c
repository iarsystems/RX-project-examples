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
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name    : main.c
* Version      : 2.01
* Device(s)    : Renesas
* Tool-Chain   : Renesas
* OS           : Not use
* H/W Platform : Independent
* Description  : USB Host/Peripheral main loop code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 16.02.2011 1.00    Mass Production Release
*           : 01.03.2011 1.01    Version 1.01 Release(Bug fixed)
*           : 31.03.2012 1.80    Basic FW mini Ver2.00(ES Version)
*           : 30.11.2012 2.00    Basic FW mini Ver2.00 Release
*           : 31.01.2013 2.01    Corresponded to RL78/L1C
*******************************************************************************/

/******************************************************************************
Section definitions
******************************************************************************/
#ifdef  R8CUSB
#pragma section program P_smpl
#pragma section bss B_smpl
#pragma section data R_smpl
#pragma section rom C_smpl
#endif
#ifdef  RL78USB
#endif
#ifdef  RL78USB_SECTION
#pragma section @@CODE   P_SMP
#pragma section @@BASE   T_SMP
#pragma section @@CNST   C_SMP
#pragma section @@R_INIT D_SMP
#pragma section @@DATA   B_SMP
#pragma section @@INIT   R_SMP
#endif


/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_usbip.h"                /* Definition of the USB register & bit define */
#include "r_usb_regaccess.h"            /* Definition of the USB register access macro */
#include "r_usb_api.h"                  /* USB API public function header */
#include "r_usb_vendor_api.h"
#include "hw_resource.h"
#include "lcd.h"


/******************************************************************************
Macro definitions
******************************************************************************/
#define USB_TBL_MAX                 10

/* Sample data transfer size */
#define USB_SMPL_TRNSIZE4           64      /* PIPE4 */
#define USB_SMPL_TRNSIZE5           64      /* PIPE5 */
#define USB_SMPL_TRNSIZE6           64      /* PIPE6 */
#define USB_SMPL_TRNSIZE7           64      /* PIPE7 */
/* Sample data transfer count */
#define USB_SMPL_TRNCNT4            55      /* PIPE4 */
#define USB_SMPL_TRNCNT5            99      /* PIPE5 */
#define USB_SMPL_TRNCNT6            88      /* PIPE6 */
#define USB_SMPL_TRNCNT7            66      /* PIPE7 */


/******************************************************************************
Typedef definitions
******************************************************************************/
/* There is no typedef definition. */


/******************************************************************************
Private global variables and functions
******************************************************************************/
/* variables */
USB_STATIC volatile uint8_t g_usb_suspend_flag;
USB_STATIC volatile uint8_t g_usb_int_key_flag;

/* Sample transfer data area */
USB_STATIC uint8_t      g_usb_SmplTrnData4[USB_SMPL_TRNSIZE4 + (4 - (USB_SMPL_TRNSIZE4 % 4))];
USB_STATIC uint8_t      g_usb_SmplTrnData5[USB_SMPL_TRNSIZE5 + (4 - (USB_SMPL_TRNSIZE5 % 4))];
USB_STATIC uint8_t      g_usb_SmplTrnData6[USB_SMPL_TRNSIZE6 + (4 - (USB_SMPL_TRNSIZE6 % 4))];
USB_STATIC uint8_t      g_usb_SmplTrnData7[USB_SMPL_TRNSIZE7 + (4 - (USB_SMPL_TRNSIZE7 % 4))];
USB_STATIC uint16_t     g_usb_LcdTrnCnt[USB_TBL_MAX];

/* functions */
USB_STATIC void         usb_smpl_key_disp(uint8_t keydata);


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */
/* Common global area definition */
usb_utr_t   g_usb_SmplTrnMsg[USB_TBL_MAX];
usb_leng_t  g_usb_SmplTrnSize[USB_TBL_MAX];
uint16_t    g_usb_SmplTrnCnt[USB_TBL_MAX];
uint8_t     *g_usb_SmplTrnPtr[USB_TBL_MAX];

/* functions */
void        main(void);
void        usb_smpl_data_init(void);
void        usb_smpl_counter_init(void);
void        usb_smpl_lcd_pipe_count(usb_pipe_t pipe);
void        usb_smpl_lcd_numeric(uint8_t pos, uint8_t data);
void        usb_smpl_lcd_string(uint8_t pos, uint8_t *str);
void        usb_smpl_led_position(uint8_t ledpos, uint8_t data);
uint8_t     usb_smpl_key_input(void);
void        usb_smpl_set_suspend_flag(uint8_t data);
uint8_t     usb_smpl_get_suspend_flag(void);
void        usb_smpl_set_intkey(uint8_t data);
uint8_t     usb_smpl_get_intkey(void);


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */

/* functions */
#if USB_FUNCSEL_PP == USB_PERI_PP
extern void     usb_psmpl_main_init(void);
extern void     usb_psmpl_apl_task(void);
extern void     usb_psmpl_keyprocess(uint8_t keydata);
extern void     usb_cpu_stop_mode(void);
extern void     usb_cpu_intp0_enable(void);
extern void     usb_cpu_intp0_disable(void);
#endif /* USBC_PERI_PP */
#if USB_FUNCSEL_PP == USB_HOST_PP
extern void     usb_hsmpl_main_init(void);
extern void     usb_hsmpl_apl_task(void);
extern void     usb_hsmpl_keyprocess(uint8_t keydata);
#endif /* USBC_HOST_PP */

/* Special function drivers */
#ifdef  USB_LCD_ENABLE
extern void     usb_cpu_LcdContDisp(uint8_t pos, uint8_t data);
extern void     usb_cpu_LcdDisp(uint8_t pos, uint8_t *str);
extern void     usb_cpu_LcdResetFactor(void);
#endif /* USB_LCD_ENABLE */

#ifdef USB_LED_ENABLE
extern void     usb_cpu_led_set_bit(uint8_t ledpos, uint8_t data);
#endif /* USB_LED_ENABLE */

#ifdef USB_KEY_ENABLE
extern uint8_t  usb_cpu_key_read(void);
#endif
#ifdef USB_ADC_ENABLE
extern uint8_t  usb_cpu_key_read(void);
#endif

/******************************************************************************
Renesas Host Sample Vendor Driver functions
******************************************************************************/

/******************************************************************************
Function Name   : main
Description     : Main Task
Arguments       : none
Return value    : none
******************************************************************************/
void main(void)
{
#if USB_FUNCSEL_PP == USB_PERI_PP
    uint8_t keydata;

    /* Initialized USBIP */
    usb_psmpl_main_init();
#ifdef  USB_LSPERI_PP
    usb_smpl_lcd_string(0, (uint8_t *)"LS-PERI ");
#endif  /* USB_LSPERI_PP */
#ifdef  USB_FSPERI_PP
    usb_smpl_lcd_string(0, (uint8_t *)"FS-PERI ");
#endif  /* USB_FSPERI_PP */
    usb_smpl_led_position(LED_POS3,LED_ON);

    /* Sample main loop  */
    while( 1 )
    {
        if( R_usb_cstd_Scheduler() == USB_FLGSET )
        {
            usb_smpl_led_position(LED_POS2,LED_ON);
            R_usb_pstd_PcdTask();       /* PCD Task */
            R_usb_pvndr_Task();
            usb_psmpl_apl_task();
        }
        usb_smpl_led_position(LED_POS2,LED_OFF);
        keydata = usb_smpl_key_input();
        if (keydata != 0x00)
        {
            usb_psmpl_keyprocess(keydata);
            usb_smpl_key_disp(keydata);
        }

#ifdef USB_STOP_MODE_ENABLE
        if ( g_usb_suspend_flag == USB_YES )
        {
#ifdef USB_KEY_ENABLE
            usb_cpu_stop_mode();
#endif
        }
#endif

    }
#endif /* USBC_PERI_PP */

#if USB_FUNCSEL_PP == USB_HOST_PP
    uint8_t keydata;

    /* Initialized USBIP */
    usb_hsmpl_main_init();
#if USB_PORTSEL_PP == USB_1PORT_PP
    usb_smpl_lcd_string(0, (uint8_t *)"HOST-1P  ");
#endif  /* USB_PORTSEL_PP == USB_1PORT_PP */
#if USB_PORTSEL_PP == USB_2PORT_PP
    usb_smpl_lcd_string(0, (uint8_t *)"HOST-2P ");
#endif  /* USB_PORTSEL_PP == USB_2PORT_PP */
    usb_smpl_led_position(LED_POS3,LED_ON);

    /* Sample main loop  */
    while( 1 )
    {
        if( R_usb_cstd_Scheduler() == USB_FLGSET )
        {
            usb_smpl_led_position(LED_POS2,LED_ON);
            R_usb_hstd_HcdTask();       /* HCD Task */
            R_usb_hstd_MgrTask();       /* MGR Task */
            R_usb_hvndr_Task();
            usb_hsmpl_apl_task();
        }
        usb_smpl_led_position(LED_POS2,LED_OFF);

        keydata = usb_smpl_key_input();
        if (keydata != 0x00)
        {
            usb_hsmpl_keyprocess(keydata);
            usb_smpl_key_disp(keydata);
        }
    }
#endif /* USBC_HOST_PP */
}   /* eof main() */

/******************************************************************************
Function Name   : usb_smpl_key_disp
Description     : Displayed key number
Arguments       : keydata
Return value    : none
******************************************************************************/
USB_STATIC void usb_smpl_key_disp(uint8_t keydata)
{
    switch(keydata)
    {
        case 1:
            usb_smpl_lcd_string(6, (uint8_t *)" 1");
        break;
        case 2:
            usb_smpl_lcd_string(6, (uint8_t *)" 2");
        break;
        case 3:
            usb_smpl_lcd_string(6, (uint8_t *)" 3");
        break;
        case 4:
            usb_smpl_lcd_string(6, (uint8_t *)" 4");
        break;
        default:
            usb_smpl_lcd_string(6, (uint8_t *)"*");
        break;
    }
}   /* eof usb_smpl_key_disp() */

/******************************************************************************
Function Name   : usb_smpl_data_init
Description     : Initialized transfer area and transfer counter
Arguments       : none
Return value    : none
******************************************************************************/
void usb_smpl_data_init(void)
{
    uint16_t    j;

    /* Sample data set PIPE4 */
    for( j = 0; j < USB_SMPL_TRNSIZE4; j++ )
    {
        g_usb_SmplTrnData4[j] = (uint8_t)j;
    }
    /* Sample data set PIPE5 */
    for( j = 0; j < USB_SMPL_TRNSIZE5; ++j )
    {
        g_usb_SmplTrnData5[j] = (uint8_t)j;
    }
    /* Sample data set PIPE6 */
    for( j = 0; j < USB_SMPL_TRNSIZE6; j++ )
    {
        g_usb_SmplTrnData6[j] = (uint8_t)j;
    }
    /* Sample data set PIPE7 */
    for( j = 0; j < USB_SMPL_TRNSIZE7; ++j )
    {
        g_usb_SmplTrnData7[j] = (uint8_t)j;
    }

    /* Sample data area address set */
    g_usb_SmplTrnPtr[USB_PIPE4]     = g_usb_SmplTrnData4;
    g_usb_SmplTrnPtr[USB_PIPE5]     = g_usb_SmplTrnData5;
    g_usb_SmplTrnPtr[USB_PIPE6]     = g_usb_SmplTrnData6;
    g_usb_SmplTrnPtr[USB_PIPE7]     = g_usb_SmplTrnData7;

    /* Sample data size Initialized */
    g_usb_SmplTrnSize[USB_PIPE4]    = USB_SMPL_TRNSIZE4;
    g_usb_SmplTrnSize[USB_PIPE5]    = USB_SMPL_TRNSIZE5;
    g_usb_SmplTrnSize[USB_PIPE6]    = USB_SMPL_TRNSIZE6;
    g_usb_SmplTrnSize[USB_PIPE7]    = USB_SMPL_TRNSIZE7;

    usb_smpl_counter_init();
}   /* eof usb_smpl_data_init() */

/******************************************************************************
Function Name   : usb_smpl_counter_init
Description     : Initialized transfer counter
Arguments       : none
Return value    : none
******************************************************************************/
void usb_smpl_counter_init(void)
{
    /* Sample data counter Initialized */
#if USB_FUNCSEL_PP == USB_PERI_PP
#ifndef  USB_LSPERI_PP
    g_usb_SmplTrnCnt[USB_PIPE4]     = USB_SMPL_TRNCNT4;
    g_usb_SmplTrnCnt[USB_PIPE5]     = USB_SMPL_TRNCNT5;
#endif  /* USB_LSPERI_PP */
    g_usb_SmplTrnCnt[USB_PIPE6]     = USB_SMPL_TRNCNT6;
    g_usb_SmplTrnCnt[USB_PIPE7]     = USB_SMPL_TRNCNT7;
#endif  /* USB_PERI_PP */
#if USB_FUNCSEL_PP == USB_HOST_PP
    g_usb_SmplTrnCnt[USB_PIPE4]     = USB_0;
    g_usb_SmplTrnCnt[USB_PIPE5]     = USB_0;
    if (R_usb_hvndr_UsePipe(USB_PIPE4) == USB_YES)
    {
        g_usb_SmplTrnCnt[USB_PIPE4]     = USB_SMPL_TRNCNT4;
    }
    if (R_usb_hvndr_UsePipe(USB_PIPE5) == USB_YES)
    {
        g_usb_SmplTrnCnt[USB_PIPE5]     = USB_SMPL_TRNCNT5;
    }
    g_usb_SmplTrnCnt[USB_PIPE6]     = USB_SMPL_TRNCNT6;
    g_usb_SmplTrnCnt[USB_PIPE7]     = USB_SMPL_TRNCNT7;
#endif /* USBC_HOST_PP */
    g_usb_LcdTrnCnt[USB_PIPE4] = 0;
    g_usb_LcdTrnCnt[USB_PIPE5] = 0;
    g_usb_LcdTrnCnt[USB_PIPE6] = 0;
    g_usb_LcdTrnCnt[USB_PIPE7] = 0;
}   /* eof usb_smpl_counter_init() */


/******************************************************************************
Function Name   : usb_smpl_lcd_pipe_count
Description     : Updata dislay data
Arguments       : pipe number
Return value    : none
******************************************************************************/
void usb_smpl_lcd_pipe_count(usb_pipe_t pipe)
{
    uint8_t     pos, data;
    uint8_t buf[3];

    g_usb_LcdTrnCnt[pipe]++;
    data    = (uint8_t)(g_usb_LcdTrnCnt[pipe] % 100);
    pos = (pipe - USB_PIPE4) * 2 + 8;

    buf[0] = (data / 10) | 0x30;
    buf[1] = (data % 10) | 0x30;
    buf[2] = 0x00;
#ifdef  USB_LCD_ENABLE
    usb_cpu_LcdContDisp(pos, data);
#endif
#ifdef  USB_PMOD_LCD_ENABLE
    if (pos < LCD_POS_D0)
    {
        Display_LCD(0, buf);
    }
    else
    {
        Display_LCD(1, buf);
    }
#endif
}   /* eof usb_smpl_lcd_pipe_count() */


/******************************************************************************
Function Name   : usb_smpl_lcd_numeric
Description     : Displayed pipe counter
Arguments       : pos, data
Return value    : none
******************************************************************************/
void usb_smpl_lcd_numeric(uint8_t pos, uint8_t data)
{
#ifdef  USB_LCD_ENABLE
    usb_cpu_LcdContDisp(pos, data);
#endif
#ifdef  USB_PMOD_LCD_ENABLE
    uint8_t buf[3];

    buf[0] = (data / 10) | 0x30;
    buf[1] = (data % 10) | 0x30;
    buf[2] = 0x00;

    if (pos < LCD_POS_D0)
    {
        Display_LCD(0, buf);
    }
    else
    {
        Display_LCD(1, buf);
    }
#endif
}   /* eof usb_smpl_lcd_numeric() */


/******************************************************************************
Function Name   : usb_smpl_lcd_string
Description     : Displayed string data
Arguments       : uint8_t pos           : Position
                : uint8_t *str          : Write data (string data)
Return value    : none
******************************************************************************/
void usb_smpl_lcd_string(uint8_t pos, uint8_t *str)
{
#ifdef  USB_LCD_ENABLE
    usb_cpu_LcdDisp(pos, str);
#endif
#ifdef  USB_PMOD_LCD_ENABLE
    if (pos < LCD_POS_D0)
    {
        Display_LCD(0, str);
    }
    else
    {
        Display_LCD(1, str);
    }
#endif
}   /* eof usb_smpl_lcd_string() */


/******************************************************************************
Function Name   : usb_smpl_led_position
Description     : Set LED ON/OFF
Arguments       : uint8_t ledpos        : Led Set Position
                : uint8_t data          : Led On/Off
Return value    : none
******************************************************************************/
void usb_smpl_led_position(uint8_t ledpos, uint8_t data)
{
#ifdef USB_LED_ENABLE
    usb_cpu_led_set_bit(ledpos, data);
#endif
}   /* eof usb_smpl_led_position() */


/******************************************************************************
Function Name   : usb_smpl_key_input
Description     : Read key code
Arguments       : none
Return value    : key code
******************************************************************************/
uint8_t usb_smpl_key_input(void)
{
#ifdef USB_KEY_ENABLE
    return  usb_cpu_key_read();
#endif
#ifdef USB_ADC_ENABLE
    return  usb_cpu_key_read();
#endif
    return  NO_PUSH;                    /* KEY OFF */
}   /* eof usb_smpl_key_input() */


/******************************************************************************
Function Name   : usb_smpl_set_suspend_flag
Description     : Set resume interruptt flag
Arguments       : uint8_t data
Return value    : none
******************************************************************************/
void usb_smpl_set_suspend_flag(uint8_t data)
{
    g_usb_suspend_flag  = data;
}   /* eof usb_smpl_set_suspend_flag() */


/******************************************************************************
Function Name   : usb_smpl_get_suspend_flag
Description     : Get resume interruptt flag
Arguments       : none
Return value    : g_usb_suspend_flag
******************************************************************************/
uint8_t usb_smpl_get_suspend_flag(void)
{
    return  g_usb_suspend_flag;
}   /* eof usb_smpl_get_suspend_flag() */


/******************************************************************************
Function Name   : usb_smpl_set_intkey
Description     : Set external key interruptt flag
Arguments       : uint8_t data
Return value    : none
******************************************************************************/
void usb_smpl_set_intkey(uint8_t data)
{
    g_usb_int_key_flag  = data;
}   /* eof usb_smpl_set_intkey() */


/******************************************************************************
Function Name   : usb_smpl_get_intkey
Description     : Get external key interruptt flag
Arguments       : none
Return value    : g_usb_int_key_flag
******************************************************************************/
uint8_t usb_smpl_get_intkey(void)
{
    return  g_usb_int_key_flag;
}   /* eof usb_smpl_get_intkey() */


/******************************************************************************
End of file
******************************************************************************/

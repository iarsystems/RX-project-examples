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
* File Name     : main.c
* Version       : 2.10
* Device(s)     : Renesas
* Tool-Chain    : Renesas
* OS            : Not use
* H/W Platform  : Independent
* Description   : USB Peripheral HID class driver code.
*******************************************************************************/
/*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 30.11.2012 2.00    First Release
*           : 31.05.2013 2.10    Corresponded to RL78/L1C, RX111
*******************************************************************************/


/******************************************************************************
Section definitions
******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_usbip.h"                /* Definition of the USB register & bit define */
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_api.h"                  /* USB API public function header */
#include "hw_resource.h"
#include "r_usb_phid_usrcfg.h"
#include "r_usb_phid_define.h"
#include "r_usb_phid_api.h"
#include "lcd.h"


/******************************************************************************
Macro definitions
******************************************************************************/
/* There is no macro definition. */


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

/* functions */


/******************************************************************************
Exported global variables and functions (to be accessed by other files)
******************************************************************************/
/* variables */

/* functions */
void        main(void);
void        usb_smpl_lcd_string(uint8_t pos, uint8_t *str);
void        usb_smpl_led_position(uint8_t ledpos, uint8_t data);
uint8_t     usb_smpl_key_input(void);
uint16_t    usb_smpl_ad_read(void);
void        usb_smpl_set_suspend_flag(uint8_t data);
uint8_t     usb_smpl_get_suspend_flag(void);
void        usb_smpl_set_intkey(uint8_t data);
uint8_t     usb_smpl_get_intkey(void);


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* variables */

/* functions */
extern void     usb_psmpl_main_init(void);
extern void     usb_phid_main_task(void);
extern void     R_usb_phid_task(void);
extern void     usb_cpu_stop_mode( void );

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
extern uint8_t  usb_cpu_sw_data(void);
#endif
#ifdef USB_ADC_ENABLE
extern uint16_t  usb_cpu_adc_read(void);
#endif


/******************************************************************************
Renesas PHID Sample
******************************************************************************/

/******************************************************************************
Function Name   : main
Description     : Main Task
Arguments       : none
Return value    : none
******************************************************************************/
void main(void)
{
    usb_er_t ret_wakeup;

    /* Initialized USBIP */
    g_usb_suspend_flag = USB_NO;
    g_usb_int_key_flag = USB_NO;
    usb_psmpl_main_init();
#if USB_PHID_APL_MODE == USB_PHID_KEYBOARD_MODE
    usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"USB KBD " );
#endif  /* Peripheral HID Application keyboard mode */
#if USB_PHID_APL_MODE == USB_PHID_MOUSE_MODE
    usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"USB MSE " );
#endif  /* Peripheral HID Application mouse mode */
#if USB_PHID_APL_MODE == USB_PHID_DEMO_MODE
    usb_smpl_lcd_string(LCD_POS_U0, (uint8_t *)"USB DEMO" );
#endif  /* Peripheral HID Application demo mode */
    usb_smpl_lcd_string(LCD_POS_D0, (uint8_t *)"Appstart" );

    /* Sample main loop  */
    while(1)
    {
        if( USB_FLGSET == R_usb_cstd_Scheduler() )
        {
            R_usb_pstd_PcdTask();       /* PCD Task */
            R_usb_phid_task();          /* Peripheral HID Task */
            usb_phid_main_task();       /* Peripheral HID demo sample application Task */
        }
        if ( g_usb_suspend_flag == USB_YES )
        {
            usb_cpu_stop_mode();

            if ( g_usb_int_key_flag == USB_YES )
            {
                g_usb_int_key_flag = USB_NO;
//@@@               g_usb_suspend_flag = USB_NO;

                ret_wakeup = R_usb_pstd_PcdChangeDeviceState( (usb_strct_t)USB_DO_REMOTEWAKEUP );
                if ( ret_wakeup != USB_E_OK )
                {
                    USB_DEBUG_HOOK(USB_DEBUG_HOOK_APL | USB_DEBUG_HOOK_CODE1);
                }
                else
                {
                    g_usb_suspend_flag = USB_NO;
                }
            }
        }
    }
}   /* eof main() */


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
    return  usb_cpu_sw_data();
#else
    return  NO_PUSH;                    /* KEY OFF */
#endif
}   /* eof usb_smpl_key_input() */


/******************************************************************************
Function Name   : usb_smpl_ad_read
Description     : Read ADPOT code
Arguments       : none
Return value    : key code
******************************************************************************/
uint16_t usb_smpl_ad_read(void)
{
#ifdef USB_ADC_ENABLE
    return  usb_cpu_adc_read();
#else
    return  NO_PUSH;                    /* KEY OFF */
#endif
}   /* eof usb_smpl_ad_read() */


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
End  Of File
******************************************************************************/

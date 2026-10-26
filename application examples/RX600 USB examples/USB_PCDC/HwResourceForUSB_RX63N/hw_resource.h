/******************************************************************************
* File Name    : hw_resource.h
* Version      : 1.00
* Description  : RX63N MCU/RSK Board define
*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 31.12.2012 1.00    Basic FW Ver2.00
******************************************************************************/
#ifndef __HW_RESOURCE_H__
#define __HW_RESOURCE_H__

/******************************************************************************
Constant macro definitions
******************************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    RSK USE IO SELECT
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
#define     USB_LCD_ENABLE
#define     USB_LED_ENABLE
#define     USB_KEY_ENABLE
#define     USB_UART_ENABLE
//#define     USB_ADC_ENABLE

/******************************************************************************
Macro definitions
******************************************************************************/
/* LCD DEFINE */
#define     LCD_MAXCHARA    8
#define     LCD_POS_U0      0
#define     LCD_POS_U1      1
#define     LCD_POS_U2      2
#define     LCD_POS_U3      3
#define     LCD_POS_U4      4
#define     LCD_POS_U5      5
#define     LCD_POS_U6      6
#define     LCD_POS_U7      7
#define     LCD_POS_D0      LCD_POS_U0+LCD_MAXCHARA
#define     LCD_POS_D1      LCD_POS_U1+LCD_MAXCHARA
#define     LCD_POS_D2      LCD_POS_U2+LCD_MAXCHARA
#define     LCD_POS_D3      LCD_POS_U3+LCD_MAXCHARA
#define     LCD_POS_D4      LCD_POS_U4+LCD_MAXCHARA
#define     LCD_POS_D5      LCD_POS_U5+LCD_MAXCHARA
#define     LCD_POS_D6      LCD_POS_U6+LCD_MAXCHARA
#define     LCD_POS_D7      LCD_POS_U7+LCD_MAXCHARA

#endif  /* __HW_RESOURCE_H__ */
/******************************************************************************
End Of File
******************************************************************************/

/******************************************************************************
* File Name     : hw_resource.h
* Version       : 2.00
* Description   : RX111 MCU/TargetBoard define
*******************************************************************************
* History   : DD.MM.YYYY Version Description
*           : 31.03.2012 1.80    Basic FW mini Ver2.00(ES Version)
*           : 30.11.2012 2.00
*           : 08.05.2013         For LibUSB.
******************************************************************************/
#ifndef __HW_RESOURCE_H__
#define __HW_RESOURCE_H__


/******************************************************************************
Constant macro definitions
******************************************************************************/
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Select the driver by the peripheral function integrated into the MCU.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
//#define       USB_LCD_ENABLE  1
#define     USB_ADC_ENABLE  1
#define     USB_LED_ENABLE  1
#define     USB_PMOD_LCD_ENABLE 1
/* Only SW1 interrupt for original LibUSB Demo. */
#define     USB_KEY_ENABLE  1
//#define   USB_STOP_MODE_ENABLE    1


/******************************************************************************
Imported global variables and functions (from other files)
******************************************************************************/
/* Variables */

/* MCU & USB functions */
extern void     usb_cpu_target_init(void);          /* Target initialize */
extern void     usb_cpu_int_enable(void);           /* Enable USB interrupt */
extern void     usb_cpu_int_disable(void);          /* Disable USB interrupt */
extern void     usb_cpu_delay_xms(uint16_t time);   /* Delay */
extern void     usb_cpu_delay_1us(uint16_t time);
extern void     usb_cpu_irq4_interrupt_enable(void);
extern void     usb_cpu_irq4_interrupt_disable(void);
extern void     usb_cpu_LcdDisp(uint8_t pos, uint8_t *str);

/******************************************************************************
Macro definitions
******************************************************************************/
#define     PORT_LOW        0
#define     PORT_HIGH       1
#define     PORT_OUT_MODE   1
#define     PORT_IN_MODE    0
/************* 48pin Setting *************/
#define     PIOR_VALUE      0x00        /* PIOR0 is Don't care */
#define     ADPC_VALUE      0x04        /* P20-P22is ad input */


/* LCD DEFINE */
#define     LCD_MAXCHARA    20
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


/* ADC DEFINE */


/* LED DEFINE */
#define     LED_OFF         0
#define     LED_ON          1
#define     LED_POS0        1
#define     LED_POS1        2
#define     LED_POS2        4
#define     LED_POS3        8


/* KEY DEFINE */
#define NO_PUSH             0x00
#define SW1_PUSH            0x01
#define SW2_PUSH            0x02

#endif  /* __HW_RESOURCE_H__ */
/******************************************************************************
End Of File
******************************************************************************/

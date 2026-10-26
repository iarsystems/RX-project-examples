/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_main.c
* Version      : 2.00
* Device(s)    : R5F5113xAxFP,R5F5231xAxFP,R5F51305AxFN
* Tool-Chain   : CC-RX Compiler ( V2.03.00 )
* OS           : -
* H/W Platform : -
* Description  : This file is system main.
* Operation    : Compile and download the sample code. Click 'Reset Go' to start the software.
* Limitations  : -
***********************************************************************************************************************/

/***********************************************************************************************************************
* History      : DD.MM.YYYY Version    Description
*              : xx.xx.2014   1.00     First Release
*              : 05.02.2015   2.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
/* System include header */
#include "r_cg_macrodriver.h"

/* H/W include header */
#include "iodefine.h"
#include "r_mpc.h"
#include "r_ctsu.h"
#include "r_cgc.h"
#include "r_dtc.h"

#ifdef  SCI_SERIAL_USE
    #include "r_sci.h"
#endif  /* SCI_SERIAL_USE */

/* USB H/W include header */
#include "r_usb_ctypedef.h"             /* Type define */
#include "r_usb_ckernelid.h"            /* Kernel ID definition */
#include "r_usb_usrconfig.h"            /* System definition */
#include "r_usb_cdefusbip.h"            /* USB-FW Library Header */
#include "r_usb_usrconfig.h"            /* System definition, USB-FW Library Header */
#include "r_usb_api.h"                  /* USB API public function header */
#include "hw_resource.h"
#include "r_cg_cmt.h"

#include "r_usb_pcdc_driver.h"
#include "r_usb_pcdc_define.h"
#include "r_usb_pcdc_api.h"

/* USB S/W include header */
#include "r_cg_userdefine.h"
#include "r_touch.h"
#include "r_touch_API.h"

#include "r_serial_control.h"
#include "r_okaya_lcd.h"
#include "rskrx130def.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
#define DF_TOUCH_NOKEY      (0)
#define DF_TOUCH_KEYON      (1)
#define TOUCH_KEY1_MASK     (0x0800)                     /* Touch key1 mask data */
#define TOUCH_KEY2_MASK     (0x1000)                     /* Touch key2 mask data */

/* slioder define */
#define SLIDER_NOTOUCH      (0xFFFF)                     /* slider no touch */
#define SLIDER_MAX_VALUE    (400)                        /* slider pos max value */
#define SLIDER_STEP_LCD     (4)                          /* slider step for LCD */
#define SLIDER_STEP_LED     (8)                          /* slider step for LED */
#define SLIDER_RESOL_LED    (SLIDER_MAX_VALUE/(SLIDER_STEP_LED-1))    /* slider resolution for LED */

#define DF_KEY1             (0)
#define DF_KEY2             (1)
#define MAX_KEY             (2)

/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/
static uint8_t key_data_moni[MAX_KEY] = { DF_TOUCH_NOKEY, DF_TOUCH_NOKEY }; /* key decode data monitor(for LCD) */
static uint8_t slider_data_moni = 0;                                        /* volume monitor(for LCD) */

static void lcd_initialize(void);
static void touch_key_check(void);
static void touch_slider_check(void);

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/
#ifdef  USB_SERIAL_USE
USB_STATIC volatile uint8_t g_usb_suspend_flag;
USB_STATIC volatile uint8_t g_usb_int_key_flag;
#endif  /* USB_SERIAL_USE */

/* functions */
void       main(void);
void       R_Set_Workbench_Communication_Create(void);
void       R_USBPCDC_Create(void);
void       R_Set_Workbench_Communication_Data(void);
void       R_USBPCDC_Apl_Task_Switch(void);
#ifdef  USB_SERIAL_USE
void       usb_smpl_set_suspend_flag(uint8_t data);
uint8_t    usb_smpl_get_suspend_flag(void);
void       usb_smpl_set_intkey(uint8_t data);
uint8_t    usb_smpl_get_intkey(void);
#endif  /* USB_SERIAL_USE */

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/
/* functions */
#ifdef  USB_SERIAL_USE
extern void     usb_cpu_target_init(void);
extern void     usb_pcdc_task_start(void);
extern void     usb_apl_task_switch(void);
#endif  /* USB_SERIAL_USE */

/***********************************************************************************************************************
* Function Name: main
* Description  : System main function
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
void main(void)
{
    uint8_t method;
    uint8_t ret_val;

    method = 0;

    ret_val = R_Set_Cap_Touch_Create( method );                    /* Capacitive touch create API call                */
    if (CREATE_SUCCESS == ret_val)                                 /* Create result check                             */
    {
        g_ctsu_soft_mode = CTSU_READY_MODE;                        /* CTSU masurement ready mode setting              */
    }
    else
    {
        g_ctsu_soft_mode = CTSU_STOP_MODE;                         /* CTSU masurement stop mode setting               */
    }

    R_Set_Workbench_Communication_Create();                        /* Workbench communication function create API call */

    R_CMT0_Start();                                                /* CMT0 start API call                             */

    SerialCommandInitial();                                        /* Initialize serial command communication task    */

    /* LCD Initial Display */
    lcd_initialize();

    while (1U)                                                     /* Main loop                                       */
    {
        R_Set_Workbench_Communication_Data();

        if (1 == g_touch_system.flag.timing)
        {
            g_touch_system.flag.timing = 0;
            R_Set_Cap_Touch_Measurement_Start( method );

            if (GET_OK == R_Get_Cap_Touch_Data_Check( method ))
            {
                if (INITIAL_END == R_Get_Cap_Touch_Initial_Status())
                {
                    if (CREATE_SUCCESS == R_Set_Cap_Touch_Result_Create( method ))
                    {
                        /* Touch feedback    Led etc...    */
                        touch_key_check();
                        touch_slider_check();

                    }
                }
                else
                {
                    R_Set_Cap_Touch_Initial_Tuning( method );
                }
            }
            method = R_Set_Cap_Touch_Next_Method_Change( method );

            PrepareReplayMessage(); /* Make the replay message of the serial command from Workbench */
        }
    }
}    /* End of function main() */

/***********************************************************************************************************************
* Function Name   : R_Set_Workbench_Communication_Create
* Description     : Workbench communication function create
* Arguments       : None
* Return value    : None
***********************************************************************************************************************/
void R_Set_Workbench_Communication_Create(void)
{
#if (MCU_SERIES_T == R5F5113 || MCU_SERIES_T == R5F5231)
    R_USBPCDC_Create();                                            /* USB CDC create API call                         */
#endif    /* (MCU_SERIES_T == R5F5113 || MCU_SERIES_T == R5F5231) */

#ifdef  SCI_SERIAL_USE
    DTC_Set_Initial_of_SCI6();
    DTC_Set_Start_of_SCI6();
    R_SCI6_Create();                                               /* SCI6 create API call                            */
    R_SCI6_Start();                                                /* SCI6 start API call                             */
#endif  /* SCI_SERIAL_USE */
}    /* End of function R_Set_Workbench_Communication_Create() */

/***********************************************************************************************************************
* Function Name   : R_USBPCDC_Create
* Description     : 
* Arguments       : None
* Return value    : None
***********************************************************************************************************************/
void R_USBPCDC_Create(void)
{
#ifdef  USB_SERIAL_USE
    SYSTEM.PRCR.WORD           = PRCR1_ENA;        /* Enables writing to the registers */
    SYSTEM.MSTPCRB.BIT.MSTPB19 = 0;                /* Module stop control register (Enable USB0 module(MSTPB19)) */
    SYSTEM.PRCR.WORD           = PRCR1_DIS;        /* Disables writing to the registers */
    usb_cpu_target_init();
    usb_smpl_set_suspend_flag(USB_NO);
    R_usb_pstd_PcdChangeDeviceState(USB_DO_INITHWFUNCTION);
    R_usb_pstd_PcdOpen();
    usb_pcdc_task_start();      /* Start Peripheral USB driver */
#endif  /* USB_SERIAL_USE */
}    /* End of function R_USBPCDC_Create() */

/***********************************************************************************************************************
* Function Name   : R_Set_Workbench_Communication_Data
* Description     : Workbench communication data setting
* Arguments       : None
* Return value    : None
***********************************************************************************************************************/
void R_Set_Workbench_Communication_Data(void)
{
#if (MCU_SERIES_T == R5F5113 || MCU_SERIES_T == R5F5231)
    R_USBPCDC_Apl_Task_Switch();                                   /* USB switch task for nonOS                       */
#endif    /* (MCU_SERIES_T == R5F5113 || MCU_SERIES_T == R5F5231) */

#ifdef  SCI_SERIAL_USE
    R_SCI6_Receive_Data_Check();
#endif  /* SCI_SERIAL_USE */
}    /* End of function R_Set_Workbench_Communication_Data() */

/***********************************************************************************************************************
* Function Name   : R_USBPCDC_Apl_Task_Switch
* Description     : 
* Arguments       : None
* Return value    : None
***********************************************************************************************************************/
void R_USBPCDC_Apl_Task_Switch(void)
{
#ifdef  USB_SERIAL_USE
    usb_apl_task_switch();        /* Switch task for nonOS */
#endif  /* USB_SERIAL_USE */
}   /* End of function R_USBPCDC_Apl_Task_Switch */

#ifdef  USB_SERIAL_USE
/***********************************************************************************************************************
* Function Name   : usb_smpl_set_suspend_flag
* Description     : Set resume interruptt flag
* Arguments       : uint8_t data
* Return value    : none
***********************************************************************************************************************/
void usb_smpl_set_suspend_flag(uint8_t data)
{
    g_usb_suspend_flag  = data;
}   /* End of function usb_smpl_set_suspend_flag() */

/***********************************************************************************************************************
* Function Name   : usb_smpl_get_suspend_flag
* Description     : Get resume interruptt flag
* Arguments       : none
* Return value    : g_usb_suspend_flag
***********************************************************************************************************************/
uint8_t usb_smpl_get_suspend_flag(void)
{
    return  (g_usb_suspend_flag);
}   /* End of function usb_smpl_get_suspend_flag() */

/***********************************************************************************************************************
* Function Name   : usb_smpl_set_intkey
* Description     : Set external key interruptt flag
* Arguments       : uint8_t data
* Return value    : none
***********************************************************************************************************************/
void usb_smpl_set_intkey(uint8_t data)
{
    g_usb_int_key_flag  = data;
}   /* End of function usb_smpl_set_intkey() */

/***********************************************************************************************************************
* Function Name   : usb_smpl_get_intkey
* Description     : Get external key interruptt flag
* Arguments       : none
* Return value    : g_usb_int_key_flag
***********************************************************************************************************************/
uint8_t usb_smpl_get_intkey(void)
{
//    return  (g_usb_int_key_flag);
}   /* End of function usb_smpl_get_intkey() */

#endif  /* USB_SERIAL_USE */

/***********************************************************************************************************************
* Function Name: lcd_initialize
* Description  : LCD Initialize
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void lcd_initialize(void)
{
    R_LCD_Init();                                           /* LCD initialize */
    R_LCD_Display(DF_LINE1, (uint8_t *)"TOUCH DEMO      "); /* LCD strings */
    R_LCD_Display(DF_LINE3, (uint8_t *)"KEY1: OFF       ");
    R_LCD_Display(DF_LINE4, (uint8_t *)"KEY2: OFF       ");
    R_LCD_Display(DF_LINE6, (uint8_t *)"SLIDER:");
    R_LCD_Display(DF_LINE7, (uint8_t *)"  -               +");
    R_LCD_SliderInit();
}   /* End of function lcd_initialize() */

/***********************************************************************************************************************
* Function Name: touch_key_check
* Description  : 
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void touch_key_check(void)
{
    touch_result_t  ts_result;
    uint8_t method = 0;
    uint8_t keydec_work[MAX_KEY] = { DF_TOUCH_NOKEY, DF_TOUCH_NOKEY};   /* key decode for work */

    /* KEY1 detect */
    ts_result = R_Get_Cap_Touch_Result( method );
    if( ts_result.button[0] & TOUCH_KEY1_MASK )
    {
        keydec_work[DF_KEY1] = DF_TOUCH_KEYON;
    }
    else
    {
        keydec_work[DF_KEY1] = DF_TOUCH_NOKEY;
    }
    
    if( keydec_work[DF_KEY1] != key_data_moni[DF_KEY1] )
    {
        key_data_moni[DF_KEY1] = keydec_work[DF_KEY1];

        if(DF_TOUCH_KEYON == key_data_moni[DF_KEY1])
        {
            R_LCD_Display(DF_LINE3, (uint8_t *)"KEY1: ON        ");
#ifndef TOUCH_LED_UNUSED
            LED0 = LED_ON;                /* LED0 ON  */
            LED2 = LED_ON;                /* LED2 ON  */
#endif
        }else{
            R_LCD_Display(DF_LINE3, (uint8_t *)"KEY1: OFF       ");
#ifndef TOUCH_LED_UNUSED
            LED0 = LED_OFF;                /* LED0 OFF */
            LED2 = LED_OFF;                /* LED2 OFF */
#endif
        }
    }

    /* KEY2 detect */
    if( ts_result.button[0] & TOUCH_KEY2_MASK )
    {
        keydec_work[DF_KEY2] = DF_TOUCH_KEYON;
    }
    else
    {
        keydec_work[DF_KEY2] = DF_TOUCH_NOKEY;
    }

    if( keydec_work[DF_KEY2] != key_data_moni[DF_KEY2] )
    {
        key_data_moni[DF_KEY2] = keydec_work[DF_KEY2];

        if(    DF_TOUCH_KEYON == key_data_moni[DF_KEY2] )
        {
            R_LCD_Display(DF_LINE4, (uint8_t *)"KEY2: ON        ");
#ifndef TOUCH_LED_UNUSED
            LED1 = LED_ON;                /* LED1 ON  */
            LED3 = LED_ON;                /* LED3 ON  */
#endif
        }else{
            R_LCD_Display(DF_LINE4, (uint8_t *)"KEY2: OFF       ");
#ifndef TOUCH_LED_UNUSED
            LED1 = LED_OFF;                /* LED1 OFF */
            LED3 = LED_OFF;                /* LED3 OFF */
#endif
        }
    }
    
}   /* End of function touch_key_check() */

/***********************************************************************************************************************
* Function Name: touch_slider_check
* Description  : 
* Arguments    : none
* Return Value : none
***********************************************************************************************************************/
static void touch_slider_check(void)
{
    touch_result_t ts_result;
    uint8_t     method = 0;
    uint16_t    slider_work;    /* slider work */
    uint8_t     cnt;
    uint8_t     slider_data;

#ifndef TOUCH_LED_UNUSED
    const uint8_t tbl_slider_led[SLIDER_STEP_LED]=
    {
    /*  ----  ---O  --OO  --O-  -OO-  -O--  OO--  O--- */
        0x00, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08,
    };
#endif
    
    /* Touch slider detect */
    ts_result = R_Get_Cap_Touch_Result( method );
    slider_work = ts_result.slider[0];
    
    if( SLIDER_NOTOUCH == slider_work )
    {
        slider_data = 0;
    }
    else
    {
        /* Convert resolution for Slider display (400step to 100step) */
        slider_data = (uint8_t)((slider_work/SLIDER_STEP_LCD)+1);
    }
    
    if( slider_data != slider_data_moni )
    {
        slider_data_moni = slider_data;
        R_LCD_SliderDisplay(slider_data);
    }
    
    /* On LED disp(resolution:8step) */
    cnt = 0;
    if( SLIDER_NOTOUCH != slider_work )
    {
        cnt = (uint8_t)(slider_work/SLIDER_RESOL_LED);
        if( SLIDER_STEP_LED<cnt ){    cnt = SLIDER_STEP_LED-1;    }/* up limit check */
        cnt = cnt ^ 0x07;
    }
#ifndef TOUCH_LED_UNUSED
    if( (DF_TOUCH_NOKEY == key_data_moni[DF_KEY1]) && (DF_TOUCH_NOKEY == key_data_moni[DF_KEY2]) )
    {
        LED0 = (tbl_slider_led[cnt] & LED_POS0) ? LED_ON : LED_OFF;
        LED1 = (tbl_slider_led[cnt] & LED_POS1) ? LED_ON : LED_OFF;
        LED2 = (tbl_slider_led[cnt] & LED_POS2) ? LED_ON : LED_OFF;
        LED3 = (tbl_slider_led[cnt] & LED_POS3) ? LED_ON : LED_OFF;
    }
#endif
    
}   /* End of function touch_slider_check() */

/***********************************************************************************************************************
* End of file
***********************************************************************************************************************/
